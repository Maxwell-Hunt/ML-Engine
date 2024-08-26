#ifndef __CONTEXT__
#define __CONTEXT__

#include "operations.h"
#include <random>
#include <stdexcept>

// TODO: Note that currently, in all of the operators which go between
//       variable and constant expression, we're creating a copy of the
//       constant variable.  This is probably not ideal and may slow down
//       the resulting program significantly.

template <typename T>
Variable<T> createVariable(T value) {
    return Variable(new ConstExpression(std::move(value)));
}

template <std::size_t ...Dims>
Variable<Tensor<float, Dims...>> createTensorVariable() {
    Tensor<float, Dims...> t = 0;
    return Variable(new ConstExpression(std::move(t)));
}

template <std::size_t ...Dims>
Variable<Tensor<float, Dims...>> createTensorVariable(std::initializer_list<float> items) {
    Tensor<float, Dims...> t = items;
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

template <std::size_t rows, std::size_t cols>
Variable<Matrix<float, rows, cols>> createMatrixVariable(std::initializer_list<float> items) {
    Matrix<float, rows, cols> m = items;
    return Variable(new ConstExpression(std::move(m)));
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
auto operator+(const Variable<T>& a, const H& b) {
    Variable<H> v = createVariable(b);
    return a + v;
}

template <typename T, typename H>
auto operator+(const T& a, const Variable<H>& b) {
    Variable<T> v = createVariable(a);
    return v + b;
}

template <typename T, typename H>
Variable<T> operator-(const Variable<T>& a, const Variable<H>& b) {
    return Variable(new Subtraction<T, H>(a.get(), b.get()));
}

template <typename T, typename H>
auto operator-(const Variable<T>& a, const H& b) {
    Variable<H> v = createVariable(b);
    return a - v;
}

template <typename T, typename H>
auto operator-(const T& a, const Variable<H>& b) {
    Variable<T> v = createVariable(a);
    return v - b;
}

template <typename T, typename H>
Variable<H> operator*(const Variable<T>& a, const Variable<H>& b) requires (Floating<T> && TensorType<H>) {
    return Variable(new Multiplication<T, H, H>(a.get(), b.get()));
}

template <typename T, typename H>
Variable<T> operator*(const Variable<T>& a, const Variable<H>& b) requires (!Floating<T> || !TensorType<H>) {
    return Variable(new Multiplication<T, H, T>(a.get(), b.get()));
}

template <typename T, typename H>
auto operator*(const Variable<T>& a, const H& b) {
    Variable<H> v = createVariable(b);
    return a * v;
}

template <typename T, typename H>
auto operator*(const T& a, const Variable<H>& b) {
    Variable<T> v = createVariable(a);
    return v * b;
}

template <typename T, typename H>
Variable<T> operator/(const Variable<T>& a, const Variable<H>& b) {
    return Variable(new Division<T, H>(a.get(), b.get()));
}

template <typename T, typename H>
auto operator/(const Variable<T>& a, const H& b) {
    Variable<H> v = createVariable(b);
    return a / v;
}

template <typename T, typename H>
auto operator/(const T& a, const Variable<H>& b) {
    Variable<T> v = createVariable(a);
    return v / b;
}

template <typename MatrixA, typename MatrixB>
Variable<typename MatMul<MatrixA, MatrixB>::ResultType> matmul(const Variable<MatrixA>& a, const Variable<MatrixB>& b) {
    return Variable(new MatMul<MatrixA, MatrixB>(a.get(), b.get()));
}

template <typename MatrixA, typename MatrixB>
auto matmul(const Variable<MatrixA>& a, const MatrixB& b) {
    Variable<MatrixB> v = createVariable(b);
    return matmul(a, v);
}

template <typename MatrixA, typename MatrixB>
auto matmul(const MatrixA& a, const Variable<MatrixB>& b) {
    Variable<MatrixA> v = createVariable(a);
    return matmul(b, a);
}

template <TensorType T>
Variable<float> reduceAdd(const Variable<T>& t) {
    return Variable(new ReduceAdd<T, float>(t.get()));
}

#endif