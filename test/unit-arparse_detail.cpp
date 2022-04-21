#include <catch2/catch.hpp>

#include <argparse/argparse.hpp>

TEST_CASE("1. split to args", "[detail]")
{
    SECTION("without quotes") {
        REQUIRE(argparse::detail::_split_to_args("").empty());
        REQUIRE(argparse::detail::_split_to_args("abc").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("abc xyz").size() == 2);
        REQUIRE(argparse::detail::_split_to_args("abc\\ xyz").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("-f=abc xyz").size() == 2);
    }

    SECTION("with quotes") {
        REQUIRE(argparse::detail::_split_to_args("Homer's dog").size() == 2);
        REQUIRE(argparse::detail::_split_to_args("Homer's\\ dog").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("'Homer's dog'").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("-f='Homer's dog'").size() == 1);
    }
}

TEST_CASE("2. type name", "[detail]")
{
    SECTION("current type") {
        REQUIRE(argparse::detail::_type_name<int>() == "int");
        REQUIRE(argparse::detail::_type_name<char>() == "char");
        REQUIRE(argparse::detail::_type_name<float>() == "float");
        REQUIRE(argparse::detail::_type_name<double>() == "double");
        REQUIRE(argparse::detail::_type_name<std::string>() == "std::string");
//        REQUIRE(argparse::detail::_type_name<std::tuple<int, int>>() == "std::tuple<int, int>");
        REQUIRE(argparse::detail::_type_name<std::pair<int, int>>() == "std::pair<int, int>");
        REQUIRE(argparse::detail::_type_name<std::map<int, int>>() == "std::map<int, int>");
        REQUIRE(argparse::detail::_type_name<std::unordered_map<int, int>>() == "std::map<int, int>");
        REQUIRE(argparse::detail::_type_name<std::deque<int>>() == "std::deque<int>");
        REQUIRE(argparse::detail::_type_name<std::forward_list<int>>() == "std::forward_list<int>");
        REQUIRE(argparse::detail::_type_name<std::list<int>>() == "std::list<int>");
        REQUIRE(argparse::detail::_type_name<std::multiset<int>>() == "std::multiset<int>");
        REQUIRE(argparse::detail::_type_name<std::priority_queue<int>>() == "std::priority_queue<int>");
        REQUIRE(argparse::detail::_type_name<std::set<int>>() == "std::set<int>");
        REQUIRE(argparse::detail::_type_name<std::vector<int>>() == "std::vector<int>");
        REQUIRE(argparse::detail::_type_name<std::unordered_multiset<int>>() == "std::unordered_multiset<int>");
        REQUIRE(argparse::detail::_type_name<std::unordered_set<int>>() == "std::unordered_set<int>");
        REQUIRE(argparse::detail::_type_name<std::vector<std::string>>() == "std::vector<std::string>");
        REQUIRE(argparse::detail::_type_name<std::vector<std::pair<int, int>>>() == "std::vector<std::pair<int, int>>");
        REQUIRE(argparse::detail::_type_name<std::vector<std::vector<int>>>() == "std::vector<std::vector<int>>");
    }

    SECTION("basic type") {
        REQUIRE(argparse::detail::_basic_type<int>() == "int");
        REQUIRE(argparse::detail::_basic_type<char>() == "char");
        REQUIRE(argparse::detail::_basic_type<float>() == "float");
        REQUIRE(argparse::detail::_basic_type<double>() == "double");
        REQUIRE(argparse::detail::_basic_type<std::string>() == "std::string");
//        REQUIRE(argparse::detail::_basic_type<std::tuple<int, int>>() == "std::tuple<int, int>");
        REQUIRE(argparse::detail::_basic_type<std::pair<int, int>>() == "std::pair<int, int>");
        REQUIRE(argparse::detail::_basic_type<std::map<int, int>>() == "std::pair<int, int>");
        REQUIRE(argparse::detail::_basic_type<std::unordered_map<int, int>>() == "std::pair<int, int>");
        REQUIRE(argparse::detail::_basic_type<std::deque<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::forward_list<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::list<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::multiset<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::priority_queue<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::set<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::vector<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::unordered_multiset<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::unordered_set<int>>() == "int");
        REQUIRE(argparse::detail::_basic_type<std::vector<std::string>>() == "std::string");
        REQUIRE(argparse::detail::_basic_type<std::vector<std::pair<int, int>>>() == "std::pair<int, int>");
        REQUIRE(argparse::detail::_basic_type<std::vector<std::vector<int>>>() == "int");
    }
}
