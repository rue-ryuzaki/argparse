#include <catch2/catch.hpp>

#include <argparse/argparse.hpp>

TEST_CASE("1. initization check", "[argument_parser]")
{
    auto parser = argparse::ArgumentParser();

    SECTION("1.1. default values") {
        REQUIRE(parser.prog() == "untitled");
        REQUIRE(parser.usage() == "");
        REQUIRE(parser.description() == "");
        REQUIRE(parser.epilog() == "");
        REQUIRE(parser.prefix_chars() == "-");
        REQUIRE(parser.fromfile_prefix_chars() == "");
        REQUIRE(parser.argument_default() == "");
        REQUIRE(parser.add_help() == true);
        REQUIRE(parser.allow_abbrev() == true);
        REQUIRE(parser.exit_on_error() == true);
    }

    SECTION("1.2. change values") {
        std::string prog = "prog";
        std::string usage = "prog usage";
        std::string description = "description";
        std::string epilog = "epilog";
        std::string prefix_chars = "-+/";
        std::string fromfile_prefix_chars = "@";
        std::string argument_default = "42";
        bool add_help = false;
        bool allow_abbrev = false;
        bool exit_on_error = false;

        parser.prog(prog)
                .usage(usage)
                .description(description)
                .epilog(epilog)
                .prefix_chars(prefix_chars)
                .fromfile_prefix_chars(fromfile_prefix_chars)
                .argument_default(argument_default)
                .add_help(add_help)
                .allow_abbrev(allow_abbrev)
                .exit_on_error(exit_on_error);

        REQUIRE(parser.prog() == prog);
        REQUIRE(parser.usage() == usage);
        REQUIRE(parser.description() == description);
        REQUIRE(parser.epilog() == epilog);
        REQUIRE(parser.prefix_chars() == prefix_chars);
        REQUIRE(parser.fromfile_prefix_chars() == fromfile_prefix_chars);
        REQUIRE(parser.argument_default() == argument_default);
        REQUIRE(parser.add_help() == add_help);
        REQUIRE(parser.allow_abbrev() == allow_abbrev);
        REQUIRE(parser.exit_on_error() == exit_on_error);

        // check properties, which can be cleared
        parser.usage("")
                .description("")
                .epilog("")
                .fromfile_prefix_chars("")
                .argument_default("");

        REQUIRE_FALSE(parser.usage() == usage);
        REQUIRE_FALSE(parser.description() == description);
        REQUIRE_FALSE(parser.epilog() == epilog);
        REQUIRE_FALSE(parser.fromfile_prefix_chars() == fromfile_prefix_chars);
        REQUIRE_FALSE(parser.argument_default() == argument_default);

        // check properties, which can't be cleared
        parser.prog("").prefix_chars("");

        REQUIRE(parser.prog() == prog);
        REQUIRE(parser.prefix_chars() == prefix_chars);
    }
}

TEST_CASE("2. optional arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument({ "-f", "--foo" });
    parser.add_argument({ "-b", "--bar" }).default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("2.1. no arguments") {
        auto args = parser.parse_args({ });
        REQUIRE(args.get<std::string>("-f") == global_default);
        REQUIRE(args.get<std::string>("-b") == local_default);
        REQUIRE(args.get<std::string>("--foo") == global_default);
        REQUIRE(args.get<std::string>("--bar") == local_default);
        REQUIRE(args.get<std::string>("f") == global_default);
        REQUIRE(args.get<std::string>("b") == local_default);
        REQUIRE(args.get<std::string>("foo") == global_default);
        REQUIRE(args.get<std::string>("bar") == local_default);
    }

    SECTION("2.2. one argument") {
        auto args1 = parser.parse_args({ "-f", foo });
        REQUIRE(args1.get<std::string>("-f") == foo);
        REQUIRE(args1.get<std::string>("-b") == local_default);
        REQUIRE(args1.get<std::string>("--foo") == foo);
        REQUIRE(args1.get<std::string>("--bar") == local_default);
        REQUIRE(args1.get<std::string>("f") == foo);
        REQUIRE(args1.get<std::string>("b") == local_default);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == local_default);

        auto args2 = parser.parse_args({ "--bar", bar });
        REQUIRE(args2.get<std::string>("-f") == global_default);
        REQUIRE(args2.get<std::string>("-b") == bar);
        REQUIRE(args2.get<std::string>("--foo") == global_default);
        REQUIRE(args2.get<std::string>("--bar") == bar);
        REQUIRE(args2.get<std::string>("f") == global_default);
        REQUIRE(args2.get<std::string>("b") == bar);
        REQUIRE(args2.get<std::string>("foo") == global_default);
        REQUIRE(args2.get<std::string>("bar") == bar);
    }

    SECTION("2.3. both arguments") {
        auto args = parser.parse_args({ "-f", foo, "--bar", bar });
        REQUIRE(args.get<std::string>("-f") == foo);
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == foo);
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == foo);
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }

    SECTION("2.4. use equal '='") {
        auto args = parser.parse_args({ "-f=" + foo, "--bar=" + bar });
        REQUIRE(args.get<std::string>("-f") == foo);
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == foo);
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == foo);
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }

    SECTION("2.5. arguments override") {
        auto args = parser.parse_args({ "-f", foo, "--bar", bar, "--foo", bar, "-b", foo });
        REQUIRE(args.get<std::string>("-f") == bar);
        REQUIRE(args.get<std::string>("-b") == foo);
        REQUIRE(args.get<std::string>("--foo") == bar);
        REQUIRE(args.get<std::string>("--bar") == foo);
        REQUIRE(args.get<std::string>("f") == bar);
        REQUIRE(args.get<std::string>("b") == foo);
        REQUIRE(args.get<std::string>("foo") == bar);
        REQUIRE(args.get<std::string>("bar") == foo);
    }

    SECTION("2.6. allow_abbrev=true") {
        parser.allow_abbrev(true);

        auto args = parser.parse_args({ "--f", foo, "--ba", bar });
        REQUIRE(args.get<std::string>("-f") == foo);
        REQUIRE(args.get<std::string>("-b") == bar);
        REQUIRE(args.get<std::string>("--foo") == foo);
        REQUIRE(args.get<std::string>("--bar") == bar);
        REQUIRE(args.get<std::string>("f") == foo);
        REQUIRE(args.get<std::string>("b") == bar);
        REQUIRE(args.get<std::string>("foo") == foo);
        REQUIRE(args.get<std::string>("bar") == bar);
    }

    SECTION("2.7. allow_abbrev=false") {
        parser.allow_abbrev(false);

        REQUIRE_THROWS(parser.parse_args({ "--", foo }));
        REQUIRE_THROWS(parser.parse_args({ "--f", foo }));
        REQUIRE_THROWS(parser.parse_args({ "--fo", foo }));
        REQUIRE_THROWS(parser.parse_args({ "--b", bar }));
        REQUIRE_THROWS(parser.parse_args({ "--ba", bar }));
    }

    SECTION("2.8. conflicting option string") {
        REQUIRE_THROWS(parser.add_argument({ "-f", "--foo" }));
        REQUIRE_THROWS(parser.add_argument("--foo").dest("foo"));
    }
}

TEST_CASE("3. optional arguments containing -", "[argument_parser]")
{
    SECTION("3.1. no negative number options") {
        auto parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("-x");
        parser.add_argument("foo").nargs("?");

        // so -1 is a positional argument
        auto args = parser.parse_args({ "-x", "-1" });
        REQUIRE(args.get<std::string>("-x") == "-1");
        REQUIRE(args.get<std::string>("foo") == "");

        // so -1 and -5 are positional arguments
        auto args2 = parser.parse_args({ "-x", "-1", "-5" });
        REQUIRE(args2.get<std::string>("-x") == "-1");
        REQUIRE(args2.get<std::string>("foo") == "-5");
    }

    SECTION("3.2. negative number options present") {
        auto parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("-1").dest("one");
        parser.add_argument("foo").nargs("?");

        // so -1 is an option
        auto args = parser.parse_args({ "-1", "x" });
        REQUIRE(args.get<std::string>("one") == "x");
        REQUIRE(args.get<std::string>("foo") == "");

        // so -2 is an option
        REQUIRE_THROWS(parser.parse_args({ "-2" }));
        // so both -1s are options
        REQUIRE_THROWS(parser.parse_args({ "-1", "-1" }));
    }
}

