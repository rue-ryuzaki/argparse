/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

#ifdef ARGPARSE_CXX_11
#include <array>
#include <forward_list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#endif  // C++11+

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

TEST_CASE("1. type name", "[detail]")
{
    SECTION("1.1. current type") {
        CHECK(argparse::detail::Type::name<bool>() == "bool");
        CHECK(argparse::detail::Type::name<short>() == "int");
        CHECK(argparse::detail::Type::name<int>() == "int");
        CHECK(argparse::detail::Type::name<long>() == "int");
        CHECK(argparse::detail::Type::name<long long>() == "int");
        CHECK(argparse::detail::Type::name<char>() == "char");
        CHECK(argparse::detail::Type::name<float>() == "float");
        CHECK(argparse::detail::Type::name<double>() == "float");
        CHECK(argparse::detail::Type::name<std::string>() == "std::string");
#ifdef ARGPARSE_HAS_STRING_VIEW
        CHECK(argparse::detail::Type::name<std::string_view>() == "std::string");
#endif  // ARGPARSE_HAS_STRING_VIEW

#ifdef ARGPARSE_CXX_11
        CHECK(argparse::detail::Type::name<std::array<int, 1> >() == "std::array<int, 1>");
        CHECK(argparse::detail::Type::name<std::forward_list<int> >() == "std::forward_list<int>");
        CHECK(argparse::detail::Type::name<std::tuple<int, int> >() == "std::tuple<int, int>");
        CHECK(argparse::detail::Type::name<std::unordered_map<int, int> >() == "std::unordered_map<int, int>");
        CHECK(argparse::detail::Type::name<std::unordered_multiset<int> >() == "std::unordered_multiset<int>");
        CHECK(argparse::detail::Type::name<std::unordered_set<int> >() == "std::unordered_set<int>");
#endif  // C++11+
        CHECK((argparse::detail::Type::name<std::pair<int, int> >() == "std::pair<int, int>"));
        CHECK((argparse::detail::Type::name<std::map<int, int> >() == "std::map<int, int>"));
        CHECK(argparse::detail::Type::name<std::deque<int> >() == "std::deque<int>");
        CHECK(argparse::detail::Type::name<std::list<int> >() == "std::list<int>");
        CHECK(argparse::detail::Type::name<std::multiset<int> >() == "std::multiset<int>");
        CHECK(argparse::detail::Type::name<std::priority_queue<int> >() == "std::priority_queue<int>");
        CHECK(argparse::detail::Type::name<std::set<int> >() == "std::set<int>");
        CHECK(argparse::detail::Type::name<std::vector<int> >() == "std::vector<int>");
        CHECK(argparse::detail::Type::name<std::stack<int> >() == "std::stack<int>");
        CHECK(argparse::detail::Type::name<std::queue<int> >() == "std::queue<int>");
#ifdef ARGPARSE_HAS_SPAN
        CHECK(argparse::detail::Type::name<std::span<int> >() == "std::span<int>");
#endif  // ARGPARSE_HAS_SPAN

#ifdef ARGPARSE_CXX_11
        CHECK(argparse::detail::Type::name<std::array<std::string, 1> >() == "std::array<std::string, 1>");
        CHECK(argparse::detail::Type::name<std::forward_list<std::string> >() == "std::forward_list<std::string>");
        CHECK(argparse::detail::Type::name<std::tuple<std::string, std::string> >()
                                                                             == "std::tuple<std::string, std::string>");
        CHECK(argparse::detail::Type::name<std::unordered_map<std::string, std::string> >()
                                                                     == "std::unordered_map<std::string, std::string>");
        CHECK(argparse::detail::Type::name<std::unordered_multiset<std::string> >()
                                                                             == "std::unordered_multiset<std::string>");
        CHECK(argparse::detail::Type::name<std::unordered_set<std::string> >() == "std::unordered_set<std::string>");
#endif  // C++11+
        CHECK((argparse::detail::Type::name<std::pair<std::string, std::string> >()
                                                                             == "std::pair<std::string, std::string>"));
        CHECK((argparse::detail::Type::name<std::map<std::string, std::string> >()
                                                                              == "std::map<std::string, std::string>"));
        CHECK(argparse::detail::Type::name<std::deque<std::string> >() == "std::deque<std::string>");
        CHECK(argparse::detail::Type::name<std::list<std::string> >() == "std::list<std::string>");
        CHECK(argparse::detail::Type::name<std::multiset<std::string> >() == "std::multiset<std::string>");
        CHECK(argparse::detail::Type::name<std::priority_queue<std::string> >() == "std::priority_queue<std::string>");
        CHECK(argparse::detail::Type::name<std::set<std::string> >() == "std::set<std::string>");
        CHECK(argparse::detail::Type::name<std::vector<std::string> >() == "std::vector<std::string>");
        CHECK(argparse::detail::Type::name<std::stack<std::string> >() == "std::stack<std::string>");
        CHECK(argparse::detail::Type::name<std::queue<std::string> >() == "std::queue<std::string>");

        CHECK((argparse::detail::Type::name<std::vector<std::pair<int, int> > >()
                                                                                == "std::vector<std::pair<int, int>>"));
        CHECK(argparse::detail::Type::name<std::vector<std::vector<int> > >() == "std::vector<std::vector<int>>");
    }

    SECTION("1.2. basic type") {
        CHECK(argparse::detail::Type::basic<bool>() == "bool");
        CHECK(argparse::detail::Type::basic<short>() == "int");
        CHECK(argparse::detail::Type::basic<int>() == "int");
        CHECK(argparse::detail::Type::basic<long>() == "int");
        CHECK(argparse::detail::Type::basic<long long>() == "int");
        CHECK(argparse::detail::Type::basic<char>() == "char");
        CHECK(argparse::detail::Type::basic<float>() == "float");
        CHECK(argparse::detail::Type::basic<double>() == "float");
        CHECK(argparse::detail::Type::basic<std::string>() == "std::string");
#ifdef ARGPARSE_HAS_STRING_VIEW
        CHECK(argparse::detail::Type::basic<std::string_view>() == "std::string");
#endif  // ARGPARSE_HAS_STRING_VIEW

#ifdef ARGPARSE_CXX_11
        CHECK(argparse::detail::Type::basic<std::array<int, 1> >() == "int");
        CHECK(argparse::detail::Type::basic<std::forward_list<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::tuple<int, int> >() == "std::tuple<int, int>");
        CHECK(argparse::detail::Type::basic<std::unordered_map<int, int> >() == "std::pair<int, int>");
        CHECK(argparse::detail::Type::basic<std::unordered_multiset<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::unordered_set<int> >() == "int");
#endif  // C++11+
        CHECK((argparse::detail::Type::basic<std::pair<int, int> >() == "std::pair<int, int>"));
        CHECK((argparse::detail::Type::basic<std::map<int, int> >() == "std::pair<int, int>"));
        CHECK(argparse::detail::Type::basic<std::deque<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::list<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::multiset<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::priority_queue<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::set<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::vector<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::stack<int> >() == "int");
        CHECK(argparse::detail::Type::basic<std::queue<int> >() == "int");
#ifdef ARGPARSE_HAS_SPAN
        CHECK(argparse::detail::Type::basic<std::span<int> >() == "int");
#endif  // ARGPARSE_HAS_SPAN

#ifdef ARGPARSE_CXX_11
        CHECK(argparse::detail::Type::basic<std::array<std::string, 1> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::forward_list<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::tuple<std::string, std::string> >()
                                                                             == "std::tuple<std::string, std::string>");
        CHECK(argparse::detail::Type::basic<std::unordered_map<std::string, std::string> >()
                                                                              == "std::pair<std::string, std::string>");
        CHECK(argparse::detail::Type::basic<std::unordered_multiset<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::unordered_set<std::string> >() == "std::string");
#endif  // C++11+
        CHECK((argparse::detail::Type::basic<std::pair<std::string, std::string> >()
                                                                             == "std::pair<std::string, std::string>"));
        CHECK((argparse::detail::Type::basic<std::map<std::string, std::string> >()
                                                                             == "std::pair<std::string, std::string>"));
        CHECK(argparse::detail::Type::basic<std::deque<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::list<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::multiset<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::priority_queue<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::set<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::vector<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::stack<std::string> >() == "std::string");
        CHECK(argparse::detail::Type::basic<std::queue<std::string> >() == "std::string");

        CHECK((argparse::detail::Type::basic<std::vector<std::pair<int, int> > >() == "std::pair<int, int>"));
        CHECK(argparse::detail::Type::basic<std::vector<std::vector<int> > >() == "int");
    }
}

TEST_CASE("2. byte type check", "[detail]")
{
    // true
    CHECK(argparse::detail::is_byte_type<char>::value == true);
    CHECK(argparse::detail::is_byte_type<unsigned char>::value == true);
    CHECK(argparse::detail::is_byte_type<signed char>::value == true);
    CHECK(argparse::detail::is_byte_type<int8_t>::value == true);
    CHECK(argparse::detail::is_byte_type<uint8_t>::value == true);
#ifdef ARGPARSE_CXX_17
    CHECK(argparse::detail::is_byte_type<std::byte>::value == true);
#endif  // C++17+
#ifdef ARGPARSE_CXX_20
    CHECK(argparse::detail::is_byte_type<char8_t>::value == true);
#endif  // C++20+
    // false
    CHECK(argparse::detail::is_byte_type<bool>::value == false);
    CHECK(argparse::detail::is_byte_type<int>::value == false);
    CHECK(argparse::detail::is_byte_type<short>::value == false);
    CHECK(argparse::detail::is_byte_type<long>::value == false);
    CHECK(argparse::detail::is_byte_type<double>::value == false);
    CHECK(argparse::detail::is_byte_type<float>::value == false);
    CHECK(argparse::detail::is_byte_type<std::string>::value == false);
}
