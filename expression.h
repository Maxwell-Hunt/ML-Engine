#ifndef __EXPRESSION__
#define __EXPRESSION__

class Expression {
public:
    virtual float getValue() const = 0;
    virtual float getPartial() const = 0;
};

#endif