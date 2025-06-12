#ifndef STORED_CRVALUE_ACCESSOR_H
#define STORED_CRVALUE_ACCESSOR_H

#include "data/registry.h"

#include <cstdint>
#include <type_traits>

auto get_crfundamental() -> uint32_t const&;

struct pod_t
{
    int a;
    char b;
    double c;

    friend auto operator==(pod_t const& lhs, pod_t const& rhs) -> bool
    {
        return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c;
    }
};

static_assert(std::is_pod<pod_t>::value, "should be POD");

auto get_crpod() -> pod_t const&;

// register entries
_e(crfundamental_t, uint32_t, get_crfundamental);
_e(crpod_t, pod_t, get_crpod);

#endif // STORED_CRVALUE_ACCESSOR_H
