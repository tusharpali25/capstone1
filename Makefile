all:
	g++ main.cpp FileManager.cpp UIManager.cpp -lncurses -o file_explorer

run:
	./file_explorer
