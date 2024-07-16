#include <iostream>
#include <fstream>
#include <sstream>
#include "../expression.h"
#include "../context.h"
#include "../network.h"

Engine::Expression mse(const Tensor<Engine::Expression>& Yhat, const Tensor<float>& Y) {
    return Engine::reduceAdd(square(Yhat - Y)) / Y.shape().at({Y.shape().size()-1});
}

int main() {
    using namespace Engine;
    std::cout << "Loading Data...\n";
    std::ifstream train_file("./data/mnist_train.csv");
    std::string s;
    std::getline(train_file, s);

    const std::size_t numDataPoints = 10000;
    std::vector<Tensor<float>> X(numDataPoints, Tensor<float>({1, 784}));
    std::vector<Tensor<float>> Y(numDataPoints, Tensor<float>({1, 10}));
    
    std::size_t i = 0;
    while(std::getline(train_file, s) && i < numDataPoints) {
        std::istringstream line(std::move(s));
        std::string temp;
        std::getline(line, temp, ',');
        std::size_t label = std::stoull(temp);
        for(std::size_t j = 0;j < 10;j++) Y.at(i).at({0, j}) = (j == label) ? 1 : 0;
        auto it = X.at(i).begin();
        while(std::getline(line, temp, ',')) {
            *it = std::stof(temp) / 255.f;
            ++it;
        }
        i++;
    }
    std::cout << "Data Loaded\n";
    std::cout << "Creating Model...\n";
    Network nn(0.1, [](const Tensor<Expression>& Yhat, const Tensor<float>& Y) { return mse(Yhat, Y); });
    nn.addLayer(new Dense(784, 16, [](const Tensor<Expression>& t) { return sigmoid(t); }));
    nn.addLayer(new Dense(16, 16,  [](const Tensor<Expression>& t) { return sigmoid(t); }));
    nn.addLayer(new Dense(16, 10, [](const Tensor<Expression>& t) { return sigmoid(t); }));
    std::cout << "Model Created\n";

    std::cout << "Starting training...\n";
    for(std::size_t i = 0;i < X.size();i++) {
        bool verbose = false;
        if(i % 100 == 0) verbose = true;
        nn.fit(X.at(i), Y.at(i), 1, verbose);
    }
    std::cout << "Training finished";
}

void mnistExample() {
    
}