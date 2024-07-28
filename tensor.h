#ifndef __TENSOR__
#define __TENSOR__

#include <array>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>

template <std::size_t First, std::size_t ...Rest>
struct IndexStructure : std::true_type { 
    using rest = IndexStructure<Rest...>;
};

template <std::size_t Single>
struct IndexStructure<Single> : std::true_type {
    using rest = std::false_type;
};


template <typename IndicesA, typename IndicesB>
struct isSuffix : isSuffix<IndicesA, typename IndicesB::rest> {};

// The empty list is allways a suffix of anything
template <typename IndicesB>
struct isSuffix<std::false_type, IndicesB> : std::true_type {};

// Nothing except the empty list is a suffix of the empty list
template <typename IndicesA>
struct isSuffix<IndicesA, std::false_type> : std::false_type {};

template <>
struct isSuffix<std::false_type, std::false_type> : std::true_type {};

// If both sets of indices are the same then we return true_type
template <std::size_t ...Indices>
struct isSuffix<IndexStructure<Indices...>, IndexStructure<Indices...>> : std::true_type {};

template <typename T, std::size_t ...Dims>
class Tensor;

template <typename T>
struct isTensor : std::false_type {};

template <typename T, std::size_t ...Dims>
struct isTensor<Tensor<T, Dims...>> : std::true_type {};

// Should we require that the value type of a Tensor is a NonTensor? That might make sense.
template <typename T>
concept NonTensor = !isTensor<T>::value;

template <typename T, std::size_t ...Dims>
class Tensor {
public:
    constexpr Tensor() = default;
    constexpr Tensor(std::initializer_list<T> list) { std::copy(list.begin(), list.end(), _data.begin()); }
    
    static constexpr std::size_t size() { return _size; }
    static constexpr std::array<std::size_t, sizeof...(Dims)> dims() { return _dims; }

    T& at(std::initializer_list<std::size_t> indices) { return _data.at(getIndex(indices)); }
    const T& at(std::initializer_list<std::size_t> indices) const { return _data.at(getIndex(indices)); }

    std::array<T, size()>::iterator begin() { return _data.begin(); }
    std::array<T, size()>::iterator end() { return _data.end(); }

    const std::array<T, size()>::const_iterator end() const { return _data.end(); }
    const std::array<T, size()>::const_iterator begin() const { return _data.begin(); }

    template <typename H, std::size_t ...OtherDims>
    auto operator+(const Tensor<H, OtherDims...>& other) const -> Tensor<decltype(std::declval<T>() + std::declval<H>()), Dims...> {
        return applyBinaryOperation<std::plus>(other);
    }

    template <typename H, std::size_t ...OtherDims>
    auto operator-(const Tensor<H, OtherDims...>& other) const -> Tensor<decltype(std::declval<T>() - std::declval<H>()), Dims...> {
        return applyBinaryOperation<std::minus>(other);
    }

    template <typename H, std::size_t ...OtherDims>
    auto operator*(const Tensor<H, OtherDims...>& other) const -> Tensor<decltype(std::declval<T>() * std::declval<H>()), Dims...> {
        return applyBinaryOperation<std::multiplies>(other);
    }

    template <typename H, std::size_t ...OtherDims>
    auto operator/(const Tensor<H, OtherDims...>& other) const -> Tensor<decltype(std::declval<T>() / std::declval<H>()), Dims...> {
        return applyBinaryOperation<std::divides>(other);
    }

    template<NonTensor H>
    auto operator*(const H& scalar) const -> Tensor<decltype(std::declval<T>() * std::declval<H>()), Dims...> {
        using ResultType = decltype(std::declval<T>() * std::declval<H>());
        Tensor<ResultType, Dims...> result;
        std::transform(begin(), end(), result.begin(), [&scalar](const T& val) { return val * scalar; });
        return result;
    }

    template <NonTensor H>
    auto operator/(const H& scalar) const -> Tensor<decltype(std::declval<T>() * std::declval<H>()), Dims...> {
        using ResultType = decltype(std::declval<T>() * std::declval<H>());
        Tensor<ResultType, Dims...> result;
        std::transform(begin(), end(), result.begin(), [&scalar](const T& val) { return val / scalar; });
        return result;
    }

private:
    std::size_t getIndex(std::initializer_list<std::size_t> indices) const {
        std::size_t result = 0;
        std::size_t spread = _size;
        auto it = indices.begin();
        for(std::size_t i = 0;it != indices.end() && i < _size;++i, ++it) {
            if(*it >= _dims[i]) { throw std::length_error("Tensor index out of bounds");}
            spread /= _dims[i];
            result += spread * *it;
        }
        return result;
    }

    // Returns the result of performing the binary operation on this tensor as well as another.
    // If the second Tensor does not have the exact same shape, it will broadcast it accross.
    // For this to work it is required that the dimensions list of the second tensor is a suffix
    // of the first.
    template <template <typename> class BinaryOperation, typename H, std::size_t ...OtherDims>
    auto applyBinaryOperation(const Tensor<H, OtherDims...>& other) const ->
        Tensor<decltype(BinaryOperation<T>()(std::declval<T>(), std::declval<H>())), Dims...>
    {
        static_assert(isSuffix<IndexStructure<OtherDims...>, IndexStructure<Dims...>>::value, "Dimensions mismatch");

        using ResultType = decltype(BinaryOperation<T>()(std::declval<T>(), std::declval<H>()));
        Tensor<ResultType, Dims...> result;
        // Apply Broadcasting as is done by numpy. Broadcasting is explained in more detail here
        // https://numpy.org/doc/stable/user/basics.broadcasting.html
        const std::size_t spread = (OtherDims * ...);
        const std::size_t iters = _size / spread;
        for(std::size_t i = 0; i < iters; i++) {
            const std::size_t offset = i * spread;
            std::transform(begin() + offset, begin() + offset + spread, other.begin(), result.begin() + offset, BinaryOperation<ResultType>());
        }
        return result;
    }

    static constexpr std::size_t _size = (Dims * ...);
    static constexpr std::array<std::size_t, sizeof...(Dims)> _dims = {Dims...};
    std::array<T, _size> _data;
};

template <NonTensor H, typename T, std::size_t ...Dims>
auto operator*(const H& scalar, const Tensor<T, Dims...>& t) -> Tensor<decltype(std::declval<H>() * std::declval<T>()), Dims...> {
    return t * scalar;
}

template <std::size_t rows, std::size_t cols>
using Matrix = Tensor<float, rows, cols>;

template <std::size_t size>
using Vector = Tensor<float, size>;


#endif