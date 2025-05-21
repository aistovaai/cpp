#include <iostream>

namespace internal
{
    struct Void {};
}

template<typename ...Args>
struct TypeList
{
    typedef internal::Void Head;
    typedef internal::Void Tail;
};

typedef TypeList<> EmptyTypeList;

template<typename H, typename ...T>
struct TypeList<H, T...>
{
    typedef H Head;
    typedef TypeList<T...> Tail;
};

// Проверка пустой ли список
template<typename T>
struct IsEmpty : std::true_type {};

template<>
struct IsEmpty<TypeList<internal::Void, internal::Void>> :
    std::true_type {};

template<typename ...Args>
struct IsEmpty<TypeList<Args...>>:
    std::integral_constant<bool, std::is_same<typename TypeList<Args...>::Head, internal::Void>::value &&
    IsEmpty<typename TypeList<Args...>::Tail>::value>
{};

// Использование
typedef TypeList<int> TL1;
static_assert(IsEmpty<TL1>::value == false);
typedef TypeList<> TL2;
static_assert(IsEmpty<TL2>::value);


// Contains

template<typename T, typename TL>
struct Contains : std::false_type {};

template<typename ...Args>
struct Contains<internal::Void, Args...> : std::false_type {};

template<typename T, typename ...Args>
struct Contains<T, TypeList<Args...>> :
    std::integral_constant<bool, std::is_same<T, typename TypeList<Args...>::Head>::value
        || Contains<T, typename TypeList<Args...>::Tail>::value>
{};


typedef TypeList<double, float, float, double, int, char, char, int, char> TL;
static_assert(Contains<char, TL>::value) ;
static_assert(Contains<float, TypeList<double>>() == false);


// Length

template<typename TL>
struct Length : std::integral_constant<unsigned int, 0>
{};

template<typename ...Args>
struct Length<TypeList<Args...>>: std::integral_constant<unsigned int,
    IsEmpty<TypeList<Args...>>::value ? 0 :
                            1 + Length<typename TypeList<Args...>::Tail>::value>
{};


/*
typedef TypeList<double, float, float, double, int, char, char, int, char> TL;
std::cout << Length<TL>::value << " " << Length<EmptyTypeList>() << std::endl;
*/

// TypeAt получение типа по индексу

template<unsigned int N, typename TL>
struct TypeAt
{
    typedef internal::Void type;
};

template<typename ...Args>
struct TypeAt<0, TypeList<Args...>>
{  
    typedef typename TypeList<Args...>::Head type;
};

template<unsigned int N, typename ...Args>
struct TypeAt<N, TypeList<Args...>>
{
    static_assert(N < Length<TypeList<Args...>>::value, "N is too big");

    typedef typename TypeAt<N - 1, typename TypeList<Args...>::Tail>::type type;
};

typedef TypeList<char, short> TL3;
static_assert(std::is_same<TypeAt<1, TL3>::type, short>::value, "Something wrong!");


// Добавление типа в список

template<typename T, typename TL>
struct PushBack {};

template<typename T, typename ...Args>
struct PushBack<T, TypeList<Args...>>
{
    typedef TypeList<Args..., T> type;
};

template<typename T, typename TL>
struct PushFront {};

template<typename T, typename ...Args>
struct PushFront<T, TypeList<Args...>>
{
    typedef TypeList<T, Args...> type;
};

typedef TypeList<int, short, std::string, bool> TL4;
static_assert(std::is_same<PushBack<bool, TL4>::type, TypeList<int, short, std::string, bool, bool>>::value);

typedef TypeList<int, short, std::string, bool> TL5;
static_assert(std::is_same<PushFront<bool, TL5>::type, TypeList<bool, int, short, std::string, bool>>::value);

// Получение индекса типа в списке

struct Constants
{
    typedef std::integral_constant<unsigned int, 99999> npos;
};


template<typename T, typename TL>
struct FindIndex
{
};

template<typename T,typename ...Args>
struct FindIndex<T, TypeList<T, Args...>> : std::integral_constant<int, 0>
{
};

template<typename T,typename ...Args>
struct FindIndex<T, TypeList<Args...>> : 
    std::integral_constant<int, FindIndex<T, typename TypeList<Args...>::Tail>::value + 1>
{
};

template<typename T>
struct FindIndex<T, EmptyTypeList> : Constants::npos
{

};

typedef TypeList<double, float, float, double, int, char, char, int, char> TL;
static_assert(std::is_same<TypeAt<FindIndex<double, TL>::value, TL>::type, double>::value);

int main()
{
    
}