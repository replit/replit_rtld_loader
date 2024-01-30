.PHONY: all
all: loader.so

.PHONY: test
test: binsearch_test
	./binsearch_test

loader.so: $(shell find src -type f) src/lookup_by_channel.generated.c
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o loader.so

src/lookup_by_channel.generated.c: scripts/gen_lookup_by_channel.py $(shell find registry -type f)
	scripts/gen_lookup_by_channel.py

binsearch_test: test/binsearch_test.c src/string_funs.c src/binsearch_lookup.c
	gcc test/binsearch_test.c src/binsearch_lookup.c src/string_funs.c -g -o binsearch_test -I src

.PHONY: clean
clean:
	rm loader.so