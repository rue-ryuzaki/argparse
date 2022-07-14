/*
* Copyright (c) 2021-2022 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse.hpp>

#include "./catch-define.h"

TEST_CASE("1. split to args", "[detail]")
{
    SECTION("1.1. without quotes") {
        REQUIRE(argparse::detail::_split_to_args("").empty());
        REQUIRE(argparse::detail::_split_to_args("abc").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("abc xyz").size() == 2);
        REQUIRE(argparse::detail::_split_to_args("abc\\ xyz").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("-f=abc xyz").size() == 2);
    }

    SECTION("1.2. with quotes") {
        REQUIRE(argparse::detail::_split_to_args("Homer's dog").size() == 2);
        REQUIRE(argparse::detail::_split_to_args("Homer's\\ dog").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("'Homer's dog'").size() == 1);
        REQUIRE(argparse::detail::_split_to_args("-f='Homer's dog'").size() == 1);
    }
}

TEST_CASE("2. type name", "[detail]")
{
    SECTION("2.1. current type") {
        REQUIRE(argparse::detail::Type::name<int>() == "int");
        REQUIRE(argparse::detail::Type::name<char>() == "char");
        REQUIRE(argparse::detail::Type::name<float>() == "float");
        REQUIRE(argparse::detail::Type::name<double>() == "double");
        REQUIRE(argparse::detail::Type::name<std::string>() == "std::string");

#ifdef _ARGPARSE_CXX_11
        REQUIRE(argparse::detail::Type::name<std::array<int, 1> >() == "std::array<int, 1>");
        REQUIRE(argparse::detail::Type::name<std::forward_list<int> >() == "std::forward_list<int>");
        REQUIRE(argparse::detail::Type::name<std::tuple<int, int> >() == "std::tuple<int, int>");
        REQUIRE(argparse::detail::Type::name<std::unordered_map<int, int> >() == "std::unordered_map<int, int>");
        REQUIRE(argparse::detail::Type::name<std::unordered_multiset<int> >() == "std::unordered_multiset<int>");
        REQUIRE(argparse::detail::Type::name<std::unordered_set<int> >() == "std::unordered_set<int>");
#endif  // C++11+
        REQUIRE((argparse::detail::Type::name<std::pair<int, int> >() == "std::pair<int, int>"));
        REQUIRE((argparse::detail::Type::name<std::map<int, int> >() == "std::map<int, int>"));
        REQUIRE(argparse::detail::Type::name<std::deque<int> >() == "std::deque<int>");
        REQUIRE(argparse::detail::Type::name<std::list<int> >() == "std::list<int>");
        REQUIRE(argparse::detail::Type::name<std::multiset<int> >() == "std::multiset<int>");
        REQUIRE(argparse::detail::Type::name<std::priority_queue<int> >() == "std::priority_queue<int>");
        REQUIRE(argparse::detail::Type::name<std::set<int> >() == "std::set<int>");
        REQUIRE(argparse::detail::Type::name<std::vector<int> >() == "std::vector<int>");
        REQUIRE(argparse::detail::Type::name<std::stack<int> >() == "std::stack<int>");
        REQUIRE(argparse::detail::Type::name<std::queue<int> >() == "std::queue<int>");

#ifdef _ARGPARSE_CXX_11
        REQUIRE(argparse::detail::Type::name<std::array<std::string, 1> >() == "std::array<std::string, 1>");
        REQUIRE(argparse::detail::Type::name<std::forward_list<std::string> >() == "std::forward_list<std::string>");
        REQUIRE(argparse::detail::Type::name<std::tuple<std::string, std::string> >()
                                                                             == "std::tuple<std::string, std::string>");
        REQUIRE(argparse::detail::Type::name<std::unordered_map<std::string, std::string> >()
                                                                     == "std::unordered_map<std::string, std::string>");
        REQUIRE(argparse::detail::Type::name<std::unordered_multiset<std::string> >()
                                                                             == "std::unordered_multiset<std::string>");
        REQUIRE(argparse::detail::Type::name<std::unordered_set<std::string> >() == "std::unordered_set<std::string>");
#endif  // C++11+
        REQUIRE((argparse::detail::Type::name<std::pair<std::string, std::string> >()
                                                                             == "std::pair<std::string, std::string>"));
        REQUIRE((argparse::detail::Type::name<std::map<std::string, std::string> >()
                                                                              == "std::map<std::string, std::string>"));
        REQUIRE(argparse::detail::Type::name<std::deque<std::string> >() == "std::deque<std::string>");
        REQUIRE(argparse::detail::Type::name<std::list<std::string> >() == "std::list<std::string>");
        REQUIRE(argparse::detail::Type::name<std::multiset<std::string> >() == "std::multiset<std::string>");
        REQUIRE(argparse::detail::Type::name<std::priority_queue<std::string> >()
                                                                                 == "std::priority_queue<std::string>");
        REQUIRE(argparse::detail::Type::name<std::set<std::string> >() == "std::set<std::string>");
        REQUIRE(argparse::detail::Type::name<std::vector<std::string> >() == "std::vector<std::string>");
        REQUIRE(argparse::detail::Type::name<std::stack<std::string> >() == "std::stack<std::string>");
        REQUIRE(argparse::detail::Type::name<std::queue<std::string> >() == "std::queue<std::string>");

        REQUIRE((argparse::detail::Type::name<std::vector<std::pair<int, int> > >()
                                                                                == "std::vector<std::pair<int, int>>"));
        REQUIRE(argparse::detail::Type::name<std::vector<std::vector<int> > >() == "std::vector<std::vector<int>>");
    }

    SECTION("2.2. basic type") {
        REQUIRE(argparse::detail::Type::basic<int>() == "int");
        REQUIRE(argparse::detail::Type::basic<char>() == "char");
        REQUIRE(argparse::detail::Type::basic<float>() == "float");
        REQUIRE(argparse::detail::Type::basic<double>() == "double");
        REQUIRE(argparse::detail::Type::basic<std::string>() == "std::string");

#ifdef _ARGPARSE_CXX_11
        REQUIRE(argparse::detail::Type::basic<std::array<int, 1> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::forward_list<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::tuple<int, int> >() == "std::tuple<int, int>");
        REQUIRE(argparse::detail::Type::basic<std::unordered_map<int, int> >() == "std::pair<int, int>");
        REQUIRE(argparse::detail::Type::basic<std::unordered_multiset<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::unordered_set<int> >() == "int");
#endif  // C++11+
        REQUIRE((argparse::detail::Type::basic<std::pair<int, int> >() == "std::pair<int, int>"));
        REQUIRE((argparse::detail::Type::basic<std::map<int, int> >() == "std::pair<int, int>"));
        REQUIRE(argparse::detail::Type::basic<std::deque<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::list<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::multiset<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::priority_queue<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::set<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::vector<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::stack<int> >() == "int");
        REQUIRE(argparse::detail::Type::basic<std::queue<int> >() == "int");

#ifdef _ARGPARSE_CXX_11
        REQUIRE(argparse::detail::Type::basic<std::array<std::string, 1> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::forward_list<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::tuple<std::string, std::string> >()
                                                                             == "std::tuple<std::string, std::string>");
        REQUIRE(argparse::detail::Type::basic<std::unordered_map<std::string, std::string> >()
                                                                              == "std::pair<std::string, std::string>");
        REQUIRE(argparse::detail::Type::basic<std::unordered_multiset<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::unordered_set<std::string> >() == "std::string");
#endif  // C++11+
        REQUIRE((argparse::detail::Type::basic<std::pair<std::string, std::string> >()
                                                                             == "std::pair<std::string, std::string>"));
        REQUIRE((argparse::detail::Type::basic<std::map<std::string, std::string> >()
                                                                             == "std::pair<std::string, std::string>"));
        REQUIRE(argparse::detail::Type::basic<std::deque<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::list<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::multiset<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::priority_queue<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::set<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::vector<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::stack<std::string> >() == "std::string");
        REQUIRE(argparse::detail::Type::basic<std::queue<std::string> >() == "std::string");

        REQUIRE((argparse::detail::Type::basic<std::vector<std::pair<int, int> > >() == "std::pair<int, int>"));
        REQUIRE(argparse::detail::Type::basic<std::vector<std::vector<int> > >() == "int");
    }
}
