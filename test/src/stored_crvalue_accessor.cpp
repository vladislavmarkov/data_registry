#include "stored_crvalue_accessor.h"

auto get_crfundamental() -> uint32_t const&
{
    static uint32_t _value = 0U;
    return ++_value;
}

auto get_crpod() -> pod_t const&
{
    static pod_t _pod{42, 'z', 3.14};
    return _pod;
}
