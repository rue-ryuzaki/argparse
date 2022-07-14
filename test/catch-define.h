/*
* Copyright (c) 2021-2022 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#ifndef _ARGPARSE_TEST_CATCH_DEFINE_H_
#define _ARGPARSE_TEST_CATCH_DEFINE_H_

#ifdef _MSVC_LANG
#if _MSVC_LANG >= 201103L
#define _ARGPARSE_CXX_11
#endif  // C++11+
#if _MSVC_LANG >= 201402L
#define _ARGPARSE_CXX_14
#endif  // C++14+
#if _MSVC_LANG >= 201703L
#define _ARGPARSE_CXX_17
#endif  // C++17+
#if _MSVC_LANG >= 202002L
#define _ARGPARSE_CXX_20
#endif  // C++20+
#else
#if __cplusplus >= 201103L
#define _ARGPARSE_CXX_11
#endif  // C++11+
#if __cplusplus >= 201402L
#define _ARGPARSE_CXX_14
#endif  // C++14+
#if __cplusplus >= 201703L
#define _ARGPARSE_CXX_17
#endif  // C++17+
#if __cplusplus >= 202002L
#define _ARGPARSE_CXX_20
#endif  // C++20+
#endif  // _MSVC_LANG

#ifdef _ARGPARSE_CXX_11
#include <catch2/catch.hpp>
#else
#include <catch1/catch.hpp>
#endif  // C++11+

#endif  //  _ARGPARSE_TEST_CATCH_DEFINE_H_