TEST_CASE("4. positional arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument("foo");
    parser.add_argument("bar").default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("4.1. no arguments") {
        REQUIRE_THROWS(parser.parse_args({ }));
    }

    SECTION("4.2. one argument") {
        REQUIRE_THROWS(parser.parse_args({ foo }));
        REQUIRE_THROWS(parser.parse_args({ bar }));
    }

    SECTION("4.3. both arguments") {
        auto args1 = parser.parse_args({ foo, bar });
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == bar);

        auto args2 = parser.parse_args({ bar, foo });
        REQUIRE(args2.get<std::string>("foo") == bar);
        REQUIRE(args2.get<std::string>("bar") == foo);
    }

    SECTION("4.4. excess arguments") {
        REQUIRE_THROWS(parser.parse_args({ foo, bar, foo + bar }));
        REQUIRE_THROWS(parser.parse_args({ foo, bar, foo, bar }));
    }
}

TEST_CASE("5. optional and positional arguments", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
    parser.add_argument("-f");
    parser.add_argument("-b").default_value(local_default);
    parser.add_argument("foo");
    parser.add_argument("bar").default_value(local_default);

    std::string foo = "foo";
    std::string bar = "bar";

    SECTION("5.1. no positional arguments") {
        REQUIRE_THROWS(parser.parse_args({ }));
        REQUIRE_THROWS(parser.parse_args({ "-f", foo }));
        REQUIRE_THROWS(parser.parse_args({ "-b", bar, "-f", foo }));
    }

    SECTION("5.2. with positional arguments") {
        auto args1 = parser.parse_args({ foo, bar });
        REQUIRE(args1.get<std::string>("-f") == global_default);
        REQUIRE(args1.get<std::string>("-b") == local_default);
        REQUIRE(args1.get<std::string>("foo") == foo);
        REQUIRE(args1.get<std::string>("bar") == bar);

        auto args2 = parser.parse_args({ foo, "-f", foo, bar, "-b", bar });
        REQUIRE(args2.get<std::string>("-f") == foo);
        REQUIRE(args2.get<std::string>("-b") == bar);
        REQUIRE(args2.get<std::string>("foo") == foo);
        REQUIRE(args2.get<std::string>("bar") == bar);
    }
}

TEST_CASE("6. argument choices", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

    SECTION("6.1. simple usage") {
        parser.add_argument("--foo").choices({ "foo1", "foo2", "foo3" });
        parser.add_argument("--bar").choices({ "bar1", "bar2", "bar3" }).default_value(local_default);
        parser.add_argument("foobar").choices({ "foobar1", "foobar2", "foobar3" });

        REQUIRE_THROWS(parser.parse_args({ "foo" }));
        REQUIRE_THROWS(parser.parse_args({ "foobar" }));
        REQUIRE_THROWS(parser.parse_args({ "foobar1", "--foo", "bar1" }));
        REQUIRE_THROWS(parser.parse_args({ "foobar2", "--foo", "bar1", "--bar=bar1" }));

        auto args1 = parser.parse_args({ "foobar1" });
        REQUIRE(args1.get<std::string>("--foo") == global_default);
        REQUIRE(args1.get<std::string>("--bar") == local_default);
        REQUIRE(args1.get<std::string>("foobar") == "foobar1");

        auto args2 = parser.parse_args({ "--foo=foo3", "foobar3" });
        REQUIRE(args2.get<std::string>("--foo") == "foo3");
        REQUIRE(args2.get<std::string>("--bar") == local_default);
        REQUIRE(args2.get<std::string>("foobar") == "foobar3");
    }

    SECTION("6.2. choices as string") {
        parser.add_argument("--foo").choices("FO");
        parser.add_argument("--bar").choices("BAR").default_value(local_default);

        REQUIRE_THROWS(parser.parse_args({ "--foo", "bar" }));
        REQUIRE_THROWS(parser.parse_args({  "--foo", "bar", "--bar=bar" }));

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--foo") == global_default);
        REQUIRE(args1.get<std::string>("--bar") == local_default);

        auto args2 = parser.parse_args({ "--foo=F", "--bar", "R" });
        REQUIRE(args2.get<std::string>("--foo") == "F");
        REQUIRE(args2.get<std::string>("--bar") == "R");
    }

    SECTION("6.3. empty choices") {
        parser.add_argument("--foo").choices("");
        parser.add_argument("--bar").choices("BAR").default_value(local_default);

        REQUIRE_THROWS(parser.parse_args({ "--foo", "bar" }));
        REQUIRE_THROWS(parser.parse_args({ "--foo", "bar", "--bar=bar" }));

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--foo") == global_default);
        REQUIRE(args1.get<std::string>("--bar") == local_default);

        REQUIRE_THROWS(parser.parse_args({ "--foo=F", "--bar", "R" }));

        auto args2 = parser.parse_args({ "--bar", "R" });
        REQUIRE(args2.get<std::string>("--foo") == global_default);
        REQUIRE(args2.get<std::string>("--bar") == "R");

        auto args3 = parser.parse_args({ "--foo=", "--bar", "R" });
        REQUIRE(args3.get<std::string>("--foo") == "");
        REQUIRE(args3.get<std::string>("--bar") == "R");

        auto args4 = parser.parse_args({ "--foo=''", "--bar", "R" });
        REQUIRE(args4.get<std::string>("--foo") == "");
        REQUIRE(args4.get<std::string>("--bar") == "R");

        auto args5 = parser.parse_args({ "--foo", "", "--bar", "R" });
        REQUIRE(args5.get<std::string>("--foo") == "");
        REQUIRE(args5.get<std::string>("--bar") == "R");
    }
}

TEST_CASE("7. argument dest", "[argument]")
{
    std::string global_default = "global";
    std::string local_default = "local";

    std::string dest_foo = "foo_dest";
    std::string dest_bar = "bar_dest";
    std::string dest_foobar = "foobar_dest";

    auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

    SECTION("7.1. positional arguments") {
        REQUIRE_THROWS(parser.add_argument("foobar").dest(dest_foobar));
    }

    SECTION("7.2. optional arguments") {
        parser.add_argument("--foo").dest(dest_foo);
        parser.add_argument("--bar").dest(dest_bar).default_value(local_default);
        parser.add_argument("foobar");

        auto args1 = parser.parse_args({ "foobar" });
        REQUIRE_THROWS(args1.get<std::string>("--foo"));
        REQUIRE_THROWS(args1.get<std::string>("--bar"));
        REQUIRE(args1.get<std::string>("foobar") == "foobar");

        REQUIRE(args1.get<std::string>(dest_foo) == global_default);
        REQUIRE(args1.get<std::string>(dest_bar) == local_default);
        REQUIRE(args1.get<std::string>("foobar") == "foobar");

        auto args2 = parser.parse_args({ "--foo=foo", "foobar" });
        REQUIRE(args2.get<std::string>(dest_foo) == "foo");
        REQUIRE(args2.get<std::string>(dest_bar) == local_default);
        REQUIRE(args2.get<std::string>("foobar") == "foobar");
    }
}

