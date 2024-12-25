/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument groups", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    parser.add_argument("--foo").action("store_true");
    parser.add_argument("bar");

    argparse::ArgumentGroup& group1 = parser.add_argument_group("group 1", "desc");
    group1.add_argument("--group1").action("store");

    argparse::ArgumentGroup& group2 = parser.add_argument_group("group 2", "desc");
    group2.add_argument("--group2").action("store");

    argparse::Namespace args0 = parser.parse_args(_make_vec("--foo", "bar"));
    REQUIRE(args0.get<bool>("foo") == true);
    REQUIRE(args0.get<std::string>("bar") == "bar");
    REQUIRE(args0.get<std::string>("group1") == "");
    REQUIRE(args0.get<std::string>("group2") == "");

    argparse::Namespace args1 = parser.parse_known_args(_make_vec("--foo", "bar", "--group2=2", "--group1=1"));
    REQUIRE(args1.get<bool>("foo") == true);
    REQUIRE(args1.get<std::string>("bar") == "bar");
    REQUIRE(args1.get<std::string>("group1") == "1");
    REQUIRE(args1.get<std::string>("group2") == "2");
}

TEST_CASE("2. mutual exclusion", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("2.1. required=false") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz=").required(false);

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<bool>("foo") == false);
        REQUIRE(args0.get<bool>("bar") == true);
        REQUIRE(args0.get<std::string>("baz") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--foo"));
        REQUIRE(args1.get<bool>("foo") == true);
        REQUIRE(args1.get<bool>("bar") == true);
        REQUIRE(args1.get<std::string>("baz") == "");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--bar"));
        REQUIRE(args2.get<bool>("foo") == false);
        REQUIRE(args2.get<bool>("bar") == false);
        REQUIRE(args2.get<std::string>("baz") == "");

        argparse::Namespace args3 = parser.parse_args(_make_vec("baz=1"));
        REQUIRE(args3.get<bool>("foo") == false);
        REQUIRE(args3.get<bool>("bar") == true);
        REQUIRE(args3.get<std::string>("baz") == "1");

        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "--bar")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "baz=1")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--bar", "baz=1")));
    }

    SECTION("2.2. required=true") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group().required(true);
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz=").required(false);

        REQUIRE_THROWS(parser.parse_args(_make_vec()));

        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo"));
        REQUIRE(args0.get<bool>("foo") == true);
        REQUIRE(args0.get<bool>("bar") == true);
        REQUIRE(args0.get<std::string>("baz") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--bar"));
        REQUIRE(args1.get<bool>("foo") == false);
        REQUIRE(args1.get<bool>("bar") == false);
        REQUIRE(args1.get<std::string>("baz") == "");

        argparse::Namespace args2 = parser.parse_args(_make_vec("baz=1"));
        REQUIRE(args2.get<bool>("foo") == false);
        REQUIRE(args2.get<bool>("bar") == true);
        REQUIRE(args2.get<std::string>("baz") == "1");
    }

    SECTION("2.3. required positional") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz");

        REQUIRE_THROWS_WITH(parser.parse_args(""), "ValueError: mutually exclusive arguments must be optional");
    }

    SECTION("2.4. required operand") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz=").required(true);

        REQUIRE_THROWS_WITH(parser.parse_args(""), "ValueError: mutually exclusive arguments must be optional");
    }

    SECTION("2.5. required optional") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true").required(true);
        group.add_argument("--bar").action("store_false");

        REQUIRE_THROWS_WITH(parser.parse_args(""), "ValueError: mutually exclusive arguments must be optional");
    }
}

TEST_CASE("3. argument group with mutual exclusion", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    argparse::ArgumentGroup& group1 = parser.add_argument_group("group 1", "desc");

    argparse::MutuallyExclusiveGroup& group = group1.add_mutually_exclusive_group();
    group.add_argument("--foo").action("store_true");
    group.add_argument("--bar").action("store_false");
    group.add_argument("baz=").required(false);

    REQUIRE_NOTHROW(parser.parse_args("--foo").contains("foo"));
}
