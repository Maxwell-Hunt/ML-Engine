#ifndef __EXPRESSION__
#define __EXPRESSION__

#include <vector>
#include <memory>
#include <unordered_set>
#include <stack>
#include "tensor.h"

template <typename T>
concept Floating = std::is_floating_point<T>::value;


// The reasoning behind having this ExpressionBase is that we don't necessarily
// know the shape of the tensors of the children of any given Expression and
// so we don't know the templates for its type.
class ExpressionBase {
protected:
    using Children = std::pair<ExpressionBase*, ExpressionBase*>;
    virtual Children children() const = 0;
    virtual void updatePartials() = 0;
};

template <Floating T, std::size_t ...Dims>
class Expression : public ExpressionBase { 
public:
    virtual ~Expression() = default;
    const Tensor<T, Dims...>& value() const { return _data; }
    const Tensor<T, Dims...>& partials() const { return _gradients; }
protected:
    Expression(Tensor<T, Dims...>&& value) : _data{std::move(value)} {}

    void addToPartial(const std::shared_ptr<Expression>& ex, const Tensor<T, Dims...>& value) {
        ex->_gradients = ex->_gradients + value;
    }

private:
    void backPropagate() {
        std::fill(_gradients.begin(), _gradients.end(), 1);
        std::stack<ExpressionBase*> s;
        std::unordered_set<ExpressionBase*> visited;

        buildtopo(this, s, visited);

        while(!s.empty()) {
            // Think of ways to avoid using the static cast here.  Also note that this
            // might be the source of potential issues in the future so keep an eye out
            // for this
            ExpressionBase* ex = s.top(); s.pop();
            ex->updatePartials();
        }
    }

    void buildTopo(ExpressionBase* ex, std::stack<ExpressionBase*>& s, std::unordered_set<ExpressionBase*>& visited) const {
        if(visited.count(ex)) return;
        visited.insert(ex);
        
        // Vist children (there should not be more than 2 of them) 
        auto c = children();
        if(c.first)  buildTopo(c.first, s, visited);
        if(c.second) buildTopo(c.second, s, visited);

        s.push(ex);
    }

    Tensor<T, Dims...> _data;
    Tensor<T, Dims...> _gradients;
};

template <Floating T, std::size_t ...Dims>
class Variable {
public:
    Variable(Expression<T, Dims...>* data) : _data{std::shared_ptr<Expression<T, Dims...>>(data)} {}
    const Tensor<T, Dims...>& value() const { return _data->value(); }
    const Tensor<T, Dims...>& partials() const { return _data->partials(); }
    std::shared_ptr<Expression<T, Dims...>> get() const { return _data; }
private:
    std::shared_ptr<Expression<T, Dims...>> _data;
};


#endif