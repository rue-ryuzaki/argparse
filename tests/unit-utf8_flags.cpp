/*
 * Copyright (c) 2022-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. utf8 abbreviations", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. simple count test") {
        parser.add_argument("-α").action(argparse::count);

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<uint32_t>("α") == 0);

        argparse::Namespace args2 = parser.parse_args("-α");
        CHECK(args2.get<uint32_t>("α") == 1);

        argparse::Namespace args3 = parser.parse_args("-αα");
        CHECK(args3.get<uint32_t>("α") == 2);

        argparse::Namespace args4 = parser.parse_args("-ααα");
        CHECK(args4.get<uint32_t>("α") == 3);

        argparse::Namespace args5 = parser.parse_args("-ααα -αα");
        CHECK(args5.get<uint32_t>("α") == 5);
    }

    SECTION("1.2. multiargument test") {
        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-β").action(argparse::count);
        parser.add_argument("-γ").action(argparse::count);
        parser.add_argument("-δ").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<uint32_t>("α") == 0);
        CHECK(args1.get<uint32_t>("β") == 0);
        CHECK(args1.get<uint32_t>("γ") == 0);
        CHECK(args1.get<bool>("δ") == false);

        argparse::Namespace args2 = parser.parse_args("-α");
        CHECK(args2.get<uint32_t>("α") == 1);
        CHECK(args2.get<uint32_t>("β") == 0);
        CHECK(args2.get<uint32_t>("γ") == 0);
        CHECK(args2.get<bool>("δ") == false);

        argparse::Namespace args3 = parser.parse_args("-αββγα");
        CHECK(args3.get<uint32_t>("α") == 2);
        CHECK(args3.get<uint32_t>("β") == 2);
        CHECK(args3.get<uint32_t>("γ") == 1);
        CHECK(args3.get<bool>("δ") == false);

        argparse::Namespace args4 = parser.parse_args("-αααγγ");
        CHECK(args4.get<uint32_t>("α") == 3);
        CHECK(args4.get<uint32_t>("β") == 0);
        CHECK(args4.get<uint32_t>("γ") == 2);
        CHECK(args4.get<bool>("δ") == false);

        argparse::Namespace args5 = parser.parse_args("-αδααγ -ββαα");
        CHECK(args5.get<uint32_t>("α") == 5);
        CHECK(args5.get<uint32_t>("β") == 2);
        CHECK(args5.get<uint32_t>("γ") == 1);
        CHECK(args5.get<bool>("δ") == true);
    }

    SECTION("1.3. multiargument store test") {
        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-β").action(argparse::store);

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<uint32_t>("α") == 0);
        CHECK(args1.get<std::string>("β") == "");

        argparse::Namespace args2 = parser.parse_args("-α");
        CHECK(args2.get<uint32_t>("α") == 1);
        CHECK(args2.get<std::string>("β") == "");

        argparse::Namespace args3 = parser.parse_args("-αββγα");
        CHECK(args3.get<uint32_t>("α") == 1);
        CHECK(args3.get<std::string>("β") == "βγα");

        argparse::Namespace args4 = parser.parse_args("-αααββ");
        CHECK(args4.get<uint32_t>("α") == 3);
        CHECK(args4.get<std::string>("β") == "β");

        CHECK_THROWS(parser.parse_args("-αααβ"));
    }

    SECTION("1.4. same prefix store test") {
        parser.add_argument("-α").action(argparse::store);
        parser.add_argument("-αδδ").action(argparse::store);

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<std::string>("α") == "");
        CHECK(args1.get<std::string>("αδδ") == "");

        CHECK_THROWS(parser.parse_args("-α"));
        CHECK_THROWS(parser.parse_args("-αδδ"));

        argparse::Namespace args4 = parser.parse_args("-αδδ1");
        CHECK(args4.get<std::string>("α") == "δδ1");
        CHECK(args4.get<std::string>("αδδ") == "");

        argparse::Namespace args5 = parser.parse_args("-αδδ=1");
        CHECK(args5.get<std::string>("α") == "");
        CHECK(args5.get<std::string>("αδδ") == "1");
    }

    SECTION("1.5. same name test (allow_abbrev=true)") {
        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-ααα").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<uint32_t>("α") == 0);
        CHECK(args1.get<bool>("ααα") == false);

        argparse::Namespace args2 = parser.parse_args("-α");
        CHECK(args2.get<uint32_t>("α") == 1);
        CHECK(args2.get<bool>("ααα") == false);

        CHECK_THROWS(parser.parse_args("-αα"));

        argparse::Namespace args4 = parser.parse_args("-ααα");
        CHECK(args4.get<uint32_t>("α") == 0);
        CHECK(args4.get<bool>("ααα") == true);

        argparse::Namespace args5 = parser.parse_args("-αααα");
        CHECK(args5.get<uint32_t>("α") == 4);
        CHECK(args5.get<bool>("ααα") == false);
    }

    SECTION("1.6. same name test (allow_abbrev=false)") {
        parser.allow_abbrev(false);

        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-ααα").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args("");
        CHECK(args1.get<uint32_t>("α") == 0);
        CHECK(args1.get<bool>("ααα") == false);

        argparse::Namespace args2 = parser.parse_args("-α");
        CHECK(args2.get<uint32_t>("α") == 1);
        CHECK(args2.get<bool>("ααα") == false);

        argparse::Namespace args3 = parser.parse_args("-αα");
        CHECK(args3.get<uint32_t>("α") == 2);
        CHECK(args3.get<bool>("ααα") == false);

        argparse::Namespace args4 = parser.parse_args("-ααα");
        CHECK(args4.get<uint32_t>("α") == 0);
        CHECK(args4.get<bool>("ααα") == true);

        argparse::Namespace args5 = parser.parse_args("-αααα");
        CHECK(args5.get<uint32_t>("α") == 4);
        CHECK(args5.get<bool>("ααα") == false);
    }
}
