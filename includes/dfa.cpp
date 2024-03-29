#include "dfa.hpp"
#include <iostream>
#include <time.h>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <stack>
#include <cassert>

using namespace std;

#define TEST 0

//Used to find SCCs.
stack<int> stk;
int current_order;
vector<int> order;
vector<int> low_link;
vector<bool> in_stack;
vector<vector<int>> scc;


Dfa::Dfa() {
    srand(time(NULL));                             //Seed random number generator with time.

    if (TEST) {
        size = 6;
    } else {
        size = rand() % (MAX_SIZE-MIN_SIZE) + MIN_SIZE;  //Generate random number between 16 and 64.
    }

    mat.resize(2, vector<int>(size)); //Set mat to 2 x size.
}

void Dfa::setMat(vector<vector<int>>& new_mat) {
    mat = new_mat;
    size = mat[0].size();
}

void Dfa::seed() {

    if (TEST) {
        
        mat = {
         //NODE:A B C D E F
         /*a:*/{1,2,0,3,3,4},
         /*b:*/{5,2,5,3,5,4}};
         //INDX:0 1 2 3 4 5

        mat = {
         //NODE:A B C D E F
         /*a:*/{1,5,2,3,5,4},
         /*b:*/{5,4,0,4,5,5}};
         //INDX:0 1 2 3 4 5

        mat = {
         //NODE:A B C D E F G H I J
         /*a:*/{9,6,4,9,5,6,3,6,4,5},
         /*b:*/{0,1,9,5,7,8,5,1,6,3}};
         //INDX:0 1 2 3 4 5 6 7 8 9

        //Dead statees test
        mat = {
         //NODE:A B C D E F
         /*a:*/{1,3,2,5,4,2},
         /*b:*/{5,2,2,4,4,5}};
         //INDX:0 1 2 3 4 5


        // mat = {
        //  //NODE:A B C D E F
        //  /*a:*/{3,3,2,2,1,5},
        //  /*b:*/{0,4,0,2,4,5}};
        //  //INDX:0 1 2 3 4 5
        

        start = 0;
        final = {0,0,0,0,1,0}; 


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

    int length = sizeof(word[0])*8;

    //Gernerate testing strings
    for (int i = 0; i < STRING_NUM; i++) {
        
        //Generate random string, represented as a bit stream. 0 -> a, 1-> b
        int current_state = start;
        long t;
        for (int j = 0; j < length; j++) {
            
            //Choose 'a' or 'b' transition.
            t = rand() % 2;

            //Save generated string
            word[i] |= t << j;

            //Move to the next state.
            current_state = mat[t][current_state];
        }
        //Was the generated string accepted?
        accepted[i] = final[current_state];
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
    cout << "Start -> " << id[start] << '\n';
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

bool Dfa::isEqual(vector<vector<int>> x, vector<vector<int>> y) {  
    if (x.size() != y.size())
        return false;

    for (int i = 0; i < x.size(); i++) {
        if (x[i] != y[i])
            return false;
    }

    return true;
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

vector<int> Dfa::BFS(int starting_vertex) {
    //Queue to perform BFS with.
    queue<int> q;

    //Pair to store children of node.
    pair<int, int> p;

    //Stores which vertices have been visited.
    vector<int> visited(size, 0);

    /*Marker is a dummy element. It is popped and re-queued into the queue every time it is encountered.
    It is used to keep track of the depth.*/
    const int MARKER = -1;


    //Push starting state and MARKER into queue.
    q.push(starting_vertex); q.push(MARKER);
    visited[starting_vertex] = true;

    //Repeat until the queue is 'empty' (all traversable nodes have been reached)
    while (q.size() != 1 || q.front() != MARKER) {
        
        //Increment depth if MARKER is encountered.
        if (q.front() == MARKER) {
            q.pop();
            q.push(MARKER);
        }

        //Expand the first element in the queue.
        p = getChildren(q.front());
        
        //Remove that element from queue.
        q.pop();

        //Place that element's children in the queue and mark as visited, provided they have not been encountered yet.
        if (!visited[p.first]) {
            q.push(p.first);
            visited[p.first] = true;
        }

        if (!visited[p.second]) {
            q.push(p.second);
            visited[p.second] = true; 
        }
    }
    
    return visited;
    
}

vector<int> Dfa::getUnreachable() {
   
   auto visited = BFS(start);
   vector<int> unreachable;

   for (int i = 0; i < visited.size(); i++) {
        if (!visited[i]) unreachable.push_back(i);
   }

   return unreachable;

}

vector<int> Dfa::getDead() {

    //To store dead nodes.
    vector<int> dead;

    //Perform BFS on every node and determine if it is dead or not.
    for (int i = 0; i < size; i++) {

        auto visited = BFS(i);
        bool alive = false;


        for (int j = 0; j < size; j++)
            alive |= visited[j] && final[j];
        

        if (!alive)
            dead.push_back(i);
    }

    return dead;

}

int Dfa::addState(int a, int b, bool is_final) {

    int new_state = size;

    assert((0 <= a && a < size+1 && 0 <= b && b < size+1) && 
           "Invalid transition supplied in addState");


    mat[0].push_back(a);
    mat[1].push_back(b);

    final.push_back(is_final);

    size++;

    return new_state;
}

void Dfa::optimise() {

    //Get all unreachable and dead nodes which are unique to each other and store them in vertices.    
    auto vertices = getUnreachable();
    auto dead = getDead();

    for (auto d : dead) {
        if (find(vertices.begin(), vertices.end(), d) == vertices.end())
            vertices.push_back(d);
    }

    //Add dummy state to keep DFA complete.
    int dummy = size;
    addState(dummy,dummy,false);

    /*If the starting state is in vertices it implies it is a dead state.
     (We know that the starting state is not unreachable)
     If the starting state is a dead state then it implies the language recognised by the automaton is Ø.
     We know that the simplest DFA that recognises Ø is one with no states.*/
    if (find(vertices.begin(), vertices.end(), start) != vertices.end()) {
        vector<vector<int>> empty_mat = {{0},{0}};
        setMat(empty_mat);
        final = vector<bool>(1,0);
        start = 0;
        return;
    }

    //vertices -> the vertices to remove

    //For all vertices to removee
    for (int i = 0; i < vertices.size(); i++) {

        int v = vertices[i];

        //Remove element
        mat[0].erase(mat[0].begin()+v);
        mat[1].erase(mat[1].begin()+v);
        final.erase(final.begin()+v);
        size--;

        if(start > v) {
            start --;
        }

        //Update all transitions
        for (int j = 0; j < size; j++) {
            for (int k : {0,1}) {
                

                if (mat[k][j] > v) {
                    mat[k][j]--;
                }
                else if(mat[k][j] == v)
                    mat[k][j] = mat[0][size-1]-1;

            }
        }
        
        //Update vertices index.
        for (int j = i; j < vertices.size(); j++) {
            if (vertices[j] > v)
                vertices[j]--;
        }


    }


    //If dummy state is used, return.
    for (int i = 0; i < size-1; i++) {
        for (auto j : {0,1}) {
            if (mat[j][i] == mat[0][size-1])
                return;
        }
    }


    //Otherwise remove dummy state.
    mat[0].pop_back();
    mat[1].pop_back();
    final.pop_back();
    size --;

    return;




}

//HopCroft's Minimisation Algorithm
void Dfa::minimise(bool print) {
    
    //If all states are accepting/non-acceptting, then the minimsation algorithm won't change anything.
    if (*max_element(begin(final), end(final)) == *min_element(begin(final), end(final))) {
        return;
    }

    vector<vector<int>> npart = {};      //Next Partitions.
    vector<vector<int>> ppart = {{},{}}; //Previous Partitions.
    int ipart[size];                     //The partition every state belonged to in ppart.

    int s1,s2, old_npart_size;
    int eqv = 0;
    pair<int,int> s1Child, s2Child;
    bool partioned = false;
    bool first_iteration = true;
    
    //Seperate accepting and non-accepting states.
    for (int i = 0; i < size; i++) {
        ppart[final[i]].push_back(i);
        ipart[i] = final[i]; //Mark the partions the states are in.
    }
    
    do {
        //Do not swap partitions in the first iteration.
        if (!first_iteration) {
            ppart = npart; //Previous Partions become Next Partitions.
            npart.clear(); //Next Partions are reset.
        } else first_iteration = false;

        //For every partition i in ppart.
        for (int i = 0; i < ppart.size(); i++) {

            if (ppart[i].size() == 1) {
                /*Do not attempt to match.
                Place in a singleton set and go to next partition.*/
                npart.push_back({ppart[i][0]});
                continue;
            }

            //This variable is used to keep track of which partitions are valid for matching.
            old_npart_size = npart.size();

            s1 = ppart[i][0];
            s2 = ppart[i][1];

            s1Child = getChildren(s1);
            s2Child = getChildren(s2);

            //In which partition does s1 & s2 belong to?
            //For every transition x ∈ {a,b}: t(x,s1) & t(x,s2), must either be the same, or belong in the same partition in ppart.
            if ((s1Child.first  == s2Child.first  || ipart[s1Child.first]  == ipart[s2Child.first]) &&
                (s1Child.second == s2Child.second || ipart[s1Child.second] == ipart[s2Child.second])) {
                    
                //s1 & s2 belong in the same partition.
                npart.push_back({s1,s2});
            } else {
                //s1 & s2 belong in different partitions.
                npart.push_back({s1}); npart.push_back({s2});
            }      
            
            /*Match every state in ppart[i] with every possible partition in npart. 
            If no partition in npart accepts the state, then create a new one for it.*/

            //For every state j in ppart[i]
            for (int j = 2; j < ppart[i].size(); j++) {
                s1 = ppart[i][j];
                partioned = false; //Checks wether s1 has been added to a partion.

                //For every possible partition in npart, attempt to add s1 to it. 
                for (int k = old_npart_size; k < npart.size(); k++) {

                    /*Choose the first element in npart[k]. This is beacause if s1 matches with some element in npart[k].
                    Then it will match with all elements in npart[k].
                    Similairly, if s1 fails to match with an element in npart[k], then it will not match with the rest.
                    Choosing the first element is always guaranteed, as a partiton has at least 1 element.*/
                    s2 = npart[k][0]; 

                    s1Child = getChildren(s1);
                    s2Child = getChildren(s2);

                    //Does s1 belong in the same partition as s2?
                    if ((s1Child.first  == s2Child.first  || ipart[s1Child.first]  == ipart[s2Child.first]) &&
                        (s1Child.second == s2Child.second || ipart[s1Child.second] == ipart[s2Child.second])) {
                            
                        //s1 & s2 belong in the same partition.
                        npart[k].push_back(s1); //Add s1 in the same partition as s2.
                        partioned = true;

                        /*Break from the loop:
                        s1 has been added to a partition and hence we do not need to check the other partitions.*/
                        break; 
                    }
                }

                //If s1 hasn't been added to a pre-existing partition, create a new one for it.
                if (!partioned) {
                    npart.push_back({s1});
                }
            }      
        }
        
        //Print k-Equivalence Partions.
        if (print) {
            cout << eqv << "-Equivalence:\n";
            for (int i = 0; i < ppart.size(); i++) {
                
                cout << "{ ";
                for (int j = 0; j < ppart[i].size(); j++) {
                    cout << id[ppart[i][j]] << " ";
                }
                cout << "}\n";
            }
            cout << "\n";
        }
        
        //Update ipart to reference npart.
        for (int i = 0; i < npart.size(); i++) {
            for (int j = 0; j < npart[i].size(); j++) {
                ipart[npart[i][j]] = i;
            }
        }

        old_npart_size = 0;
        eqv ++;
    } while (!isEqual(npart,ppart)); //Keep repeating until the algorithm converges.
    
    //Print k-Equivalence Partions.
    if (print) {

        cout << eqv << "-Equivalence:\n";
        for (int i = 0; i < npart.size(); i++) {
            cout << "{ ";
            for (int j = 0; j < npart[i].size(); j++) {
                cout << id[npart[i][j]] << " ";
            }
            cout << "}\n";
        }
        cout << "\n";
    }

    //The DFA remained exactly the same.
    if (npart.size() == size)
        return;

    //Initalise placeholder variables.
    vector<vector<int>> new_mat(2, vector<int>(npart.size()));
    vector<bool> new_final(npart.size());

    /*Create the new transitions. For every partition in npart,
    consult the original transition of any state within that partition
    to get the new transitions.*/
    for (int i = 0; i < npart.size(); i++) {
        //Get the first state witin npart[i]
        s1 = npart[i][0];
        
        s1Child = getChildren(s1);

        //What is the partition that the 'a' child belongs to?
        new_mat[0][i] = ipart[s1Child.first];

        //What is the partition that the 'b' child belongs to?
        new_mat[1][i] = ipart[s1Child.second];
    }
    
    /*The partition containing the original starting state,
    will be the new starting state.*/
    start = ipart[start];
    
    /*Find which partions the original final states where placed in,
    and make the element at that index in new_final true.*/
    for (int i = 0; i < size; i++) {
        new_final[ipart[i]] = final[i];
    }

    final = new_final; //Copy over values.
    
    
    //Update underlying datastructure.
    setMat(new_mat);

    return;
}

vector<vector<int>> Dfa::getScc() {

    //Holds the order in which states are traversed.
    order.clear(); 
    order.resize(size);
    fill(order.begin(), order.end(), -1); //Initally all -1.

    current_order = 0;

    /*Holds the low link value of each state.
    The low link value of a state is the smallest low link value in that state's SCC.
    States with the same low link value belong in the same SCC.*/
    low_link.clear();
    low_link.resize(size);
    fill(low_link.begin(), low_link.end(), -1); //Initally all -1.

    //Whether a state was visited by DFS or not.
    in_stack.clear();
    in_stack.resize(size);
    fill(in_stack.begin(), in_stack.end(), false); //Initally all false.

    //Clear stack used to retrieve states in the same SCC.
    stk = stack<int>();

    //Clear the array where the output will be stored.
    scc.clear();

    //For all unvisited states, perform DFS and fill scc.
    for (int i = 0; i < size; i++) {
        if (order[i] == -1) {
            sccSearch(i);
        }
    }

    return scc;
    
}

void Dfa::sccSearch(int s) {

    //Set traversal order of current state.
    order[s] = current_order ++;

    //Initally the low_link value is the same as the order traversal.
    low_link[s] = order[s];

    //Add current state to stack
    stk.push(s);
    in_stack[s] = true;

    //Expand s.
    auto sChild = getChildren(s);

    //If both transitions lead to the same state only consider one child.
    if (sChild.first == sChild.second) {
        
        //If 'a' child has not been traversed before.
        if (order[sChild.first] == -1) {
            //Recurse over the 'a' child
            sccSearch(sChild.first); 

            /*If the low_link value of the 'a' child is smaller than s,
            update the low_link value of s.*/
            low_link[s] = min(low_link[sChild.first], low_link[s]);
        }

        //If 'a' child has been traversed and is in the stack.
        else if (in_stack[sChild.first]) {
            //The 'a' transition is  back edge and closes off an SCC.
            low_link[s] = min(order[sChild.first], low_link[s]);
        }
        
    } else {

        //If 'a' child has not been traversed before.
        if (order[sChild.first] == -1) {

            //Recurse over the 'a' child.
            sccSearch(sChild.first); 

            /*If the low_link value of the 'a' child is smaller than s,
            update the low_link value of s.*/
            low_link[s] = min(low_link[sChild.first], low_link[s]);
        }

        //If 'a' child has been traversed and is in the stack.
        else if (in_stack[sChild.first]) {
            //The 'a' transition is  back edge and closes off an SCC.
            low_link[s] = min(order[sChild.first], low_link[s]);
        }

        //If 'b' child has not been traversed before.
        if (order[sChild.second] == -1) {
            //Recurse over the 'b' child.
            sccSearch(sChild.second); 

            /*If the low_link value of the 'b' child is smaller than s,
            update the low_link value of s.*/
            low_link[s] = min(low_link[sChild.second], low_link[s]);
        }

        //If 'b' child has been traversed and is in the stack.
        else if (in_stack[sChild.second]) {
            //The 'b' transition is a back edge and closes off an SCC.
            low_link[s] = min(order[sChild.second], low_link[s]);
        }
        
    }
    
    /*If an SCC has been found, pop all the states
    until you reach the root state of the SCC.*/

    //Root states of an SCC have their order and low_link value the same.
    if (order[s] == low_link[s]) {
        vector<int> current_scc; //Current SCC

        //Repeat until s is found in the stack.
        while(s != stk.top()) {
            current_scc.insert(current_scc.begin(),stk.top()); //Add top element to current_scc
            in_stack[stk.top()] = false; //Update in_stack
            stk.pop(); //Remove top element.
        }

        //Add s to current_scc.
        current_scc.insert(current_scc.begin(),s);
        in_stack[s] = false;
        stk.pop();

        scc.push_back(current_scc);
    }
}

bool Dfa::test() {

    //Pass previously generated testing strings to the DFA.
    int length = sizeof(word[0])*8;
    
    for (int i = 0; i < STRING_NUM; i++) {
        
        int current_state = start;
        unsigned long long current_transition = word[i];

        for (int j = 0; j < length; j++) {
            current_state = mat[(current_transition) & 1][current_state];
            current_transition >>= 1;
        }

        //Strings was not supposed to be accepted/rejected.
        if (accepted[i] != final[current_state]) {
            cout << "Error on string " << i << " :";

            for (int j = 0; j < length; j++) {
                cout << "ab"[(word[i]>>j) & 1];
            }
            cout << '\n';

            cout << "Expected: " << accepted[i] << " Actual: " << final[current_state] << '\n';
            return false;
        }
    }

    //All strings were accepted.
    return true;

}
