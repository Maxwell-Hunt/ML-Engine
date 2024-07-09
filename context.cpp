#include "context.h"
#include "variable.h"
#include "operations.h"
#include "tensor.h"
#include "expression.h"
#include <ranges>

// TODO: Turn this class into a collection of functions

namespace Engine {

static std::random_device rd;

Engine::Expression createVariable(float val) {
    Internal::Variable* v = new Internal::Variable(val);
    return Engine::Expression(v);
}

Tensor<Engine::Expression> createZeroTensor(const std::vector<std::size_t>& shape) {
    Tensor<Engine::Expression> t(shape);
    for(Engine::Expression& ex : t) ex = createVariable(0.f);
    return t;
}

Tensor<Engine::Expression> createRandomTensor(const std::vector<std::size_t>& shape) {
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.5f, 1.5f);
    Tensor<Engine::Expression> t(shape);
    for(Engine::Expression& ex : t) ex = createVariable(dis(gen));
    return t;
}

Engine::Expression add(const Engine::Expression& e1, const Engine::Expression& e2) {
    Internal::Addition* sum = new Internal::Addition(e1.getData(), e2.getData());
    return Engine::Expression(sum);
}

Engine::Expression add(float val, const Engine::Expression& e2) {
    Engine::Expression e1 = createVariable(val);
    return add(e1, e2);
}

Engine::Expression add(const Engine::Expression& e1, float val) {
    Engine::Expression e2 = createVariable(val);
    return add(e1, e2);
}

Engine::Expression sub(const Engine::Expression& e1, const Engine::Expression& e2) {
    Internal::Subtraction* diff = new Internal::Subtraction(e1.getData(), e2.getData());
    return Engine::Expression(diff);
}

Engine::Expression sub(float val, const Engine::Expression& e2) {
    Engine::Expression e1 = createVariable(val);
    return sub(e1, e2);
}

Engine::Expression sub(const Engine::Expression& e1, float val) {
    Engine::Expression e2 = createVariable(val);
    return sub(e1, e2);
}

Engine::Expression mult(const Engine::Expression& e1, const Engine::Expression& e2) {
    Internal::Multiplication* prod = new Internal::Multiplication(e1.getData(), e2.getData());
    return Engine::Expression(prod);
}

Engine::Expression mult(float val, const Engine::Expression& e2) {
    Engine::Expression e1 = createVariable(val);
    return mult(e1, e2);
}

Engine::Expression mult(const Engine::Expression& e1, float val) {
    Engine::Expression e2 = createVariable(val);
    return mult(e1, e2);
}

Engine::Expression div(const Engine::Expression& e1, const Engine::Expression& e2) {
    Internal::Division* quotient = new Internal::Division(e1.getData(), e2.getData());
    return Engine::Expression(quotient);
}

Engine::Expression div(float val, const Engine::Expression& e2) {
    Engine::Expression e1 = createVariable(val);
    return div(e1, e2);
}

Engine::Expression div(const Engine::Expression& e1, float val) {
    Engine::Expression e2 = createVariable(val);
    return div(e1, e2);
}

Engine::Expression square(const Engine::Expression& ex) {
    Internal::Square* sq = new Internal::Square(ex.getData());
    return Engine::Expression(sq);
}

Tensor<Engine::Expression> square(const Tensor<Engine::Expression>& t) {
    Tensor<Engine::Expression> result(t.shape());
    auto it1 = result.begin();
    auto it2 = t.begin();
    while(it1 != result.end()) {
        *it1 = square(*it2);
        ++it1;
        ++it2;
    }

    return result;
}

Engine::Expression sigmoid(const Engine::Expression& ex) {
    Internal::Sigmoid* s = new Internal::Sigmoid(ex.getData());
    return Engine::Expression(s);
}

Tensor<Engine::Expression> sigmoid(const Tensor<Engine::Expression>& t) {
    Tensor<Engine::Expression> result(t.shape());
    auto it1 = result.begin();
    auto it2 = t.begin();
    while(it1 != result.end()) {
        *it1 = sigmoid(*it2);
        ++it1;
        ++it2;
    }

    return result;
}

Engine::Expression log(const Engine::Expression& ex) {
    Internal::Log* l = new Internal::Log(ex.getData());
    return Engine::Expression(l);
}

