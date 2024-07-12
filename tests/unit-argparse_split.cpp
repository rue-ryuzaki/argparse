/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. split to args", "[argparse]")
{
    SECTION("1.1. without quotes") {
        REQUIRE(argparse::split_to_args("").empty());
        REQUIRE(argparse::split_to_args("abc").size() == 1);
        REQUIRE(argparse::split_to_args("abc xyz").size() == 2);
        REQUIRE(argparse::split_to_args("abc\\ xyz").size() == 1);
        REQUIRE(argparse::split_to_args("-f=abc xyz").size() == 2);
    }

    SECTION("1.2. with quotes") {
        REQUIRE(argparse::split_to_args("Homer's dog").size() == 2);
        REQUIRE(argparse::split_to_args("Homer's\\ dog").size() == 1);
        REQUIRE(argparse::split_to_args("'Homer's dog'").size() == 1);
        REQUIRE(argparse::split_to_args("-f='Homer's dog'").size() == 1);
    }
}
