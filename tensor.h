#ifndef __TENSOR__
#define __TENSOR__

#include <vector>
#include <numeric>
#include <stdexcept>
#include <type_traits>

template <typename T>
class Tensor {

    template <typename U, typename H>
    friend auto operator+(const Tensor<U>& a, const Tensor<H>& b) -> Tensor<decltype(std::declval<U>() + std::declval<H>())>;

    template <typename U, typename H>
    friend auto operator-(const Tensor<U>& a, const Tensor<H>& b) -> Tensor<decltype(std::declval<U>() - std::declval<H>())>;

    template <typename U, typename H>
    friend auto operator*(const Tensor<U>& a, const Tensor<H>& b) -> Tensor<decltype(std::declval<U>() * std::declval<H>())>;

    template <typename U, typename H>
    friend auto operator*(const Tensor<U>& a, const H& scalar) -> Tensor<decltype(std::declval<U>() * std::declval<H>())>;

    template <typename U, typename H>
    friend auto operator*(const U& scalar, const Tensor<H>& a) -> Tensor<decltype(std::declval<U>() * std::declval<H>())>;

    template <typename U, typename H>
    friend auto operator/(const Tensor<U>& a, const H& scalar) -> Tensor<decltype(std::declval<U>() / std::declval<H>())>;

public:
    Tensor(const std::vector<std::size_t>& shape);
    T& at(const std::vector<std::size_t>& indices);
    const T& at(const std::vector<std::size_t>& indices) const;
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;

    const std::vector<std::size_t>& shape() const;
    std::size_t size() const;

    const std::vector<T>& data() const;

    void reshape(std::vector<std::size_t>&& new_shape);
private:

    std::size_t getIndex(const std::vector<std::size_t>& indices) const;

    std::vector<std::size_t> _shape;
    std::size_t _size;
    std::vector<T> _data;
};

template <typename T>
Tensor<T>::Tensor(const std::vector<std::size_t>& shape) : 
    _shape{shape},
    _size{std::accumulate(_shape.begin(), _shape.end(), 1u, std::multiplies<std::size_t>())},
    _data{std::vector<T>(_size)}
    {}

template <typename T>
std::size_t Tensor<T>::getIndex(const std::vector<std::size_t>& indices) const {
    if(indices.size() != _shape.size()) {
        throw std::runtime_error("Incorrect number of indices");
    }

    std::size_t arr_size = _size;
    std::size_t index = 0;
    for(std::size_t i = 0;i < _shape.size();i++) {
        arr_size /= _shape.at(i);
        index += arr_size * indices.at(i);
    }

    return index;
}

template <typename T>
const T& Tensor<T>::at(const std::vector<std::size_t>& indices) const {
    return _data.at(getIndex(indices));
}


template <typename T>
T& Tensor<T>::at(const std::vector<std::size_t>& indices) {
    return _data.at(getIndex(indices));
}

template <typename T>
const std::vector<std::size_t>& Tensor<T>::shape() const {
    return _shape;
}

template <typename T>
void Tensor<T>::reshape(std::vector<std::size_t>&& new_shape) {
    if(std::accumulate(new_shape.begin(), new_shape.end(), 1u, std::multiplies<std::size_t>()) != _size) {
        throw std::runtime_error("Invalid Reshape");
    }
    _shape = std::move(new_shape);
}

template <typename T>
std::size_t Tensor<T>::size() const {
    return _size;
}

template <typename T>
const std::vector<T>& Tensor<T>::data() const {
    return _data;
}

template <typename T>
typename std::vector<T>::iterator Tensor<T>::begin() { return _data.begin(); }

template <typename T>
typename std::vector<T>::iterator Tensor<T>::end() { return _data.end(); }

template <typename T>
typename std::vector<T>::const_iterator Tensor<T>::begin() const { return _data.cbegin(); }

template <typename T>
typename std::vector<T>::const_iterator Tensor<T>::end() const { return _data.cend(); }

template <typename U, typename H>
auto operator+(const Tensor<U>& a, const Tensor<H>& b) -> Tensor<decltype(std::declval<U>() + std::declval<H>())> {
    if(a.shape() != b.shape()) {
        throw std::runtime_error("Addition cannot proceed as tensor shapes do not match");
    }

    using ResultType = decltype(std::declval<U>() + std::declval<H>());
    Tensor<ResultType> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) + b._data.at(i);
    }

    return result;
}

template <typename U, typename H>
auto operator-(const Tensor<U>& a, const Tensor<H>& b) -> Tensor<decltype(std::declval<U>() - std::declval<H>())> {
    if(a.shape() != b.shape()) {
        throw std::runtime_error("Subtraction cannot proceed as tensor shapes do not match");
    }

    using ResultType = decltype(std::declval<U>() - std::declval<H>());
    Tensor<ResultType> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) - b._data.at(i);
    }

    return result;
}

template <typename U, typename H>
auto operator*(const Tensor<U>& a, const Tensor<H>& b) -> Tensor<decltype(std::declval<U>() * std::declval<H>())> {
    if(a.shape() != b.shape()) {
        throw std::runtime_error("Elementwise multiplication cannot proceed as tensor shapes do not match");
    }

    using ResultType = decltype(std::declval<U>() * std::declval<H>());
    Tensor<ResultType> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) * b._data.at(i);
    }
    
    return result;
}

template <typename U, typename H>
auto operator*(const Tensor<U>& a, const H& scalar) -> Tensor<decltype(std::declval<U>() * std::declval<H>())> {
    using ResultType = decltype(std::declval<U>() * std::declval<H>());
    Tensor<ResultType> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) * scalar;
    }

    return result;
}

template <typename U, typename H>
auto  operator*(const U& scalar, const Tensor<H>& a) -> Tensor<decltype(std::declval<U>() * std::declval<H>())> {
    return a * scalar;
}

template <typename U, typename H>
auto operator/(const Tensor<U>& a, const H& scalar) -> Tensor<decltype(std::declval<U>() / std::declval<H>())> {
    using ResultType = decltype(std::declval<U>() / std::declval<H>());
    Tensor<ResultType> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) / scalar;
    }

    return result;
}
#endif