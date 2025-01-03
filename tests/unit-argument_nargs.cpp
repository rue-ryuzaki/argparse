/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument nargs", "[argument]")
{
    // with argument_default (store_true, store_false, count) actions are invalid in python
    std::string default_value = "local";
    std::string const_value = "const";
    std::string new_value = "new";

    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. nargs break actions") {
        CHECK_THROWS(parser.add_argument("--store_const?").action(argparse::store_const)
                                                                                  .const_value(const_value).nargs("?"));
        CHECK_THROWS(parser.add_argument("--store_const*").action(argparse::store_const)
                                                                                  .const_value(const_value).nargs("*"));
        CHECK_THROWS(parser.add_argument("--store_const+").action(argparse::store_const)
                                                                                  .const_value(const_value).nargs("+"));
        CHECK_THROWS(parser.add_argument("--store_constN").action(argparse::store_const)
                                                                                    .const_value(const_value).nargs(2));
        CHECK_THROWS(parser.add_argument("--store_true?").action(argparse::store_true).nargs("?"));
        CHECK_THROWS(parser.add_argument("--store_true*").action(argparse::store_true).nargs("*"));
        CHECK_THROWS(parser.add_argument("--store_true+").action(argparse::store_true).nargs("+"));
        CHECK_THROWS(parser.add_argument("--store_trueN").action(argparse::store_true).nargs(2));
        CHECK_THROWS(parser.add_argument("--store_false?").action(argparse::store_false).nargs("?"));
        CHECK_THROWS(parser.add_argument("--store_false*").action(argparse::store_false).nargs("*"));
        CHECK_THROWS(parser.add_argument("--store_false+").action(argparse::store_false).nargs("+"));
        CHECK_THROWS(parser.add_argument("--store_falseN").action(argparse::store_false).nargs(2));
        CHECK_THROWS(parser.add_argument("--append_const?").action(argparse::append_const)
                                                                                  .const_value(const_value).nargs("?"));
        CHECK_THROWS(parser.add_argument("--append_const*").action(argparse::append_const)
                                                                                  .const_value(const_value).nargs("*"));
        CHECK_THROWS(parser.add_argument("--append_const+").action(argparse::append_const)
                                                                                  .const_value(const_value).nargs("+"));
        CHECK_THROWS(parser.add_argument("--append_constN").action(argparse::append_const)
                                                                                    .const_value(const_value).nargs(2));
        CHECK_THROWS(parser.add_argument("--count?").action(argparse::count).nargs("?"));
        CHECK_THROWS(parser.add_argument("--count*").action(argparse::count).nargs("*"));
        CHECK_THROWS(parser.add_argument("--count+").action(argparse::count).nargs("+"));
        CHECK_THROWS(parser.add_argument("--countN").action(argparse::count).nargs(2));

        CHECK_THROWS(parser.add_argument("store_const?").action(argparse::store_const)
                                                                                  .const_value(const_value).nargs("?"));
        CHECK_THROWS(parser.add_argument("store_const*").action(argparse::store_const)
                                                                                  .const_value(const_value).nargs("*"));
        CHECK_THROWS(parser.add_argument("store_const+").action(argparse::store_const)
                                                                                  .const_value(const_value).nargs("+"));
        CHECK_THROWS(parser.add_argument("store_constN").action(argparse::store_const)
                                                                                    .const_value(const_value).nargs(2));
        CHECK_THROWS(parser.add_argument("store_true?").action(argparse::store_true).nargs("?"));
        CHECK_THROWS(parser.add_argument("store_true*").action(argparse::store_true).nargs("*"));
        CHECK_THROWS(parser.add_argument("store_true+").action(argparse::store_true).nargs("+"));
        CHECK_THROWS(parser.add_argument("store_trueN").action(argparse::store_true).nargs(2));
        CHECK_THROWS(parser.add_argument("store_false?").action(argparse::store_false).nargs("?"));
        CHECK_THROWS(parser.add_argument("store_false*").action(argparse::store_false).nargs("*"));
        CHECK_THROWS(parser.add_argument("store_false+").action(argparse::store_false).nargs("+"));
        CHECK_THROWS(parser.add_argument("store_falseN").action(argparse::store_false).nargs(2));
        CHECK_THROWS(parser.add_argument("append_const?").action(argparse::append_const)
                                                                                  .const_value(const_value).nargs("?"));
        CHECK_THROWS(parser.add_argument("append_const*").action(argparse::append_const)
                                                                                  .const_value(const_value).nargs("*"));
        CHECK_THROWS(parser.add_argument("append_const+").action(argparse::append_const)
                                                                                  .const_value(const_value).nargs("+"));
        CHECK_THROWS(parser.add_argument("append_constN").action(argparse::append_const)
                                                                                    .const_value(const_value).nargs(2));
        CHECK_THROWS(parser.add_argument("count?").action(argparse::count).nargs("?"));
        CHECK_THROWS(parser.add_argument("count*").action(argparse::count).nargs("*"));
        CHECK_THROWS(parser.add_argument("count+").action(argparse::count).nargs("+"));
        CHECK_THROWS(parser.add_argument("countN").action(argparse::count).nargs(2));
    }

    SECTION("1.2. nargs ?") {
        parser.add_argument("--foo").nargs("?").const_value("c").default_value("d");
        parser.add_argument("bar").nargs("?").default_value("d");
        parser.add_argument("--store");

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("foo") == "d");
        CHECK(args1.get<std::string>("bar") == "d");

        argparse::Namespace args2 = parser.parse_args(_make_vec("XX", "--foo"));
        CHECK(args2.get<std::string>("foo") == "c");
        CHECK(args2.get<std::string>("bar") == "XX");

        CHECK_THROWS(parser.parse_args(_make_vec("XX", "--foo", "--store")));

        argparse::Namespace args3 = parser.parse_args(_make_vec("XX", "--foo", "--store", "store"));
        CHECK(args3.get<std::string>("foo") == "c");
        CHECK(args3.get<std::string>("bar") == "XX");

        CHECK_THROWS(parser.parse_args(_make_vec("XX", "YY")));

        argparse::Namespace args4 = parser.parse_args(_make_vec("XX", "--foo", "YY"));
        CHECK(args4.get<std::string>("foo") == "YY");
        CHECK(args4.get<std::string>("bar") == "XX");
    }

    SECTION("1.3. nargs ? optional") {
        parser.add_argument("--store").action(argparse::store).nargs("?").default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("--append").action(argparse::append).nargs("?");
        // default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs("?");

        // no args
        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--store") == default_value);
        CHECK(args1.get<std::string>("--append") == "");
        CHECK(args1.get<std::string>("--extend") == "");
        CHECK(args1.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("--append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--extend").size() == 0);

        CHECK_THROWS(parser.parse_args(_make_vec("--store", new_value, new_value)));

        // all args
        argparse::Namespace args2
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, "--extend", new_value));
        CHECK(args2.get<std::string>("--store") == new_value);
        CHECK(args2.get<std::string>("--append") == new_value);  // return array value
        CHECK(args2.get<std::string>("--extend") == new_value);  // return array value
        CHECK(args2.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--append").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--extend").size() == 1);

        // override args
        argparse::Namespace args3
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, "--extend", new_value,
                                              "--store", new_value, "--append", new_value, "--extend", new_value));
        CHECK(args3.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("--append").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("--extend").size() == 2);
    }

    SECTION("1.4. nargs ? positional") {
        parser.add_argument("store").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("append").action(argparse::append).nargs("?");
//        parser.add_argument({ "extend" }).action(argparse::extend).nargs("?");  // invalid in python without arguments

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("store") == default_value);
        CHECK(args1.get<std::string>("append") == "");  // return array value
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("append").size() == 0);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value));
        CHECK(args2.get<std::string>("store") == new_value);
        CHECK(args2.get<std::string>("append") == "");  // return array value
        CHECK(args2.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("append").size() == 0);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value));
        CHECK(args3.get<std::string>("store") == new_value);
        CHECK(args3.get<std::string>("append") == new_value);  // return array value
        CHECK(args3.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("append").size() == 1);

        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value)));
    }

    SECTION("1.5. nargs * optional") {
        parser.add_argument("--store").action(argparse::store).nargs("*").default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("--append").action(argparse::append).nargs("*");
        // default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs("*");

        // no args
        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--store") == default_value);
        CHECK(args1.get<std::string>("--append") == "");
        CHECK(args1.get<std::string>("--extend") == "");
        CHECK(args1.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("--append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        argparse::Namespace args2
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, "--extend", new_value));
        CHECK(args2.get<std::string>("--store") == new_value);
        CHECK(args2.get<std::string>("--append") == new_value);  // return array value
        CHECK(args2.get<std::string>("--extend") == new_value);  // return array value
        CHECK(args2.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--append").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--extend").size() == 1);

        // override args
        argparse::Namespace args3
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, "--extend", new_value,
                                              "--store", new_value, new_value, "--append", new_value, new_value,
                                              "--extend", new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("--store").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("--append").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("--extend").size() == 3);
    }

    SECTION("1.6. nargs * positional") {
        parser.add_argument("store").action(argparse::store).nargs("*").default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("append").action(argparse::append).nargs("*");
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("*");

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("store") == default_value);
        CHECK(args1.get<std::string>("append") == "");
        CHECK(args1.get<std::string>("extend") == "");
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("extend").size() == 0);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("store").size() == 4);
        CHECK(args2.get<std::vector<std::string> >("append").size() == 0);
        CHECK(args2.get<std::vector<std::string> >("extend").size() == 0);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("store").size() == 5);
        CHECK(args3.get<std::vector<std::string> >("append").size() == 0);
        CHECK(args3.get<std::vector<std::string> >("extend").size() == 0);
    }

    SECTION("1.7. nargs * positional [2]") {
        // default value are invalid in python if flag used
        parser.add_argument("append").action(argparse::append).nargs("*");
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("*");
        parser.add_argument("store").action(argparse::store).nargs("*").default_value(default_value);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("store") == default_value);
        CHECK(args1.get<std::string>("append") == "");
        CHECK(args1.get<std::string>("extend") == "");
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("extend").size() == 0);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args2.get<std::string>("store") == default_value);
        CHECK(args2.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("append").size() == 4);
        CHECK(args2.get<std::vector<std::string> >("extend").size() == 0);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::string>("store") == default_value);
        CHECK(args3.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("append").size() == 5);
        CHECK(args3.get<std::vector<std::string> >("extend").size() == 0);
    }

    SECTION("1.8. nargs + optional") {
        parser.add_argument("--store").action(argparse::store).nargs("+").default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("--append").action(argparse::append).nargs("+");
        // default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs("+");

        // no args
        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--store") == default_value);
        CHECK(args1.get<std::string>("--append") == "");
        CHECK(args1.get<std::string>("--extend") == "");
        CHECK(args1.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("--append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        argparse::Namespace args2
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, "--extend", new_value));
        CHECK(args2.get<std::string>("--store") == new_value);
        CHECK(args2.get<std::string>("--append") == new_value);  // return array value
        CHECK(args2.get<std::string>("--extend") == new_value);  // return array value
        CHECK(args2.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--append").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--extend").size() == 1);

        // override args
        argparse::Namespace args3
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, "--extend", new_value,
                                              "--store", new_value, new_value, "--append", new_value, new_value,
                                              "--extend", new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("--store").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("--append").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("--extend").size() == 3);
    }

    SECTION("1.9. nargs + positional") {
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("append").action(argparse::append).nargs("+");
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("+");

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args1.get<std::string>("store") == new_value);
        CHECK(args1.get<std::string>("append") == new_value);
        CHECK(args1.get<std::string>("extend") == new_value);
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("append").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("extend").size() == 1);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("store").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("append").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("extend").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("store").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("append").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("extend").size() == 1);
    }

    SECTION("1.10. nargs + positional [2]") {
        // default value are invalid in python if flag used
        parser.add_argument("append").action(argparse::append).nargs("+");
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("+");
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args1.get<std::string>("append") == new_value);
        CHECK(args1.get<std::string>("extend") == new_value);
        CHECK(args1.get<std::string>("store") == new_value);
        CHECK(args1.get<std::vector<std::string> >("append").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("extend").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("append").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("extend").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("append").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("extend").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("store").size() == 1);
    }

    SECTION("1.11. nargs mixed positional") {
        // default value are invalid in python if flag used
        parser.add_argument("append").action(argparse::append).nargs("+");
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs("*");
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value));
        CHECK(args1.get<std::string>("append") == new_value);
        CHECK(args1.get<std::string>("extend") == "");
        CHECK(args1.get<std::string>("store") == new_value);
        CHECK(args1.get<std::vector<std::string> >("append").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("extend").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("append").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("extend").size() == 0);
        CHECK(args2.get<std::vector<std::string> >("store").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("append").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("extend").size() == 0);
        CHECK(args3.get<std::vector<std::string> >("store").size() == 1);
    }

    SECTION("1.12. nargs mixed positional [2]") {
        // default value are invalid in python if flag used
        parser.add_argument("append1").action(argparse::append).nargs("+");
        // default value are invalid in python if flag used
        parser.add_argument("append2").action(argparse::append).nargs("?");
        parser.add_argument("store").action(argparse::store).nargs("+").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value));
        CHECK(args1.get<std::string>("append1") == new_value);
        CHECK(args1.get<std::string>("append2") == "");
        CHECK(args1.get<std::string>("store") == new_value);
        CHECK(args1.get<std::vector<std::string> >("append1").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("append2").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("store").size() == 1);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("append1").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("append2").size() == 0);
        CHECK(args2.get<std::vector<std::string> >("store").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("append1").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("append2").size() == 0);
        CHECK(args3.get<std::vector<std::string> >("store").size() == 1);
    }

    SECTION("1.13. nargs mixed positional [3]") {
        parser.add_argument("store1").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("+").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value));
        CHECK(args1.get<std::string>("store1") == default_value);
        CHECK(args1.get<std::string>("store2") == default_value);
        CHECK(args1.get<std::string>("store3") == new_value);
        CHECK(args1.get<std::string>("store4") == default_value);
        CHECK(args1.get<std::string>("store5") == default_value);
        CHECK(args1.get<std::string>("store6") == new_value);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args2.get<std::string>("store1") == new_value);
        CHECK(args2.get<std::string>("store2") == default_value);
        CHECK(args2.get<std::string>("store3") == new_value);
        CHECK(args2.get<std::string>("store4") == default_value);
        CHECK(args2.get<std::string>("store5") == default_value);
        CHECK(args2.get<std::string>("store6") == new_value);
        CHECK(args2.get<std::vector<std::string> >("store1").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store3").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store6").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::string>("store2") == default_value);
        CHECK(args3.get<std::string>("store3") == new_value);
        CHECK(args3.get<std::string>("store4") == default_value);
        CHECK(args3.get<std::string>("store5") == default_value);
        CHECK(args3.get<std::string>("store6") == new_value);
        CHECK(args3.get<std::vector<std::string> >("store1").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("store3").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("store6").size() == 1);
    }

    SECTION("1.14. nargs mixed positional [4]") {
        parser.add_argument("store1").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs("?").default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("*").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value));
        CHECK(args1.get<std::string>("store1") == new_value);
        CHECK(args1.get<std::string>("store2") == default_value);
        CHECK(args1.get<std::string>("store3") == default_value);
        CHECK(args1.get<std::string>("store4") == new_value);
        CHECK(args1.get<std::string>("store5") == default_value);
        CHECK(args1.get<std::string>("store6") == default_value);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args2.get<std::string>("store2") == default_value);
        CHECK(args2.get<std::string>("store3") == default_value);
        CHECK(args2.get<std::string>("store4") == new_value);
        CHECK(args2.get<std::string>("store5") == default_value);
        CHECK(args2.get<std::string>("store6") == default_value);
        CHECK(args2.get<std::vector<std::string> >("store1").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("store4").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::string>("store2") == default_value);
        CHECK(args3.get<std::string>("store3") == default_value);
        CHECK(args3.get<std::string>("store4") == new_value);
        CHECK(args3.get<std::string>("store5") == default_value);
        CHECK(args3.get<std::string>("store6") == default_value);
        CHECK(args3.get<std::vector<std::string> >("store1").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("store4").size() == 1);
    }

    SECTION("1.15. nargs mixed positional [5]") {
        parser.add_argument("store1").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("*").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value,
                                                                new_value, new_value));
        CHECK(args1.get<std::string>("store1") == new_value);
        CHECK(args1.get<std::string>("store3") == default_value);
        CHECK(args1.get<std::string>("store4") == new_value);
        CHECK(args1.get<std::string>("store6") == default_value);
        CHECK(args1.get<std::vector<std::string> >("store1").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("store2").size() == 2);
        CHECK(args1.get<std::vector<std::string> >("store4").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("store5").size() == 2);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value,
                                                                new_value, new_value, new_value));
        CHECK(args2.get<std::string>("store3") == default_value);
        CHECK(args2.get<std::string>("store4") == new_value);
        CHECK(args2.get<std::string>("store6") == default_value);
        CHECK(args2.get<std::vector<std::string> >("store1").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("store2").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("store4").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store5").size() == 2);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value,
                                                                new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::string>("store3") == default_value);
        CHECK(args3.get<std::string>("store4") == new_value);
        CHECK(args3.get<std::string>("store6") == default_value);
        CHECK(args3.get<std::vector<std::string> >("store1").size() == 3);
        CHECK(args3.get<std::vector<std::string> >("store2").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("store4").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("store5").size() == 2);
    }

    SECTION("1.16. nargs mixed positional [6]") {
        parser.add_argument("store1").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store2").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store3").action(argparse::store).nargs("+").default_value(default_value);
        parser.add_argument("store4").action(argparse::store).nargs("*").default_value(default_value);
        parser.add_argument("store5").action(argparse::store).nargs(2).default_value(default_value);
        parser.add_argument("store6").action(argparse::store).nargs("+").default_value(default_value);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value)));

        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value,
                                                                new_value, new_value));
        CHECK(args1.get<std::string>("store1") == default_value);
        CHECK(args1.get<std::string>("store3") == new_value);
        CHECK(args1.get<std::string>("store4") == default_value);
        CHECK(args1.get<std::string>("store6") == new_value);
        CHECK(args1.get<std::vector<std::string> >("store2").size() == 2);
        CHECK(args1.get<std::vector<std::string> >("store3").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("store5").size() == 2);
        CHECK(args1.get<std::vector<std::string> >("store6").size() == 1);

        argparse::Namespace args2 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value,
                                                                new_value, new_value, new_value));
        CHECK(args2.get<std::string>("store1") == new_value);
        CHECK(args2.get<std::string>("store3") == new_value);
        CHECK(args2.get<std::string>("store4") == default_value);
        CHECK(args2.get<std::string>("store6") == new_value);
        CHECK(args2.get<std::vector<std::string> >("store1").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store2").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("store3").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store5").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("store6").size() == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value,
                                                                new_value, new_value, new_value, new_value));
        CHECK(args3.get<std::string>("store3") == new_value);
        CHECK(args3.get<std::string>("store4") == default_value);
        CHECK(args3.get<std::string>("store6") == new_value);
        CHECK(args3.get<std::vector<std::string> >("store1").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("store2").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("store3").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("store5").size() == 2);
        CHECK(args3.get<std::vector<std::string> >("store6").size() == 1);
    }

    SECTION("1.17. nargs N optional") {
        parser.add_argument("--store").action(argparse::store).nargs(1).default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("--append").action(argparse::append).nargs(2);
        // default value are invalid in python if flag used
        parser.add_argument("--extend").action(argparse::extend).nargs(3);

        // no args
        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--store") == default_value);
        CHECK(args1.get<std::string>("--append") == "");
        CHECK(args1.get<std::string>("--extend") == "");
        CHECK(args1.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("--append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        argparse::Namespace args2 = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, new_value,
                                                                "--extend", new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--append").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("--extend").size() == 3);

        CHECK_THROWS(parser.parse_args(
                           _make_vec("--store", new_value, "--append", new_value, "--extend", new_value)));

        // override args
        argparse::Namespace args3
                = parser.parse_args(_make_vec("--store", new_value, "--append", new_value, new_value,
                                              "--extend", new_value, new_value, new_value,
                                              "--store", new_value, "--append", new_value, new_value,
                                              "--extend", new_value, new_value, new_value));
        CHECK(args3.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args3.get<std::vector<std::string> >("--append").size() == 4);
        CHECK(args3.get<std::vector<std::string> >("--extend").size() == 6);
    }

    SECTION("1.18. nargs N positional") {
        parser.add_argument("store").action(argparse::store).nargs(1).default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("append").action(argparse::append).nargs(2);
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs(3);

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value, new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value)));

        argparse::Namespace args
                = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value, new_value, new_value));
        CHECK(args.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args.get<std::vector<std::string> >("append").size() == 2);
        CHECK(args.get<std::vector<std::string> >("extend").size() == 3);

        CHECK_THROWS(parser.parse_args(
                           _make_vec(new_value, new_value, new_value, new_value, new_value, new_value, new_value)));
    }

    SECTION("1.19. nargs N optional + positional") {
        parser.add_argument("--store").action(argparse::store).nargs(1).default_value(default_value);
        // default value are invalid in python if flag used
        parser.add_argument("store").action(argparse::store).nargs(2);
        // default value are invalid in python if flag used
        parser.add_argument("extend").action(argparse::extend).nargs(2);

        CHECK_THROWS(parser.parse_args(_make_vec(new_value, "--store", new_value, new_value,
                                                   new_value, new_value, new_value, new_value)));

        // no args
        argparse::Namespace args1 = parser.parse_args(_make_vec(new_value, new_value, new_value, new_value));
        CHECK(args1.get<std::string>("--store") == default_value);
        CHECK(args1.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args1.get<std::vector<std::string> >("store").size() == 2);
        CHECK(args1.get<std::vector<std::string> >("extend").size() == 2);

        // all args
        argparse::Namespace args2
                = parser.parse_args(_make_vec("--store", new_value, new_value, new_value, new_value, new_value));
        CHECK(args2.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("store").size() == 2);
        CHECK(args2.get<std::vector<std::string> >("extend").size() == 2);
    }

    SECTION("1.20. optional * remainder") {
        parser.add_argument("baz").nargs("*").default_value("123");
        parser.add_argument("args").remainder().default_value("12345");
        parser.add_argument("--bar");

        CHECK(parser.parse_args("").to_string()          == "Namespace(baz='123', args=[], bar=None)");
        CHECK(parser.parse_args("a b c d e").to_string()
                                                      == "Namespace(baz=['a', 'b', 'c', 'd', 'e'], args=[], bar=None)");
        CHECK(parser.parse_args("--bar c").to_string()   == "Namespace(baz='123', args=[], bar='c')");
        CHECK(parser.parse_args("--bar c d").to_string() == "Namespace(baz=['d'], args=[], bar='c')");

        // in original argparse will be - "Namespace(baz=['a'], args=['--bar', 'c'], bar=None)"
        CHECK(parser.parse_args("a --bar c").to_string() == "Namespace(baz=['a'], args=[], bar='c')");
    }

    SECTION("1.21. optional remainder *") {
        parser.add_argument("args").remainder().default_value("12345");
        parser.add_argument("baz").nargs("*").default_value("123");
        parser.add_argument("--bar");

        CHECK(parser.parse_args("").to_string()          == "Namespace(args=[], baz='123', bar=None)");
        CHECK(parser.parse_args("a b c d e").to_string()
                                                   == "Namespace(args=['a', 'b', 'c', 'd', 'e'], baz='123', bar=None)");
        CHECK(parser.parse_args("--bar c").to_string()   == "Namespace(args=[], baz='123', bar='c')");
        CHECK(parser.parse_args("--bar c d").to_string() == "Namespace(args=['d'], baz='123', bar='c')");
        CHECK(parser.parse_args("a --bar c").to_string() == "Namespace(args=['a', '--bar', 'c'], baz='123', bar=None)");
    }
}
