#include "data/registry.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

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
 * All reg::get-s must have the same type as the reader itself.
 */
TEST_CASE("reader must be same as get return type")
{
    using fundamental_result_t = decltype(get_crfundamental());

    CHECK(is_same<fundamental_result_t, uint32_t const&>::value == true);

    using pod_result_type = decltype(get_crpod());

    CHECK(is_same<pod_result_type, pod_t const&>::value == true);

    // is reference, otherwise it won't pass the test
    auto& fundamental = reg::get<crfundamental_t>();
    CHECK(is_same<decltype(fundamental), uint32_t const&>::value == true);
    CHECK(fundamental == 1U);

    // returned by value - won't be modified by subsequent calls to
    // reg::get(), even if user-provided reader does any modifications
    auto fundamentalv = reg::get<crfundamental_t>();
    CHECK(is_same<decltype(fundamentalv), uint32_t>::value == true);
    CHECK(fundamentalv == 2U);

    // ... but can be changed from somewhere else
    (void)reg::get<crfundamental_t>();
    CHECK(fundamental == 3U);

    // value is not modified
    CHECK(fundamentalv == 2U);

    auto const& pod = reg::get<crpod_t>();
    CHECK(pod == pod_t{42, 'z', 3.14});
}

/*
 * Requirement:
 *
 * Return value must always be the same as the return value of the reader.
 */
TEST_CASE_TEMPLATE(
    "reader return type constistency ", Tag, crfundamental_t, crpod_t)
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
    "fundamental statics are returned by value ",
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

static char g_z{};

auto get_char_ctx(int) -> char { return g_z; }

void set_char_ctx(char c, int) { g_z = c; }

_e(reader_ctx, char, get_char_ctx);

/*
 * Requirement:
 *
 * Both reg::get() and reg::set() should accept context data if required by
 * corresponding reader/writers are accepting it.
 */
TEST_CASE("r/o + ctx")
{
    auto c = reg::get<reader_ctx>(42);
    CHECK(c == char{});
}

_e(rw_ctx, char, get_char_ctx, set_char_ctx);

TEST_CASE("r/w + ctx")
{
    CHECK(reg::get<rw_ctx>(42) == char{});
    reg::set<rw_ctx>('z', 42);
}

/*
 * Requirement:
 *
 * API should support passing both raw functions and callable objects as
 * readers/writers.
 */
static auto lmbd_get_char_ctx = [](int) -> char { return g_z; };
static auto lmbd_set_char_ctx = [](char c, int) { g_z = c; };

_e(rw_ctx_callable, char, lmbd_get_char_ctx, lmbd_set_char_ctx);

TEST_CASE("r/w + ctx | callable")
{
    (void)lmbd_set_char_ctx;
    CHECK(reg::get<rw_ctx_callable>(42) == char{});
    reg::set<rw_ctx_callable>('z', 42);
    CHECK(reg::get<rw_ctx_callable>(42) == 'z');
}
