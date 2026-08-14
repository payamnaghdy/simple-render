PLATFORM ?= linux

all:build/Makefile
	cmake --build build --parallel
build/Makefile:
	cmake -B build -DPLATFORM=$(PLATFORM)
clean:
	rm -rf build
