/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument groups", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. example") {
        parser.add_argument("--foo").action("store_true");
        parser.add_argument("bar");

        argparse::ArgumentGroup& group1 = parser.add_argument_group("group 1", "desc1");
        group1.add_argument("--group1").action("store");

        argparse::ArgumentGroup& group2 = parser.add_argument_group("group 2", "desc2");
        group2.add_argument("--group2").action("store");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo", "bar"));
        CHECK(args0.get<bool>("foo") == true);
        CHECK(args0.get<std::string>("bar") == "bar");
        CHECK(args0.get<std::string>("group1") == "");
        CHECK(args0.get<std::string>("group2") == "");

        argparse::Namespace args1 = parser.parse_known_args(_make_vec("--foo", "bar", "--group2=2", "--group1=1"));
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.get<std::string>("bar") == "bar");
        CHECK(args1.get<std::string>("group1") == "1");
        CHECK(args1.get<std::string>("group2") == "2");
    }

    SECTION("1.2. argument_default") {
        parser.argument_default("0");
        parser.add_argument("--foo0");

        argparse::ArgumentGroup& group1 = parser.add_argument_group("group1", "desc1").argument_default("1");
        group1.add_argument("--foo1");

        argparse::ArgumentGroup& group2 = parser.add_argument_group("group2", "desc2").argument_default("2");
        group2.add_argument("--foo2");

        argparse::Namespace args = parser.parse_args("");
        CHECK(args.get<std::string>("foo0") == "0");
        CHECK(args.get<std::string>("foo1") == "1");
        CHECK(args.get<std::string>("foo2") == "2");
    }
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
        CHECK(args0.get<bool>("foo") == false);
        CHECK(args0.get<bool>("bar") == true);
        CHECK(args0.get<std::string>("baz") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--foo"));
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.get<bool>("bar") == true);
        CHECK(args1.get<std::string>("baz") == "");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--bar"));
        CHECK(args2.get<bool>("foo") == false);
        CHECK(args2.get<bool>("bar") == false);
        CHECK(args2.get<std::string>("baz") == "");

        argparse::Namespace args3 = parser.parse_args(_make_vec("baz=1"));
        CHECK(args3.get<bool>("foo") == false);
        CHECK(args3.get<bool>("bar") == true);
        CHECK(args3.get<std::string>("baz") == "1");

        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "--bar")));
        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "baz=1")));
        CHECK_THROWS(parser.parse_args(_make_vec("--bar", "baz=1")));
    }

    SECTION("2.2. required=true") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group().required(true);
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz=").required(false);

        CHECK_THROWS(parser.parse_args(_make_vec()));

        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo"));
        CHECK(args0.get<bool>("foo") == true);
        CHECK(args0.get<bool>("bar") == true);
        CHECK(args0.get<std::string>("baz") == "");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--bar"));
        CHECK(args1.get<bool>("foo") == false);
        CHECK(args1.get<bool>("bar") == false);
        CHECK(args1.get<std::string>("baz") == "");

        argparse::Namespace args2 = parser.parse_args(_make_vec("baz=1"));
        CHECK(args2.get<bool>("foo") == false);
        CHECK(args2.get<bool>("bar") == true);
        CHECK(args2.get<std::string>("baz") == "1");
    }

    SECTION("2.3. required positional") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz");

        CHECK_THROWS_WITH(parser.parse_args(""), "ValueError: mutually exclusive arguments must be optional");
    }

    SECTION("2.4. required operand") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true");
        group.add_argument("--bar").action("store_false");
        group.add_argument("baz=").required(true);

        CHECK_THROWS_WITH(parser.parse_args(""), "ValueError: mutually exclusive arguments must be optional");
    }

    SECTION("2.5. required optional") {
        argparse::MutuallyExclusiveGroup& group = parser.add_mutually_exclusive_group();
        group.add_argument("--foo").action("store_true").required(true);
        group.add_argument("--bar").action("store_false");

        CHECK_THROWS_WITH(parser.parse_args(""), "ValueError: mutually exclusive arguments must be optional");
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

    CHECK_NOTHROW(parser.parse_args("--foo").contains("foo"));
}
