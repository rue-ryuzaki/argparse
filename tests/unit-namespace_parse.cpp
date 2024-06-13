/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#include <argparse/argparse_decl.hpp>
#include "./catch-define.h"

TEST_CASE("1. namespace", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument(_make_vec("pos"));
    parser.add_argument(_make_vec("-f", "--foo"));
    parser.add_argument(_make_vec("-b", "--bar")).default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";
    std::string pos = "pos";

    SECTION("1.1. simple example") {
        argparse::Namespace args0 = parser.parse_args(_make_vec(pos));
        REQUIRE(args0.get<std::string>("-f") == global_default);
        REQUIRE(args0.get<std::string>("-b") == local_default);
        REQUIRE(args0.get<std::string>("--foo") == global_default);
        REQUIRE(args0.get<std::string>("--bar") == local_default);
        REQUIRE(args0.get<std::string>("f") == global_default);
        REQUIRE(args0.get<std::string>("b") == local_default);
        REQUIRE(args0.get<std::string>("foo") == global_default);
        REQUIRE(args0.get<std::string>("bar") == local_default);
        REQUIRE(args0.get<std::string>("pos") == pos);

        argparse::Namespace args1 = parser.parse_args(_make_vec(pos, "-f", foo));
        REQUIRE(args1.get<std::string>("-f") == foo);
        REQUIRE(args1.get<std::string>("-b") == local_default);
        REQUIRE(args1.get<std::string>("--foo") == foo);
        REQUIRE(args1.get<std::string>("--bar") == local_default);
        REQUIRE(args1.get<std::string>("f") == foo);
        REQUIRE(args1.get<std::string>("b") == local_default);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == local_default);
        REQUIRE(args1.get<std::string>("pos") == pos);

        argparse::Namespace args2 = parser.parse_args(_make_vec(foo, "--bar", bar), args1);
        REQUIRE(args2.get<std::string>("-f") == foo);
        REQUIRE(args2.get<std::string>("-b") == bar);
        REQUIRE(args2.get<std::string>("--foo") == foo);
        REQUIRE(args2.get<std::string>("--bar") == bar);
        REQUIRE(args2.get<std::string>("f") == foo);
        REQUIRE(args2.get<std::string>("b") == bar);
        REQUIRE(args2.get<std::string>("foo") == foo);
        REQUIRE(args2.get<std::string>("bar") == bar);
        REQUIRE(args2.get<std::string>("pos") == foo);

        argparse::Namespace args3 = parser.parse_args(_make_vec(bar, "--foo", bar, "--bar", bar), args1);
        REQUIRE(args3.get<std::string>("-f") == bar);
        REQUIRE(args3.get<std::string>("-b") == bar);
        REQUIRE(args3.get<std::string>("--foo") == bar);
        REQUIRE(args3.get<std::string>("--bar") == bar);
        REQUIRE(args3.get<std::string>("f") == bar);
        REQUIRE(args3.get<std::string>("b") == bar);
        REQUIRE(args3.get<std::string>("foo") == bar);
        REQUIRE(args3.get<std::string>("bar") == bar);
        REQUIRE(args3.get<std::string>("pos") == bar);
    }

    SECTION("1.2. intermixed parsing") {
        argparse::Namespace args1 = parser.parse_args(_make_vec(pos, "-f", foo, "--bar", bar));
        REQUIRE(args1.get<std::string>("-f") == foo);
        REQUIRE(args1.get<std::string>("-b") == bar);
        REQUIRE(args1.get<std::string>("--foo") == foo);
        REQUIRE(args1.get<std::string>("--bar") == bar);
        REQUIRE(args1.get<std::string>("f") == foo);
        REQUIRE(args1.get<std::string>("b") == bar);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == bar);
        REQUIRE(args1.get<std::string>("pos") == pos);

        std::string baz = "baz";

        argparse::Namespace args2 = parser.parse_args(_make_vec(pos, "-f", foo, "--bar", bar));
        REQUIRE(args2.get<std::string>("-f") == foo);
        REQUIRE(args2.get<std::string>("-b") == bar);
        REQUIRE(args2.get<std::string>("--foo") == foo);
        REQUIRE(args2.get<std::string>("--bar") == bar);
        REQUIRE(args2.get<std::string>("f") == foo);
        REQUIRE(args2.get<std::string>("b") == bar);
        REQUIRE(args2.get<std::string>("foo") == foo);
        REQUIRE(args2.get<std::string>("bar") == bar);
        REQUIRE(args2.get<std::string>("pos") == pos);

        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        parser2.add_argument(_make_vec("--baz"));

        // if parser2 have arguments in python there are error: AttributeError: 'tuple' object has no attribute 'baz'
        REQUIRE_THROWS(parser2.parse_args("", parser.parse_known_args(_make_vec(pos, "-f", foo, "--bar", bar))));
        REQUIRE_THROWS(parser2.parse_args("", parser.parse_known_args(
                                                _make_vec(pos, "-f", foo, "--bar", bar, "--baz", baz))));

        argparse::Namespace args3 = parser2.parse_args("", args2);
        REQUIRE(args3.get<std::string>("-f") == foo);
        REQUIRE(args3.get<std::string>("-b") == bar);
        REQUIRE(args3.get<std::string>("--foo") == foo);
        REQUIRE(args3.get<std::string>("--bar") == bar);
        REQUIRE(args3.get<std::string>("f") == foo);
        REQUIRE(args3.get<std::string>("b") == bar);
        REQUIRE(args3.get<std::string>("foo") == foo);
        REQUIRE(args3.get<std::string>("bar") == bar);
        REQUIRE(args3.get<std::string>("pos") == pos);
        REQUIRE(args3.get<std::string>("--baz") == "");
        REQUIRE(args3.get<std::string>("baz") == "");

        argparse::Namespace args4 = parser2.parse_args(_make_vec("--baz", baz), args2);
        REQUIRE(args4.get<std::string>("-f") == foo);
        REQUIRE(args4.get<std::string>("-b") == bar);
        REQUIRE(args4.get<std::string>("--foo") == foo);
        REQUIRE(args4.get<std::string>("--bar") == bar);
        REQUIRE(args4.get<std::string>("f") == foo);
        REQUIRE(args4.get<std::string>("b") == bar);
        REQUIRE(args4.get<std::string>("foo") == foo);
        REQUIRE(args4.get<std::string>("bar") == bar);
        REQUIRE(args4.get<std::string>("pos") == pos);
        REQUIRE(args4.get<std::string>("--baz") == baz);
        REQUIRE(args4.get<std::string>("baz") == baz);
    }
}
