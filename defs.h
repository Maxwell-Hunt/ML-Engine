#ifndef __DEFS__
#define __DEFS__

#include "expression.h"

template <std::size_t ...Dims>
Variable<float, Dims...> createVariable();

template <std::size_t ...Dims>
Variable<float, Dims...> createRandomVariable();

template <Floating T, std::size_t ...Dims>
Variable<T, Dims...> square(const Variable<T, Dims...>& v);

#endif