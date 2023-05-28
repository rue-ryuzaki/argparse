/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse_decl.hpp>
#include "./catch-define.h"

TEST_CASE("1. default values", "[argument_parser]")
{
    std::string global_default = "global";
    std::string local_default = "local";
    std::string new_default = "new";

    SECTION("1.1. have global value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(local_default).help("bar help");

        REQUIRE(parser.get_default("foo") == global_default);
        REQUIRE(parser.get_default("bar") == local_default);

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<std::string>("foo") == global_default);
        REQUIRE(args0.get<std::string>("bar") == local_default);

#ifdef _ARGPARSE_CXX_11
        parser.set_defaults({ { "foo", new_default } });
#else
        std::vector<std::pair<std::string, std::string> > values1;
        values1.push_back(std::make_pair("foo", new_default));
        parser.set_defaults(values1);
#endif  // C++11+
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<std::string>("foo") == new_default);
        REQUIRE(args1.get<std::string>("bar") == local_default);

#ifdef _ARGPARSE_CXX_11
        parser.set_defaults({ { "foobaz", new_default } });
#else
        std::vector<std::pair<std::string, std::string> > values2;
        values2.push_back(std::make_pair("foobaz", new_default));
        parser.set_defaults(values2);
#endif  // C++11+
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);
        REQUIRE(parser.get_default("foobaz") == new_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec());
        REQUIRE(args2.get<std::string>("foo") == new_default);
        REQUIRE(args2.get<std::string>("bar") == local_default);
        REQUIRE(args2.get<std::string>("foobaz") == new_default);
    }

    SECTION("1.2. no global value") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(local_default).help("bar help");

        REQUIRE(parser.get_default("foo") == "");
        REQUIRE(parser.get_default("bar") == local_default);

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<std::string>("foo") == "");
        REQUIRE(args0.get<std::string>("bar") == local_default);

#ifdef _ARGPARSE_CXX_11
        parser.set_defaults({ { "foo", new_default } });
#else
        std::vector<std::pair<std::string, std::string> > values1;
        values1.push_back(std::make_pair("foo", new_default));
        parser.set_defaults(values1);
#endif  // C++11+
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<std::string>("foo") == new_default);
        REQUIRE(args1.get<std::string>("bar") == local_default);

#ifdef _ARGPARSE_CXX_11
        parser.set_defaults({ { "foobaz", new_default } });
#else
        std::vector<std::pair<std::string, std::string> > values2;
        values2.push_back(std::make_pair("foobaz", new_default));
        parser.set_defaults(values2);
#endif  // C++11+
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);
        REQUIRE(parser.get_default("foobaz") == new_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec());
        REQUIRE(args2.get<std::string>("foo") == new_default);
        REQUIRE(args2.get<std::string>("bar") == local_default);
        REQUIRE(args2.get<std::string>("foobaz") == new_default);
    }

    SECTION("1.3. suppress global value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(argparse::SUPPRESS).exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(local_default).help("bar help");

        REQUIRE(parser.get_default("foo") == argparse::detail::_suppress);
        REQUIRE(parser.get_default("bar") == local_default);

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.exists("foo") == false);
        REQUIRE_THROWS(args0.get<std::string>("foo"));
        REQUIRE(args0.get<std::string>("bar") == local_default);

        argparse::Namespace args1 = parser.parse_args(_make_vec("--foo", new_default));
        REQUIRE(args1.get<std::string>("foo") == new_default);
        REQUIRE(args1.get<std::string>("bar") == local_default);

#ifdef _ARGPARSE_CXX_11
        parser.set_defaults({ { "foo", new_default } });
#else
        std::vector<std::pair<std::string, std::string> > values1;
        values1.push_back(std::make_pair("foo", new_default));
        parser.set_defaults(values1);
#endif  // C++11+
        REQUIRE(parser.get_default("foo") == new_default);
        REQUIRE(parser.get_default("bar") == local_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec());
        REQUIRE(args2.get<std::string>("foo") == new_default);
        REQUIRE(args2.get<std::string>("bar") == local_default);
    }

    SECTION("1.4. suppress local value") {
        argparse::ArgumentParser parser
                = argparse::ArgumentParser().argument_default(global_default).exit_on_error(false);

        parser.add_argument("--foo").action("store").help("foo help");
        parser.add_argument("--bar").action("store").default_value(argparse::SUPPRESS).help("bar help");

        REQUIRE(parser.get_default("foo") == global_default);
        REQUIRE(parser.get_default("bar") == argparse::detail::_suppress);

        argparse::Namespace args0 = parser.parse_args(_make_vec());
        REQUIRE(args0.get<std::string>("foo") == global_default);
        REQUIRE(args0.exists("bar") == false);
        REQUIRE_THROWS(args0.get<std::string>("bar"));

        argparse::Namespace args1 = parser.parse_args(_make_vec("--bar", new_default));
        REQUIRE(args1.get<std::string>("foo") == global_default);
        REQUIRE(args1.get<std::string>("bar") == new_default);

#ifdef _ARGPARSE_CXX_11
        parser.set_defaults({ { "bar", new_default } });
#else
        std::vector<std::pair<std::string, std::string> > values1;
        values1.push_back(std::make_pair("bar", new_default));
        parser.set_defaults(values1);
#endif  // C++11+
        REQUIRE(parser.get_default("foo") == global_default);
        REQUIRE(parser.get_default("bar") == new_default);

        argparse::Namespace args2 = parser.parse_args(_make_vec());
        REQUIRE(args2.get<std::string>("foo") == global_default);
        REQUIRE(args2.get<std::string>("bar") == new_default);
    }
}
