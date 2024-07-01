#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"

class Variable : public Internal::Expression {
friend class Context;
private:
    Variable(float value, Context& context);
    virtual void updatePartials() override;
    virtual const std::vector<std::shared_ptr<Internal::Expression>> children() const override;
};

#endif