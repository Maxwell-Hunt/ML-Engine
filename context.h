#ifndef __CONTEXT__
#define __CONTEXT__

#include "variable.h"
#include "operations.h"
#include "vector.h"
#include <unordered_set>
#include <stdexcept>

class Context {
public:
    ~Context();

    Expression createVariable(float val);
    Vector createVector(std::size_t size, bool isZero);

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

private:
    void checkExpressions(const Expression& e1, const Expression& e2) const;

    std::unordered_set<const Internal::Expression*> expressions;
};

#endif