TEST_CASE("8. argument actions", "[argument]")
{
    // TODO : with argument_default (store_true, store_false, count) actions are invalid in python
    std::string const_value = "const";
    std::string new_value = "new";

    auto parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("8.1. optional arguments") {
        parser.add_argument("--store").action(argparse::store);
        parser.add_argument("--store_const").action(argparse::store_const).const_value(const_value);
        parser.add_argument("--store_true").action(argparse::store_true);
        parser.add_argument("--store_false").action(argparse::store_false);
        parser.add_argument("--append").action(argparse::append);
        parser.add_argument("--append_const").action(argparse::append_const).const_value(const_value);
        parser.add_argument("--count").action(argparse::count);
        parser.add_argument("--extend").action(argparse::extend);

        // no args
        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--store") == "");
        REQUIRE(args1.get<std::string>("--store_const") == "");
        REQUIRE(args1.get<bool>("--store_true") == false);
        REQUIRE(args1.get<bool>("--store_false") == true);
        REQUIRE(args1.get<std::string>("--append") == "");
        REQUIRE(args1.get<std::string>("--append_const") == "");
        REQUIRE(args1.get<size_t>("--count") == 0);
        REQUIRE(args1.get<std::string>("--extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("--store").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--append_const").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        auto args2 = parser.parse_args({ "--store", new_value, "--store_const", "--store_true", "--store_false", "--append", new_value, "--append_const", "--count", "--extend", new_value });
        REQUIRE(args2.get<std::string>("--store") == new_value);
        REQUIRE(args2.get<std::string>("--store_const") == const_value);
        REQUIRE(args2.get<bool>("--store_true") == true);
        REQUIRE(args2.get<bool>("--store_false") == false);
        REQUIRE(args2.get<std::string>("--append") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::string>("--append_const") == const_value); // TODO : return array value
        REQUIRE(args2.get<size_t>("--count") == 1);
        REQUIRE(args2.get<std::string>("--extend") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--append").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--append_const").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--extend").size() == 1);
    }

    SECTION("8.2. positional arguments") {
        parser.add_argument("store").action(argparse::store);
        parser.add_argument("store_const").action(argparse::store_const).const_value(const_value);
        parser.add_argument("store_true").action(argparse::store_true);
        parser.add_argument("store_false").action(argparse::store_false);
        parser.add_argument("append").action(argparse::append);
        parser.add_argument("append_const").action(argparse::append_const).const_value(const_value);
        parser.add_argument("count").action(argparse::count);
        parser.add_argument("extend").action(argparse::extend);

        REQUIRE_THROWS(parser.parse_args({ }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value }));

        auto args = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args.get<std::string>("store") == new_value);
        REQUIRE(args.get<std::string>("store_const") == const_value);
        REQUIRE(args.get<bool>("store_true") == true);
        REQUIRE(args.get<bool>("store_false") == false);
        REQUIRE(args.get<std::string>("append") == new_value); // TODO : return array value
        REQUIRE(args.get<std::string>("append_const") == const_value); // TODO : return array value
        REQUIRE(args.get<size_t>("count") == 1);
        REQUIRE(args.get<std::string>("extend") == new_value); // TODO : return array value
        REQUIRE(args.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args.get<std::vector<std::string> >("append").size() == 1);
        REQUIRE(args.get<std::vector<std::string> >("append_const").size() == 1);
        REQUIRE(args.get<std::vector<std::string> >("extend").size() == 1);
    }
}

