/*
 * Copyright (c) 2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

uint16_t const int16_t_max_plus_1 = static_cast<uint16_t>(std::numeric_limits<int16_t>::max()) + 1;

TEST_CASE("1. default_value", "[argument]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. optional action store") {
        parser.add_argument("-f").default_value(argparse::detail::_to_string(int16_t_max_plus_1));

        argparse::Namespace const args = parser.parse_args("");

        CHECK(args.get<uint16_t>("f") == int16_t_max_plus_1);
        CHECK_THROWS(args.get<int16_t>("f"));
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args.try_get<uint16_t>("f").value() == int16_t_max_plus_1);
        CHECK(!args.try_get<int16_t>("f").has_value());
#endif  // ARGPARSE_HAS_OPTIONAL
    }

    SECTION("1.2. optional action count") {
        parser.add_argument("-f").action("count").default_value(argparse::detail::_to_string(int16_t_max_plus_1));

        argparse::Namespace const args = parser.parse_args("");

        CHECK(args.get<uint16_t>("f") == int16_t_max_plus_1);
        CHECK_THROWS(args.get<int16_t>("f"));
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args.try_get<uint16_t>("f").value() == int16_t_max_plus_1);
        CHECK(!args.try_get<int16_t>("f").has_value());
#endif  // ARGPARSE_HAS_OPTIONAL
    }

    SECTION("1.3. optional action count increment") {
        parser.add_argument("-f").action("count").default_value<int16_t>(int16_t_max_plus_1 - 3);

        argparse::Namespace const args2 = parser.parse_args("-ff");
        argparse::Namespace const args3 = parser.parse_args("-fff");

        CHECK(args2.get<int16_t>("f") == int16_t_max_plus_1 - 1);
        CHECK(args3.get<uint16_t>("f") == int16_t_max_plus_1);
        CHECK_THROWS(args3.get<int16_t>("f"));
#ifdef ARGPARSE_HAS_OPTIONAL
        CHECK(args2.try_get<int16_t>("f").value() == int16_t_max_plus_1 - 1);
        CHECK(args3.try_get<uint16_t>("f").value() == int16_t_max_plus_1);
        CHECK(!args3.try_get<int16_t>("f").has_value());
#endif  // ARGPARSE_HAS_OPTIONAL
    }
}

TEST_CASE("2. get | try_get", "[namespace]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("2.1. parse integer value") {
        parser.add_argument("-f");

        argparse::Namespace const args = parser.parse_args("-f=" + argparse::detail::_to_string(int16_t_max_plus_1));

        CHECK(args.get<uint16_t>("f") == int16_t_max_plus_1);
        CHECK_THROWS(args.get<int16_t>("f"));
#ifdef ARGPARSE_HAS_STRING_VIEW
        CHECK(args.try_get<uint16_t>("f").value() == int16_t_max_plus_1);
        CHECK(!args.try_get<int16_t>("f").has_value());
#endif  // ARGPARSE_HAS_STRING_VIEW
    }

    SECTION("2.2. integer count action") {
        parser.add_argument("-f").action("count");
        std::string max_plus_1 = "-" + std::string(int16_t_max_plus_1, 'f');

        argparse::Namespace const args = parser.parse_args(max_plus_1);

        CHECK(args.get<uint16_t>("f") == int16_t_max_plus_1);
        CHECK_THROWS(args.get<int16_t>("f"));
#ifdef ARGPARSE_HAS_STRING_VIEW
        CHECK(args.try_get<uint16_t>("f").value() == int16_t_max_plus_1);
        CHECK(!args.try_get<int16_t>("f").has_value());
#endif  // ARGPARSE_HAS_STRING_VIEW
    }
}
