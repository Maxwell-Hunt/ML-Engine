#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"


class BinaryOperation : public Internal::Expression {
public:
    virtual ~BinaryOperation() = default;
protected:
    BinaryOperation(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    std::shared_ptr<Internal::Expression> e1;
    std::shared_ptr<Internal::Expression> e2;
private:
    virtual void backPropagateInternal() override;
    virtual void updatePartials() = 0;
};

class UnaryOperation : public Internal::Expression {
public:
    virtual ~UnaryOperation() = default;
protected:
    UnaryOperation(const std::shared_ptr<Internal::Expression>& subexpr, Context& context);
    std::shared_ptr<Internal::Expression> subexpr;
};

class Addition : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
private:
    Addition(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Subtraction : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
private:
    Subtraction(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Multiplication : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
private:
    Multiplication(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Division : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
private:
    Division(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Square : public UnaryOperation {
friend class Context;
public:
    virtual float getValue() const override;
private:
    Square(const std::shared_ptr<Internal::Expression>& subexpr, Context& context);
    virtual void backPropagateInternal() override;
};

#endif