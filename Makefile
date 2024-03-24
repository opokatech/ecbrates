BUILD_DIR=build
RELEASE_SUFFIX=_release
DEBUG_SUFFIX=_debug

CPU?=native
TESTS?=OFF

MAKEFLAGS+= --no-print-directory

.PHONY:help
help:
	@echo "Targets:"
	@echo ""
	@echo " release    : compile with optimizations"
	@echo " debug      : compile without optimizations"
	@echo " tests      : as debug but with tests"
	@echo ""
	@echo "Options:"
	@echo " CPU    : TODO - work in progress."
	@echo "          Set this for crosscompile for armhf (only). If set it implies STATIC and NO TESTS"
	@echo "          Needed debian packagees: g++-arm-linux-gnueabihf, libssl-dev:armhf, qemu-user"
	@echo "          Examples:"
	@echo "          RaspberryPi3 - cortex-a53"
	@echo "          BananPiPro   - cortex-a7"
	@echo " TESTS  : ON|OFF build google tests (current value: ${TESTS})"

_build:
	@if [ "x${CPU}" != "xnative" ]; \
	then \
		cmake -S . -B ${BUILD_DIR} -G Ninja \
			  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm.cmake \
			  -DCMAKE_CROSSCOMPILING_EMULATOR=qemu-arm \
			  -DECB_PARAM_CPU=${CPU} \
			  -DECB_PARAM_DEBUG=OFF \
			  -DECB_PARAM_TESTS=OFF; \
		cmake --build ${BUILD_DIR}; \
	else \
		cmake -S . -B ${BUILD_DIR} -G Ninja \
			  -DECB_PARAM_DEBUG=${DEBUG} \
			  -DECB_PARAM_TESTS=${TESTS}; \
		cmake --build ${BUILD_DIR}; \
		rm -f compile_commands.json; \
	    ln -s ${BUILD_DIR}/compile_commands.json; \
	fi

clean:
	rm -rf ${BUILD_DIR}* compile_commands.json

clang-format:
	clang-format -i src/*pp tests/*pp

release:
	${MAKE} BUILD_DIR=${BUILD_DIR}_release_cpu_${CPU} DEBUG=OFF _build
	cmake --build ${BUILD_DIR}_release_cpu_${CPU} --target strip

debug:
	${MAKE} BUILD_DIR=${BUILD_DIR}_debug_cpu_${CPU} DEBUG=ON _build
	cmake --build ${BUILD_DIR}_debug_cpu_${CPU}

tests:
	TESTS=ON ${MAKE} debug
	./build_debug_cpu_native/bin/ecb_tests

.PHONY: _build clean clang-format release debug tests
