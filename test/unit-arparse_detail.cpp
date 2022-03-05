#include <catch2/catch.hpp>

#include <argparse/argparse.hpp>

TEST_CASE("split to args", "[detail]")
{
    SECTION("without quotes") {
        REQUIRE(argparse::detail::_split_to_args("").empty());
        REQUIRE(argparse::detail::_split_to_args("abc").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("abc xyz").size() == 2);
        REQUIRE(argparse::detail::_split_to_args("abc\\ xyz").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("-f=abc xyz").size() == 2);
    }

    SECTION("with quotes") {
        REQUIRE(argparse::detail::_split_to_args("Homer's dog").size() == 2);
        REQUIRE(argparse::detail::_split_to_args("Homer's\\ dog").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("'Homer's dog'").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("-f='Homer's dog'").size() == 1);
    }
}
