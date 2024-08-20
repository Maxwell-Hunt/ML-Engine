#ifndef __CONTEXT__
#define __CONTEXT__

#include "operations.h"
#include <random>

template <std::size_t ...Dims>
Variable<Tensor<float, Dims...>> createTensorVariable() {
    Tensor<float, Dims...> t = 0;
    return Variable(new ConstExpression(std::move(t)));
}

template <std::size_t ...Dims>
Variable<Tensor<float, Dims...>> createRandomTensorVariable() {
    Tensor<float, Dims...> t;
    static std::random_device rd;
    static std::mt19937 gen;
    std::uniform_real_distribution dis(-1.0, 1.0);
    std::generate(t.begin(), t.end(), [&]() { return dis(gen); });
    return Variable(new ConstExpression(std::move(t)));
}

template <typename T>
void computeGradients(const Variable<T>& ex) {
    ex.get()->backPropagate();
}

template <typename T>
Variable<T> square(const Variable<T>& v) {
    return Variable(new Square(v.get()));
}

template <typename T>
Variable<T> exp(const Variable<T>& v) {
    return Variable(new Exp(v.get()));
}

template <typename T, typename H>
Variable<T> operator+(const Variable<T>& a, const Variable<H>& b) {
    return Variable(new Addition<T, H>(a.get(), b.get()));
}

template <typename T, typename H>
Variable<T> operator-(const Variable<T>& a, const Variable<H>& b) {
    return Variable(new Subtraction<T, H>(a.get(), b.get()));
}

template <typename T, typename H>
Variable<T> operator*(const Variable<T>& a, const Variable<H>& b) {
    return Variable(new Multiplication<T, H>(a.get(), b.get()));
}

template <typename T, typename H>
Variable<T> operator/(const Variable<T>& a, const Variable<H>& b) {
    return Variable(new Division<T, H>(a.get(), b.get()));
}
#endif