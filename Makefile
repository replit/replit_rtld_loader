.PHONY: all
all: rtld_loader.so test

.PHONY: test
test: binsearch_lookup_test.bin string_funs_test.bin env_parser_test.bin dynamic_lookup_test.bin
	./binsearch_lookup_test.bin
	./string_funs_test.bin
	./env_parser_test.bin
	python test/dynamic_lookup_test.py

rtld_loader.so: $(shell find src -type f) src/lookup_by_channel.generated.c
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o rtld_loader.so

src/lookup_by_channel.generated.c: scripts/gen_lookup_by_channel.py $(shell find registry -type f)
	scripts/gen_lookup_by_channel.py

binsearch_lookup_test.bin: test/binsearch_lookup_test.c src/string_funs.[ch] src/binsearch_lookup.[ch]
	gcc $^ -g -o $@ -I src

string_funs_test.bin: test/string_funs_test.c src/string_funs.[ch]
	gcc $^ -g -o $@ -I src

env_parser_test.bin: test/env_parser_test.c src/env_parser.[ch] src/string_funs.[ch] src/syscalls.h src/logging.[ch]
	gcc $^ -g -o $@ -I src

dynamic_lookup_test.bin: test/dynamic_lookup_test.c src/dynamic_lookup.[ch] src/string_funs.[ch] src/syscalls.h src/logging.[ch]
	gcc $^ -g -o $@ -I src

.PHONY: lint
lint: src/*.[ch] test/*.[ch]
	clang-format -i $^

.PHONY: clean
clean:
	rm rtld_loader.so || true
	rm *.bin || true