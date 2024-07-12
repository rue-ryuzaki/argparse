/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#ifdef _MSVC_LANG
# if _MSVC_LANG >= 201103L
#  define CATCH_CXX_11
# endif  // C++11+
#else
# if __cplusplus >= 201103L
#  define CATCH_CXX_11
# endif  // C++11+
#endif  // _MSVC_LANG

#define CATCH_CONFIG_MAIN
#ifdef CATCH_CXX_11
# include <catch2/catch.hpp>
#else
# include <catch1/catch.hpp>
#endif  // C++11+
