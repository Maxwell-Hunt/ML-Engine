#ifndef __EXPRESSION__
#define __EXPRESSION__

#include <memory>
#include <stack>
#include <vector>
#include <unordered_set>

class Context;

namespace Internal {

class Expression : public std::enable_shared_from_this<Expression> {
friend class ::Context;
public:
    virtual ~Expression() = default;
    float getValue() { return value; }
    virtual float getPartial() const { return partial; }
    Context& getContext() const { return context; }

protected:
    Expression(Context& context, float value) : context{context}, value{value}, partial{0} {}

    void addToPartial(Expression& other, float value) const { other.partial += value; }

    Context& context;
private:
    void backPropagate() {
        partial = 1;
        std::stack<std::shared_ptr<Expression>> s;
        std::unordered_set<std::shared_ptr<Expression>> visited;

        buildTopo(shared_from_this(), s, visited);

        while(!s.empty()) {
            std::shared_ptr<Expression> ex = s.top(); s.pop();
            ex->updatePartials();
        }
    }

    void buildTopo(const std::shared_ptr<Expression>& ex, std::stack<std::shared_ptr<Expression>>& s, std::unordered_set<std::shared_ptr<Expression>>& visited) const {
        if(visited.count(ex)) return;
        visited.insert(ex);
        for(const std::shared_ptr<Expression>& next : ex->children()) {
            buildTopo(next, s, visited);
        }
        s.push(ex);
    }

    virtual void updatePartials() = 0;
    virtual const std::vector<std::shared_ptr<Expression>> children() const = 0;
    float value;
    float partial;
};

}

class Expression {
public:
    Expression() : expr{nullptr} {}
    Expression(Internal::Expression* expr) : expr{expr} {}

    Expression(const Expression& expr) = delete;
    Expression(Expression&& expr) noexcept : expr{(expr.expr)} {}

    Expression& operator=(const Expression&) = delete;
    Expression& operator=(Expression&& expr) { this->expr = std::move(expr.expr); return *this; }

    Context& getContext() const { return expr->getContext(); }

    float getValue() const { return expr->getValue(); }
    float getPartial() const { return expr->getPartial(); }
    std::shared_ptr<Internal::Expression> getData() const { return expr; }

private:
    std::shared_ptr<Internal::Expression> expr;
};

#endif