.PHONY: all
all: rtld_loader.so

.PHONY: test
test: string_funs_test.bin env_parser_test.bin dynamic_lookup_test.bin rtld_loader.so
	./string_funs_test.bin
	./env_parser_test.bin
	python3 test/dynamic_lookup_test.py
	python3 test/integration_tests.py

rtld_loader.so: $(shell find src -type f)
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o rtld_loader.so

string_funs_test.bin: test/string_funs_test.c src/string_funs.[ch]
	gcc $^ -g -o $@ -I src

env_parser_test.bin: test/env_parser_test.c src/env_parser.[ch] src/string_funs.[ch] src/syscalls.h src/logging.[ch]
	gcc $^ -g -o $@ -I src

dynamic_lookup_test.bin: test/dynamic_lookup_test.c src/dynamic_lookup.[ch] src/string_funs.[ch] src/syscalls.h src/logging.[ch]
	gcc $^ -g -o $@ -I src

.PHONY: lint
lint: src/*.[ch] test/*.[ch]
	clang-format -i $^

.PHONY: lint-check
lint-check: src/*.[ch] test/*.[ch]
	clang-format --dry-run --Werror -i $^

.PHONY: clean
clean:
	rm rtld_loader.so || true
	rm *.bin || true