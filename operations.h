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

class PlusConst : public Expression {
public:
    PlusConst(const Expression& ex1, float other);
    PlusConst(float other, const Expression& ex1);
    virtual ~PlusConst() override {}

    virtual float getValue() const override;
    virtual float getPartial(const Expression& x) const override;
private:
    const Expression& ex1;
    float other;
};

class Multiply : public BinaryOperation {
public:
    Multiply(const Expression& ex1, const Expression& ex2);
    virtual ~Multiply() override {}

    virtual float getValue() const override;
    virtual float getPartial(const Expression& x) const override;
};

class MultiplyConst : public Expression {
public:
    MultiplyConst(const Expression& ex1, float other);
    MultiplyConst(float other, const Expression& ex1);
    virtual ~MultiplyConst() override {}

    virtual float getValue() const override;
    virtual float getPartial(const Expression& x) const override;
private:
    const Expression& ex1;
    float other;
};

Plus operator+(const Expression& a, const Expression& b);
PlusConst operator+(const Expression& a, float other);
PlusConst operator+(float other, const Expression& a);

Multiply operator*(const Expression& a, const Expression& b);
MultiplyConst operator*(const Expression& a, float other);
MultiplyConst operator*(float other, const Expression& a);


#endif