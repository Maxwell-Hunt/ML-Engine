#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"

class Variable : public Expression {
public:
    Variable(float value);

    virtual float getValue() const override;
    virtual float getPartial() const override;

private:
    float value, partial;
};

#endif