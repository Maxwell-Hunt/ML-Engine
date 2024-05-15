#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"

class Variable : public Internal::Expression {
friend class Context;
public:
    virtual float getValue() const override;
private:
    Variable(float val, Context& context);
    virtual void backPropagateInternal() override;
    float val;
};

#endif