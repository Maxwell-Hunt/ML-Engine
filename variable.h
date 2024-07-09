#ifndef __VARIABLE__
#define __VARIABLE

#include "expression.h"
#include "context.h"

namespace Internal {
    
class Variable : public Internal::Expression {
friend class Engine::Expression Engine::createVariable(float val);
private:
    Variable(float value);
    virtual void updatePartials() override;
    virtual std::vector<std::shared_ptr<Internal::Expression>> children() const override;
};

}


#endif