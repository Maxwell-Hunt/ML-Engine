#include "operations.h"

BinaryOperation::BinaryOperation(Expression& ex1, Expression& ex2) : ex1{ex1}, ex2{ex2} {}

Plus::Plus(Expression& ex1, Expression& ex2) : BinaryOperation(ex1, ex2) {}

float Plus::getValue() const {
    return ex1.getValue() + ex2.getValue();
}

float Plus::getPartial() const {
    return ex1.getPartial() + ex2.getPartial();
}

Multiply::Multiply(Expression& ex1, Expression& ex2) : BinaryOperation(ex1, ex2) {}

float Multiply::getValue() const {
    return ex1.getValue() * ex2.getValue();
}

float Multiply::getPartial() const {
    return ex1.getValue() * ex2.getPartial() + ex2.getValue() * ex1.getPartial();
}