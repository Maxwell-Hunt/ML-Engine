#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"

class Variable : public Internal::Expression {
friend class Context;
private:
    Variable(float value, Context& context);
    virtual void backPropagateInternal() override;
};

#endif