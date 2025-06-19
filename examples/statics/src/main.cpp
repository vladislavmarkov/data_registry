#include "data/registry.h"

#include <iostream>
#include <string>

/*
 * An example of a user-defined structure.
 * NOTE: Namespaces are okay.
 */
namespace geo
{

struct vec2d_t
{
    int x;
    int y;
};

} // namespace geo

_e(number, unsigned short);
_e(text, std::string);
_e(location, geo::vec2d_t);

/*
 * Storing statics in the same translation unit is not necessary
 * but it's done here for sake of having short example.
 */
_store_e(number, unsigned short);
_store_e(text, std::string);
_store_e(location, geo::vec2d_t);

auto main() -> int
{
    reg::set<number>(42U);
    reg::set<text>("hello");
    reg::set<location>(geo::vec2d_t{15, 15});

    using std::cout;
    cout << "number: " << reg::get<number>() << '\n';
    cout << "  text: " << reg::get<text>() << '\n';
    cout << " vec2d: { " << reg::get<location>().x << ", "
         << reg::get<location>().y << " }" << '\n';

    return 0;
}
