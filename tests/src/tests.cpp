#include "data/registry.h"

#include <tuple>

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
reg_e(crfundamental_t, uint32_t, get_crfundamental);
reg_e(crpod_t, pod_t, get_crpod);

// register all fundamental statics
reg_e(signed_char_t, signed char);
reg_e(unsigned_char_t, unsigned char);
reg_e(short_int_t, short int);
reg_e(unsigned_short_int_t, unsigned short int);
reg_e(int_t, int);
reg_e(unsigned_int_t, unsigned int);
reg_e(long_int_t, long int);
reg_e(unsigned_long_int_t, unsigned long int);
reg_e(long_long_int_t, long long int);
reg_e(unsigned_long_long_int_t, unsigned long long int);

// storing them
reg_store_e(signed_char_t);
reg_store_e(unsigned_char_t);
reg_store_e(short_int_t);
reg_store_e(unsigned_short_int_t);
reg_store_e(int_t);
reg_store_e(unsigned_int_t);
reg_store_e(long_int_t);
reg_store_e(unsigned_long_int_t);
reg_store_e(long_long_int_t);
reg_store_e(unsigned_long_long_int_t);

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

reg_e(reader_ctx, char, get_char_ctx);

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

reg_e(rw_ctx, char, get_char_ctx, set_char_ctx);

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

reg_e(rw_ctx_callable, char, lmbd_get_char_ctx, lmbd_set_char_ctx);

TEST_CASE("r/w + ctx | callable")
{
    (void)lmbd_set_char_ctx;
    CHECK(reg::get<rw_ctx_callable>(42) == char{});
    reg::set<rw_ctx_callable>('z', 42);
    CHECK(reg::get<rw_ctx_callable>(42) == 'z');
}

/*
 * Requirement:
 *
 * reg_store_e() should support initialization value / constructor call
 */

class non_default_constructible
{
    int _i;
    char _c;
    float _f;
    string _str;
    int _k;
    int _l;
    int _m;

public:
    explicit non_default_constructible(
        int init_int,
        char init_char,
        float init_float,
        char const* str,
        int k,
        int l,
        int m)
    : _i(init_int)
    , _c(init_char)
    , _f(init_float)
    , _str(str)
    , _k(k)
    , _l(l)
    , _m(m)
    {}

    auto get() const -> tuple<int, char, float, string, int, int, int>
    {
        return make_tuple(_i, _c, _f, _str, _k, _l, _m);
    }
};

reg_e(initialization_required, non_default_constructible);

reg_store_e(
    initialization_required, 42, 'z', 36.6F, "Hi there!", 123, 234, 345);

TEST_CASE("reg_store_e() with initialization value")
{
    CHECK(
        reg::get<initialization_required>().get()
        == make_tuple(42, 'z', 36.6F, string("Hi there!"), 123, 234, 345));
}
