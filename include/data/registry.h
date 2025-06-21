#ifndef REG_DATA_REGISTRY_H
#define REG_DATA_REGISTRY_H

#include <type_traits>
#include <utility>

namespace reg
{

template<typename Tag, typename... Args>
inline constexpr auto get(Args&&... args) -> decltype(Tag::get(args...))
{
    return Tag::get(args...);
}

template<typename Tag, typename T, typename... Args>
void set(T&& value, Args&&... args)
{
    Tag::set(::std::forward<T>(value), ::std::forward<Args>(args)...);
}

} // namespace reg

/*
 * Variations
 *
 * Core API:
 *
 *  1. _e(tag,   type)                 - static
 *  2. _e(tag,   type, reader)         - r/o
 *  3. _e(tag,   type, reader, writer) - r/w
 *
 *  Additional requirements:
 *
 *  1. It should be possible for readers/writers to be represented with either
 *     raw function, or a functional object.
 */

namespace reg
{

namespace detail
{

template<typename T>
using modify_type
    = ::std::conditional_t<::std::is_fundamental<T>::value, T, T const&>;

}

} // namespace reg

#define _e_s(Tag, Type)                                                \
    struct Tag                                                         \
    {                                                                  \
        using type = Type;                                             \
                                                                       \
        static inline auto get() -> ::reg::detail::modify_type<type>   \
        {                                                              \
            return _value;                                             \
        }                                                              \
                                                                       \
        static inline void set(::reg::detail::modify_type<type> value) \
        {                                                              \
            _value = value;                                            \
        }                                                              \
                                                                       \
    private:                                                           \
        static type _value;                                            \
    }

#define _e_r(Tag, Type, Reader)                                             \
    struct Tag                                                              \
    {                                                                       \
        using type = Type;                                                  \
                                                                            \
        static_assert(                                                      \
            (std::is_const<Type>::value && std::is_reference<Type>::value)  \
                || !std::is_reference<Type>::value,                         \
            "read accessor should return const reference or a value");      \
                                                                            \
        template<typename... Args>                                          \
        static inline auto get(Args&&... args) -> decltype(Reader(args...)) \
        {                                                                   \
            return Reader(::std::forward<Args>(args)...);                   \
        }                                                                   \
    }

#define _e_rw(Tag, Type, Reader, Writer)                                    \
    struct Tag                                                              \
    {                                                                       \
        using type = Type;                                                  \
                                                                            \
        template<typename... Args>                                          \
        static inline auto get(Args&&... args) -> decltype(Reader(args...)) \
        {                                                                   \
            return Reader(::std::forward<Args>(args)...);                   \
        }                                                                   \
                                                                            \
        template<typename... Args>                                          \
        static inline void set(type value, Args&&... args)                  \
        {                                                                   \
            Writer(value, ::std::forward<Args>(args)...);                   \
        }                                                                   \
    }

// store static
#define _store_e(Tag, Type) \
    Tag::type Tag::_value {}

#define _GET_LAST_ARG(tag, arg1, arg2, arg3, arg4, ...) arg4

#define _e_choose(...) _GET_LAST_ARG(__VA_ARGS__, _e_rw, _e_r, _e_s, )

#define _e(...) _e_choose(__VA_ARGS__)(__VA_ARGS__)

#endif // REG_DATA_REGISTRY_H
