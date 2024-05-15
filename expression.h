#ifndef __EXPRESSION__
#define __EXPRESSION__

#include <memory>

class Context;

namespace Internal {

class Expression {
friend class ::Context;
public:
    virtual ~Expression() = default;
    float getValue() { return value; }
    Context& getContext() const { return context; }

protected:
    Expression(Context& context, float value) : context{context}, value{value}, partial{0} {}

    virtual float getPartial() const { return partial; }

    void addToPartial(Expression& other, float value) const { other.partial += value; }

    void propagate(Expression& other) { other.backPropagateInternal(); }


    Context& context;
private:
    void backPropagate() {
        partial = 1;
        backPropagateInternal();
    }

    virtual void backPropagateInternal() = 0;
    float value;
    float partial;
};

}

class Expression {
public:
    Expression() : expr{nullptr} {}
    Expression(Internal::Expression* expr) : expr{expr} {}

    Expression(const Expression& expr) = delete;
    Expression(Expression&& expr) : expr{(expr.expr)} {}

    Expression& operator=(const Expression&) = delete;
    Expression& operator=(Expression&& expr) { this->expr = std::move(expr.expr); return *this; }

    Context& getContext() const { return expr->getContext(); }

    float getValue() const { return expr->getValue(); }
    std::shared_ptr<Internal::Expression> getData() const { return expr; }

private:
    std::shared_ptr<Internal::Expression> expr;
};

#endif