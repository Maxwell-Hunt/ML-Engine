#include "operations.h"
#include "context.h"
#include <numeric>
#include <cmath>

ReductionOperation::ReductionOperation(std::vector<std::shared_ptr<Internal::Expression>>&& data, Context& context, float value) :
    Internal::Expression{context, value}, data{data} {}

BinaryOperation::BinaryOperation(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context, float value) :
    Internal::Expression{context, value}, e1{e1}, e2{e2} {}

void BinaryOperation::backPropagateInternal() {
    updatePartials();
    propagate(*e1);
    // Propagating gradients backwards twice would result in incorect partials
    if(e1 != e2) { propagate(*e2); }
}

UnaryOperation::UnaryOperation(const std::shared_ptr<Internal::Expression>& subexpr, Context& context, float value) :
    Internal::Expression{context, value}, subexpr{subexpr} {}

Addition::Addition(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context, e1->getValue() + e2->getValue()} {}

void Addition::updatePartials() {
    addToPartial(*e1, getPartial());
    addToPartial(*e2, getPartial());
}

Subtraction::Subtraction(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context, e1->getValue() - e2->getValue()} {}

void Subtraction::updatePartials() {
    addToPartial(*e1, getPartial());
    addToPartial(*e2, -getPartial());
}

Multiplication::Multiplication(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context, e1->getValue() * e2->getValue()} {}

void Multiplication::updatePartials() {
    addToPartial(*e1, e2->getValue() * getPartial());
    addToPartial(*e2, e1->getValue() * getPartial());
}

Division::Division(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context, e1->getValue() / e2->getValue()} {}

void Division::updatePartials() {
    float val1 = e1->getValue();
    float val2 = e2->getValue();
    addToPartial(*e1,  (1 / val2) * getPartial());
    addToPartial(*e2, (-val1 / (val2 * val2)) * getPartial());
}

Square::Square(const std::shared_ptr<Internal::Expression>& subexpr, Context& context) :
    UnaryOperation{subexpr, context, subexpr->getValue() * subexpr->getValue()} {}

void Square::backPropagateInternal() {
    addToPartial(*subexpr, 2 * subexpr->getValue() * getPartial());
    propagate(*subexpr);
}

Sigmoid::Sigmoid(const std::shared_ptr<Internal::Expression>& subexpr, Context& context) :
    UnaryOperation{subexpr, context,  1 / (1 + std::exp(-subexpr->getValue()))} {}

void Sigmoid::backPropagateInternal() {

    // sigmoid' = sigmoid * (1 - sigmoid)
    addToPartial(*subexpr, getValue() * (1 - getValue()) * getPartial());
    propagate(*subexpr);
}

Log::Log(const std::shared_ptr<Internal::Expression>& subexpr, Context& context) :
    UnaryOperation{subexpr, context, std::log(subexpr->getValue())} {}

void Log::backPropagateInternal() {
    addToPartial(*subexpr, (1 / subexpr->getValue()) * getPartial());
    propagate(*subexpr);
}

ReduceAdd::ReduceAdd(std::vector<std::shared_ptr<Internal::Expression>>&& data, Context& context) :
    ReductionOperation{std::move(data), context, 
        std::accumulate(data.begin(), data.end(), 0.f, 
        [](float sum, const std::shared_ptr<Internal::Expression>& next)
            { return sum + next->getValue();})} {}

void ReduceAdd::backPropagateInternal() {
    for(auto& expr : data) addToPartial(*expr, getPartial());
    // This step relies on the fact that data does not contain duplicate expressions
    for(auto& expr : data) propagate(*expr);
}