g++ -Wall -std=c++2a -march=native launcher.cpp includes/dfa.cpp -o launcher
valgrind --leak-check=yes --verbose ./launcher
