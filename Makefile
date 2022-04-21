clean:
	@echo "Cleaning"
	@rm -f build/bin/mylang
	@echo "Done!"

build: clean
	@echo "Compiling"
	clang++ -g -O3 -c `llvm-config --cxxflags` -I./header ./src/*.cpp
	@echo "Linking"
	clang++ *.o `llvm-config --ldflags --libs` -lpthread -lncurses -o build/bin/mylang
	@rm *.o
	@echo "Done!"
