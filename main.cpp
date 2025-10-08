#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <aircraft.h>

void check_case(const std::string& name, const Aircraft& ac, const std::vector<Point>& tunnel, double width, int expected) {
    int got = const_cast<Aircraft&>(ac).is_intruder(tunnel, width);
    std::cout << name << ": got=" << got << " expected=" << expected
              << (got == expected ? "  [OK]" : "  [FAIL]") << "\n";
    assert(got == expected);
}

int main() {
    // Построим простой «воздушный тоннель» — прямая по широте от (0,0,0) до (10,0,0)
    std::vector<Point> tunnel;
    tunnel.emplace_back(0.0, 0.0, 0.0);
    tunnel.emplace_back(10.0, 0.0, 0.0);

    // Ширина допуска (радиус) вокруг линии: 1.0
    double width = 1.0;

    // Кейс 1: точно на линии — не нарушитель
    Aircraft ac_on_line(1, /*vel*/0.0, /*accel*/0.0, Point(5.0, 0.0, 0.0));
    check_case("On line", ac_on_line, tunnel, width, /*expected*/0);

    // Кейс 2: ровно на границе ширины — не нарушитель (т.к. в коде > width, а не >=)
    Aircraft ac_on_border(2, 0.0, 0.0, Point(5.0, 1.0, 0.0)); // расстояние = 1
    check_case("On border", ac_on_border, tunnel, width, /*expected*/0);

    // Кейс 3: дальше границы — нарушитель
    Aircraft ac_outside(3, 0.0, 0.0, Point(5.0, 2.0, 0.0)); // расстояние = 2
    check_case("Outside width", ac_outside, tunnel, width, /*expected*/1);

    // Кейс 4: точка у начала тоннеля — не нарушитель
    Aircraft ac_at_start(4, 0.0, 0.0, Point(0.0, 0.0, 0.0));
    check_case("At start", ac_at_start, tunnel, width, /*expected*/0);

    // Кейс 5: 3D-отклонение по высоте
    Aircraft ac_above(5, 0.0, 0.0, Point(5.0, 0.0, 0.8)); // dist = 0.8 < 1
    check_case("Above but inside", ac_above, tunnel, width, /*expected*/0);

    Aircraft ac_too_high(6, 0.0, 0.0, Point(5.0, 0.0, 1.01)); // dist ~1.01 > 1
    check_case("Too high", ac_too_high, tunnel, width, /*expected*/1);

    // Кейс 6: «тоннель» ломаная — несколько точек
    std::vector<Point> tunnel_poly {
        {0.0, 0.0, 0.0},
        {5.0, 0.0, 0.0},
        {10.0, 1.0, 0.0},   // поворот по долготе
        {15.0, 1.0, 0.0}
    };
    Aircraft ac_near_bend(7, 0.0, 0.0, Point(9.5, 0.8, 0.0)); // близко к второй секции
    check_case("Near bend (polyline)", ac_near_bend, tunnel_poly, width, /*expected*/0);

    std::cout << "All tests passed.\n";
    return 0;
}