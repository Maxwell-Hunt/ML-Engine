#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"

class Variable : public Internal::Expression {
friend class Context;
public:
    virtual float getValue() const override;
    virtual float getPartial(const Internal::Expression& other) const override;
private:
    Variable(float val, Context& context);
    float val;
};

#endif