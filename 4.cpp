#include <iostream>
#include <assert.h>


template <typename T>
class less_than_comparable
{
public:
    bool operator>(const T& other) const {
        return static_cast<const T*>(this)->value() > other.value();
    }

    bool operator<(const T& other) const {
        return static_cast<const T*>(this)->value() < other.value();
    }

    bool operator>=(const T& other) const {
        return !(static_cast<const T*>(this)->value() < other.value());
    }

    bool operator<=(const T& other) const {
        return !(static_cast<const T*>(this)->value() > other.value());
    }

    bool operator==(const T& other) const {
        return static_cast<const T*>(this)->value() == other.value();
    }

    bool operator!=(const T& other) const {
        return !(static_cast<const T*>(this)->value() == other.value());
    }
};

template <typename T>
class counter{
private:
    static int instance_count;

public:
    counter() {
        ++instance_count;
    }

    ~counter() {
        --instance_count;
    }

    static int count() {
        return instance_count;
    }
};

template <typename T>
int counter<T>::instance_count = 0;

class Number: public less_than_comparable<Number>, public counter<Number> {
public:
    Number(int value): m_value{value} {}

    int value() const { return m_value; }

    bool operator<(Number const& other) const {
        return m_value < other.m_value;
    }

private:
    int m_value;
};

int main()
{
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};
    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);
    std::cout << "Count: " << counter<Number>::count() << std::endl;
    return 0;
}