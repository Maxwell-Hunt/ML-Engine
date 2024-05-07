#ifndef __EXPRESSION__
#define __EXPRESSION__
class Context;

namespace Internal {

class Expression {
protected:
    Expression(Context& context) : context{context} {}
    Context& context;
public:
    virtual ~Expression() {}
    virtual float getValue() const = 0;
    virtual float getPartial(const Expression& other) const = 0;
    Context& getContext() const { return context; }
};

}

class Expression {
public:
    Expression() : expr{nullptr} {}
    Expression(const Internal::Expression& expr) : expr{&expr} {}

    Expression(const Expression& expr) = delete;
    Expression(Expression&& expr) : expr{(expr.expr)} {}

    Expression& operator=(const Expression&) = delete;
    Expression& operator=(Expression&& expr) { this->expr = expr.expr; return *this; }

    Context& getContext() const { return expr->getContext(); }

    float getValue() const { return expr->getValue(); }
    float getPartial(const Expression& other) const { return expr->getPartial(other.data()); }

    const Internal::Expression& data() const { return *expr; }

private:
    const Internal::Expression* expr;
};

#endif