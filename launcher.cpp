#include <iostream>
#include "includes/dfa.hpp"

int main() {
    Dfa dfa;
    
    dfa.seed();
    dfa.print(); 
    cout << dfa.getDepth() << '\n';
    // auto p = dfa.getChildren(0);
    // cout << p.first <<" " << p.second << '\n';
}