TEST_CASE("9. argument nargs", "[argument]")
{
    // TODO : with argument_default (store_true, store_false, count) actions are invalid in python
    std::string default_value = "local";
    std::string const_value = "const";
    std::string new_value = "new";

    auto parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("9.1. nargs break actions") {
        REQUIRE_THROWS(parser.add_argument("--store_const?").action(argparse::store_const).const_value(const_value).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("--store_const*").action(argparse::store_const).const_value(const_value).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("--store_const+").action(argparse::store_const).const_value(const_value).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("--store_constN").action(argparse::store_const).const_value(const_value).nargs(2));
        REQUIRE_THROWS(parser.add_argument("--store_true?").action(argparse::store_true).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("--store_true*").action(argparse::store_true).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("--store_true+").action(argparse::store_true).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("--store_trueN").action(argparse::store_true).nargs(2));
        REQUIRE_THROWS(parser.add_argument("--store_false?").action(argparse::store_false).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("--store_false*").action(argparse::store_false).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("--store_false+").action(argparse::store_false).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("--store_falseN").action(argparse::store_false).nargs(2));
        REQUIRE_THROWS(parser.add_argument("--append_const?").action(argparse::append_const).const_value(const_value).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("--append_const*").action(argparse::append_const).const_value(const_value).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("--append_const+").action(argparse::append_const).const_value(const_value).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("--append_constN").action(argparse::append_const).const_value(const_value).nargs(2));
        REQUIRE_THROWS(parser.add_argument("--count?").action(argparse::count).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("--count*").action(argparse::count).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("--count+").action(argparse::count).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("--countN").action(argparse::count).nargs(2));

        REQUIRE_THROWS(parser.add_argument("store_const?").action(argparse::store_const).const_value(const_value).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("store_const*").action(argparse::store_const).const_value(const_value).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("store_const+").action(argparse::store_const).const_value(const_value).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("store_constN").action(argparse::store_const).const_value(const_value).nargs(2));
        REQUIRE_THROWS(parser.add_argument("store_true?").action(argparse::store_true).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("store_true*").action(argparse::store_true).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("store_true+").action(argparse::store_true).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("store_trueN").action(argparse::store_true).nargs(2));
        REQUIRE_THROWS(parser.add_argument("store_false?").action(argparse::store_false).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("store_false*").action(argparse::store_false).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("store_false+").action(argparse::store_false).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("store_falseN").action(argparse::store_false).nargs(2));
        REQUIRE_THROWS(parser.add_argument("append_const?").action(argparse::append_const).const_value(const_value).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("append_const*").action(argparse::append_const).const_value(const_value).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("append_const+").action(argparse::append_const).const_value(const_value).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("append_constN").action(argparse::append_const).const_value(const_value).nargs(2));
        REQUIRE_THROWS(parser.add_argument("count?").action(argparse::count).nargs("?"));
        REQUIRE_THROWS(parser.add_argument("count*").action(argparse::count).nargs("*"));
        REQUIRE_THROWS(parser.add_argument("count+").action(argparse::count).nargs("+"));
        REQUIRE_THROWS(parser.add_argument("countN").action(argparse::count).nargs(2));
    }

    SECTION("9.2. nargs ?") {
        parser.add_argument("--foo").nargs("?").const_value("c").default_value("d");
        parser.add_argument("bar").nargs("?").default_value("d");
        parser.add_argument("--store");

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("foo") == "d");
        REQUIRE(args1.get<std::string>("bar") == "d");

        auto args2 = parser.parse_args({ "XX", "--foo" });
        REQUIRE(args2.get<std::string>("foo") == "c");
        REQUIRE(args2.get<std::string>("bar") == "XX");

        REQUIRE_THROWS(parser.parse_args({ "XX", "--foo", "--store" }));

        auto args3 = parser.parse_args({ "XX", "--foo", "--store", "store" });
        REQUIRE(args3.get<std::string>("foo") == "c");
        REQUIRE(args3.get<std::string>("bar") == "XX");

        REQUIRE_THROWS(parser.parse_args({ "XX", "YY" }));

        auto args4 = parser.parse_args({ "XX", "--foo", "YY"});
        REQUIRE(args4.get<std::string>("foo") == "YY");
        REQUIRE(args4.get<std::string>("bar") == "XX");
    }

    SECTION("9.3. nargs ? optional") {
        parser.add_argument("--store").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("--append").action(argparse::append).nargs("?"); // TODO: default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs("?"); // TODO: default value are invalid in python if flag used

        // no args
        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--store") == default_value);
        REQUIRE(args1.get<std::string>("--append") == "");
        REQUIRE(args1.get<std::string>("--extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("--append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--extend").size() == 0);

        REQUIRE_THROWS(parser.parse_args({ "--store", new_value, new_value }));

        // all args
        auto args2 = parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value });
        REQUIRE(args2.get<std::string>("--store") == new_value);
        REQUIRE(args2.get<std::string>("--append") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::string>("--extend") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--append").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--extend").size() == 1);

        // override args
        auto args3 = parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value,
                                         "--store", new_value, "--append", new_value, "--extend", new_value });
        REQUIRE(args3.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("--append").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("--extend").size() == 2);
    }

    SECTION("9.4. nargs ? positional") {
        parser.add_argument("store").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("append").action(argparse::append).nargs("?");
//        parser.add_argument({ "extend" }).action(argparse::extend).nargs("?"); // TODO : invalid in python without arguments

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("store") == default_value);
        REQUIRE(args1.get<std::string>("append") == ""); // TODO : return array value
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("append").size() == 0);

        auto args2 = parser.parse_args({ new_value });
        REQUIRE(args2.get<std::string>("store") == new_value);
        REQUIRE(args2.get<std::string>("append") == ""); // TODO : return array value
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("append").size() == 0);

        auto args3 = parser.parse_args({ new_value, new_value });
        REQUIRE(args3.get<std::string>("store") == new_value);
        REQUIRE(args3.get<std::string>("append") == new_value); // TODO : return array value
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("append").size() == 1);

        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value }));
    }

    SECTION("9.5. nargs * optional") {
        parser.add_argument("--store").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("--append").action(argparse::append).nargs("*"); // TODO: default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs("*"); // TODO: default value are invalid in python if flag used

        // no args
        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--store") == default_value);
        REQUIRE(args1.get<std::string>("--append") == "");
        REQUIRE(args1.get<std::string>("--extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("--append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        auto args2 = parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value });
        REQUIRE(args2.get<std::string>("--store") == new_value);
        REQUIRE(args2.get<std::string>("--append") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::string>("--extend") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--append").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--extend").size() == 1);

        // override args
        auto args3 = parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value,
                                         "--store", new_value, new_value, "--append", new_value, new_value, "--extend", new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("--store").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("--append").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("--extend").size() == 3);
    }

    SECTION("9.6. nargs * positional") {
        parser.add_argument("store").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("append").action(argparse::append).nargs("*"); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("*"); // TODO: default value are invalid in python if flag used

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("store") == default_value);
        REQUIRE(args1.get<std::string>("append") == "");
        REQUIRE(args1.get<std::string>("extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("extend").size() == 0);

        auto args2 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 4);
        REQUIRE(args2.get<std::vector<std::string> >("append").size() == 0);
        REQUIRE(args2.get<std::vector<std::string> >("extend").size() == 0);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 5);
        REQUIRE(args3.get<std::vector<std::string> >("append").size() == 0);
        REQUIRE(args3.get<std::vector<std::string> >("extend").size() == 0);
    }

    SECTION("9.7. nargs * positional [2]") {
        parser.add_argument("append").action(argparse::append).nargs("*"); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("*"); // TODO: default value are invalid in python if flag used
        parser.add_argument("store").action(argparse::store).nargs("*").default_value(default_value);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("store") == default_value);
        REQUIRE(args1.get<std::string>("append") == "");
        REQUIRE(args1.get<std::string>("extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("extend").size() == 0);

        auto args2 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::string>("store") == default_value);
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("append").size() == 4);
        REQUIRE(args2.get<std::vector<std::string> >("extend").size() == 0);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::string>("store") == default_value);
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("append").size() == 5);
        REQUIRE(args3.get<std::vector<std::string> >("extend").size() == 0);
    }

    SECTION("9.8. nargs + optional") {
        parser.add_argument("--store").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("--append").action(argparse::append).nargs("+"); // TODO: default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs("+"); // TODO: default value are invalid in python if flag used

        // no args
        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--store") == default_value);
        REQUIRE(args1.get<std::string>("--append") == "");
        REQUIRE(args1.get<std::string>("--extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("--append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        auto args2 = parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value });
        REQUIRE(args2.get<std::string>("--store") == new_value);
        REQUIRE(args2.get<std::string>("--append") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::string>("--extend") == new_value); // TODO : return array value
        REQUIRE(args2.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--append").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--extend").size() == 1);

        // override args
        auto args3 = parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value,
                                         "--store", new_value, new_value, "--append", new_value, new_value, "--extend", new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("--store").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("--append").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("--extend").size() == 3);
    }

    SECTION("9.9. nargs + positional") {
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("append").action(argparse::append).nargs("+"); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("+"); // TODO: default value are invalid in python if flag used

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value }));

        auto args1 = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args1.get<std::string>("store") == new_value);
        REQUIRE(args1.get<std::string>("append") == new_value);
        REQUIRE(args1.get<std::string>("extend") == new_value);
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("append").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("extend").size() == 1);

        auto args2 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("append").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("extend").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("append").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("extend").size() == 1);
    }

    SECTION("9.10. nargs + positional [2]") {
        parser.add_argument("append").action(argparse::append).nargs("+"); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("+"); // TODO: default value are invalid in python if flag used
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value }));

        auto args1 = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args1.get<std::string>("append") == new_value);
        REQUIRE(args1.get<std::string>("extend") == new_value);
        REQUIRE(args1.get<std::string>("store") == new_value);
        REQUIRE(args1.get<std::vector<std::string> >("append").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("extend").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);

        auto args2 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("append").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("extend").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("append").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("extend").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 1);
    }

    SECTION("9.11. nargs mixed positional") {
        parser.add_argument("append").action(argparse::append).nargs("+"); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("*"); // TODO: default value are invalid in python if flag used
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));

        auto args1 = parser.parse_args({ new_value, new_value });
        REQUIRE(args1.get<std::string>("append") == new_value);
        REQUIRE(args1.get<std::string>("extend") == "");
        REQUIRE(args1.get<std::string>("store") == new_value);
        REQUIRE(args1.get<std::vector<std::string> >("append").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("extend").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);

        auto args2 = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("append").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("extend").size() == 0);
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("append").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("extend").size() == 0);
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 1);
    }

    SECTION("9.12. nargs mixed positional [2]") {
        parser.add_argument("append1").action(argparse::append).nargs("+"); // TODO: default value are invalid in python if flag used
        parser.add_argument("append2").action(argparse::append).nargs("?"); // TODO: default value are invalid in python if flag used
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));

        auto args1 = parser.parse_args({ new_value, new_value });
        REQUIRE(args1.get<std::string>("append1") == new_value);
        REQUIRE(args1.get<std::string>("append2") == "");
        REQUIRE(args1.get<std::string>("store") == new_value);
        REQUIRE(args1.get<std::vector<std::string> >("append1").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("append2").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 1);

        auto args2 = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("append1").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("append2").size() == 0);
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("append1").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("append2").size() == 0);
        REQUIRE(args3.get<std::vector<std::string> >("store").size() == 1);
    }

    SECTION("9.13. nargs mixed positional [3]") {
        parser.add_argument("store1").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("+").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));

        auto args1 = parser.parse_args({ new_value, new_value });
        REQUIRE(args1.get<std::string>("store1") == default_value);
        REQUIRE(args1.get<std::string>("store2") == default_value);
        REQUIRE(args1.get<std::string>("store3") == new_value);
        REQUIRE(args1.get<std::string>("store4") == default_value);
        REQUIRE(args1.get<std::string>("store5") == default_value);
        REQUIRE(args1.get<std::string>("store6") == new_value);

        auto args2 = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args2.get<std::string>("store1") == new_value);
        REQUIRE(args2.get<std::string>("store2") == default_value);
        REQUIRE(args2.get<std::string>("store3") == new_value);
        REQUIRE(args2.get<std::string>("store4") == default_value);
        REQUIRE(args2.get<std::string>("store5") == default_value);
        REQUIRE(args2.get<std::string>("store6") == new_value);
        REQUIRE(args2.get<std::vector<std::string> >("store1").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store3").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store6").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::string>("store2") == default_value);
        REQUIRE(args3.get<std::string>("store3") == new_value);
        REQUIRE(args3.get<std::string>("store4") == default_value);
        REQUIRE(args3.get<std::string>("store5") == default_value);
        REQUIRE(args3.get<std::string>("store6") == new_value);
        REQUIRE(args3.get<std::vector<std::string> >("store1").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("store3").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("store6").size() == 1);
    }

    SECTION("9.14. nargs mixed positional [4]") {
        parser.add_argument("store1").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("*").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));

        auto args1 = parser.parse_args({ new_value, new_value });
        REQUIRE(args1.get<std::string>("store1") == new_value);
        REQUIRE(args1.get<std::string>("store2") == default_value);
        REQUIRE(args1.get<std::string>("store3") == default_value);
        REQUIRE(args1.get<std::string>("store4") == new_value);
        REQUIRE(args1.get<std::string>("store5") == default_value);
        REQUIRE(args1.get<std::string>("store6") == default_value);

        auto args2 = parser.parse_args({ new_value, new_value, new_value });
        REQUIRE(args2.get<std::string>("store2") == default_value);
        REQUIRE(args2.get<std::string>("store3") == default_value);
        REQUIRE(args2.get<std::string>("store4") == new_value);
        REQUIRE(args2.get<std::string>("store5") == default_value);
        REQUIRE(args2.get<std::string>("store6") == default_value);
        REQUIRE(args2.get<std::vector<std::string> >("store1").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("store4").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::string>("store2") == default_value);
        REQUIRE(args3.get<std::string>("store3") == default_value);
        REQUIRE(args3.get<std::string>("store4") == new_value);
        REQUIRE(args3.get<std::string>("store5") == default_value);
        REQUIRE(args3.get<std::string>("store6") == default_value);
        REQUIRE(args3.get<std::vector<std::string> >("store1").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("store4").size() == 1);
    }

    SECTION("9.15. nargs mixed positional [5]") {
        parser.add_argument("store1").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("*").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value, new_value }));

        auto args1 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args1.get<std::string>("store1") == new_value);
        REQUIRE(args1.get<std::string>("store3") == default_value);
        REQUIRE(args1.get<std::string>("store4") == new_value);
        REQUIRE(args1.get<std::string>("store6") == default_value);
        REQUIRE(args1.get<std::vector<std::string> >("store1").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("store2").size() == 2);
        REQUIRE(args1.get<std::vector<std::string> >("store4").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("store5").size() == 2);

        auto args2 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::string>("store3") == default_value);
        REQUIRE(args2.get<std::string>("store4") == new_value);
        REQUIRE(args2.get<std::string>("store6") == default_value);
        REQUIRE(args2.get<std::vector<std::string> >("store1").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("store2").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("store4").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store5").size() == 2);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::string>("store3") == default_value);
        REQUIRE(args3.get<std::string>("store4") == new_value);
        REQUIRE(args3.get<std::string>("store6") == default_value);
        REQUIRE(args3.get<std::vector<std::string> >("store1").size() == 3);
        REQUIRE(args3.get<std::vector<std::string> >("store2").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("store4").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("store5").size() == 2);
    }

    SECTION("9.16. nargs mixed positional [6]") {
        parser.add_argument("store1").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("+").default_value(default_value);

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value, new_value }));

        auto args1 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args1.get<std::string>("store1") == default_value);
        REQUIRE(args1.get<std::string>("store3") == new_value);
        REQUIRE(args1.get<std::string>("store4") == default_value);
        REQUIRE(args1.get<std::string>("store6") == new_value);
        REQUIRE(args1.get<std::vector<std::string> >("store2").size() == 2);
        REQUIRE(args1.get<std::vector<std::string> >("store3").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("store5").size() == 2);
        REQUIRE(args1.get<std::vector<std::string> >("store6").size() == 1);

        auto args2 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::string>("store1") == new_value);
        REQUIRE(args2.get<std::string>("store3") == new_value);
        REQUIRE(args2.get<std::string>("store4") == default_value);
        REQUIRE(args2.get<std::string>("store6") == new_value);
        REQUIRE(args2.get<std::vector<std::string> >("store1").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store2").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("store3").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store5").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("store6").size() == 1);

        auto args3 = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args3.get<std::string>("store3") == new_value);
        REQUIRE(args3.get<std::string>("store4") == default_value);
        REQUIRE(args3.get<std::string>("store6") == new_value);
        REQUIRE(args3.get<std::vector<std::string> >("store1").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("store2").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("store3").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("store5").size() == 2);
        REQUIRE(args3.get<std::vector<std::string> >("store6").size() == 1);
    }

    SECTION("9.17. nargs N optional") {
        parser.add_argument("--store").action(argparse::store).nargs(1).default_value(default_value);
        parser.add_argument("--append").action(argparse::append).nargs(2); // TODO: default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs(3); // TODO: default value are invalid in python if flag used

        // no args
        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("--store") == default_value);
        REQUIRE(args1.get<std::string>("--append") == "");
        REQUIRE(args1.get<std::string>("--extend") == "");
        REQUIRE(args1.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("--append").size() == 0);
        REQUIRE(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        auto args2 = parser.parse_args({ "--store", new_value, "--append", new_value, new_value, "--extend", new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("--append").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("--extend").size() == 3);

        REQUIRE_THROWS(parser.parse_args({ "--store", new_value, "--append", new_value, "--extend", new_value }));

        // override args
        auto args3 = parser.parse_args({ "--store", new_value, "--append", new_value, new_value, "--extend", new_value, new_value, new_value,
                                         "--store", new_value, "--append", new_value, new_value, "--extend", new_value, new_value, new_value });
        REQUIRE(args3.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args3.get<std::vector<std::string> >("--append").size() == 4);
        REQUIRE(args3.get<std::vector<std::string> >("--extend").size() == 6);
    }

    SECTION("9.18. nargs N positional") {
        parser.add_argument("store").action(argparse::store).nargs(1).default_value(default_value);
        parser.add_argument("append").action(argparse::append).nargs(2); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs(3); // TODO: default value are invalid in python if flag used

        REQUIRE_THROWS(parser.parse_args({  }));
        REQUIRE_THROWS(parser.parse_args({ new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value }));
        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value, new_value }));

        auto args = parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args.get<std::vector<std::string> >("store").size() == 1);
        REQUIRE(args.get<std::vector<std::string> >("append").size() == 2);
        REQUIRE(args.get<std::vector<std::string> >("extend").size() == 3);

        REQUIRE_THROWS(parser.parse_args({ new_value, new_value, new_value, new_value, new_value, new_value, new_value }));
    }

    SECTION("9.19. nargs N optional + positional") {
        parser.add_argument("--store").action(argparse::store).nargs(1).default_value(default_value);
        parser.add_argument("store").action(argparse::store).nargs(2); // TODO: default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs(2); // TODO: default value are invalid in python if flag used

        REQUIRE_THROWS(parser.parse_args({ new_value, "--store", new_value, new_value, new_value, new_value, new_value, new_value }));

        // no args
        auto args1 = parser.parse_args({ new_value, new_value, new_value, new_value });
        REQUIRE(args1.get<std::string>("--store") == default_value);
        REQUIRE(args1.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args1.get<std::vector<std::string> >("store").size() == 2);
        REQUIRE(args1.get<std::vector<std::string> >("extend").size() == 2);

        // all args
        auto args2 = parser.parse_args({ "--store", new_value, new_value, new_value, new_value, new_value });
        REQUIRE(args2.get<std::vector<std::string> >("--store").size() == 1);
        REQUIRE(args2.get<std::vector<std::string> >("store").size() == 2);
        REQUIRE(args2.get<std::vector<std::string> >("extend").size() == 2);
    }
}

