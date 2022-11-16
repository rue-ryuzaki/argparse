/*
* Copyright (c) 2022 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <iostream>

#include <argparse/argparse.hpp>

#include "./catch-define.h"

struct Coord
{
    Coord()
        : x(),
          y(),
          z()
    { }

    explicit Coord(int x, int y, int z)
        : x(x),
          y(y),
          z(z)
    { }

    inline void print() const
    {
        std::cout << "x=" << x << ";y=" << y << ";z=" << z << std::endl;
    }

    int x;
    int y;
    int z;
};

inline std::istream& operator >>(std::istream& is, Coord& c)
{
    is >> c.x >> c.y >> c.z;
    return is;
}

inline std::ostream& operator <<(std::ostream& os, Coord const& c)
{
    os << c.x << " " << c.y << " " << c.z;
    return os;
}

TEST_CASE("1. custom types", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. custom type with nargs=1") {
        parser.add_argument("--coord").action("append").nargs(1).help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<std::vector<Coord> >("coord").size() == 0);
        REQUIRE(args1.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args1.try_get<std::vector<Coord> >("coord")->size() == 0);
        REQUIRE(args1.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
        REQUIRE(args1.get<Coord>("const_coord").x == 0);

        argparse::Namespace args2 = parser.parse_args("--coord='1 2 3' --const_coord");
        REQUIRE(args2.get<std::vector<Coord> >("coord").size() == 1);
        REQUIRE(args2.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args2.try_get<std::vector<Coord> >("coord")->size() == 1);
        REQUIRE(args2.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
        REQUIRE(args2.get<Coord>("coord").x == 1);
        REQUIRE(args2.get<Coord>("const_coord").x == 1);

        argparse::Namespace args3 = parser.parse_args("--coord='1 2 3' --coord='4 5 6' --const_coord");
        REQUIRE(args3.get<std::vector<Coord> >("coord").size() == 2);
        REQUIRE(args3.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args3.try_get<std::vector<Coord> >("coord")->size() == 2);
        REQUIRE(args3.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
    }

    SECTION("1.2. custom type with nargs=3") {
        parser.add_argument("--coord").action("append").nargs(3).help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<std::vector<Coord> >("coord").size() == 0);
        REQUIRE(args1.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args1.try_get<std::vector<Coord> >("coord")->size() == 0);
        REQUIRE(args1.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
        REQUIRE(args1.get<Coord>("const_coord").x == 0);

        argparse::Namespace args2 = parser.parse_args("--coord 1 2 3 --const_coord");
        REQUIRE(args2.get<std::vector<Coord> >("coord").size() == 1);
        REQUIRE(args2.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args2.try_get<std::vector<Coord> >("coord")->size() == 1);
        REQUIRE(args2.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
        REQUIRE(args2.get<Coord>("coord").x == 1);
        REQUIRE(args2.get<Coord>("const_coord").x == 1);

        argparse::Namespace args3 = parser.parse_args("--coord 1 2 3 --coord 4 5 6 --const_coord");
        REQUIRE(args3.get<std::vector<Coord> >("coord").size() == 2);
        REQUIRE(args3.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args3.try_get<std::vector<Coord> >("coord")->size() == 2);
        REQUIRE(args3.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
    }

    SECTION("1.3. custom type with nargs=*") {
        parser.add_argument("--coord").action("append").nargs("*").help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<std::vector<Coord> >("coord").size() == 0);
        REQUIRE(args1.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args1.try_get<std::vector<Coord> >("coord")->size() == 0);
        REQUIRE(args1.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
        REQUIRE(args1.get<Coord>("const_coord").x == 0);

        argparse::Namespace args2 = parser.parse_args("--coord 1 2 3 --const_coord");
        REQUIRE(args2.get<std::vector<Coord> >("coord").size() == 1);
        REQUIRE(args2.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args2.try_get<std::vector<Coord> >("coord")->size() == 1);
        REQUIRE(args2.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
        REQUIRE(args2.get<Coord>("coord").x == 1);
        REQUIRE(args2.get<Coord>("const_coord").x == 1);

        argparse::Namespace args3 = parser.parse_args("--coord 1 2 3 4 5 6 --const_coord");
        REQUIRE(args3.get<std::vector<Coord> >("coord").size() == 2);
        REQUIRE(args3.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args3.try_get<std::vector<Coord> >("coord")->size() == 2);
        REQUIRE(args3.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // C++17+
    }
}
