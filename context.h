#ifndef __CONTEXT__
#define __CONTEXT__

#include <unordered_set>
#include <random>
#include <stdexcept>
#include "expression.h"
#include "tensor.h"

class Context {
public:
    Expression createVariable(float val);
    Tensor<Expression> createZeroTensor(const std::vector<std::size_t>& shape);
    Tensor<Expression> createRandomTensor(const std::vector<std::size_t>& shape);

    Expression add(const Expression& e1, const Expression& e2);
    Expression add(float val, const Expression& e2);
    Expression add(const Expression& e1, float val);

    Expression sub(const Expression& e1, const Expression& e2);
    Expression sub(float val, const Expression& e2);
    Expression sub(const Expression& e1, float val);

    Expression mult(const Expression& e1, const Expression& e2);
    Expression mult(float val, const Expression& e2);
    Expression mult(const Expression& e1, float val);

    Expression div(const Expression& e1, const Expression& e2);
    Expression div(float val, const Expression& e2);
    Expression div(const Expression& e1, float val);

    Expression square(const Expression& ex);
    Tensor<Expression> square(const Tensor<Expression>& t);

    Expression sigmoid(const Expression& ex);
    Tensor<Expression> sigmoid(const Tensor<Expression>& t);

    Expression log(const Expression& ex);
    Tensor<Expression> log(const Tensor<Expression>& t);

    Expression reduceAdd(const Tensor<Expression>& expressions);


    Tensor<Expression> matmul(const Tensor<Expression>& a, const Tensor<Expression>& b);
    Tensor<Expression> matmul(const Tensor<Expression>& a, const Tensor<float>& b);
    Tensor<Expression> matmul(const Tensor<float>& a, const Tensor<Expression>& b);

    void computeGradients(const Expression& target);
private:
    std::random_device rd;
};


Expression operator+(const Expression& e1, const Expression& e2);
Expression operator+(const Expression& e1, float val);
Expression operator+(float val, const Expression& e2);

Expression operator-(const Expression& e1, const Expression& e2);
Expression operator-(const Expression& e1, float val);
Expression operator-(float val, const Expression& e2);

void operator+=(Expression& e1, const Expression& e2);
void operator-=(Expression& e1, const Expression& e2);

Expression operator*(const Expression& e1, const Expression& e2);
Expression operator*(const Expression& e1, float val);
Expression operator*(float val, const Expression& e2);

Expression operator/(const Expression& e1, const Expression& e2);
Expression operator/(const Expression& e1, float val);
Expression operator/(float val, const Expression& e2);

#endif