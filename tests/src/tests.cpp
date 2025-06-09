#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include "stored_data.h"

using namespace std;

TEST_CASE("demo")
{
    CHECK(reg::get<speed>() == uint16_t{});
    CHECK(reg::get<temp>() == float{});
}
