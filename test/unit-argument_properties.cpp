/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include "./argparse_decl.hpp"
#include "./catch-define.h"

TEST_CASE("1. argument choices", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

    SECTION("1.1. simple usage") {
        parser.add_argument("--foo").choices(_make_vec("foo1", "foo2", "foo3"));
        parser.add_argument("--bar").choices(_make_vec("bar1", "bar2", "bar3")).default_value(local_default);
        parser.add_argument("foobar").choices(_make_vec("foobar1", "foobar2", "foobar3"));

        REQUIRE_THROWS(parser.parse_args(_make_vec("foo")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("foobar")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("foobar1", "--foo", "bar1")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("foobar2", "--foo", "bar1", "--bar=bar1")));

        argparse::Namespace args1 = parser.parse_args(_make_vec("foobar1"));
        REQUIRE(args1.get<std::string>("--foo") == global_default);
        REQUIRE(args1.get<std::string>("--bar") == local_default);
        REQUIRE(args1.get<std::string>("foobar") == "foobar1");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=foo3", "foobar3"));
        REQUIRE(args2.get<std::string>("--foo") == "foo3");
        REQUIRE(args2.get<std::string>("--bar") == local_default);
        REQUIRE(args2.get<std::string>("foobar") == "foobar3");
    }

    SECTION("1.2. choices as string") {
        parser.add_argument("--foo").choices("FO");
        parser.add_argument("--bar").choices("BAR").default_value(local_default);

        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "bar")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "bar", "--bar=bar")));

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<std::string>("--foo") == global_default);
        REQUIRE(args1.get<std::string>("--bar") == local_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=F", "--bar", "R"));
        REQUIRE(args2.get<std::string>("--foo") == "F");
        REQUIRE(args2.get<std::string>("--bar") == "R");
    }

    SECTION("1.3. empty choices") {
        parser.add_argument("--foo").choices("");
        parser.add_argument("--bar").choices("BAR").default_value(local_default);

        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "bar")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo", "bar", "--bar=bar")));

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<std::string>("--foo") == global_default);
        REQUIRE(args1.get<std::string>("--bar") == local_default);

        REQUIRE_THROWS(parser.parse_args(_make_vec("--foo=F", "--bar", "R")));

        argparse::Namespace args2 = parser.parse_args(_make_vec("--bar", "R"));
        REQUIRE(args2.get<std::string>("--foo") == global_default);
        REQUIRE(args2.get<std::string>("--bar") == "R");

        argparse::Namespace args3 = parser.parse_args(_make_vec( "--foo=", "--bar", "R"));
        REQUIRE(args3.get<std::string>("--foo") == "");
        REQUIRE(args3.get<std::string>("--bar") == "R");

        argparse::Namespace args4 = parser.parse_args(_make_vec("--foo=''", "--bar", "R"));
        REQUIRE(args4.get<std::string>("--foo") == "");
        REQUIRE(args4.get<std::string>("--bar") == "R");

        argparse::Namespace args5 = parser.parse_args(_make_vec("--foo", "", "--bar", "R"));
        REQUIRE(args5.get<std::string>("--foo") == "");
        REQUIRE(args5.get<std::string>("--bar") == "R");
    }
}

TEST_CASE("2. argument dest", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    std::string dest_foo = "foo_dest";
    std::string dest_bar = "bar_dest";
    std::string dest_foobar = "foobar_dest";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

    SECTION("2.1. positional arguments") {
        REQUIRE_THROWS(parser.add_argument("foobar").dest(dest_foobar));
    }

    SECTION("2.2. optional arguments") {
        parser.add_argument("--foo").dest(dest_foo);
        parser.add_argument("--bar").dest(dest_bar).default_value(local_default);
        parser.add_argument("foobar");

        argparse::Namespace args1 = parser.parse_args(_make_vec("foobar"));
        REQUIRE_THROWS(args1.get<std::string>("--foo"));
        REQUIRE_THROWS(args1.get<std::string>("--bar"));
        REQUIRE(args1.get<std::string>("foobar") == "foobar");

        REQUIRE(args1.get<std::string>(dest_foo) == global_default);
        REQUIRE(args1.get<std::string>(dest_bar) == local_default);
        REQUIRE(args1.get<std::string>("foobar") == "foobar");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=foo", "foobar"));
        REQUIRE(args2.get<std::string>(dest_foo) == "foo");
        REQUIRE(args2.get<std::string>(dest_bar) == local_default);
        REQUIRE(args2.get<std::string>("foobar") == "foobar");
    }
}

TEST_CASE("3. argument const", "[argument]")
{
    std::string const_value = "const";

    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("3.1. optional arguments") {
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo1").action("store").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo2").action("store_true").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo3").action("store_false").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo4").action("store_const").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo5").action("append").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo6").action("append_const").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo7").action("extend").const_value(const_value)));

        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo1?").action("store").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo2?").action("store_true").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo3?").action("store_false").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo4?").action("store_const").nargs("?").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo5?").action("append").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("--foo6?").action("append_const").nargs("?").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo7?").action("extend").nargs("?").const_value(const_value)));
    }

    SECTION("3.2. optional arguments") {
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo1").action("store").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo2").action("store_true").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo3").action("store_false").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("foo4").action("store_const").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo5").action("append").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("foo6").action("append_const").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo7").action("extend").const_value(const_value)));

        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("foo1?").action("store").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo2?").action("store_true").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo3?").action("store_false").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo4?").action("store_const").nargs("?").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("foo5?").action("append").nargs("?").const_value(const_value)));
        REQUIRE_THROWS(parser.add_argument(
                           argparse::Argument("foo6?").action("append_const").nargs("?").const_value(const_value)));
        REQUIRE_NOTHROW(parser.add_argument(
                            argparse::Argument("foo7?").action("extend").nargs("?").const_value(const_value)));
    }
}
