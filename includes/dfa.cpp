#include "dfa.hpp"
#include <iostream>
#include <time.h>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

#define TEST 0


Dfa::Dfa() {
    srand(time(NULL));                             //Seed random number generator with time.

    if (TEST) {
        size = 6;
    } else {
        size = rand() % (MAX_SIZE-MIN_SIZE) + MIN_SIZE;  //Generate random number between 16 and 64.
    }

    mat.resize(2, std::vector<int>(size)); //Set mat to 2 x size.
}

void Dfa::seed() {

    if (TEST) {
    
        mat = {
         //NODE:A B C D E F
         /*a:*/{1,5,2,3,5,4},
         /*b:*/{5,4,0,4,5,5}};
         //INDX:0 1 2 3 4 5

        start = 0; //A
        final = {0,0,1,0,1,0}; //C & E

    } else {
        //Give all states 2 outgoing transitions (a,b) to any other state.
        for (int i = 0; i < size; i++) {
            //Set 'a' transition from node i to a random node j
            int j = rand() % size;
            mat[0][i] = j;
            
            //Set 'b' transition from node i to a random node j
            j = rand() % size;
            mat[1][i] = j;
        }

        //Choose a random start state
        start = rand() % size;

        //Flip a coin to decide if a state is a final state.
        for (int i = 0; i < size; i++) {
                final.push_back(rand()%2);
            }
    }     
}


void Dfa::print() {
    cout << "DFA |\ta\tb\n----+------------\n";

    for (int i = 0; i < size; i++) {
        
        if (final[i]) {
            cout << '[' << id[i] <<"] |\t";
        } else {
            cout << ' ' << id[i] <<"  |\t";
        }
        cout << id[mat[0][i]] << "\t" << id[mat[1][i]] << "\n";
    }
}

pair<int, int> Dfa::getChildren(int state) {
    return make_pair(mat[0][state],mat[1][state]);
}

int Dfa::getShortestPath(int start, int end) {

    queue<int> q;   //Queue
    vector<int> v; //Visited vector
    
    pair<int, int> p = make_pair(-1,-1);
    int depth = 0;

    //Expand start
    q.push(start);
    v.push_back(start);

    //Repeat until one of the children expanded is the end node.
    while (p.first != end && p.second != end) {
        
        //Expand the first element in the queue.
        p = getChildren(q.front());

        //Remove that element from queue.
        q.pop();

        //Place that elements children in the queue, provided they have not been encountered.
        if (!count(v.begin(), v.end(), p.first)) {
            q.push(p.first);
        }

        if (!count(v.begin(), v.end(), p.second)) {
            q.push(p.second);
        }
        
        //Increment the depth.
        depth++;
    }
    
    return depth;
}

int Dfa::getDepth() {
    
    //Queue to perform BFS with.
    queue<int> q;

    //Pair to store children of node.
    pair<int, int> p;

    //An array of depths for every state
    int depth[size] = {};

    /*Marker is a dummy element. It is popped and re-queued into the queue every time it is encountered.
    It is used to keep track of the depth.*/
    const int MARKER = -1;
    int current_depth = 1;

    //Initalise all depths to -1 except for the first one which will be 0.
    memset(depth+1, -1, sizeof(int)*(size-1));

    //Push starting state and MARKER into queue.
    q.push(start); q.push(MARKER);

    //Repeat until the queue is 'empty' (all traversable nodes have been reached)
    while (q.size() != 1 || q.front() != MARKER) {
        
        //Increment depth if MARKER is encountered.
        if (q.front() == MARKER) {
            q.pop();
            q.push(MARKER);
            current_depth++;
        }

        //Expand the first element in the queue.
        p = getChildren(q.front());
        
        //Remove that element from queue.
        q.pop();

        //Place that element's children in the queue and tally their depth, provided they have not been encountered yet.
        if (depth[p.first] == -1) {
            q.push(p.first);
            depth[p.first] = current_depth;             
        }

        if (depth[p.second] == -1) {
            q.push(p.second);
            depth[p.second] = current_depth; 
        }
    }


    // for (int i = 0; i < size; i++)
    //     cout << depth[i] << '\n';

    return current_depth-1;
}

