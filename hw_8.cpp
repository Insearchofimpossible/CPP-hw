#include <iostream>
#include <string>
#include <map>

class Expression {
public:
    virtual ~Expression() = default;
    virtual void print() const = 0;
    virtual int calculate(const std::map<std::string, int>& context) const = 0;
    virtual bool isFlyweight() const { return false; }
};

class Constant : public Expression {
private:
    int value;
public:
    Constant(int v) : value(v) {}
    void print() const override { std::cout << value; }
    int calculate(const std::map<std::string, int>&) const override { return value; }
    bool isFlyweight() const override { return true; }
};

class Variable : public Expression {
private:
    std::string name;
public:
    Variable(const std::string& n) : name(n) {}
    void print() const override { std::cout << name; }
    int calculate(const std::map<std::string, int>& context) const override {
        auto it = context.find(name);
        return (it != context.end()) ? it->second : 0;
    }
    bool isFlyweight() const override { return true; }
};

class Addition : public Expression {
private:
    Expression* left;
    Expression* right;
public:
    Addition(Expression* l, Expression* r) : left(l), right(r) {}
    ~Addition() {
        if (left && !left->isFlyweight()) delete left;
        if (right && !right->isFlyweight()) delete right;
    }
    void print() const override {
        left->print();
        std::cout << " + ";
        right->print();
    }
    int calculate(const std::map<std::string, int>& context) const override {
        return left->calculate(context) + right->calculate(context);
    }
};

class ExpressionFactory {
private:
    std::map<int, Constant*> constants;
    std::map<std::string, Variable*> variables;

public:
    ExpressionFactory() {
        for (int i = -5; i <= 256; ++i) {
            constants[i] = new Constant(i);
        }
    }

    ~ExpressionFactory() {
        for (auto& pair : constants) {
            delete pair.second;
        }
        for (auto& pair : variables) {
            delete pair.second;
        }
    }

    Constant* createConstant(int value) {
        auto it = constants.find(value);
        if (it != constants.end()) {
            return it->second;
        }
        Constant* c = new Constant(value);
        constants[value] = c;
        return c;
    }

    Variable* createVariable(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second;
        }
        Variable* v = new Variable(name);
        variables[name] = v;
        return v;
    }

    void removeConstant(int value) {
        if (value >= -5 && value <= 256) return;
        auto it = constants.find(value);
        if (it != constants.end()) {
            delete it->second;
            constants.erase(it);
        }
    }

    void removeVariable(const std::string& name) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            delete it->second;
            variables.erase(it);
        }
    }
};

int main() {
    ExpressionFactory factory;
    
    Constant *c = factory.createConstant(2);
    Variable *v = factory.createVariable("x");
    Addition *expression = new Addition(c, v);
    
    std::map<std::string, int> context; 
    context["x"] = 3;
    
    std::cout << expression->calculate(context) << std::endl;
    
    delete expression; 
    
    return 0;
}