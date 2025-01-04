/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

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
        CHECK(args.get<std::string>("-f") == global_default);
        CHECK(args.get<std::string>("-b") == local_default);
        CHECK(args.get<std::string>("--foo") == global_default);
        CHECK(args.get<std::string>("--bar") == local_default);
        CHECK(args.get<std::string>("f") == global_default);
        CHECK(args.get<std::string>("b") == local_default);
        CHECK(args.get<std::string>("foo") == global_default);
        CHECK(args.get<std::string>("bar") == local_default);
    }

    SECTION("1.2. one argument") {
        argparse::Namespace args1 = parser.parse_args(_make_vec("-f", foo));
        CHECK(args1.get<std::string>("-f") == foo);
        CHECK(args1.get<std::string>("-b") == local_default);
        CHECK(args1.get<std::string>("--foo") == foo);
        CHECK(args1.get<std::string>("--bar") == local_default);
        CHECK(args1.get<std::string>("f") == foo);
        CHECK(args1.get<std::string>("b") == local_default);
        CHECK(args1.get<std::string>("foo") == foo);
        CHECK(args1.get<std::string>("bar") == local_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec("--bar", bar));
        CHECK(args2.get<std::string>("-f") == global_default);
        CHECK(args2.get<std::string>("-b") == bar);
        CHECK(args2.get<std::string>("--foo") == global_default);
        CHECK(args2.get<std::string>("--bar") == bar);
        CHECK(args2.get<std::string>("f") == global_default);
        CHECK(args2.get<std::string>("b") == bar);
        CHECK(args2.get<std::string>("foo") == global_default);
        CHECK(args2.get<std::string>("bar") == bar);
    }

    SECTION("1.3. both arguments") {
        argparse::Namespace args = parser.parse_args(_make_vec("-f", foo, "--bar", bar));
        CHECK(args.get<std::string>("-f") == foo);
        CHECK(args.get<std::string>("-b") == bar);
        CHECK(args.get<std::string>("--foo") == foo);
        CHECK(args.get<std::string>("--bar") == bar);
        CHECK(args.get<std::string>("f") == foo);
        CHECK(args.get<std::string>("b") == bar);
        CHECK(args.get<std::string>("foo") == foo);
        CHECK(args.get<std::string>("bar") == bar);
    }

    SECTION("1.4. use equal '='") {
        argparse::Namespace args = parser.parse_args(_make_vec("-f=" + foo, "--bar=" + bar));
        CHECK(args.get<std::string>("-f") == foo);
        CHECK(args.get<std::string>("-b") == bar);
        CHECK(args.get<std::string>("--foo") == foo);
        CHECK(args.get<std::string>("--bar") == bar);
        CHECK(args.get<std::string>("f") == foo);
        CHECK(args.get<std::string>("b") == bar);
        CHECK(args.get<std::string>("foo") == foo);
        CHECK(args.get<std::string>("bar") == bar);
    }

    SECTION("1.5. arguments override") {
        argparse::Namespace args = parser.parse_args(_make_vec("-f", foo, "--bar", bar, "--foo", bar, "-b", foo));
        CHECK(args.get<std::string>("-f") == bar);
        CHECK(args.get<std::string>("-b") == foo);
        CHECK(args.get<std::string>("--foo") == bar);
        CHECK(args.get<std::string>("--bar") == foo);
        CHECK(args.get<std::string>("f") == bar);
        CHECK(args.get<std::string>("b") == foo);
        CHECK(args.get<std::string>("foo") == bar);
        CHECK(args.get<std::string>("bar") == foo);
    }

    SECTION("1.6. allow_abbrev=true") {
        parser.allow_abbrev(true);

        argparse::Namespace args = parser.parse_args(_make_vec("--f", foo, "--ba", bar));
        CHECK(args.get<std::string>("-f") == foo);
        CHECK(args.get<std::string>("-b") == bar);
        CHECK(args.get<std::string>("--foo") == foo);
        CHECK(args.get<std::string>("--bar") == bar);
        CHECK(args.get<std::string>("f") == foo);
        CHECK(args.get<std::string>("b") == bar);
        CHECK(args.get<std::string>("foo") == foo);
        CHECK(args.get<std::string>("bar") == bar);
    }

    SECTION("1.7. allow_abbrev=false") {
        parser.allow_abbrev(false);

        CHECK_THROWS(parser.parse_args(_make_vec("--", foo)));
        CHECK_THROWS(parser.parse_args(_make_vec("--f", foo)));
        CHECK_THROWS(parser.parse_args(_make_vec("--fo", foo)));
        CHECK_THROWS(parser.parse_args(_make_vec("--b", bar)));
        CHECK_THROWS(parser.parse_args(_make_vec("--ba", bar)));
    }

    SECTION("1.8. conflicting option string") {
        CHECK_THROWS(parser.add_argument("-f", "--foo"));
        CHECK_THROWS(parser.add_argument("--foo").dest("foo"));
    }

    SECTION("1.9. non-existent arguments") {
        argparse::ArgumentParser p = argparse::ArgumentParser().exit_on_error(false);
        p.add_argument(_make_vec("-f", "--foo"));
        p.add_argument(_make_vec("-b", "--bar"));

        argparse::Namespace args = p.parse_args(_make_vec("-b", bar));

        CHECK(args.get<std::string>("-f") == "");
        CHECK(args.get<std::string>("-b") == bar);
        CHECK(args.get<std::string>("--foo") == "");
        CHECK(args.get<std::string>("--bar") == bar);
        CHECK(args.get<std::string>("f") == "");
        CHECK(args.get<std::string>("b") == bar);
        CHECK(args.get<std::string>("foo") == "");
        CHECK(args.get<std::string>("bar") == bar);
        CHECK(args.exists("f") == false);
        CHECK(args.exists("-f") == false);
        CHECK(args.exists("foo") == false);
        CHECK(args.exists("--foo") == false);
        CHECK(args.exists("b") == true);
        CHECK(args.exists("-b") == true);
        CHECK(args.exists("bar") == true);
        CHECK(args.exists("--bar") == true);

        std::string non_exist = "baz";
        CHECK(args.exists(non_exist) == false);
        CHECK_THROWS(args.get<std::string>(non_exist));
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args.try_get<std::string>("foo").operator bool() == false);
        CHECK(args.try_get<std::string>("bar").operator bool() == true);
        CHECK(args.try_get<std::string>("bar").value() == bar);
        CHECK(args.try_get<std::vector<std::string> >(non_exist).operator bool() == false);
