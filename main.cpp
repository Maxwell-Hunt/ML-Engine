#include <iostream>
#include <vector>
#include "context.h"
#include "tensor.h"

int main() {
    Context context;
    Tensor<Expression> expressions({2, 2});
    // Expression sum = context.add(expressions.at({0, 0}), context.reduceAdd(expressions));

    // float gradient = context.computeGradients(sum, expressions.at({0, 0}));

    // std::cout << sum.getValue() << ' ' << gradient;
}