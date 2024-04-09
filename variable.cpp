#include "variable.h"

Variable::Variable(float value) : value{value} {}

float Variable::getValue() const {
    return value;
}

float Variable::getPartial(const Expression& x) const {
    if(&x == this) return 1;
    return 0;
}