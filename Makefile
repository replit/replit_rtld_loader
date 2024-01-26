.PHONY: all
all: rtld_loader.so

rtld_loader.so: $(shell find src -type f)
	echo 
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o loader.so

.PHONY: clean
clean:
	rm rtld_loader.so