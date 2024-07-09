#include <numeric>
#include <cmath>
#include <memory>

#include "operations.h"
#include "expression.h"

namespace Internal {

ReductionOperation::ReductionOperation(std::vector<std::shared_ptr<Internal::Expression>>&& data, float value) :
    Internal::Expression{value}, data{data} {}

std::vector<std::shared_ptr<Internal::Expression>> ReductionOperation::children() const {
    return data;
}

BinaryOperation::BinaryOperation(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, float value) :
    Internal::Expression{value}, e1{e1}, e2{e2} {}

std::vector<std::shared_ptr<Internal::Expression>>  BinaryOperation::children() const {
    return {e1, e2};
}

UnaryOperation::UnaryOperation(const std::shared_ptr<Internal::Expression>& subexpr, float value) :
    Internal::Expression{value}, subexpr{subexpr} {}

std::vector<std::shared_ptr<Internal::Expression>> UnaryOperation::children() const {
    return {subexpr};
}

Addition::Addition(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2) :
    BinaryOperation{e1, e2, e1->getValue() + e2->getValue()} {}

void Addition::updatePartials() {
    addToPartial(*e1, getPartial());
    addToPartial(*e2, getPartial());
}

Subtraction::Subtraction(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2) :
    BinaryOperation{e1, e2, e1->getValue() - e2->getValue()} {}

void Subtraction::updatePartials() {
    addToPartial(*e1, getPartial());
    addToPartial(*e2, -getPartial());
}

Multiplication::Multiplication(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2) :
    BinaryOperation{e1, e2, e1->getValue() * e2->getValue()} {}

void Multiplication::updatePartials() {
    addToPartial(*e1, e2->getValue() * getPartial());
    addToPartial(*e2, e1->getValue() * getPartial());
}

Division::Division(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2) :
    BinaryOperation{e1, e2, e1->getValue() / e2->getValue()} {}

void Division::updatePartials() {
    float val1 = e1->getValue();
    float val2 = e2->getValue();
    addToPartial(*e1,  (1 / val2) * getPartial());
    addToPartial(*e2, (-val1 / (val2 * val2)) * getPartial());
}

Square::Square(const std::shared_ptr<Internal::Expression>& subexpr) :
    UnaryOperation{subexpr, subexpr->getValue() * subexpr->getValue()} {}

void Square::updatePartials() {
    addToPartial(*subexpr, 2 * subexpr->getValue() * getPartial());
}

Sigmoid::Sigmoid(const std::shared_ptr<Internal::Expression>& subexpr) :
    UnaryOperation{subexpr, 1 / (1 + std::exp(-subexpr->getValue()))} {}

void Sigmoid::updatePartials() {
    // sigmoid' = sigmoid * (1 - sigmoid)
    addToPartial(*subexpr, getValue() * (1 - getValue()) * getPartial());
}

Log::Log(const std::shared_ptr<Internal::Expression>& subexpr) :
    UnaryOperation{subexpr, std::log(subexpr->getValue())} {}

void Log::updatePartials() {
    addToPartial(*subexpr, (1 / subexpr->getValue()) * getPartial());
}

ReduceAdd::ReduceAdd(std::vector<std::shared_ptr<Internal::Expression>>&& data) :
    ReductionOperation{std::move(data), 
        std::accumulate(data.begin(), data.end(), 0.f, 
        [](float sum, const std::shared_ptr<Internal::Expression>& next)
            { return sum + next->getValue();})} {}

void ReduceAdd::updatePartials() {
    for(auto& expr : data) addToPartial(*expr, getPartial());
}

}