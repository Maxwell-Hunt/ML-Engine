#include "vector.h"
#include "context.h"

#include <random>

Vector::Vector(std::size_t size, bool zeros, Context& context) : size{size}, data{std::vector<Expression>{size}}, context{context} {
    if(zeros) {
        for(Expression& ex : data) ex = context.createVariable(0);
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-1, 1);
        for(Expression& ex : data) ex = context.createVariable(dis(gen));
    }
}

Vector::Vector(std::vector<Expression>&& data, Context& context) : size{data.size()}, data{std::move(data)}, context{context} {}

float Vector::at(std::size_t index) const {
    return data.at(index).getValue();
}

std::vector<Expression>::iterator Vector::begin() {
    return data.begin();
}

std::vector<Expression>::iterator Vector::end() {
    return data.end();
}