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

// register all fundamental statics
_e(signed_char_t, signed char);
_e(unsigned_char_t, unsigned char);
_e(short_int_t, short int);
_e(unsigned_short_int_t, unsigned short int);
_e(int_t, int);
_e(unsigned_int_t, unsigned int);
_e(long_int_t, long int);
_e(unsigned_long_int_t, unsigned long int);
_e(long_long_int_t, long long int);
_e(unsigned_long_long_int_t, unsigned long long int);

#endif // STORED_CRVALUE_ACCESSOR_H
