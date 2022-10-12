/*
* Copyright (c) 2022 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse.hpp>

#include "./catch-define.h"

TEST_CASE("1. utf8 abbreviations", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

    SECTION("1.1. simple count test") {
        parser.add_argument("-α").action(argparse::count);

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<uint32_t>("α") == 0);

        argparse::Namespace args2 = parser.parse_args("-α");
        REQUIRE(args2.get<uint32_t>("α") == 1);

        argparse::Namespace args3 = parser.parse_args("-αα");
        REQUIRE(args3.get<uint32_t>("α") == 2);

        argparse::Namespace args4 = parser.parse_args("-ααα");
        REQUIRE(args4.get<uint32_t>("α") == 3);

        argparse::Namespace args5 = parser.parse_args("-ααα -αα");
        REQUIRE(args5.get<uint32_t>("α") == 5);
    }

    SECTION("1.2. multiargument test") {
        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-β").action(argparse::count);
        parser.add_argument("-γ").action(argparse::count);
        parser.add_argument("-δ").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<uint32_t>("α") == 0);
        REQUIRE(args1.get<uint32_t>("β") == 0);
        REQUIRE(args1.get<uint32_t>("γ") == 0);
        REQUIRE(args1.get<bool>("δ") == false);

        argparse::Namespace args2 = parser.parse_args("-α");
        REQUIRE(args2.get<uint32_t>("α") == 1);
        REQUIRE(args2.get<uint32_t>("β") == 0);
        REQUIRE(args2.get<uint32_t>("γ") == 0);
        REQUIRE(args2.get<bool>("δ") == false);

        argparse::Namespace args3 = parser.parse_args("-αββγα");
        REQUIRE(args3.get<uint32_t>("α") == 2);
        REQUIRE(args3.get<uint32_t>("β") == 2);
        REQUIRE(args3.get<uint32_t>("γ") == 1);
        REQUIRE(args3.get<bool>("δ") == false);

        argparse::Namespace args4 = parser.parse_args("-αααγγ");
        REQUIRE(args4.get<uint32_t>("α") == 3);
        REQUIRE(args4.get<uint32_t>("β") == 0);
        REQUIRE(args4.get<uint32_t>("γ") == 2);
        REQUIRE(args4.get<bool>("δ") == false);

        argparse::Namespace args5 = parser.parse_args("-αδααγ -ββαα");
        REQUIRE(args5.get<uint32_t>("α") == 5);
        REQUIRE(args5.get<uint32_t>("β") == 2);
        REQUIRE(args5.get<uint32_t>("γ") == 1);
        REQUIRE(args5.get<bool>("δ") == true);
    }

    SECTION("1.3. multiargument store test") {
        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-β").action(argparse::store);

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<uint32_t>("α") == 0);
        REQUIRE(args1.get<std::string>("β") == "");

        argparse::Namespace args2 = parser.parse_args("-α");
        REQUIRE(args2.get<uint32_t>("α") == 1);
        REQUIRE(args2.get<std::string>("β") == "");

        argparse::Namespace args3 = parser.parse_args("-αββγα");
        REQUIRE(args3.get<uint32_t>("α") == 1);
        REQUIRE(args3.get<std::string>("β") == "βγα");

        argparse::Namespace args4 = parser.parse_args("-αααββ");
        REQUIRE(args4.get<uint32_t>("α") == 3);
        REQUIRE(args4.get<std::string>("β") == "β");

        REQUIRE_THROWS(parser.parse_args("-αααβ"));
    }

    SECTION("1.4. same prefix store test") {
        parser.add_argument("-α").action(argparse::store);
        parser.add_argument("-αδδ").action(argparse::store);

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<std::string>("α") == "");
        REQUIRE(args1.get<std::string>("αδδ") == "");

        REQUIRE_THROWS(parser.parse_args("-α"));
        REQUIRE_THROWS(parser.parse_args("-αδδ"));

        argparse::Namespace args4 = parser.parse_args("-αδδ1");
        REQUIRE(args4.get<std::string>("α") == "δδ1");
        REQUIRE(args4.get<std::string>("αδδ") == "");

        argparse::Namespace args5 = parser.parse_args("-αδδ=1");
        REQUIRE(args5.get<std::string>("α") == "");
        REQUIRE(args5.get<std::string>("αδδ") == "1");
    }

    SECTION("1.5. same name test (allow_abbrev=true)") {
        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-ααα").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<uint32_t>("α") == 0);
        REQUIRE(args1.get<bool>("ααα") == false);

        argparse::Namespace args2 = parser.parse_args("-α");
        REQUIRE(args2.get<uint32_t>("α") == 1);
        REQUIRE(args2.get<bool>("ααα") == false);

        REQUIRE_THROWS(parser.parse_args("-αα"));

        argparse::Namespace args4 = parser.parse_args("-ααα");
        REQUIRE(args4.get<uint32_t>("α") == 0);
        REQUIRE(args4.get<bool>("ααα") == true);

        argparse::Namespace args5 = parser.parse_args("-αααα");
        REQUIRE(args5.get<uint32_t>("α") == 4);
        REQUIRE(args5.get<bool>("ααα") == false);
    }

    SECTION("1.6. same name test (allow_abbrev=false)") {
        parser.allow_abbrev(false);

        parser.add_argument("-α").action(argparse::count);
        parser.add_argument("-ααα").action(argparse::store_true);

        argparse::Namespace args1 = parser.parse_args("");
        REQUIRE(args1.get<uint32_t>("α") == 0);
        REQUIRE(args1.get<bool>("ααα") == false);

        argparse::Namespace args2 = parser.parse_args("-α");
        REQUIRE(args2.get<uint32_t>("α") == 1);
        REQUIRE(args2.get<bool>("ααα") == false);

        argparse::Namespace args3 = parser.parse_args("-αα");
        REQUIRE(args3.get<uint32_t>("α") == 2);
        REQUIRE(args3.get<bool>("ααα") == false);

        argparse::Namespace args4 = parser.parse_args("-ααα");
        REQUIRE(args4.get<uint32_t>("α") == 0);
        REQUIRE(args4.get<bool>("ααα") == true);

        argparse::Namespace args5 = parser.parse_args("-αααα");
        REQUIRE(args5.get<uint32_t>("α") == 4);
        REQUIRE(args5.get<bool>("ααα") == false);
    }
}
