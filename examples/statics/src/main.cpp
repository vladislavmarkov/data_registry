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

reg_e(number, unsigned short);
reg_e(text, std::string);
reg_e(location, geo::vec2d_t);

/*
 * Storing statics in the same translation unit is not necessary
 * but it's done here for sake of having short example.
 */
reg_store_e(number);
reg_store_e(text);
reg_store_e(location);

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
