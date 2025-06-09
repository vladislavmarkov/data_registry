#ifndef DATA_REGISTRY_H
#define DATA_REGISTRY_H

#define _eID(tag)     struct tag
#define _e(tag, type) reg::entry<_eID(tag), type>

namespace reg
{

template<typename Tag, typename R>
struct entry
{
    using tag = Tag;

    template<typename TagType>
    struct type
    {
        using result_t = R;
    };
};

template<typename... Entries>
struct registry : Entries::template type<typename Entries::tag>...
{
    registry()                                   = delete;
    registry(registry const&)                    = delete;
    registry(registry&&)                         = delete;
    auto operator=(registry const&) -> registry& = delete;
    auto operator=(registry&&) -> registry&      = delete;
    ~registry()                                  = delete;
};

} // namespace reg

#endif // DATA_REGISTRY_H
