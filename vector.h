#ifndef __VECTOR__
#define __VECTOR__

#include <vector>

#include "expression.h"

// TODO :
//          Eventually I would like to be able to pass a Distribution object into Vector which
//          allows the user to specify a probability distribution with which everything is generated

class Context;

class Vector {
friend class Context;
public:
    // Note this is NOT a constant time function as it requires calling get_value()
    float at(std::size_t index) const;

    std::vector<Expression>::iterator begin();
    std::vector<Expression>::iterator end();
private:
    Vector(std::size_t size, bool zeros, Context& context);
    Vector(std::vector<Expression>&& data, Context& context);

    std::size_t size;
    std::vector<Expression> data;
    Context& context;
};

#endif