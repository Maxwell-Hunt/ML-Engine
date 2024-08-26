#include <iostream>
#include <fstream>
#include <sstream>
#include "../context.h"

std::pair<Tensor<float, 9>, Tensor<float, 9>> generateData() {

    Tensor<float, 9> X;
    Tensor<float, 9> Y;
    for(std::size_t i = 0;i < 9;i++) {
        X.at({i}) = -5.f  + (float)i * 3.f / 9.f;
        Y.at({i}) = X.at({i}) * 2;
    }

    return {X, Y};
}

int main() {
    const auto& [X, Y] = generateData();
    
    Variable w = createVariable(1.f);
    float alpha = 0.01;

    const unsigned int NUM_ITERATIONS = 25;

    for(unsigned int iteration = 1;iteration <= NUM_ITERATIONS;iteration++) {
        Variable result = X * w;
        Variable squaredError = square(result - Y);
        Variable mse = reduceAdd(squaredError) / 9.f;

        computeGradients(mse);
        // for(float item : result.partials()) {
        //     std::cout << item << ' ';
        // }
        // std::cout << std::endl;
        std::cout << "W: " << w.value() << " MSE: " << mse.value() << " Gradient: " << w.partials() << std::endl;

        
        float gradient = w.partials();
        w = createVariable(w.value() - alpha * gradient);
    }
}

// int main() {
//     Variable x = createMatrixVariable<2, 2>({1, 2, 3, 4});
//     Matrix<float, 2, 2> y({1, 2, 3, 4});

//     Variable z = matmul(x, y);

//     for(float item : z.value()) std::cout << item << ' ';
// }