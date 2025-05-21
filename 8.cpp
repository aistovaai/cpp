#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

class Expression {
public:
    virtual ~Expression() = default;
    virtual int calculate(const std::map<std::string, int>& context) const = 0;
    virtual void print(std::ostream& os) const = 0;
};

class Constant : public Expression {
    int value;
    friend class ExpressionFactory;

public:
    Constant(int v) : value(v) {}
    int calculate(const std::map<std::string, int>&) const override
    {
        return value;
    }
    void print(std::ostream& os) const override
    {
        os << value;
    }
};

class Variable : public Expression {
    std::string name;
    friend class ExpressionFactory;

public:
    Variable(std::string n) : name(n) {}
    int calculate(const std::map<std::string, int>& context) const override
    {
        auto it = context.find(name);
        if (it != context.end()) return it->second;
        throw std::runtime_error("Variable '" + name + "' not found");
    }
    void print(std::ostream& os) const override
    {
        os << name;
    }
};

class Addition : public Expression
{
    std::shared_ptr<Expression> left, right;

public:
    Addition(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : left(std::move(l)), right(std::move(r)) {}

    int calculate(const std::map<std::string, int>& context) const override
    {
        return left->calculate(context) + right->calculate(context);
    }

    void print(std::ostream& os) const override
    {
        os << "(";
        left->print(os);
        os << " + ";
        right->print(os);
        os << ")";
    }
};

class Multiplication : public Expression
{
    std::shared_ptr<Expression> left, right;

public:
    Multiplication(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
        : left(std::move(l)), right(std::move(r)) {}

    int calculate(const std::map<std::string, int>& context) const override {
        return left->calculate(context) * right->calculate(context);
    }

    void print(std::ostream& os) const override {
        os << "(";
        left->print(os);
        os << " * ";
        right->print(os);
        os << ")";
    }
};

class ExpressionFactory
{
    std::map<int, std::shared_ptr<Constant>> predefined_constants;
    std::map<int, std::weak_ptr<Constant>> dynamic_constants;
    std::map<std::string, std::weak_ptr<Variable>> variables;

    void initPredefined() {
        for (int i = -5; i <= 256; ++i)
            predefined_constants[i] = std::make_shared<Constant>(i);
    }

public:
    ExpressionFactory() { initPredefined(); }

    std::shared_ptr<Constant> createConstant(int value)
    {
        if (value >= -5 && value <= 256) return predefined_constants[value];
        
        auto& weak = dynamic_constants[value];
        if (auto shared = weak.lock()) return shared;
        
        auto shared = std::make_shared<Constant>(value);
        weak = shared;
        return shared;
    }

    std::shared_ptr<Variable> createVariable(const std::string& name)
    {
    // Проверяем, не устарела ли запись для текущего ключа
    if (auto it = variables.find(name); it != variables.end()) {
        if (it->second.expired()) {
            variables.erase(it);
        }
    }

    auto& weak = variables[name];
    if (auto shared = weak.lock()) {
        return shared;
    }

    auto shared = std::make_shared<Variable>(name);
    weak = shared;
    return shared;
    }
};

int main()
{
    ExpressionFactory factory;
    
    // (2 + x) * 4
    auto c = factory.createConstant(2);
    auto v = factory.createVariable("x");
    auto addition = std::make_shared<Addition>(c, v);
    auto multiplication = std::make_shared<Multiplication>(addition, factory.createConstant(4));

    std::map<std::string, int> context{{"x", 3}};
    
    // (2 + 3) * 4 = 20
    std::cout << "Result: " << multiplication->calculate(context) << std::endl; 
    multiplication->print(std::cout); // ((2 + x) * 4)

    return 0;
}