/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
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

#ifdef _ARGPARSE_CXX_11
#define _make_vec(...) { __VA_ARGS__ }
#else
inline std::vector<std::string> _make_vec()
{
    return std::vector<std::string>();
}

inline std::vector<std::string> _make_vec(std::string const& arg1)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12,
                                          std::string const& arg13)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    res.push_back(arg13);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12,
                                          std::string const& arg13,
                                          std::string const& arg14)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    res.push_back(arg13);
    res.push_back(arg14);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12,
                                          std::string const& arg13,
                                          std::string const& arg14,
                                          std::string const& arg15)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    res.push_back(arg13);
    res.push_back(arg14);
    res.push_back(arg15);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12,
                                          std::string const& arg13,
                                          std::string const& arg14,
                                          std::string const& arg15,
                                          std::string const& arg16)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    res.push_back(arg13);
    res.push_back(arg14);
    res.push_back(arg15);
    res.push_back(arg16);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12,
                                          std::string const& arg13,
                                          std::string const& arg14,
                                          std::string const& arg15,
                                          std::string const& arg16,
                                          std::string const& arg17)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    res.push_back(arg13);
    res.push_back(arg14);
    res.push_back(arg15);
    res.push_back(arg16);
    res.push_back(arg17);
    return res;
}

inline std::vector<std::string> _make_vec(std::string const& arg1,
                                          std::string const& arg2,
                                          std::string const& arg3,
                                          std::string const& arg4,
                                          std::string const& arg5,
                                          std::string const& arg6,
                                          std::string const& arg7,
                                          std::string const& arg8,
                                          std::string const& arg9,
                                          std::string const& arg10,
                                          std::string const& arg11,
                                          std::string const& arg12,
                                          std::string const& arg13,
                                          std::string const& arg14,
                                          std::string const& arg15,
                                          std::string const& arg16,
                                          std::string const& arg17,
                                          std::string const& arg18)
{
    std::vector<std::string> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    res.push_back(arg5);
    res.push_back(arg6);
    res.push_back(arg7);
    res.push_back(arg8);
    res.push_back(arg9);
    res.push_back(arg10);
    res.push_back(arg11);
    res.push_back(arg12);
    res.push_back(arg13);
    res.push_back(arg14);
    res.push_back(arg15);
    res.push_back(arg16);
    res.push_back(arg17);
    res.push_back(arg18);
    return res;
}
#endif  // C++11+

#endif  // _ARGPARSE_TEST_CATCH_DEFINE_H_