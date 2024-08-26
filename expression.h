#ifndef __EXPRESSION__
#define __EXPRESSION__

#include <vector>
#include <memory>
#include <unordered_set>
#include <stack>
#include <iostream>
#include "tensor.h"

template <typename T>
concept Floating = std::is_floating_point<T>::value;

template <typename T>
class Variable;

// The reasoning behind having this ExpressionBase is that we don't necessarily
// know the shape of the tensors of the children of any given Expression and
// so we don't know the templates for its type.
class ExpressionBase {
template <typename T>
friend void computeGradients(const Variable<T>& ex);
protected:
    using Children = std::pair<ExpressionBase*, ExpressionBase*>;
    virtual Children children() const = 0;
private:
    virtual void updatePartials() = 0;
    virtual void initializeGradients() = 0;

    void backPropagate() {
        initializeGradients();
        std::stack<ExpressionBase*> s;
        std::unordered_set<ExpressionBase*> visited;

        buildTopo(this, s, visited);

        while(!s.empty()) {
            ExpressionBase* ex = s.top(); s.pop();
            ex->updatePartials();
        }
    }

    void buildTopo(ExpressionBase* ex, std::stack<ExpressionBase*>& s, std::unordered_set<ExpressionBase*>& visited) const {
        if(visited.count(ex)) return;
        visited.insert(ex);
        
        // Vist children (there should not be more than 2 of them) 
        Children c = ex->children();
        if(c.first)  buildTopo(c.first, s, visited);
        if(c.second) buildTopo(c.second, s, visited);

        s.push(ex);
    }
};

template <typename T>
class Expression : public ExpressionBase {
public:
    virtual ~Expression() = default;
    const T& value() const { return _data; }
    const T& partials() const { return _gradients; }

    // I would like to not expose this function if possible but for now
    // it shall remain this way
    void setPartials(const T& updated) {
        _gradients = updated;
    }
protected:
    Expression(T&& value) : _data{std::move(value)} {
        _gradients = 0.f;
    }

    template <typename H, typename K>
    void addToPartial(const std::shared_ptr<Expression<H>>& ex, const K& value) {
        ex->setPartials(ex->partials() + value);
    }

private:
    virtual void initializeGradients() override final {
        _gradients = 1;
    }

    T _data;
    T _gradients;
};

template <typename T>
class Variable {
public:
    Variable(Expression<T>* data) : _data{std::shared_ptr<Expression<T>>(data)} {}
    const T& value() const { return _data->value(); }
    const T& partials() const { return _data->partials(); }
    std::shared_ptr<Expression<T>> get() const { return _data; }
private:
    std::shared_ptr<Expression<T>> _data;
};


#endif