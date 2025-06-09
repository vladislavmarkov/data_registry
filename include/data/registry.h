#ifndef DATA_REGISTRY_H
#define DATA_REGISTRY_H

namespace reg
{

template<typename Tag>
auto get()
{
    return Tag::get();
}

} // namespace reg

#define _e(tag, type)                                       \
    struct tag                                              \
    {                                                       \
        static type _value;                                 \
        static inline auto get() -> type { return _value; } \
    }

#define _store_e(tag, type) \
    type tag::_value {}

#endif // DATA_REGISTRY_H