TEST_CASE("10. abbreviations", "[argument]")
{
    auto parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("10.1. simple count test") {
        parser.add_argument("-c").action(argparse::count);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<uint32_t>("-c") == 0);

        auto args2 = parser.parse_args({ "-c" });
        REQUIRE(args2.get<uint32_t>("-c") == 1);

        auto args3 = parser.parse_args({ "-cc" });
        REQUIRE(args3.get<uint32_t>("-c") == 2);

        auto args4 = parser.parse_args({ "-ccc" });
        REQUIRE(args4.get<uint32_t>("-c") == 3);

        auto args5 = parser.parse_args({ "-ccc", "-cc" });
        REQUIRE(args5.get<uint32_t>("-c") == 5);
    }

    SECTION("10.2. multiargument test") {
        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-d").action(argparse::count);
        parser.add_argument("-e").action(argparse::count);
        parser.add_argument("-f").action(argparse::store_true);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<uint32_t>("-d") == 0);
        REQUIRE(args1.get<uint32_t>("-e") == 0);
        REQUIRE(args1.get<bool>("-f") == false);

        auto args2 = parser.parse_args({ "-c" });
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<uint32_t>("-d") == 0);
        REQUIRE(args2.get<uint32_t>("-e") == 0);
        REQUIRE(args2.get<bool>("-f") == false);

        auto args3 = parser.parse_args({ "-cddec" });
        REQUIRE(args3.get<uint32_t>("-c") == 2);
        REQUIRE(args3.get<uint32_t>("-d") == 2);
        REQUIRE(args3.get<uint32_t>("-e") == 1);
        REQUIRE(args3.get<bool>("-f") == false);

        auto args4 = parser.parse_args({ "-cccee" });
        REQUIRE(args4.get<uint32_t>("-c") == 3);
        REQUIRE(args4.get<uint32_t>("-d") == 0);
        REQUIRE(args4.get<uint32_t>("-e") == 2);
        REQUIRE(args4.get<bool>("-f") == false);

        auto args5 = parser.parse_args({ "-cfcce", "-ddcc" });
        REQUIRE(args5.get<uint32_t>("-c") == 5);
        REQUIRE(args5.get<uint32_t>("-d") == 2);
        REQUIRE(args5.get<uint32_t>("-e") == 1);
        REQUIRE(args5.get<bool>("-f") == true);
    }

    SECTION("10.3. multiargument store test") {
        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-d").action(argparse::store);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<std::string>("-d") == "");

        auto args2 = parser.parse_args({ "-c" });
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<std::string>("-d") == "");

        auto args3 = parser.parse_args({ "-cddec" });
        REQUIRE(args3.get<uint32_t>("-c") == 1);
        REQUIRE(args3.get<std::string>("-d") == "dec");

        auto args4 = parser.parse_args({ "-cccdd" });
        REQUIRE(args4.get<uint32_t>("-c") == 3);
        REQUIRE(args4.get<std::string>("-d") == "d");

        REQUIRE_THROWS(parser.parse_args({ "-cccd" }));
    }

    SECTION("10.4. same prefix store test") {
        parser.add_argument("-f").action(argparse::store);
        parser.add_argument("-foo").action(argparse::store);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("-f") == "");
        REQUIRE(args1.get<std::string>("-foo") == "");

        REQUIRE_THROWS(parser.parse_args({ "-f" }));
        REQUIRE_THROWS(parser.parse_args({ "-foo" }));

        auto args4 = parser.parse_args({ "-foo1" });
        REQUIRE(args4.get<std::string>("-f") == "oo1");
        REQUIRE(args4.get<std::string>("-foo") == "");

        auto args5 = parser.parse_args({ "-foo=1" });
        REQUIRE(args5.get<std::string>("-f") == "");
        REQUIRE(args5.get<std::string>("-foo") == "1");
    }

    SECTION("10.5. same name test (allow_abbrev=true)") {
        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-ccc").action(argparse::store_true);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<bool>("-ccc") == false);

        auto args2 = parser.parse_args({ "-c" });
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<bool>("-ccc") == false);

        REQUIRE_THROWS(parser.parse_args({ "-cc" }));

        auto args4 = parser.parse_args({ "-ccc" });
        REQUIRE(args4.get<uint32_t>("-c") == 0);
        REQUIRE(args4.get<bool>("-ccc") == true);

        auto args5 = parser.parse_args({ "-cccc" });
        REQUIRE(args5.get<uint32_t>("-c") == 4);
        REQUIRE(args5.get<bool>("-ccc") == false);
    }

    SECTION("10.6. same name test (allow_abbrev=false)") {
        parser.allow_abbrev(false);

        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-ccc").action(argparse::store_true);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<bool>("-ccc") == false);

        auto args2 = parser.parse_args({ "-c" });
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<bool>("-ccc") == false);

        auto args3 = parser.parse_args({ "-cc" });
        REQUIRE(args3.get<uint32_t>("-c") == 2);
        REQUIRE(args3.get<bool>("-ccc") == false);

        auto args4 = parser.parse_args({ "-ccc" });
        REQUIRE(args4.get<uint32_t>("-c") == 0);
        REQUIRE(args4.get<bool>("-ccc") == true);

        auto args5 = parser.parse_args({ "-cccc" });
        REQUIRE(args5.get<uint32_t>("-c") == 4);
        REQUIRE(args5.get<bool>("-ccc") == false);
    }
}

