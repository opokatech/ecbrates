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
	@if [ ! -d ${BUILD_DIR} ] ; \
	then \
		mkdir ${BUILD_DIR}; \
		cd ${BUILD_DIR}; \
		if [ "x${ARMCPU}" != "xnone" ]; \
	    then \
			cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
			      -DECB_ARMCPU=${ARMCPU} \
				  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm.cmake \
				  -DCMAKE_CROSSCOMPILING_EMULATOR=qemu-arm \
				  -DECB_BUILD_TESTS=OFF \
				  -DECB_STATIC_BUILD=ON \
				  ..; \
		else \
			cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
				  -DECB_BUILD_TESTS=${TESTS} \
				  -DECB_STATIC_BUILD=${STATIC} \
				  ..; \
		fi; \
		${MAKE}; \
	else \
		cd ${BUILD_DIR}; \
		${MAKE}; \
	fi

	@rm -f compile_commands.json
	@ln -s ${BUILD_DIR}/compile_commands.json

clean:
	rm -rf ${BUILD_DIR}* compile_commands.json

ctags:
	(cd src && ctags -R . ../externals)

clang-format:
	clang-format -i src/*pp tests/*pp

cleanctags:
	find -type f -name tags -exec rm -f {} \;

.PHONY:release
release:
	${MAKE} BUILD_DIR=${BUILD_DIR}${RELEASE_SUFFIX}_arm_${ARMCPU}_tests_${TESTS}_static_${STATIC} BUILD_TYPE=Release _build
	${MAKE} -C ${BUILD_DIR}${RELEASE_SUFFIX}_arm_${ARMCPU}_tests_${TESTS}_static_${STATIC} strip

.PHONY:debug
debug:
	${MAKE} BUILD_DIR=${BUILD_DIR}${DEBUG_SUFFIX}_arm_${ARMCPU}_tests_${TESTS}_static_${STATIC} BUILD_TYPE=Debug _build
