#include "operations.h"

BinaryOperation::BinaryOperation(const Expression& ex1, const Expression& ex2) : ex1{ex1}, ex2{ex2} {}

Plus::Plus(const Expression& ex1, const Expression& ex2) : BinaryOperation(ex1, ex2) {}

float Plus::getValue() const {
    return ex1.getValue() + ex2.getValue();
}

float Plus::getPartial(const Expression& x) const {
    return ex1.getPartial(x) + ex2.getPartial(x);
}

PlusConst::PlusConst(const Expression& ex1, float other) : ex1{ex1}, other{other} {}
PlusConst::PlusConst(float other, const Expression& ex1) : ex1{ex1}, other{other} {}

float PlusConst::getValue() const {
    return ex1.getValue() + other;
}

float PlusConst::getPartial(const Expression& x) const {
    return ex1.getPartial(x);
}

Multiply::Multiply(const Expression& ex1, const Expression& ex2) : BinaryOperation(ex1, ex2) {}

float Multiply::getValue() const {
    return ex1.getValue() * ex2.getValue();
}

float Multiply::getPartial(const Expression& x) const {
    return ex1.getValue() * ex2.getPartial(x) + ex2.getValue() * ex1.getPartial(x);
}

MultiplyConst::MultiplyConst(const Expression& ex1, float other) : ex1{ex1}, other{other} {}
MultiplyConst::MultiplyConst(float other, const Expression& ex1) : ex1{ex1}, other{other} {}

float MultiplyConst::getValue() const {
    return ex1.getValue() * other;
}

float MultiplyConst::getPartial(const Expression& x) const {
    return ex1.getPartial(x) * other;
}

Plus operator+(const Expression& a, const Expression& b) {
    return Plus(a, b);
}

PlusConst operator+(const Expression& a, float other) {
    return PlusConst(a, other);
}

PlusConst operator+(float other, const Expression& a) {
    return PlusConst(a, other);
}

Multiply operator*(const Expression& a, const Expression& b) {
    return Multiply(a, b);
}

MultiplyConst operator*(float other, const Expression& a) {
    return MultiplyConst(a, other);
}

MultiplyConst operator*(const Expression& a, float other) {
    return MultiplyConst(a, other);
}