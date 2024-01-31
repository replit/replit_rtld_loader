.PHONY: all
all: rtld_loader.so

.PHONY: test
test: binsearch_lookup_test.bin string_funs_test.bin env_parser_test.bin
	./binsearch_lookup_test.bin
	./string_funs_test.bin
	./env_parser_test.bin

rtld_loader.so: $(shell find src -type f) src/lookup_by_channel.generated.c
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o rtld_loader.so

src/lookup_by_channel.generated.c: scripts/gen_lookup_by_channel.py $(shell find registry -type f)
	scripts/gen_lookup_by_channel.py

binsearch_lookup_test.bin: test/binsearch_lookup_test.c src/string_funs.c src/binsearch_lookup.c
	gcc test/binsearch_lookup_test.c src/binsearch_lookup.c src/string_funs.c -g -o binsearch_lookup_test.bin -I src

string_funs_test.bin: test/string_funs_test.c src/string_funs.c src/string_funs.h
	gcc test/string_funs_test.c src/string_funs.c -g -o string_funs_test.bin -I src

env_parser_test.bin: test/env_parser_test.c src/env_parser.c src/string_funs.c src/string_funs.h src/syscalls.h src/logging.c
	gcc test/env_parser_test.c src/env_parser.c src/string_funs.c src/logging.c -g -o env_parser_test.bin -I src

.PHONY: clean
clean:
	rm rtld_loader.so || true
	rm *.bin || true