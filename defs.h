#ifndef __DEFS__
#define __DEFS__
#include "expression.h"

template <std::size_t ...Dims>
Variable<Tensor<float, Dims...>> createTensorVariable();

template <std::size_t ...Dims>
Variable<Tensor<float, Dims...>> createRandomTensorVariable();

template <typename T, typename H>
Variable<T> operator+(const Variable<T>& a, const Variable<H>& b);

template <typename T, typename H>
Variable<T> operator-(const Variable<T>& a, const Variable<H>& b);

template <typename T, typename H>
Variable<T> operator*(const Variable<T>& a, const Variable<H>& b);

template <typename T, typename H>
Variable<T> operator/(const Variable<T>& a, const Variable<H>& b);

template <typename T>
Variable<T> exp(const Variable<T>& v);

template <TensorType T>
Variable<float> reduceAdd(const Variable<T>& t);

#endif