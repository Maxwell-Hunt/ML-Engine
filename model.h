#ifndef __MODEL__
#define __MODEL__

#include "expression.h"
#include "tensor.h"

class Model {
public:
    virtual Tensor<Engine::Expression> operator()(const Tensor<float>& X) const = 0;
    virtual void fit(const Tensor<float>& X, const Tensor<float>& Y, std::size_t iterations) const = 0;
};

#endif