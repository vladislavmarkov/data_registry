#include "data/registry.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include "stored_crvalue_accessor.h"

using namespace std;

/**
 * Requirement:
 *
 * All reg::get-s have to have the same type as a reader itself.
 */
TEST_CASE("reader_return_type_constistency")
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
    "read_accessor_return_type_constistency", Tag, crfundamental_t, crpod_t)
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
    "read_accessor_fundamental_types_returned_by_value",
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
