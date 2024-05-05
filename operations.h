#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"

class BinaryOperation : public Internal::Expression {
protected:
    BinaryOperation(const Internal::Expression& e1, const Internal::Expression& e2, Context& context);
    const Internal::Expression& e1;
    const Internal::Expression& e2;
};

class UnaryOperation : public Internal::Expression {
protected:
    UnaryOperation(const Internal::Expression& subexpr, Context& context);
    const Internal::Expression& subexpr;
};

class Addition : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;

    virtual float getPartial(const Internal::Expression& other) const override;

private:
    Addition(const Internal::Expression& e1, const Internal::Expression& e2, Context& context);
};

class Subtraction : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;

    virtual float getPartial(const Internal::Expression& other) const override;
private:
    Subtraction(const Internal::Expression& e1, const Internal::Expression& e2, Context& context);
};

class Multiplication : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Internal::Expression& other) const override;
private:
    Multiplication(const Internal::Expression& e1, const Internal::Expression& e2, Context& context);
};

class Division : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Internal::Expression& other) const override;
private:
    Division(const Internal::Expression& e1, const Internal::Expression& e2, Context& context);
};

class Square : public UnaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Internal::Expression& other) const override;
private:
    Square(const Internal::Expression& subexpr, Context& context);
};

Expression operator+(const Expression& e1, const Expression& e2);
Expression operator+(const Expression& e1, float val);
Expression operator+(float val, const Expression& e2);

Expression operator-(const Expression& e1, const Expression& e2);
Expression operator-(const Expression& e1, float val);
Expression operator-(float val, const Expression& e2);

Expression operator*(const Expression& e1, const Expression& e2);
Expression operator*(const Expression& e1, float val);
Expression operator*(float val, const Expression& e2);

Expression operator/(const Expression& e1, const Expression& e2);
Expression operator/(const Expression& e1, float val);
Expression operator/(float val, const Expression& e2);

#endif