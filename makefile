all:
	g++ -std=c++11 -Wall -Werror quadtree.cpp test_main.cpp -o tp -llodepng -L.
	./tp
