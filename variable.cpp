#include "variable.h"

Internal::Variable::Variable(float value) : Internal::Expression{value} {}

void Internal::Variable::updatePartials() {}
std::vector<std::shared_ptr<Internal::Expression>> Internal::Variable::children() const { 
    return std::vector<std::shared_ptr<Internal::Expression>>(); 
}