TEST_CASE("11. subparsers", "[argument_parser]")
{
    auto parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("11.1. main parser without positional arguments") {
        parser.add_argument("--foo").action("store_true").help("foo help");

        auto& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        auto& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        auto& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices({ "X", "Y", "Z" }).help("baz help");

        auto args0 = parser.parse_args({ "--foo" });
        REQUIRE(args0.get<uint32_t>("bar") == 0);
        REQUIRE(args0.get<bool>("foo") == true);
        REQUIRE(args0.get<std::string>("baz") == "");
        REQUIRE(args0.get<std::string>("cmd") == "");

        auto args1 = parser.parse_args({ "a", "12" });
        REQUIRE(args1.get<uint32_t>("bar") == 12);
        REQUIRE(args1.get<bool>("foo") == false);
        REQUIRE(args1.get<std::string>("baz") == "");
        REQUIRE(args1.get<std::string>("cmd") == "a");

        auto args2 = parser.parse_args({ "--foo", "b", "--baz", "Z" });
        REQUIRE(args2.get<uint32_t>("bar") == 0);
        REQUIRE(args2.get<bool>("foo") == true);
        REQUIRE(args2.get<std::string>("baz") == "Z");
        REQUIRE(args2.get<std::string>("cmd") == "b");

        REQUIRE_THROWS(parser.parse_args({ "a", "12", "--foo" }));
        REQUIRE_THROWS(parser.parse_args({ "b", "--foo" }));
        REQUIRE_THROWS(parser.parse_args({ "--foo", "a", "12", "b", "--baz", "Z" }));
    }

    SECTION("11.2. main parser with store positional arguments") {
        parser.add_argument("--foo").action("store_true").help("foo help");
        parser.add_argument("boo").action("store").help("boo help");

        auto& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        auto& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        auto& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices({ "X", "Y", "Z" }).help("baz help");

        REQUIRE_THROWS(parser.parse_args({ }));

        auto args0 = parser.parse_args({ "boo", "--foo" });
        REQUIRE(args0.get<std::string>("boo") == "boo");
        REQUIRE(args0.get<uint32_t>("bar") == 0);
        REQUIRE(args0.get<bool>("foo") == true);
        REQUIRE(args0.get<std::string>("baz") == "");
        REQUIRE(args0.get<std::string>("cmd") == "");

        auto args1 = parser.parse_args({ "boo", "--foo", "a", "12" });
        REQUIRE(args1.get<std::string>("boo") == "boo");
        REQUIRE(args1.get<uint32_t>("bar") == 12);
        REQUIRE(args1.get<bool>("foo") == true);
        REQUIRE(args1.get<std::string>("baz") == "");
        REQUIRE(args1.get<std::string>("cmd") == "a");

        auto args2 = parser.parse_args({ "boo", "--foo", "b", "--baz", "Z" });
        REQUIRE(args2.get<std::string>("boo") == "boo");
        REQUIRE(args2.get<uint32_t>("bar") == 0);
        REQUIRE(args2.get<bool>("foo") == true);
        REQUIRE(args2.get<std::string>("baz") == "Z");
        REQUIRE(args2.get<std::string>("cmd") == "b");

        auto args3 = parser.parse_args({ "boo", "a", "12" });
        REQUIRE(args3.get<std::string>("boo") == "boo");
        REQUIRE(args3.get<uint32_t>("bar") == 12);
        REQUIRE(args3.get<bool>("foo") == false);
        REQUIRE(args3.get<std::string>("baz") == "");
        REQUIRE(args3.get<std::string>("cmd") == "a");

        auto args4 = parser.parse_args({ "--foo", "boo", "a", "12" });
        REQUIRE(args4.get<std::string>("boo") == "boo");
        REQUIRE(args4.get<uint32_t>("bar") == 12);
        REQUIRE(args4.get<bool>("foo") == true);
        REQUIRE(args4.get<std::string>("baz") == "");
        REQUIRE(args4.get<std::string>("cmd") == "a");

        auto args5 = parser.parse_args({ "--foo", "boo", "b", "--baz", "Z" });
        REQUIRE(args5.get<std::string>("boo") == "boo");
        REQUIRE(args5.get<uint32_t>("bar") == 0);
        REQUIRE(args5.get<bool>("foo") == true);
        REQUIRE(args5.get<std::string>("baz") == "Z");
        REQUIRE(args5.get<std::string>("cmd") == "b");
    }

    SECTION("11.3. main parser with store_const positional arguments") {
        std::string const_value = "const";

        parser.add_argument("--foo").action("store_true").help("foo help");
        parser.add_argument("boo").action("store_true").help("boo help");
        parser.add_argument("doo").action("store_false").help("doo help");

        auto& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        auto& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        auto& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices({ "X", "Y", "Z" }).help("baz help");

        parser.add_argument("coo").action("store_const").const_value(const_value).help("coo help");

        auto args0 = parser.parse_args({ "--foo" });
        REQUIRE(args0.get<bool>("boo") == true);
        REQUIRE(args0.get<bool>("doo") == false);
        REQUIRE(args0.get<std::string>("coo") == const_value);
        REQUIRE(args0.get<uint32_t>("bar") == 0);
        REQUIRE(args0.get<bool>("foo") == true);
        REQUIRE(args0.get<std::string>("baz") == "");
        REQUIRE(args0.get<std::string>("cmd") == "");

        auto args1 = parser.parse_args({ "--foo", "a", "12" });
        REQUIRE(args1.get<bool>("boo") == true);
        REQUIRE(args1.get<bool>("doo") == false);
        REQUIRE(args1.get<std::string>("coo") == const_value);
        REQUIRE(args1.get<uint32_t>("bar") == 12);
        REQUIRE(args1.get<bool>("foo") == true);
        REQUIRE(args1.get<std::string>("baz") == "");
        REQUIRE(args1.get<std::string>("cmd") == "a");

        auto args2 = parser.parse_args({ "--foo", "b", "--baz", "Z" });
        REQUIRE(args2.get<bool>("boo") == true);
        REQUIRE(args2.get<bool>("doo") == false);
        REQUIRE(args2.get<std::string>("coo") == const_value);
        REQUIRE(args2.get<uint32_t>("bar") == 0);
        REQUIRE(args2.get<bool>("foo") == true);
        REQUIRE(args2.get<std::string>("baz") == "Z");
        REQUIRE(args2.get<std::string>("cmd") == "b");

        auto args3 = parser.parse_args({ "a", "12" });
        REQUIRE(args3.get<bool>("boo") == true);
        REQUIRE(args3.get<bool>("doo") == false);
        REQUIRE(args3.get<std::string>("coo") == const_value);
        REQUIRE(args3.get<uint32_t>("bar") == 12);
        REQUIRE(args3.get<bool>("foo") == false);
        REQUIRE(args3.get<std::string>("baz") == "");
        REQUIRE(args3.get<std::string>("cmd") == "a");

        auto args4 = parser.parse_args({ "--foo", "b", "--baz", "Z" });
        REQUIRE(args4.get<bool>("boo") == true);
        REQUIRE(args4.get<bool>("doo") == false);
        REQUIRE(args4.get<std::string>("coo") == const_value);
        REQUIRE(args4.get<uint32_t>("bar") == 0);
        REQUIRE(args4.get<bool>("foo") == true);
        REQUIRE(args4.get<std::string>("baz") == "Z");
        REQUIRE(args4.get<std::string>("cmd") == "b");
    }

    SECTION("11.4. main parser with negative number options present") {
        parser.add_argument("-1").dest("one").help("one help");
        parser.add_argument("--foo").action("store_true").help("foo help");

        auto& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        auto& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");

        auto& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        // so -1 is an option in main
        auto args0 = parser.parse_args({ "-1", "x" });
        REQUIRE(args0.get<std::string>("one") == "x");
        REQUIRE(args0.get<uint32_t>("bar") == 0);
        REQUIRE(args0.get<bool>("foo") == false);
        REQUIRE(args0.get<std::string>("baz") == "");
        REQUIRE(args0.get<std::string>("cmd") == "");

        auto args1 = parser.parse_args({ "a", "-1" });
        REQUIRE(args1.get<std::string>("one") == "");
        REQUIRE(args1.get<int32_t>("bar") == -1);
        REQUIRE(args1.get<bool>("foo") == false);
        REQUIRE(args1.get<std::string>("baz") == "");
        REQUIRE(args1.get<std::string>("cmd") == "a");

        // so -2 is an option in main
        REQUIRE_THROWS(parser.parse_args({ "-2" }));

        auto args2 = parser.parse_args({ "--foo", "a", "-2" });
        REQUIRE(args2.get<std::string>("one") == "");
        REQUIRE(args2.get<int32_t>("bar") == -2);
        REQUIRE(args2.get<bool>("foo") == true);
        REQUIRE(args2.get<std::string>("baz") == "");
        REQUIRE(args2.get<std::string>("cmd") == "a");

        // so both -1s are options in main
        REQUIRE_THROWS(parser.parse_args({ "-1", "-1" }));
    }

    SECTION("11.5. subparser with negative number options present") {
        parser.add_argument("--foo").action("store_true").help("foo help");
        parser.add_argument("--boo").action("store").help("boo help");

        auto& subparsers = parser.add_subparsers().dest("cmd").help("sub-command help");

        auto& parser_a = subparsers.add_parser("a").help("a help");
        parser_a.add_argument("bar").help("bar help");
        parser_a.add_argument("-1").dest("one").help("one help");

        auto& parser_b = subparsers.add_parser("b").help("b help");
        parser_b.add_argument("--baz").choices("XYZ").help("baz help");

        // so -1 is an option in subparser
        auto args0 = parser.parse_args({ "--boo", "-1", "a", "-1", "x", "1" });
        REQUIRE(args0.get<std::string>("one") == "x");
        REQUIRE(args0.get<std::string>("boo") == "-1");
        REQUIRE(args0.get<int32_t>("bar") == 1);
        REQUIRE(args0.get<bool>("foo") == false);
        REQUIRE(args0.get<std::string>("baz") == "");
        REQUIRE(args0.get<std::string>("cmd") == "a");

        auto args1 = parser.parse_args({ "a", "-1", "x", "1" });
        REQUIRE(args1.get<std::string>("one") == "x");
        REQUIRE(args1.get<std::string>("boo") == "");
        REQUIRE(args1.get<int32_t>("bar") == 1);
        REQUIRE(args1.get<bool>("foo") == false);
        REQUIRE(args1.get<std::string>("baz") == "");
        REQUIRE(args1.get<std::string>("cmd") == "a");

        // so -2 is an option in subparser
        REQUIRE_THROWS(parser.parse_args({ "a", "-1", "x", "-2" }));
        // so both -1s are options in subparser
        REQUIRE_THROWS(parser.parse_args({ "a", "-1", "-1" }));
    }
}

