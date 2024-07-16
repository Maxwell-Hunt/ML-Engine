#include <iostream>
#include <fstream>
#include <sstream>
#include "../expression.h"
#include "../context.h"

std::pair<Tensor<float>, Tensor<float>> generateData(std::size_t size) {

    Tensor<float> X({size});
    Tensor<float> Y({size});
    for(std::size_t i = 0;i < size;i++) {
        X.at({i}) = -5.f  + (float)i * 10.f / (float)size;
        Y.at({i}) = X.at({i}) * 2;
    }

    return {X, Y};
}

std::pair<Tensor<float>, Tensor<float>> generate2DRandom(std::size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(0.f, 1.f);

    Tensor<float> X({size, 2});
    Tensor<float> Y({size, 1});

    for(std::size_t i = 0;i < size;i++) {
        X.at({i, 0}) = dis(gen);
        X.at({i, 1}) = dis(gen);
        // Are we above the line y = x?
        Y.at({i, 0}) = (float)(X.at({i, 1}) > X.at({i, 0}));
    }

    return {X, Y};
}

// Corresponds to linear.py in test files
int main() {
    using namespace Engine;
    const std::size_t DATA_SIZE = 100;
    const auto& [X, Y] = generateData(DATA_SIZE);
    
    Expression w = createVariable(1.f);
    float alpha = 0.01;

    const unsigned int NUM_ITERATIONS = 25;

    for(unsigned int iteration = 1;iteration <= NUM_ITERATIONS;iteration++) {
        Tensor<Expression> result = X * w;
        Tensor<Expression> squaredError = square(result - Y);
        Expression mse = reduceAdd(squaredError) / (float)DATA_SIZE;

         std::cout << "W: " << w.getValue() << " MSE: " << mse.getValue() << std::endl;

        computeGradients(mse);
        float gradient = w.getPartial();
        w = createVariable(w.getValue() - alpha * gradient);
    }
}