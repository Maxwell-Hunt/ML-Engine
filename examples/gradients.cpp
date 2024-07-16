#include <iostream>
#include <fstream>
#include <sstream>
#include "../expression.h"
#include "../context.h"

int main() {
    using namespace Engine;
    Expression a = createVariable(5.f);
    Expression b = square(a);
    Expression c = 2 * a + b;
    Expression d = 1 - b;
    Expression e = log(b + c);
    computeGradients(e);
    std::cout << a.getPartial();
}