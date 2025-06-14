#include "data/registry.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include "stored_demo.h"

TEST_CASE("demo")
{
    /*
     * Static entries are default-constructed
     */

    CHECK(reg::get<speed>() == uint16_t{});
    CHECK(reg::get<temp>() == float{});

    /*
     * reg::get-s are calling the reader when the entry declared with one.
     *
     * Example:
     *   _e(heartbeat, uint32_t, get_heartbeat);
     */

    CHECK(reg::get<heartbeat>() == 1U);

    CHECK(reg::get<heartbeat>() == 2U);
    CHECK(reg::get<heartbeat>() == 3U);

    /*
     * reg::set-s can be used if writer was specified
     *
     * Example:
     *   _(state, state_t, get_state, set_state);
     */

    CHECK(reg::get<state>() == state_t::initialization);
    reg::set<state>(state_t::operation_cycle);
    CHECK(reg::get<state>() == state_t::operation_cycle);

    /*
     * User-provided data structures beyond fundamentals and PODs are supported.
     */

    CHECK(reg::get<location>() == location_t{});
    CHECK(reg::get<location>().latitude == double{});
    CHECK(reg::get<location>().longtitude == double{});
}
