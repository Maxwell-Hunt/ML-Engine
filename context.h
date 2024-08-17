#ifndef __CONTEXT__
#define __CONTEXT__

#include "operations.h"
#include <random>

template <std::size_t ...Dims>
Variable<float, Dims...> createVariable() {
    Tensor<float, Dims...> t;
    std::fill(t.begin(), t.end(), 0);
    return Variable(new ConstExpression(std::move(t)));
}

template <std::size_t ...Dims>
Variable<float, Dims...> createRandomVariable() {
    Tensor<float, Dims...> t;
    static std::random_device rd;
    static std::mt19937 gen;
    std::uniform_real_distribution dis(-1.0, 1.0);
    std::generate(t.begin(), t.end(), [&]() { return dis(gen); });
    return Variable(new ConstExpression(std::move(t)));
}

template <Floating T, std::size_t ...Dims>
void computeGradients(const Variable<T, Dims...>& ex) {
    ex.get()->backPropagate();
}

template <Floating T, std::size_t ...Dims>
Variable<T, Dims...> square(const Variable<T, Dims...>& v) {
    return Variable(new Square(v.get()));
}

template <Floating T, std::size_t ...Dims>
Variable<T, Dims...> exp(const Variable<T, Dims...>& v) {
    return Variable(new Exp(v.get()));
}

template <Floating T, std::size_t ...DimsA, std::size_t ...DimsB>
Variable<T, DimsA...> operator+(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b) {
    using templateA = Expression<T, DimsA...>;
    using templateB = Expression<T, DimsB...>;
    return Variable(new Addition<templateA, templateB>(a.get(), b.get()));
}

template <Floating T, std::size_t ...DimsA, std::size_t ...DimsB>
Variable<T, DimsA...> operator-(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b) {
    using templateA = Expression<T, DimsA...>;
    using templateB = Expression<T, DimsB...>;
    return Variable(new Subtraction<templateA, templateB>(a.get(), b.get()));
}

template <Floating T, std::size_t ...DimsA, std::size_t ...DimsB>
Variable<T, DimsA...> operator*(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b) {
    using templateA = Expression<T, DimsA...>;
    using templateB = Expression<T, DimsB...>;
    return Variable(new Multiplication<templateA, templateB>(a.get(), b.get()));
}

template <Floating T, std::size_t ...DimsA, std::size_t ...DimsB>
Variable<T, DimsA...> operator/(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b) {
    using templateA = Expression<T, DimsA...>;
    using templateB = Expression<T, DimsB...>;
    return Variable(new Division<templateA, templateB>(a.get(), b.get()));
}
#endif