TEST_CASE("12. default values", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";
    std::string new_default = "new";

    SECTION("12.1. have global value") {
        auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(local_default).help("bar help");

        REQUIRE(parser.get_default("foo") == global_default);
        REQUIRE(parser.get_default("bar") == local_default);

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.get<std::string>("foo") == global_default);
        REQUIRE(args0.get<std::string>("bar") == local_default);

        parser.set_defaults({ { "foo", new_default } });
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("foo") == new_default);
        REQUIRE(args1.get<std::string>("bar") == local_default);

        parser.set_defaults({ { "foobaz", new_default } });
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);
        REQUIRE(parser.get_default("foobaz") == new_default);

        auto args2 = parser.parse_args({ });
        REQUIRE(args2.get<std::string>("foo") == new_default);
        REQUIRE(args2.get<std::string>("bar") == local_default);
        REQUIRE(args2.get<std::string>("foobaz") == new_default);
    }

    SECTION("12.2. no global value") {
        auto parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(local_default).help("bar help");

        REQUIRE(parser.get_default("foo") == "");
        REQUIRE(parser.get_default("bar") == local_default);

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.get<std::string>("foo") == "");
        REQUIRE(args0.get<std::string>("bar") == local_default);

        parser.set_defaults({ { "foo", new_default } });
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);

        auto args1 = parser.parse_args({ });
        REQUIRE(args1.get<std::string>("foo") == new_default);
        REQUIRE(args1.get<std::string>("bar") == local_default);

        parser.set_defaults({ { "foobaz", new_default } });
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);
        REQUIRE(parser.get_default("foobaz") == new_default);

        auto args2 = parser.parse_args({ });
        REQUIRE(args2.get<std::string>("foo") == new_default);
        REQUIRE(args2.get<std::string>("bar") == local_default);
        REQUIRE(args2.get<std::string>("foobaz") == new_default);
    }
}

