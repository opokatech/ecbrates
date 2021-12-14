BUILD_DIR=build
RELEASE_SUFFIX=_release
DEBUG_SUFFIX=_debug

ARMCPU ?= none
TESTS ?= 0
STATIC ?= 0

MAKEFLAGS+= --no-print-directory

.PHONY:help
help:
	@echo "Targets:"
	@echo ""
	@echo " release    : compile with optimizations and without debug logging"
	@echo " debug      : compile without optimizations and with debug logging"
	@echo ""
	@echo "Options:"
	@echo " ARMCPU : set this for crosscompile for armhf (only). If set it implies STATIC and NO TESTS"
	@echo "          Needed debian packagees: g++-arm-linux-gnueabihf, libssl-dev:armhf, qemu-user"
	@echo "          Examples:"
	@echo "          RaspberryPi3 - cortex-a53"
	@echo "          BananPiPro   - cortex-a7"
	@echo " TESTS  : 0|1 build google tests, default 0"
	@echo " STATIC : 0|1 links statically, default 0"

_build:
	@if [ "x${ARMCPU}" != "xnone" ]; \
	then \
		cmake -S . -B ${BUILD_DIR} -G Ninja \
			  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
			  -DECB_ARMCPU=${ARMCPU} \
			  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm.cmake \
			  -DCMAKE_CROSSCOMPILING_EMULATOR=qemu-arm \
			  -DECB_BUILD_TESTS=OFF \
			  -DECB_STATIC_BUILD=ON; \
		cmake --build ${BUILD_DIR}; \
	else \
		cmake -S . -B ${BUILD_DIR} -G Ninja \
			  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
			  -DECB_BUILD_TESTS=${TESTS} \
			  -DECB_STATIC_BUILD=${STATIC}; \
		cmake --build ${BUILD_DIR}; \
		rm -f compile_commands.json; \
	    ln -s ${BUILD_DIR}/compile_commands.json; \
	fi 

clean:
	rm -rf ${BUILD_DIR}* compile_commands.json

clang-format:
	clang-format -i src/*pp tests/*pp

.PHONY:release
release:
	${MAKE} BUILD_DIR=${BUILD_DIR}${RELEASE_SUFFIX}_arm_${ARMCPU}_static_${STATIC} BUILD_TYPE=Release _build
	cmake --build ${BUILD_DIR}${RELEASE_SUFFIX}_arm_${ARMCPU}_static_${STATIC} --target strip

.PHONY:debug
debug:
	${MAKE} BUILD_DIR=${BUILD_DIR}${DEBUG_SUFFIX}_arm_${ARMCPU}_static_${STATIC} BUILD_TYPE=Debug _build
