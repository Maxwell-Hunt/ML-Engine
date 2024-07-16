#include <iostream>
#include <fstream>
#include <sstream>
#include "../expression.h"
#include "../context.h"
#include "../network.h"

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

int main() {
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

        for(Expression& ex : w1) ex = createVariable(ex.getValue() - alpha * ex.getPartial());
        for(Expression& ex : w2) ex = createVariable(ex.getValue() - alpha * ex.getPartial());
    }
}