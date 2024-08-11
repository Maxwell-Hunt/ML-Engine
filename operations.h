#ifndef __OPERATIONS__
#define __OPERATIONS__

#include "expression.h"
#include "defs.h"

template <Floating T, std::size_t ...Dims>
class ConstExpression : public Expression<T, Dims...> {
friend Variable<T, Dims...> createVariable<>();
friend Variable<T, Dims...> createRandomVariable<>();
private:
    ConstExpression(Tensor<T, Dims...>&& value) : Expression<T, Dims...>(std::move(value)) {}
    virtual ExpressionBase::Children children() const override { return {}; }
    virtual void updatePartials() override {}
};

template <Floating T, std::size_t ...Dims>
class UnaryExpression : public Expression<T, Dims...> {
protected:
    UnaryExpression(std::shared_ptr<Expression<T, Dims...>> subexpr, Tensor<T, Dims...>&& value) : 
        Expression<T, Dims...>{std::move(value)},
        subexpr{std::move(subexpr)}
        {}
private:
    virtual ExpressionBase::Children children() const override { return {subexpr.get(), nullptr}; }

protected:
    std::shared_ptr<Expression<T, Dims...>> subexpr;
};

template <typename T, typename H>
class BinaryExpression {};

template <Floating T, std::size_t ...DimsA, std::size_t ...DimsB>
class BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>> : public Expression<T, DimsA...> {
public:
    using ChildA_t = Expression<T, DimsA...>;
    using ChildB_t = Expression<T, DimsB...>;
protected:
    BinaryExpression(std::shared_ptr<ChildA_t> a, std::shared_ptr<ChildB_t> b, Tensor<T, DimsA...>&& value) :
        Expression<T, DimsA...>{std::move(value)},
        childA{std::move(a)},
        childB{std::move(b)}
        {}
private:
    virtual ExpressionBase::Children children() const override { return {childA.get(), childB.get()}; }
protected:
    std::shared_ptr<ChildA_t> childA;
    std::shared_ptr<ChildB_t> childB;
};

template <Floating T, std::size_t ...Dims>
class Square : public UnaryExpression<T, Dims...> {
friend Variable<T, Dims...> square<>(const Variable<T, Dims...>& v);
private:
    Square(std::shared_ptr<Expression<T, Dims...>> subexpr) : 
        UnaryExpression<T, Dims...>{subexpr, subexpr->value() * subexpr->value()} {}

    virtual void updatePartials() override {
        this->addToPartial(this->subexpr, 2.f * this->subexpr->value() * this->partials());
    }
};

template <typename A, typename B>
class Addition;

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
class Addition<Expression<T, DimsA...>, Expression<T, DimsB...>> : 
BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>> {
    friend Variable<T, DimsA...> operator+<>(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);
    using Super = BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>>;
private:
    Addition(std::shared_ptr<Expression<T, DimsA...>> a, std::shared_ptr<Expression<T, DimsB...>> b) :
        Super{a, b, a->value() + b->value()} {}

    virtual void updatePartials() override {
        this->addToPartial(this->childA, this->partials());
        this->addToPartial(this->childB, this->partials().template narrowCast<DimsB...>());
    }
};

template <typename A, typename B>
class Subtraction;

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
class Subtraction<Expression<T, DimsA...>, Expression<T, DimsB...>> :
BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>> {
    friend Variable<T, DimsA...> operator-<>(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);
    using Super = BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>>;
private:
    Subtraction(std::shared_ptr<Expression<T, DimsA...>> a, std::shared_ptr<Expression<T, DimsB...>> b) :
        Super{a, b, a->value() - b->value()} {}

    virtual void updatePartials() override {
        this->addToPartial(this->childA, this->partials());
        this->addToPartial(this->childB, this->partials().template narrowCast<DimsB...>() * -1);
    }
};

template <typename A, typename B>
class Multiplication;

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
class Multiplication<Expression<T, DimsA...>, Expression<T, DimsB...>> :
BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>> {
    friend Variable<T, DimsA...> operator*<>(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);
    using Super = BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>>;
private:
    Multiplication(std::shared_ptr<Expression<T, DimsA...>> a, std::shared_ptr<Expression<T, DimsB...>> b) :
        Super{a, b, a->value() * b->value()} {}
    
    virtual void updatePartials() override {
        this->addToPartial(this->childA, this->partials() * this->childB->value());
        this->addToPartial(this->childB, (this->partials() * this->childA->value()).template narrowCast<DimsB...>());
    }
};

template <typename A, typename B>
class Division;

template <Floating T, std::size_t... DimsA, std::size_t... DimsB>
class Division<Expression<T, DimsA...>, Expression<T, DimsB...>> :
BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>> {
    friend Variable<T, DimsA...> operator/<>(const Variable<T, DimsA...>& a, const Variable<T, DimsB...>& b);
    using Super = BinaryExpression<Expression<T, DimsA...>, Expression<T, DimsB...>>;
private:
    Division(std::shared_ptr<Expression<T, DimsA...>> a, std::shared_ptr<Expression<T, DimsB...>> b) :
        Super{a, b, a->value() / b->value()} {}

    virtual void updatePartials() override {
        Tensor<T, DimsB...> reciporicalB;
        std::transform(
            this->childB->value().begin(),
            this->childB->value().end(),
            reciporicalB.begin(),
            [](float v) { return 1.0 / v; });

        // d/dx (x/y) = 1/y
        this->addToPartial(this->childA, this->partials() * reciporicalB);

        // d/dy (x/y) = -x / y^2
        Tensor<T, DimsA...> intermediate = this->partials() * -1 * this->childA->value() * reciporicalB * reciporicalB;
        this->addToPartial(this->childB, intermediate.template narrowCast<DimsB...>());
    }
};

#endif