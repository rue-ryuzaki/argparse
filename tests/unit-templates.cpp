/*
 * Copyright (c) 2024-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

#ifdef ARGPARSE_CXX_11
#include <array>
#include <forward_list>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

inline std::size_t
hash_combiner(
        std::size_t left,
        std::size_t right)
{
    return left ^ right;
}

template <class T, class U>
struct std::hash<std::pair<T, U> >
{
    inline size_t
    operator() (
            std::pair<T, U> const& key) const
    {
        return hash_combiner(std::hash<T>()(key.first), std::hash<U>()(key.second));
    }
};

template <std::size_t Idx, class... Ts>
struct hash_impl
{
    inline std::size_t
    operator() (
            std::size_t a,
            std::tuple<Ts...> const& t) const
    {
        typedef typename std::tuple_element<Idx - 1, std::tuple<Ts...> >::type nexttype;
        hash_impl<Idx - 2, Ts...> next;
        std::size_t b = std::hash<nexttype>()(std::get<Idx - 1>(t));
        return next(hash_combiner(a, b), t);
    }
};

template <class... Ts>
struct hash_impl<1, Ts...>
{
    inline std::size_t
    operator() (
            std::size_t a,
            std::tuple<Ts...> const& t) const
    {
        typedef typename std::tuple_element<0, std::tuple<Ts...> >::type nexttype;
        std::size_t b = std::hash<nexttype>()(std::get<0>(t));
        return hash_combiner(a, b);
    }
};

template <class... Ts>
struct hash_impl<0, Ts...>
{
    inline std::size_t
    operator() (
            std::size_t a,
            std::tuple<Ts...> const&) const
    {
        return a;
    }
};

template <class... Ts>
struct std::hash<std::tuple<Ts...> >
{
    inline std::size_t
    operator() (
            std::tuple<Ts...> const& x) const
    {
        return hash_impl<std::tuple_size<std::tuple<Ts...> >::value, Ts...>()(0, x);
    }
};
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
    CHECK(L <bool> R);\
    CHECK(L <char> R);\
    CHECK(L <int> R);\
    CHECK(L <std::string> R);

#define MACRO_TEST2(L, R) \
    CHECK(L <bool, 1> R);\
    CHECK(L <char, 1> R);\
    CHECK(L <int, 1> R);\
    CHECK(L <std::string, 1> R);

#define MACRO_TEST3(L, R) \
    CHECK((L <int, bool> R));\
    CHECK((L <char, std::string> R));

#define MACRO_SIMPLE(M, R) \
    MACRO_TEST1(AD::M,                                      ::value == R)

#define MACRO_ARRAY(M, R) \
    MACRO_TEST2(AD::M<std::array,                           >::value == R)

#define MACRO_CONTAINER(M, R) \
    MACRO_TEST1(AD::M<std::deque,                           >::value == R)\
    MACRO_TEST1(AD::M<std::list,                            >::value == R)\
    MACRO_TEST1(AD::M<std::multiset,                        >::value == R)\
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
    MACRO_TEST3(AD::M<std::multiset<std::pair,              > >::value == R)\
    MACRO_TEST3(AD::M<std::priority_queue<std::pair,        > >::value == R)\
    MACRO_TEST3(AD::M<std::set<std::pair,                   > >::value == R)\
    MACRO_TEST3(AD::M<std::vector<std::pair,                > >::value == R)

#define MACRO_CONTAINER_PAIR11(M, R) \
    MACRO_TEST3(AD::M<std::forward_list<std::pair,          > >::value == R)\
    MACRO_TEST3(AD::M<std::unordered_multiset<std::pair,    > >::value == R)\
    MACRO_TEST3(AD::M<std::unordered_set<std::pair,         > >::value == R)

#define MACRO_CONTAINER_TUPLE(M, R) \
    MACRO_TEST3(AD::M<std::deque<std::tuple,                > >::value == R)\
    MACRO_TEST3(AD::M<std::list<std::tuple,                 > >::value == R)\
    MACRO_TEST3(AD::M<std::multiset<std::tuple,             > >::value == R)\
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
    MACRO_TEST1(AD::M<C<std::multiset,                      > >::value == R)\
    MACRO_TEST1(AD::M<C<std::priority_queue,                > >::value == R)\
    MACRO_TEST1(AD::M<C<std::set,                           > >::value == R)\
    MACRO_TEST1(AD::M<C<std::vector,                        > >::value == R)\
    MACRO_TEST1(AD::M<C<std::queue,                         > >::value == R)\
    MACRO_TEST1(AD::M<C<std::stack,                         > >::value == R)

#define MACRO_MATRIX11(M, C, R) \
    MACRO_TEST2(AD::M<C<std::array,                         > >::value == R)\
    MACRO_TEST1(AD::M<C<std::forward_list,                  > >::value == R)\
    MACRO_TEST1(AD::M<C<std::unordered_multiset,            > >::value == R)\
    MACRO_TEST1(AD::M<C<std::unordered_set,                 > >::value == R)

#define MACRO_SPAN(M, R) \
    MACRO_TEST1(AD::M<std::span,                            >::value == R)

TEST_CASE("1. templates", "[detail]")
{
    bool is_array = false;
    bool is_container = false;
    bool is_container_pair = false;
    bool is_container_tuple = false;
    bool is_map = false;
    bool is_queue = false;
    bool is_matrix = false;
    bool is_span = false;

#ifndef ARGPARSE_CXX_11
    (void)is_array;
    (void)is_container_tuple;
#endif  // C++11+
#ifndef ARGPARSE_HAS_SPAN
    (void)is_span;
#endif  // ARGPARSE_HAS_SPAN

    SECTION("1.1 is_stl_array") {
        is_array = true;

        MACRO_SIMPLE(is_stl_array, false);
        MACRO_CONTAINER(is_stl_array, is_container);
        MACRO_CONTAINER_PAIR(is_stl_array, is_container_pair);
        MACRO_MAP(is_stl_array, is_map);
        MACRO_QUEUE(is_stl_array, is_queue);
        MACRO_MATRIX(is_stl_array, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_array, std::list, is_matrix);
        MACRO_MATRIX(is_stl_array, std::vector, is_matrix);
        // array
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_array, is_array);
        MACRO_CONTAINER11(is_stl_array, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_array, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_array, is_container_tuple);
        MACRO_MAP11(is_stl_array, is_map);
        MACRO_MATRIX11(is_stl_array, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_array, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_array, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_array, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.2 is_stl_container") {
        is_container = true;
        is_container_pair = true;
        is_container_tuple = true;
        is_matrix = true;

        MACRO_SIMPLE(is_stl_container, false);
        MACRO_CONTAINER(is_stl_container, is_container);
        MACRO_CONTAINER_PAIR(is_stl_container, is_container_pair);
        MACRO_MAP(is_stl_container, is_map);
        MACRO_QUEUE(is_stl_container, is_queue);
        MACRO_MATRIX(is_stl_container, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_container, std::list, is_matrix);
        MACRO_MATRIX(is_stl_container, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_container, is_array);
        MACRO_CONTAINER11(is_stl_container, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_container, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_container, is_container_tuple);
        MACRO_MAP11(is_stl_container, is_map);
        MACRO_MATRIX11(is_stl_container, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_container, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_container, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_container, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.3 is_stl_container_paired") {
        is_container_pair = true;

        MACRO_SIMPLE(is_stl_container_paired, false);
        MACRO_CONTAINER(is_stl_container_paired, is_container);
        MACRO_CONTAINER_PAIR(is_stl_container_paired, is_container_pair);
        MACRO_MAP(is_stl_container_paired, is_map);
        MACRO_QUEUE(is_stl_container_paired, is_queue);
        MACRO_MATRIX(is_stl_container_paired, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_container_paired, std::list, is_matrix);
        MACRO_MATRIX(is_stl_container_paired, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_container_paired, is_array);
        MACRO_CONTAINER11(is_stl_container_paired, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_container_paired, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_container_paired, is_container_tuple);
        MACRO_MAP11(is_stl_container_paired, is_map);
        MACRO_MATRIX11(is_stl_container_paired, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_container_paired, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_container_paired, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_container_paired, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.4 is_stl_container_tupled") {
        is_container_tuple = true;

        MACRO_SIMPLE(is_stl_container_tupled, false);
        MACRO_CONTAINER(is_stl_container_tupled, is_container);
        MACRO_CONTAINER_PAIR(is_stl_container_tupled, is_container_pair);
        MACRO_MAP(is_stl_container_tupled, is_map);
        MACRO_QUEUE(is_stl_container_tupled, is_queue);
        MACRO_MATRIX(is_stl_container_tupled, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_container_tupled, std::list, is_matrix);
        MACRO_MATRIX(is_stl_container_tupled, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_container_tupled, is_array);
        MACRO_CONTAINER11(is_stl_container_tupled, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_container_tupled, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_container_tupled, is_container_tuple);
        MACRO_MAP11(is_stl_container_tupled, is_map);
        MACRO_MATRIX11(is_stl_container_tupled, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_container_tupled, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_container_tupled, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_container_tupled, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.5 is_stl_map") {
        is_map = true;

        MACRO_SIMPLE(is_stl_map, false);
        MACRO_CONTAINER(is_stl_map, is_container);
        MACRO_CONTAINER_PAIR(is_stl_map, is_container_pair);
        MACRO_MAP(is_stl_map, is_map);
        MACRO_QUEUE(is_stl_map, is_queue);
        MACRO_MATRIX(is_stl_map, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_map, std::list, is_matrix);
        MACRO_MATRIX(is_stl_map, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_map, is_array);
        MACRO_CONTAINER11(is_stl_map, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_map, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_map, is_container_tuple);
        MACRO_MAP11(is_stl_map, is_map);
        MACRO_MATRIX11(is_stl_map, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_map, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_map, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_map, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.6 is_stl_queue") {
        is_queue = true;

        MACRO_SIMPLE(is_stl_queue, false);
        MACRO_CONTAINER(is_stl_queue, is_container);
        MACRO_CONTAINER_PAIR(is_stl_queue, is_container_pair);
        MACRO_MAP(is_stl_queue, is_map);
        MACRO_QUEUE(is_stl_queue, is_queue);
        MACRO_MATRIX(is_stl_queue, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_queue, std::list, is_matrix);
        MACRO_MATRIX(is_stl_queue, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_queue, is_array);
        MACRO_CONTAINER11(is_stl_queue, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_queue, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_queue, is_container_tuple);
        MACRO_MAP11(is_stl_queue, is_map);
        MACRO_MATRIX11(is_stl_queue, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_queue, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_queue, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_queue, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.7 is_stl_matrix") {
        is_matrix = true;

        MACRO_SIMPLE(is_stl_matrix, false);
        MACRO_CONTAINER(is_stl_matrix, is_container);
        MACRO_CONTAINER_PAIR(is_stl_matrix, is_container_pair);
        MACRO_MAP(is_stl_matrix, is_map);
        MACRO_QUEUE(is_stl_matrix, is_queue);
        MACRO_MATRIX(is_stl_matrix, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_matrix, std::list, is_matrix);
        MACRO_MATRIX(is_stl_matrix, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_matrix, is_array);
        MACRO_CONTAINER11(is_stl_matrix, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_matrix, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_matrix, is_container_tuple);
        MACRO_MAP11(is_stl_matrix, is_map);
        MACRO_MATRIX11(is_stl_matrix, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_matrix, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_matrix, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        MACRO_SPAN(is_stl_matrix, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }

    SECTION("1.8 is_stl_span") {
        MACRO_SIMPLE(is_stl_span, false);
        MACRO_CONTAINER(is_stl_span, is_container);
        MACRO_CONTAINER_PAIR(is_stl_span, is_container_pair);
        MACRO_MAP(is_stl_span, is_map);
        MACRO_QUEUE(is_stl_span, is_queue);
        MACRO_MATRIX(is_stl_span, std::deque, is_matrix);
        MACRO_MATRIX(is_stl_span, std::list, is_matrix);
        MACRO_MATRIX(is_stl_span, std::vector, is_matrix);
#ifdef ARGPARSE_CXX_11
        MACRO_ARRAY(is_stl_span, is_array);
        MACRO_CONTAINER11(is_stl_span, is_container);
        MACRO_CONTAINER_PAIR11(is_stl_span, is_container_pair);
        MACRO_CONTAINER_TUPLE(is_stl_span, is_container_tuple);
        MACRO_MAP11(is_stl_span, is_map);
        MACRO_MATRIX11(is_stl_span, std::deque, is_matrix);
        MACRO_MATRIX11(is_stl_span, std::list, is_matrix);
        MACRO_MATRIX11(is_stl_span, std::vector, is_matrix);
#endif  // C++11+
#ifdef ARGPARSE_HAS_SPAN
        is_span = true;
        MACRO_SPAN(is_stl_span, is_span);
#endif  // ARGPARSE_HAS_SPAN
    }
}
