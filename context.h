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
    Vector createVector(std::size_t size, float value);

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

    Vector add(const Vector& a, const Vector& b);
    Vector sub(const Vector& a, const Vector& b);
    Vector mult(const Vector& a, const Expression& b);
    Vector mult(const Expression& a, const Vector& b);
    Vector div(const Vector& a, const Expression& b);

    // TODO: This needs to be implemented
    Expression dot(const Vector& a, const Vector& b);

private:
    void checkExpressions(const Expression& e1, const Expression& e2) const;
    void checkVectors(const Vector& a, const Vector& b) const;

    VectorBehaviour vb;
    std::unordered_set<const Internal::Expression*> expressions;
};

#endif