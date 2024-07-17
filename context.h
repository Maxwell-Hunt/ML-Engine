#ifndef __CONTEXT__
#define __CONTEXT__

#include <unordered_set>
#include <random>
#include <stdexcept>
#include "tensor.h"

namespace Engine {
    class Expression;

    Engine::Expression createVariable(float val);
    Tensor<Engine::Expression> createZeroTensor(const std::vector<std::size_t>& shape);
    Tensor<Engine::Expression> createRandomTensor(const std::vector<std::size_t>& shape, float radius=1);

    Engine::Expression add(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression add(float val, const Engine::Expression& e2);
    Engine::Expression add(const Engine::Expression& e1, float val);

    Engine::Expression sub(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression sub(float val, const Engine::Expression& e2);
    Engine::Expression sub(const Engine::Expression& e1, float val);

    Engine::Expression mult(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression mult(float val, const Engine::Expression& e2);
    Engine::Expression mult(const Engine::Expression& e1, float val);

    Engine::Expression div(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression div(float val, const Engine::Expression& e2);
    Engine::Expression div(const Engine::Expression& e1, float val);

    Engine::Expression square(const Engine::Expression& ex);
    Tensor<Engine::Expression> square(const Tensor<Engine::Expression>& t);

    Engine::Expression sigmoid(const Engine::Expression& ex);
    Tensor<Engine::Expression> sigmoid(const Tensor<Engine::Expression>& t);

    Engine::Expression relu(const Engine::Expression& ex);
    Tensor<Engine::Expression> relu(const Tensor<Engine::Expression>& t);

    Tensor<Engine::Expression> softmax(const Tensor<Engine::Expression>& t);

    Engine::Expression exp(const Engine::Expression& ex);
    Tensor<Engine::Expression> exp(const Tensor<Engine::Expression>& t);

    Engine::Expression log(const Engine::Expression& ex);
    Tensor<Engine::Expression> log(const Tensor<Engine::Expression>& t);

    Engine::Expression reduceAdd(const Tensor<Engine::Expression>& expressions);

    Tensor<Engine::Expression> matmul(const Tensor<Engine::Expression>& a, const Tensor<Engine::Expression>& b);
    Tensor<Engine::Expression> matmul(const Tensor<Engine::Expression>& a, const Tensor<float>& b);
    Tensor<Engine::Expression> matmul(const Tensor<float>& a, const Tensor<Engine::Expression>& b);

    void computeGradients(const Engine::Expression& target);

    Engine::Expression operator+(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression operator+(const Engine::Expression& e1, float val);
    Engine::Expression operator+(float val, const Engine::Expression& e2);

    Engine::Expression operator-(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression operator-(const Engine::Expression& e1, float val);
    Engine::Expression operator-(float val, const Engine::Expression& e2);

    void operator+=(Engine::Expression& e1, const Engine::Expression& e2);
    void operator-=(Engine::Expression& e1, const Engine::Expression& e2);

    Engine::Expression operator*(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression operator*(const Engine::Expression& e1, float val);
    Engine::Expression operator*(float val, const Engine::Expression& e2);

    Engine::Expression operator/(const Engine::Expression& e1, const Engine::Expression& e2);
    Engine::Expression operator/(const Engine::Expression& e1, float val);
    Engine::Expression operator/(float val, const Engine::Expression& e2);
}



#endif