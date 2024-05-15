#ifndef __CONTEXT__
#define __CONTEXT__

#include <unordered_set>
#include <stdexcept>
#include "expression.h"

class Context {
public:
    Expression createVariable(float val);

    Expression add(const Expression& e1, const Expression& e2);
    Expression add(float val, const Expression& e2);
    Expression add(const Expression& e1, float val);

    Expression sub(const Expression& e1, const Expression& e2);
    Expression sub(float val, const Expression& e2);
    Expression sub(const Expression& e1, float val);

    Expression mult(const Expression& e1, const Expression& e2);
    Expression mult(float val, const Expression& e2);
    Expression mult(const Expression& e1, float val);

    Expression div(const Expression& e1, const Expression& e2);
    Expression div(float val, const Expression& e2);
    Expression div(const Expression& e1, float val);

    Expression square(const Expression& ex);

    float computeGradients(const Expression& target, const Expression& source);
};


Expression operator+(const Expression& e1, const Expression& e2);
Expression operator+(const Expression& e1, float val);
Expression operator+(float val, const Expression& e2);

Expression operator-(const Expression& e1, const Expression& e2);
Expression operator-(const Expression& e1, float val);
Expression operator-(float val, const Expression& e2);

void operator+=(Expression& e1, const Expression& e2);
void operator-=(Expression& e1, const Expression& e2);

Expression operator*(const Expression& e1, const Expression& e2);
Expression operator*(const Expression& e1, float val);
Expression operator*(float val, const Expression& e2);

Expression operator/(const Expression& e1, const Expression& e2);
Expression operator/(const Expression& e1, float val);
Expression operator/(float val, const Expression& e2);

#endif