/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse_decl.hpp>
#include "./catch-define.h"

TEST_CASE("1. optional arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument(_make_vec("-f", "--foo"));
    parser.add_argument(_make_vec("-b", "--bar")).default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("1.1. no arguments") {
        argparse::Namespace args = parser.parse_args(_make_vec());
        REQUIRE(args.get<std::string>("-f") == global_default);
        REQUIRE(args.get<std::string>("-b") == local_default);
        REQUIRE(args.get<std::string>("--foo") == global_default);
        REQUIRE(args.get<std::string>("--bar") == local_default);
        REQUIRE(args.get<std::string>("f") == global_default);
        REQUIRE(args.get<std::string>("b") == local_default);
        REQUIRE(args.get<std::string>("foo") == global_default);
        REQUIRE(args.get<std::string>("bar") == local_default);
    }

    SECTION("1.2. one argument") {
        argparse::Namespace args1 = parser.parse_args(_make_vec("-f", foo));
        REQUIRE(args1.get<std::string>("-f") == foo);
        REQUIRE(args1.get<std::string>("-b") == local_default);
        REQUIRE(args1.get<std::string>("--foo") == foo);
        REQUIRE(args1.get<std::string>("--bar") == local_default);
        REQUIRE(args1.get<std::string>("f") == foo);
        REQUIRE(args1.get<std::string>("b") == local_default);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == local_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec("--bar", bar));
        REQUIRE(args2.get<std::string>("-f") == global_default);
        REQUIRE(args2.get<std::string>("-b") == bar);
        REQUIRE(args2.get<std::string>("--foo") == global_default);
        REQUIRE(args2.get<std::string>("--bar") == bar);
        REQUIRE(args2.get<std::string>("f") == global_default);
        REQUIRE(args2.get<std::string>("b") == bar);
        REQUIRE(args2.get<std::string>("foo") == global_default);
        REQUIRE(args2.get<std::string>("bar") == bar);
    }

    SECTION("1.3. both arguments") {
        argparse::Namespace args = parser.parse_args(_make_vec("-f", foo, "--bar", bar));
        REQUIRE(args.get<std::string>("-f") == foo);
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == foo);
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == foo);
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }

    SECTION("1.4. use equal '='") {
        argparse::Namespace args = parser.parse_args(_make_vec("-f=" + foo, "--bar=" + bar));
        REQUIRE(args.get<std::string>("-f") == foo);
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == foo);
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == foo);
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }

    SECTION("1.5. arguments override") {
        argparse::Namespace args = parser.parse_args(_make_vec("-f", foo, "--bar", bar, "--foo", bar, "-b", foo));
        REQUIRE(args.get<std::string>("-f") == bar);
        REQUIRE(args.get<std::string>("-b") == foo);
        REQUIRE(args.get<std::string>("--foo") == bar);
        REQUIRE(args.get<std::string>("--bar") == foo);
        REQUIRE(args.get<std::string>("f") == bar);
        REQUIRE(args.get<std::string>("b") == foo);
        REQUIRE(args.get<std::string>("foo") == bar);
        REQUIRE(args.get<std::string>("bar") == foo);
    }

    SECTION("1.6. allow_abbrev=true") {
        parser.allow_abbrev(true);

        argparse::Namespace args = parser.parse_args(_make_vec("--f", foo, "--ba", bar));
        REQUIRE(args.get<std::string>("-f") == foo);
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == foo);
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == foo);
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }

    SECTION("1.7. allow_abbrev=false") {
        parser.allow_abbrev(false);

        REQUIRE_THROWS(parser.parse_args(_make_vec("--", foo)));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--f", foo)));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--fo", foo)));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--b", bar)));
        REQUIRE_THROWS(parser.parse_args(_make_vec("--ba", bar)));
    }

    SECTION("1.8. conflicting option string") {
        REQUIRE_THROWS(parser.add_argument(_make_vec("-f", "--foo")));
        REQUIRE_THROWS(parser.add_argument("--foo").dest("foo"));
    }

    SECTION("1.9. non-existent arguments") {
        argparse::ArgumentParser p = argparse::ArgumentParser().exit_on_error(false);
        p.add_argument(_make_vec("-f", "--foo"));
        p.add_argument(_make_vec("-b", "--bar"));

        argparse::Namespace args = p.parse_args(_make_vec("-b", bar));

        REQUIRE(args.get<std::string>("-f") == "");
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == "");
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == "");
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == "");
        REQUIRE(args.get<std::string>("bar") == bar);
        REQUIRE(args.exists("f") == false);
        REQUIRE(args.exists("-f") == false);
        REQUIRE(args.exists("foo") == false);
        REQUIRE(args.exists("--foo") == false);
        REQUIRE(args.exists("b") == true);
        REQUIRE(args.exists("-b") == true);
        REQUIRE(args.exists("bar") == true);
        REQUIRE(args.exists("--bar") == true);

        std::string non_exist = "baz";
        REQUIRE(args.exists(non_exist) == false);
        REQUIRE_THROWS(args.get<std::string>(non_exist));
