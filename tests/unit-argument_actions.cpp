/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. argument actions", "[argument]")
{
    // with argument_default (store_true, store_false, count) actions are invalid in python
    std::string const_value = "const";
    std::string new_value = "new";

    SECTION("1.1. optional arguments") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--store").action(argparse::store);
        parser.add_argument("--store_const").action(argparse::store_const).const_value(const_value);
        parser.add_argument("--store_true").action(argparse::store_true);
        parser.add_argument("--store_false").action(argparse::store_false);
        parser.add_argument("--append").action(argparse::append);
        parser.add_argument("--append_const").action(argparse::append_const).const_value(const_value);
        parser.add_argument("--count").action(argparse::count);
        parser.add_argument("--extend").action(argparse::extend);

        CHECK_THROWS(parser.add_argument(argparse::Argument("--help").action(argparse::help)));
        parser.add_argument(argparse::Argument("--my_help").action(argparse::help));
        parser.add_argument(argparse::Argument("--version").action(argparse::version));

        // no args
        argparse::Namespace args1 = parser.parse_args(_make_vec());
        CHECK(args1.get<std::string>("--store") == "");
        CHECK(args1.get<std::string>("--store_const") == "");
        CHECK(args1.get<bool>("--store_true") == false);
        CHECK(args1.get<bool>("--store_false") == true);
        CHECK(args1.get<std::string>("--append") == "");
        CHECK(args1.get<std::string>("--append_const") == "");
        CHECK(args1.get<std::size_t>("--count") == 0);
        CHECK(args1.get<std::string>("--extend") == "");
        CHECK(args1.get<std::vector<std::string> >("--store").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--append").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--append_const").size() == 0);
        CHECK(args1.get<std::vector<std::string> >("--extend").size() == 0);

        // all args
        argparse::Namespace args2
                = parser.parse_args(
                      _make_vec("--store", new_value, "--store_const", "--store_true", "--store_false",
                                "--append", new_value, "--append_const", "--count", "--extend", new_value));
        CHECK(args2.get<std::string>("--store") == new_value);
        CHECK(args2.get<std::string>("--store_const") == const_value);
        CHECK(args2.get<bool>("--store_true") == true);
        CHECK(args2.get<bool>("--store_false") == false);
        CHECK(args2.get<std::string>("--append") == new_value);  // return array value
        CHECK(args2.get<std::string>("--append_const") == const_value);  // return array value
        CHECK(args2.get<std::size_t>("--count") == 1);
        CHECK(args2.get<std::string>("--extend") == new_value);  // return array value
        CHECK(args2.get<std::vector<std::string> >("--store").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--append").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--append_const").size() == 1);
        CHECK(args2.get<std::vector<std::string> >("--extend").size() == 1);
    }

    SECTION("1.2. positional arguments") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("store").action(argparse::store);
        parser.add_argument("store_const").action(argparse::store_const).const_value(const_value);
        parser.add_argument("store_true").action(argparse::store_true);
        parser.add_argument("store_false").action(argparse::store_false);
        parser.add_argument("append").action(argparse::append);
        parser.add_argument("append_const").action(argparse::append_const).const_value(const_value);
        parser.add_argument("count").action(argparse::count);
        parser.add_argument("extend").action(argparse::extend);

        CHECK_THROWS(parser.add_argument(argparse::Argument("required_true").required(true)));
        CHECK_THROWS(parser.add_argument(argparse::Argument("required_false").required(false)));
        CHECK_THROWS(parser.add_argument(argparse::Argument("dest").dest("dest")));
        CHECK_THROWS(parser.add_argument(argparse::Argument("help").action(argparse::help)));
        CHECK_THROWS(parser.add_argument(argparse::Argument("version").action(argparse::version)));

        CHECK_THROWS(parser.parse_args(_make_vec()));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value)));
        CHECK_THROWS(parser.parse_args(_make_vec(new_value, new_value)));

        argparse::Namespace args = parser.parse_args(_make_vec(new_value, new_value, new_value));
        CHECK(args.get<std::string>("store") == new_value);
        CHECK(args.get<std::string>("store_const") == const_value);
        CHECK(args.get<bool>("store_true") == true);
        CHECK(args.get<bool>("store_false") == false);
        CHECK(args.get<std::string>("append") == new_value);  // return array value
        CHECK(args.get<std::string>("append_const") == const_value);  // return array value
        CHECK(args.get<std::size_t>("count") == 1);
        CHECK(args.get<std::string>("extend") == new_value);  // return array value
        CHECK(args.get<std::vector<std::string> >("store").size() == 1);
        CHECK(args.get<std::vector<std::string> >("append").size() == 1);
        CHECK(args.get<std::vector<std::string> >("append_const").size() == 1);
        CHECK(args.get<std::vector<std::string> >("extend").size() == 1);
    }

    SECTION("1.3. BooleanOptionalAction example") {
        argparse::ArgumentParser parser = argparse::ArgumentParser()
                        .output_width(80).exit_on_error(false).color(false);

        std::string default_value = "default";

        parser.add_argument("--foo").action(argparse::BooleanOptionalAction).default_value(default_value);

        CHECK(parser.format_usage() == "usage: untitled [-h] [--foo | --no-foo]");

        // no args -> empty or default value
        argparse::Namespace args0 = parser.parse_args();

        CHECK(args0.get<std::string>("foo") == default_value);
        CHECK(args0.to_string("foo") == default_value);

        // set --foo -> foo = true
        argparse::Namespace args1 = parser.parse_args("--foo");

        CHECK(args1.get<std::string>("foo") == "1");
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.to_string("foo") == "true");

        // set --no-foo -> foo = false
        argparse::Namespace args2 = parser.parse_args("--no-foo");

        CHECK(args2.get<std::string>("foo") == "");
        CHECK(args2.get<bool>("foo") == false);
        CHECK(args2.to_string("foo") == "false");
    }

    SECTION("1.4. BooleanOptionalAction conflict options [1]") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--foo").action(argparse::BooleanOptionalAction);
        parser.add_argument("--no-bar").action(argparse::store_true);

        CHECK_THROWS(parser.parse_args("--no-"));  // --no-foo or --no-bar?
    }

    SECTION("1.5. BooleanOptionalAction conflict options [2]") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--foo").action(argparse::BooleanOptionalAction);
        REQUIRE_THROWS(parser.add_argument("--no-foo"));
    }

    SECTION("1.6. BooleanOptionalAction conflict options [3]") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--no-foo").action(argparse::store_true);
        REQUIRE_THROWS(parser.add_argument("--foo").action(argparse::BooleanOptionalAction));
    }

    SECTION("1.7. BooleanOptionalAction conflict options resolved [1]") {
        argparse::ArgumentParser parser1 = argparse::ArgumentParser()
                        .conflict_handler("resolve").output_width(80).exit_on_error(false).color(false);

        parser1.add_argument("--foo").action(argparse::BooleanOptionalAction);
        parser1.add_argument("--no-foo").action(argparse::store_true);

        CHECK(parser1.format_usage() == "usage: untitled [-h] [--foo] [--no-foo]");

        argparse::ArgumentParser parser2 = argparse::ArgumentParser()
                        .conflict_handler("resolve").output_width(80).exit_on_error(false).color(false);

        parser2.add_argument("--no-foo").action(argparse::store_true);
        parser2.add_argument("--foo").action(argparse::BooleanOptionalAction);

        CHECK(parser2.format_usage() == "usage: untitled [-h] [--foo | --no-foo]");

        argparse::ArgumentParser parser3 = argparse::ArgumentParser()
                        .conflict_handler("resolve").output_width(80).exit_on_error(false).color(false);

        parser3.add_argument(argparse::Argument(_make_vec("--foo")).action(argparse::BooleanOptionalAction));
        parser3.add_argument(argparse::Argument(_make_vec("--no-foo")).action(argparse::store_true));

        CHECK(parser3.format_usage() == "usage: untitled [-h] [--foo] [--no-foo]");

        argparse::ArgumentParser parser4 = argparse::ArgumentParser()
                        .conflict_handler("resolve").output_width(80).exit_on_error(false).color(false);

        parser4.add_argument(argparse::Argument(_make_vec("--no-foo")).action(argparse::store_true));
        parser4.add_argument(argparse::Argument(_make_vec("--foo")).action(argparse::BooleanOptionalAction));

        CHECK(parser4.format_usage() == "usage: untitled [-h] [--foo | --no-foo]");
    }

    SECTION("1.8. BooleanOptionalAction conflict options resolved [2]") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().conflict_handler("resolve").exit_on_error(false);

        std::string default_value = "default";

        // if 'dest' is not set, you can't get the parsed value of the argument (because of resolve from next argument)
        parser.add_argument("--foo").action(argparse::BooleanOptionalAction).dest("bar").default_value(default_value);
        parser.add_argument("--foo").action("store_true");

        argparse::Namespace args0 = parser.parse_args(_make_vec());

        CHECK(args0.to_string("bar") == default_value);
        CHECK(args0.get<bool>("foo") == false);
        CHECK(args0.to_string("foo") == "false");

        argparse::Namespace args1 = parser.parse_args(_make_vec("--foo"));

        CHECK(args1.to_string("bar") == default_value);
        CHECK(args1.get<bool>("foo") == true);
        CHECK(args1.to_string("foo") == "true");

        argparse::Namespace args2 = parser.parse_args(_make_vec("--no-foo"));

        CHECK(args2.get<bool>("bar") == false);
        CHECK(args2.to_string("bar") == "false");
        CHECK(args2.get<bool>("foo") == false);
        CHECK(args2.to_string("foo") == "false");
    }

    SECTION("1.9. shared argument dest") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        std::string default_value = "default";

        parser.add_argument("--arg").dest("arg").default_value(default_value);
        parser.add_argument("--no-arg").action("store_const").dest("arg").const_value(const_value);

        CHECK(parser.parse_args("").get<std::string>("arg") == default_value);
        CHECK(parser.parse_args("--no-arg").get<std::string>("arg") == const_value);
        CHECK(parser.parse_args("--arg x").get<std::string>("arg") == "x");
        CHECK(parser.parse_args("--arg x --no-arg").get<std::string>("arg") == const_value);
    }

    SECTION("1.10. shared argument dest action count") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--arg").action("count").dest("arg");
        parser.add_argument("--no-arg").action("count").dest("arg");

        CHECK(parser.parse_args("").get<uint32_t>("arg") == 0);
        CHECK(parser.parse_args("--no-arg").get<uint32_t>("arg") == 1);
        CHECK(parser.parse_args("--arg").get<uint32_t>("arg") == 1);
        CHECK(parser.parse_args("--arg --no-arg").get<uint32_t>("arg") == 2);
    }
}
