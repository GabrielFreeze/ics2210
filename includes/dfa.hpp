#pragma once
#include <vector>
#include <iostream>
#include <stack>
#include <cstdint>

using namespace std;

#define MAX_SIZE 64
#define MIN_SIZE 16
#define STRING_NUM 100000

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

        int getDepth();
        vector<vector<int>> getScc();
        void minimise(bool print = false);
        pair<int, int> getChildren(int state);
        int getShortestPath(int start, int end);
        void setMat(vector<vector<int>>& new_mat);
        bool minimiseTest();
        
        
    
    private:

        long word[STRING_NUM] = {0};
        bool accepted[STRING_NUM];

        vector<bool> final;
        bool isEqual(vector<vector<int>> x, vector<vector<int>> y);
        void sccSearch(int s);
        
        vector<vector<int>> mat;


};
