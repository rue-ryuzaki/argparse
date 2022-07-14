/*
* Copyright (c) 2021-2022 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse.hpp>

#include "./catch-define.h"

TEST_CASE("1. to string", "[namespace]")
{
    std::string global_default = "global";
    std::string const_value = "const";

    SECTION("1.1. sample") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo");
        parser.add_argument("--bar").dest("foobar");

        REQUIRE(parser.parse_args("").to_string() == "Namespace(foo=None, foobar=None)");
        REQUIRE(parser.parse_args("--foo a").to_string() == "Namespace(foo='a', foobar=None)");
        REQUIRE(parser.parse_known_args("").to_string() == "(Namespace(foo=None, foobar=None), [])");
        REQUIRE(parser.parse_known_args("a").to_string() == "(Namespace(foo=None, foobar=None), ['a'])");
        REQUIRE(parser.parse_known_args("--bar a").to_string() == "(Namespace(foo=None, foobar='a'), [])");

        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
#ifdef _ARGPARSE_CXX_11
        parser2.set_defaults({ { "foo", "bar" } });
#else
        std::vector<std::pair<std::string, std::string> > values;
        values.push_back(std::make_pair("foo", "bar"));
        parser2.set_defaults(values);
#endif  // C++11+
        REQUIRE(parser2.parse_args("").to_string() == "Namespace(foo='bar')");
    }

    SECTION("1.2. optional action store without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo1").action("store");
        parser.add_argument("--foo2").action("store").nargs("?");
        parser.add_argument("--foo3").action("store").nargs("*");
        parser.add_argument("--foo4").action("store").nargs("+");
        parser.add_argument("--foo5").action("store").nargs(2);
        parser.add_argument("--foo6").action("store").nargs("?").const_value(const_value);
        parser.add_argument("--foo7").action("store").nargs("?").const_value("");

        REQUIRE(parser.parse_args("").to_string()
                           == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()
                            == "Namespace(foo1='a', foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo2").to_string()
                           == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo2 a").to_string()
                            == "Namespace(foo1=None, foo2='a', foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo3").to_string()
                             == "Namespace(foo1=None, foo2=None, foo3=[], foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo3 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=['a'], foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=None, foo4=['a'], foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo5 a a").to_string()
                     == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=['a', 'a'], foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo6").to_string()
                        == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6='const', foo7=None)");
        REQUIRE(parser.parse_args("--foo6 a").to_string()
                            == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6='a', foo7=None)");
        REQUIRE(parser.parse_args("--foo7").to_string()
                             == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7='')");
        REQUIRE(parser.parse_args("--foo7 a").to_string()
                            == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7='a')");
    }

    SECTION("1.3. optional action store with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo1").action("store");
        parser.add_argument("--foo2").action("store").nargs("?");
        parser.add_argument("--foo3").action("store").nargs("*");
        parser.add_argument("--foo4").action("store").nargs("+");
        parser.add_argument("--foo5").action("store").nargs(2);
        parser.add_argument("--foo6").action("store").nargs("?").const_value(const_value);
        parser.add_argument("--foo7").action("store").nargs("?").const_value("");

        REQUIRE(parser.parse_args("").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()
                == "Namespace(foo1='a', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo2").to_string()
                == "Namespace(foo1='global', foo2=None, foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo2 a").to_string()
                == "Namespace(foo1='global', foo2='a', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo3").to_string()
                == "Namespace(foo1='global', foo2='global', foo3=[], foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo3 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3=['a'], foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4=['a'],"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo5 a a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5=['a', 'a'], foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo6").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='const', foo7='global')");
        REQUIRE(parser.parse_args("--foo6 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='a', foo7='global')");
        REQUIRE(parser.parse_args("--foo7").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='')");
        REQUIRE(parser.parse_args("--foo7 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='a')");
    }

    SECTION("1.4. optional action append without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo1").action("append");
        parser.add_argument("--foo2").action("append").nargs("?");
        parser.add_argument("--foo3").action("append").nargs("*");
        parser.add_argument("--foo4").action("append").nargs("+");
        parser.add_argument("--foo5").action("append").nargs(2);
        parser.add_argument("--foo6").action("append").nargs("?").const_value(const_value);
        parser.add_argument("--foo7").action("append").nargs("?").const_value("");

        REQUIRE(parser.parse_args("").to_string()
                           == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()
                          == "Namespace(foo1=['a'], foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo2").to_string()
                         == "Namespace(foo1=None, foo2=[None], foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo2 a").to_string()
                          == "Namespace(foo1=None, foo2=['a'], foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo3").to_string()
                           == "Namespace(foo1=None, foo2=None, foo3=[[]], foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo3 a").to_string()
                        == "Namespace(foo1=None, foo2=None, foo3=[['a']], foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()
                        == "Namespace(foo1=None, foo2=None, foo3=None, foo4=[['a']], foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo5 a a").to_string()
                   == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=[['a', 'a']], foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo6").to_string()
                      == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=['const'], foo7=None)");
        REQUIRE(parser.parse_args("--foo6 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=['a'], foo7=None)");
        REQUIRE(parser.parse_args("--foo7").to_string()
                           == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=[''])");
        REQUIRE(parser.parse_args("--foo7 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=['a'])");
    }

    SECTION("1.5. optional action append with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo1").action("append");
        parser.add_argument("--foo2").action("append").nargs("?");
        parser.add_argument("--foo3").action("append").nargs("*");
        parser.add_argument("--foo4").action("append").nargs("+");
        parser.add_argument("--foo5").action("append").nargs(2);
        parser.add_argument("--foo6").action("append").nargs("?").const_value(const_value);
        parser.add_argument("--foo7").action("append").nargs("?").const_value("");

        REQUIRE(parser.parse_args("").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()
                == "Namespace(foo1=['a'], foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo2").to_string()
                == "Namespace(foo1='global', foo2=[None], foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo2 a").to_string()
                == "Namespace(foo1='global', foo2=['a'], foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo3").to_string()
                == "Namespace(foo1='global', foo2='global', foo3=[[]], foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo3 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3=[['a']], foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4=[['a']],"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo5 a a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5=[['a', 'a']], foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo6").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6=['const'], foo7='global')");
        REQUIRE(parser.parse_args("--foo6 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6=['a'], foo7='global')");
        REQUIRE(parser.parse_args("--foo7").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7=[''])");
        REQUIRE(parser.parse_args("--foo7 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7=['a'])");
    }

    SECTION("1.6. optional action extend without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo1").action("extend");
        parser.add_argument("--foo2").action("extend").nargs("?");
        parser.add_argument("--foo3").action("extend").nargs("*");
        parser.add_argument("--foo4").action("extend").nargs("+");
        parser.add_argument("--foo5").action("extend").nargs(2);
        parser.add_argument("--foo6").action("extend").nargs("?").const_value(const_value);
        parser.add_argument("--foo7").action("extend").nargs("?").const_value("");

        REQUIRE(parser.parse_args("").to_string()
                           == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()
                          == "Namespace(foo1=['a'], foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo2"));
        REQUIRE(parser.parse_args("--foo2 a").to_string()
                          == "Namespace(foo1=None, foo2=['a'], foo3=None, foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo3").to_string()
                             == "Namespace(foo1=None, foo2=None, foo3=[], foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo3 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=['a'], foo4=None, foo5=None, foo6=None, foo7=None)");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=None, foo4=['a'], foo5=None, foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo5 a a").to_string()
                     == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=['a', 'a'], foo6=None, foo7=None)");
        REQUIRE(parser.parse_args("--foo6").to_string()
                ==
         "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=['c', 'o', 'n', 's', 't'], foo7=None)");
        REQUIRE(parser.parse_args("--foo6 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=['a'], foo7=None)");
        REQUIRE(parser.parse_args("--foo7").to_string()
                             == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=[])");
        REQUIRE(parser.parse_args("--foo7 a").to_string()
                          == "Namespace(foo1=None, foo2=None, foo3=None, foo4=None, foo5=None, foo6=None, foo7=['a'])");
    }

    SECTION("1.7. optional action extend with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo1").action("extend");
        parser.add_argument("--foo2").action("extend").nargs("?");
        parser.add_argument("--foo3").action("extend").nargs("*");
        parser.add_argument("--foo4").action("extend").nargs("+");
        parser.add_argument("--foo5").action("extend").nargs(2);
        parser.add_argument("--foo6").action("extend").nargs("?").const_value(const_value);
        parser.add_argument("--foo7").action("extend").nargs("?").const_value("");

        REQUIRE(parser.parse_args("").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo1"));
        REQUIRE(parser.parse_args("--foo1 a").to_string()
                == "Namespace(foo1=['a'], foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo2"));
        REQUIRE(parser.parse_args("--foo2 a").to_string()
                == "Namespace(foo1='global', foo2=['a'], foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo3").to_string()
                == "Namespace(foo1='global', foo2='global', foo3=[], foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo3 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3=['a'], foo4='global',"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE_THROWS(parser.parse_args("--foo4"));
        REQUIRE(parser.parse_args("--foo4 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4=['a'],"
                   " foo5='global', foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo5 a a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5=['a', 'a'], foo6='global', foo7='global')");
        REQUIRE(parser.parse_args("--foo6").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6=['c', 'o', 'n', 's', 't'], foo7='global')");
        REQUIRE(parser.parse_args("--foo6 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6=['a'], foo7='global')");
        REQUIRE(parser.parse_args("--foo7").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7=[])");
        REQUIRE(parser.parse_args("--foo7 a").to_string()
                == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global',"
                   " foo5='global', foo6='global', foo7=['a'])");
    }

    SECTION("1.8. optional action store const without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo1").action("store_const").const_value(const_value);
        parser.add_argument("--foo2").action("store_const").const_value("");
        parser.add_argument("--foo3").action("store_true");
        parser.add_argument("--foo4").action("store_false");

        REQUIRE(parser.parse_args("").to_string()       == "Namespace(foo1=None, foo2=None, foo3=false, foo4=true)");
        REQUIRE(parser.parse_args("--foo1").to_string() == "Namespace(foo1='const', foo2=None, foo3=false, foo4=true)");
        REQUIRE(parser.parse_args("--foo2").to_string() == "Namespace(foo1=None, foo2='', foo3=false, foo4=true)");
        REQUIRE(parser.parse_args("--foo3").to_string() == "Namespace(foo1=None, foo2=None, foo3=true, foo4=true)");
        REQUIRE(parser.parse_args("--foo4").to_string() == "Namespace(foo1=None, foo2=None, foo3=false, foo4=false)");
    }

    SECTION("1.9. optional action store const with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo1").action("store_const").const_value(const_value);
        parser.add_argument("--foo2").action("store_const").const_value("");
        parser.add_argument("--foo3").action("store_true");
        parser.add_argument("--foo4").action("store_false");

        REQUIRE(parser.parse_args("").to_string()
                                            == "Namespace(foo1='global', foo2='global', foo3='global', foo4='global')");
        REQUIRE(parser.parse_args("--foo1").to_string()
                                             == "Namespace(foo1='const', foo2='global', foo3='global', foo4='global')");
        REQUIRE(parser.parse_args("--foo2").to_string()
                                                  == "Namespace(foo1='global', foo2='', foo3='global', foo4='global')");
        REQUIRE(parser.parse_args("--foo3").to_string()
                                                == "Namespace(foo1='global', foo2='global', foo3=true, foo4='global')");
        REQUIRE(parser.parse_args("--foo4").to_string()
                                               == "Namespace(foo1='global', foo2='global', foo3='global', foo4=false)");
    }

    SECTION("1.10. optional action append_const without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo1").action("append_const").const_value(const_value);
        parser.add_argument("--foo2").action("append_const").const_value("");

        REQUIRE(parser.parse_args("").to_string()       == "Namespace(foo1=None, foo2=None)");
        REQUIRE(parser.parse_args("--foo1").to_string() == "Namespace(foo1=['const'], foo2=None)");
        REQUIRE(parser.parse_args("--foo2").to_string() == "Namespace(foo1=None, foo2=[''])");
    }

    SECTION("1.11. optional action append_const with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo1").action("append_const").const_value(const_value);
        parser.add_argument("--foo2").action("append_const").const_value("");

        REQUIRE(parser.parse_args("").to_string()       == "Namespace(foo1='global', foo2='global')");
        REQUIRE(parser.parse_args("--foo1").to_string() == "Namespace(foo1=['const'], foo2='global')");
        REQUIRE(parser.parse_args("--foo2").to_string() == "Namespace(foo1='global', foo2=[''])");
    }

    SECTION("1.12. optional action BooleanOptionalAction without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("--foo").action(argparse::BooleanOptionalAction);

        REQUIRE(parser.parse_args("").to_string()           == "Namespace(foo=None)");
        REQUIRE(parser.parse_args("--foo").to_string()      == "Namespace(foo=true)");
        REQUIRE(parser.parse_args("--no-foo").to_string()   == "Namespace(foo=false)");
    }

    SECTION("1.13. optional action BooleanOptionalAction with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("--foo").action(argparse::BooleanOptionalAction);

        REQUIRE(parser.parse_args("").to_string()           == "Namespace(foo='global')");
        REQUIRE(parser.parse_args("--foo").to_string()      == "Namespace(foo=true)");
        REQUIRE(parser.parse_args("--no-foo").to_string()   == "Namespace(foo=false)");
    }

    SECTION("1.14. optional action count") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("-f", "--foo").action("count");

        REQUIRE(parser.parse_args("").to_string()       == "Namespace(foo=None)");
        REQUIRE(parser.parse_args("-f").to_string()     == "Namespace(foo=1)");
        REQUIRE(parser.parse_args("-f -f").to_string()  == "Namespace(foo=2)");
    }

    SECTION("1.15. positional action store without default value") {
        argparse::ArgumentParser parser1 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser3 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser4 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser5 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser6 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser7 = argparse::ArgumentParser().exit_on_error(false);
        parser1.add_argument("foo").action("store");
        parser2.add_argument("foo").action("store").nargs("?");
        parser3.add_argument("foo").action("store").nargs("*");
        parser4.add_argument("foo").action("store").nargs("+");
        parser5.add_argument("foo").action("store").nargs(2);
        parser6.add_argument("foo").action("store").nargs("?").const_value(const_value);
        parser7.add_argument("foo").action("store").nargs("?").const_value("");

        REQUIRE_THROWS(parser1.parse_args(""));
        REQUIRE(parser1.parse_args("a").to_string()     == "Namespace(foo='a')");
        REQUIRE(parser2.parse_args("").to_string()      == "Namespace(foo=None)");
        REQUIRE(parser2.parse_args("a").to_string()     == "Namespace(foo='a')");
        REQUIRE(parser3.parse_args("").to_string()      == "Namespace(foo=[])");
        REQUIRE(parser3.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser4.parse_args(""));
        REQUIRE(parser4.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser5.parse_args(""));
        REQUIRE(parser5.parse_args("a a").to_string()   == "Namespace(foo=['a', 'a'])");
        REQUIRE(parser6.parse_args("").to_string()      == "Namespace(foo=None)");
        REQUIRE(parser6.parse_args("a").to_string()     == "Namespace(foo='a')");
        REQUIRE(parser7.parse_args("").to_string()      == "Namespace(foo=None)");
        REQUIRE(parser7.parse_args("a").to_string()     == "Namespace(foo='a')");
    }

    SECTION("1.16. positional action store with default value") {
        argparse::ArgumentParser parser1
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser2
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser3
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser4
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser5
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser6
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser7
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser1.add_argument("foo").action("store");
        parser2.add_argument("foo").action("store").nargs("?");
        parser3.add_argument("foo").action("store").nargs("*");
        parser4.add_argument("foo").action("store").nargs("+");
        parser5.add_argument("foo").action("store").nargs(2);
        parser6.add_argument("foo").action("store").nargs("?").const_value(const_value);
        parser7.add_argument("foo").action("store").nargs("?").const_value("");

        REQUIRE_THROWS(parser1.parse_args(""));
        REQUIRE(parser1.parse_args("a").to_string()     == "Namespace(foo='a')");
        REQUIRE(parser2.parse_args("").to_string()      == "Namespace(foo='global')");
        REQUIRE(parser2.parse_args("a").to_string()     == "Namespace(foo='a')");
        REQUIRE(parser3.parse_args("").to_string()      == "Namespace(foo='global')");
        REQUIRE(parser3.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser4.parse_args(""));
        REQUIRE(parser4.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser5.parse_args(""));
        REQUIRE(parser5.parse_args("a a").to_string()   == "Namespace(foo=['a', 'a'])");
        REQUIRE(parser6.parse_args("").to_string()      == "Namespace(foo='global')");
        REQUIRE(parser6.parse_args("a").to_string()     == "Namespace(foo='a')");
        REQUIRE(parser7.parse_args("").to_string()      == "Namespace(foo='global')");
        REQUIRE(parser7.parse_args("a").to_string()     == "Namespace(foo='a')");
    }

    SECTION("1.17. positional action append without default value") {
        argparse::ArgumentParser parser1 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser3 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser4 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser5 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser6 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser7 = argparse::ArgumentParser().exit_on_error(false);
        parser1.add_argument("foo").action("append");
        parser2.add_argument("foo").action("append").nargs("?");
        parser3.add_argument("foo").action("append").nargs("*");
        parser4.add_argument("foo").action("append").nargs("+");
        parser5.add_argument("foo").action("append").nargs(2);
        parser6.add_argument("foo").action("append").nargs("?").const_value(const_value);
        parser7.add_argument("foo").action("append").nargs("?").const_value("");

        REQUIRE_THROWS(parser1.parse_args(""));
        REQUIRE(parser1.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser2.parse_args("").to_string()      == "Namespace(foo=[None])");
        REQUIRE(parser2.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser3.parse_args("").to_string()      == "Namespace(foo=[[]])");
        REQUIRE(parser3.parse_args("a").to_string()     == "Namespace(foo=[['a']])");
        REQUIRE_THROWS(parser4.parse_args(""));
        REQUIRE(parser4.parse_args("a").to_string()     == "Namespace(foo=[['a']])");
        REQUIRE_THROWS(parser5.parse_args(""));
        REQUIRE(parser5.parse_args("a a").to_string()   == "Namespace(foo=[['a', 'a']])");
        REQUIRE(parser6.parse_args("").to_string()      == "Namespace(foo=[None])");
        REQUIRE(parser6.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser7.parse_args("").to_string()      == "Namespace(foo=[None])");
        REQUIRE(parser7.parse_args("a").to_string()     == "Namespace(foo=['a'])");
    }

    SECTION("1.18. positional action append with default value") {
        argparse::ArgumentParser parser1
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser2
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser3
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser4
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser5
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser6
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser7
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser1.add_argument("foo").action("append");
        parser2.add_argument("foo").action("append").nargs("?");
        parser3.add_argument("foo").action("append").nargs("*");
        parser4.add_argument("foo").action("append").nargs("+");
        parser5.add_argument("foo").action("append").nargs(2);
        parser6.add_argument("foo").action("append").nargs("?").const_value(const_value);
        parser7.add_argument("foo").action("append").nargs("?").const_value("");

        REQUIRE_THROWS(parser1.parse_args(""));
        REQUIRE(parser1.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser2.parse_args("").to_string()      == "Namespace(foo=[None])");
        REQUIRE(parser2.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser3.parse_args("").to_string()      == "Namespace(foo=[[]])");
        REQUIRE(parser3.parse_args("a").to_string()     == "Namespace(foo=[['a']])");
        REQUIRE_THROWS(parser4.parse_args(""));
        REQUIRE(parser4.parse_args("a").to_string()     == "Namespace(foo=[['a']])");
        REQUIRE_THROWS(parser5.parse_args(""));
        REQUIRE(parser5.parse_args("a a").to_string()   == "Namespace(foo=[['a', 'a']])");
        REQUIRE(parser6.parse_args("").to_string()      == "Namespace(foo=[None])");
        REQUIRE(parser6.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser7.parse_args("").to_string()      == "Namespace(foo=[None])");
        REQUIRE(parser7.parse_args("a").to_string()     == "Namespace(foo=['a'])");
    }

    SECTION("1.19. positional action extend without default value") {
        argparse::ArgumentParser parser1 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser3 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser4 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser5 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser6 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser7 = argparse::ArgumentParser().exit_on_error(false);
        parser1.add_argument("foo").action("extend");
        parser2.add_argument("foo").action("extend").nargs("?");
        parser3.add_argument("foo").action("extend").nargs("*");
        parser4.add_argument("foo").action("extend").nargs("+");
        parser5.add_argument("foo").action("extend").nargs(2);
        parser6.add_argument("foo").action("extend").nargs("?").const_value(const_value);
        parser7.add_argument("foo").action("extend").nargs("?").const_value("");

        REQUIRE_THROWS(parser1.parse_args(""));
        REQUIRE(parser1.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser2.parse_args(""));
        REQUIRE(parser2.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser3.parse_args("").to_string()      == "Namespace(foo=[])");
        REQUIRE(parser3.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser4.parse_args(""));
        REQUIRE(parser4.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser5.parse_args(""));
        REQUIRE(parser5.parse_args("a a").to_string()   == "Namespace(foo=['a', 'a'])");
        REQUIRE_THROWS(parser6.parse_args(""));
        REQUIRE(parser6.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser7.parse_args(""));
        REQUIRE(parser7.parse_args("a").to_string()     == "Namespace(foo=['a'])");
    }

    SECTION("1.20. positional action extend with default value") {
        argparse::ArgumentParser parser1
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser2
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser3
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser4
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser5
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser6
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser7
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser1.add_argument("foo").action("extend");
        parser2.add_argument("foo").action("extend").nargs("?");
        parser3.add_argument("foo").action("extend").nargs("*");
        parser4.add_argument("foo").action("extend").nargs("+");
        parser5.add_argument("foo").action("extend").nargs(2);
        parser6.add_argument("foo").action("extend").nargs("?").const_value(const_value);
        parser7.add_argument("foo").action("extend").nargs("?").const_value("");

        REQUIRE_THROWS(parser1.parse_args(""));
        REQUIRE(parser1.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser2.parse_args(""));
        REQUIRE(parser2.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE(parser3.parse_args("").to_string()      == "Namespace(foo=[])");
        REQUIRE(parser3.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser4.parse_args(""));
        REQUIRE(parser4.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser5.parse_args(""));
        REQUIRE(parser5.parse_args("a a").to_string()   == "Namespace(foo=['a', 'a'])");
        REQUIRE_THROWS(parser6.parse_args(""));
        REQUIRE(parser6.parse_args("a").to_string()     == "Namespace(foo=['a'])");
        REQUIRE_THROWS(parser7.parse_args(""));
        REQUIRE(parser7.parse_args("a").to_string()     == "Namespace(foo=['a'])");
    }

    SECTION("1.21. positional action store const without default value") {
        argparse::ArgumentParser parser1 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser3 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser4 = argparse::ArgumentParser().exit_on_error(false);
        parser1.add_argument("foo").action("store_const").const_value(const_value);
        parser2.add_argument("foo").action("store_const").const_value("");
        parser3.add_argument("foo").action("store_true");
        parser4.add_argument("foo").action("store_false");

        REQUIRE(parser1.parse_args("").to_string()  == "Namespace(foo='const')");
        REQUIRE(parser2.parse_args("").to_string()  == "Namespace(foo='')");
        REQUIRE(parser3.parse_args("").to_string()  == "Namespace(foo=true)");
        REQUIRE(parser4.parse_args("").to_string()  == "Namespace(foo=false)");
    }

    SECTION("1.22. positional action store const with default value") {
        argparse::ArgumentParser parser1
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser2
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser3
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser4
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser1.add_argument("foo").action("store_const").const_value(const_value);
        parser2.add_argument("foo").action("store_const").const_value("");
        parser3.add_argument("foo").action("store_true");
        parser4.add_argument("foo").action("store_false");

        REQUIRE(parser1.parse_args("").to_string()  == "Namespace(foo='const')");
        REQUIRE(parser2.parse_args("").to_string()  == "Namespace(foo='')");
        REQUIRE(parser3.parse_args("").to_string()  == "Namespace(foo=true)");
        REQUIRE(parser4.parse_args("").to_string()  == "Namespace(foo=false)");
    }

    SECTION("1.23. positional action append_const without default value") {
        argparse::ArgumentParser parser1 = argparse::ArgumentParser().exit_on_error(false);
        argparse::ArgumentParser parser2 = argparse::ArgumentParser().exit_on_error(false);
        parser1.add_argument("foo").action("append_const").const_value(const_value);
        parser2.add_argument("foo").action("append_const").const_value("");

        REQUIRE(parser1.parse_args("").to_string()  == "Namespace(foo=['const'])");
        REQUIRE(parser2.parse_args("").to_string()  == "Namespace(foo=[''])");
    }

    SECTION("1.24. positional action append_const with default value") {
        argparse::ArgumentParser parser1
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        argparse::ArgumentParser parser2
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser1.add_argument("foo").action("append_const").const_value(const_value);
        parser2.add_argument("foo").action("append_const").const_value("");

        REQUIRE(parser1.parse_args("").to_string()  == "Namespace(foo=['const'])");
        REQUIRE(parser2.parse_args("").to_string()  == "Namespace(foo=[''])");
    }

    SECTION("1.25. positional action BooleanOptionalAction without default value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("foo").action(argparse::BooleanOptionalAction);

        REQUIRE(parser.parse_args("").to_string()   == "Namespace(foo=None)");
        REQUIRE_THROWS(parser.parse_args("a"));
    }

    SECTION("1.26. positional action BooleanOptionalAction with default value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);
        parser.add_argument("foo").action(argparse::BooleanOptionalAction);

        REQUIRE(parser.parse_args("").to_string()   == "Namespace(foo='global')");
        REQUIRE_THROWS(parser.parse_args("a"));
    }

    SECTION("1.27. positional action count") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);
        parser.add_argument("foo").action("count");

        REQUIRE(parser.parse_args("").to_string()   == "Namespace(foo=1)");
        REQUIRE_THROWS(parser.parse_args("a"));
        REQUIRE_THROWS(parser.parse_args("a a"));
    }
}
