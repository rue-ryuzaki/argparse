/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse.hpp>

#include "./catch-define.h"

TEST_CASE("1. parse known arguments", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    parser.add_argument("--foo").action("store_true");
    parser.add_argument("bar");

    SECTION("1.1. all known arguments") {
        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo", "bar"));
        REQUIRE(args0.get<bool>("foo") == true);
        REQUIRE(args0.get<std::string>("bar") == "bar");

        argparse::Namespace args1 = parser.parse_known_args(_make_vec("--foo", "bar"));
        REQUIRE(args1.get<bool>("foo") == true);
        REQUIRE(args1.get<std::string>("bar") == "bar");
    }

    SECTION("1.2. have unknown arguments") {
        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "--boo", "bar", "baz")));

        argparse::Namespace args = parser.parse_known_args(_make_vec("--foo", "--boo", "bar", "baz"));
        REQUIRE(args.get<bool>("foo") == true);
        REQUIRE(args.get<std::string>("bar") == "bar");
        REQUIRE(args.unrecognized_args().size() == 2);
        REQUIRE(args.unrecognized_args_to_args() == "--boo baz");
    }
}

TEST_CASE("2. intermixed parsing", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("2.1. simple example") {
        parser.add_argument("--foo");
        parser.add_argument("cmd");
        parser.add_argument("rest").nargs("*");

        argparse::Namespace args0 = parser.parse_known_args(_make_vec("doit", "1", "--foo", "bar", "2", "3"));
        REQUIRE(args0.get<std::string>("cmd") == "doit");
        REQUIRE(args0.get<std::string>("foo") == "bar");
        REQUIRE(args0.get<int>("rest") == 1);
        REQUIRE(args0.get<std::vector<int> >("rest").size() == 1);

        argparse::Namespace args1 = parser.parse_intermixed_args(_make_vec("doit", "1", "--foo", "bar", "2", "3"));
        REQUIRE(args1.get<std::string>("cmd") == "doit");
        REQUIRE(args1.get<std::string>("foo") == "bar");
        REQUIRE(args1.get<std::vector<int> >("rest").size() == 3);
        REQUIRE(args1.to_string("rest") == "[1, 2, 3]");
    }

    SECTION("2.2. subparsers") {
        parser.add_argument("--foo").action("store_true").help("foo help");

        argparse::ArgumentParser::Subparser& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        REQUIRE_THROWS(parser.parse_intermixed_args(_make_vec()));
        REQUIRE_THROWS(parser.parse_intermixed_args(_make_vec("a", "11")));
        REQUIRE_THROWS(parser.parse_intermixed_args(_make_vec("b", "--baz")));
    }
}
