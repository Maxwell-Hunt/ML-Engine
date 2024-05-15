#include "variable.h"

Variable::Variable(float value, Context& context) : Internal::Expression{context, value} {}

void Variable::backPropagateInternal() {}