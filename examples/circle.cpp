#include <iostream>
#include "../expression.h"
#include "../context.h"
#include "../network.h"

// Generates data in the range (-1, 1) and labels it based on whether it is inside of the 
std::pair<Tensor<float>, Tensor<float>> circleData(std::size_t data_size) {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution dist(-1.f, 1.f);

    const float R = 0.8;
    Tensor<float> X({data_size, 2});
    Tensor<float> Y({data_size, 1});

    for(std::size_t i = 0;i < data_size;i++) {
        float x = dist(e2);
        float y = dist(e2);
        X.at({i, 0}) = x;
        X.at({i, 1}) = y;
        Y.at({i, 0}) = (float)((x*x + y*y < R*R));
    }

    return {X, Y};
}

Engine::Expression binarycrossentropy(const Tensor<Engine::Expression>& Yhat, const Tensor<float>& Y) {
    using namespace Engine;
    float size = Y.shape().at(0);
    return (-1.0 / size) * reduceAdd(Y * log(Yhat) + (1 - Y) * log(1 - Yhat));
}

Engine::Expression mse(const Tensor<Engine::Expression>& Yhat, const Tensor<float>& Y) {
    return Engine::reduceAdd(square(Yhat - Y)) / Y.shape().at({Y.shape().size()-1});
}

int main() {
    using namespace Engine;
    const std::size_t data_size = 100;
    
    const auto& [X, Y] = circleData(data_size);

    Network nn(0.1, [](const Tensor<Expression>& Yhat, const Tensor<float>& Y) { return binarycrossentropy(Yhat, Y); });
    nn.addLayer(new Dense(2, 16, [](const Tensor<Expression>& t) { return sigmoid(t); }));
    nn.addLayer(new Dense(16, 1, [](const Tensor<Expression>& t) { return sigmoid(t); }));

    auto Yhat = nn(X);  
    float accuracy = 0;
    for(std::size_t i = 0;i < data_size;i++) {
        accuracy += (std::abs(std::round(Yhat.at({i, 0}).getValue()) - Y.at({i, 0})) < 0.1);
    }
    accuracy /= (float)data_size;
    std::cout << "Accuracy: " << accuracy << std::endl;

    nn.fit(X, Y, 200, true);

    const auto& [Xn, Yn] = circleData(data_size);
    auto Yhatn = nn(Xn);  
    accuracy = 0;
    for(std::size_t i = 0;i < data_size;i++) {
        accuracy += (std::abs(std::round(Yhatn.at({i, 0}).getValue()) - Yn.at({i, 0})) < 0.1);
    }
    accuracy /= (float)data_size;
    std::cout << "Accuracy: " << accuracy;
}
