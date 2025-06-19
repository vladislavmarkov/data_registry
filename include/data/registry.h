#ifndef REG_DATA_REGISTRY_H
#define REG_DATA_REGISTRY_H

#include <tuple>
#include <utility>

namespace reg
{

namespace traits
{

template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R (*)(Args...)>
{
    using result_t       = R;
    using argument_types = std::tuple<Args...>;

    static constexpr std::size_t argsnum = sizeof...(Args);

    template<std::size_t N>
    struct argument
    {
        static_assert(N < argsnum, "Invalid argument index");
        using type =
            typename std::tuple_element_t<N, std::tuple<Args...>>::type;
    };
};

} // namespace traits

template<typename Tag>
inline constexpr auto get() -> decltype(auto)
{
    return Tag::get();
}

template<typename Tag, typename T>
void set(T&& value)
{
    Tag::set(std::forward<T>(value));
}

} // namespace reg

/*
 * 1. Fundamental statics are read by value.
 * 2. Everything else is read by const reference.
 * 3. Statics are r/w, since there're no other ways to modify them.
 */
#define _e_static(Tag, Type)                                   \
    struct Tag                                                 \
    {                                                          \
        using type = std::conditional_t<                       \
            std::is_fundamental<Type>::value,                  \
            Type,                                              \
            Type const&>;                                      \
                                                               \
        static inline auto get() -> type { return _value; }    \
        static inline void set(type value) { _value = value; } \
                                                               \
    private:                                                   \
        static Type _value;                                    \
    }

#define _e_reader(Tag, Type, Reader)                                        \
    struct Tag                                                              \
    {                                                                       \
        static_assert(                                                      \
            (std::is_const<Type>::value && std::is_reference<Type>::value)  \
                || !std::is_reference<Type>::value,                         \
            "read accessor should return const reference or a value");      \
                                                                            \
        using type = Type;                                                  \
                                                                            \
        static inline auto get() -> decltype(Reader()) { return Reader(); } \
    }

#define _store_e(Tag, Type) \
    Type Tag::_value {}

#define _e_reader_writer(Tag, Type, Reader, Writer)                         \
    struct Tag                                                              \
    {                                                                       \
        using type = Type;                                                  \
                                                                            \
        static inline auto get() -> decltype(Reader()) { return Reader(); } \
                                                                            \
        static inline void set(Type value)                                  \
        {                                                                   \
            return Writer(std::forward<Type>(value));                       \
        }                                                                   \
    }

#define _GET_LAST_ARG(tag, arg1, arg2, arg3, arg4, ...) arg4
#define _e_choose(...) \
    _GET_LAST_ARG(__VA_ARGS__, _e_reader_writer, _e_reader, _e_static, )

#define _e(...) _e_choose(__VA_ARGS__)(__VA_ARGS__)

#endif // REG_DATA_REGISTRY_H
