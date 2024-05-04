#ifndef __EXPRESSION__
#define __EXPRESSION__
class Context;

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
#endif