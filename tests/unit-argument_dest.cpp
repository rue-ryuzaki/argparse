/*
 * Copyright (c) 2021-2026 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument dest", "[argument]")
{
}

TEST_CASE("2. argument dest override", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    std::string dest_foo = "foo_dest";
    std::string dest_bar = "bar_dest";
    std::string dest_foobar = "foobar_dest";

    argparse::ArgumentParser parser = argparse::ArgumentParser()
            .argument_default(global_default).exit_on_error(false);

    SECTION("2.1. positional arguments") {
        CHECK_THROWS(parser.add_argument("foobar").dest(dest_foobar));
    }

    SECTION("2.2. optional arguments") {
        parser.add_argument("--foo").dest(dest_foo);
        parser.add_argument("--bar").dest(dest_bar).default_value(local_default);
        parser.add_argument("foobar");

        argparse::Namespace args1 = parser.parse_args(_make_vec("foobar"));
        CHECK_THROWS(args1.get<std::string>("--foo"));
        CHECK_THROWS(args1.get<std::string>("--bar"));
        CHECK(args1.get<std::string>("foobar") == "foobar");

        CHECK(args1.get<std::string>(dest_foo) == global_default);
        CHECK(args1.get<std::string>(dest_bar) == local_default);
        CHECK(args1.get<std::string>("foobar") == "foobar");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=foo", "foobar"));
        CHECK(args2.get<std::string>(dest_foo) == "foo");
        CHECK(args2.get<std::string>(dest_bar) == local_default);
        CHECK(args2.get<std::string>("foobar") == "foobar");
    }
}
