#include <iostream>
#include <fstream>
#include <sstream>
#include "../context.h"

int main() {
    Variable x = createRandomTensorVariable<2, 2>();
    Variable y = createRandomTensorVariable<2, 2>();
    Variable z = createRandomTensorVariable<2, 2>();
    Variable l = createRandomTensorVariable<2, 2>();
    Variable n = (((x + y) + z) + l);
    
    computeGradients(n);

    for(auto item : x.value()) {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
    for(auto item : x.partials()) {
        std::cout << item << ' ';
    }
}