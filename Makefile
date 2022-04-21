

link: compile
	clang++ build/*.o `llvm-config --ldflags --libs` -lpthread -lncurses -o build/bin/mylang

compile: build/ast.o build/parser.o build/error.o build/codegen.o build/main.o

build/ast.o: src/ast.cpp
	clang++ -g -O3 -c `llvm-config --cxxflags` -I./header src/ast.cpp -o build/ast.o

build/parser.o: src/parser.cpp
	clang++ -g -O3 -c `llvm-config --cxxflags` -I./header src/parser.cpp -o build/parser.o

build/error.o: src/error.cpp
	clang++ -g -O3 -c `llvm-config --cxxflags` -I./header src/error.cpp -o build/error.o

build/codegen.o: src/codegen.cpp
	clang++ -g -O3 -c `llvm-config --cxxflags` -I./header src/codegen.cpp -o build/codegen.o

build/main.o: src/main.cpp
	clang++ -g -O3 -c `llvm-config --cxxflags` -I./header src/main.cpp -o build/main.o

clean:
	rm -f build/*.o
	rm -f build/bin/*

