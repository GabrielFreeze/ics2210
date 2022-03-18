#pragma once
#include <vector>
#include <iostream>
#include <cstdint>

using namespace std;

/*An empty transition from state X to state Y will be represented as 0 (00 in binary) in the adjacency matrix.
A transition for the symbol 'a' from state X to state Y will be represented as TR_A (01 in binary) in the adjacency matrix.
A transition for the symbol 'b' from state X to state Y will be represented as TR_B (10 in binary) in the adjacency matrix.
A transition for the symbol 'a' and the symbol 'b' from state X to state Y will be represented as TR_A | TR_B (11 in binary) in the adjacency matrix.*/
#define TR_A 1
#define TR_B 2
#define TR_AB (TR_A | TR_B)

#define MAX_SIZE 8
#define MIN_SIZE 3
class Dfa {
    public:
        
        int size;
        int start;
        char id[64] = {'A','B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                          'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                          '0','1','2','3','4','5','6','7','8','9',
                          '!','?'};

        Dfa();
        void seed();
        void print();

        int getShortestPath(int start, int end);
        pair<int, int> getChildren(int state);
        int getDepth();
        
    
    private:
        vector<vector<int>> mat;
        vector<bool> final;
};
