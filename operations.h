#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"

class BinaryOperation : public Expression {
protected:
    BinaryOperation(Expression& ex1, Expression& ex2);
    Expression& ex1;
    Expression& ex2;
};

class Plus : public BinaryOperation {
public:
    Plus(Expression& ex1, Expression& ex2);

    virtual float getValue() const override;
    virtual float getPartial() const override;
};

class Multiply : public BinaryOperation {
public:
    Multiply(Expression& ex1, Expression& ex2);

    virtual float getValue() const override;
    virtual float getPartial() const override;
};

#endif