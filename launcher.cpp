#include <iostream>
#include <cstring>
#include <vector>
#include "includes/dfa.hpp"

int main() {
    // Dfa dfa;
    
    // //Creates a random DFA.
    // dfa.seed();
    // // dfa.print();

    // cout << "Number of States in A: " << dfa.size << "\n";
    // cout << "Depth of A: " << dfa.getDepth() << "\n\n";
    
    // //Minimise DFA using Hopcroft's Algorithm
    // dfa.minimise();
    // // dfa.print();

    // if (dfa.minimiseTest())
    //     cout << "Minimsation Successful.\n";
    // else cout << "Minimsation Failed.\n";

    // cout << "\nNumber of States in M: " << dfa.size << "\n";
    // cout << "Depth of M: " << dfa.getDepth() << "\n\n";
    
    // //Get Vector of Strongly Connected Components
    // auto v = dfa.getScc();
    // size_t largest_scc = 0;
    // size_t smallest_scc = dfa.size;

    // //Get size of largest SCC.
    // for (auto w : v) largest_scc = max(largest_scc, w.size()); 

    // //Get size of smallest SCC.
    // for (auto w : v) smallest_scc = min(smallest_scc, w.size()); 


    // cout << "Number of Strongly Connected Components in M: " << v.size() << "\n";
    // cout << "Size of Largest SCC in M: " << largest_scc << "\n";
    // cout << "Size of Smallest SCC in M: " << smallest_scc << "\n";




    Dfa dfa;
    dfa.seed();
    dfa.print();
    dfa.optimise();
    dfa.print();

    if (!dfa.minimiseTest()) {
        cout << "Error\n";
    } 



}