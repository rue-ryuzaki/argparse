/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument choices", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

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

TEST_CASE("2. argument dest", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    std::string dest_foo = "foo_dest";
    std::string dest_bar = "bar_dest";
    std::string dest_foobar = "foobar_dest";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

    SECTION("2.1. positional arguments") {
        CHECK_THROWS(parser.add_argument("foobar").dest(dest_foobar));
    }

    SECTION("2.2. optional arguments") {
        parser.add_argument("--foo").dest(dest_foo);
        parser.add_argument("--bar").dest(dest_bar).default_value(local_default);
        parser.add_argument("foobar");

        argparse::Namespace args1 = parser.parse_args(_make_vec("foobar"));
        CHECK_THROWS(args1.get<std::string>("--foo"));
        CHECK_THROWS(args1.get<std::string>("--bar"));
        CHECK(args1.get<std::string>("foobar") == "foobar");

        CHECK(args1.get<std::string>(dest_foo) == global_default);
        CHECK(args1.get<std::string>(dest_bar) == local_default);
        CHECK(args1.get<std::string>("foobar") == "foobar");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--foo=foo", "foobar"));
        CHECK(args2.get<std::string>(dest_foo) == "foo");
        CHECK(args2.get<std::string>(dest_bar) == local_default);
        CHECK(args2.get<std::string>("foobar") == "foobar");
    }
}

TEST_CASE("3. argument const", "[argument]")
{
    std::string const_value = "const";

    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("3.1. optional arguments") {
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo1").action("store").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo2").action("store_true").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo3").action("store_false").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo4").action("store_const").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo5").action("append").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo6").action("append_const").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo7").action("extend").const_value(const_value)));

        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo1?").action("store").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo2?").action("store_true").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo3?").action("store_false").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo4?").action("store_const").nargs("?").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo5?").action("append").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("--foo6?").action("append_const").nargs("?").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("--foo7?").action("extend").nargs("?").const_value(const_value)));
    }

    SECTION("3.2. optional arguments") {
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo1").action("store").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo2").action("store_true").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo3").action("store_false").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("foo4").action("store_const").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo5").action("append").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("foo6").action("append_const").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo7").action("extend").const_value(const_value)));

        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("foo1?").action("store").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo2?").action("store_true").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo3?").action("store_false").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo4?").action("store_const").nargs("?").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("foo5?").action("append").nargs("?").const_value(const_value)));
        CHECK_THROWS(parser.add_argument(
                           argparse::Argument("foo6?").action("append_const").nargs("?").const_value(const_value)));
        CHECK_NOTHROW(parser.add_argument(
                            argparse::Argument("foo7?").action("extend").nargs("?").const_value(const_value)));
    }
}
