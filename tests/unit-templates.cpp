/*
* Copyright (c) 2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include <argparse/argparse_decl.hpp>
#include "./catch-define.h"

#ifdef _ARGPARSE_CXX_11
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

#define AD argparse::detail

#define MACRO_TEST1(L, R) \
    REQUIRE(L <bool> R);\
    REQUIRE(L <char> R);\
    REQUIRE(L <int> R);\
    REQUIRE(L <std::string> R);

#define MACRO_TEST2(L, R) \
    REQUIRE(L <bool, 1> R);\
    REQUIRE(L <char, 1> R);\
    REQUIRE(L <int, 1> R);\
    REQUIRE(L <std::string, 1> R);

#define MACRO_TEST3(L, R) \
    REQUIRE((L <int, bool> R));\
    REQUIRE((L <char, std::string> R));

#define MACRO_SIMPLE(M, R) \
    MACRO_TEST1(AD::M,                                      ::value == R)

#define MACRO_ARRAY(M, R) \
    MACRO_TEST2(AD::M<std::array,                           >::value == R)

#define MACRO_CONTAINER(M, R) \
    MACRO_TEST1(AD::M<std::deque,                           >::value == R)\
    MACRO_TEST1(AD::M<std::list,                            >::value == R)\
    MACRO_TEST1(AD::M<std::priority_queue,                  >::value == R)\
    MACRO_TEST1(AD::M<std::set,                             >::value == R)\
    MACRO_TEST1(AD::M<std::vector,                          >::value == R)

#define MACRO_CONTAINER11(M, R) \
    MACRO_TEST1(AD::M<std::forward_list,                    >::value == R)\
    MACRO_TEST1(AD::M<std::unordered_multiset,              >::value == R)\
    MACRO_TEST1(AD::M<std::unordered_set,                   >::value == R)

#define MACRO_CONTAINER_PAIR(M, R) \
    MACRO_TEST3(AD::M<std::deque<std::pair,                 > >::value == R)\
    MACRO_TEST3(AD::M<std::list<std::pair,                  > >::value == R)\
    MACRO_TEST3(AD::M<std::vector<std::pair,                > >::value == R)

#define MACRO_CONTAINER_PAIR11(M, R) \
    MACRO_TEST3(AD::M<std::forward_list<std::pair,          > >::value == R)\
    MACRO_TEST3(AD::M<std::unordered_multiset<std::pair,    > >::value == R)\
    MACRO_TEST3(AD::M<std::unordered_set<std::pair,         > >::value == R)

#define MACRO_CONTAINER_TUPLE(M, R) \
    MACRO_TEST3(AD::M<std::deque<std::tuple,                > >::value == R)\
    MACRO_TEST3(AD::M<std::list<std::tuple,                 > >::value == R)\
    MACRO_TEST3(AD::M<std::priority_queue<std::tuple,       > >::value == R)\
    MACRO_TEST3(AD::M<std::set<std::tuple,                  > >::value == R)\
    MACRO_TEST3(AD::M<std::vector<std::tuple,               > >::value == R)\
    MACRO_TEST3(AD::M<std::forward_list<std::tuple,         > >::value == R)\
    MACRO_TEST3(AD::M<std::unordered_multiset<std::tuple,   > >::value == R)\
    MACRO_TEST3(AD::M<std::unordered_set<std::tuple,        > >::value == R)

#define MACRO_MAP(M, R) \
    MACRO_TEST3(AD::M<std::map,                             >::value == R)

#define MACRO_MAP11(M, R) \
    MACRO_TEST3(AD::M<std::unordered_map,                   >::value == R)

#define MACRO_QUEUE(M, R) \
    MACRO_TEST1(AD::M<std::queue,                           >::value == R)\
    MACRO_TEST1(AD::M<std::stack,                           >::value == R)

#define MACRO_MATRIX(M, C, R) \
    MACRO_TEST1(AD::M<C<std::deque,                         > >::value == R)\
    MACRO_TEST1(AD::M<C<std::list,                          > >::value == R)\
    MACRO_TEST1(AD::M<C<std::priority_queue,                > >::value == R)\
    MACRO_TEST1(AD::M<C<std::set,                           > >::value == R)\
    MACRO_TEST1(AD::M<C<std::vector,                        > >::value == R)

#define MACRO_MATRIX11(M, C, R) \
    MACRO_TEST1(AD::M<C<std::forward_list,                  > >::value == R)\
    MACRO_TEST1(AD::M<C<std::unordered_multiset,            > >::value == R)\
    MACRO_TEST1(AD::M<C<std::unordered_set,                 > >::value == R)

#define MACRO_MATRIX_QUEUE(M, C, R) \
    MACRO_TEST1(AD::M<C<std::stack,                         > >::value == R)\
    MACRO_TEST1(AD::M<C<std::queue,                         > >::value == R)

TEST_CASE("1. templates", "[detail]")
{
    SECTION("1.1 is_stl_array") {
        bool is_array = true;
        bool is_container = false;
        bool is_container_pair = false;
        bool is_container_tuple = false;
        bool is_map = false;
        bool is_queue = false;
        bool is_matrix = false;
        bool is_matrix_queue = false;

        MACRO_SIMPLE(is_stl_array, false);
        MACRO_CONTAINER(is_stl_array, is_container);
        MACRO_CONTAINER_PAIR(is_stl_array, is_container_pair);
        MACRO_MAP(is_stl_array, is_map);
        MACRO_QUEUE(is_stl_array, is_queue);
        MACRO_MATRIX(is_stl_array, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_array, std::list, is_matrix);
        MACRO_MATRIX(is_stl_array, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_array, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_array, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_array, std::vector, is_matrix_queue);
        // array
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_array, is_array);
        MACRO_CONTAINER11(is_stl_array, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_array, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_array, is_container_tuple);
        MACRO_MAP11(is_stl_array, is_map);
        MACRO_MATRIX11(is_stl_array, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_array, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_array, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.2 is_stl_container") {
        bool is_array = false;
        bool is_container = true;
        bool is_container_pair = true;
        bool is_container_tuple = true;
        bool is_map = false;
        bool is_queue = false;
        bool is_matrix = true;
        bool is_matrix_queue = true;

        MACRO_SIMPLE(is_stl_container, false);
        MACRO_CONTAINER(is_stl_container, is_container);
        MACRO_CONTAINER_PAIR(is_stl_container, is_container_pair);
        MACRO_MAP(is_stl_container, is_map);
        MACRO_QUEUE(is_stl_container, is_queue);
        MACRO_MATRIX(is_stl_container, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_container, std::list, is_matrix);
        MACRO_MATRIX(is_stl_container, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_container, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_container, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_container, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_container, is_array);
        MACRO_CONTAINER11(is_stl_container, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_container, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_container, is_container_tuple);
        MACRO_MAP11(is_stl_container, is_map);
        MACRO_MATRIX11(is_stl_container, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_container, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_container, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.3 is_stl_container_paired") {
        bool is_array = false;
        bool is_container = false;
        bool is_container_pair = true;
        bool is_container_tuple = false;
        bool is_map = false;
        bool is_queue = false;
        bool is_matrix = false;
        bool is_matrix_queue = false;

        MACRO_SIMPLE(is_stl_container_paired, false);
        MACRO_CONTAINER(is_stl_container_paired, is_container);
        MACRO_CONTAINER_PAIR(is_stl_container_paired, is_container_pair);
        MACRO_MAP(is_stl_container_paired, is_map);
        MACRO_QUEUE(is_stl_container_paired, is_queue);
        MACRO_MATRIX(is_stl_container_paired, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_container_paired, std::list, is_matrix);
        MACRO_MATRIX(is_stl_container_paired, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_container_paired, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_container_paired, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_container_paired, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_container_paired, is_array);
        MACRO_CONTAINER11(is_stl_container_paired, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_container_paired, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_container_paired, is_container_tuple);
        MACRO_MAP11(is_stl_container_paired, is_map);
        MACRO_MATRIX11(is_stl_container_paired, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_container_paired, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_container_paired, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.4 is_stl_container_tupled") {
        bool is_array = false;
        bool is_container = false;
        bool is_container_pair = false;
        bool is_container_tuple = true;
        bool is_map = false;
        bool is_queue = false;
        bool is_matrix = false;
        bool is_matrix_queue = false;

        MACRO_SIMPLE(is_stl_container_tupled, false);
        MACRO_CONTAINER(is_stl_container_tupled, is_container);
        MACRO_CONTAINER_PAIR(is_stl_container_tupled, is_container_pair);
        MACRO_MAP(is_stl_container_tupled, is_map);
        MACRO_QUEUE(is_stl_container_tupled, is_queue);
        MACRO_MATRIX(is_stl_container_tupled, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_container_tupled, std::list, is_matrix);
        MACRO_MATRIX(is_stl_container_tupled, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_container_tupled, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_container_tupled, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_container_tupled, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_container_tupled, is_array);
        MACRO_CONTAINER11(is_stl_container_tupled, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_container_tupled, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_container_tupled, is_container_tuple);
        MACRO_MAP11(is_stl_container_tupled, is_map);
        MACRO_MATRIX11(is_stl_container_tupled, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_container_tupled, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_container_tupled, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.5 is_stl_map") {
        bool is_array = false;
        bool is_container = false;
        bool is_container_pair = false;
        bool is_container_tuple = false;
        bool is_map = true;
        bool is_queue = false;
        bool is_matrix = false;
        bool is_matrix_queue = false;

        MACRO_SIMPLE(is_stl_map, false);
        MACRO_CONTAINER(is_stl_map, is_container);
        MACRO_CONTAINER_PAIR(is_stl_map, is_container_pair);
        MACRO_MAP(is_stl_map, is_map);
        MACRO_QUEUE(is_stl_map, is_queue);
        MACRO_MATRIX(is_stl_map, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_map, std::list, is_matrix);
        MACRO_MATRIX(is_stl_map, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_map, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_map, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_map, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_map, is_array);
        MACRO_CONTAINER11(is_stl_map, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_map, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_map, is_container_tuple);
        MACRO_MAP11(is_stl_map, is_map);
        MACRO_MATRIX11(is_stl_map, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_map, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_map, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.6 is_stl_queue") {
        bool is_array = false;
        bool is_container = false;
        bool is_container_pair = false;
        bool is_container_tuple = false;
        bool is_map = false;
        bool is_queue = true;
        bool is_matrix = false;
        bool is_matrix_queue = false;

        MACRO_SIMPLE(is_stl_queue, false);
        MACRO_CONTAINER(is_stl_queue, is_container);
        MACRO_CONTAINER_PAIR(is_stl_queue, is_container_pair);
        MACRO_MAP(is_stl_queue, is_map);
        MACRO_QUEUE(is_stl_queue, is_queue);
        MACRO_MATRIX(is_stl_queue, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_queue, std::list, is_matrix);
        MACRO_MATRIX(is_stl_queue, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_queue, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_queue, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_queue, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_queue, is_array);
        MACRO_CONTAINER11(is_stl_queue, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_queue, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_queue, is_container_tuple);
        MACRO_MAP11(is_stl_queue, is_map);
        MACRO_MATRIX11(is_stl_queue, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_queue, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_queue, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.7 is_stl_matrix") {
        bool is_array = false;
        bool is_container = false;
        bool is_container_pair = false;
        bool is_container_tuple = false;
        bool is_map = false;
        bool is_queue = false;
        bool is_matrix = true;
        bool is_matrix_queue = false;

        MACRO_SIMPLE(is_stl_matrix, false);
        MACRO_CONTAINER(is_stl_matrix, is_container);
        MACRO_CONTAINER_PAIR(is_stl_matrix, is_container_pair);
        MACRO_MAP(is_stl_matrix, is_map);
        MACRO_QUEUE(is_stl_matrix, is_queue);
        MACRO_MATRIX(is_stl_matrix, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_matrix, std::list, is_matrix);
        MACRO_MATRIX(is_stl_matrix, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_matrix, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_matrix, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_matrix, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_matrix, is_array);
        MACRO_CONTAINER11(is_stl_matrix, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_matrix, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_matrix, is_container_tuple);
        MACRO_MAP11(is_stl_matrix, is_map);
        MACRO_MATRIX11(is_stl_matrix, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_matrix, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_matrix, std::vector, is_matrix);
#endif  // C++11+
    }

    SECTION("1.8 is_stl_matrix_queue") {
        bool is_array = false;
        bool is_container = false;
        bool is_container_pair = false;
        bool is_container_tuple = false;
        bool is_map = false;
        bool is_queue = false;
        bool is_matrix = false;
        bool is_matrix_queue = true;

        MACRO_SIMPLE(is_stl_matrix_queue, false);
        MACRO_CONTAINER(is_stl_matrix_queue, is_container);
        MACRO_CONTAINER_PAIR(is_stl_matrix_queue, is_container_pair);
        MACRO_MAP(is_stl_matrix_queue, is_map);
        MACRO_QUEUE(is_stl_matrix_queue, is_queue);
        MACRO_MATRIX(is_stl_matrix_queue, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_matrix_queue, std::list, is_matrix);
        MACRO_MATRIX(is_stl_matrix_queue, std::vector, is_matrix);
        MACRO_MATRIX_QUEUE(is_stl_matrix_queue, std::deque, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_matrix_queue, std::list, is_matrix_queue);
        MACRO_MATRIX_QUEUE(is_stl_matrix_queue, std::vector, is_matrix_queue);
#ifdef _ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_matrix_queue, is_array);
        MACRO_CONTAINER11(is_stl_matrix_queue, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_matrix_queue, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_matrix_queue, is_container_tuple);
        MACRO_MAP11(is_stl_matrix_queue, is_map);
        MACRO_MATRIX11(is_stl_matrix_queue, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_matrix_queue, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_matrix_queue, std::vector, is_matrix);
#endif  // C++11+
    }
}
