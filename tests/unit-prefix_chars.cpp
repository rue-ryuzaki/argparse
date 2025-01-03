/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. prefix chars '='", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().prefix_chars("=").exit_on_error(false);

    SECTION("1.1. without store actions") {
        parser.add_argument(_make_vec("=f", "==foo")).action("store_true").help("foo help");
        parser.add_argument("==bar").action("store_false").help("bar help");

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        CHECK(args0.get<bool>("f") == false);
        CHECK(args0.get<bool>("foo") == false);
        CHECK(args0.get<bool>("bar") == true);

        argparse::Namespace args1 = parser.parse_args(_make_vec("=f", "==bar"));
        CHECK(args1.get<bool>("f") == true);
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.get<bool>("bar") == false);
    }

    SECTION("1.2. with store actions") {
        parser.add_argument(_make_vec("=f", "==foo")).action("store").help("foo help");
        parser.add_argument("==bar").action("store").help("bar help");

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        CHECK(args0.get<std::string>("f") == "");
        CHECK(args0.get<std::string>("foo") == "");
        CHECK(args0.get<std::string>("bar") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("=f", "foo", "==bar=bar"));
        CHECK(args1.get<std::string>("f") == "foo");
        CHECK(args1.get<std::string>("foo") == "foo");
        CHECK(args1.get<std::string>("bar") == "bar");
    }
}
