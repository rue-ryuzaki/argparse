/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include "./argparse_decl.hpp"
#include "./catch-define.h"

TEST_CASE("1. prefix chars '='", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().prefix_chars("=").exit_on_error(false);

    SECTION("1.1. without store actions") {
        parser.add_argument(_make_vec("=f", "==foo")).action("store_true").help("foo help");
        parser.add_argument("==bar").action("store_false").help("bar help");

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<bool>("f") == false);
        REQUIRE(args0.get<bool>("foo") == false);
        REQUIRE(args0.get<bool>("bar") == true);

        argparse::Namespace args1 = parser.parse_args(_make_vec("=f", "==bar"));
        REQUIRE(args1.get<bool>("f") == true);
        REQUIRE(args1.get<bool>("foo") == true);
        REQUIRE(args1.get<bool>("bar") == false);
    }

    SECTION("1.2. with store actions") {
        parser.add_argument(_make_vec("=f", "==foo")).action("store").help("foo help");
        parser.add_argument("==bar").action("store").help("bar help");

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<std::string>("f") == "");
        REQUIRE(args0.get<std::string>("foo") == "");
        REQUIRE(args0.get<std::string>("bar") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("=f", "foo", "==bar=bar"));
        REQUIRE(args1.get<std::string>("f") == "foo");
        REQUIRE(args1.get<std::string>("foo") == "foo");
        REQUIRE(args1.get<std::string>("bar") == "bar");
    }
}
