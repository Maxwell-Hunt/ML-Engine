#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"

class Variable : public Expression {
public:
    Variable(float value);
    virtual ~Variable() {}

    virtual float getValue() const override;
    virtual float getPartial(const Expression& x) const override;

private:
    float value;
};

#endif