#ifdef _ARGPARSE_CXX_17
        REQUIRE(args.try_get<std::string>("foo").operator bool() == false);
        REQUIRE(args.try_get<std::string>("bar").operator bool() == true);
        REQUIRE(args.try_get<std::string>("bar").value() == bar);
        REQUIRE(args.try_get<std::vector<std::string> >(non_exist).operator bool() == false);
#endif  // C++17+
    }
}

TEST_CASE("2. optional arguments containing -", "[argument_parser]")
{
    SECTION("2.1. no negative number options") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("-x");
        parser.add_argument("foo").nargs("?");

        // so -1 is a positional argument
        argparse::Namespace args = parser.parse_args(_make_vec("-x", "-1"));
        REQUIRE(args.get<std::string>("-x") == "-1");
        REQUIRE(args.get<std::string>("foo") == "");

        // so -1 and -5 are positional arguments
        argparse::Namespace args2 = parser.parse_args(_make_vec("-x", "-1", "-5"));
        REQUIRE(args2.get<std::string>("-x") == "-1");
        REQUIRE(args2.get<std::string>("foo") == "-5");
    }

    SECTION("2.2. negative number options present") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("-1").dest("one");
        parser.add_argument("foo").nargs("?");

        // so -1 is an option
        argparse::Namespace args = parser.parse_args(_make_vec("-1", "x"));
        REQUIRE(args.get<std::string>("one") == "x");
        REQUIRE(args.get<std::string>("foo") == "");

        // so -2 is an option
        REQUIRE_THROWS(parser.parse_args(_make_vec("-2")));
        // so both -1s are options
        REQUIRE_THROWS(parser.parse_args(_make_vec("-1", "-1")));
    }
}

TEST_CASE("3. positional arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument("foo");
    parser.add_argument("bar").default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("3.1. no arguments") {
        REQUIRE_THROWS(parser.parse_args(_make_vec()));
    }

    SECTION("3.2. one argument") {
        REQUIRE_THROWS(parser.parse_args(_make_vec(foo)));
        REQUIRE_THROWS(parser.parse_args(_make_vec(bar)));
    }

    SECTION("3.3. both arguments") {
        argparse::Namespace args1 = parser.parse_args(_make_vec(foo, bar));
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == bar);

        argparse::Namespace args2 = parser.parse_args(_make_vec(bar, foo));
        REQUIRE(args2.get<std::string>("foo") == bar);
        REQUIRE(args2.get<std::string>("bar") == foo);
    }

    SECTION("3.4. excess arguments") {
        REQUIRE_THROWS(parser.parse_args(_make_vec(foo, bar, foo + bar)));
        REQUIRE_THROWS(parser.parse_args(_make_vec(foo, bar, foo, bar)));
    }
}

TEST_CASE("4. optional and positional arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument("-f");
    parser.add_argument("-b").default_value(local_default);
    parser.add_argument("foo");
    parser.add_argument("bar").default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("4.1. no positional arguments") {
        REQUIRE_THROWS(parser.parse_args(_make_vec()));
        REQUIRE_THROWS(parser.parse_args(_make_vec("-f", foo)));
        REQUIRE_THROWS(parser.parse_args(_make_vec("-b", bar, "-f", foo)));
    }

    SECTION("4.2. with positional arguments") {
        argparse::Namespace args1 = parser.parse_args(_make_vec(foo, bar));
        REQUIRE(args1.get<std::string>("-f") == global_default);
        REQUIRE(args1.get<std::string>("-b") == local_default);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == bar);

        argparse::Namespace args2 = parser.parse_args(_make_vec(foo, "-f", foo, bar, "-b", bar));
        REQUIRE(args2.get<std::string>("-f") == foo);
        REQUIRE(args2.get<std::string>("-b") == bar);
        REQUIRE(args2.get<std::string>("foo") == foo);
        REQUIRE(args2.get<std::string>("bar") == bar);
    }
}

TEST_CASE("5. operand arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    argparse::ArgumentParser parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument("foo=").required(false);
    parser.add_argument("bar=").required(false).default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("5.1. no arguments") {
        argparse::Namespace args = parser.parse_args("");
        REQUIRE(args.get<std::string>("foo=") == global_default);
        REQUIRE(args.get<std::string>("bar=") == local_default);
        REQUIRE(args.get<std::string>("foo") == global_default);
        REQUIRE(args.get<std::string>("bar") == local_default);
    }

    SECTION("5.2. one argument") {
        argparse::Namespace args1 = parser.parse_args("foo=" + foo);
        REQUIRE(args1.get<std::string>("foo=") == foo);
        REQUIRE(args1.get<std::string>("bar=") == local_default);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == local_default);

        argparse::Namespace args2 = parser.parse_args("bar=" + bar);
        REQUIRE(args2.get<std::string>("foo=") == global_default);
        REQUIRE(args2.get<std::string>("bar=") == bar);
        REQUIRE(args2.get<std::string>("foo") == global_default);
        REQUIRE(args2.get<std::string>("bar") == bar);
    }

    SECTION("5.3. both arguments") {
        argparse::Namespace args = parser.parse_args(_make_vec("foo=" + foo, "bar=" + bar));
        REQUIRE(args.get<std::string>("foo=") == foo);
        REQUIRE(args.get<std::string>("bar=") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }
}
