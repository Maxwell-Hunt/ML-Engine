#include "variable.h"

Variable::Variable(float val, Context& context) : Internal::Expression{context}, val{val} {}

float Variable::getValue() const { return val; }
float Variable::getPartial(const Internal::Expression& other) const {
    if(this == &other) return 1;
    return 0;
}