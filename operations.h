#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"

class BinaryOperation : public Expression {
protected:
    BinaryOperation(const Expression& ex1, const Expression& ex2);
    virtual ~BinaryOperation() override {}
    const Expression& ex1;
    const Expression& ex2;
};

class Plus : public BinaryOperation {
public:
    Plus(const Expression& ex1, const Expression& ex2);
    virtual ~Plus() override {}

    virtual float getValue() const override;
    virtual float getPartial(const Expression& x) const override;
};

class Multiply : public BinaryOperation {
public:
    Multiply(const Expression& ex1, const Expression& ex2);
    virtual ~Multiply() override {}

    virtual float getValue() const override;
    virtual float getPartial(const Expression& x) const override;
};

Plus operator+(const Expression& a, const Expression& b);
Multiply operator*(const Expression& a, const Expression& b);

#endif