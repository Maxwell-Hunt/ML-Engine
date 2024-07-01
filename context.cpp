#include "context.h"
#include "variable.h"
#include "operations.h"
#include "tensor.h"
#include <ranges>

// TODO: Turn this class into a collection of functions

Expression Context::createVariable(float val) {
    Variable* v = new Variable(val, *this);
    return Expression(v);
}

Tensor<Expression> Context::createZeroTensor(const std::vector<std::size_t>& shape) {
    Tensor<Expression> t(shape);
    for(Expression& ex : t) ex = createVariable(0.f);
    return t;
}

Tensor<Expression> Context::createRandomTensor(const std::vector<std::size_t>& shape) {
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.5f, 1.5f);
    Tensor<Expression> t(shape);
    for(Expression& ex : t) ex = createVariable(dis(gen));
    return t;
}

Expression Context::add(const Expression& e1, const Expression& e2) {
    Addition* sum = new Addition(e1.getData(), e2.getData(), *this);
    return Expression(sum);
}

Expression Context::add(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return add(e1, e2);
}

Expression Context::add(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return add(e1, e2);
}

Expression Context::sub(const Expression& e1, const Expression& e2) {
    Subtraction* diff = new Subtraction(e1.getData(), e2.getData(), *this);
    return Expression(diff);
}

Expression Context::sub(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return sub(e1, e2);
}

Expression Context::sub(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return sub(e1, e2);
}

Expression Context::mult(const Expression& e1, const Expression& e2) {
    Multiplication* prod = new Multiplication(e1.getData(), e2.getData(), *this);
    return Expression(prod);
}

Expression Context::mult(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return mult(e1, e2);
}

Expression Context::mult(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return mult(e1, e2);
}

Expression Context::div(const Expression& e1, const Expression& e2) {
    Division* quotient = new Division(e1.getData(), e2.getData(), *this);
    return Expression(quotient);
}

Expression Context::div(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return div(e1, e2);
}

Expression Context::div(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return div(e1, e2);
}

Expression Context::square(const Expression& ex) {
    Square* sq = new Square(ex.getData(), *this);
    return Expression(sq);
}

Tensor<Expression> Context::square(const Tensor<Expression>& t) {
    Tensor<Expression> result(t.shape());
    auto it1 = result.begin();
    auto it2 = t.begin();
    while(it1 != result.end()) {
        *it1 = square(*it2);
        ++it1;
        ++it2;
    }

    return result;
}

Expression Context::sigmoid(const Expression& ex) {
    Sigmoid* s = new Sigmoid(ex.getData(), *this);
    return Expression(s);
}

Tensor<Expression> Context::sigmoid(const Tensor<Expression>& t) {
    Tensor<Expression> result(t.shape());
    auto it1 = result.begin();
    auto it2 = t.begin();
    while(it1 != result.end()) {
        *it1 = sigmoid(*it2);
        ++it1;
        ++it2;
    }

    return result;
}

Expression Context::log(const Expression& ex) {
    Log* l = new Log(ex.getData(), *this);
    return Expression(l);
}

Tensor<Expression> Context::log(const Tensor<Expression>& t) {
    Tensor<Expression> result(t.shape());
    auto it1 = result.begin();
    auto it2 = t.begin();
    while(it1 != result.end()) {
        *it1 = log(*it2);
        ++it1;
        ++it2;
    }

    return result;
}

Expression Context::reduceAdd(const Tensor<Expression>& expressions) {
    auto v = expressions.data() | std::views::transform([](const auto& e){ return e.getData(); });
    std::vector<std::shared_ptr<Internal::Expression>> temp(v.begin(), v.end());
    ReduceAdd* sum = new ReduceAdd(std::move(temp), *this);
    return Expression(sum);
}

Tensor<Expression> Context::matmul(const Tensor<Expression>& a, const Tensor<Expression>& b) {
    std::size_t dimsA = a.shape().size();
    std::size_t dimsB = b.shape().size();
    if(dimsA != 2 || dimsB != 2) {
        throw std::runtime_error("Can only perform matmul on 2 dimensional tensors");
    }

    if(a.shape().at(1) != b.shape().at(0)) {
        throw std::runtime_error("Dimensions incorrect for matrix multiplication");
    }
    
    Tensor<Expression> result({a.shape().at(0), b.shape().at(1)});

    for(std::size_t i = 0;i < a.shape().at(0);i++) {
        for(std::size_t j = 0;j < b.shape().at(1);j++) {
            Tensor<Expression> temp({a.shape().at(1)});
            for(std::size_t k = 0;k < a.shape().at(1);k++) {
                temp.at({k}) = mult(a.at({i, k}), b.at({k, j}));
            }
            result.at({i, j}) = reduceAdd(temp);
        }
    }

    return result;
}

// This function as well as the next one is complete garbage.  It should be templated like the other operations
Tensor<Expression> Context::matmul(const Tensor<Expression>& a, const Tensor<float>& b) {
    Tensor<Expression> B(b.shape());
    for(std::size_t i = 0;i < b.shape().at(0);i++) {
        for(std::size_t j = 0;j < b.shape().at(1);j++){
            B.at({i, j}) = createVariable(b.at({i, j}));
        } 
    }

    return matmul(a, B);
}

// This function as well as the previous one is complete garbage. It should be templated like the other operations
Tensor<Expression> Context::matmul(const Tensor<float>& a, const Tensor<Expression>& b) {
    Tensor<Expression> A(a.shape());
    for(std::size_t i = 0;i < a.shape().at(0);i++) {
        for(std::size_t j = 0;j < a.shape().at(1);j++) {
            A.at({i, j}) = createVariable(a.at({i, j}));
        }
    }

    return matmul(A, b);
}

void Context::computeGradients(const Expression& target) {
    target.getData()->backPropagate();
}

Expression operator+(const Expression& e1, const Expression& e2) {
    return e1.getContext().add(e1, e2);
}

Expression operator+(const Expression& e1, float val) {
    return e1.getContext().add(e1, val);
}

Expression operator+(float val, const Expression& e2) {
    return e2.getContext().add(val, e2);
} 

Expression operator-(const Expression& e1, const Expression& e2) {
    return e1.getContext().sub(e1, e2);
}

Expression operator-(const Expression& e1, float val) {
    return e1.getContext().sub(e1, val);
}

Expression operator-(float val, const Expression& e2) {
    return e2.getContext().sub(val, e2);
} 

void operator+=(Expression& e1, const Expression& e2) {
    e1 = e1 + e2;
}

void operator-=(Expression& e1, const Expression& e2) {
    e1 = e1 - e2;
}

Expression operator*(const Expression& e1, const Expression& e2) {
    return e1.getContext().mult(e1, e2);
}

Expression operator*(const Expression& e1, float val) {
    return e1.getContext().mult(e1, val);
}

Expression operator*(float val, const Expression& e2) {
    return e2.getContext().mult(val, e2);
}

Expression operator/(const Expression& e1, const Expression& e2) {
    return e1.getContext().div(e1, e2);
}

Expression operator/(const Expression& e1, float val) {
    return e1.getContext().div(e1, val);
}

Expression operator/(float val, const Expression& e2) {
    return e2.getContext().div(val, e2);
}