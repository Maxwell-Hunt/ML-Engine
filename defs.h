#ifndef __DEFS__
#define __DEFS__
#include "expression.h"

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

#endif