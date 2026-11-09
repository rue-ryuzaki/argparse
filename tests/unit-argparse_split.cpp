/*
 * Copyright (c) 2021-2026 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#include <map>

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. split to args", "[argparse]")
{
    SECTION("1.1. without quotes") {
        CHECK(argparse::split_to_args("").empty());
        CHECK(argparse::split_to_args("abc").size() == 1);
        CHECK(argparse::split_to_args("abc xyz").size() == 2);
        CHECK(argparse::split_to_args("abc\\ xyz").size() == 1);
        CHECK(argparse::split_to_args("-f=abc xyz").size() == 2);
    }

    SECTION("1.2. with quotes") {
        CHECK(argparse::split_to_args("Homer\\'s dog").size() == 2);
        CHECK(argparse::split_to_args("Homer\\'s\\ dog").size() == 1);
        CHECK(argparse::split_to_args("'Homer\\'s dog'").size() == 1);
        CHECK(argparse::split_to_args("\"Homer's dog\"").size() == 1);
        CHECK(argparse::split_to_args("-f='Homer\\'s dog'").size() == 1);
        CHECK(argparse::split_to_args("-f'Homer\\'s dog'").size() == 1);
        CHECK(argparse::split_to_args("-f=\"Homer's dog\"").size() == 1);
        CHECK(argparse::split_to_args("-f\"Homer's dog\"").size() == 1);
    }
}

TEST_CASE("2. parse_args()", "[ArgumentParser]")
{
    SECTION("2.1. read args from string") {
        argparse::ArgumentParser parser = argparse::ArgumentParser().exit_on_error(false);

        parser.add_argument("-s1").action("append").one_or_more().help("s1 help");
        parser.add_argument("-s2").action("append").one_or_more().help("s2 help");
        parser.add_argument("-s3").action("append").one_or_more().help("s3 help");
        parser.add_argument("-v").action("append").one_or_more().help("v help");
        parser.add_argument("-m1").action("append").one_or_more(2).help("m1 help");
        parser.add_argument("-m2").action("append").one_or_more().help("m2 help");
        parser.add_argument("-m3").action("append").one_or_more().help("m2 help");

        // split string to args: quotes are allowed
        argparse::Namespace args = parser.parse_args(
                    "-s1 1 -s2 '2' -s3 \"3\""
                    " -v 1 '2' \"3\""
                    " -m1 1 1 '2' '2' \"3\" \"3\""
                    " -m2 1:1 '2':'2' \"3\":\"3\""
                    " -m3 1:1 '2:2' \"3:3\""
                    );

        std::vector<std::string> vec2 = args.get<std::vector<std::string> >("v");
        std::map<std::string, std::string> map21 = args.get<std::map<std::string, std::string> >("m1", ' ');
        std::map<std::string, std::string> map22 = args.get<std::map<std::string, std::string> >("m2", ':');
        std::map<std::string, std::string> map23 = args.get<std::map<std::string, std::string> >("m3", ':');

        CHECK(args.get<std::string>("s1") == "1");
        CHECK(args.get<std::string>("s2") == "2");
        CHECK(args.get<std::string>("s3") == "3");
        REQUIRE(vec2.size() == 3);
        CHECK(vec2.at(0) == "1");
        CHECK(vec2.at(1) == "2");
        CHECK(vec2.at(2) == "3");
        REQUIRE(map21.find("1") != map21.end());
        REQUIRE(map21.find("2") != map21.end());
        REQUIRE(map21.find("3") != map21.end());
        CHECK((map21.at("1") == "1"));
        CHECK((map21.at("2") == "2"));
        CHECK((map21.at("3") == "3"));
        REQUIRE(map22.find("1") != map22.end());
        REQUIRE(map22.find("2") != map22.end());
        REQUIRE(map22.find("3") != map22.end());
        CHECK((map22.at("1") == "1"));
        CHECK((map22.at("2") == "2"));
        CHECK((map22.at("3") == "3"));
        REQUIRE(map23.find("1") != map23.end());
        REQUIRE(map23.find("2") != map23.end());
        REQUIRE(map23.find("3") != map23.end());
        CHECK((map23.at("1") == "1"));
        CHECK((map23.at("2") == "2"));
        CHECK((map23.at("3") == "3"));

#ifdef ARGPARSE_HAS_OPTIONAL
        auto opt_vec2 = args.try_get<std::vector<std::string> >("v");
        auto opt_map21 = args.try_get<std::map<std::string, std::string> >("m1", ' ');
        auto opt_map22 = args.try_get<std::map<std::string, std::string> >("m2", ':');
        auto opt_map23 = args.try_get<std::map<std::string, std::string> >("m3", ':');

        CHECK(args.try_get<std::string>("s1").value() == "1");
        CHECK(args.try_get<std::string>("s2").value() == "2");
        CHECK(args.try_get<std::string>("s3").value() == "3");

        REQUIRE(opt_vec2->size() == 3);
        CHECK(opt_vec2->at(0) == "1");
        CHECK(opt_vec2->at(1) == "2");
        CHECK(opt_vec2->at(2) == "3");
        REQUIRE(opt_map21->find("1") != opt_map21->end());
        REQUIRE(opt_map21->find("2") != opt_map21->end());
        REQUIRE(opt_map21->find("3") != opt_map21->end());
        CHECK((opt_map21->at("1") == "1"));
        CHECK((opt_map21->at("2") == "2"));
        CHECK((opt_map21->at("3") == "3"));
        REQUIRE(opt_map22->find("1") != opt_map22->end());
        REQUIRE(opt_map22->find("2") != opt_map22->end());
        REQUIRE(opt_map22->find("3") != opt_map22->end());
        CHECK((opt_map22->at("1") == "1"));
        CHECK((opt_map22->at("2") == "2"));
        CHECK((opt_map22->at("3") == "3"));
        REQUIRE(opt_map23->find("1") != opt_map23->end());
        REQUIRE(opt_map23->find("2") != opt_map23->end());
        REQUIRE(opt_map23->find("3") != opt_map23->end());
        CHECK((opt_map23->at("1") == "1"));
        CHECK((opt_map23->at("2") == "2"));
        CHECK((opt_map23->at("3") == "3"));
#endif  // ARGPARSE_HAS_OPTIONAL
    }
}
