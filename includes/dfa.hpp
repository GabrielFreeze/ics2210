#pragma once
#include <vector>
#include <iostream>
#include <cstdint>

using namespace std;

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
        void minimise();
        void setMat(vector<vector<int>>& new_mat);
        
        
    
    private:

        bool isEqual(vector<vector<int>> x, vector<vector<int>> y);

        vector<vector<int>> mat;
        vector<bool> final;
};
