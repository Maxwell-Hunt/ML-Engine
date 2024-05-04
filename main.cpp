#include <unordered_set>
#include <iostream>

class Context;

class Expression {
protected:
    Expression(Context& context) : context{context} {}
    Context& context;
public:
    virtual ~Expression() {}
    virtual float getValue() const = 0;
    virtual float getPartial(const Expression& other) const = 0;
    Context& getContext() const { return context; }
};

class Variable : public Expression {
friend class Context;
public:
    virtual float getValue() const override { return val; }
    virtual float getPartial(const Expression& other) const override {
        if(this == &other) return 1;
        return 0;
    }
private:
    Variable(float val, Context& context) : Expression{context}, val{val} {}
    float val;
};

class BinaryOperation : public Expression {
protected:
    BinaryOperation(const Expression& e1, const Expression& e2, Context& context) : Expression{context}, e1{e1}, e2{e2} {}
    const Expression& e1;
    const Expression& e2;
};

class Addition : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override {
        return e1.getValue() + e2.getValue();
    }

    virtual float getPartial(const Expression& other) const override {
        return e1.getPartial(other) + e2.getPartial(other);
    }

private:
    Addition(const Expression& e1, const Expression& e2, Context& context) : BinaryOperation{e1, e2, context} {}
};

class Subtraction : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override {
        return e1.getValue() - e2.getValue();
    }

    virtual float getPartial(const Expression& other) const override {
        return e1.getPartial(other) - e2.getPartial(other);
    }
private:
    Subtraction(const Expression& e1, const Expression& e2, Context& context) : BinaryOperation{e1, e2, context} {}
};

class Multiplication : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override {
        return e1.getValue() * e2.getValue();
    }

    virtual float getPartial(const Expression& other) const override {
        return e1.getPartial(other) * e2.getValue() + e2.getPartial(other) * e1.getValue();
    }
private:
    Multiplication(const Expression& e1, const Expression& e2, Context& context) : BinaryOperation{e1, e2, context} {}
};

class Division : public BinaryOperation {
friend class Context;
public:
    virtual float getValue() const override {
        return e1.getValue() / e2.getValue();
    }

    virtual float getPartial(const Expression& other) const override {
        float e1Value = e1.getValue();
        float e2Value = e2.getValue();
        float e1Partial = e1.getPartial(other);
        float e2Partial = e2.getPartial(other);
        return (e1Partial* e2Value - e2Partial * e1Value) / (e2Value * e2Value);
    }
private:
    Division(const Expression& e1, const Expression& e2, Context& context) : BinaryOperation{e1, e2, context} {}
};

class Context {
public:
    ~Context() {
        for(const Expression* ex : expressions) delete ex;
    }

    const Expression& createVariable(float val) {
        Variable* v = new Variable(val, *this);
        expressions.insert(v);
        return *v;
    }

    const Expression& add(const Expression& e1, const Expression& e2) {
        checkExpressions(e1, e2);

        Addition* sum = new Addition(e1, e2, *this);
        expressions.insert(sum);
        return *sum;
    }

    const Expression& add(float val, const Expression& e2) {
        const Expression& e1 = createVariable(val);
        return add(e1, e2);
    }

    const Expression& add(const Expression& e1, float val) {
        const Expression& e2 = createVariable(val);
        return add(e1, e2);
    }

    const Expression& sub(const Expression& e1, const Expression& e2) {
        checkExpressions(e1, e2);
        Subtraction* diff = new Subtraction(e1, e2, *this);
        expressions.insert(diff);
        return *diff;
    }

    const Expression& sub(float val, const Expression& e2) {
        const Expression& e1 = createVariable(val);
        return sub(e1, e2);
    }

    const Expression& sub(const Expression& e1, float val) {
        const Expression& e2 = createVariable(val);
        return sub(e1, e2);
    }

    const Expression& mult(const Expression& e1, const Expression& e2) {
        checkExpressions(e1, e2);
        Multiplication* prod = new Multiplication(e1, e2, *this);
        expressions.insert(prod);
        return *prod;
    }

    const Expression& mult(float val, const Expression& e2) {
        const Expression& e1 = createVariable(val);
        return mult(e1, e2);
    }

    const Expression& mult(const Expression& e1, float val) {
        const Expression& e2 = createVariable(val);
        return mult(e1, e2);
    }

    const Expression& div(const Expression& e1, const Expression& e2) {
        checkExpressions(e1, e2);
        Division* quotient = new Division(e1, e2, *this);
        expressions.insert(quotient);
        return *quotient;
    }

    const Expression& div(float val, const Expression& e2) {
        const Expression& e1 = createVariable(val);
        return div(e1, e2);
    }

    const Expression& div(const Expression& e1, float val) {
        const Expression& e2 = createVariable(val);
        return div(e1, e2);
    }

private:

    void checkExpressions(const Expression& e1, const Expression& e2) {
        if(!(expressions.count(&e1) && expressions.count(&e2))) {
            throw std::runtime_error("WHY ARE YOU DOING THIS???");
        }
    }

    std::unordered_set<const Expression*> expressions;
};

const Expression& operator+(const Expression& e1, const Expression& e2) {
    return e1.getContext().add(e1, e2);
}

const Expression& operator+(const Expression& e1, float val) {
    return e1.getContext().add(e1, val);
}

const Expression& operator+(float val, const Expression& e2) {
    return e2.getContext().add(val, e2);
} 

const Expression& operator-(const Expression& e1, const Expression& e2) {
    return e1.getContext().sub(e1, e2);
}

const Expression& operator-(const Expression& e1, float val) {
    return e1.getContext().sub(e1, val);
}

const Expression& operator-(float val, const Expression& e2) {
    return e2.getContext().sub(val, e2);
} 

const Expression& operator*(const Expression& e1, const Expression& e2) {
    return e1.getContext().mult(e1, e2);
}

const Expression& operator*(const Expression& e1, float val) {
    return e1.getContext().mult(e1, val);
}

const Expression& operator*(float val, const Expression& e2) {
    return e2.getContext().mult(val, e2);
}

const Expression& operator/(const Expression& e1, const Expression& e2) {
    return e1.getContext().div(e1, e2);
}

const Expression& operator/(const Expression& e1, float val) {
    return e1.getContext().div(e1, val);
}

const Expression& operator/(float val, const Expression& e2) {
    return e2.getContext().div(val, e2);
} 

int main() {
    Context context;
    const Expression& v = context.createVariable(2);
    const Expression& w = context.createVariable(3);

    const Expression& result = (v - w) / 5 * v + 2;

    std::cout << v.getValue() << ' ' << w.getValue() << ' ' << result.getValue() << ' ' << result.getPartial(v);
}