.PHONY: all
all: loader.so

.PHONY: test
test: binsearch_test.bin string_funs_test.bin env_parser_test.bin
	./binsearch_test.bin
	./string_funs_test.bin
	./env_parser_test.bin

loader.so: $(shell find src -type f) src/lookup_by_channel.generated.c
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o loader.so

src/lookup_by_channel.generated.c: scripts/gen_lookup_by_channel.py $(shell find registry -type f)
	scripts/gen_lookup_by_channel.py

binsearch_test.bin: test/binsearch_test.c src/string_funs.c src/binsearch_lookup.c
	gcc test/binsearch_test.c src/binsearch_lookup.c src/string_funs.c -g -o binsearch_test.bin -I src

string_funs_test.bin: test/string_funs_test.c src/string_funs.c src/string_funs.h
	gcc test/string_funs_test.c src/string_funs.c -g -o string_funs_test.bin -I src

env_parser_test.bin: test/env_parser_test.c src/env_parser.c src/string_funs.c src/string_funs.h src/syscalls.h src/logging.c
	gcc test/env_parser_test.c src/env_parser.c src/string_funs.c src/logging.c -g -o env_parser_test.bin -I src

.PHONY: clean
clean:
	rm loader.so
	rm *.bin