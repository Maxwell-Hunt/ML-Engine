#include "operations.h"
#include "context.h"

BinaryOperation::BinaryOperation(const Expression& e1, const Expression& e2, Context& context) :
    Expression{context}, e1{e1}, e2{e2} {}

UnaryOperation::UnaryOperation(const Expression& subexpr, Context& context) :
    Expression{context}, subexpr{subexpr} {}

Addition::Addition(const Expression& e1, const Expression& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Addition::getValue() const {
    return e1.getValue() + e2.getValue();
}

float Addition::getPartial(const Expression& other) const {
    return e1.getPartial(other) + e2.getPartial(other);
}

Subtraction::Subtraction(const Expression& e1, const Expression& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Subtraction::getValue() const {
    return e1.getValue() - e2.getValue();
}

float Subtraction::getPartial(const Expression& other) const {
    return e1.getPartial(other) - e2.getPartial(other);
}

Multiplication::Multiplication(const Expression& e1, const Expression& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Multiplication::getValue() const {
    return e1.getValue() * e2.getValue();
}

float Multiplication::getPartial(const Expression& other) const {
    return e1.getPartial(other) * e2.getValue() + e1.getValue() * e2.getPartial(other);
}

Division::Division(const Expression& e1, const Expression& e2, Context& context) :
    BinaryOperation{e1, e2, context} {}

float Division::getValue() const {
    return e1.getValue() / e2.getValue();
}

float Division::getPartial(const Expression& other) const {
    float e1Value = e1.getValue();
    float e2Value = e2.getValue();
    float e1Partial = e1.getPartial(other);
    float e2Partial = e2.getPartial(other);
    return (e1Partial* e2Value - e2Partial * e1Value) / (e2Value * e2Value);
}

Square::Square(const Expression& subexpr, Context& context) : UnaryOperation{subexpr, context} {}

float Square::getValue() const {
    float x = subexpr.getValue();
    return x * x;
}

float Square::getPartial(const Expression& other) const {
    return 2 * subexpr.getValue() * subexpr.getPartial(other);
}

const Expression& operator+(const Expression& e1, const Expression& e2) {
    return e1.getContext().add(e1, e2);
}

const Expression& operator+(const Expression& e1, float val) {
    return e1.getContext().add(e1, val);
}

const Expression& operator+(float val, const Expression& e2) {
    return e2.getContext().add(val, e2);
} 

const Expression& operator-(const Expression& e1, const Expression& e2) {
    return e1.getContext().sub(e1, e2);
}

const Expression& operator-(const Expression& e1, float val) {
    return e1.getContext().sub(e1, val);
}

const Expression& operator-(float val, const Expression& e2) {
    return e2.getContext().sub(val, e2);
} 

const Expression& operator*(const Expression& e1, const Expression& e2) {
    return e1.getContext().mult(e1, e2);
}

const Expression& operator*(const Expression& e1, float val) {
    return e1.getContext().mult(e1, val);
}

const Expression& operator*(float val, const Expression& e2) {
    return e2.getContext().mult(val, e2);
}

const Expression& operator/(const Expression& e1, const Expression& e2) {
    return e1.getContext().div(e1, e2);
}

const Expression& operator/(const Expression& e1, float val) {
    return e1.getContext().div(e1, val);
}

const Expression& operator/(float val, const Expression& e2) {
    return e2.getContext().div(val, e2);
}