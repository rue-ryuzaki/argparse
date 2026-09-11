/*
 * Copyright (c) 2026 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument metavar", "[argument]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
    argparse::Argument& argument = parser.add_argument("-f");

    SECTION("1.1. no value") {
        CHECK(argument.metavar() == std::string());
    }

    SECTION("1.2. single value") {
        argument.metavar("foo");

        CHECK(argument.metavar() == "foo");
    }

    SECTION("1.3. multiple values") {
        argument.nargs(2).metavar("foo", "bar");

        CHECK(argument.metavar() == "(foo, bar)");
    }
}
