#include "operations.h"

BinaryOperation::BinaryOperation(const Expression& ex1, const Expression& ex2) : ex1{ex1}, ex2{ex2} {}

Plus::Plus(const Expression& ex1, const Expression& ex2) : BinaryOperation(ex1, ex2) {}

float Plus::getValue() const {
    return ex1.getValue() + ex2.getValue();
}

float Plus::getPartial(const Expression& x) const {
    return ex1.getPartial(x) + ex2.getPartial(x);
}

Multiply::Multiply(const Expression& ex1, const Expression& ex2) : BinaryOperation(ex1, ex2) {}

float Multiply::getValue() const {
    return ex1.getValue() * ex2.getValue();
}

float Multiply::getPartial(const Expression& x) const {
    return ex1.getValue() * ex2.getPartial(x) + ex2.getValue() * ex1.getPartial(x);
}

Plus operator+(const Expression& a, const Expression& b) {
    return Plus(a, b);
}

Multiply operator*(const Expression& a, const Expression& b) {
    return Multiply(a, b);
}