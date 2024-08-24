#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"
#include "defs.h"

#include <cmath>
#include <numeric>

template <typename T>
class ConstExpression : public Expression<T> {
friend Variable<float> createScalarVariable(float value);

template <std::size_t ...Dims>
friend Variable<Tensor<float, Dims...>> createTensorVariable();

template <std::size_t ...Dims>
friend Variable<Tensor<float, Dims...>> createRandomTensorVariable();

private:
    ConstExpression(T&& value) : Expression<T>(std::move(value)) {}
    virtual ExpressionBase::Children children() const final override { return {}; }
    virtual void updatePartials() final override {}
};

template <typename T>
class UnaryExpression : public Expression<T> {
protected:
    UnaryExpression(std::shared_ptr<Expression<T>> subexpr, T&& value) : 
        Expression<T>{std::move(value)},
        subexpr{std::move(subexpr)}
        {}
private:
    virtual ExpressionBase::Children children() const final override { return {subexpr.get(), nullptr}; }

protected:
    std::shared_ptr<Expression<T>> subexpr;
};

template <typename T, typename H>
class BinaryExpression : public Expression<T> {
protected:
    BinaryExpression(std::shared_ptr<Expression<T>> a, std::shared_ptr<Expression<H>> b, T&& value) :
        Expression<T>{std::move(value)},
        childA{std::move(a)},
        childB{std::move(b)}
        {}
private:
    virtual ExpressionBase::Children children() const final override { return {childA.get(), childB.get()}; }
protected:
    std::shared_ptr<Expression<T>> childA;
    std::shared_ptr<Expression<H>> childB;
};

// TODO: There is a good amount of repeated code between this class
// and UnaryExpression.  Think about how this can be fixed
template <TensorType T, Floating F>
class ReductionExpression : public Expression<F> {
protected:
    ReductionExpression(std::shared_ptr<Expression<T>> subexpr, F&& value) : 
        Expression<F>{std::move(value)},
        subexpr{std::move(subexpr)}
        {}
private:
    // THIS DOES NOT WORK
    virtual ExpressionBase::Children children() const final override { return {subexpr.get(), nullptr}; }

protected:
    std::shared_ptr<Expression<T>> subexpr;
};

template <typename T>
class Square : public UnaryExpression<T> {
friend Variable<T> square<>(const Variable<T>& v);
private:
    Square(std::shared_ptr<Expression<T>> subexpr) : 
        UnaryExpression<T>{subexpr, subexpr->value() * subexpr->value()} {}

    virtual void updatePartials() override {
        this->addToPartial(this->subexpr, 2.f * this->subexpr->value() * this->partials());
    }
};

template <typename T>
class Exp : public UnaryExpression<T> {
friend Variable<T> exp<>(const Variable<T>& v);
private:
    T findValue(const std::shared_ptr<Expression<T>>& subexpr) const requires TensorType<T> {
        return subexpr->value().map([](const auto& x) { return std::exp(x); });
    }

    T findValue(const std::shared_ptr<Expression<T>>& subexpr) const requires Floating<T> {
        return std::exp(subexpr->value());
    }

    Exp(std::shared_ptr<Expression<T>> subexpr) :
        UnaryExpression<T>{subexpr, findValue(subexpr)} {}

    virtual void updatePartials() override {
        this->addToPartial(this->subexpr, this->value() * this->partials());
    }
};

template <typename T, typename H>
class Addition : BinaryExpression<T, H> {
    friend Variable<T> operator+<>(const Variable<T>& a, const Variable<H>& b);
private:
    Addition(std::shared_ptr<Expression<T>> a, std::shared_ptr<Expression<H>> b) :
        BinaryExpression<T, H>{a, b, a->value() + b->value()} {}

    void updatePartialsHelper() requires TensorType<T> {
        this->addToPartial(this->childA, this->partials());
        this->addToPartial(this->childB, this->partials().template narrowCast<H>());
    }

    void updatePartialsHelper() requires Floating<T> {
        this->addToPartial(this->childA, this->partials());
        this->addToPartial(this->childB, this->partials());
    }

    virtual void updatePartials() override {
        updatePartialsHelper();
    }
};

template <typename T, typename H>
class Subtraction : BinaryExpression<T, H> {
    friend Variable<T> operator-<>(const Variable<T>& a, const Variable<H>& b);
private:
    Subtraction(std::shared_ptr<Expression<T>> a, std::shared_ptr<Expression<H>> b) :
        BinaryExpression<T, H>{a, b, a->value() - b->value()} {}

    void updatePartialsHelper() requires TensorType<T> {
        this->addToPartial(this->childA, this->partials());
        this->addToPartial(this->childB, this->partials().template narrowCast<H>() * -1);
    }

    void updatePartialsHelper() requires Floating<T> {
        this->addToPartial(this->childA, this->partials());
        this->addToPartial(this->childB, this->partials() * -1);
    }

    virtual void updatePartials() override {
        updatePartialsHelper();
    }
};

template <typename T, typename H>
class Multiplication : BinaryExpression<T, H> {
    friend Variable<T> operator*<>(const Variable<T>& a, const Variable<H>& b);
private:
    Multiplication(std::shared_ptr<Expression<T>> a, std::shared_ptr<Expression<H>> b) :
        BinaryExpression<T, H>{a, b, a->value() * b->value()} {}

    void updatePartialsHelper() requires TensorType<T> {
        this->addToPartial(this->childA, this->partials() * this->childB->value());
        this->addToPartial(this->childB, (this->partials() * this->childA->value()).template narrowCast<H>());
    }

    void updatePartialsHelper() requires Floating<T> {
        this->addToPartial(this->childA, this->partials() * this->childB->value());
        this->addToPartial(this->childB, this->partials() * this->childA->value());
    }
    
    virtual void updatePartials() override {
        updatePartialsHelper();
    }
};

template <typename T, typename H>
class Division : BinaryExpression<T, H> {
    friend Variable<T> operator/<>(const Variable<T>& a, const Variable<H>& b);
private:
    Division(std::shared_ptr<Expression<T>> a, std::shared_ptr<Expression<H>> b) :
        BinaryExpression<T, H>{a, b, a->value() / b->value()} {}

    void updatePartialsHelper() requires TensorType<T> {
        H reciporicalB;
        std::transform(
            this->childB->value().begin(),
            this->childB->value().end(),
            reciporicalB.begin(),
            [](float v) { return 1.0 / v; });

        // d/dx (x/y) = 1/y
        this->addToPartial(this->childA, this->partials() * reciporicalB);

        // d/dy (x/y) = -x / y^2
        T intermediate = this->partials() * -1 * this->childA->value() * reciporicalB * reciporicalB;
        this->addToPartial(this->childB, intermediate.template narrowCast<H>());
    }

    void updatePartialsHelper() requires Floating<T> {
        this->addToPartial(this->childA, this->partials() * 1 / this->childB->value());

        T negRecipSquared = -1 / (this->childB->value() * this->childB->value());
        this->addToPartial(this->childB, this->partials() * this->childA->value() * negRecipSquared);
    }

    virtual void updatePartials() override {
        updatePartialsHelper();
    }
};

template <TensorType T, Floating F>
class ReduceAdd : ReductionExpression<T, F> {
friend Variable<T> reduceAdd(const Variable<T>& t)
private:
    ReduceAdd(std::shared_ptr<Expression<T>> subexpr) :
        ReductionExpression<T, F>(subexpr, std::accumulate(subexpr.begin(), subexpr.end(), 0, std::plus<F>())) {}
    
    virtual void updatePartials() override final {
        this->addToPartial(this->subexpr, this->partials());
    }
};

#endif