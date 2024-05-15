#include "variable.h"

Variable::Variable(float val, Context& context) : Internal::Expression{context}, val{val} {}

float Variable::getValue() const { return val; }
void Variable::backPropagateInternal() {}