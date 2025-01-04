/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

#ifndef ARGPARSE_CXX_11
void
parser_a_handle_check(argparse::Namespace const& args)
{
    CHECK(args.exists("foo") == false);
    CHECK(args.exists("cmd") == true);
    CHECK(args.exists("bar") == true);
    CHECK(args.exists("baz") == false);
}

void
parser_b_handle_check(argparse::Namespace const& args)
{
    CHECK(args.exists("foo") == false);
    CHECK(args.exists("cmd") == true);
    CHECK(args.exists("bar") == false);
    CHECK(args.exists("baz") == true);
}
#endif  // C++11-

TEST_CASE("1. subparsers", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. main parser without positional arguments (required=false)") {
        parser.add_argument("--foo").action("store_true").help("foo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo"));
        CHECK(args0.exists("cmd") == false);
        CHECK(args0.exists("bar") == false);
        CHECK(args0.exists("baz") == false);
        CHECK(args0.get<bool>("foo") == true);

        argparse::Namespace args1 = parser.parse_args(_make_vec("a", "12"));
        CHECK(args1.exists("cmd") == true);
        CHECK(args1.exists("bar") == true);
        CHECK(args1.exists("baz") == false);
        CHECK(args1.get<uint32_t>("bar") == 12);
        CHECK(args1.get<bool>("foo") == false);
        CHECK(args1.get<std::string>("cmd") == "a");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo", "b", "--baz", "Z"));
        CHECK(args2.exists("cmd") == true);
        CHECK(args2.exists("bar") == false);
        CHECK(args2.exists("baz") == true);
        CHECK(args2.get<bool>("foo") == true);
        CHECK(args2.get<std::string>("baz") == "Z");
        CHECK(args2.get<std::string>("cmd") == "b");

        CHECK_THROWS(parser.parse_args(_make_vec("a", "12", "--foo")));
        CHECK_THROWS(parser.parse_args(_make_vec("b", "--foo")));
        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "a", "12", "b", "--baz", "Z")));
    }

    SECTION("1.2. main parser with store positional arguments") {
        parser.add_argument("--foo").action("store_true").help("foo help");
        parser.add_argument("boo").action("store").help("boo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices(_make_vec("X", "Y", "Z")).help("baz help");

        CHECK_THROWS(parser.parse_args(_make_vec()));

        argparse::Namespace args0 = parser.parse_args(_make_vec("boo", "--foo"));
        CHECK(args0.exists("cmd") == false);
        CHECK(args0.exists("bar") == false);
        CHECK(args0.exists("baz") == false);
        CHECK(args0.get<std::string>("boo") == "boo");
        CHECK(args0.get<bool>("foo") == true);

        argparse::Namespace args1 = parser.parse_args(_make_vec("boo", "--foo", "a", "12"));
        CHECK(args1.exists("cmd") == true);
        CHECK(args1.exists("bar") == true);
        CHECK(args1.exists("baz") == false);
        CHECK(args1.get<std::string>("boo") == "boo");
        CHECK(args1.get<uint32_t>("bar") == 12);
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.get<std::string>("cmd") == "a");

        argparse::Namespace args2 = parser.parse_args(_make_vec("boo", "--foo", "b", "--baz", "Z"));
        CHECK(args2.exists("cmd") == true);
        CHECK(args2.exists("bar") == false);
        CHECK(args2.exists("baz") == true);
        CHECK(args2.get<std::string>("boo") == "boo");
        CHECK(args2.get<bool>("foo") == true);
        CHECK(args2.get<std::string>("baz") == "Z");
        CHECK(args2.get<std::string>("cmd") == "b");

        argparse::Namespace args3 = parser.parse_args(_make_vec("boo", "a", "12"));
        CHECK(args3.exists("cmd") == true);
        CHECK(args3.exists("bar") == true);
        CHECK(args3.exists("baz") == false);
        CHECK(args3.get<std::string>("boo") == "boo");
        CHECK(args3.get<uint32_t>("bar") == 12);
        CHECK(args3.get<bool>("foo") == false);
        CHECK(args3.get<std::string>("cmd") == "a");

        argparse::Namespace args4 = parser.parse_args(_make_vec("--foo", "boo", "a", "12"));
        CHECK(args4.exists("cmd") == true);
        CHECK(args4.exists("bar") == true);
        CHECK(args4.exists("baz") == false);
        CHECK(args4.get<std::string>("boo") == "boo");
        CHECK(args4.get<uint32_t>("bar") == 12);
        CHECK(args4.get<bool>("foo") == true);
        CHECK(args4.get<std::string>("cmd") == "a");

        argparse::Namespace args5 = parser.parse_args(_make_vec("--foo", "boo", "b", "--baz", "Z"));
        CHECK(args5.exists("cmd") == true);
        CHECK(args5.exists("bar") == false);
        CHECK(args5.exists("baz") == true);
        CHECK(args5.get<std::string>("boo") == "boo");
        CHECK(args5.get<bool>("foo") == true);
        CHECK(args5.get<std::string>("baz") == "Z");
        CHECK(args5.get<std::string>("cmd") == "b");
    }

    SECTION("1.3. main parser with store_const positional arguments") {
        std::string const_value = "const";

        parser.add_argument("--foo").action("store_true").help("foo help");
        parser.add_argument("boo").action("store_true").help("boo help");
        parser.add_argument("doo").action("store_false").help("doo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices(_make_vec("X", "Y", "Z")).help("baz help");

        parser.add_argument("coo").action("store_const").const_value(const_value).help("coo help");

        argparse::Namespace args0 = parser.parse_args(_make_vec("--foo"));
        CHECK(args0.exists("cmd") == false);
        CHECK(args0.exists("bar") == false);
        CHECK(args0.exists("baz") == false);
        CHECK(args0.get<bool>("boo") == true);
        CHECK(args0.get<bool>("doo") == false);
        CHECK(args0.get<std::string>("coo") == const_value);
        CHECK(args0.get<bool>("foo") == true);

        argparse::Namespace args1 = parser.parse_args(_make_vec("--foo", "a", "12"));
        CHECK(args1.exists("cmd") == true);
        CHECK(args1.exists("bar") == true);
        CHECK(args1.exists("baz") == false);
        CHECK(args1.get<bool>("boo") == true);
        CHECK(args1.get<bool>("doo") == false);
        CHECK(args1.get<std::string>("coo") == const_value);
        CHECK(args1.get<uint32_t>("bar") == 12);
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.get<std::string>("cmd") == "a");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo", "b", "--baz", "Z"));
        CHECK(args2.exists("cmd") == true);
        CHECK(args2.exists("bar") == false);
        CHECK(args2.exists("baz") == true);
        CHECK(args2.get<bool>("boo") == true);
        CHECK(args2.get<bool>("doo") == false);
        CHECK(args2.get<std::string>("coo") == const_value);
        CHECK(args2.get<bool>("foo") == true);
        CHECK(args2.get<std::string>("baz") == "Z");
        CHECK(args2.get<std::string>("cmd") == "b");

        argparse::Namespace args3 = parser.parse_args(_make_vec("a", "12"));
        CHECK(args3.exists("cmd") == true);
        CHECK(args3.exists("bar") == true);
        CHECK(args3.exists("baz") == false);
        CHECK(args3.get<bool>("boo") == true);
        CHECK(args3.get<bool>("doo") == false);
        CHECK(args3.get<std::string>("coo") == const_value);
        CHECK(args3.get<uint32_t>("bar") == 12);
        CHECK(args3.get<bool>("foo") == false);
        CHECK(args3.get<std::string>("cmd") == "a");

        argparse::Namespace args4 = parser.parse_args(_make_vec("--foo", "b", "--baz", "Z"));
        CHECK(args4.exists("cmd") == true);
        CHECK(args4.exists("bar") == false);
        CHECK(args4.exists("baz") == true);
        CHECK(args4.get<bool>("boo") == true);
        CHECK(args4.get<bool>("doo") == false);
        CHECK(args4.get<std::string>("coo") == const_value);
        CHECK(args4.get<bool>("foo") == true);
        CHECK(args4.get<std::string>("baz") == "Z");
        CHECK(args4.get<std::string>("cmd") == "b");
    }

    SECTION("1.4. main parser with negative number options present") {
        parser.add_argument("-1").dest("one").help("one help");
        parser.add_argument("--foo").action("store_true").help("foo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        // so -1 is an option in main
        argparse::Namespace args0 = parser.parse_args(_make_vec("-1", "x"));
        CHECK(args0.exists("cmd") == false);
        CHECK(args0.exists("bar") == false);
        CHECK(args0.exists("baz") == false);
        CHECK(args0.get<std::string>("one") == "x");
        CHECK(args0.get<bool>("foo") == false);

        argparse::Namespace args1 = parser.parse_args(_make_vec("a", "-1"));
        CHECK(args1.exists("cmd") == true);
        CHECK(args1.exists("bar") == true);
        CHECK(args1.exists("baz") == false);
        CHECK(args1.get<std::string>("one") == "");
        CHECK(args1.get<int32_t>("bar") == -1);
        CHECK(args1.get<bool>("foo") == false);
        CHECK(args1.get<std::string>("cmd") == "a");

        // so -2 is an option in main
        CHECK_THROWS(parser.parse_args(_make_vec("-2")));

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo", "a", "-2"));
        CHECK(args2.exists("cmd") == true);
        CHECK(args2.exists("bar") == true);
        CHECK(args2.exists("baz") == false);
        CHECK(args2.get<std::string>("one") == "");
        CHECK(args2.get<int32_t>("bar") == -2);
        CHECK(args2.get<bool>("foo") == true);
        CHECK(args2.get<std::string>("cmd") == "a");

        // so both -1s are options in main
        CHECK_THROWS(parser.parse_args(_make_vec("-1", "-1")));
    }

    SECTION("1.5. subparsers with negative number options present") {
        parser.add_argument("--foo").action("store_true").help("foo help");
        parser.add_argument("--boo").action("store").help("boo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");
        parser_a.add_argument("-1").dest("one").help("one help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        // so -1 is an option in subparsers
        argparse::Namespace args0 = parser.parse_args(_make_vec("--boo", "-1", "a", "-1", "x", "1"));
        CHECK(args0.exists("bar") == true);
        CHECK(args0.exists("baz") == false);
        CHECK(args0.get<std::string>("one") == "x");
        CHECK(args0.get<std::string>("boo") == "-1");
        CHECK(args0.get<int32_t>("bar") == 1);
        CHECK(args0.get<bool>("foo") == false);
        CHECK(args0.get<std::string>("cmd") == "a");

        argparse::Namespace args1 = parser.parse_args(_make_vec("a", "-1", "x", "1"));
        CHECK(args1.exists("bar") == true);
        CHECK(args1.exists("baz") == false);
        CHECK(args1.get<std::string>("one") == "x");
        CHECK(args1.get<std::string>("boo") == "");
        CHECK(args1.get<int32_t>("bar") == 1);
        CHECK(args1.get<bool>("foo") == false);
        CHECK(args1.get<std::string>("cmd") == "a");

        // so -2 is an option in subparsers
        CHECK_THROWS(parser.parse_args(_make_vec("a", "-1", "x", "-2")));
        // so both -1s are options in subparsers
        CHECK_THROWS(parser.parse_args(_make_vec("a", "-1", "-1")));
    }

    SECTION("1.6. subparsers required=true") {
        parser.add_argument("--foo").action("store_true").help("foo help");

        argparse::SubParsers& subparsers
                = parser.add_subparsers().required(true).dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec("--foo")));
    }

    SECTION("1.7. subparsers namespace handle") {
        parser.add_argument("--foo").action("store_true").help("foo help");

        argparse::SubParsers& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        argparse::ArgumentParser& parser_a = subparsers.add_parser("a").help("a help");
#ifdef ARGPARSE_CXX_11
        parser_a.handle([] (argparse::Namespace const& args)
        {
            CHECK(args.exists("foo") == false);
            CHECK(args.exists("cmd") == true);
            CHECK(args.exists("bar") == true);
            CHECK(args.exists("baz") == false);
        });
#else
        parser_a.handle(&parser_a_handle_check);
#endif  // C++11+
        parser_a.add_argument("bar").help("bar help");
        CHECK(subparsers.parser_names().size() == 1);

        argparse::ArgumentParser& parser_b = subparsers.add_parser("b").help("b help");
#ifdef ARGPARSE_CXX_11
        parser_b.handle([] (argparse::Namespace const& args)
        {
             CHECK(args.exists("foo") == false);
             CHECK(args.exists("cmd") == true);
             CHECK(args.exists("bar") == false);
             CHECK(args.exists("baz") == true);
        });
#else
        parser_b.handle(&parser_b_handle_check);
#endif  // C++11+
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");
        CHECK(subparsers.parser_names().size() == 2);

        parser.parse_args(_make_vec("--foo"));
        parser.parse_args(_make_vec("a", "12"));
        parser.parse_args(_make_vec("--foo", "b", "--baz", "Z"));
    }
}
