#include "stored_crvalue_accessor.h"

_store_e(signed_char_t, signed char);
_store_e(unsigned_char_t, unsigned char);
_store_e(short_int_t, short int);
_store_e(unsigned_short_int_t, unsigned short int);
_store_e(int_t, int);
_store_e(unsigned_int_t, unsigned int);
_store_e(long_int_t, long int);
_store_e(unsigned_long_int_t, unsigned long int);
_store_e(long_long_int_t, long long int);
_store_e(unsigned_long_long_int_t, unsigned long long int);

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
