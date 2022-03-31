#pragma once
#include <vector>
#include <iostream>
#include <stack>
#include <cstdint>

using namespace std;

#define MAX_SIZE 5
#define MIN_SIZE 6
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
        vector<int> BFS(int starting_vertex);
        vector<vector<int>> getScc();
        vector<int> getUnreachable();
        vector<int> getDead();

        pair<int, int> getChildren(int state);
        int getShortestPath(int start, int end);
        void setMat(vector<vector<int>>& new_mat);

        void minimise(bool print = false);
        bool minimiseTest();
        void optimise();

        int addState(int a, int b, bool is_final);


        
    
    private:

        long word[STRING_NUM] = {0};
        bool accepted[STRING_NUM];

        vector<bool> final;
        bool isEqual(vector<vector<int>> x, vector<vector<int>> y);
        void sccSearch(int s);
        
        vector<vector<int>> mat;


};
