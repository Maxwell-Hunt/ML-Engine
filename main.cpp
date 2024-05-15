#include <iostream>
#include "context.h"

int main() {
    Context context;

    Expression x = context.createVariable(10);
    Expression y = context.createVariable(5);

    Expression quot = context.div(x, y);
    quot = context.div(quot, 10);
    quot = context.div(1, quot);

    quot = context.sub(quot, 100);

    float grad = context.computeGradients(quot, x);

    std::cout << grad;
}