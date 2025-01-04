/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. parse known arguments", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    parser.add_argument("--foo").action("store_true");
    parser.add_argument("bar");

    SECTION("1.1. all known arguments") {
        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo", "bar"));
        CHECK(args0.get<bool>("foo") == true);
        CHECK(args0.get<std::string>("bar") == "bar");

        argparse::Namespace args1 = parser.parse_known_args(_make_vec("--foo", "bar"));
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.get<std::string>("bar") == "bar");
    }

    SECTION("1.2. have unknown arguments") {
        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "--boo", "bar", "baz")));

        argparse::Namespace args = parser.parse_known_args(_make_vec("--foo", "--boo", "bar", "baz"));
        CHECK(args.get<bool>("foo") == true);
        CHECK(args.get<std::string>("bar") == "bar");
        CHECK(args.unrecognized_args().size() == 2);
        CHECK(args.unrecognized_args_to_args() == "--boo baz");
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
        CHECK(args0.get<std::string>("cmd") == "doit");
        CHECK(args0.get<std::string>("foo") == "bar");
        CHECK(args0.get<int>("rest") == 1);
        CHECK(args0.get<std::vector<int> >("rest").size() == 1);

        argparse::Namespace args1 = parser.parse_intermixed_args(_make_vec("doit", "1", "--foo", "bar", "2", "3"));
        CHECK(args1.get<std::string>("cmd") == "doit");
        CHECK(args1.get<std::string>("foo") == "bar");
        CHECK(args1.get<std::vector<int> >("rest").size() == 3);
        CHECK(args1.to_string("rest") == "[1, 2, 3]");
    }

    SECTION("2.2. subparsers") {
        parser.add_argument("--foo").action("store_true").help("foo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        CHECK_THROWS(parser.parse_intermixed_args(_make_vec()));
        CHECK_THROWS(parser.parse_intermixed_args(_make_vec("a", "11")));
        CHECK_THROWS(parser.parse_intermixed_args(_make_vec("b", "--baz")));
    }
}
