#ifndef __EXPRESSION__
#define __EXPRESSION__

class Expression {
public:
    virtual ~Expression() {}
    virtual float getValue() const = 0;
    virtual float getPartial(const Expression& x) const = 0;
};

#endif