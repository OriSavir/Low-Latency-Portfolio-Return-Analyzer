all:
	mkdir -p build
	cd build && cmake ..
	cmake --build build

clean:
	rm -rf build