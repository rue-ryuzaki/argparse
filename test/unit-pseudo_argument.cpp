/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse.hpp>

#include "./catch-define.h"

TEST_CASE("1. pseudo-argument '--'", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. options like '--' [fail]") {
        parser.add_argument("--").action("store").help("-- help");

        // dest= is required for options like '--'
        REQUIRE_THROWS(parser.parse_args(_make_vec()));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--=1")));
    }

    SECTION("1.2. options like '--' [ ok ]") {
        // better don't use dest='--'
        parser.add_argument("--").action("store").dest("--").help("-- help");
        REQUIRE_THROWS(parser.add_argument("--").action("store").dest("foo").help("foo help"));

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<std::string>("--") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--=1"));
        REQUIRE(args1.get<std::string>("--") == "1");
    }

    SECTION("1.3. store arguments like optional") {
        parser.add_argument("store1").action("store").help("store1 help");
        parser.add_argument("--store2").action("store").help("store2 help");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--", "--store2"));
        REQUIRE(args0.get<std::string>("store1") == "--store2");
        REQUIRE(args0.get<std::string>("store2") == "");

        REQUIRE_THROWS(parser.parse_args(_make_vec("--", "--store2", "--store2=--store2")));

        argparse::Namespace args1 = parser.parse_args(_make_vec("--store2=--store2", "--", "--store2"));
        REQUIRE(args1.get<std::string>("store1") == "--store2");
        REQUIRE(args1.get<std::string>("store2") == "--store2");
    }

    SECTION("1.4. prefix chars '+'") {
        parser.prefix_chars("+");

        parser.add_argument("store1").action("store").help("store1 help");
        parser.add_argument("++store2").action("store").help("store2 help");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--", "++store2"));
        REQUIRE(args0.get<std::string>("store1") == "++store2");
        REQUIRE(args0.get<std::string>("store2") == "");

        REQUIRE_THROWS(parser.parse_args(_make_vec("--", "++store2", "++store2=++store2")));

        argparse::Namespace args1 = parser.parse_args(_make_vec("++store2=++store2", "--", "++store2"));
        REQUIRE(args1.get<std::string>("store1") == "++store2");
        REQUIRE(args1.get<std::string>("store2") == "++store2");
    }
}