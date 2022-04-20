clean:
	@echo "Cleaning"
	@rm build/bin/mylang
	@echo "Done!"

build: clean
	@echo "Compiling"
	@clang++ -g src/*.cpp -o build/bin/mylang
	@echo "Done!"
