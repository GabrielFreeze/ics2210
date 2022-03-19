#include <iostream>
#include "includes/dfa.hpp"

int main() {
    Dfa dfa;
    
    dfa.seed();
    dfa.print(); 

    dfa.minimise(true);
    dfa.print(); 
}