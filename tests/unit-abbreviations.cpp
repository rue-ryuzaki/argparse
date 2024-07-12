/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. abbreviations", "[argument]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. simple count test") {
        parser.add_argument("-c").action(argparse::count);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<uint32_t>("-c") == 0);

        argparse::Namespace args2 = parser.parse_args(_make_vec("-c"));
        REQUIRE(args2.get<uint32_t>("-c") == 1);

        argparse::Namespace args3 = parser.parse_args(_make_vec("-cc"));
        REQUIRE(args3.get<uint32_t>("-c") == 2);

        argparse::Namespace args4 = parser.parse_args(_make_vec("-ccc"));
        REQUIRE(args4.get<uint32_t>("-c") == 3);

        argparse::Namespace args5 = parser.parse_args(_make_vec("-ccc", "-cc"));
        REQUIRE(args5.get<uint32_t>("-c") == 5);
    }

    SECTION("1.2. multiargument test") {
        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-d").action(argparse::count);
        parser.add_argument("-e").action(argparse::count);
        parser.add_argument("-f").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<uint32_t>("-d") == 0);
        REQUIRE(args1.get<uint32_t>("-e") == 0);
        REQUIRE(args1.get<bool>("-f") == false);

        argparse::Namespace args2 = parser.parse_args(_make_vec("-c"));
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<uint32_t>("-d") == 0);
        REQUIRE(args2.get<uint32_t>("-e") == 0);
        REQUIRE(args2.get<bool>("-f") == false);

        argparse::Namespace args3 = parser.parse_args(_make_vec("-cddec"));
        REQUIRE(args3.get<uint32_t>("-c") == 2);
        REQUIRE(args3.get<uint32_t>("-d") == 2);
        REQUIRE(args3.get<uint32_t>("-e") == 1);
        REQUIRE(args3.get<bool>("-f") == false);

        argparse::Namespace args4 = parser.parse_args(_make_vec("-cccee"));
        REQUIRE(args4.get<uint32_t>("-c") == 3);
        REQUIRE(args4.get<uint32_t>("-d") == 0);
        REQUIRE(args4.get<uint32_t>("-e") == 2);
        REQUIRE(args4.get<bool>("-f") == false);

        argparse::Namespace args5 = parser.parse_args(_make_vec("-cfcce", "-ddcc"));
        REQUIRE(args5.get<uint32_t>("-c") == 5);
        REQUIRE(args5.get<uint32_t>("-d") == 2);
        REQUIRE(args5.get<uint32_t>("-e") == 1);
        REQUIRE(args5.get<bool>("-f") == true);
    }

    SECTION("1.3. multiargument store test") {
        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-d").action(argparse::store);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<std::string>("-d") == "");

        argparse::Namespace args2 = parser.parse_args(_make_vec("-c"));
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<std::string>("-d") == "");

        argparse::Namespace args3 = parser.parse_args(_make_vec("-cddec"));
        REQUIRE(args3.get<uint32_t>("-c") == 1);
        REQUIRE(args3.get<std::string>("-d") == "dec");

        argparse::Namespace args4 = parser.parse_args(_make_vec("-cccdd"));
        REQUIRE(args4.get<uint32_t>("-c") == 3);
        REQUIRE(args4.get<std::string>("-d") == "d");

        REQUIRE_THROWS(parser.parse_args(_make_vec("-cccd")));
    }

    SECTION("1.4. same prefix store test") {
        parser.add_argument("-f").action(argparse::store);
        parser.add_argument("-foo").action(argparse::store);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<std::string>("-f") == "");
        REQUIRE(args1.get<std::string>("-foo") == "");

        REQUIRE_THROWS(parser.parse_args(_make_vec("-f")));
        REQUIRE_THROWS(parser.parse_args(_make_vec("-foo")));

        argparse::Namespace args4 = parser.parse_args(_make_vec("-foo1"));
        REQUIRE(args4.get<std::string>("-f") == "oo1");
        REQUIRE(args4.get<std::string>("-foo") == "");

        argparse::Namespace args5 = parser.parse_args(_make_vec("-foo=1"));
        REQUIRE(args5.get<std::string>("-f") == "");
        REQUIRE(args5.get<std::string>("-foo") == "1");
    }

    SECTION("1.5. same name test (allow_abbrev=true)") {
        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-ccc").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<bool>("-ccc") == false);

        argparse::Namespace args2 = parser.parse_args(_make_vec("-c"));
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<bool>("-ccc") == false);

        REQUIRE_THROWS(parser.parse_args(_make_vec("-cc")));

        argparse::Namespace args4 = parser.parse_args(_make_vec("-ccc"));
        REQUIRE(args4.get<uint32_t>("-c") == 0);
        REQUIRE(args4.get<bool>("-ccc") == true);

        argparse::Namespace args5 = parser.parse_args(_make_vec("-cccc"));
        REQUIRE(args5.get<uint32_t>("-c") == 4);
        REQUIRE(args5.get<bool>("-ccc") == false);
    }

    SECTION("1.6. same name test (allow_abbrev=false)") {
        parser.allow_abbrev(false);

        parser.add_argument("-c").action(argparse::count);
        parser.add_argument("-ccc").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args(_make_vec());
        REQUIRE(args1.get<uint32_t>("-c") == 0);
        REQUIRE(args1.get<bool>("-ccc") == false);

        argparse::Namespace args2 = parser.parse_args(_make_vec("-c"));
        REQUIRE(args2.get<uint32_t>("-c") == 1);
        REQUIRE(args2.get<bool>("-ccc") == false);

        argparse::Namespace args3 = parser.parse_args(_make_vec("-cc"));
        REQUIRE(args3.get<uint32_t>("-c") == 2);
        REQUIRE(args3.get<bool>("-ccc") == false);

        argparse::Namespace args4 = parser.parse_args(_make_vec("-ccc"));
        REQUIRE(args4.get<uint32_t>("-c") == 0);
        REQUIRE(args4.get<bool>("-ccc") == true);

        argparse::Namespace args5 = parser.parse_args(_make_vec("-cccc"));
        REQUIRE(args5.get<uint32_t>("-c") == 4);
        REQUIRE(args5.get<bool>("-ccc") == false);
    }
}
