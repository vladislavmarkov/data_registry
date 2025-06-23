#include "data/registry.h"

#include <iostream>
#include <string>

/*
 * An example of a user-defined structure.
 * NOTE: Namespaces are okay.
 */
namespace geo
{

class vec2d_t
{
    int _x;
    int _y;

public:
    vec2d_t(int arg_x, int arg_y) : _x(arg_x), _y(arg_y)
    {
        std::cout << "vec2d_t::vec2d_t(" << _x << ", " << _y << ")\n";
    }

    auto get_x() const -> int { return _x; }

    auto get_y() const -> int { return _y; }
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
reg_store_e(location, 1, 2);

auto main() -> int
{
    reg::set<number>(42U);
    reg::set<text>("hello");
    reg::set<location>(geo::vec2d_t{15, 15});

    using std::cout;
    cout << "number: " << reg::get<number>() << '\n';
    cout << "  text: " << reg::get<text>() << '\n';
    cout << " vec2d: { " << reg::get<location>().get_x() << ", "
         << reg::get<location>().get_y() << " }" << '\n';

    return 0;
}
