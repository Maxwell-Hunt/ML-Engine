#ifndef __TENSOR__
#define __TENSOR__

#include <vector>
#include <numeric>
#include <stdexcept>

template <typename T>
class Tensor {

template <typename H>
friend Tensor<H> operator+(const Tensor<H>& a, const Tensor<H>& b);
template <typename H>
friend Tensor<H> operator-(const Tensor<H>& a, const Tensor<H>& b);
template <typename H>
friend Tensor<T> operator*(const Tensor<H>& a, const Tensor<H>& b);
template <typename H>
friend Tensor<H> operator*(const Tensor<H>& a, const H& scalar);
template <typename H>
friend Tensor<H> operator*(const H& scalar, const Tensor<H>& a);
template <typename H>
friend Tensor<H> operator/(const Tensor<H>& a, const H& scalar);

public:
    Tensor(const std::vector<std::size_t>& shape);
    T& at(const std::vector<std::size_t>& indices);
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();

    const std::vector<std::size_t>& shape() const;
    std::size_t size() const;

    const std::vector<T>& data() const;

    T dot(const Tensor& other) const;
private:
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
T& Tensor<T>::at(const std::vector<std::size_t>& indices) {
    if(indices.size() != _shape.size()) {
        throw std::runtime_error("Incorrect number of indices");
    }

    std::size_t arr_size = _size;
    std::size_t index = 0;
    for(std::size_t i = 0;i < _shape.size();i++) {
        arr_size /= _shape.at(i);
        index += arr_size * indices.at(i);
    }

    return _data.at(index);
}

template <typename T>
const std::vector<std::size_t>& Tensor<T>::shape() const {
    return _shape;
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
Tensor<T> operator+(const Tensor<T>& a, const Tensor<T>& b) {
    if(a.shape() != b.shape()) {
        throw std::runtime_error("Addition cannot proceed as tensor shapes do not match");
    }

    Tensor<T> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) + b._data.at(i);
    }

    return result;
}

template <typename T>
Tensor<T> operator-(const Tensor<T>& a, const Tensor<T>& b) {
    if(a.shape() != b.shape()) {
        throw std::runtime_error("Subtraction cannot proceed as tensor shapes do not match");
    }

    Tensor<T> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) - b._data.at(i);
    }

    return result;
}

template <typename T>
Tensor<T> operator*(const Tensor<T>& a, const Tensor<T>& b) {
    if(a.shape() != b.shape()) {
        throw std::runtime_error("Elementwise multiplication cannot proceed as tensor shapes do not match");
    }

    Tensor<T> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) * b._data.at(i);
    }
    
    return result;
}

template <typename T>
Tensor<T> operator*(const Tensor<T>& a, const T& scalar) {
    Tensor<T> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) * scalar;
    }

    return result;
}

template <typename T>
Tensor<T> operator*(const T& scalar, const Tensor<T>& a) {
    return a * scalar;
}

template <typename T>
Tensor<T> operator/(const Tensor<T>& a, const T& scalar) {
    Tensor<T> result(a.shape());
    for(std::size_t i = 0;i < a.size();i++) {
        result._data.at(i) = a._data.at(i) / scalar;
    }

    return result;
}
#endif