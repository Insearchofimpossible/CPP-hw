#include <type_traits>

template <typename... Ts>
struct TypeList {};

namespace detail {

    template <typename List> 
    struct size_impl;

    template <typename... Ts>
    struct size_impl<TypeList<Ts...>> {
        static constexpr std::size_t value = sizeof...(Ts);
    };

    template <std::size_t Index, typename List> 
    struct get_impl;

    template <typename T, typename... Ts>
    struct get_impl<0, TypeList<T, Ts...>> {
        using type = T;
    };

    template <std::size_t Index, typename T, typename... Ts>
    struct get_impl<Index, TypeList<T, Ts...>> {
        using type = typename get_impl<Index - 1, TypeList<Ts...>>::type;
    };

    template <typename T, typename List> 
    struct contains_impl;

    template <typename T, typename... Ts>
    struct contains_impl<T, TypeList<Ts...>> {
        static constexpr bool value = (std::is_same_v<T, Ts> || ...);
    };

    template <typename T, typename List, std::size_t Current = 0> 
    struct index_of_impl;

    template <typename T, typename... Ts, std::size_t Current>
    struct index_of_impl<T, TypeList<T, Ts...>, Current> {
        static constexpr std::size_t value = Current;
    };

    template <typename T, typename Head, typename... Ts, std::size_t Current>
    struct index_of_impl<T, TypeList<Head, Ts...>, Current> {
        static constexpr std::size_t value = index_of_impl<T, TypeList<Ts...>, Current + 1>::value;
    };

    template <typename T, typename List> 
    struct push_back_impl;

    template <typename T, typename... Ts>
    struct push_back_impl<T, TypeList<Ts...>> {
        using type = TypeList<Ts..., T>;
    };

    template <typename T, typename List> 
    struct push_front_impl;

    template <typename T, typename... Ts>
    struct push_front_impl<T, TypeList<Ts...>> {
        using type = TypeList<T, Ts...>;
    };

}

template <typename List>
constexpr std::size_t Size_v = detail::size_impl<List>::value;

template <std::size_t Index, typename List>
using Get_t = typename detail::get_impl<Index, List>::type;

template <typename T, typename List>
constexpr bool Contains_v = detail::contains_impl<T, List>::value;

template <typename T, typename List>
constexpr std::size_t IndexOf_v = detail::index_of_impl<T, List>::value;

template <typename T, typename List>
using PushBack_t = typename detail::push_back_impl<T, List>::type;

template <typename T, typename List>
using PushFront_t = typename detail::push_front_impl<T, List>::type;

int main() {
    using MyList = TypeList<int, float, double>;

    static_assert(Size_v<MyList> == 3);

    static_assert(std::is_same_v<Get_t<0, MyList>, int>);
    static_assert(std::is_same_v<Get_t<1, MyList>, float>);
    static_assert(std::is_same_v<Get_t<2, MyList>, double>);
    
    static_assert(Contains_v<float, MyList> == true);
    static_assert(Contains_v<char, MyList> == false);

    static_assert(IndexOf_v<float, MyList> == 1);
    static_assert(IndexOf_v<double, MyList> == 2);

    using AppendedList = PushBack_t<char, MyList>;
    static_assert(std::is_same_v<AppendedList, TypeList<int, float, double, char>>);

    using PrependedList = PushFront_t<char, MyList>;
    static_assert(std::is_same_v<PrependedList, TypeList<char, int, float, double>>);

    return 0;
}