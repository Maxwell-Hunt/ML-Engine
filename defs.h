#ifndef __DEFS__
#define __DEFS__
#include "expression.h"

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
Variable<T, DimsA...> operator+(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
Variable<T, DimsA...> operator-(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
Variable<T, DimsA...> operator*(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
Variable<T, DimsA...> operator/(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);

#endif