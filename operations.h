#ifndef __OPERATIONS__
#define __OPERATIONS__

#include <vector>
#include <memory>
#include "context.h"
#include "expression.h"

namespace Internal {

class ReductionOperation : public Expression {
public:
    virtual ~ReductionOperation() = default;
protected:
    ReductionOperation(std::vector<std::shared_ptr<Expression>>&& data, float value);
    std::vector<std::shared_ptr<Expression>> data;
private:
    virtual std::vector<std::shared_ptr<Expression>> children() const override;
};

class BinaryOperation : public Expression {
public:
    virtual ~BinaryOperation() = default;
protected:
    BinaryOperation(const std::shared_ptr<Expression>& e1, const std::shared_ptr<Expression>& e2, float value);
    std::shared_ptr<Expression> e1;
    std::shared_ptr<Expression> e2;
private:
    virtual std::vector<std::shared_ptr<Expression>> children() const override;
};

class UnaryOperation : public Expression {
public:
    virtual ~UnaryOperation() = default;
protected:
    UnaryOperation(const std::shared_ptr<Expression>& subexpr, float value);
    std::shared_ptr<Expression> subexpr;
private:
    virtual std::vector<std::shared_ptr<Expression>> children() const override;
};

class Addition : public BinaryOperation {
friend Engine::Expression Engine::add(const Engine::Expression& e1, const Engine::Expression& e2);
friend Engine::Expression Engine::add(float val, const Engine::Expression& e2);
friend Engine::Expression Engine::add(const Engine::Expression& e1, float val);
private:
    Addition(const std::shared_ptr<Expression>& e1, const std::shared_ptr<Expression>& e2);
    virtual void updatePartials() override;
};

class Subtraction : public BinaryOperation {
friend Engine::Expression Engine::sub(const Engine::Expression& e1, const Engine::Expression& e2);
friend Engine::Expression Engine::sub(float val, const Engine::Expression& e2);
friend Engine::Expression Engine::sub(const Engine::Expression& e1, float val);
private:
    Subtraction(const std::shared_ptr<Expression>& e1, const std::shared_ptr<Expression>& e2);
    virtual void updatePartials() override;
};

class Multiplication : public BinaryOperation {
friend Engine::Expression Engine::mult(const Engine::Expression& e1, const Engine::Expression& e2);
friend Engine::Expression Engine::mult(float val, const Engine::Expression& e2);
friend Engine::Expression Engine::mult(const Engine::Expression& e1, float val);
private:
    Multiplication(const std::shared_ptr<Expression>& e1, const std::shared_ptr<Expression>& e2);
    virtual void updatePartials() override;
};

class Division : public BinaryOperation {
friend Engine::Expression Engine::div(const Engine::Expression& e1, const Engine::Expression& e2);
friend Engine::Expression Engine::div(float val, const Engine::Expression& e2);
friend Engine::Expression Engine::div(const Engine::Expression& e1, float val);
private:
    Division(const std::shared_ptr<Expression>& e1, const std::shared_ptr<Expression>& e2);
    virtual void updatePartials() override;
};

class Square : public UnaryOperation {
friend Engine::Expression Engine::square(const Engine::Expression& ex);
private:
    Square(const std::shared_ptr<Expression>& subexpr);
    virtual void updatePartials() override;
};

class Sigmoid : public UnaryOperation {
friend Engine::Expression Engine::sigmoid(const Engine::Expression& ex);
private:
    Sigmoid(const std::shared_ptr<Expression>& subexpr);
    virtual void updatePartials() override;
};

class Relu : public UnaryOperation {
friend Engine::Expression Engine::relu(const Engine::Expression& ex);
private:
    Relu(const std::shared_ptr<Expression>& subexpr);
    virtual void updatePartials();
};

class Exp : public UnaryOperation {
friend Engine::Expression Engine::exp(const Engine::Expression& ex);
private:
    Exp(const std::shared_ptr<Expression>& subexpr);
    virtual void updatePartials() override;
};

class Log : public UnaryOperation {
friend Engine::Expression Engine::log(const Engine::Expression& ex);
private:
    Log(const std::shared_ptr<Expression>& subexpr);
    virtual void updatePartials() override;
};

class ReduceAdd : public ReductionOperation {
friend Engine::Expression Engine::reduceAdd(const Tensor<Engine::Expression>& expressions);
private:
    ReduceAdd(std::vector<std::shared_ptr<Expression>>&& data);
    virtual void updatePartials() override;
};

}

#endif