#ifndef DATA_REGISTRY_H
#define DATA_REGISTRY_H

#include <type_traits>
#include <utility>

namespace reg
{

template<typename Tag>
auto get()
{
    return Tag::get();
}

template<typename Tag, typename T>
void set(T&& value)
{
    Tag::set(std::forward<T>(value));
}

} // namespace reg

#define _e_static(tag, type)                                \
    struct tag                                              \
    {                                                       \
        static inline auto get() -> type { return _value; } \
                                                            \
    private:                                                \
        static type _value;                                 \
    }

#define _e_reader(tag, type, reader)                          \
    struct tag                                                \
    {                                                         \
        static inline auto get() -> type { return reader(); } \
    }

#define _store_e(tag, type) \
    type tag::_value {}

#define _e_reader_writer(tag, type, reader, writer)           \
    struct tag                                                \
    {                                                         \
        static inline auto get() -> type { return reader(); } \
        static inline void set(type&& value)                  \
        {                                                     \
            return writer(std::forward<type>(value));         \
        }                                                     \
    }

#define _GET_LAST_ARG(tag, arg1, arg2, arg3, arg4, ...) arg4
#define _e_choose(...) \
    _GET_LAST_ARG(__VA_ARGS__, _e_reader_writer, _e_reader, _e_static, )

#define _e(...) _e_choose(__VA_ARGS__)(__VA_ARGS__)

#endif // DATA_REGISTRY_H
