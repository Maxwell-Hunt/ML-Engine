#include "operations.h"
#include "context.h"

BinaryOperation::BinaryOperation(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    Internal::Expression{context}, e1{e1}, e2{e2} {}

void BinaryOperation::backPropagateInternal() {
    updatePartials();
    propagate(*e1);
    // Propagating errors backwards twice would result in incorect partials
    if(e1 != e2) { propagate(*e2); }
}

UnaryOperation::UnaryOperation(const std::shared_ptr<Internal::Expression>& subexpr, Context& context) :
    Internal::Expression{context}, subexpr{subexpr} {}

Addition::Addition(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Addition::getValue() const {
    return e1->getValue() + e2->getValue();
}

void Addition::updatePartials() {
    addToPartial(*e1, getPartial());
    addToPartial(*e2, getPartial());
}

Subtraction::Subtraction(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Subtraction::getValue() const {
    return e1->getValue() - e2->getValue();
}

void Subtraction::updatePartials() {
    addToPartial(*e1, getPartial());
    addToPartial(*e2, -getPartial());
}

Multiplication::Multiplication(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Multiplication::getValue() const {
    return e1->getValue() * e2->getValue();
}

void Multiplication::updatePartials() {
    addToPartial(*e1, e2->getValue() * getPartial());
    addToPartial(*e2, e1->getValue() * getPartial());
}

Division::Division(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Division::getValue() const {
    return e1->getValue() / e2->getValue();
}

void Division::updatePartials() {
    float val1 = e1->getValue();
    float val2 = e2->getValue();
    addToPartial(*e1,  (1 / val2) * getPartial());
    addToPartial(*e2, (-val1 / (val2 * val2)) * getPartial());
}

Square::Square(const std::shared_ptr<Internal::Expression>& subexpr, Context& context) :
    UnaryOperation{subexpr, context} {}

float Square::getValue() const {
    float val = subexpr->getValue();
    return val * val;
}

void Square::backPropagateInternal() {
    addToPartial(*subexpr, 2 * subexpr->getValue() * getPartial());
}

Expression operator+(const Expression& e1, const Expression& e2) {
    return e1.getContext().add(e1, e2);
}

Expression operator+(const Expression& e1, float val) {
    return e1.getContext().add(e1, val);
}

Expression operator+(float val, const Expression& e2) {
    return e2.getContext().add(val, e2);
} 

Expression operator-(const Expression& e1, const Expression& e2) {
    return e1.getContext().sub(e1, e2);
}

Expression operator-(const Expression& e1, float val) {
    return e1.getContext().sub(e1, val);
}

Expression operator-(float val, const Expression& e2) {
    return e2.getContext().sub(val, e2);
} 

Expression operator*(const Expression& e1, const Expression& e2) {
    return e1.getContext().mult(e1, e2);
}

Expression operator*(const Expression& e1, float val) {
    return e1.getContext().mult(e1, val);
}

Expression operator*(float val, const Expression& e2) {
    return e2.getContext().mult(val, e2);
}

Expression operator/(const Expression& e1, const Expression& e2) {
    return e1.getContext().div(e1, e2);
}

Expression operator/(const Expression& e1, float val) {
    return e1.getContext().div(e1, val);
}

Expression operator/(float val, const Expression& e2) {
    return e2.getContext().div(val, e2);
}