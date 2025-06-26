#include "data/registry.h"

#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

static random_device rd;
uniform_int_distribution<uint64_t> dist(
    numeric_limits<uint64_t>::min() + 1ULL, numeric_limits<uint64_t>::max());

#define CHECK(x) \
    if (!(x))    \
        result = -1;

#include GENERATED_ENTRIES
#include GENERATED_STORES

auto main() -> int
{
    int result = 0;
#include GENERATED_CHECKS
    if (result != 0)
    {
        cerr << "test failed\n";
    }
    else
    {
        cout << "test was successful\n";
    }
    return result;
}
