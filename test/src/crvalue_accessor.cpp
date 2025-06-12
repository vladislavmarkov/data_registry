#include "data/registry.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include "stored_crvalue_accessor.h"

/**
 * Requirement:
 *
 * All reader accessor helping functions have to have the same type as
 * a reader accessor itself.
 */
TEST_CASE("reader_return_type_constistency")
{
    using fundamental_result_type = decltype(get_crfundamental());

    CHECK(
        std::is_same<fundamental_result_type, uint32_t const&>::value == true);

    using pod_result_type = decltype(get_crpod());

    CHECK(std::is_same<pod_result_type, pod_t const&>::value == true);

    // reference is constant...
    auto const& fundamental = reg::get<crfundamental_t>();
    CHECK(fundamental == 1U);

    // ... but can be changed from somewhere else
    (void)reg::get<crfundamental_t>();
    CHECK(fundamental == 2U);

    auto const& pod = reg::get<crpod_t>();
    CHECK(pod == pod_t{42, 'z', 3.14});
}

TEST_CASE_TEMPLATE(
    "getter_return_type_constistency", Tag, crfundamental_t, crpod_t)
{
    using cr_get_result_t = decltype(reg::get<Tag>());

    CHECK(
        std::is_same<cr_get_result_t, typename Tag::type const&>::value
        == true);
}
