#include <iostream>
#include "includes/dfa.hpp"

int main() {
    Dfa dfa;
    
    dfa.seed();
    dfa.print(); 

    // dfa.minimise(true);
    // dfa.print(); 

    auto v = dfa.getScc();

    for (int i = 0; i < v.size(); i++) {
        cout << "{ ";
        for (int j = 0; j < v[i].size(); j++) {
            cout << dfa.id[v[i][j]] <<" ";
        }
        cout << "}\n";
    }

}