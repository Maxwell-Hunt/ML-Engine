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

#endif