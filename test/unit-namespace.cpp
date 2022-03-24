#include <catch2/catch.hpp>

#include <argparse/argparse.hpp>

TEST_CASE("1. to string", "[namespace]")
{
    std::string global_default = "global";

    SECTION("1.1. sample") {
        auto parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo");
        parser.add_argument("--bar").dest("foobar");

        REQUIRE(parser.parse_args("").to_string() == "Namespace(foo=None, foobar=None)");
        REQUIRE(parser.parse_args("--foo a").to_string() == "Namespace(foo='a', foobar=None)");
        REQUIRE(parser.parse_known_args("").to_string() == "(Namespace(foo=None, foobar=None), [])");
        REQUIRE(parser.parse_known_args("a").to_string() == "(Namespace(foo=None, foobar=None), ['a'])");
        REQUIRE(parser.parse_known_args("--bar a").to_string() == "(Namespace(foo=None, foobar='a'), [])");
    }

    SECTION("1.2. optional action store without default value") {
        auto parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo1").action("store");
        parser.add_argument("--foo2").action("store").nargs("?");
        parser.add_argument("--foo3").action("store").nargs("*");
        parser.add_argument("--foo4").action("store").nargs("+");
        parser.add_argument("--foo5").action("store").nargs(2);

        REQUIRE(parser.parse_args("").to_string()           == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None)");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()   == "Namespace(foo1='a', foo2=None, foo3=None, foo4=None, foo5=None)");
        REQUIRE(parser.parse_args("--foo2").to_string()     == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None)");
        REQUIRE(parser.parse_args("--foo2 a").to_string()   == "Namespace(foo1=None, foo2='a', foo3=None, foo4=None, foo5=None)");
        REQUIRE(parser.parse_args("--foo3").to_string()     == "Namespace(foo1=None, foo2=None, foo3=[], foo4=None, foo5=None)");
        REQUIRE(parser.parse_args("--foo3 a").to_string()   == "Namespace(foo1=None, foo2=None, foo3=['a'], foo4=None, foo5=None)");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()   == "Namespace(foo1=None, foo2=None, foo3=None, foo4=['a'], foo5=None)");
        REQUIRE(parser.parse_args("--foo5 a a").to_string() == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=['a', 'a'])");
    }

    SECTION("1.3. optional action store with default value") {
        auto parser = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo1").action("store");
        parser.add_argument("--foo2").action("store").nargs("?");
        parser.add_argument("--foo3").action("store").nargs("*");
        parser.add_argument("--foo4").action("store").nargs("+");
        parser.add_argument("--foo5").action("store").nargs(2);

        REQUIRE(parser.parse_args("").to_string()           == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global', foo5='global')");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()   == "Namespace(foo1='a', foo2='global', foo3='global', foo4='global', foo5='global')");
        REQUIRE(parser.parse_args("--foo2").to_string()     == "Namespace(foo1='global', foo2=None, foo3='global', foo4='global', foo5='global')");
        REQUIRE(parser.parse_args("--foo2 a").to_string()   == "Namespace(foo1='global', foo2='a', foo3='global', foo4='global', foo5='global')");
        REQUIRE(parser.parse_args("--foo3").to_string()     == "Namespace(foo1='global', foo2='global', foo3=[], foo4='global', foo5='global')");
        REQUIRE(parser.parse_args("--foo3 a").to_string()   == "Namespace(foo1='global', foo2='global', foo3=['a'], foo4='global', foo5='global')");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()   == "Namespace(foo1='global', foo2='global', foo3='global', foo4=['a'], foo5='global')");
        REQUIRE(parser.parse_args("--foo5 a a").to_string() == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global', foo5=['a', 'a'])");
    }
}
