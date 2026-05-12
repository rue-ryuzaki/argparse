/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument const", "[argument]")
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
