/*
 * Copyright (c) 2021-2026 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument choices", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser()
            .argument_default(global_default).exit_on_error(false);

    SECTION("1.1. simple usage") {
        parser.add_argument("--foo").choices(_make_vec("foo1", "foo2", "foo3"));
        parser.add_argument("--bar").choices(_make_vec("bar1", "bar2", "bar3")).default_value(local_default);
        parser.add_argument("foobar").choices(_make_vec("foobar1", "foobar2", "foobar3"));

        CHECK_THROWS(parser.parse_args(_make_vec("foo")));
        CHECK_THROWS(parser.parse_args(_make_vec("foobar")));
        CHECK_THROWS(parser.parse_args(_make_vec("foobar1", "--foo", "bar1")));
        CHECK_THROWS(parser.parse_args(_make_vec("foobar2", "--foo", "bar1", "--bar=bar1")));

        argparse::Namespace args1 = parser.parse_args(_make_vec("foobar1"));
        CHECK(args1.get<std::string>("--foo") == global_default);
        CHECK(args1.get<std::string>("--bar") == local_default);
        CHECK(args1.get<std::string>("foobar") == "foobar1");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=foo3", "foobar3"));
        CHECK(args2.get<std::string>("--foo") == "foo3");
        CHECK(args2.get<std::string>("--bar") == local_default);
        CHECK(args2.get<std::string>("foobar") == "foobar3");
    }

    SECTION("1.2. choices as string") {
        parser.add_argument("--foo").choices("FO");
        parser.add_argument("--bar").choices("BAR").default_value(local_default);

        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "bar")));
        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "bar", "--bar=bar")));

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--foo") == global_default);
        CHECK(args1.get<std::string>("--bar") == local_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=F", "--bar", "R"));
        CHECK(args2.get<std::string>("--foo") == "F");
        CHECK(args2.get<std::string>("--bar") == "R");
    }

    SECTION("1.2.1. choices as utf-8 string") {
        parser.add_argument("--foo").choices("αβγδ");

        CHECK_THROWS(parser.parse_args("--foo=bar"));

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<std::string>("--foo") == global_default);

        argparse::Namespace args2 = parser.parse_args("--foo=δ");
        CHECK(args2.get<std::string>("--foo") == "δ");
    }

    SECTION("1.3. empty choices") {
        parser.add_argument("--foo").choices("");
        parser.add_argument("--bar").choices("BAR").default_value(local_default);

        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "bar")));
        CHECK_THROWS(parser.parse_args(_make_vec("--foo", "bar", "--bar=bar")));

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--foo") == global_default);
        CHECK(args1.get<std::string>("--bar") == local_default);

        CHECK_THROWS(parser.parse_args(_make_vec("--foo=F", "--bar", "R")));

        argparse::Namespace args2 = parser.parse_args(_make_vec("--bar", "R"));
        CHECK(args2.get<std::string>("--foo") == global_default);
        CHECK(args2.get<std::string>("--bar") == "R");

        argparse::Namespace args3 = parser.parse_args(_make_vec( "--foo=", "--bar", "R"));
        CHECK(args3.get<std::string>("--foo") == "");
        CHECK(args3.get<std::string>("--bar") == "R");

        argparse::Namespace args4 = parser.parse_args(_make_vec("--foo=''", "--bar", "R"));
        CHECK(args4.get<std::string>("--foo") == "");
        CHECK(args4.get<std::string>("--bar") == "R");

        argparse::Namespace args5 = parser.parse_args(_make_vec("--foo", "", "--bar", "R"));
        CHECK(args5.get<std::string>("--foo") == "");
        CHECK(args5.get<std::string>("--bar") == "R");
    }
}
