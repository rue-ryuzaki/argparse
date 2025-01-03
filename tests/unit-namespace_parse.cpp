/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

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
        CHECK(args0.get<std::string>("-f") == global_default);
        CHECK(args0.get<std::string>("-b") == local_default);
        CHECK(args0.get<std::string>("--foo") == global_default);
        CHECK(args0.get<std::string>("--bar") == local_default);
        CHECK(args0.get<std::string>("f") == global_default);
        CHECK(args0.get<std::string>("b") == local_default);
        CHECK(args0.get<std::string>("foo") == global_default);
        CHECK(args0.get<std::string>("bar") == local_default);
        CHECK(args0.get<std::string>("pos") == pos);

        argparse::Namespace args1 = parser.parse_args(_make_vec(pos, "-f", foo));
        CHECK(args1.get<std::string>("-f") == foo);
        CHECK(args1.get<std::string>("-b") == local_default);
        CHECK(args1.get<std::string>("--foo") == foo);
        CHECK(args1.get<std::string>("--bar") == local_default);
        CHECK(args1.get<std::string>("f") == foo);
        CHECK(args1.get<std::string>("b") == local_default);
        CHECK(args1.get<std::string>("foo") == foo);
        CHECK(args1.get<std::string>("bar") == local_default);
        CHECK(args1.get<std::string>("pos") == pos);

        argparse::Namespace args2 = parser.parse_args(_make_vec(foo, "--bar", bar), args1);
        CHECK(args2.get<std::string>("-f") == foo);
        CHECK(args2.get<std::string>("-b") == bar);
        CHECK(args2.get<std::string>("--foo") == foo);
        CHECK(args2.get<std::string>("--bar") == bar);
        CHECK(args2.get<std::string>("f") == foo);
        CHECK(args2.get<std::string>("b") == bar);
        CHECK(args2.get<std::string>("foo") == foo);
        CHECK(args2.get<std::string>("bar") == bar);
        CHECK(args2.get<std::string>("pos") == foo);

        argparse::Namespace args3 = parser.parse_args(_make_vec(bar, "--foo", bar, "--bar", bar), args1);
        CHECK(args3.get<std::string>("-f") == bar);
        CHECK(args3.get<std::string>("-b") == bar);
        CHECK(args3.get<std::string>("--foo") == bar);
        CHECK(args3.get<std::string>("--bar") == bar);
        CHECK(args3.get<std::string>("f") == bar);
        CHECK(args3.get<std::string>("b") == bar);
        CHECK(args3.get<std::string>("foo") == bar);
        CHECK(args3.get<std::string>("bar") == bar);
        CHECK(args3.get<std::string>("pos") == bar);
    }

    SECTION("1.2. intermixed parsing") {
        argparse::Namespace args1 = parser.parse_args(_make_vec(pos, "-f", foo, "--bar", bar));
        CHECK(args1.get<std::string>("-f") == foo);
        CHECK(args1.get<std::string>("-b") == bar);
        CHECK(args1.get<std::string>("--foo") == foo);
        CHECK(args1.get<std::string>("--bar") == bar);
        CHECK(args1.get<std::string>("f") == foo);
        CHECK(args1.get<std::string>("b") == bar);
        CHECK(args1.get<std::string>("foo") == foo);
        CHECK(args1.get<std::string>("bar") == bar);
        CHECK(args1.get<std::string>("pos") == pos);

        std::string baz = "baz";

        argparse::Namespace args2 = parser.parse_args(_make_vec(pos, "-f", foo, "--bar", bar));
        CHECK(args2.get<std::string>("-f") == foo);
        CHECK(args2.get<std::string>("-b") == bar);
        CHECK(args2.get<std::string>("--foo") == foo);
        CHECK(args2.get<std::string>("--bar") == bar);
        CHECK(args2.get<std::string>("f") == foo);
        CHECK(args2.get<std::string>("b") == bar);
        CHECK(args2.get<std::string>("foo") == foo);
        CHECK(args2.get<std::string>("bar") == bar);
        CHECK(args2.get<std::string>("pos") == pos);

        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        parser2.add_argument(_make_vec("--baz"));

        // if parser2 have arguments in python there are error: AttributeError: 'tuple' object has no attribute 'baz'
        CHECK_THROWS(parser2.parse_args("", parser.parse_known_args(_make_vec(pos, "-f", foo, "--bar", bar))));
        CHECK_THROWS(parser2.parse_args("", parser.parse_known_args(
                                                _make_vec(pos, "-f", foo, "--bar", bar, "--baz", baz))));

        argparse::Namespace args3 = parser2.parse_args("", args2);
        CHECK(args3.get<std::string>("-f") == foo);
        CHECK(args3.get<std::string>("-b") == bar);
        CHECK(args3.get<std::string>("--foo") == foo);
        CHECK(args3.get<std::string>("--bar") == bar);
        CHECK(args3.get<std::string>("f") == foo);
        CHECK(args3.get<std::string>("b") == bar);
        CHECK(args3.get<std::string>("foo") == foo);
        CHECK(args3.get<std::string>("bar") == bar);
        CHECK(args3.get<std::string>("pos") == pos);
        CHECK(args3.get<std::string>("--baz") == "");
        CHECK(args3.get<std::string>("baz") == "");

        argparse::Namespace args4 = parser2.parse_args(_make_vec("--baz", baz), args2);
        CHECK(args4.get<std::string>("-f") == foo);
        CHECK(args4.get<std::string>("-b") == bar);
        CHECK(args4.get<std::string>("--foo") == foo);
        CHECK(args4.get<std::string>("--bar") == bar);
        CHECK(args4.get<std::string>("f") == foo);
        CHECK(args4.get<std::string>("b") == bar);
        CHECK(args4.get<std::string>("foo") == foo);
        CHECK(args4.get<std::string>("bar") == bar);
        CHECK(args4.get<std::string>("pos") == pos);
        CHECK(args4.get<std::string>("--baz") == baz);
        CHECK(args4.get<std::string>("baz") == baz);
    }
}
