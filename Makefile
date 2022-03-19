all:
	g++ -std=c++2a -march=native launcher.cpp includes/dfa.cpp -o launcher
