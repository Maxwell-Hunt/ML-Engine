#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"
#include <vector>

class ReductionOperation : public Internal::Expression {
public:
    virtual ~ReductionOperation() = default;
protected:
    ReductionOperation(std::vector<std::shared_ptr<Internal::Expression>>&& data, Context& context, float value);
    std::vector<std::shared_ptr<Internal::Expression>> data;
private:
    virtual const std::vector<std::shared_ptr<Internal::Expression>> children() const override;
};

class BinaryOperation : public Internal::Expression {
public:
    virtual ~BinaryOperation() = default;
protected:
    BinaryOperation(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context, float value);
    std::shared_ptr<Internal::Expression> e1;
    std::shared_ptr<Internal::Expression> e2;
private:
    virtual const std::vector<std::shared_ptr<Internal::Expression>> children() const override;
};

class UnaryOperation : public Internal::Expression {
public:
    virtual ~UnaryOperation() = default;
protected:
    UnaryOperation(const std::shared_ptr<Internal::Expression>& subexpr, Context& context, float value);
    std::shared_ptr<Internal::Expression> subexpr;
private:
    virtual const std::vector<std::shared_ptr<Internal::Expression>> children() const override;
};

class Addition : public BinaryOperation {
friend class Context;
private:
    Addition(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Subtraction : public BinaryOperation {
friend class Context;
private:
    Subtraction(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Multiplication : public BinaryOperation {
friend class Context;
private:
    Multiplication(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Division : public BinaryOperation {
friend class Context;
private:
    Division(const std::shared_ptr<Internal::Expression>& e1, const std::shared_ptr<Internal::Expression>& e2, Context& context);
    virtual void updatePartials() override;
};

class Square : public UnaryOperation {
friend class Context;
private:
    Square(const std::shared_ptr<Internal::Expression>& subexpr, Context& context);
    virtual void updatePartials() override;
};

class Sigmoid : public UnaryOperation {
friend class Context;
private:
    Sigmoid(const std::shared_ptr<Internal::Expression>& subexpr, Context& context);
    virtual void updatePartials() override;
};

class Log : public UnaryOperation {
friend class Context;
private:
    Log(const std::shared_ptr<Internal::Expression>& subexpr, Context& context);
    virtual void updatePartials() override;
};

class ReduceAdd : public ReductionOperation {
friend class Context;
private:
    ReduceAdd(std::vector<std::shared_ptr<Internal::Expression>>&& data, Context& context);
    virtual void updatePartials() override;
};

#endif