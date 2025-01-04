/*
 * Copyright (c) 2022-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#include <iostream>

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

struct Coord
{
    Coord()
        : x(),
          y(),
          z()
    { }

    explicit
    Coord(int x,
            int y,
            int z)
        : x(x),
          y(y),
          z(z)
    { }

    inline void
    print() const
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

inline void coord_type_builder(std::string const& str, void* res)
{
    std::stringstream ss(str);
    // just use operator >>
    ss >> *reinterpret_cast<Coord*>(res);
}

TEST_CASE("1. custom types", "[argument]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. custom type with factory function") {
        parser.add_argument("--coord")
                .type(&coord_type_builder).help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .type(&coord_type_builder)
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        Coord coord1 = args1.get<Coord>("coord");
        Coord const_coord1 = args1.get<Coord>("const_coord");
        CHECK(coord1.x == 0);
        CHECK(coord1.y == 0);
        CHECK(coord1.z == 0);
        CHECK(const_coord1.x == 0);
        CHECK(const_coord1.y == 0);
        CHECK(const_coord1.z == 0);

        argparse::Namespace args2 = parser.parse_args("--coord='1 2 3' --const_coord");
        Coord coord2 = args2.get<Coord>("coord");
        Coord const_coord2 = args2.get<Coord>("const_coord");
        CHECK(coord2.x == 1);
        CHECK(coord2.y == 2);
        CHECK(coord2.z == 3);
        CHECK(const_coord2.x == 1);
        CHECK(const_coord2.y == 1);
        CHECK(const_coord2.z == 1);
    }
}

TEST_CASE("2. containers with custom types", "[argument]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("2.1. custom type with nargs=1") {
        parser.add_argument("--coord").action("append").nargs(1).help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<std::vector<Coord> >("coord").size() == 0);
        CHECK(args1.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args1.try_get<std::vector<Coord> >("coord")->size() == 0);
        CHECK(args1.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
        CHECK(args1.get<Coord>("const_coord").x == 0);

        argparse::Namespace args2 = parser.parse_args("--coord='1 2 3' --const_coord");
        CHECK(args2.get<std::vector<Coord> >("coord").size() == 1);
        CHECK(args2.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args2.try_get<std::vector<Coord> >("coord")->size() == 1);
        CHECK(args2.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
        CHECK(args2.get<Coord>("coord").x == 1);
        CHECK(args2.get<Coord>("const_coord").x == 1);

        argparse::Namespace args3 = parser.parse_args("--coord='1 2 3' --coord='4 5 6' --const_coord");
        CHECK(args3.get<std::vector<Coord> >("coord").size() == 2);
        CHECK(args3.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args3.try_get<std::vector<Coord> >("coord")->size() == 2);
        CHECK(args3.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
    }

    SECTION("2.2. custom type with nargs=3") {
        parser.add_argument("--coord").action("append").nargs(3).help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<std::vector<Coord> >("coord").size() == 0);
        CHECK(args1.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args1.try_get<std::vector<Coord> >("coord")->size() == 0);
        CHECK(args1.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
        CHECK(args1.get<Coord>("const_coord").x == 0);

        argparse::Namespace args2 = parser.parse_args("--coord 1 2 3 --const_coord");
        CHECK(args2.get<std::vector<Coord> >("coord").size() == 1);
        CHECK(args2.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args2.try_get<std::vector<Coord> >("coord")->size() == 1);
        CHECK(args2.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
        CHECK(args2.get<Coord>("coord").x == 1);
        CHECK(args2.get<Coord>("const_coord").x == 1);

        argparse::Namespace args3 = parser.parse_args("--coord 1 2 3 --coord 4 5 6 --const_coord");
        CHECK(args3.get<std::vector<Coord> >("coord").size() == 2);
        CHECK(args3.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args3.try_get<std::vector<Coord> >("coord")->size() == 2);
        CHECK(args3.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
    }

    SECTION("2.3. custom type with combined nargs=*") {
        // combined nargs since v1.8.4
        parser.add_argument("--coord").action("append").nargs("*", 3).help("coord help");
        parser.add_argument("--const_coord").action("store_const")
                .default_value(Coord(0, 0, 0))
                .const_value(Coord(1, 1, 1)).help("const coord help");

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<std::vector<Coord> >("coord").size() == 0);
        CHECK(args1.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args1.try_get<std::vector<Coord> >("coord")->size() == 0);
        CHECK(args1.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
        CHECK(args1.get<Coord>("const_coord").x == 0);

        argparse::Namespace args2 = parser.parse_args("--coord 1 2 3 --const_coord");
        CHECK(args2.get<std::vector<Coord> >("coord").size() == 1);
        CHECK(args2.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args2.try_get<std::vector<Coord> >("coord")->size() == 1);
        CHECK(args2.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL
        CHECK(args2.get<Coord>("coord").x == 1);
        CHECK(args2.get<Coord>("const_coord").x == 1);

        // invalid in v1.8.3
        // work since v1.8.4
        argparse::Namespace args3 = parser.parse_args("--coord 1 2 3 4 5 6 --const_coord");
        CHECK(args3.get<std::vector<Coord> >("coord").size() == 2);
        CHECK(args3.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args3.try_get<std::vector<Coord> >("coord")->size() == 2);
        CHECK(args3.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL

        // work since v1.8.3
        argparse::Namespace args4 = parser.parse_args("--coord 1 2 3 --coord 4 5 6 --const_coord");
        CHECK(args4.get<std::vector<Coord> >("coord").size() == 2);
        CHECK(args4.get<std::vector<Coord> >("const_coord").size() == 1);
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args4.try_get<std::vector<Coord> >("coord")->size() == 2);
        CHECK(args4.try_get<std::vector<Coord> >("const_coord")->size() == 1);
#endif  // ARGPARSE_HAS_OPTIONAL

        CHECK_THROWS(parser.parse_args("--coord 1"));
        CHECK_THROWS(parser.parse_args("--coord 1 2"));
        CHECK_THROWS(parser.parse_args("--coord 1 2 3 4"));
        CHECK_THROWS(parser.parse_args("--coord 1 2 3 4 5"));
    }
}
