#include "data/registry.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// #include "stored_crvalue_accessor.h"

using namespace std;

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

static_assert(std::is_trivial<pod_t>::value, "should be POD");

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

// storing them
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

/**
 * Requirement:
 *
 * All reg::get-s have to have the same type as a reader itself.
 */
TEST_CASE("reader_same_as_get_return_type")
{
    using fundamental_result_t = decltype(get_crfundamental());

    CHECK(is_same<fundamental_result_t, uint32_t const&>::value == true);

    using pod_result_type = decltype(get_crpod());

    CHECK(is_same<pod_result_type, pod_t const&>::value == true);

    // reference is constant...
    auto const& fundamental = reg::get<crfundamental_t>();
    CHECK(fundamental == 1U);

    // ... but can be changed from somewhere else
    (void)reg::get<crfundamental_t>();
    CHECK(fundamental == 2U);

    auto const& pod = reg::get<crpod_t>();
    CHECK(pod == pod_t{42, 'z', 3.14});
}

/*
 * Requirement:
 *
 * Return value must always be the same as the return value of the reader.
 */
TEST_CASE_TEMPLATE(
    "reader_return_type_constistency", Tag, crfundamental_t, crpod_t)
{
    using cr_get_result_t = decltype(reg::get<Tag>());

    CHECK(is_same<cr_get_result_t, typename Tag::type const&>::value == true);
}

/*
 * Requirement:
 *
 * The reg::get-s for fundamental statics are returned by value
 */
TEST_CASE_TEMPLATE(
    "fundamental_statics_are_returned_by_value",
    Tag,
    signed_char_t,
    unsigned_char_t,
    short_int_t,
    unsigned_short_int_t,
    int_t,
    unsigned_int_t,
    long_int_t,
    unsigned_long_int_t,
    long_long_int_t,
    unsigned_long_long_int_t)
{
    using get_result_t = decltype(reg::get<Tag>());

    CHECK(is_same<get_result_t, typename Tag::type>::value == true);
    CHECK(is_same<get_result_t, decay_t<typename Tag::type>>::value == true);

    // statics are default-initialized
    CHECK(reg::get<Tag>() == typename Tag::type{});
}
