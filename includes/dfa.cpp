#include "dfa.hpp"
#include <iostream>
#include <time.h>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

#define TEST 1


Dfa::Dfa() {
    cout << "Class Created\n";
    srand(time(NULL));                             //Seed random number generator with time.

    if (TEST) {
        size = 6;
    } else {
        size = rand() % (MAX_SIZE-MIN_SIZE) + MIN_SIZE;  //Generate random number between 16 and 64.
    }

    mat.resize(size, std::vector<int>(size)); //Set mat to size x size.
}

void Dfa::seed() {

    if (TEST) {
        // mat = {
        //  //TO:       A B C D E F
        //  /*FROM: A*/{0,0,1,0,2,0},
        //  /*FROM: B*/{3,0,0,0,0,0},
        //  /*FROM: C*/{0,2,0,0,0,1},
        //  /*FROM: D*/{0,0,0,2,1,0},
        //  /*FROM: E*/{1,0,0,0,0,2},
        //  /*FROM: F*/{1,0,2,0,0,0}};
        
    
        mat = {
         //TO:       A B C D E F
         /*FROM: A*/{0,1,0,0,0,2},
         /*FROM: B*/{0,0,0,0,2,1},
         /*FROM: C*/{2,0,1,0,0,0},
         /*FROM: D*/{0,0,0,1,2,0},
         /*FROM: E*/{0,0,0,0,0,3},
         /*FROM: F*/{0,0,0,0,1,2}};
        

        start = 0; //A
        final = {0,0,1,0,1,0}; //C & E

    } else {
        //Give all states 2 outgoing transitions (a,b) to any other state.
        for (int i = 0; i < size; i++) {
            //Set 'a' transition from node i to a random node j
            int j = rand() % size;
            mat[i][j] |= TR_A;
            
            //Set 'b' transition from node i to a random node j
            j = rand() % size;
            mat[i][j] |= TR_B;
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


    string map[4] = {"__","a_","_b","ab"};

    cout << "\nFROM:" << " ";
    for(int i = 0; i < size; i++) {
        if (final[i])
            cout << "[" << id[i] << "] ";
        else 
            cout << " " << id[i] << "  ";
    }
    
    cout << "\nTO:";

    for (int i = 0; i < size; i++) {
        cout << '\n';
        
        if (final[i])
            cout << "[" << id[i] << "]    ";
        else 
            cout << " " << id[i] << "     ";
        for (int j = 0; j < size; j++) {
            cout << map[mat[j][i]] << "  ";
        }
    }
    cout << "\nStarting State: " << id[start] << "\n";
}

pair<int, int> Dfa::getChildren(int state) {
    
    int first = -1;
    int second = -1;

    for (int i = 0; i < size; i++) {
        //If the first bit is 1 then there is an 'a' transition from 'state' to state i.
        if (mat[state][i] & TR_A)
            first = i;

        //If the second bit is 1 then there is an 'b' transition from 'state' to state i.
        if (mat[state][i] & TR_B)
            second = i;
        
        //Both 'a' and 'b' transitions must have been encountered in order to break from the loop.
        if ((first | second) != -1)
            break;
    }

    return make_pair(first,second);
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

