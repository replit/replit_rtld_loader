.PHONY: all
all: dl_loader.so

dl_loader.so: $(shell find src -type f)
	echo 
	gcc -shared -nostdlib -fno-stack-protector -fPIC -O2 src/*.c -o loader.so

.PHONY: clean
clean:
	rm dl_loader.so