TEST_CASE("13. value exists check", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";
    std::string new_default = "default";
    std::string new_value = "new";

    SECTION("13.1. have default value") {
        auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(local_default).help("bar help");

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.exists("foo") == true);
        REQUIRE(args0.exists("bar") == true);
        REQUIRE(args0.exists("foobar") == false);
        REQUIRE(args0.get<std::string>("foo") == global_default);
        REQUIRE(args0.get<std::string>("bar") == local_default);
    }

    SECTION("13.2. no default value") {
        auto parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").help("bar help");

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.exists("foo") == false);
        REQUIRE(args0.exists("bar") == false);
        REQUIRE(args0.exists("foobar") == false);
        REQUIRE(args0.get<std::string>("foo") == "");
        REQUIRE(args0.get<std::string>("bar") == "");

        parser.set_defaults({ { "foo", new_default } });

        auto args1 = parser.parse_args({ "--bar", new_value });
        REQUIRE(args1.exists("foo") == true);
        REQUIRE(args1.exists("bar") == true);
        REQUIRE(args1.exists("foobar") == false);
        REQUIRE(args1.get<std::string>("foo") == new_default);
        REQUIRE(args1.get<std::string>("bar") == new_value);
    }
}

TEST_CASE("14. pseudo-argument '--'", "[argument_parser]")
{
    auto parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("14.1. options like '--' [fail]") {
        parser.add_argument("--").action("store").help("-- help");

        // dest= is required for options like '--'
        REQUIRE_THROWS(parser.parse_args({ }));
        REQUIRE_THROWS(parser.parse_args({ "--=1" }));
    }

    SECTION("14.2. options like '--' [ ok ]") {
        // better don't use dest='--'
        parser.add_argument("--").action("store").dest("--").help("-- help");
        REQUIRE_THROWS(parser.add_argument("--").action("store").dest("foo").help("foo help"));

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.get<std::string>("--") == "");

        auto args1 = parser.parse_args({ "--=1" });
        REQUIRE(args1.get<std::string>("--") == "1");
    }

    SECTION("14.3. store arguments like optional") {
        parser.add_argument("store1").action("store").help("store1 help");
        parser.add_argument("--store2").action("store").help("store2 help");

        auto args0 = parser.parse_args({ "--", "--store2" });
        REQUIRE(args0.get<std::string>("store1") == "--store2");
        REQUIRE(args0.get<std::string>("store2") == "");

        REQUIRE_THROWS(parser.parse_args({ "--", "--store2", "--store2=--store2" }));

        auto args1 = parser.parse_args({ "--store2=--store2", "--", "--store2" });
        REQUIRE(args1.get<std::string>("store1") == "--store2");
        REQUIRE(args1.get<std::string>("store2") == "--store2");
    }

    SECTION("14.4. prefix chars '+'") {
        parser.prefix_chars("+");

        parser.add_argument("store1").action("store").help("store1 help");
        parser.add_argument("++store2").action("store").help("store2 help");

        auto args0 = parser.parse_args({ "--", "++store2" });
        REQUIRE(args0.get<std::string>("store1") == "++store2");
        REQUIRE(args0.get<std::string>("store2") == "");

        REQUIRE_THROWS(parser.parse_args({ "--", "++store2", "++store2=++store2" }));

        auto args1 = parser.parse_args({ "++store2=++store2", "--", "++store2" });
        REQUIRE(args1.get<std::string>("store1") == "++store2");
        REQUIRE(args1.get<std::string>("store2") == "++store2");
    }
}

TEST_CASE("15. prefix chars '='", "[argument_parser]")
{
    auto parser = argparse::ArgumentParser().prefix_chars("=").exit_on_error(false);

    SECTION("15.1. without store actions") {
        parser.add_argument({ "=f", "==foo" }).action("store_true").help("foo help");
        parser.add_argument("==bar").action("store_false").help("bar help");

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.get<bool>("f") == false);
        REQUIRE(args0.get<bool>("foo") == false);
        REQUIRE(args0.get<bool>("bar") == true);

        auto args1 = parser.parse_args({ "=f", "==bar" });
        REQUIRE(args1.get<bool>("f") == true);
        REQUIRE(args1.get<bool>("foo") == true);
        REQUIRE(args1.get<bool>("bar") == false);
    }

    SECTION("15.2. with store actions") {
        parser.add_argument({ "=f", "==foo" }).action("store").help("foo help");
        parser.add_argument("==bar").action("store").help("bar help");

        auto args0 = parser.parse_args({ });
        REQUIRE(args0.get<std::string>("f") == "");
        REQUIRE(args0.get<std::string>("foo") == "");
        REQUIRE(args0.get<std::string>("bar") == "");

        auto args1 = parser.parse_args({ "=f", "foo", "==bar=bar" });
        REQUIRE(args1.get<std::string>("f") == "foo");
        REQUIRE(args1.get<std::string>("foo") == "foo");
        REQUIRE(args1.get<std::string>("bar") == "bar");
    }
}
