#ifndef __DENSE__
#define __DENSE__

#include "model.h"
#include <cmath>

class Layer {
public:
    virtual Tensor<Engine::Expression> operator()(const Tensor<float>& X) const = 0;
    virtual Tensor<Engine::Expression> operator()(const Tensor<Engine::Expression>& X) const = 0; 
    virtual void updateValues(float learningRate) = 0;
};

template <typename Callable>
class Dense : public Layer {
public:
    Dense(std::size_t input_size, std::size_t output_size, Callable activation);
    virtual Tensor<Engine::Expression> operator()(const Tensor<float>& X) const;
    virtual Tensor<Engine::Expression> operator()(const Tensor<Engine::Expression>& X) const;
    virtual void updateValues(float learningRate) override;
private:
    Tensor<Engine::Expression> w;
    Tensor<Engine::Expression> b;
    Callable activation;
};

template <typename Callable>
Dense<Callable>::Dense(std::size_t input_size, std::size_t output_size, Callable activation) : 
    // Glorot Uniform Initialization
    w{Engine::createRandomTensor({input_size, output_size}, std::sqrt(6 / (input_size + output_size)))}, 
    b{Engine::createZeroTensor({output_size})}, 
    activation{activation} {}

template <typename Callable>
Tensor<Engine::Expression> Dense<Callable>::operator()(const Tensor<float>& X) const {
    // return activation(Engine::matmul(X, w));
    return activation(Engine::matmul(X, w) + b);
}

template <typename Callable>
Tensor<Engine::Expression> Dense<Callable>::operator()(const Tensor<Engine::Expression>& X) const {
    // return activation(Engine::matmul(X, w));
    return activation(Engine::matmul(X, w) + b);
}

template <typename Callable>
void Dense<Callable>::updateValues(float learningRate) {
    for(auto& weight : w) { 
        weight = Engine::createVariable(weight.getValue() - weight.getPartial() * learningRate);
    }
}

#endif