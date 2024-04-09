#include <iostream>
#include "operations.h"
#include "variable.h"

int main() {
    Variable v1(5);
    Variable v2(4);

    Plus p = v1 + v2;
    Multiply m = v1 * v1;

    std::cout << m.getValue() << std::endl;
    std::cout << m.getPartial(v1) << std::endl;
}