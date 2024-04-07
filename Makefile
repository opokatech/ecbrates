BUILD_DIR=build

MAKEFLAGS+= --no-print-directory

help:
	@echo "Targets:"
	@echo ""
	@echo " release     : compile with optimizations"
	@echo " debug       : compile without optimizations"
	@echo " debug_san   : compile without optimizations and with sanitizers"
	@echo " debug_run   : run debug version using test data as starting point"
	@echo " tests       : as debug but with tests"
	@echo ""
	@echo " clean       : removes all ${BUILD_DIR}* directories and compile_commands.json"
	@echo " clang-format: format all source files"

release:
	cmake --preset release_native
	cmake --build --preset release

debug:
	cmake --preset debug_native_nosan
	cmake --build --preset debug_nosan
	rm -f compile_commands.json
	ln -s ${BUILD_DIR}_debug_cpu_native/compile_commands.json

debug_san:
	cmake --preset debug_native_san
	cmake --build --preset debug_san
	rm -f compile_commands.json
	ln -s ${BUILD_DIR}_debug_cpu_native/compile_commands.json

debug_run: debug_san
	./build_debug_cpu_native/bin/ecb_server --xml_file tests/data/hist.xml --port 8080

tests: debug
	./build_debug_cpu_native/bin/ecb_tests

clean:
	rm -rf ${BUILD_DIR}* compile_commands.json

clang-format:
	clang-format -i src/*pp tests/*pp

.PHONY: _build clean clang-format release debug tests
