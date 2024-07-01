#include "variable.h"

Variable::Variable(float value, Context& context) : Internal::Expression{context, value} {}

void Variable::updatePartials() {}
const std::vector<std::shared_ptr<Internal::Expression>> Variable::children() const { return std::vector<std::shared_ptr<Internal::Expression>>(); }