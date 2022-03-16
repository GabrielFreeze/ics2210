#include "dfa.hpp"
#include <iostream>
#include <time.h>
#include <queue>

using namespace std;


Dfa::Dfa() {
    cout << "Class Created\n";
    srand(time(NULL));                             //Seed random number generator with time.
    size = rand() % (MAX_SIZE-MIN_SIZE) + MIN_SIZE;  //Generate random number between 16 and 64.

    mat.resize(size, std::vector<uint8_t>(size)); //Set mat to size x size.
}

void Dfa::seed() {
    
        //Give all states 2 outgoing transitions (a,b) to any other state.
        for (int i = 0; i < size; i++) {
            //Set 'a' transition from node i to a random node j
            uint8_t j = rand() % size;
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

pair<uint8_t, uint8_t> Dfa::getChildren(uint8_t state) {
    
    uint8_t first = -1;
    uint8_t second = -1;

    for (int i = 0; i < size; i++) {
        //If the first bit is 1 then there is an 'a' transition from 'state' to state i.
        if (mat[state][i] & TR_A)
            first = i;

        //If the second bit is 1 then there is an 'b' transition from 'state' to state i.
        if (mat[state][i] & TR_B)
            second = i;
        
        //Both 'a' and 'b' transitions must have been encountered in order to break from the loop.
        if (first | second != -1)
            break;
    }

    return make_pair(first,second);
}

uint8_t Dfa::findShortestPath(uint8_t start, uint8_t end) {

    queue<uint8_t> q;
    pair<uint8_t, uint8_t> p = make_pair(-1,-1);
    uint8_t i = 0;

    //Expand start
    q.push(start);

    //Place children in queue
    while (p.first != end && p.second != end) {
        p = getChildren(q.front());
        q.pop();

        q.push(p.first);
        q.push(p.second);
        
        i++;
    }
    
    return i;
}


