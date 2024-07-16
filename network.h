#ifndef __NETWORK__
#define __NETWORK__

#include "model.h"
#include "layers.h"
#include <iostream>

template <typename ErrorFunction>
class Network : public Model {
public:
    Network(float learningRate, ErrorFunction loss_function);
    void addLayer(Layer* layer);
    virtual Tensor<Engine::Expression> operator()(const Tensor<float>& X) const override;
    virtual void fit(const Tensor<float>& X, const Tensor<float>& Y, std::size_t iterations, bool verbose) const override;
private:
    std::vector<std::unique_ptr<Layer>> layers;
    float learningRate;
    ErrorFunction loss_function;
};

template <typename ErrorFunction>
Network<ErrorFunction>::Network(float learningRate, ErrorFunction loss_function) : 
    learningRate{learningRate},
    loss_function{loss_function} {}

template <typename ErrorFunction>
void Network<ErrorFunction>::addLayer(Layer* layer) { layers.push_back(std::unique_ptr<Layer>(layer)); }

template <typename ErrorFunction>
Tensor<Engine::Expression> Network<ErrorFunction>::operator()(const Tensor<float>& X) const {
    Tensor<Engine::Expression> output = layers.at(0)->operator()(X);
    for(std::size_t i = 1;i < layers.size();i++) {
        output = layers.at(i)->operator()(output);
    }
    return output;
}

template <typename ErrorFunction>
void Network<ErrorFunction>::fit(const Tensor<float>& X, const Tensor<float>& Y, std::size_t iterations, bool verbose) const {
    for(std::size_t iteration = 1;iteration <= iterations;iteration++) {
        Tensor<Engine::Expression> output = operator()(X);
        Engine::Expression error = loss_function(output, Y);
        if(verbose) {
            std::cout << "Iteration: " << iteration << " Error: " << error.getValue() << '\n';
        }
        Engine::computeGradients(error);
        for(auto& layer : layers) layer->updateValues(learningRate);
    }
}

#endif