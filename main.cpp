#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "expression.h"
#include "context.h"

template <typename T>
void printTensor(const Tensor<T>& tensor) {
    for(const T& item : tensor) std::cout << item << ' ';
}

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

std::pair<Tensor<float>, Tensor<float>> generate2D(std::size_t size) {
    Tensor<float> X({size, 2});
    Tensor<float> Y({size, 1});

    std::size_t side_length = static_cast<std::size_t>(std::sqrt(size));
    std::size_t i = 0;

    float step = side_length >= 1 ? 1.f / (float)(side_length - 1) : 1.f;

    for(std::size_t row = 0;row < side_length;row++) {
        for(std::size_t col = 0;col < side_length;col++) {
            float y = row * step;
            float x = col * step;
            X.at({i, 0}) = x;
            X.at({i, 1}) = y;
            Y.at({i, 0}) = (float)(X.at({i, 1}) > X.at({i, 0}));
            i++;
        }
    }

    return {X, Y};
}

void TensorExample() {
    using namespace Engine;

    const float alpha = 1;
    const std::size_t DATA_SIZE = 36;
    const auto& [X, Y] = generate2D(DATA_SIZE);

    //for(float f : Y) std::cout << f << std::endl;

    Tensor<Expression> w1({2, 20});
    Tensor<Expression> w2({20, 1});
    for(Expression& ex : w1) ex = createVariable(0.5f);
    for(Expression& ex : w2) ex = createVariable(0.5f);

    const std::size_t NUM_ITERS = 100;

    for(std::size_t iteration = 1;iteration <= NUM_ITERS;iteration++) {
        Tensor<Expression> hidden = sigmoid(matmul(X, w1));
        Tensor<Expression> output = sigmoid(matmul(hidden, w2));
        // IN PYTHON: errors = Y * tf.math.log(output) + (1 - Y) * tf.math.log(1 - output)

        Tensor<Expression> errors = Y * log(output) + (1.f - Y) * log(1.f - output);
        //for(Expression& ex : errors) std::cout << ex.getValue() << std::endl;
        Expression error = (-1.f / DATA_SIZE) * reduceAdd(errors);

        float accuracy = 0;
        for(std::size_t i = 0;i < DATA_SIZE;i++) {
            if(std::abs(std::round(output.at({i, 0}).getValue()) - Y.at({i, 0})) < 0.1) accuracy += 1.f / (float)DATA_SIZE;
        }

        std::cout << "Iteration: " << iteration << " ERROR: " << error.getValue() << " ACCURACY: " << accuracy << std::endl;

        computeGradients(error);

        // for(Expression& ex : w1) std::cout << ex.getPartial() << std::endl;
        // std::cout << std::endl;
        // for(Expression& ex : w2) std::cout << ex.getPartial() << std::endl;
        // std::cout << std::endl;
        // for(Expression& ex : hidden) std::cout << ex.getPartial() << std::endl;
        // std::cout << std::endl;
        // for(Expression& ex : output) std::cout << ex.getPartial() << std::endl;

        for(Expression& ex : w1) ex = createVariable(ex.getValue() - alpha * ex.getPartial());
        for(Expression& ex : w2) ex = createVariable(ex.getValue() - alpha * ex.getPartial());
    }
}

void MatrixExample() {
    using namespace Engine;
    Tensor<float> F({2, 2});
    Tensor<Expression> A({2, 2});
    Tensor<Expression> B({2, 1});

    F.at({0, 0}) = 0.05;
    F.at({0, 1}) = 0.07;
    F.at({0, 2}) = 0.1;
    F.at({0, 3}) = 3;

    A.at({0, 0}) = createVariable(0.0013);
    A.at({0, 1}) = createVariable(0.002);
    A.at({1, 0}) = createVariable(0.0018);
    A.at({1, 1}) = createVariable(0.005);

    B.at({0, 0}) = createVariable(0.007);
    B.at({1, 0}) = createVariable(0.0011);

    Tensor<Expression> C = sigmoid(matmul(matmul(F, A), B));
    Tensor<Expression> D = 1 * log(1-C);

    Expression result = (1.f/5.f) * reduceAdd(D);
    computeGradients(result);

    for(Expression& ex : A) std::cout << ex.getPartial() << std::endl;
    std::cout << std::endl;
    for(Expression& ex : B) std::cout << ex.getPartial() << std::endl;
}


void simpleExample() {
    using namespace Engine;
    Expression f = createVariable(5.f);
    Expression a = log(f);
    Expression b = square(a);
    Expression c = log(a);
    Expression d = mult(b, c);
    Expression e = square(d);
    computeGradients(e);
    std::cout << f.getPartial();
}


// Corresponds to linear.py in test files
void linearExample() {
    using namespace Engine;
    const std::size_t DATA_SIZE = 100;
    const auto& [X, Y] = generateData(DATA_SIZE);
    

    Expression w = createVariable(1.f);
    float alpha = 0.01;

    const unsigned int NUM_ITERATIONS = 25;

    for(std::size_t iteration = 1;iteration <= NUM_ITERATIONS;iteration++) {
        Tensor<Expression> result = X * w;
        Tensor<Expression> squaredError = square(result - Y);
        Expression mse = reduceAdd(squaredError) / (float)DATA_SIZE;

         std::cout << "W: " << w.getValue() << " MSE: " << mse.getValue() /*<< " Gradient: " << gradient*/ << std::endl;

        computeGradients(mse);
        float gradient = w.getPartial();
        w = createVariable(w.getValue() - alpha * gradient);
    }
}

int main() {
    TensorExample();
}   
