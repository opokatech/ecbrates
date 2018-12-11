BUILD_DIR=build
RELEASE_SUFFIX=_release
DEBUG_SUFFIX=_debug

MAKEFLAGS+= --no-print-directory

.PHONY:help
help:
	@echo "Targets:"
	@echo ""
	@echo "release : compile with optimizations and without debug logging"
	@echo "debug   : compile without optimizations and with debug logging"

_build:
	@if [ ! -d ${BUILD_DIR} ] ; \
	then \
		mkdir ${BUILD_DIR}; \
		cd ${BUILD_DIR}; \
		cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DECB_BUILD_TESTS=${TESTS} ..; \
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
	${MAKE} BUILD_DIR=${BUILD_DIR}${RELEASE_SUFFIX} BUILD_TYPE=Release TESTS=ON _build
	${MAKE} -C ${BUILD_DIR}${RELEASE_SUFFIX} strip

.PHONY:debug
debug:
	${MAKE} BUILD_DIR=${BUILD_DIR}${DEBUG_SUFFIX} BUILD_TYPE=Debug TESTS=ON _build

