BUILD_DIR=build
RELEASE_SUFFIX=_release
DEBUG_SUFFIX=_debug

ECB_TESTS ?= OFF
ECB_STATIC ?= OFF

MAKEFLAGS+= --no-print-directory

.PHONY:help
help:
	@echo "Targets:"
	@echo ""
	@echo " release    : compile with optimizations and without debug logging"
	@echo " debug      : compile without optimizations and with debug logging"
	@echo ""
	@echo "Options:"
	@echo " ECB_TESTS  : [ON|OFF] build google tests, default OFF"
	@echo " ECB_STATIC : [ON|OFF] links statically, default OFF"

_build:
	@if [ ! -d ${BUILD_DIR} ] ; \
	then \
		mkdir ${BUILD_DIR}; \
		cd ${BUILD_DIR}; \
		cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
              -DECB_BUILD_TESTS=${ECB_TESTS} \
              -DECB_STATIC_BUILD=${ECB_STATIC} \
              ..; \
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
	${MAKE} BUILD_DIR=${BUILD_DIR}${RELEASE_SUFFIX}_tests_${ECB_TESTS}_static_${ECB_STATIC} BUILD_TYPE=Release _build
	${MAKE} -C ${BUILD_DIR}${RELEASE_SUFFIX} strip

.PHONY:debug
debug:
	${MAKE} BUILD_DIR=${BUILD_DIR}${DEBUG_SUFFIX}_tests_${ECB_TESTS}_static_${ECB_STATIC} BUILD_TYPE=Debug _build
