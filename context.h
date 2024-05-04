#ifndef __CONTEXT__
#define __CONTEXT__

#include "variable.h"
#include "operations.h"
#include <unordered_set>
#include <stdexcept>

class Context {
public:
    ~Context();

    const Expression& createVariable(float val);

    const Expression& add(const Expression& e1, const Expression& e2);
    const Expression& add(float val, const Expression& e2);
    const Expression& add(const Expression& e1, float val);

    const Expression& sub(const Expression& e1, const Expression& e2);
    const Expression& sub(float val, const Expression& e2);
    const Expression& sub(const Expression& e1, float val);

    const Expression& mult(const Expression& e1, const Expression& e2);
    const Expression& mult(float val, const Expression& e2);
    const Expression& mult(const Expression& e1, float val);

    const Expression& div(const Expression& e1, const Expression& e2);
    const Expression& div(float val, const Expression& e2);
    const Expression& div(const Expression& e1, float val);
    
private:
    void checkExpressions(const Expression& e1, const Expression& e2) const;

    std::unordered_set<const Expression*> expressions;
};

#endif