#endif  // ARGPARSE_HAS_OPTIONAL
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
        CHECK(args.get<std::string>("-x") == "-1");
        CHECK(args.get<std::string>("foo") == "");

        // so -1 and -5 are positional arguments
        argparse::Namespace args2 = parser.parse_args(_make_vec("-x", "-1", "-5"));
        CHECK(args2.get<std::string>("-x") == "-1");
        CHECK(args2.get<std::string>("foo") == "-5");
    }

    SECTION("2.2. negative number options present") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("-1").dest("one");
        parser.add_argument("foo").nargs("?");

        // so -1 is an option
        argparse::Namespace args = parser.parse_args(_make_vec("-1", "x"));
        CHECK(args.get<std::string>("one") == "x");
        CHECK(args.get<std::string>("foo") == "");

        // so -2 is an option
        CHECK_THROWS(parser.parse_args(_make_vec("-2")));
        // so both -1s are options
        CHECK_THROWS(parser.parse_args(_make_vec("-1", "-1")));
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
        CHECK_THROWS(parser.parse_args(_make_vec()));
    }

    SECTION("3.2. one argument") {
        CHECK_THROWS(parser.parse_args(_make_vec(foo)));
        CHECK_THROWS(parser.parse_args(_make_vec(bar)));
    }

    SECTION("3.3. both arguments") {
        argparse::Namespace args1 = parser.parse_args(_make_vec(foo, bar));
        CHECK(args1.get<std::string>("foo") == foo);
        CHECK(args1.get<std::string>("bar") == bar);

        argparse::Namespace args2 = parser.parse_args(_make_vec(bar, foo));
        CHECK(args2.get<std::string>("foo") == bar);
        CHECK(args2.get<std::string>("bar") == foo);
    }

    SECTION("3.4. excess arguments") {
        CHECK_THROWS(parser.parse_args(_make_vec(foo, bar, foo + bar)));
        CHECK_THROWS(parser.parse_args(_make_vec(foo, bar, foo, bar)));
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
        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec("-f", foo)));
        CHECK_THROWS(parser.parse_args(_make_vec("-b", bar, "-f", foo)));
    }

    SECTION("4.2. with positional arguments") {
        argparse::Namespace args1 = parser.parse_args(_make_vec(foo, bar));
        CHECK(args1.get<std::string>("-f") == global_default);
        CHECK(args1.get<std::string>("-b") == local_default);
        CHECK(args1.get<std::string>("foo") == foo);
        CHECK(args1.get<std::string>("bar") == bar);

        argparse::Namespace args2 = parser.parse_args(_make_vec(foo, "-f", foo, bar, "-b", bar));
        CHECK(args2.get<std::string>("-f") == foo);
        CHECK(args2.get<std::string>("-b") == bar);
        CHECK(args2.get<std::string>("foo") == foo);
        CHECK(args2.get<std::string>("bar") == bar);
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
        CHECK(args.get<std::string>("foo=") == global_default);
        CHECK(args.get<std::string>("bar=") == local_default);
        CHECK(args.get<std::string>("foo") == global_default);
        CHECK(args.get<std::string>("bar") == local_default);
    }

    SECTION("5.2. one argument") {
        argparse::Namespace args1 = parser.parse_args("foo=" + foo);
        CHECK(args1.get<std::string>("foo=") == foo);
        CHECK(args1.get<std::string>("bar=") == local_default);
        CHECK(args1.get<std::string>("foo") == foo);
        CHECK(args1.get<std::string>("bar") == local_default);

        argparse::Namespace args2 = parser.parse_args("bar=" + bar);
        CHECK(args2.get<std::string>("foo=") == global_default);
        CHECK(args2.get<std::string>("bar=") == bar);
        CHECK(args2.get<std::string>("foo") == global_default);
        CHECK(args2.get<std::string>("bar") == bar);
    }

    SECTION("5.3. both arguments") {
        argparse::Namespace args = parser.parse_args(_make_vec("foo=" + foo, "bar=" + bar));
        CHECK(args.get<std::string>("foo=") == foo);
        CHECK(args.get<std::string>("bar=") == bar);
        CHECK(args.get<std::string>("foo") == foo);
        CHECK(args.get<std::string>("bar") == bar);
    }
}
