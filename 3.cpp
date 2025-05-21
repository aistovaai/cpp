#include <iostream>
#include <string>

template<typename...T>
class TypeMapStorage;

template<typename T, typename... Rest>
class TypeMapStorage<T, Rest...> : private TypeMapStorage<Rest...> {
    T value_;

public:
    using Base = TypeMapStorage<Rest...>;

    TypeMapStorage() = default;

    template<typename U, typename... Args>
    void insert(Args&&... args) {
        if constexpr (std::is_same_v<U, T>) {
            value_ = T(std::forward<Args>(args)...);
        } else {
            Base::template insert<U>(std::forward<Args>(args)...);
        }
    }

    template<typename U>
    U& get() {
        if constexpr (std::is_same_v<U, T>) {
            return value_;
        } else {
            return Base::template get<U>();
        }
    }

    template<typename U>
    bool contains() const {
        if constexpr (std::is_same_v<U, T>) {
            return true;
        } else {
            return Base::template contains<U>();
        }
    }

    template<typename U>
    void erase() {
        if constexpr (std::is_same_v<U, T>) {
            value_ = T(); // По умолчанию
        } else {
            Base::template erase<U>();
        }
    }
};

template<> // Базовый шаблон
class TypeMapStorage<> {
public:
    template<typename T, typename... Args>
    void insert(Args&&...) {}

    template<typename T>
    T& get() {
        static_assert(sizeof(T) == 0, "Type not found in TypeMap");
    }

    template<typename T>
    bool contains() const {
        return false;
    }

    template<typename T>
    void erase() {}
};

template<typename... Types>
class TypeMap {
    TypeMapStorage<Types...> storage_;

public:
    TypeMap() = default;

    template<typename T, typename... Args>
    void insert(Args&&... args) {
        storage_.template insert<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    T& get() {
        return storage_.template get<T>();
    }

    template<typename T>
    bool contains() const {
        return storage_.template contains<T>();
    }

    template<typename T>
    void erase() {
        storage_.template erase<T>();
    }
};

int main()
{
    TypeMap<int, std::string, double> map;

    map.insert<int>(42);
    map.insert<std::string>("Hello");
    map.insert<double>(3.14);

    std::cout << map.get<int>() << "\n";
    std::cout << map.get<std::string>() << "\n";
    std::cout << map.get<double>() << "\n";

    map.erase<int>();

    std::cout << std::boolalpha;
    std::cout << map.get<int>() <<"\n";
    std::cout << "int present? " << map.contains<int>() << "\n";      // true (value reset)
    std::cout << "double present? " << map.contains<double>() << "\n"; // true
}
