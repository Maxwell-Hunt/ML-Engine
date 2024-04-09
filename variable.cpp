#include "variable.h"

Variable::Variable(float value) : value{value}, partial{0} {}

float Variable::getValue() const {
    return value;
}

float Variable::getPartial() const {
    return partial;
}