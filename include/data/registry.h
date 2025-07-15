#ifndef REG_DATA_REGISTRY_H
#define REG_DATA_REGISTRY_H

#include <type_traits>
#include <utility>

namespace reg
{

template<typename Tag, typename... Args>
inline constexpr auto get(Args&&... args) -> decltype(Tag::get(args...))
{
    return Tag::get(::std::forward<Args>(args)...);
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
 *  1. reg_e(tag, type)                    - static
 *  2. reg_e(tag, type, reader)            - r/o
 *  3. reg_e(tag, type, reader, writer)    - r/w
 *  4. reg_store_e(tag, ctrarg1...ctrargN) - storing statics (pass arguments to
 *     ctr if required)
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

#define IMPL_REG_E_S(Tag, Type)                                        \
    struct Tag final                                                   \
    {                                                                  \
        Tag()                              = delete;                   \
        Tag(Tag const&)                    = delete;                   \
        auto operator=(Tag const&) -> Tag& = delete;                   \
        Tag(Tag&&)                         = delete;                   \
        auto operator=(Tag&&) -> Tag&      = delete;                   \
        ~Tag()                             = delete;                   \
                                                                       \
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

#define IMPL_REG_E_R(Tag, Type, Reader)                                     \
    struct Tag final                                                        \
    {                                                                       \
        Tag()                              = delete;                        \
        Tag(Tag const&)                    = delete;                        \
        auto operator=(Tag const&) -> Tag& = delete;                        \
        Tag(Tag&&)                         = delete;                        \
        auto operator=(Tag&&) -> Tag&      = delete;                        \
        ~Tag()                             = delete;                        \
                                                                            \
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

#define IMPL_REG_E_RW(Tag, Type, Reader, Writer)                            \
    struct Tag final                                                        \
    {                                                                       \
        Tag()                              = delete;                        \
        Tag(Tag const&)                    = delete;                        \
        auto operator=(Tag const&) -> Tag& = delete;                        \
        Tag(Tag&&)                         = delete;                        \
        auto operator=(Tag&&) -> Tag&      = delete;                        \
        ~Tag()                             = delete;                        \
                                                                            \
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

#define IMPL_STORE_E(Tag) \
    Tag::type Tag::_value {}

#define IMPL_STORE_E_ARGS(Tag, ...) \
    Tag::type Tag::_value { __VA_ARGS__ }

#define IMPL_REG_SWITCH_ARG( \
    Tag,                     \
    Arg1,                    \
    Arg2,                    \
    Arg3,                    \
    Arg4,                    \
    Arg5,                    \
    Arg6,                    \
    Arg7,                    \
    Arg8,                    \
    Arg9,                    \
    Arg10,                   \
    Arg11,                   \
    Arg12,                   \
    Arg13,                   \
    Arg14,                   \
    Arg15,                   \
    ...)                     \
    Arg15

#define IMPL_REG_STORE_E_CHOOSE(...) \
    IMPL_REG_SWITCH_ARG(             \
        __VA_ARGS__,                 \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E_ARGS,           \
        IMPL_STORE_E, )

#define reg_store_e(...) IMPL_REG_STORE_E_CHOOSE(__VA_ARGS__)(__VA_ARGS__)

/*
 * Having this many TOO_MANY_ARGUMENTS placeholders allows to pass up to 14
 * constructor arguments to reg_store_e().
 */
#define IMPL_REG_E_CHOOSE(...) \
    IMPL_REG_SWITCH_ARG(       \
        __VA_ARGS__,           \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        TOO_MANY_ARGUMENTS,    \
        IMPL_REG_E_RW,         \
        IMPL_REG_E_R,          \
        IMPL_REG_E_S, )

#define reg_e(...) IMPL_REG_E_CHOOSE(__VA_ARGS__)(__VA_ARGS__)

#endif // REG_DATA_REGISTRY_H
