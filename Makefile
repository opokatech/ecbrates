I386_DIR=build_i386
RELEASE_SUFFIX=_release
DEBUG_SUFFIX=_debug

MAKEFLAGS+= --no-print-directory

.PHONY:help
help:
	@echo "Targets:"
	@echo ""
	@echo "i386       : Release build for gnu i386"
	@echo "i386-debug : BUild with debug output for gnu i386"

build:
	rm -rf ${BUILD_DIR}
	mkdir ${BUILD_DIR}

	cd ${BUILD_DIR}; \
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DECB_BUILD_TESTS=${TESTS} ..; \
	${MAKE}

clean:
	rm -rf ${I386_DIR}*

ctags:
	(cd src && ctags -R . ../externals)

clang-format:
	clang-format -i src/*pp tests/*pp

cleanctags:
	find -type f -name tags -exec rm -f {} \;

.PHONY:i386
i386:
	${MAKE} BUILD_DIR=${I386_DIR}${RELEASE_SUFFIX} BUILD_TYPE=Release TESTS=ON build
	${MAKE} -C ${I386_DIR}${RELEASE_SUFFIX} strip

.PHONY:i386-debug
i386-debug:
	${MAKE} BUILD_DIR=${I386_DIR}${DEBUG_SUFFIX} BUILD_TYPE=Debug TESTS=ON build

