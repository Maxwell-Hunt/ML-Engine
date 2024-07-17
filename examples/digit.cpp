#include <iostream>
#include <fstream>
#include <sstream>
#include "../expression.h"
#include "../context.h"
#include "../network.h"

std::pair<std::vector<Tensor<float>>, std::vector<Tensor<float>>> loadData(const std::string& path, std::size_t data_size) {
    std::ifstream train_file(path);
    std::string s;
    std::getline(train_file, s);

    std::vector<Tensor<float>> X(data_size, Tensor<float>({1, 784}));
    std::vector<Tensor<float>> Y(data_size, Tensor<float>({1, 10}));
    
    std::size_t i = 0;
    while(std::getline(train_file, s) && i < data_size) {
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

    return {X, Y};
}

Engine::Expression mse(const Tensor<Engine::Expression>& Yhat, const Tensor<float>& Y) {
    return Engine::reduceAdd(square(Yhat - Y)) / Y.shape().at({Y.shape().size()-1});
}

// Standard MNIST Handwritten Digit Example
// Data available here in csv format
// https://www.kaggle.com/datasets/oddrationale/mnist-in-csv
// Achieves results on par with keras given the same settings
int main() {
    using namespace Engine;
    
    std::cout << "Loading Data...\n";
    const auto& [X, Y] = loadData("../data/mnist_train.csv", 10000);
    std::cout << "Finished Loading\n";

    std::cout << "Creating Model...\n";
    Network nn(0.005, [](const Tensor<Expression>& Yhat, const Tensor<float>& Y) { return mse(Yhat, Y); });
    nn.addLayer(new Dense(784, 16, [](const Tensor<Expression>& t) { return relu(t); }));
    nn.addLayer(new Dense(16, 16,  [](const Tensor<Expression>& t) { return relu(t); }));
    nn.addLayer(new Dense(16, 10, [](const Tensor<Expression>& t) { return softmax(t); }));
    std::cout << "Model Created\n";

    std::cout << "Starting training...\n";
    for(std::size_t i = 0;i < X.size();i++) {
        bool verbose = false;
        if(i % 100 == 0) verbose = true;
        nn.fit(X.at(i), Y.at(i), 1, verbose);
    }
    std::cout << "Training finished\n";

    const std::size_t num_test_samples = 1000;
    const auto& [Xtest, Ytest] = loadData("../data/mnist_test.csv", num_test_samples);
    float accuracy = 0;
    for(std::size_t i = 0;i < num_test_samples;i++) {
        auto Yhat = nn(X.at(i));
        float mPred = 0;
        float mReal = 0;
        std::size_t argMaxPred = 0;
        std::size_t argMaxReal = 0;
        for(std::size_t j = 1;j < 10;j++) {
            if(Yhat.at({0, j}).getValue() > mPred) { mPred = Yhat.at({0, j}).getValue(); argMaxPred = j; }
            if(Y.at(i).at({0, j}) > mReal) { mReal = Y.at(i).at({0, j}); argMaxReal = j; }
        }
        accuracy += (float)(argMaxPred == argMaxReal);
    }
    accuracy /= num_test_samples;
    std::cout << "Accuracy: " << accuracy;
}