Tensor<Engine::Expression> log(const Tensor<Engine::Expression>& t) {
    Tensor<Engine::Expression> result(t.shape());
    auto it1 = result.begin();
    auto it2 = t.begin();
    while(it1 != result.end()) {
        *it1 = log(*it2);
        ++it1;
        ++it2;
    }

    return result;
}

Engine::Expression reduceAdd(const Tensor<Engine::Expression>& expressions) {
    auto v = expressions.data() | std::views::transform([](const auto& e){ return e.getData(); });
    std::vector<std::shared_ptr<Internal::Expression>> temp(v.begin(), v.end());
    Internal::ReduceAdd* sum = new Internal::ReduceAdd(std::move(temp));
    return Engine::Expression(sum);
}

Tensor<Engine::Expression> matmul(const Tensor<Engine::Expression>& a, const Tensor<Engine::Expression>& b) {
    std::size_t dimsA = a.shape().size();
    std::size_t dimsB = b.shape().size();
    if(dimsA != 2 || dimsB != 2) {
        throw std::runtime_error("Can only perform matmul on 2 dimensional tensors");
    }

    if(a.shape().at(1) != b.shape().at(0)) {
        throw std::runtime_error("Dimensions incorrect for matrix multiplication");
    }
    
    Tensor<Engine::Expression> result({a.shape().at(0), b.shape().at(1)});

    for(std::size_t i = 0;i < a.shape().at(0);i++) {
        for(std::size_t j = 0;j < b.shape().at(1);j++) {
            Tensor<Engine::Expression> temp({a.shape().at(1)});
            for(std::size_t k = 0;k < a.shape().at(1);k++) {
                temp.at({k}) = mult(a.at({i, k}), b.at({k, j}));
            }
            result.at({i, j}) = reduceAdd(temp);
        }
    }

    return result;
}

// This function as well as the next one is complete garbage.  It should be templated like the other operations
Tensor<Engine::Expression> matmul(const Tensor<Engine::Expression>& a, const Tensor<float>& b) {
    Tensor<Engine::Expression> B(b.shape());
    for(std::size_t i = 0;i < b.shape().at(0);i++) {
        for(std::size_t j = 0;j < b.shape().at(1);j++){
            B.at({i, j}) = createVariable(b.at({i, j}));
        } 
    }

    return matmul(a, B);
}

// This function as well as the previous one is complete garbage. It should be templated like the other operations
Tensor<Engine::Expression> matmul(const Tensor<float>& a, const Tensor<Engine::Expression>& b) {
    Tensor<Engine::Expression> A(a.shape());
    for(std::size_t i = 0;i < a.shape().at(0);i++) {
        for(std::size_t j = 0;j < a.shape().at(1);j++) {
            A.at({i, j}) = createVariable(a.at({i, j}));
        }
    }

    return matmul(A, b);
}

void computeGradients(const Engine::Expression& target) {
    target.getData()->backPropagate();
}

Engine::Expression operator+(const Engine::Expression& e1, const Engine::Expression& e2) {
    return Engine::add(e1, e2);
}

Engine::Expression operator+(const Engine::Expression& e1, float val) {
    return Engine::add(e1, val);
}

Engine::Expression operator+(float val, const Engine::Expression& e2) {
    return Engine::add(val, e2);
} 

Engine::Expression operator-(const Engine::Expression& e1, const Engine::Expression& e2) {
    return Engine::sub(e1, e2);
}

Engine::Expression operator-(const Engine::Expression& e1, float val) {
    return Engine::sub(e1, val);
}

Engine::Expression operator-(float val, const Engine::Expression& e2) {
    return Engine::sub(val, e2);
} 

void operator+=(Engine::Expression& e1, const Engine::Expression& e2) {
    e1 = e1 + e2;
}

void operator-=(Engine::Expression& e1, const Engine::Expression& e2) {
    e1 = e1 - e2;
}

Engine::Expression operator*(const Engine::Expression& e1, const Engine::Expression& e2) {
    return Engine::mult(e1, e2);
}

Engine::Expression operator*(const Engine::Expression& e1, float val) {
    return Engine::mult(e1, val);
}

Engine::Expression operator*(float val, const Engine::Expression& e2) {
    return Engine::mult(val, e2);
}

Engine::Expression operator/(const Engine::Expression& e1, const Engine::Expression& e2) {
    return div(e1, e2);
}

Engine::Expression operator/(const Engine::Expression& e1, float val) {
    return div(e1, val);
}

Engine::Expression operator/(float val, const Engine::Expression& e2) {
    return div(val, e2);
}

}