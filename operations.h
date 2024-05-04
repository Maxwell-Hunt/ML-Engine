#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"

class BinaryOperation : public Expression {
protected:
    BinaryOperation(const Expression& e1, const Expression& e2, Context& context);
    const Expression& e1;
    const Expression& e2;
};

class UnaryOperation : public Expression {
protected:
    UnaryOperation(const Expression& subexpr, Context& context);
    const Expression& subexpr;
};

class Addition : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;

    virtual float getPartial(const Expression& other) const override;

private:
    Addition(const Expression& e1, const Expression& e2, Context& context);
};

class Subtraction : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;

    virtual float getPartial(const Expression& other) const override;
private:
    Subtraction(const Expression& e1, const Expression& e2, Context& context);
};

class Multiplication : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Expression& other) const override;
private:
    Multiplication(const Expression& e1, const Expression& e2, Context& context);
};

class Division : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Expression& other) const override;
private:
    Division(const Expression& e1, const Expression& e2, Context& context);
};

class Square : public UnaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Expression& other) const override;
private:
    Square(const Expression& subexpr, Context& context);
};

const Expression& operator+(const Expression& e1, const Expression& e2);
const Expression& operator+(const Expression& e1, float val);
const Expression& operator+(float val, const Expression& e2);

const Expression& operator-(const Expression& e1, const Expression& e2);
const Expression& operator-(const Expression& e1, float val);
const Expression& operator-(float val, const Expression& e2);

const Expression& operator*(const Expression& e1, const Expression& e2);
const Expression& operator*(const Expression& e1, float val);
const Expression& operator*(float val, const Expression& e2);

const Expression& operator/(const Expression& e1, const Expression& e2);
const Expression& operator/(const Expression& e1, float val);
const Expression& operator/(float val, const Expression& e2);

#endif