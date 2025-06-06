/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. pseudo-argument '--'", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. options like '--' [fail]") {
        parser.add_argument("--").action("store").help("-- help");

        // dest= is required for options like '--'
        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec("--=1")));
    }

    SECTION("1.2. options like '--' [ ok ]") {
        // better don't use dest='--'
        parser.add_argument("--").action("store").dest("--").help("-- help");
        REQUIRE_THROWS(parser.add_argument("--").action("store").dest("foo").help("foo help"));

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        CHECK(args0.get<std::string>("--") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--=1"));
        CHECK(args1.get<std::string>("--") == "1");
    }

    SECTION("1.3. store arguments like optional") {
        parser.add_argument("store1").action("store").help("store1 help");
        parser.add_argument("--store2").action("store").help("store2 help");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--", "--store2"));
        CHECK(args0.get<std::string>("store1") == "--store2");
        CHECK(args0.get<std::string>("store2") == "");

        CHECK_THROWS(parser.parse_args(_make_vec("--", "--store2", "--store2=--store2")));

        argparse::Namespace args1 = parser.parse_args(_make_vec("--store2=--store2", "--", "--store2"));
        CHECK(args1.get<std::string>("store1") == "--store2");
        CHECK(args1.get<std::string>("store2") == "--store2");
    }

    SECTION("1.4. prefix chars '+'") {
        parser.prefix_chars("+");

        parser.add_argument("store1").action("store").help("store1 help");
        parser.add_argument("++store2").action("store").help("store2 help");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--", "++store2"));
        CHECK(args0.get<std::string>("store1") == "++store2");
        CHECK(args0.get<std::string>("store2") == "");

        CHECK_THROWS(parser.parse_args(_make_vec("--", "++store2", "++store2=++store2")));

        argparse::Namespace args1 = parser.parse_args(_make_vec("++store2=++store2", "--", "++store2"));
        CHECK(args1.get<std::string>("store1") == "++store2");
        CHECK(args1.get<std::string>("store2") == "++store2");
    }
}
