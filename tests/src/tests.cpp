#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include "stored_data.h"

using namespace std;

// Goal - share data between components
//   1. ~~Lightweight~~ - everything is compile time. The only question is -
//      storing acessor functions - should be possible to store them in ROM.
//   2. ~~Lock Policy support~~ - can be solved with the context passing
//   3. No generator should be required - not a problem - it's not required as
//   is.
//   4. PODs should be supported (not only fundamentals).
//   5. ~~Specify who owns the data~~ - not needed, because we can solve
//      this by using read / write accessor functions
//   6. All data must be initialized prior to registry declaration
//
// Rationale
// Why does this thing need to exist?
//   1. It's possible to share only bits of data between components, but not
//   ENTIRE components.
//   2. It's a possible replacement for RTE but for opensource.

TEST_CASE("demo")
{
    CHECK(
        is_same<uint32_t, stored_data::type<_eID(speed)>::result_t>::value
        == true);
}
