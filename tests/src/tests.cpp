#include "data/registry.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include "stored_data.h"

TEST_CASE("demo")
{
    CHECK(reg::get<speed>() == uint16_t{});
    CHECK(reg::get<temp>() == float{});
    CHECK(reg::get<heartbeat>() == 1U);
    CHECK(reg::get<heartbeat>() == 2U);
    CHECK(reg::get<heartbeat>() == 3U);

    CHECK(reg::get<state>() == state_t::initialization);
    reg::set<state>(state_t::operation_cycle);
    CHECK(reg::get<state>() == state_t::operation_cycle);
}
