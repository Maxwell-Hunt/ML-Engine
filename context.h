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
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution dis(-1.0, 1.0);
    std::transform(t.begin(), t.end(), t.begin(), [&dis, &gen](float v) { return dis(gen); });
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

#endif