#include "../context.h"
#include <iostream>
#include <vector>

int main() {
    auto v = createRandomVariable<2, 2>();
    auto w = createVariable<2, 2>();

    auto y = square(v);

    computeGradients(y);

    std::cout << v.partials().at({0, 0}) << ' ' << v.partials().at({1, 1}) << std::endl;
    std::cout << y.value().at({0, 0}) << ' ' << y.value().at({1, 1});
}