/* SPDX-License-Identifier: MIT
 *  __            __   __  __   __   __   __  __   __
 * |   _|_ _|_   |  | |   |  | |  | |  | |   |__  |__|
 * |__  |   |    \_/| |   \__| |_/  \_/| |    __| \__
 *                         __| |
 * MIT License
 *
 * Argument parser for C++ (ArgumentParser v1.9.4)
 *
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _ARGPARSE_ARGUMENT_PARSER_HPP_
#define _ARGPARSE_ARGUMENT_PARSER_HPP_

// -- #undef ------------------------------------------------------------------
// -- version -----------------------------------------------------------------
#undef ARGPARSE_VERSION_MAJOR
#undef ARGPARSE_VERSION_MINOR
#undef ARGPARSE_VERSION_PATCH
#undef ARGPARSE_VERSION_TWEAK
#undef ARGPARSE_VERSION_NUM
#undef ARGPARSE_VERSION_COMPILED
#undef ARGPARSE_VERSION_AT_LEAST
// -- standard ----------------------------------------------------------------
#undef ARGPARSE_CXX_98
#undef ARGPARSE_CXX_11
#undef ARGPARSE_CXX_14
#undef ARGPARSE_CXX_17
#undef ARGPARSE_CXX_20
#undef ARGPARSE_CXX_23
// -- features ----------------------------------------------------------------
#undef ARGPARSE_HAS_OPTIONAL
#undef ARGPARSE_HAS_STRING_VIEW
// -- attributes --------------------------------------------------------------
// C++11+
#undef ARGPARSE_ATTR_NORETURN
#undef ARGPARSE_ATTR_CARRIES_DEPENDENCY
// C++14+
#undef ARGPARSE_ATTR_DEPRECATED
#undef ARGPARSE_ATTR_DEPRECATED_REASON
// C++17+
#undef ARGPARSE_ATTR_FALLTHROUGH
#undef ARGPARSE_ATTR_MAYBE_UNUSED
#undef ARGPARSE_ATTR_NODISCARD
// C++20+
#undef ARGPARSE_ATTR_LIKELY
#undef ARGPARSE_ATTR_NODISCARD_REASON
#undef ARGPARSE_ATTR_NO_UNIQUE_ADDRESS
#undef ARGPARSE_ATTR_UNLIKELY
// C++23+
#undef ARGPARSE_ATTR_ASSUME
// -- specifiers --------------------------------------------------------------
#undef ARGPARSE_CONSTEXPR
#undef ARGPARSE_ENUM_TYPE
#undef ARGPARSE_EXPORT
#undef ARGPARSE_FINAL
#undef ARGPARSE_INLINE_VARIABLE
#undef ARGPARSE_MOVE
#undef ARGPARSE_NOEXCEPT
#undef ARGPARSE_NULLPTR
#undef ARGPARSE_OVERRIDE
#undef ARGPARSE_USE_CONSTEXPR

// -- #define -----------------------------------------------------------------
// -- version -----------------------------------------------------------------
#define ARGPARSE_VERSION_MAJOR 1
#define ARGPARSE_VERSION_MINOR 9
#define ARGPARSE_VERSION_PATCH 4

/*!
 *  This macro turns the version numbers into a numeric value:
 *  \verbatim
    (1,2,3) -> (10203)
    \endverbatim
 */
#define ARGPARSE_VERSION_NUM(X, Y, Z) ((X) * 10000 + (Y) * 100 + (Z))

/*!
 *  This is the version number macro for the current argparse version.
 */
#define ARGPARSE_VERSION_COMPILED \
    ARGPARSE_VERSION_NUM(ARGPARSE_VERSION_MAJOR, \
                         ARGPARSE_VERSION_MINOR, \
                         ARGPARSE_VERSION_PATCH)

/*!
 *  This macro will evaluate to true if compiled with argparse at least X.Y.Z.
 */
#define ARGPARSE_VERSION_AT_LEAST(X, Y, Z) \
    (ARGPARSE_VERSION_COMPILED >= ARGPARSE_VERSION_NUM(X, Y, Z))

// -- standard ----------------------------------------------------------------
#define ARGPARSE_CXX_98

#ifdef _MSVC_LANG
# if _MSVC_LANG >= 201103L
#  define ARGPARSE_CXX_11
# endif  // C++11+
# if _MSVC_LANG >  201103L
#  define ARGPARSE_CXX_14
# endif  // C++14+
# if _MSVC_LANG >  201402L
#  define ARGPARSE_CXX_17
# endif  // C++17+
# if _MSVC_LANG >  201703L
#  define ARGPARSE_CXX_20
# endif  // C++20+
# if _MSVC_LANG >  202002L
#  define ARGPARSE_CXX_23
# endif  // C++23+
#else
# if __cplusplus >= 201103L
#  define ARGPARSE_CXX_11
# endif  // C++11+
# if __cplusplus >  201103L
#  define ARGPARSE_CXX_14
# endif  // C++14+
# if __cplusplus >  201402L
#  define ARGPARSE_CXX_17
# endif  // C++17+
# if __cplusplus >  201703L
#  define ARGPARSE_CXX_20
# endif  // C++20+
# if __cplusplus >  202002L
#  define ARGPARSE_CXX_23
# endif  // C++23+
#endif  // _MSVC_LANG

// -- static building ---------------------------------------------------------
#undef ARGPARSE_INL
#ifdef ARGPARSE_DECLARATION
// header
# undef ARGPARSE_IMPLEMENTATION
#else
# ifdef ARGPARSE_IMPLEMENTATION
// static build
#  define ARGPARSE_INL
# else
// single-header
#  define ARGPARSE_INL inline
# endif  // ARGPARSE_IMPLEMENTATION
#endif  // ARGPARSE_DECLARATION

// -- terminal size detection -------------------------------------------------
#undef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#if !defined ARGPARSE_DISABLE_TERMINAL_SIZE_DETECTION
# define ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION

#ifdef ARGPARSE_INL
# ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#  if defined(_WIN32)
#   undef ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#   undef ARGPARSE_DEFINE_VC_EXTRALEAN

#   ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    define ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#   endif  // WIN32_LEAN_AND_MEAN

#   ifndef VC_EXTRALEAN
#    define VC_EXTRALEAN
#    define ARGPARSE_DEFINE_VC_EXTRALEAN
#   endif  // VC_EXTRALEAN

#   ifndef NOMINMAX
#    define NOMINMAX
#    define ARGPARSE_DEFINE_NOMINMAX
#   endif  // NOMINMAX

#   ifdef _MSC_VER
#    include <Windows.h>
#   else
#    include <windows.h>
#   endif  // _MSC_VER

#   ifdef ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#    undef WIN32_LEAN_AND_MEAN
#   endif  // ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN

#   ifdef ARGPARSE_DEFINE_VC_EXTRALEAN
#    undef VC_EXTRALEAN
#   endif  // ARGPARSE_DEFINE_VC_EXTRALEAN

#   ifdef ARGPARSE_DEFINE_NOMINMAX
#    undef NOMINMAX
#   endif  // ARGPARSE_DEFINE_NOMINMAX

#   undef ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#   undef ARGPARSE_DEFINE_VC_EXTRALEAN
#   undef ARGPARSE_DEFINE_NOMINMAX
#  else  // UNIX
#   include <sys/ioctl.h>
#   include <fcntl.h>
#   include <unistd.h>
#  endif  // _WIN32
# endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#endif  // ARGPARSE_INL

// -- #include ----------------------------------------------------------------
#ifdef ARGPARSE_CXX_11
# include <array>
# include <cstdint>
# include <functional>
# include <initializer_list>
# include <memory>
# include <tuple>
# include <type_traits>
# include <unordered_map>
#else
# include <stdint.h>
#endif  // C++11+

#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef ARGPARSE_CXX_17
# define ARGPARSE_HAS_OPTIONAL
# define ARGPARSE_HAS_STRING_VIEW
#endif  // C++17+

#ifdef ARGPARSE_HAS_OPTIONAL
# include <optional>
#endif  // ARGPARSE_HAS_OPTIONAL

#ifdef ARGPARSE_INL
# include <fstream>
# ifdef ARGPARSE_CXX_11
#  include <regex>
# endif  // C++11+
# ifdef ARGPARSE_HAS_STRING_VIEW
#  include <string_view>
# endif  // ARGPARSE_HAS_STRING_VIEW
#endif  // ARGPARSE_INL

// -- attributes --------------------------------------------------------------
#ifdef ARGPARSE_CXX_11
# define ARGPARSE_ATTR_NORETURN             [[noreturn]]
# define ARGPARSE_ATTR_CARRIES_DEPENDENCY   [[carries_dependency]]
#else
# define ARGPARSE_ATTR_NORETURN
# define ARGPARSE_ATTR_CARRIES_DEPENDENCY
#endif  // C++11+

#ifdef ARGPARSE_CXX_14
# define ARGPARSE_ATTR_DEPRECATED           [[deprecated]]
# define ARGPARSE_ATTR_DEPRECATED_REASON(X) [[deprecated(X)]]
#else
# define ARGPARSE_ATTR_DEPRECATED
# define ARGPARSE_ATTR_DEPRECATED_REASON(X)
#endif  // C++14+

#ifdef ARGPARSE_CXX_17
# define ARGPARSE_ATTR_FALLTHROUGH          [[fallthrough]]
# define ARGPARSE_ATTR_MAYBE_UNUSED         [[maybe_unused]]
# define ARGPARSE_ATTR_NODISCARD            [[nodiscard]]
# define ARGPARSE_ATTR_NODISCARD_REASON(X)  [[nodiscard]]
#else
# define ARGPARSE_ATTR_FALLTHROUGH
# define ARGPARSE_ATTR_MAYBE_UNUSED
# define ARGPARSE_ATTR_NODISCARD
# define ARGPARSE_ATTR_NODISCARD_REASON(X)
#endif  // C++17+

#ifdef ARGPARSE_CXX_20
# undef ARGPARSE_ATTR_NODISCARD_REASON

# define ARGPARSE_ATTR_LIKELY               [[likely]]
# define ARGPARSE_ATTR_NODISCARD_REASON(X)  [[nodiscard(X)]]
# define ARGPARSE_ATTR_NO_UNIQUE_ADDRESS    [[no_unique_address]]
# define ARGPARSE_ATTR_UNLIKELY             [[unlikely]]
#else
# define ARGPARSE_ATTR_LIKELY
# define ARGPARSE_ATTR_NO_UNIQUE_ADDRESS
# define ARGPARSE_ATTR_UNLIKELY
#endif  // C++20+

#ifdef ARGPARSE_CXX_23
# define ARGPARSE_ATTR_ASSUME(X)            [[assume(X)]]
#else
# define ARGPARSE_ATTR_ASSUME(X)
#endif  // C++23+

// -- specifiers --------------------------------------------------------------
#define ARGPARSE_EXPORT

#ifdef ARGPARSE_CXX_11
# define ARGPARSE_FINAL final
# define ARGPARSE_NOEXCEPT noexcept
# define ARGPARSE_OVERRIDE override
# define ARGPARSE_CONSTEXPR constexpr
# define ARGPARSE_USE_CONSTEXPR constexpr
# define ARGPARSE_ENUM_TYPE(X) : X
# define ARGPARSE_MOVE(X) std::move(X)
#else
# define ARGPARSE_FINAL
# define ARGPARSE_NOEXCEPT
# define ARGPARSE_OVERRIDE
# define ARGPARSE_CONSTEXPR
# define ARGPARSE_USE_CONSTEXPR const
# define ARGPARSE_ENUM_TYPE(X)
# define ARGPARSE_MOVE(X) (X)
#endif  // C++11+

#ifdef ARGPARSE_CXX_17
# define ARGPARSE_INLINE_VARIABLE inline
#else
# define ARGPARSE_INLINE_VARIABLE static
#endif  // C++17+

namespace argparse {
#ifdef ARGPARSE_CXX_11
# define ARGPARSE_NULLPTR nullptr
#else
const
class nullptr_t
{
public:
    template <class T>
    operator T*     () const { return 0; }

    template <class C, class T>
    operator T C::* () const { return 0; }

private:
    void operator & () const;
} _nullptr = {};

# define ARGPARSE_NULLPTR _nullptr
#endif  // C++11+

/*!
 *  \brief Action values
 *
 *  \enum Action
 */
ARGPARSE_EXPORT enum Action ARGPARSE_ENUM_TYPE(uint16_t)
{
    store                   = 0x0001,
    store_const             = 0x0002,
    store_true              = 0x0004,
    store_false             = 0x0008,
    append                  = 0x0010,
    append_const            = 0x0020,
    count                   = 0x0040,
    help                    = 0x0080,
    version                 = 0x0100,
    extend                  = 0x0200,
    /// since v1.7.1
    language                = 0x0400,
    BooleanOptionalAction   = 0x1000,
};

/*!
 *  \brief ArgumentError handler
 */
ARGPARSE_EXPORT class ArgumentError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct ArgumentError handler
     *
     *  \param error Error message
     *
     *  \return ArgumentError object
     */
    explicit
    ArgumentError(
            std::string const& error)
        : std::invalid_argument("argparse::ArgumentError: " + error)
    { }
};

/*!
 *  \brief AttributeError handler
 */
ARGPARSE_EXPORT class AttributeError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct AttributeError handler
     *
     *  \param error Error message
     *
     *  \return AttributeError object
     */
    explicit
    AttributeError(
            std::string const& error)
        : std::invalid_argument("AttributeError: " + error)
    { }
};

/*!
 *  \brief ValueError handler
 */
ARGPARSE_EXPORT class ValueError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct ValueError handler
     *
     *  \param error Error message
     *
     *  \return ValueError object
     */
    explicit
    ValueError(
            std::string const& error)
        : std::invalid_argument("ValueError: " + error)
    { }
};

/*!
 *  \brief IndexError handler
 */
ARGPARSE_EXPORT class IndexError : public std::out_of_range
{
public:
    /*!
     *  \brief Construct IndexError handler
     *
     *  \param error Error message
     *
     *  \return IndexError object
     */
    explicit
    IndexError(
            std::string const& error)
        : std::out_of_range("IndexError: " + error)
    { }
};

/*!
 *  \brief NameError handler
 *
 *  \since v1.7.1
 */
ARGPARSE_EXPORT class NameError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct NameError handler
     *
     *  \param error Error message
     *
     *  \return NameError object
     */
    explicit
    NameError(
            std::string const& error)
        : std::invalid_argument("NameError: " + error)
    { }
};

/*!
 *  \brief TypeError handler
 */
ARGPARSE_EXPORT class TypeError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct TypeError handler
     *
     *  \param error Error message
     *
     *  \return TypeError object
     */
    explicit
    TypeError(
            std::string const& error)
        : std::invalid_argument("TypeError: " + error)
    { }
};

/*!
 *  \brief Don't use this enum name! use argparse::SUPPRESS value directly
 *
 *  \enum _SUPPRESS
 */
ARGPARSE_EXPORT ARGPARSE_INLINE_VARIABLE
enum _SUPPRESS ARGPARSE_ENUM_TYPE(uint8_t) {} SUPPRESS;

/*!
 *  \brief Don't use this enum name! use argparse::REMAINDER value directly
 *
 *  \enum _REMAINDER
 */
ARGPARSE_EXPORT ARGPARSE_INLINE_VARIABLE
enum _REMAINDER ARGPARSE_ENUM_TYPE(uint8_t) {} REMAINDER;

namespace detail {
// -- constants ---------------------------------------------------------------
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _equal               = '=';
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _spaces[]            = " ";

ARGPARSE_INLINE_VARIABLE int32_t ARGPARSE_USE_CONSTEXPR
_store_action = argparse::store | argparse::append | argparse::extend;

// -- standard type traits ----------------------------------------------------
typedef int8_t _yes;
typedef int16_t _no;

template <class T, T>
struct func_tag { };

#ifdef ARGPARSE_CXX_11
using std::decay;
using std::enable_if;
using std::is_constructible;
using std::is_floating_point;
using std::is_integral;
using std::is_same;
using std::false_type;
using std::true_type;

template <class...>
struct voider { using type = void; };

template <class T>
struct type_tag { };

template <class T, T... Ints>
struct integer_sequence { };

template <class T, T N, class = void>
struct make_integer_sequence_impl
{
    template <class>
    struct tmp;

    template <T... Prev>
    struct tmp<integer_sequence<T, Prev...> >
    {
        using type = integer_sequence<T, Prev..., N - 1>;
    };

    using type
      = typename tmp<typename make_integer_sequence_impl<T, N - 1>::type>::type;
};

template <class T, T N>
struct make_integer_sequence_impl<T, N, typename enable_if<N == 0>::type>
{
    using type = integer_sequence<T>;
};

template <class T, T N>
using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;
#else
template <bool B, class T, class F>
struct conditional                  { typedef T type; };
template <class T, class F>
struct conditional<false, T, F>     { typedef F type; };

template <bool, class T = void>
struct enable_if { };
template <class T>
struct enable_if<true, T>           { typedef T type; };

template <class T>
struct remove_extent                { typedef T type; };
template <class T>
struct remove_extent<T[]>           { typedef T type; };
template <class T, std::size_t N>
struct remove_extent<T[N]>          { typedef T type; };

template <class T>
struct remove_reference             { typedef T type; };
template <class T>
struct remove_reference<T&>         { typedef T type; };

template <class T>
struct remove_const                 { typedef T type; };
template <class T>
struct remove_const<const T>        { typedef T type; };

template <class T>
struct remove_volatile              { typedef T type; };
template <class T>
struct remove_volatile<volatile T>  { typedef T type; };

template <class T>
struct remove_cv                    { typedef T type; };
template <class T>
struct remove_cv<const T>           { typedef T type; };
template <class T>
struct remove_cv<volatile T>        { typedef T type; };
template <class T>
struct remove_cv<const volatile T>  { typedef T type; };

template <class T, T Val>
struct integral_constant
{
    typedef integral_constant type;
    typedef T value_type;
    enum {
        value = Val
    };
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T>
struct _is_pointer_helper                                         :false_type{};
template <class T>
struct _is_pointer_helper<T*>                                      :true_type{};

template <class T>
struct is_pointer
{
    static bool const value
                       = _is_pointer_helper<typename remove_cv<T>::type>::value;
};

template <class T, class U>
struct is_same                                                    :false_type{};
template <class T>
struct is_same<T, T>                                               :true_type{};

template <class>
struct _is_integral_helper                                        :false_type{};
template <>
struct _is_integral_helper<bool>                                   :true_type{};
template <>
struct _is_integral_helper<char>                                   :true_type{};
template <>
struct _is_integral_helper<signed char>                            :true_type{};
template <>
struct _is_integral_helper<unsigned char>                          :true_type{};
template <>
struct _is_integral_helper<short>                                  :true_type{};
template <>
struct _is_integral_helper<unsigned short>                         :true_type{};
template <>
struct _is_integral_helper<int>                                    :true_type{};
template <>
struct _is_integral_helper<unsigned int>                           :true_type{};
template <>
struct _is_integral_helper<long>                                   :true_type{};
template <>
struct _is_integral_helper<unsigned long>                          :true_type{};
template <>
struct _is_integral_helper<long long>                              :true_type{};
template <>
struct _is_integral_helper<unsigned long long>                     :true_type{};

template <class T>
struct is_integral   : _is_integral_helper<typename remove_cv<T>::type>::type{};

template <class>
struct _is_floating_point_helper                                  :false_type{};
template <>
struct _is_floating_point_helper<float>                            :true_type{};
template <>
struct _is_floating_point_helper<double>                           :true_type{};
template <>
struct _is_floating_point_helper<long double>                      :true_type{};

template <class T>
struct is_floating_point
               : _is_floating_point_helper<typename remove_cv<T>::type>::type{};

template <class>
struct is_array                                                   :false_type{};
template <class T, std::size_t N>
struct is_array<T[N]>                                              :true_type{};
template <class T>
struct is_array<T[]>                                               :true_type{};

template <class T>
struct decay
{
    typedef typename remove_reference<T>::type U;

    typedef typename conditional<
        is_array<U>::value,
        typename remove_extent<U>::type*,
        typename remove_cv<U>::type
    >::type type;
};

template <class T>
struct _replace_array_with_pointer
{
    typedef typename conditional<
        is_array<T>::value, typename remove_extent<T>::type*, T
    >::type type;
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
struct _is_constructible_impl
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;
    typedef typename _replace_array_with_pointer<AT_2>::type AU_2;
    typedef typename _replace_array_with_pointer<AT_3>::type AU_3;
    typedef typename _replace_array_with_pointer<AT_4>::type AU_4;

    template <class C_T, class C_AT_1, class C_AT_2, class C_AT_3, class C_AT_4>
    static _yes test(
        typename enable_if<
            sizeof(C_T) ==
            sizeof(C_T(
                static_cast<C_AT_1>(
                  *static_cast<typename remove_reference<C_AT_1>::type*>(NULL)),
                static_cast<C_AT_2>(
                  *static_cast<typename remove_reference<C_AT_2>::type*>(NULL)),
                static_cast<C_AT_3>(
                  *static_cast<typename remove_reference<C_AT_3>::type*>(NULL)),
                static_cast<C_AT_4>(
                  *static_cast<typename remove_reference<C_AT_4>::type*>(NULL))
            ))
        >::type*
    );

    template <class, class, class, class, class>
    static _no test(...);

    static bool const value
              = (sizeof(test<T, AU_1, AU_2, AU_3, AU_4>(NULL)) == sizeof(_yes));
};

template <class T, class AT_1, class AT_2, class AT_3>
struct _is_constructible_impl<T, AT_1, AT_2, AT_3, void>
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;
    typedef typename _replace_array_with_pointer<AT_2>::type AU_2;
    typedef typename _replace_array_with_pointer<AT_3>::type AU_3;

    template <class C_T, class C_AT_1, class C_AT_2, class C_AT_3>
    static _yes test(
        typename enable_if<
            sizeof(C_T) ==
            sizeof(C_T(
                static_cast<C_AT_1>(
                  *static_cast<typename remove_reference<C_AT_1>::type*>(NULL)),
                static_cast<C_AT_2>(
                  *static_cast<typename remove_reference<C_AT_2>::type*>(NULL)),
                static_cast<C_AT_3>(
                  *static_cast<typename remove_reference<C_AT_3>::type*>(NULL))
            ))
        >::type*
    );

    template <class, class, class, class>
    static _no test(...);

    static bool const value
                    = (sizeof(test<T, AU_1, AU_2, AU_3>(NULL)) == sizeof(_yes));
};

template <class T, class AT_1, class AT_2>
struct _is_constructible_impl<T, AT_1, AT_2, void, void>
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;
    typedef typename _replace_array_with_pointer<AT_2>::type AU_2;

    template <class C_T, class C_AT_1, class C_AT_2>
    static _yes test(
        typename enable_if<
            sizeof(C_T) ==
            sizeof(C_T(
                static_cast<C_AT_1>(
                  *static_cast<typename remove_reference<C_AT_1>::type*>(NULL)),
                static_cast<C_AT_2>(
                  *static_cast<typename remove_reference<C_AT_2>::type*>(NULL))
            ))
        >::type*
    );

    template <class, class, class>
    static _no test(...);

    static bool const value
                          = (sizeof(test<T, AU_1, AU_2>(NULL)) == sizeof(_yes));
};

template <class T, class AT_1>
struct _is_constructible_impl<T, AT_1, void, void, void>
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;

    template <class C_T, class C_AT_1>
    static _yes test(
        typename enable_if<
            sizeof(C_T) ==
            sizeof(C_T(
                static_cast<C_AT_1>(
                   *static_cast<typename remove_reference<C_AT_1>::type*>(NULL))
            ))
        >::type*
    );

    template <class, class>
    static _no test(...);

    static bool const value = (sizeof(test<T, AU_1>(NULL)) == sizeof(_yes));
};

template <class T>
struct _is_constructible_impl<T, void, void, void, void>
{
    template <class C_T>
    static C_T testFun(C_T);

    template <class C_T>
    static _yes test(
            typename enable_if<sizeof(C_T)
                    == sizeof(_is_constructible_impl::testFun(C_T()))>::type*);

    template <class>
    static _no test(...);

    static bool const value = (sizeof(test<T>(NULL)) == sizeof(_yes));
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
struct _is_constructible_impl_ptr
{
    static bool const value = false;
};

template <class T, class AT_1>
struct _is_constructible_impl_ptr<T, AT_1,
        typename enable_if<is_pointer<
            typename remove_reference<T>::type>::value, void>::type, void, void>
{
    template <class C_T>
    static _yes test(C_T);

    template <class>
    static _no test(...);

    static bool const value
                   = (sizeof(test<T>(static_cast<AT_1>(NULL))) == sizeof(_yes));
};

template <class T>
struct _is_constructible_impl_ptr<T, void, void, void, void>
{
    static bool const value = true;
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
struct is_constructible
{
    typedef typename _replace_array_with_pointer<T>::type U;

    static bool const value = (
        is_pointer<typename remove_reference<U>::type>::value
                ? _is_constructible_impl_ptr<U, AT_1, AT_2, AT_3, AT_4>::value
                : _is_constructible_impl<U, AT_1, AT_2, AT_3, AT_4>::value
    );
};

template <>
struct is_constructible<std::string, _SUPPRESS>
{
    static bool const value = false;
};

template <>
struct is_constructible<std::string, std::vector<std::string> >
{
    static bool const value = false;
};

template <class T, class = void>
struct voider { typedef void type; };
#endif  // C++11+

// -- library type traits -----------------------------------------------------
namespace _stream_check {
    struct anyx { template <class T> anyx(T const&); };

    _no operator <<(anyx const&, anyx const&);
    _no operator >>(anyx const&, anyx const&);

    template <class T>
    static _yes test(T const&);

    template <class T>
    static _no test(...);

    template <class StreamType, class T>
    struct has_loading_support
    {
        static StreamType& stream;
        static T& x;
        static bool const value = sizeof(test(stream >> x)) == sizeof(_yes);
    };

    template <class StreamType, class T>
    struct has_saving_support
    {
        static StreamType& stream;
        static T& x;
        static bool const value = sizeof(test(stream << x)) == sizeof(_yes);
    };

    template <class StreamType, class T>
    struct has_stream_operators
    {
        static bool const value = has_loading_support<StreamType, T>::value
                                && has_saving_support<StreamType, T>::value;
    };
}  // namespace _stream_check

template <class T>
struct has_operator_in : _stream_check::has_loading_support<std::istream, T> {};

template <class T>
struct has_insert
{
    template <class C>
    static _yes test(
            func_tag<typename C::iterator(C::*)(
                typename C::value_type const&), &C::insert>*);

    template <class C>
    static _yes test(
            func_tag<std::pair<typename C::iterator, bool>(C::*)(
                typename C::value_type const&), &C::insert>*);

    template <class>
    static _no test(...);

    static bool const value = sizeof(test<T>(NULL)) == sizeof(_yes);
};

template <class T>
struct has_push
{
    template <class C>
    static _yes test(
            func_tag<void(C::*)(typename C::value_type const&), &C::push>*);

    template <class>
    static _no test(...);

    static bool const value = sizeof(test<T>(NULL)) == sizeof(_yes);
};

template <class T>
struct has_push_back
{
    template <class C>
    static _yes test(
           func_tag<void(C::*)(typename C::value_type const&), &C::push_back>*);

    template <class>
    static _no test(...);

    static bool const value = sizeof(test<T>(NULL)) == sizeof(_yes);
};

template <class T>
struct has_push_front
{
    template <class C>
    static _yes test(
          func_tag<void(C::*)(typename C::value_type const&), &C::push_front>*);

    template <class>
    static _no test(...);

    static bool const value = sizeof(test<T>(NULL)) == sizeof(_yes);
};

template <class T>
struct is_byte_type
{
    static bool const value
                        = sizeof(T) == sizeof(char) && !is_same<bool, T>::value;
};

template <class T>
struct is_string_ctor
{
    static bool const value = is_constructible<std::string, T>::value;
};

template <class T, class = void>
struct has_value_type                      { static bool const value = false; };
template <class T>
struct has_value_type<T, typename voider<
             typename T::value_type>::type> { static bool const value = true; };

template <class T, class = void>
struct has_sub_string_ctor                 { static bool const value = false; };
template <class T>
struct has_sub_string_ctor<T, typename voider<typename T::value_type>::type>
{
    static bool const value = is_string_ctor<typename T::value_type>::value;
};

template <class T, class = void>
struct has_vector_ctor                     { static bool const value = false; };
template <class T>
struct has_vector_ctor<T, typename voider<typename T::value_type>::type>
{
    static bool const value = is_constructible<
            T, typename std::vector<typename T::value_type>::iterator,
               typename std::vector<typename T::value_type>::iterator>::value;
};

template <class T, class = void>
struct has_deque_ctor                      { static bool const value = false; };
template <class T>
struct has_deque_ctor<T, typename voider<typename T::value_type>::type>
{
    static bool const value = is_constructible<
            T, typename std::deque<typename T::value_type> >::value;
};

template <class T, class = void>
struct has_sub_vector_ctor                 { static bool const value = false; };
template <class T>
struct has_sub_vector_ctor<T, typename voider<typename T::value_type>::type>
{
    static bool const value = has_vector_ctor<typename T::value_type>::value;
};

template <class T, class = void>
struct has_sub_deque_ctor                  { static bool const value = false; };
template <class T>
struct has_sub_deque_ctor<T, typename voider<typename T::value_type>::type>
{
    static bool const value = has_deque_ctor<typename T::value_type>::value;
};

template <class T, class = void>
struct is_stl_map                                                 :false_type{};
template <class T>
struct is_stl_map<T, typename voider<typename T::key_type,
                                    typename T::mapped_type>::type>:true_type{};

template <class T, class = void>
struct is_stl_pair                                                :false_type{};
template <class T>
struct is_stl_pair<T, typename voider<typename T::first_type,
                                    typename T::second_type>::type>:true_type{};

template <class T, class = void>
struct is_stl_container_paired             { static bool const value = false; };
template <class T>
struct is_stl_container_paired<T, typename voider<typename T::value_type>::type>
{
    static bool const value
          = is_stl_pair<typename T::value_type>::value && !is_stl_map<T>::value;
};

template <class T, class = void>
struct is_stl_container_tupled             { static bool const value = false; };

template <class T>
struct is_stl_array                                               :false_type{};

template <class T>
struct is_stl_container
{
    static bool const value = has_vector_ctor<T>::value
                          && !is_string_ctor<T>::value && !is_stl_map<T>::value;
};

template <class T>
struct is_stl_queue
{
    static bool const value = has_deque_ctor<T>::value
                          && !has_vector_ctor<T>::value;
};

template <class T, class = void>
struct is_stl_sub_array                    { static bool const value = false; };
template <class T>
struct is_stl_sub_array<T, typename voider<typename T::value_type>::type>
{
    static bool const value = is_stl_array<typename T::value_type>::value;
};

template <class T>
struct is_stl_matrix
{
    static bool const value =
            (is_stl_array<T>::value
             || is_stl_container<T>::value
             || is_stl_queue<T>::value)
            && (is_stl_sub_array<T>::value
                || (has_sub_vector_ctor<T>::value
                    && !has_sub_string_ctor<T>::value)
                || has_sub_deque_ctor<T>::value);
};

template <class T>
struct is_stl_tuple                                               :false_type{};

#ifdef ARGPARSE_CXX_11
template <class T, std::size_t N>
struct is_stl_array<std::array                          <T, N> >   :true_type{};

template <class... Args>
struct is_stl_tuple<std::tuple                          <Args...> >:true_type{};

template <class T>
struct is_stl_container_tupled<T, typename voider<typename T::value_type>::type>
{
    static bool const value = is_stl_tuple<typename T::value_type>::value;
};
#endif  // C++11+

#ifdef ARGPARSE_CXX_11
template <class T>
struct rval { typedef T&& type; };
template <class T>
struct func1 { typedef std::function<void(T)> type; };
template <class T, class U>
struct func2 { typedef std::function<void(T, U)> type; };

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
#else
template <class T>
struct rval { typedef T const& type; };
template <class T>
struct func1 { typedef void (*type)(T); };
template <class T, class U>
struct func2 { typedef void (*type)(T, U); };

struct ref_count_data
{
    std::size_t weak_count;
    std::size_t use_count;
};

class reference_count
{
public:
    reference_count()
        : ref_counts()
    {
        ref_counts.weak_count = 1;
        ref_counts.use_count = 0;
    }

    inline void
    add_shared_ref()
    {
        ++ref_counts.use_count;
    }

    inline ref_count_data
    release_shared_ref()
    {
        --ref_counts.use_count;
        if (!ref_counts.use_count) {
            --ref_counts.weak_count;
        }
        return ref_counts;
    }

    inline void
    add_weak_ref()
    {
        ++ref_counts.weak_count;
    }

    inline ref_count_data
    release_weak_ref()
    {
        --ref_counts.weak_count;
        return ref_counts;
    }

    inline std::size_t
    use_count() const
    {
        return ref_counts.use_count;
    }

private:
    ref_count_data ref_counts;
};

template <class T>
class shared_ptr
{
public:
    typedef T element_type;

    shared_ptr() throw()
        : px(NULL),
          pn(NULL)
    { }

    shared_ptr(
            nullptr_t) throw()
        : px(NULL),
          pn(NULL)
    { }

    explicit
    shared_ptr(
            T* value)
        : px(value),
          pn(NULL)
    {
        if (px) {
            pn = new reference_count();
            pn->add_shared_ref();
        }
    }

    shared_ptr(
            shared_ptr<T> const& orig) throw()
        : px(orig.px),
          pn(orig.pn)
    {
        if (pn) {
            pn->add_shared_ref();
        }
    }

    template <class U>
    explicit
    shared_ptr(
            shared_ptr<U> const& orig) throw()
        : px(static_cast<typename shared_ptr<T>::element_type*>(orig.get())),
          pn(orig.pn)
    {
        if (pn) {
            pn->add_shared_ref();
        }
    }

    ~shared_ptr() throw()
    {
        if (pn) {
            ref_count_data updated_counts = pn->release_shared_ref();
            if (!updated_counts.use_count) {
                delete px;
                px = NULL;
            }
            if (!updated_counts.weak_count) {
                delete pn;
                pn = NULL;
            }
        }
    }

    inline shared_ptr<T>&
    operator =(
            nullptr_t) throw()
    {
        reset();
        return *this;
    }

    inline shared_ptr&
    operator =(
            shared_ptr<T> const& rhs) throw()
    {
        shared_ptr(rhs).swap(*this);
        return *this;
    }

    template <class U>
    inline shared_ptr&
    operator =(
            shared_ptr<U> const& rhs) throw()
    {
        shared_ptr(rhs).swap(*this);
        return *this;
    }

    inline void
    reset() throw()
    {
        shared_ptr().swap(*this);
    }

    inline void
    reset(T* ptr) throw()
    {
        shared_ptr(ptr).swap(*this);
    }

    inline void
    swap(shared_ptr<T>& other) throw()
    {
        std::swap(px, other.px);
        std::swap(pn, other.pn);
    }

    inline T&
    operator *() const throw()
    {
        return *px;
    }

    inline T*
    operator ->() const throw()
    {
        return px;
    }

    inline T*
    get() const throw()
    {
        return px;
    }

    inline
    operator bool() const throw()
    {
        return use_count() > 0;
    }

    inline bool
    operator !() const throw()
    {
        return !px;
    }

    inline std::size_t
    unique() const
    {
        return use_count() == 1;
    }

    inline std::size_t
    use_count() const throw()
    {
        return pn ? pn->use_count() : 0;
    }

    inline bool
    operator ==(
            void* rhs) const throw()
    {
        return px == rhs;
    }

    inline bool
    operator !=(
            void* rhs) const throw()
    {
        return px != rhs;
    }

private:
    template <class U>
    friend class shared_ptr;

    template <class U>
    friend class weak_ptr;

    explicit
    shared_ptr(
            element_type* pvalue_arg,
            reference_count* pn_arg) throw()
        : px(pvalue_arg),
          pn(pn_arg)
    {
        if (px) {
            pn->add_shared_ref();
        } else {
            pn = NULL;
        }
    }

    element_type* px;
    reference_count* pn;
};

template <class T>
class weak_ptr
{
public:
    typedef T element_type;

    weak_ptr() throw()
        : px(NULL),
          pn(NULL)
    { }

    weak_ptr(
            weak_ptr<T> const& orig) throw()
        : px(orig.px),
          pn(orig.pn)
    {
        if (pn) {
            pn->add_weak_ref();
        }
    }

    template <class U>
    explicit
    weak_ptr(
            weak_ptr<U> const& orig) throw()
        : px(static_cast<typename shared_ptr<T>::element_type*>(orig.px)),
          pn(orig.pn)
    {
        if (pn) {
            pn->add_weak_ref();
        }
    }

    weak_ptr(
            shared_ptr<T> const& orig) throw()
        : px(orig.px),
          pn(orig.pn)
    {
        if (pn) {
            pn->add_weak_ref();
        }
    }

    template <class U>
    explicit
    weak_ptr(
            shared_ptr<U> const& orig) throw()
        : px(static_cast<typename shared_ptr<T>::element_type*>(orig.px)),
          pn(orig.pn)
    {
        if (pn) {
            pn->add_weak_ref();
        }
    }

    ~weak_ptr() throw()
    {
        if (pn) {
            ref_count_data updated_counts = pn->release_weak_ref();
            if (!updated_counts.weak_count) {
                delete pn;
                pn = NULL;
            }
        }
    }

    inline weak_ptr&
    operator =(
            weak_ptr<T> const& rhs) throw()
    {
        weak_ptr(rhs).swap(*this);
        return *this;
    }

    template <class U>
    inline weak_ptr&
    operator =(
            weak_ptr<U> const& rhs) throw()
    {
        weak_ptr(rhs).swap(*this);
        return *this;
    }

    inline shared_ptr<T>
    lock() const throw()
    {
        return shared_ptr<element_type>(px, pn);
    }

    inline std::size_t
    use_count() const throw()
    {
        return pn ? pn->use_count() : 0;
    }

    inline bool
    expired() const throw()
    {
        return use_count() == 0;
    }

    inline void
    reset() throw()
    {
        weak_ptr().swap(*this);
    }

    inline void
    swap(weak_ptr<T>& other) throw()
    {
        std::swap(px, other.px);
        std::swap(pn, other.pn);
    }

private:
    element_type* px;
    reference_count* pn;
};

template <class T, class U>
bool
operator ==(
        shared_ptr<T> const& l,
        shared_ptr<U> const& r) throw()
{
    return l.get() == r.get();
}

template <class T, class U>
bool
operator !=(
        shared_ptr<T> const& l,
        shared_ptr<U> const& r) throw()
{
    return l.get() != r.get();
}

template <class T, class U>
bool
operator <=(
        shared_ptr<T> const& l,
        shared_ptr<U> const& r) throw()
{
    return l.get() <= r.get();
}

template <class T, class U>
bool
operator <(
        shared_ptr<T> const& l,
        shared_ptr<U> const& r) throw()
{
    return l.get() < r.get();
}

template <class T, class U>
bool
operator >=(
        shared_ptr<T> const& l,
        shared_ptr<U> const& r) throw()
{
    return l.get() >= r.get();
}

template <class T, class U>
bool
operator >(
        shared_ptr<T> const& l,
        shared_ptr<U> const& r) throw()
{
    return l.get() > r.get();
}

template <class T>
shared_ptr<T>
make_shared()
{
    return shared_ptr<T>(new T());
}

template <class T, class U>
shared_ptr<T>
make_shared(
        U const& u)
{
    return shared_ptr<T>(shared_ptr<U>(new U(u)));
}
#endif  // C++11+

// -- translations support ----------------------------------------------------
// since v1.7.1
class TranslationPack
{
public:
    // -- types ---------------------------------------------------------------
    typedef std::string                         key_type;
    typedef std::string                         mapped_type;
    typedef std::pair<key_type, mapped_type>    value_type;
    typedef std::list<value_type>               map_type;
    typedef map_type::iterator                  iterator;
    typedef map_type::const_iterator            const_iterator;

    TranslationPack();

    iterator
    begin() ARGPARSE_NOEXCEPT;

    iterator
    end() ARGPARSE_NOEXCEPT;

    const_iterator
    begin() const ARGPARSE_NOEXCEPT;

    const_iterator
    end() const ARGPARSE_NOEXCEPT;

    mapped_type&
    operator [](
            key_type const& key);

    mapped_type&
    at(key_type const& key);

    mapped_type const&
    at(key_type const& key) const;

    const_iterator
    find(key_type const& key) const;

    iterator
    find(key_type const& key);

private:
    // -- data ----------------------------------------------------------------
    map_type m_data;
};

// ----------------------------------------------------------------------------
template <class T>
inline std::string
_to_string(
        T const& value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template <class T>
std::vector<std::string>
_to_vecstring(
        std::vector<T> const& vec)
{
    std::vector<std::string> res;
    res.reserve(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i) {
        res.push_back(_to_string(vec.at(i)));
    }
    return res;
}

std::string
_remove_quotes(
        std::string const& str);

std::string
_replace(
        std::string str,
        std::string const& old,
        std::string const& value);

std::string
_join(std::vector<std::string> const& values,
        std::string const& separator = _spaces,
        std::string const& quotes = std::string());

std::vector<std::string>
_split(std::string const& str,
        std::string const& sep,
        int32_t maxsplit = -1);

bool
_string_to_bool(
        std::string const& str) ARGPARSE_NOEXCEPT;

template <class T>
std::string
_get_type_name()
{
#if defined(_MSC_VER) && !defined(__clang__)
    std::string res = __FUNCSIG__;
    std::string::size_type pos = res.find("__cdecl") + 8;
    return _replace(
      res.substr(pos + 33, res.size() - pos - 33 - 7), "class ", std::string());
#elif defined(__clang__) || defined(__INTEL_COMPILER)
    std::string res = __PRETTY_FUNCTION__;
    std::string::size_type pos = res.find('=') + 2;
    return res.substr(pos, res.size() - pos - 1);
#elif defined(__GNUC__)
    std::string res = __PRETTY_FUNCTION__;
    std::string::size_type pos = res.find('=') + 2;
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7))
    return res.substr(pos, res.find(';', pos) - pos);
#else
    return res.substr(pos, res.find(',', pos) - pos);
#endif  // __GNUC__, GCC >= 4.7
#else
    return std::string();
#endif  // _MSC_VER
}

class Type
{
public:
#ifdef ARGPARSE_CXX_11
    template <class T, typename enable_if<
                  is_string_ctor<T>::value
                  || is_stl_pair<T>::value
                  || is_stl_tuple<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return name<T>();
    }

    template <class T, typename enable_if<
                  is_stl_array<T>::value
                  || is_stl_container<T>::value
                  || is_stl_queue<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return basic<typename T::value_type>();
    }

    template <class T, typename enable_if<
                  is_stl_map<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return "std::pair<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T, typename enable_if<
                  !is_string_ctor<T>::value
                  && !is_stl_array<T>::value
                  && !is_stl_container<T>::value
                  && !is_stl_map<T>::value
                  && !is_stl_pair<T>::value
                  && !is_stl_queue<T>::value
                  && !is_stl_tuple<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return name<T>();
    }
#else
    template <class T>
    static std::string
    basic(typename enable_if<is_string_ctor<T>::value
                             || is_stl_pair<T>::value, bool>::type = true)
    {
        return name<T>();
    }

    template <class T>
    static std::string
    basic(typename enable_if<is_stl_container<T>::value
                             || is_stl_queue<T>::value, bool>::type = true)
    {
        return basic<typename T::value_type>();
    }

    template <class T>
    static std::string
    basic(typename enable_if<is_stl_map<T>::value, bool>::type = true)
    {
        return "std::pair<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T>
    static std::string
    basic(typename enable_if<!is_string_ctor<T>::value
                             && !is_stl_container<T>::value
                             && !is_stl_map<T>::value
                             && !is_stl_pair<T>::value
                             && !is_stl_queue<T>::value, bool>::type = true)
    {
        return name<T>();
    }
#endif  // C++11+

#ifdef ARGPARSE_CXX_11
    template <class T, typename enable_if<
                  is_string_ctor<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return "std::string";
    }

    template <class T, typename enable_if<
                  is_stl_array<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        auto str = _replace(_get_type_name<T>(), "__cxx11::", std::string());
        str = _replace(str, "__1::", std::string());
        return str.substr(0, str.find('<'))
                + "<" + name<typename T::value_type>()
                + ", " + std::to_string(std::tuple_size<T>::value) + ">";
    }

    template <class T, typename enable_if<
                  is_stl_container<T>::value
                  || is_stl_queue<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        auto str = _replace(_get_type_name<T>(), "__cxx11::", std::string());
        str = _replace(str, "__1::", std::string());
        return str.substr(0, str.find('<'))
                + "<" + name<typename T::value_type>() + ">";
    }

    template <class T, typename enable_if<
                  is_stl_map<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        auto str = _replace(_get_type_name<T>(), "__1::", std::string());
        return str.substr(0, str.find('<')) + "<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T, typename enable_if<
                  is_stl_pair<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return "std::pair<" + name<typename T::first_type>()
                + ", " + name<typename T::second_type>() + ">";
    }

    template <class T, typename enable_if<
                  is_stl_tuple<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return tuple_as_string(type_tag<T>{});
    }

    template <class T, typename enable_if<
                  !is_string_ctor<T>::value
                  && !is_stl_array<T>::value
                  && !is_stl_container<T>::value
                  && !is_stl_map<T>::value
                  && !is_stl_pair<T>::value
                  && !is_stl_queue<T>::value
                  && !is_stl_tuple<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return _get_type_name<T>();
    }
#else
    template <class T>
    static std::string
    name(typename enable_if<is_string_ctor<T>::value, bool>::type = true)
    {
        return "std::string";
    }

    template <class T>
    static std::string
    name(typename enable_if<is_stl_container<T>::value
                            || is_stl_queue<T>::value, bool>::type = true)
    {
        std::string str
                = _replace(_get_type_name<T>(), "__cxx11::", std::string());
        str = _replace(str, "__1::", std::string());
        return str.substr(0, str.find('<'))
                + "<" + name<typename T::value_type>() + ">";
    }

    template <class T>
    static std::string
    name(typename enable_if<is_stl_map<T>::value, bool>::type = true)
    {
        std::string str = _replace(_get_type_name<T>(), "__1::", std::string());
        return str.substr(0, str.find('<')) + "<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T>
    static std::string
    name(typename enable_if<is_stl_pair<T>::value, bool>::type = true)
    {
        return "std::pair<" + name<typename T::first_type>()
                + ", " + name<typename T::second_type>() + ">";
    }

    template <class T>
    static std::string
    name(typename enable_if<!is_string_ctor<T>::value
                            && !is_stl_container<T>::value
                            && !is_stl_map<T>::value
                            && !is_stl_pair<T>::value
                            && !is_stl_queue<T>::value, bool>::type = true)
    {
        return _get_type_name<T>();
    }
#endif  // C++11+

private:
#ifdef ARGPARSE_CXX_11
    template <std::size_t N>
    static std::string&
    tuple_type(
            std::string& s)
    {
        return s;
    }

    template <std::size_t N, class T, class... Ts>
    static std::string&
    tuple_type(
            std::string& s)
    {
        if (!s.empty()) {
            s += ", ";
        }
        s += name<T>();
        return tuple_type<N - 1, Ts...>(s);
    }

    template <class... Ts>
    static std::string
    tuple_as_string(
            type_tag<std::tuple<Ts...> >)
    {
        std::string res;
        tuple_type<sizeof...(Ts), Ts...>(res);
        return "std::tuple<" + res + ">";
    }
#endif  // C++11+
};

template <class T>
class Value
{
public:
    typedef T value_type;

    Value()
        : m_value(),
          m_has_value(false)
    { }

    Value(Value const& orig)
        : m_value(orig.m_value),
          m_has_value(orig.m_has_value)
    { }

    explicit
    Value(T const& orig)
        : m_value(orig),
          m_has_value(true)
    { }

    inline Value&
    operator =(
            Value const& rhs)
    {
        if (this != &rhs) {
            m_value     = rhs.m_value;
            m_has_value = rhs.m_has_value;
        }
        return *this;
    }

    inline Value&
    operator =(
            T const& rhs)
    {
        m_value     = rhs;
        m_has_value = true;
        return *this;
    }

#ifdef ARGPARSE_CXX_11
    Value(Value&& orig) ARGPARSE_NOEXCEPT
        : m_value(std::move(orig.m_value)),
          m_has_value(std::move(orig.m_has_value))
    { }

    explicit
    Value(T&& orig) ARGPARSE_NOEXCEPT
        : m_value(std::move(orig)),
          m_has_value(true)
    { }

    inline Value&
    operator =(
            Value&& rhs) ARGPARSE_NOEXCEPT
    {
        if (this != &rhs) {
            m_value     = std::move(rhs.m_value);
            m_has_value = std::move(rhs.m_has_value);
        }
        return *this;
    }

    inline Value&
    operator =(
            T&& rhs) ARGPARSE_NOEXCEPT
    {
        m_value     = std::move(rhs);
        m_has_value = true;
        return *this;
    }
#endif  // C++11+

    inline void
    reset(T const& value = T())
    {
        m_value     = value;
        m_has_value = false;
    }

    inline bool
    has_value() const ARGPARSE_NOEXCEPT
    {
        return m_has_value;
    }

    inline T const&
    value() const ARGPARSE_NOEXCEPT
    {
        return m_value;
    }

    inline T const
    value_or(
            T const& value) const ARGPARSE_NOEXCEPT
    {
        return has_value() ? this->value() : value;
    }

private:
    // -- data ----------------------------------------------------------------
    T       m_value;
    bool    m_has_value;
};

template <class T>
class SValue
{
public:
    typedef T value_type;

    SValue()
        : m_value(),
          m_has_value(false),
          m_suppress(false)
    { }

    SValue(SValue const& orig)
        : m_value(orig.m_value),
          m_has_value(orig.m_has_value),
          m_suppress(orig.m_suppress)
    { }

    explicit
    SValue(T const& orig)
        : m_value(orig),
          m_has_value(true),
          m_suppress(false)
    { }

    inline SValue&
    operator =(
            SValue const& rhs)
    {
        if (this != &rhs) {
            m_value     = rhs.m_value;
            m_has_value = rhs.m_has_value;
            m_suppress  = rhs.m_suppress;
        }
        return *this;
    }

    inline SValue&
    operator =(
            T const& rhs)
    {
        m_value     = rhs;
        m_has_value = true;
        m_suppress  = false;
        return *this;
    }

    inline SValue&
    operator =(
            _SUPPRESS) ARGPARSE_NOEXCEPT
    {
        m_suppress = true;
        return *this;
    }

#ifdef ARGPARSE_CXX_11
    SValue(SValue&& orig) ARGPARSE_NOEXCEPT
        : m_value(std::move(orig.m_value)),
          m_has_value(std::move(orig.m_has_value)),
          m_suppress(std::move(orig.m_suppress))
    { }

    explicit
    SValue(T&& orig) ARGPARSE_NOEXCEPT
        : m_value(std::move(orig)),
          m_has_value(true),
          m_suppress(false)
    { }

    inline SValue&
    operator =(
            SValue&& rhs) ARGPARSE_NOEXCEPT
    {
        if (this != &rhs) {
            m_value     = std::move(rhs.m_value);
            m_has_value = std::move(rhs.m_has_value);
            m_suppress  = std::move(rhs.m_suppress);
        }
        return *this;
    }

    inline SValue&
    operator =(
            T&& rhs) ARGPARSE_NOEXCEPT
    {
        m_value     = std::move(rhs);
        m_has_value = true;
        m_suppress  = false;
        return *this;
    }
#endif  // C++11+

    inline void
    reset(T const& value = T())
    {
        m_value     = value;
        m_has_value = false;
        m_suppress  = false;
    }

    inline bool
    suppress() const ARGPARSE_NOEXCEPT
    {
        return m_suppress;
    }

    inline bool
    has_value() const ARGPARSE_NOEXCEPT
    {
        return m_has_value;
    }

    inline T const&
    value() const ARGPARSE_NOEXCEPT
    {
        return m_value;
    }

    inline T&
    will_have() ARGPARSE_NOEXCEPT
    {
        m_has_value = true;
        m_suppress  = false;
        return m_value;
    }

private:
    // -- data ----------------------------------------------------------------
    T       m_value;
    bool    m_has_value;
    bool    m_suppress;
};

bool
_is_type_correct(
        std::string const& expected,
        std::string const& received) ARGPARSE_NOEXCEPT;

void
_check_type(
        Value<std::string> const& expected,
        std::string const& received);

void
_check_non_count_action(
        std::string const& key,
        Action action);
}  // namespace detail

// Forward declaration
class Argument;
class ArgumentParser;

/*!
 *  \brief Formatter for generating usage messages and argument help strings
 */
ARGPARSE_EXPORT class HelpFormatter
{
    friend class ArgumentParser;

    typedef detail::shared_ptr<Argument> pArgument;

    static std::size_t const c_default_tab_size = 4;

public:
    HelpFormatter()
        : m_tab_size(c_default_tab_size)
    { }
    virtual ~HelpFormatter() ARGPARSE_NOEXCEPT { }

    inline void
    _tab_size(
            std::size_t value) ARGPARSE_NOEXCEPT
    {
        m_tab_size = value != 0 ? value : c_default_tab_size;
    }

    inline std::size_t
    _tab_size() const ARGPARSE_NOEXCEPT
    {
        return m_tab_size;
    }

    virtual std::string
    _fill_text(
            std::string const& text,
            std::size_t width,
            std::size_t indent) const;

    virtual std::string
    _get_default_metavar_for_optional(
            Argument const* action) const;

    virtual std::string
    _get_default_metavar_for_positional(
            Argument const* action) const;

    virtual std::string
    _get_help_string(
            Argument const* action,
            std::string const& lang) const;

    virtual std::vector<std::string>
    _split_lines(
            std::string const& text,
            std::size_t width) const;

private:
    struct CompletionInfo
    {
        CompletionInfo()
            : args(),
              options()
        { }

        // -- data ------------------------------------------------------------
        std::string args;
        std::vector<std::pair<pArgument, std::string> > options;
    };

    CompletionInfo
    _bash_completion_info(
            ArgumentParser const* parser) const;

    void
    _print_parser_completion(
            std::ostream& os,
            ArgumentParser const* parser,
            std::string const& prog,
            bool is_root) const;

    void
    _print_custom_usage(
            std::ostream& os,
            ArgumentParser const* parser,
            std::string const& usage_title) const;

    std::string
    _format_bash_completion(
            ArgumentParser const* parser) const;

    std::string
    _format_usage(
            ArgumentParser const* parser,
            std::string const& lang) const;

    std::string
    _format_help(
            ArgumentParser const* parser,
            std::string const& lang) const;

    // -- data ----------------------------------------------------------------
    std::size_t m_tab_size;
};

/*!
 *  \brief Help message formatter which retains any formatting in descriptions
 */
ARGPARSE_EXPORT
class _RawDescriptionHelpFormatter : virtual public HelpFormatter
{
public:
    ~_RawDescriptionHelpFormatter() ARGPARSE_NOEXCEPT ARGPARSE_OVERRIDE { }

    std::string
    _fill_text(
            std::string const& text,
            std::size_t width,
            std::size_t indent) const ARGPARSE_OVERRIDE;

protected:
    std::vector<std::string>
    _split_lines_raw(
            std::string const& text,
            std::size_t width) const;
} ARGPARSE_INLINE_VARIABLE RawDescriptionHelpFormatter;

/*!
 *  \brief Help message formatter which retains formatting of all help text
 */
ARGPARSE_EXPORT
class _RawTextHelpFormatter : virtual public _RawDescriptionHelpFormatter
{
public:
    ~_RawTextHelpFormatter() ARGPARSE_NOEXCEPT ARGPARSE_OVERRIDE { }

    std::vector<std::string>
    _split_lines(
            std::string const& text,
            std::size_t width) const ARGPARSE_OVERRIDE;
} ARGPARSE_INLINE_VARIABLE RawTextHelpFormatter;

/*!
 *  \brief Help message formatter which adds default values to argument help
 */
ARGPARSE_EXPORT
class _ArgumentDefaultsHelpFormatter : virtual public HelpFormatter
{
public:
    ~_ArgumentDefaultsHelpFormatter() ARGPARSE_NOEXCEPT ARGPARSE_OVERRIDE { }

    std::string
    _get_help_string(
            Argument const* action,
            std::string const& lang) const ARGPARSE_OVERRIDE;
} ARGPARSE_INLINE_VARIABLE ArgumentDefaultsHelpFormatter;

/*!
 *  \brief Help message formatter which uses the argument 'type' as the default
 *  metavar value (instead of the argument 'dest')
 */
ARGPARSE_EXPORT
class _MetavarTypeHelpFormatter : virtual public HelpFormatter
{
public:
    ~_MetavarTypeHelpFormatter() ARGPARSE_NOEXCEPT ARGPARSE_OVERRIDE { }

    std::string
    _get_default_metavar_for_optional(
            Argument const* action) const ARGPARSE_OVERRIDE;

    std::string
    _get_default_metavar_for_positional(
            Argument const* action) const ARGPARSE_OVERRIDE;
} ARGPARSE_INLINE_VARIABLE MetavarTypeHelpFormatter;

// -- standAlone functions ----------------------------------------------------
/*!
 *  \brief Split string with command line arguments to arguments container
 *
 *  \param str String to process
 *  \param err Output stream for errors (default: std::cerr)
 *
 *  \since v1.9.0
 *
 *  \return Container with arguments
 */
ARGPARSE_EXPORT std::vector<std::string>
split_to_args(
        std::string const& str,
        std::ostream& err = std::cerr);

// Forward declaration
class _ArgumentData;

/*!
 *  \brief Argument class
 */
ARGPARSE_EXPORT class Argument
{
    friend class _ArgumentData;
    friend class _ArgumentDefaultsHelpFormatter;
    friend class _ArgumentGroup;
    friend class _Storage;
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class HelpFormatter;
    friend class MutuallyExclusiveGroup;
    friend class Namespace;

    enum Nargs ARGPARSE_ENUM_TYPE(uint8_t)
    {
        NARGS_DEF       = 0x01,  // ""
        NARGS_NUM       = 0x02,  // "N"
        ONE_OR_MORE     = 0x04,  // "+"
        ZERO_OR_ONE     = 0x08,  // "?"
        ZERO_OR_MORE    = 0x10,  // "*"
        REMAINDING      = 0x20,  // argparse::REMAINDER
        SUPPRESSING     = 0x40,  // argparse::SUPPRESS
    };

    static uint8_t ARGPARSE_USE_CONSTEXPR
    _NARGS_COMBINED = NARGS_NUM | ONE_OR_MORE | ZERO_OR_ONE | ZERO_OR_MORE;

    enum Type ARGPARSE_ENUM_TYPE(uint8_t)
    {
        NoType      = 0x00,
        Positional  = 0x01,
        Operand     = 0x02,
        Optional    = 0x04
    };

    explicit
    Argument(
            std::vector<std::string> const& flags,
            std::string const& name,
            Type type);

    static detail::shared_ptr<Argument>
    make_argument(
            std::vector<std::string> const& flags,
            std::string const& name,
            Type type);

#ifdef ARGPARSE_CXX_11
    explicit
    Argument(
            std::vector<std::string>&& flags,
            std::string&& name,
            Type type);

    static detail::shared_ptr<Argument>
    make_argument(
            std::vector<std::string>&& flags,
            std::string&& name,
            Type type);
#endif  // C++11+

public:
#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
    template <class... Args>
    explicit
    Argument(
            Args... flags)
        : Argument(std::vector<std::string>{ flags... })
    { }

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
    explicit
    Argument(
            std::initializer_list<std::string> const& flags)
        : Argument(std::vector<std::string>{ flags })
    { }
#else
    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flag Argument flag
     *
     *  \return Argument object
     */
    explicit
    Argument(
            std::string const& flag);

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flag1 First flag
     *  \param flag2 Second flag
     *
     *  \return Argument object
     */
    explicit
    Argument(
            std::string const& flag1,
            std::string const& flag2);

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flag1 First flag
     *  \param flag2 Second flag
     *  \param flag3 Third flag
     *
     *  \since v1.7.2
     *
     *  \return Argument object
     */
    explicit
    Argument(
            std::string const& flag1,
            std::string const& flag2,
            std::string const& flag3);

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flag1 First flag
     *  \param flag2 Second flag
     *  \param flag3 Third flag
     *  \param flag4 Fourth flag
     *
     *  \since v1.7.2
     *
     *  \return Argument object
     */
    explicit
    Argument(
            std::string const& flag1,
            std::string const& flag2,
            std::string const& flag3,
            std::string const& flag4);
#endif  // C++11+

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
    explicit
    Argument(
            std::vector<std::string> const& flags);

    /*!
     *  \brief Construct argument object from another argument
     *
     *  \param orig Argument object to copy
     *
     *  \return Argument object
     */
    Argument(
            Argument const& orig);

    /*!
     *  \brief Copy argument object from another argument
     *
     *  \param rhs Argument object to copy
     *
     *  \return Current argument reference
     */
    Argument&
    operator =(
            Argument const& rhs);

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Construct argument object from another argument
     *
     *  \param orig Argument object to move
     *
     *  \return Argument object
     */
    Argument(
            Argument&& orig) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Move argument object from another argument
     *
     *  \param rhs Argument object to move
     *
     *  \return Current argument reference
     */
    Argument&
    operator =(
            Argument&& rhs) ARGPARSE_NOEXCEPT;
#endif  // C++11+

    /*!
     *  \brief Set argument 'action' value (default: "store")
     *
     *  \param value Action value
     *
     *  \return Current argument reference
     */
    Argument&
    action(std::string const& value);

    /*!
     *  \brief Set argument 'action' value (default: argparse::store)
     *
     *  \param value Action value
     *
     *  \return Current argument reference
     */
    Argument&
    action(Action value);

    /*!
     *  \brief Set argument 'nargs' value
     *
     *  \param num Number of arguments
     *
     *  \return Current argument reference
     */
    Argument&
    nargs(std::size_t num);

    /*!
     *  \brief Set argument 'nargs' value
     *  (combined nargs if num > 1)
     *
     *  \param value Nargs value: "?", "*", "+"
     *  \param num Number of arguments (default: 1)
     *
     *  \return Current argument reference
     */
    Argument&
    nargs(std::string const& value,
            std::size_t num = 1);

    /*!
     *  \brief Set argument 'nargs' value
     *
     *  \param value argparse::REMAINDER
     *
     *  \return Current argument reference
     */
    Argument&
    nargs(_REMAINDER value);

    /*!
     *  \brief Suppress argument 'nargs' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \since v1.8.0
     *
     *  \return Current argument reference
     */
    Argument&
    nargs(_SUPPRESS value);

    /*!
     *  \brief Set argument 'nargs' optional ("?") value
     *  (combined nargs if num > 1)
     *
     *  \param num Number of arguments (default: 1)
     *
     *  \return Current argument reference
     */
    inline Argument&
    optional(
            std::size_t num = 1)
    {
        return nargs("?", num);
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_one ("?") value
     *  (combined nargs if num > 1)
     *
     *  \param num Number of arguments (default: 1)
     *
     *  \return Current argument reference
     */
    inline Argument&
    zero_or_one(
            std::size_t num = 1)
    {
        return nargs("?", num);
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_more ("*") value
     *  (combined nargs if num > 1)
     *
     *  \param num Number of arguments (default: 1)
     *
     *  \return Current argument reference
     */
    inline Argument&
    zero_or_more(
            std::size_t num = 1)
    {
        return nargs("*", num);
    }

    /*!
     *  \brief Set argument 'nargs' one_or_more ("+") value
     *  (combined nargs if num > 1)
     *
     *  \param num Number of arguments (default: 1)
     *
     *  \return Current argument reference
     */
    inline Argument&
    one_or_more(
            std::size_t num = 1)
    {
        return nargs("+", num);
    }

    /*!
     *  \brief Set argument 'nargs' argparse::REMAINDER value
     *
     *  \return Current argument reference
     */
    inline Argument&
    remainder()
    {
        return nargs(argparse::REMAINDER);
    }

    /*!
     *  \brief Set argument 'nargs' argparse::SUPPRESS value
     *
     *  \since v1.9.0
     *
     *  \return Current argument reference
     */
    inline Argument&
    suppresser()
    {
        return nargs(argparse::SUPPRESS);
    }

    /*!
     *  \brief Set argument 'const' value
     *
     *  \param value Const value
     *
     *  \return Current argument reference
     */
    Argument&
    const_value(
            std::string const& value);

    /*!
     *  \brief Set custom argument 'const' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Const value
     *
     *  \return Current argument reference
     */
#ifdef ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    inline Argument&
    const_value(
            T const& value)
#else
    template <class T>
    inline Argument&
    const_value(
            T const& value,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
#endif  // C++11+
    {
        const_value(detail::_to_string<T>(value));
        m_type_name = detail::Type::basic<T>();
        return *this;
    }

    /*!
     *  \brief Set argument 'default' value
     *
     *  \param value Default value
     *
     *  \return Current argument reference
     */
    Argument&
    default_value(
            std::string const& value);

    /*!
     *  \brief Set custom argument 'default' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Default value
     *
     *  \return Current argument reference
     */
#ifdef ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    inline Argument&
    default_value(
            T const& value)
#else
    template <class T>
    inline Argument&
    default_value(
            T const& value,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
#endif  // C++11+
    {
        m_default = detail::_to_string<T>(value);
        m_type_name = detail::Type::basic<T>();
        return *this;
    }

    /*!
     *  \brief Suppress argument 'default' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument reference
     */
    Argument&
    default_value(
            _SUPPRESS value);

    /*!
     *  \brief Set argument 'implicit' value (used with nargs="?" or "*",
     *  const_value alternative for optional arguments with nargs="?")
     *
     *  \param value Implicit value
     *
     *  \return Current argument reference
     */
    Argument&
    implicit_value(
            std::string const& value);

    /*!
     *  \brief Set custom argument 'implicit' value (used with nargs="?" or "*",
     *  const_value alternative for optional arguments with nargs="?")
     *  and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Implicit value
     *
     *  \return Current argument reference
     */
#ifdef ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    inline Argument&
    implicit_value(
            T const& value)
#else
    template <class T>
    inline Argument&
    implicit_value(
            T const& value,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
#endif  // C++11+
    {
        m_implicit = detail::_to_string<T>(value);
        m_type_name = detail::Type::basic<T>();
        return *this;
    }

    /*!
     *  \brief Set argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \return Current argument reference
     */
    template <class T>
    inline Argument&
    type()
    {
        m_type_name = detail::Type::basic<T>();
        return *this;
    }

    /*!
     *  \brief Set argument 'type' name (for MetavarTypeHelpFormatter)
     *
     *  \param value Type name value
     *
     *  \return Current argument reference
     */
    Argument&
    type(std::string const& value);

    /*!
     *  \brief Set argument 'type' factory function.
     *  Called in Namespace::get/try_get<>
     *
     *  \param func Type factory function
     *
     *  \since v1.8.4
     *
     *  \return Current argument reference
     */
    Argument&
    type(detail::func2<std::string const&, void*>::type func) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choice value
     *
     *  \since v1.7.1
     *
     *  \return Current argument reference
     */
    Argument&
    choice(std::string const& value);

    /*!
     *  \brief Set argument 'choices' value. Each character is a separate choice
     *
     *  \param value Choices value
     *
     *  \return Current argument reference
     */
    Argument&
    choices(std::string const& value);

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Choice value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    inline Argument&
    choice(T const& value)
    {
        m_type_name = detail::Type::basic<T>();
        return choices(std::vector<std::string>{ detail::_to_string(value) });
    }

    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param args Other values
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T, class... Args,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    inline Argument&
    choices(T const& value1,
            T const& value2,
            Args... args)
    {
        return choices(std::vector<T>{ value1, value2, args... });
    }

    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Choices value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    Argument&
    choices(std::initializer_list<T> const& value)
    {
        return choices(std::vector<T>{ value });
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param args Other values
     *
     *  \since v1.7.1
     *
     *  \return Current argument reference
     */
    template <class... Args>
    inline Argument&
    choices(std::string const& value1,
            std::string const& value2,
            Args... args)
    {
        return choices(std::vector<std::string>{ value1, value2, args... });
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choices value
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    choices(std::initializer_list<std::string> const& value);
#else
    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Choice value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T>
    inline Argument&
    choice(T const& value,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
    {
        m_type_name = detail::Type::basic<T>();
        std::vector<std::string> res;
        res.push_back(detail::_to_string(value));
        return choices(res);
    }

    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value1 First value
     *  \param value2 Second value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T>
    inline Argument&
    choices(T const& value1,
            T const& value2,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
    {
        m_type_name = detail::Type::basic<T>();
        std::vector<std::string> res;
        res.push_back(detail::_to_string(value1));
        res.push_back(detail::_to_string(value2));
        return choices(res);
    }

    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T>
    inline Argument&
    choices(T const& value1,
            T const& value2,
            T const& value3,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
    {
        m_type_name = detail::Type::basic<T>();
        std::vector<std::string> res;
        res.push_back(detail::_to_string(value1));
        res.push_back(detail::_to_string(value2));
        res.push_back(detail::_to_string(value3));
        return choices(res);
    }

    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *  \param value4 Fourth value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
    template <class T>
    inline Argument&
    choices(T const& value1,
            T const& value2,
            T const& value3,
            T const& value4,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
    {
        m_type_name = detail::Type::basic<T>();
        std::vector<std::string> res;
        res.push_back(detail::_to_string(value1));
        res.push_back(detail::_to_string(value2));
        res.push_back(detail::_to_string(value3));
        res.push_back(detail::_to_string(value4));
        return choices(res);
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *
     *  \since v1.7.1
     *
     *  \return Current argument reference
     */
    Argument&
    choices(std::string const& value1,
            std::string const& value2);

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *
     *  \since v1.7.1
     *
     *  \return Current argument reference
     */
    Argument&
    choices(std::string const& value1,
            std::string const& value2,
            std::string const& value3);

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *  \param value4 Fourth value
     *
     *  \since v1.7.1
     *
     *  \return Current argument reference
     */
    Argument&
    choices(std::string const& value1,
            std::string const& value2,
            std::string const& value3,
            std::string const& value4);
#endif  // C++11+

    /*!
     *  \brief Set argument 'choices' value and argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get/try_get<T> type check)
     *
     *  \param value Choices value
     *
     *  \since v1.9.3
     *
     *  \return Current argument reference
     */
#ifdef ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                  !detail::is_string_ctor<T>::value>::type* = nullptr>
    inline Argument&
    choices(std::vector<T> const& value)
#else
    template <class T>
    inline Argument&
    choices(std::vector<T> const& value,
            typename detail::enable_if<
                !detail::is_string_ctor<T>::value, bool>::type = true)
#endif  // C++11+
    {
        m_type_name = detail::Type::basic<T>();
        return choices(detail::_to_vecstring(value));
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choices value
     *
     *  \return Current argument reference
     */
    Argument&
    choices(std::vector<std::string> const& value);

    /*!
     *  \brief Set 'required' value for non-positionals arguments
     *
     *  \param value Required flag
     *
     *  \return Current argument reference
     */
    Argument&
    required(
            bool value);

    /*!
     *  \brief Set argument 'help' message for selected language
     *
     *  \param value Help message
     *  \param lang Language value (default: "")
     *
     *  \return Current argument reference
     */
    Argument&
    help(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Suppress argument 'help' message
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument reference
     */
    Argument&
    help(_SUPPRESS value);

    /*!
     *  \brief Set argument 'version' for arguments with 'version' action
     *
     *  \param value Version value
     *
     *  \return Current argument reference
     */
    Argument&
    version(std::string const& value);

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar values
     *
     *  \return Current argument reference
     */
    Argument&
    metavar(std::vector<std::string> const& value);

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value First value
     *  \param args Other values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    inline Argument&
    metavar(std::string const& value,
            Args... args)
    {
        return metavar(std::vector<std::string>{ value, args... });
    }

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar values
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    metavar(std::initializer_list<std::string> const& value);
#else
    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar value
     *
     *  \return Current argument reference
     */
    Argument&
    metavar(std::string const& value);

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *
     *  \return Current argument reference
     */
    Argument&
    metavar(std::string const& value1,
            std::string const& value2);

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *
     *  \return Current argument reference
     */
    Argument&
    metavar(std::string const& value1,
            std::string const& value2,
            std::string const& value3);

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *  \param value4 Fourth value
     *
     *  \return Current argument reference
     */
    Argument&
    metavar(std::string const& value1,
            std::string const& value2,
            std::string const& value3,
            std::string const& value4);
#endif  // C++11+

    /*!
     *  \brief Set argument 'dest' value for non-positionals arguments
     *
     *  \param value Destination value
     *
     *  \return Current argument reference
     */
    Argument&
    dest(std::string const& value);

    /*!
     *  \brief Set argument 'handle' function.
     *  Called when the argument is present and passed the value of the argument
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    Argument&
    handle(detail::func1<std::string const&>::type func) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument flags values
     *
     *  \return Argument flags values
     */
    ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    flags() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'action' value (default: argparse::store)
     *
     *  \return Argument 'action' value
     */
    ARGPARSE_ATTR_NODISCARD
    Action
    action() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'nargs' value
     *
     *  \return Argument 'nargs' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    nargs() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'const' value
     *
     *  \return Argument 'const' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    const_value() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'default' value
     *
     *  \return Argument 'default' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    default_value() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'implicit' value (used with nargs="?" or "*",
     *  const_value alternative for optional arguments with nargs="?")
     *
     *  \return Argument 'implicit' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    implicit_value() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'type' name (for MetavarTypeHelpFormatter)
     *
     *  \return Argument 'type' name
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    type_name() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'choices' value
     *
     *  \return Argument 'choices' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    choices() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'required' value
     *
     *  \return Argument 'required' value
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    required() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'help' message for default language
     *
     *  \return Argument 'help' message
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    help() const;

    /*!
     *  \brief Get argument 'version' value
     *
     *  \return Argument 'version' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    version() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'metavar' value
     *
     *  \return Argument 'metavar' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    metavar() const;

    /*!
     *  \brief Get argument 'dest' value
     *
     *  \return Argument 'dest' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    dest() const ARGPARSE_NOEXCEPT;

private:
    void
    handle(std::string const& str) const;

    void
    validate() const;

    void
    prepare_action(
            Action value);

    void
    make_no_flags();

    std::string
    usage(HelpFormatter const& formatter) const;

    std::string
    flags_to_string(
            HelpFormatter const& formatter) const;

    std::string
    get_choices() const;

    std::string
    get_const() const;

    std::string
    get_default() const;

    std::string const&
    get_dest() const ARGPARSE_NOEXCEPT;

    std::string
    get_metavar() const;

    std::string
    get_nargs() const;

    std::string
    option_strings() const;

    std::string
    get_required() const;

    std::string
    get_type() const;

    std::string
    print(HelpFormatter const& formatter,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const;

    void
    process_nargs_suffix(
            std::string& res,
            HelpFormatter const& formatter) const;

    std::vector<std::string>
    get_argument_name(
            HelpFormatter const& formatter) const;

    std::vector<std::string> const&
    get_argument_flags() const ARGPARSE_NOEXCEPT;

    void
    resolve_conflict_flags(
            std::vector<std::string> const& flags);

    std::string
    error_nargs(
            std::string const& arg) const;

    bool
    is_match_name(
            std::string const& value) const;

    bool
    operator ==(
            Argument const& rhs) const;

    bool
    operator ==(
            std::string const& rhs) const;

    // -- data ----------------------------------------------------------------
    std::vector<std::string>    m_flags;
    std::vector<std::string>    m_all_flags;
    std::string                 m_name;
    std::size_t                 m_num_args;
    std::string                 m_nargs_str;
    detail::Value<std::string>  m_const;
    detail::SValue<std::string> m_default;
    detail::Value<std::string>  m_implicit;
    detail::Value<std::string>  m_type_name;
    detail::Value<std::vector<std::string> > m_choices;
    detail::SValue<detail::TranslationPack>  m_help;
    detail::Value<std::string>  m_version;
    detail::Value<std::vector<std::string> > m_metavar;
    std::vector<std::string>    m_dest;
    detail::func1<std::string const&>::type m_handle;
    detail::func2<std::string const&, void*>::type m_factory;
    detail::weak_ptr<_ArgumentData> m_post_trigger;
    uint16_t                    m_action;
    uint8_t                     m_type;
    uint8_t                     m_nargs;
    detail::Value<bool>         m_required;
};

/*!
 *  \brief _Group class
 */
class _Group
{
    friend class HelpFormatter;

protected:
    explicit
    _Group(std::string const& title,
            std::string const& description);

public:
    /*!
     *  \brief Destroy group
     */
    virtual ~_Group() ARGPARSE_NOEXCEPT { }

    /*!
     *  \brief Get group 'title' value for default language
     *
     *  \return Group 'title' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    title() const;

    /*!
     *  \brief Get group 'description' value for default language
     *
     *  \return Group 'description' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    description() const;

protected:
    virtual void
    limit_help_flags(
            HelpFormatter const& formatter,
            std::size_t& limit) const                                       = 0;

    virtual void
    print_help(
            std::ostream& os,
            bool& eat_ln,
            HelpFormatter const& formatter,
            std::string const& prog,
            bool required,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const                                  = 0;

    // -- data ----------------------------------------------------------------
    detail::TranslationPack m_title;
    detail::TranslationPack m_description;
};

/*!
 *  \brief _ArgumentData class
 */
class _ArgumentData
{
    friend class _ArgumentGroup;
    friend class Argument;
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class HelpFormatter;
    friend class MutuallyExclusiveGroup;

    typedef detail::shared_ptr<Argument> pArgument;
    typedef std::list<pArgument>::const_iterator arg_iterator;
    typedef std::list<std::pair<pArgument, bool> >::const_iterator sub_iterator;

    _ArgumentData();

    static detail::shared_ptr<_ArgumentData>
    make_argument_data();

    void
    check_conflict_arg(
            Argument const* arg);

    void
    update_help(
            bool add_help,
            std::string const& prefix_chars);

    std::vector<pArgument>
    get_arguments(
            bool add_suppress) const;

    std::vector<pArgument>
    get_optional(
            bool add_suppress,
            bool add_group) const;

    std::vector<pArgument>
    get_operand(
            bool add_suppress,
            bool add_group) const;

    std::vector<pArgument>
    get_positional(
            bool add_suppress,
            bool add_group) const;

    void
    check_conflicting_option(
            Argument const* arg,
            std::vector<std::string>& flags) const;

    void
    merge_arguments(
            _ArgumentData const& data);

    static void
    create_argument(
            detail::shared_ptr<_ArgumentData>& data,
            std::vector<std::string> const& in_flags,
            std::string const& prefix_chars);

    void
    validate_argument(
            Argument const& argument,
            std::string const& prefix_chars);

    // -- data ----------------------------------------------------------------
    std::string m_conflict_handler_str;
    std::list<pArgument> m_arguments;
    std::list<std::pair<pArgument, bool> > m_optional;
    std::list<std::pair<pArgument, bool> > m_operand;
    std::list<std::pair<pArgument, bool> > m_positional;
    uint8_t m_conflict_handler;
    bool m_add_help;
    bool m_help_added;
};

/*!
 *  \brief _ArgumentGroup class
 */
class _ArgumentGroup
{
protected:
    typedef detail::shared_ptr<Argument> pArgument;
    typedef detail::shared_ptr<_ArgumentData> pArgumentData;
    typedef std::list<pArgument>::const_iterator arg_iterator;
    typedef std::list<std::pair<pArgument, bool> >::const_iterator sub_iterator;

    explicit
    _ArgumentGroup(
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::SValue<std::string>& argument_default,
            bool is_mutex_group);

public:
    /*!
     *  \brief Create argument group object from another argument group
     *
     *  \param orig Argument group object to copy
     *
     *  \return Argument group object
     */
    _ArgumentGroup(
            _ArgumentGroup const& orig);

    /*!
     *  \brief Destroy argument group object
     */
    virtual ~_ArgumentGroup() ARGPARSE_NOEXCEPT { }

    /*!
     *  \brief Copy argument group object from another argument group
     *
     *  \param rhs Argument group object to copy
     *
     *  \return Current argument group reference
     */
    _ArgumentGroup&
    operator =(
            _ArgumentGroup const& rhs);

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flag values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    inline Argument&
    add_argument(
            Args... flags)
    {
        return add_argument(std::vector<std::string>{ flags... });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flag values
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::initializer_list<std::string> const& flags);
#else
    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag);

    /*!
     *  \brief Add argument with 2 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag1,
            std::string const& flag2);

    /*!
     *  \brief Add argument with 3 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *  \param flag3 Third value
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag1,
            std::string const& flag2,
            std::string const& flag3);

    /*!
     *  \brief Add argument with 4 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *  \param flag3 Third value
     *  \param flag4 Fourth value
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag1,
            std::string const& flag2,
            std::string const& flag3,
            std::string const& flag4);
#endif  // C++11+

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::vector<std::string> const& flags);

protected:
    void
    process_add_argument();

    // -- data ----------------------------------------------------------------
    pArgumentData m_data;
    std::string& m_prefix_chars;
    pArgumentData& m_parent_data;
    detail::SValue<std::string>& m_argument_default;

private:
    bool m_is_mutex_group;
};

/*!
 *  \brief ArgumentGroup class
 */
ARGPARSE_EXPORT class ArgumentGroup : public _Group, public _ArgumentGroup
{
    friend class ArgumentParser;

    explicit
    ArgumentGroup(
            std::string const& title,
            std::string const& description,
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::SValue<std::string>& argument_default);

    static detail::shared_ptr<ArgumentGroup>
    make_argument_group(
            std::string const& title,
            std::string const& description,
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::SValue<std::string>& argument_default);

public:
    using _ArgumentGroup::add_argument;
    using _Group::title;
    using _Group::description;

    /*!
     *  \brief Create argument group object from another argument group
     *
     *  \param orig Argument group object to copy
     *
     *  \return Argument group object
     */
    ArgumentGroup(
            ArgumentGroup const& orig);

    /*!
     *  \brief Copy argument group object from another argument group
     *
     *  \param rhs Argument group object to copy
     *
     *  \return Current argument group reference
     */
    ArgumentGroup&
    operator =(
            ArgumentGroup const& rhs);

    /*!
     *  \brief Set argument group 'title' value for selected language
     *
     *  \param value Title value
     *  \param lang Language value (default: "")
     *
     *  \return Current argument group reference
     */
    ArgumentGroup&
    title(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set argument group 'description' value for selected language
     *
     *  \param value Description value
     *  \param lang Language value (default: "")
     *
     *  \return Current argument group reference
     */
    ArgumentGroup&
    description(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current argument group reference
     */
    ArgumentGroup&
    add_argument(
            Argument const& argument);

private:
    void
    limit_help_flags(
            HelpFormatter const& formatter,
            std::size_t& limit) const ARGPARSE_OVERRIDE;

    void
    print_help(
            std::ostream& os,
            bool& eat_ln,
            HelpFormatter const& formatter,
            std::string const& prog,
            bool required,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const ARGPARSE_OVERRIDE;
};

/*!
 *  \brief MutuallyExclusiveGroup class
 */
ARGPARSE_EXPORT class MutuallyExclusiveGroup : public _ArgumentGroup
{
    friend class ArgumentParser;
    friend class HelpFormatter;

    explicit
    MutuallyExclusiveGroup(
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::SValue<std::string>& argument_default);

    static MutuallyExclusiveGroup
    make_mutex_group(
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::SValue<std::string>& argument_default);

public:
    using _ArgumentGroup::add_argument;

    /*!
     *  \brief Create mutually exclusive group object from another
     *   mutually exclusive group
     *
     *  \param orig Mutually exclusive group object to copy
     *
     *  \return Mutually exclusive group object
     */
    MutuallyExclusiveGroup(
            MutuallyExclusiveGroup const& orig);

    /*!
     *  \brief Copy mutually exclusive group object from another
     *  mutually exclusive group
     *
     *  \param rhs Mutually exclusive group object to copy
     *
     *  \return Current mutually exclusive group reference
     */
    MutuallyExclusiveGroup&
    operator =(
            MutuallyExclusiveGroup const& rhs);

    /*!
     *  \brief Set mutually exclusive group 'required' value
     *
     *  \param value Required flag
     *
     *  \return Current mutually exclusive group reference
     */
    MutuallyExclusiveGroup&
    required(
            bool value) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get mutually exclusive group 'required' value
     *
     *  \return Mutually exclusive group 'required' value
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    required() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current mutually exclusive group reference
     */
    MutuallyExclusiveGroup&
    add_argument(
            Argument const& argument);

private:
    std::string
    usage(HelpFormatter const& formatter) const;

    // -- data ----------------------------------------------------------------
    bool m_required;
};

/*!
 *  \brief _Storage class
 */
class _Storage
{
    friend class ArgumentParser;
    friend class Namespace;

    class mapped_type
    {
    public:
        mapped_type();

        explicit
        mapped_type(
                std::vector<std::string> const& values);

        void
        clear();

        bool
        exists() const ARGPARSE_NOEXCEPT;

        bool
        is_default() const ARGPARSE_NOEXCEPT;

        std::vector<std::string> const&
        operator ()() const ARGPARSE_NOEXCEPT;

        std::size_t
        size() const ARGPARSE_NOEXCEPT;

        bool
        empty() const ARGPARSE_NOEXCEPT;

        std::string const&
        front() const ARGPARSE_NOEXCEPT;

        std::string const&
        at(std::size_t i) const;

        void
        push_back(
                std::string const& value,
                bool is_default = false);

        void
        push_values(
                std::vector<std::string> const& values);

        std::vector<std::size_t> const&
        indexes() const ARGPARSE_NOEXCEPT;

        std::vector<std::string>
        sub_values(
                std::size_t i) const;

    private:
        // -- data ------------------------------------------------------------
        bool m_exists;
        bool m_is_default;
        std::vector<std::string> m_values;
        std::vector<std::size_t> m_indexes;
    };

    typedef detail::shared_ptr<Argument>            key_type;

public:
    typedef std::pair<key_type, mapped_type>        value_type;

private:
    typedef std::list<value_type>                   map_type;
    typedef map_type::iterator                      iterator;
    typedef map_type::const_iterator                const_iterator;

    _Storage();

    void
    create(key_type const& key,
            mapped_type const& value = mapped_type());

    void
    create(std::vector<key_type> const& keys);

    void
    have_value(
            key_type const& key);

    void
    store_value(
            key_type const& key,
            std::string const& value);

    void
    store_values(
            key_type const& key,
            std::vector<std::string> const& values);

    void
    store_default_value(
            key_type const& key);

    bool
    self_value_stored(
            key_type const& key);

    bool
    exists_arg(
            std::string const& key) const;

    bool
    exists(std::string const& key) const;

    bool
    exists(key_type const& key) const;

    value_type const&
    at(std::string const& key) const;

    mapped_type&
    at(key_type const& key);

    mapped_type const&
    at(key_type const& key) const;

    iterator
    erase(iterator it);

    iterator
    begin() ARGPARSE_NOEXCEPT;

    iterator
    end() ARGPARSE_NOEXCEPT;

    const_iterator
    begin() const ARGPARSE_NOEXCEPT;

    const_iterator
    end() const ARGPARSE_NOEXCEPT;

    const_iterator
    find_arg(
            std::string const& key) const;

    const_iterator
    find(std::string const& key) const;

    const_iterator
    find(key_type const& key) const;

    iterator
    find(key_type const& key);

    void
    on_process_store(
            key_type const& key,
            std::string const& value);

    void
    on_process_store(
            key_type const& key,
            std::vector<std::string> const& values);

    // -- value conversion ----------------------------------------------------
    typedef std::vector<std::string>::const_iterator        data_const_iterator;
    typedef std::vector<std::string>::difference_type                     dtype;

    template <class T>
    struct need_operator_in
    {
        static bool const value = !detail::is_same<bool, T>::value
                && !detail::is_string_ctor<T>::value
                && !detail::is_byte_type<T>::value;
    };

    template <class T>
    struct simple_element
    {
        static bool const value = detail::is_floating_point<T>::value
                || detail::is_string_ctor<T>::value
                || detail::is_integral<T>::value;
    };

    template <class T>
    static typename detail::enable_if<detail::is_string_ctor<T>::value, T>::type
    to_type(std::string const& data)
    {
        return T(data);
    }

    template <class T>
    static typename detail::enable_if<detail::is_same<bool, T>::value, T>::type
    to_type(std::string const& data) ARGPARSE_NOEXCEPT
    {
        return detail::_string_to_bool(data);
    }

    template <class T>
    static typename detail::enable_if<detail::is_byte_type<T>::value, T>::type
    to_type(std::string const& data)
    {
        if (data.size() != 1) {
            throw TypeError("got a data-array in value '" + data + "'");
        }
        return static_cast<T>(data.at(0));
    }

    template <class T>
    static typename detail::enable_if<
       detail::has_operator_in<T>::value && need_operator_in<T>::value, T>::type
    to_type(std::string const& data)
    {
        T res = T();
        std::stringstream ss(data);
        ss >> res;
        if (ss.fail() || !ss.eof()) {
            throw TypeError("invalid " + detail::Type::name<T>()
                            + " value: '" + data + "'");
        }
        return res;
    }

    template <class T>
    static typename detail::enable_if<
      !detail::has_operator_in<T>::value && need_operator_in<T>::value, T>::type
    to_type(std::string const&)
    {
        throw TypeError("unsupported type " + detail::Type::name<T>());
    }

    template <class T>
    static T
    as_type(key_type const& key,
            std::string const& value)
    {
        std::string str = detail::_remove_quotes(value);
        if (key->m_factory) {
            T res = T();
            if (!str.empty()) {
                key->m_factory(str, &res);
            }
            return res;
        }
        return str.empty() ? T() : to_type<T>(str);
    }

    template <class K, class V>
    static std::pair<K, V>
    as_pair(std::string const& name,
            key_type const& key,
            data_const_iterator beg,
            data_const_iterator end,
            char sep)
    {
        std::size_t size = static_cast<std::size_t>(end - beg);
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (size == 2) {
                return std::make_pair(as_type<K>(key, *(beg)),
                                      as_type<V>(key, *(beg + 1)));
            }
            throw TypeError("invalid data for paired argument '" + name + "'");
        }
        if (size != 1) {
            throw TypeError("got a data-array for argument '" + name + "'");
        }
        std::vector<std::string> pair
                = detail::_split(*(beg), std::string(1, sep), 1);
        pair.resize(2);
        return std::make_pair(as_type<K>(key, pair.at(0)),
                              as_type<V>(key, pair.at(1)));
    }

    template <class K, class V>
    static std::vector<std::pair<K, V> >
    as_vector_pair(
            std::string const& key,
            value_type const& v,
            char sep)
    {
        std::vector<std::string> const& vs = v.second();
        std::size_t st = std::isspace(static_cast<unsigned char>(sep)) ? 2 : 1;
        if (vs.size() % st != 0) {
            throw ValueError("invalid stored argument amount");
        }
        std::vector<std::pair<K, V> > res;
        res.reserve(vs.size() / st);
        for (std::size_t i = 0; i < vs.size(); i += st) {
            res.push_back(as_pair<K, V>(
                              key, v.first, vs.begin() + static_cast<dtype>(i),
                              vs.begin() + static_cast<dtype>(i + st), sep));
        }
        return res;
    }

    template <class T>
    static typename detail::enable_if<
        detail::is_stl_container<typename detail::decay<T>::type>::value, T
    >::type
    make_container(
            std::vector<typename T::value_type> const& vec)
    {
        return T(vec.begin(), vec.end());
    }

    template <class T>
    static typename detail::enable_if<
        detail::is_stl_queue<typename detail::decay<T>::type>::value, T
    >::type
    make_container(
            std::vector<typename T::value_type> const& vec)
    {
        return T(std::deque<typename T::value_type>(vec.begin(), vec.end()));
    }

#ifdef ARGPARSE_CXX_11
    template <class T>
    static typename detail::enable_if<
        detail::is_stl_array<typename detail::decay<T>::type>::value, T
    >::type
    make_container(
            std::vector<typename T::value_type> const& vec)
    {
        T res{};
        if (res.size() != vec.size()) {
            std::cerr << "argparse error [skip]: array size mismatch: was "
                      << res.size() << ", expected " << vec.size() << std::endl;
        }
        auto size = res.size();
        if (size > vec.size()) {
            size = vec.size();
        }
        typedef typename std::vector<
                typename T::value_type>::difference_type dtype;
        std::move(vec.begin(), std::next(
                      vec.begin(), static_cast<dtype>(size)), res.begin());
        return res;
    }

    template <class... Ts, std::size_t... Idxs>
    static std::tuple<Ts...>
    mk_tuple(
            key_type const& key,
            std::vector<std::string> const& values,
            detail::integer_sequence<std::size_t, Idxs...>)
    {
        return std::make_tuple(as_type<Ts>(key, values[Idxs])...);
    }

    template <class... Ts>
    static std::tuple<Ts...>
    gen_tuple(
            key_type const& key,
            detail::type_tag<std::tuple<Ts...> >,
            std::vector<std::string> const& values)
    {
        return mk_tuple<Ts...>(key, values,
                   detail::make_integer_sequence<std::size_t, sizeof...(Ts)>());
    }

    template <class T>
    static T
    as_tuple(
            std::string const& name,
            key_type const& key,
            data_const_iterator beg,
            data_const_iterator end,
            char sep)
    {
        std::size_t tuple_sz = std::tuple_size<T>{};
        if (tuple_sz == 0) {
            throw TypeError("unsupported empty tuple");
        }
        std::size_t size = static_cast<std::size_t>(end - beg);
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (size == tuple_sz) {
                return gen_tuple(key, detail::type_tag<T>{}, { beg, end });
            }
            throw TypeError("invalid data for tuple argument '" + name + "'");
        }
        if (size != 1) {
            throw TypeError("got a data-array for argument '" + name + "'");
        }
        auto vals = detail::_split(*(beg), std::string(1, sep),
                                   static_cast<int32_t>(tuple_sz) - 1);
        vals.resize(tuple_sz);
        return gen_tuple(key, detail::type_tag<T>{}, vals);
    }

    template <class T>
    static std::vector<T>
    as_vector_tuple(
            std::string const& key,
            value_type const& v,
            char sep)
    {
        std::vector<std::string> const& vs = v.second();
        std::size_t tuple_sz = std::tuple_size<T>{};
        auto st = std::isspace(static_cast<unsigned char>(sep)) ? tuple_sz : 1;
        if (st == 0 || vs.size() % st != 0) {
            throw ValueError("invalid stored argument amount");
        }
        std::vector<T> res;
        res.reserve(vs.size() / st);
        for (std::size_t i = 0; i < vs.size(); i += st) {
            res.push_back(as_tuple<T>(
                              key, v.first, vs.begin() + static_cast<dtype>(i),
                              vs.begin() + static_cast<dtype>(i + st), sep));
        }
        return res;
    }
#endif  // C++11+

#ifdef ARGPARSE_CXX_11
    template <class T, typename detail::enable_if<
                  simple_element<T>::value>::type* = nullptr>
    static std::vector<T>
    as_subvector(
            key_type const& key,
            std::vector<std::string> const& vs)
#else
    template <class T>
    static std::vector<T>
    as_subvector(
            key_type const& key,
            std::vector<std::string> const& vs,
            typename detail::enable_if<
                simple_element<T>::value, bool>::type = true)
#endif  // C++11+
    {
        std::vector<T> res;
        res.reserve(vs.size());
        for (data_const_iterator it = vs.begin(); it != vs.end(); ++it) {
            res.push_back(as_type<T>(key, *it));
        }
        return res;
    }

#ifdef ARGPARSE_CXX_11
    template <class T, typename detail::enable_if<
                  !simple_element<T>::value>::type* = nullptr>
    static std::vector<T>
    as_subvector(
            key_type const& key,
            std::vector<std::string> const& vs)
#else
    template <class T>
    static std::vector<T>
    as_subvector(
            key_type const& key,
            std::vector<std::string> const& vs,
            typename detail::enable_if<
                !simple_element<T>::value, bool>::type = true)
#endif  // C++11+
    {
        std::vector<T> res;
        if (vs.empty()) {
            return res;
        }
        std::size_t st = 1;
        if ((key->action() & detail::_store_action)
                && (key->m_nargs & Argument::_NARGS_COMBINED)) {
            st = key->m_num_args;
        }
        if (st == 0) {
            throw TypeError("unsupported argument with nargs=0");
        }
        if (vs.size() % st != 0) {
            throw ValueError("invalid stored argument amount");
        }
        res.reserve(vs.size() / st);
        for (std::size_t i = 0; i < vs.size(); i += st) {
            std::vector<std::string> values(
                        vs.begin() + static_cast<dtype>(i),
                        vs.begin() + static_cast<dtype>(i + st));
            res.push_back(as_type<T>(key, detail::_join(values)));
        }
        return res;
    }

    template <class T>
    static T
    get_single_value(
            std::string const& key,
            value_type const& value)
    {
        if (value.second.size() > 1) {
            throw TypeError("got a data-array for argument '" + key + "'");
        }
        return value.second.empty()
                ? T() : as_type<T>(value.first, value.second.front());
    }

    template <class T>
    static T
    get_custom_value(
            value_type const& value)
    {
        return value.second.empty()
                ? T() : as_type<T>(value.first, detail::_join(value.second()));
    }

    template <class T>
    static std::vector<T>
    get_vector(
            value_type const& value)
    {
        std::vector<T> res;
        for (std::size_t i = 0; i < value.second.indexes().size(); ++i) {
            std::vector<T> vector = as_subvector<T>(
                        value.first, value.second.sub_values(i));
            res.insert(res.end(), vector.begin(), vector.end());
        }
        return res;
    }

    template <class T>
    static typename detail::enable_if<
        detail::is_stl_matrix<typename detail::decay<T>::type>::value
        && !detail::is_stl_array<typename detail::decay<T>::type>::value, T
    >::type
    get_matrix(
            value_type const& value)
    {
        typedef typename T::value_type V;
        typedef typename V::value_type VV;
        T res;
        for (std::size_t i = 0; i < value.second.indexes().size(); ++i) {
            std::vector<VV> vector = as_subvector<VV>(
                        value.first, value.second.sub_values(i));
            push_to_container<T>(res, make_container<V>(vector));
        }
        return res;
    }

#ifdef ARGPARSE_CXX_11
    template <class T>
    static typename detail::enable_if<
        detail::is_stl_matrix<typename detail::decay<T>::type>::value
        && detail::is_stl_array<typename detail::decay<T>::type>::value, T
    >::type
    get_matrix(
            value_type const& value)
    {
        typedef typename T::value_type V;
        typedef typename V::value_type VV;
        T res;
        if (res.size() != value.second.indexes().size()) {
            std::cerr << "argparse error [skip]: array size mismatch: was "
                      << res.size() << ", expected "
                      << value.second.indexes().size() << std::endl;
        }
        auto size = res.size();
        if (size > value.second.indexes().size()) {
            size = value.second.indexes().size();
        }
        std::vector<V> vec;
        vec.reserve(size);
        for (std::size_t i = 0; i < size; ++i) {
            std::vector<VV> vector = as_subvector<VV>(
                        value.first, value.second.sub_values(i));
            push_to_container<std::vector<V> >(vec, make_container<V>(vector));
        }
        typedef typename std::vector<V>::difference_type dtype;
        std::move(vec.begin(), std::next(
                      vec.begin(), static_cast<dtype>(size)), res.begin());
        return res;
    }

    template <class T, typename detail::enable_if<
                  detail::has_push_back<T>::value>::type* = nullptr>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value)
    {
        container.push_back(value);
    }

    template <class T, typename detail::enable_if<
                  detail::has_push<T>::value
                  && !detail::has_push_back<T>::value>::type* = nullptr>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value)
    {
        container.push(value);
    }

    template <class T, typename detail::enable_if<
                  detail::has_insert<T>::value
                  && !detail::has_push<T>::value
                  && !detail::has_push_back<T>::value>::type* = nullptr>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value)
    {
        container.insert(value);
    }

    template <class T, typename detail::enable_if<
                  detail::has_push_front<T>::value
                  && !detail::has_insert<T>::value
                  && !detail::has_push<T>::value
                  && !detail::has_push_back<T>::value>::type* = nullptr>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value)
    {
        container.push_front(value);
    }
#else
    template <class T>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value,
            typename detail::enable_if<
                detail::has_push_back<T>::value, bool>::type = true)
    {
        container.push_back(value);
    }

    template <class T>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value,
            typename detail::enable_if<
                detail::has_push<T>::value
                && !detail::has_push_back<T>::value, bool>::type = true)
    {
        container.push(value);
    }

    template <class T>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value,
            typename detail::enable_if<
                detail::has_insert<T>::value
                && !detail::has_push<T>::value
                && !detail::has_push_back<T>::value, bool>::type = true)
    {
        container.insert(value);
    }

    template <class T>
    static void
    push_to_container(
            T& container,
            typename T::value_type const& value,
            typename detail::enable_if<
                detail::has_push_front<T>::value
                && !detail::has_insert<T>::value
                && !detail::has_push<T>::value
                && !detail::has_push_back<T>::value, bool>::type = true)
    {
        container.push_front(value);
    }
#endif  // C++11+

#ifdef ARGPARSE_HAS_OPTIONAL
    template <class T>
    static std::optional<T>
    to_opt_type(
            std::string const& data)
    {
        if constexpr (detail::is_string_ctor<T>::value) {
            return T(data);
        }
        if constexpr (detail::is_same<bool, T>::value) {
            return detail::_string_to_bool(data);
        }
        if constexpr (detail::is_byte_type<T>::value) {
            if (data.size() != 1) {
                return std::nullopt;
            }
            return static_cast<T>(data.front());
        }
        if constexpr (detail::has_operator_in<T>::value
                && need_operator_in<T>::value) {
            T res{};
            std::stringstream ss(data);
            ss >> res;
            if (ss.fail() || !ss.eof()) {
                return std::nullopt;
            }
            return res;
        }
        return std::nullopt;
    }

    template <class T>
    static std::optional<T>
    as_opt_type(
            key_type const& key,
            std::string const& value)
    {
        std::string str = detail::_remove_quotes(value);
        if (key->m_factory) {
            T res{};
            if (!str.empty()) {
                try {
                    key->m_factory(str, &res);
                } catch (...) {
                    return std::nullopt;
                }
            }
            return res;
        }
        return str.empty() ? T{} : to_opt_type<T>(str);
    }

    template <class K, class V>
    static std::optional<std::pair<K, V> >
    as_opt_pair(
            key_type const& key,
            data_const_iterator beg,
            data_const_iterator end,
            char sep)
    {
        std::size_t size = static_cast<std::size_t>(end - beg);
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (size == 2) {
                auto el1 = as_opt_type<K>(key, *(beg));
                auto el2 = as_opt_type<V>(key, *(beg + 1));
                if (el1.has_value() && el2.has_value()) {
                    return std::make_pair(el1.value(), el2.value());
                }
            }
            return std::nullopt;
        }
        if (size == 1) {
            auto pair = detail::_split(*(beg), std::string(1, sep), 1);
            pair.resize(2);
            auto el1 = as_opt_type<K>(key, pair.at(0));
            auto el2 = as_opt_type<V>(key, pair.at(1));
            if (el1.has_value() && el2.has_value()) {
                return std::make_pair(el1.value(), el2.value());
            }
        }
        return std::nullopt;
    }

    template <class K, class V>
    static std::optional<std::vector<std::pair<K, V> > >
    as_opt_vector_pair(
            value_type const& value,
            char sep)
    {
        auto const& vs = value.second();
        std::size_t st = std::isspace(static_cast<unsigned char>(sep)) ? 2 : 1;
        if (vs.size() % st != 0) {
            return std::nullopt;
        }
        std::vector<std::pair<K, V> > res;
        res.reserve(vs.size() / st);
        for (std::size_t i = 0; i < vs.size(); i += st) {
            auto pair = as_opt_pair<K, V>(
                        value.first, vs.begin() + static_cast<dtype>(i),
                        vs.begin() + static_cast<dtype>(i + st), sep);
            if (!pair.has_value()) {
                return std::nullopt;
            }
            res.emplace_back(pair.value());
        }
        return res;
    }

    template <class... Ts>
    static std::optional<std::tuple<Ts...> >
    gen_opt_tuple(
            key_type const& key,
            detail::type_tag<std::tuple<Ts...> >,
            std::vector<std::string> const& values)
    {
        try {
            auto res = mk_tuple<Ts...>(key, values,
                   detail::make_integer_sequence<std::size_t, sizeof...(Ts)>());
            return res;
        } catch (...) {
            return std::nullopt;
        }
    }

    template <class T>
    static std::optional<T>
    as_opt_tuple(
            key_type const& key,
            data_const_iterator beg,
            data_const_iterator end,
            char sep)
    {
        std::size_t tuple_sz = std::tuple_size<T>{};
        if (tuple_sz == 0) {
            return std::nullopt;
        }
        std::size_t size = static_cast<std::size_t>(end - beg);
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (size != tuple_sz) {
                return std::nullopt;
            }
            return gen_opt_tuple(key, detail::type_tag<T>{}, { beg, end });
        }
        if (size != 1) {
            return std::nullopt;
        }
        auto vals = detail::_split(*(beg), std::string(1, sep),
                                   static_cast<int32_t>(tuple_sz) - 1);
        vals.resize(tuple_sz);
        return gen_opt_tuple(key, detail::type_tag<T>{}, vals);
    }

    template <class T>
    static std::optional<std::vector<T> >
    as_opt_vector_tuple(
            value_type const& value,
            char sep)
    {
        auto const& vs = value.second();
        std::size_t tuple_sz = std::tuple_size<T>{};
        auto st = std::isspace(static_cast<unsigned char>(sep)) ? tuple_sz : 1;
        if (st == 0 || vs.size() % st != 0) {
            return std::nullopt;
        }
        std::vector<T> res;
        res.reserve(vs.size() / st);
        for (std::size_t i = 0; i < vs.size(); i += st) {
            auto tuple = as_opt_tuple<T>(
                        value.first, vs.begin() + static_cast<dtype>(i),
                        vs.begin() + static_cast<dtype>(i + st), sep);
            if (!tuple.has_value()) {
                return std::nullopt;
            }
            res.emplace_back(tuple.value());
        }
        return res;
    }

    template <class T>
    static std::optional<std::vector<T> >
    as_opt_subvector(
            key_type const& key,
            std::vector<std::string> const& vs)
    {
        std::vector<T> res;
        if (vs.empty()) {
            return res;
        }
        if constexpr (simple_element<T>::value) {
            res.reserve(vs.size());
            for (auto const& value : vs) {
                auto el = as_opt_type<T>(key, value);
                if (!el.has_value()) {
                    return std::nullopt;
                }
                res.emplace_back(el.value());
            }
        } else {
            std::size_t st = 1;
            if ((key->action() & detail::_store_action)
                    && (key->m_nargs & Argument::_NARGS_COMBINED)) {
                st = key->m_num_args;
            }
            if (st == 0 || vs.size() % st != 0) {
                return std::nullopt;
            }
            res.reserve(vs.size() / st);
            for (std::size_t i = 0; i < vs.size(); i += st) {
                std::vector<std::string> values(
                            vs.begin() + static_cast<dtype>(i),
                            vs.begin() + static_cast<dtype>(i + st));
                auto el = as_opt_type<T>(key, detail::_join(values));
                if (!el.has_value()) {
                    return std::nullopt;
                }
                res.emplace_back(el.value());
            }
        }
        return res;
    }

    template <class T>
    static std::optional<T>
    opt_single_value(
            value_type const& value)
    {
        if (value.second.size() != 1) {
            return std::nullopt;
        }
        return as_opt_type<T>(value.first, value.second.front());
    }

    template <class T>
    static std::optional<std::vector<T> >
    opt_vector(
            value_type const& value)
    {
        std::vector<T> res;
        for (std::size_t i = 0; i < value.second.indexes().size(); ++i) {
            auto vector = as_opt_subvector<T>(
                        value.first, value.second.sub_values(i));
            if (!vector.has_value()) {
                return std::nullopt;
            }
            res.insert(res.end(), vector.value().begin(), vector.value().end());
        }
        return res;
    }

    template <class T>
    static std::optional<typename detail::enable_if<
        detail::is_stl_matrix<typename detail::decay<T>::type>::value, T
    >::type>
    opt_matrix(
            value_type const& value)
    {
        typedef typename T::value_type V;
        typedef typename V::value_type VV;
        T res;
        for (std::size_t i = 0; i < value.second.indexes().size(); ++i) {
            auto vector = as_opt_subvector<VV>(
                        value.first, value.second.sub_values(i));
            if (!vector.has_value()) {
                return std::nullopt;
            }
            res.push_back(make_container<V>(vector.value()));
        }
        return res;
    }

    template <class T>
    static std::optional<T>
    opt_custom_value(
            value_type const& value)
    {
        if (value.second.empty()) {
            return std::nullopt;
        }
        return as_opt_type<T>(value.first, detail::_join(value.second()));
    }
#endif  // ARGPARSE_HAS_OPTIONAL

    // -- data ----------------------------------------------------------------
    map_type m_data;
};

/*!
 *  \brief Object with parsed arguments
 */
ARGPARSE_EXPORT class Namespace
{
    friend class ArgumentParser;

    explicit
    Namespace(
            _Storage const& storage = _Storage());

    explicit
    Namespace(
            _Storage const& storage,
            std::vector<std::string> const& args);

#ifdef ARGPARSE_CXX_11
    explicit
    Namespace(
            _Storage&& storage) ARGPARSE_NOEXCEPT;

    explicit
    Namespace(
            _Storage&& storage,
            std::vector<std::string>&& args) ARGPARSE_NOEXCEPT;
#endif  // C++11+

public:
    /*!
     *  \brief Check if argument name exists and specified in parsed arguments
     *
     *  \param key Argument destination name or flag
     *
     *  \since v1.7.5
     *
     *  \return True if argument name exists and specified, otherwise false
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    contains(
            std::string const& key) const;

    /*!
     *  \brief Check if argument name exists and specified in parsed arguments
     *
     *  \param key Argument destination name or flag
     *
     *  \return True if argument name exists and specified, otherwise false
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    exists(std::string const& key) const;

    /*!
     *  \brief Get parsed argument value as boolean, byte, floating point
     *  or string types.
     *  If argument not parsed, returns default value.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<detail::is_string_ctor<T>::value
                               || detail::is_floating_point<T>::value
                               || detail::is_same<bool, T>::value
                               || detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::name<T>());
        detail::_check_non_count_action(key, args.first->action());
        return _Storage::get_single_value<T>(key, args);
    }

    /*!
     *  \brief Get parsed argument value as integer types.
     *  If argument not parsed, returns default value.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<detail::is_integral<T>::value
                               && !detail::is_same<bool, T>::value
                               && !detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::name<T>());
        if (args.first->action() == argparse::count) {
            return static_cast<T>(args.second.size());
        }
        return _Storage::get_single_value<T>(key, args);
    }

    /*!
     *  \brief Get parsed argument value as stl container types.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        (detail::is_stl_container<typename detail::decay<T>::type>::value
     && !detail::is_stl_container_tupled<typename detail::decay<T>::type>::value
     && !detail::is_stl_container_paired<typename detail::decay<T>::type>::value
     && !detail::is_stl_matrix<typename detail::decay<T>::type>::value)
      || ((detail::is_stl_array<typename detail::decay<T>::type>::value
           || detail::is_stl_queue<typename detail::decay<T>::type>::value)
     && !detail::is_stl_matrix<typename detail::decay<T>::type>::value), T
    >::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        return _Storage::make_container<T>(
                    _Storage::get_vector<typename T::value_type>(args));
    }

    /*!
     *  \brief Get parsed argument value as paired container types.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
      detail::is_stl_container_paired<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key,
            char sep = detail::_equal) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        typedef typename T::value_type::first_type K;
        typedef typename T::value_type::second_type V;
        std::vector<std::pair<K, V> > vector
                = _Storage::as_vector_pair<K, V>(key, args, sep);
        return _Storage::make_container<T>(vector);
    }

    /*!
     *  \brief Get parsed argument value as mapped types.
     *  If argument not parsed, returns empty map.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_map<typename detail::decay<T>::type>::value, T>::type
    get(std::string const& key,
            char sep = detail::_equal) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        typedef typename T::key_type K;
        typedef typename T::mapped_type V;
        T res = T();
        std::vector<std::pair<K, V> > vector
                = _Storage::as_vector_pair<K, V>(key, args, sep);
        for (std::size_t i = 0; i < vector.size(); ++i) {
            res.insert(std::make_pair(vector.at(i).first, vector.at(i).second));
        }
        return res;
    }

    /*!
     *  \brief Get parsed argument value as 2D stl containers.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_matrix<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        if (args.first->action() != argparse::append
                || !(args.first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))) {
            throw TypeError("got an invalid type for argument '" + key + "'");
        }
        return _Storage::get_matrix<T>(args);
    }

    /*!
     *  \brief Get parsed argument value as paired types.
     *  If argument not parsed, returns default pair.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_pair<typename detail::decay<T>::type>::value, T>::type
    get(std::string const& key,
            char sep = detail::_equal) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::name<T>());
        detail::_check_non_count_action(key, args.first->action());
        if (args.second.empty()) {
            return T();
        }
        typedef typename T::first_type K;
        typedef typename T::second_type V;
        return _Storage::as_pair<K, V>(
              key, args.first, args.second().begin(), args.second().end(), sep);
    }

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Get parsed argument value as tupled container types.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value, T
    >::type
    get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        auto vector = _Storage::as_vector_tuple<typename T::value_type>(
                    key, args, sep);
        return _Storage::make_container<T>(vector);
    }

    /*!
     *  \brief Get parsed argument value as tuple types.
     *  If argument not parsed, returns empty tuple.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename std::enable_if<
        detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::name<T>());
        detail::_check_non_count_action(key, args.first->action());
        if (args.second.empty()) {
            return T();
        }
        return _Storage::as_tuple<T>(
              key, args.first, args.second().begin(), args.second().end(), sep);
    }
#endif  // C++11+

    /*!
     *  \brief Get parsed argument value as custom type.
     *  If argument not parsed, returns default custom type.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        !detail::is_string_ctor<T>::value
        && !detail::is_floating_point<T>::value
        && !detail::is_integral<T>::value
        && !detail::is_stl_array<typename detail::decay<T>::type>::value
        && !detail::is_stl_tuple<typename detail::decay<T>::type>::value
        && !detail::is_stl_container<typename detail::decay<T>::type>::value
        && !detail::is_stl_map<typename detail::decay<T>::type>::value
        && !detail::is_stl_pair<typename detail::decay<T>::type>::value
        && !detail::is_stl_queue<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::name<T>());
        detail::_check_non_count_action(key, args.first->action());
        return _Storage::get_custom_value<T>(args);
    }

    /*!
     *  \brief Print namespace to output stream (default: std::cout)
     *
     *  \param os Output stream
     */
    void
    print(std::ostream& os = std::cout) const;

    /*!
     *  \brief Get parsed argument value as args string
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value as args string
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    to_args(std::string const& key) const;

    /*!
     *  \brief Get parsed argument value as string
     *
     *  \param key Argument destination name or flag
     *  \param quotes Value quotes (default: "")
     *
     *  \return Parsed argument value as string
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    to_string(
            std::string const& key,
            std::string const& quotes = std::string()) const;

    /*!
     *  \brief Get namespace as string
     *
     *  \return Namespace as string
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    to_string() const;

#ifdef ARGPARSE_HAS_OPTIONAL
    /*!
     *  \brief Try get parsed argument value as boolean, byte, floating point,
     *  integer, string, stl container, 2D stl container or custom types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        !detail::is_stl_container_paired<typename std::decay<T>::type>::value
        && !detail::is_stl_container_tupled<typename std::decay<T>::type>::value
        && !detail::is_stl_map<typename std::decay<T>::type>::value
        && !detail::is_stl_pair<typename std::decay<T>::type>::value
        && !detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = opt_data(key);
        if (!args.has_value()) {
            return std::nullopt;
        }
        if constexpr (detail::has_value_type<T>::value
                && !detail::is_string_ctor<T>::value) {
            if (!detail::_is_type_correct(args->first->type_name(),
                                          detail::Type::basic<T>())) {
                return std::nullopt;
            }
        } else {
            if (!detail::_is_type_correct(args->first->type_name(),
                                          detail::Type::name<T>())) {
                return std::nullopt;
            }
        }
        if constexpr (std::is_integral<T>::value
                && !std::is_same<bool, T>::value
                && !detail::is_byte_type<T>::value) {
            if (args->first->action() == argparse::count) {
                return static_cast<T>(args->second.size());
            }
            return _Storage::opt_single_value<T>(args.value());
        }
        if (args->first->action() == argparse::count) {
            return std::nullopt;
        }
        if constexpr (detail::is_stl_matrix<
                typename std::decay<T>::type>::value) {
            if (args->first->action() != argparse::append
                    || !(args->first->m_nargs
                         & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                            | Argument::ZERO_OR_MORE))) {
                return std::nullopt;
            }
            return _Storage::opt_matrix<T>(args.value());
        }
        if constexpr ((detail::is_stl_container<
                    typename std::decay<T>::type>::value
                && !detail::is_stl_container_paired<
                    typename std::decay<T>::type>::value
                && !detail::is_stl_container_tupled<
                    typename std::decay<T>::type>::value
                && !detail::is_stl_matrix<typename std::decay<T>::type>::value)
                || ((detail::is_stl_array<typename std::decay<T>::type>::value
                   || detail::is_stl_queue<typename std::decay<T>::type>::value)
              && !detail::is_stl_matrix<typename std::decay<T>::type>::value)) {
            auto vector = _Storage::opt_vector<
                    typename T::value_type>(args.value());
            if (!vector.has_value()) {
                return std::nullopt;
            }
            return _Storage::make_container<T>(vector.value());
        } else {
            if constexpr (detail::is_string_ctor<T>::value
                    || std::is_floating_point<T>::value
                    || std::is_same<bool, T>::value
                    || detail::is_byte_type<T>::value) {
                return _Storage::opt_single_value<T>(args.value());
            } else {
                return _Storage::opt_custom_value<T>(args.value());
            }
        }
    }

    /*!
     *  \brief Try get parsed argument value as paired container,
     *  tupled container, mapped, paired or tuple types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_container_paired<typename std::decay<T>::type>::value
        || detail::is_stl_container_tupled<typename std::decay<T>::type>::value
        || detail::is_stl_map<typename std::decay<T>::type>::value
        || detail::is_stl_pair<typename std::decay<T>::type>::value
        || detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto args = opt_data(key);
        if (!args.has_value() || args->first->action() == argparse::count) {
            return std::nullopt;
        }
        if constexpr (detail::has_value_type<T>::value) {
            if (!detail::_is_type_correct(args->first->type_name(),
                                          detail::Type::basic<T>())) {
                return std::nullopt;
            }
        } else {
            if (args->second.empty()
                    || !detail::_is_type_correct(args->first->type_name(),
                                                 detail::Type::name<T>())) {
                return std::nullopt;
            }
        }
        if constexpr (detail::is_stl_container_paired<
                typename std::decay<T>::type>::value) {
            auto vector = _Storage::as_opt_vector_pair<
                    typename T::value_type::first_type,
                    typename T::value_type::second_type>(args.value(), sep);
            if (!vector.has_value()) {
                return std::nullopt;
            }
            return _Storage::make_container<T>(vector.value());
        }
        if constexpr (detail::is_stl_container_tupled<
                typename std::decay<T>::type>::value) {
            auto vector = _Storage::as_opt_vector_tuple<
                    typename T::value_type>(args.value(), sep);
            if (!vector.has_value()) {
                return std::nullopt;
            }
            return _Storage::make_container<T>(vector.value());
        }
        if constexpr (detail::is_stl_map<typename std::decay<T>::type>::value) {
            T res{};
            auto vector = _Storage::as_opt_vector_pair<
              typename T::key_type, typename T::mapped_type>(args.value(), sep);
            if (!vector.has_value()) {
                return std::nullopt;
            }
            for (auto const& pair : vector.value()) {
                res.emplace(std::make_pair(pair.first, pair.second));
            }
            return res;
        }
        if constexpr (detail::is_stl_pair<
                typename std::decay<T>::type>::value) {
            return _Storage::as_opt_pair<typename T::first_type,
                                         typename T::second_type>(
                        args->first, args->second().begin(),
                        args->second().end(), sep);
        }
        if constexpr (detail::is_stl_tuple<
                typename std::decay<T>::type>::value) {
            return _Storage::as_opt_tuple<T>(
                        args->first, args->second().begin(),
                        args->second().end(), sep);
        }
        return std::nullopt;
    }
#endif  // ARGPARSE_HAS_OPTIONAL

    /*!
     *  \brief Get unrecognized arguments
     *
     *  \return Unrecognized arguments
     */
    ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    unrecognized_args() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get unrecognized arguments as args string
     *
     *  \return Unrecognized arguments as args string
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    unrecognized_args_to_args() const;

private:
    static std::string
    store_actions_to_string(
            _Storage::value_type const& args,
            std::string const& quotes);

    _Storage::value_type const&
    data(std::string const& key) const;

    _Storage const&
    storage() const ARGPARSE_NOEXCEPT;

#ifdef ARGPARSE_HAS_OPTIONAL
    std::optional<_Storage::value_type>
    opt_data(
            std::string const& key) const;
#endif  // ARGPARSE_HAS_OPTIONAL

    // -- data ----------------------------------------------------------------
    _Storage m_storage;
    detail::Value<std::vector<std::string> > m_unrecognized_args;
};

/*!
 *  \brief Output stream overload for Namespace
 *
 *  \param os Output stream
 *  \param obj Namespace object
 *
 *  \return Output stream reference
 */
inline std::ostream&
operator <<(
        std::ostream& os,
        Namespace const& obj)
{
    os << obj.to_string();
    return os;
}

/*!
 *  \brief _ParserGroup class
 */
class _ParserGroup : public _Group
{
    friend class ArgumentParser;
    friend class HelpFormatter;

protected:
    typedef detail::shared_ptr<ArgumentParser> pParser;
    typedef std::list<pParser>::const_iterator prs_iterator;

    explicit
    _ParserGroup(
            std::string const& title,
            std::string const& description);

    static pParser
    make_parser(
            std::string const& name);

public:
    /*!
     *  \brief Get parser group 'help' message for default language
     *
     *  \return Parser group 'help' message
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    help() const;

    /*!
     *  \brief Get parser group 'metavar' value
     *
     *  \return Parser group 'metavar' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    metavar() const ARGPARSE_NOEXCEPT;

protected:
    void
    limit_help_flags(
            HelpFormatter const&,
            std::size_t& limit) const ARGPARSE_OVERRIDE;

    void
    print_help(
            std::ostream& os,
            bool& eat_ln,
            HelpFormatter const& formatter,
            std::string const& prog,
            bool required,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const ARGPARSE_OVERRIDE;

    void
    print_parser_group(
            std::ostream& os,
            HelpFormatter const& formatter,
            std::string const& prog,
            bool required,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const;

    std::vector<std::string>
    _parser_names() const;

    std::string
    _flags_to_string() const;

    bool
    _is_positional() const;

    std::string
    get_choices() const;

    std::string
    get_metavar() const;

    // -- data ----------------------------------------------------------------
    detail::SValue<detail::TranslationPack> m_help;
    detail::Value<std::string> m_metavar;
    std::list<pParser> m_parsers;
};

// Forward declaration
class SubParsers;

/*!
 *  \brief ParserGroup class
 *
 *  \since v1.9.0
 */
ARGPARSE_EXPORT class ParserGroup : public _ParserGroup
{
    friend class ArgumentParser;
    friend class SubParsers;

    explicit
    ParserGroup(
            SubParsers* parent,
            std::string const& title,
            std::string const& description);

    static detail::shared_ptr<ParserGroup>
    make_parsergroup(
            SubParsers* parent,
            std::string const& title,
            std::string const& description);

public:
    using _ParserGroup::title;
    using _ParserGroup::description;
    using _ParserGroup::help;
    using _ParserGroup::metavar;

    /*!
     *  \brief Create parser group object from another parser group
     *
     *  \param orig Parser group object to copy
     *
     *  \return Parser group object
     */
    ParserGroup(
            ParserGroup const& orig);

    /*!
     *  \brief Copy parser group object from another parser group
     *
     *  \param rhs Parser group object to copy
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    operator =(
            ParserGroup const& rhs);

    /*!
     *  \brief Set parser group 'title' value for selected language
     *
     *  \param value Title value
     *  \param lang Language value (default: "")
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    title(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set parser group 'description' value for selected language
     *
     *  \param value Description value
     *  \param lang Language value (default: "")
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    description(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set parser group 'help' message for selected language
     *
     *  \param value Help message
     *  \param lang Language value (default: "")
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    help(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Suppress parser group 'help' message
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    help(_SUPPRESS value);

    /*!
     *  \brief Set parser group 'metavar' value
     *
     *  \param value Metavar value
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    metavar(std::string const& value);

    /*!
     *  \brief Add argument parser with concrete name
     *
     *  \param name Parser name
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    add_parser(
            std::string const& name);

    /*!
     *  \brief Get parser names
     *
     *  \return Parser names container
     */
    ARGPARSE_ATTR_NODISCARD
    std::vector<std::string>
    parser_names() const;

private:
    // -- data ----------------------------------------------------------------
    SubParsers* m_parent;
};

/*!
 *  \brief SubParsers class
 */
ARGPARSE_EXPORT class SubParsers : public _ParserGroup
{
    friend class ArgumentParser;
    friend class HelpFormatter;
    friend class ParserGroup;

    typedef detail::shared_ptr<ParserGroup> pParserGroup;
    typedef std::list<pParserGroup>::const_iterator pgr_iterator;

    explicit
    SubParsers(
            std::string const& title,
            std::string const& description);

    static detail::shared_ptr<SubParsers>
    make_subparsers(
            std::string const& title,
            std::string const& description);

public:
    using _ParserGroup::title;
    using _ParserGroup::description;
    using _ParserGroup::help;
    using _ParserGroup::metavar;

    /*!
     *  \brief Set subparsers 'title' value for selected language
     *
     *  \param value Title value
     *  \param lang Language value (default: "")
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    title(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set subparsers 'description' value for selected language
     *
     *  \param value Description value
     *  \param lang Language value (default: "")
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    description(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set subparsers 'prog' value
     *
     *  \param value Program value
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    prog(std::string const& value);

    /*!
     *  \brief Set subparsers 'dest' value
     *
     *  \param value Destination value
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    dest(std::string const& value);

    /*!
     *  \brief Set subparsers 'required' value
     *
     *  \param value Required flag
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    required(
            bool value) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set subparsers 'help' message for selected language
     *
     *  \param value Help message
     *  \param lang Language value (default: "")
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    help(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Suppress subparsers 'help' message
     *
     *  \param value argparse::SUPPRESS
     *
     *  \since v1.7.3
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    help(_SUPPRESS value);

    /*!
     *  \brief Set subparsers 'metavar' value
     *
     *  \param value Metavar value
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    metavar(std::string const& value);

    /*!
     *  \brief Get subparsers 'prog' value
     *
     *  \return Subparsers 'prog' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    prog() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get subparsers 'dest' value
     *
     *  \return Subparsers 'dest' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    dest() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get subparsers 'required' value
     *
     *  \return Subparsers 'required' value
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    required() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Add argument parser with concrete name
     *
     *  \param name Parser name
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    add_parser(
            std::string const& name);

    /*!
     *  \brief Add parser group
     *
     *  \param title Parser group title (default: "")
     *  \param description Parser group description (default: "")
     *
     *  \return Current parser group reference
     */
    ParserGroup&
    add_parser_group(
            std::string const& title = std::string(),
            std::string const& description = std::string());

    /*!
     *  \brief Get parser names
     *
     *  \return Parser names container
     */
    ARGPARSE_ATTR_NODISCARD
    std::vector<std::string>
    parser_names() const;

private:
    void
    limit_help_flags(
            HelpFormatter const&,
            std::size_t& limit) const ARGPARSE_OVERRIDE;

    void
    print_help(
            std::ostream& os,
            bool& eat_ln,
            HelpFormatter const& formatter,
            std::string const& prog,
            bool required,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const ARGPARSE_OVERRIDE;

    std::list<pParser>
    list_parsers() const;

    std::string
    prog_name() const;

    void
    update_prog(
            std::string const& parent_prog,
            std::string const& parent_args);

    std::string
    usage() const;

    std::string
    flags_to_string() const;

    bool
    is_positional() const;

    bool
    is_suppress() const;

    // -- data ------------------------------------------------------------
    std::string m_parent_prog;
    std::string m_parent_args;
    std::string m_prog;
    std::string m_dest;
    std::list<pParserGroup> m_groups;
    bool        m_required;
};

/*!
 *  \brief ArgumentParser objects
 */
ARGPARSE_EXPORT class ArgumentParser
{
    friend class _ParserGroup;
    friend class HelpFormatter;
    friend class ParserGroup;
    friend class SubParsers;

    typedef detail::shared_ptr<Argument> pArgument;
    typedef std::vector<pArgument> pArguments;
    typedef detail::shared_ptr<_Group> pGroup;
    typedef detail::shared_ptr<ArgumentParser> pParser;
    typedef std::list<pParser>::const_iterator prs_iterator;
    typedef std::list<pArgument>::const_iterator arg_iterator;
    typedef std::list<MutuallyExclusiveGroup>::const_iterator mtx_it;
    typedef detail::shared_ptr<SubParsers> pSubParsers;
    typedef std::pair<pSubParsers, std::size_t> SubParsersInfo;

    void
    read_args(
            int argc,
            char const* const argv[]);

    void
    read_env(
            char const* const envp[]);

    void
    initialize_parser();

public:
    ARGPARSE_ATTR_DEPRECATED_REASON(
            "use argparse::SubParsers instead. "
            "will be removed in the next minor release (v1.10.0)")
    typedef SubParsers Subparser;

    /*!
     *  \brief Construct argument parser with concrete program name
     *  (default: "" don't override default program name "untitled")
     *
     *  \param prog Program name (default: "")
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(
            std::string const& prog = std::string());

    /*!
     *  \brief Construct argument parser from command line arguments
     *  with concrete program name
     *  (default: "" don't override argv[0] or default program name "untitled")
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *  \param prog Program name (default: "")
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(
            int argc,
            char const* const argv[],
            std::string const& prog = std::string());

    /*!
     *  \brief Construct argument parser from command line arguments
     *  with environment variables and concrete program name
     *  (default: "" don't override argv[0] or default program name "untitled")
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *  \param envp Execution environment variables
     *  \param prog Program name (default: "")
     *
     *  \since v1.8.0
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(
            int argc,
            char const* const argv[],
            char const* const envp[],
            std::string const& prog = std::string());

    /*!
     *  \brief Destroy argument parser
     */
    virtual ~ArgumentParser() ARGPARSE_NOEXCEPT { }

    /*!
     *  \brief Set argument parser 'prog' value (default: argv[0] or "untitled")
     *
     *  \param value Program name
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    prog(std::string const& value);

    /*!
     *  \brief Set argument parser 'usage' value for selected language
     *
     *  \param value Usage value
     *  \param lang Language value (default: "")
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    usage(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Suppress argument parser 'usage' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \since v1.9.0
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    usage(_SUPPRESS value);

    /*!
     *  \brief Set title for argument parser 'usage' (default: "usage")
     *  for selected language
     *
     *  \param value Title for usage
     *  \param lang Language value (default: "")
     *
     *  \since v1.7.1
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    usage_title(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set argument parser 'description' value for selected language
     *
     *  \param value Description value
     *  \param lang Language value (default: "")
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    description(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set title for positional arguments
     *  (default: "positional arguments") for selected language
     *
     *  \param value Title for positional arguments
     *  \param lang Language value (default: "")
     *
     *  \since v1.7.0
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    positionals_title(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set title for operand arguments
     *  (default: "operands") for selected language
     *
     *  \param value Title for operand arguments
     *  \param lang Language value (default: "")
     *
     *  \since v1.8.0
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    operands_title(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set title for optional arguments
     *  (default: "options") for selected language
     *
     *  \param value Title for optional arguments
     *  \param lang Language value (default: "")
     *
     *  \since v1.7.0
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    optionals_title(
            std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set argument parser 'epilog' value for selected language
     *
     *  \param value Epilog value
     *  \param lang Language value (default: "")
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    epilog(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set argument parser 'help' message (for subparsers)
     *  for selected language
     *
     *  \param value Help message
     *  \param lang Language value (default: "")
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    help(std::string const& value,
            std::string const& lang = std::string());

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value Aliases values
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    aliases(std::vector<std::string> const& value);

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value First value
     *  \param args Other values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    inline ArgumentParser&
    aliases(std::string const& value,
            Args... args)
    {
        return aliases(std::vector<std::string>{ value, args... });
    }

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value Aliases values
     *
     *  \since v1.7.2
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    aliases(std::initializer_list<std::string> const& value);
#else
    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value First value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    aliases(std::string const& value);

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value1 First value
     *  \param value2 Second value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    aliases(std::string const& value1,
            std::string const& value2);
#endif  // C++11+

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value Parents values
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    parents(std::vector<ArgumentParser> const& value);

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value Parent value
     *  \param args Parents values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    inline ArgumentParser&
    parents(ArgumentParser const& value,
            Args... args)
    {
        return parents(std::vector<ArgumentParser>{ value, args... });
    }

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value Parents values
     *
     *  \since v1.7.2
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    parents(std::initializer_list<ArgumentParser> const& value);
#else
    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value Parent value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    parents(ArgumentParser const& value);

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value1 Parent value1
     *  \param value2 Parent value2
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    parents(ArgumentParser const& value1,
            ArgumentParser const& value2);
#endif  // C++11+

    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param value HelpFormatter value
     *
     *  \return Current argument parser reference
     */
    template <class T>
    ArgumentParser&
    formatter_class(
            T const& value)
    {
        m_formatter = detail::make_shared<T>(value);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'prefix_chars' value (default: "-")
     *
     *  \param value Prefix chars values (only punctuation characters)
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    prefix_chars(
            std::string const& value);

    /*!
     *  \brief Set argument parser 'fromfile_prefix_chars' value
     *
     *  \param value Fromfile prefix chars values (only punctuation characters)
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    fromfile_prefix_chars(
            std::string const& value);

    /*!
     *  \brief Set argument parser 'comment_prefix_chars' value
     *
     *  \param value Comment prefix chars values (only punctuation characters)
     *
     *  \since v1.8.1
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    comment_prefix_chars(
            std::string const& value);

    /*!
     *  \brief Set argument parser 'argument_default' value
     *
     *  \param value Argument default value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    argument_default(
            std::string const& value);

    /*!
     *  \brief Suppress argument parser 'argument_default' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    argument_default(
            _SUPPRESS value);

    /*!
     *  \brief Set argument parser 'conflict_handler' value
     *
     *  \param value Argument default value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    conflict_handler(
            std::string const& value);

    /*!
     *  \brief Set argument parser 'add_help' value (default: true)
     *
     *  \param value Add help flag
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    add_help(bool value);

    /*!
     *  \brief Set argument parser 'allow_abbrev' value (default: true)
     *
     *  \param value Allow abbrev flag
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    allow_abbrev(
            bool value) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set argument parser 'exit_on_error' value (default: true)
     *
     *  \param value Exit on error flag
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    exit_on_error(
            bool value) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set output width value (default: auto-detected or 80, min 33)
     *
     *  \param value Output width
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    output_width(
            std::size_t value) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'prog' value (default: argv[0] or "untitled")
     *
     *  \return Argument parser 'prog' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    prog() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'usage' value for default language
     *
     *  \return Argument parser 'usage' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    usage() const;

    /*!
     *  \brief Get title for argument parser 'usage' for default language
     *  (default: "usage")
     *
     *  \since v1.7.1
     *
     *  \return Title for argument parser 'usage'
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    usage_title() const;

    /*!
     *  \brief Get argument parser 'description' value for default language
     *
     *  \return Argument parser 'description' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    description() const;

    /*!
     *  \brief Get title for positional arguments for default language
     *  (default: "positional arguments")
     *
     *  \since v1.7.0
     *
     *  \return Title for positional arguments
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    positionals_title() const;

    /*!
     *  \brief Get title for operand arguments for default language
     *  (default: "operands")
     *
     *  \since v1.8.0
     *
     *  \return Title for operand arguments
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    operands_title() const;

    /*!
     *  \brief Get title for optional arguments for default language
     *  (default: "options")
     *
     *  \since v1.7.0
     *
     *  \return Title for optional arguments
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    optionals_title() const;

    /*!
     *  \brief Get argument parser 'epilog' value for default language
     *
     *  \return Argument parser 'epilog' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    epilog() const;

    /*!
     *  \brief Get argument parser 'help' message for default language
     *  (for subparsers)
     *
     *  \return Argument parser 'help' message
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    help() const;

    /*!
     *  \brief Get argument parser 'aliases' value (for subparsers)
     *
     *  \return Argument parser 'aliases' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    aliases() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'formatter_class' value
     *
     *  \since v1.7.5
     *
     *  \return Argument parser 'formatter_class' value
     */
    ARGPARSE_ATTR_NODISCARD
    HelpFormatter&
    formatter_class() ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'prefix_chars' value (default: "-")
     *
     *  \return Argument parser 'prefix_chars' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    prefix_chars() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'fromfile_prefix_chars' value
     *
     *  \return Argument parser 'fromfile_prefix_chars' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    fromfile_prefix_chars() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'comment_prefix_chars' value
     *
     *  \since v1.8.1
     *
     *  \return Argument parser 'comment_prefix_chars' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    comment_prefix_chars() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'argument_default' value
     *
     *  \return Argument parser 'argument_default' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    argument_default() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'conflict_handler' value
     *
     *  \return Argument parser 'conflict_handler' value
     */
    ARGPARSE_ATTR_NODISCARD
    std::string const&
    conflict_handler() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'add_help' value (default: true)
     *
     *  \return Argument parser 'add_help' value
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    add_help() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'allow_abbrev' value (default: true)
     *
     *  \return Argument parser 'allow_abbrev' value
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    allow_abbrev() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'exit_on_error' value (default: true)
     *
     *  \return Argument parser 'exit_on_error' value
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    exit_on_error() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get output width value (default: auto-detected or 80, min 33)
     *
     *  \return Output width value
     */
    ARGPARSE_ATTR_NODISCARD
    std::size_t
    output_width() const;

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flag values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    inline Argument&
    add_argument(
            Args... flags)
    {
        return add_argument(std::vector<std::string>{ flags... });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flag values
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::initializer_list<std::string> const& flags);
#else
    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag);

    /*!
     *  \brief Add argument with 2 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag1,
            std::string const& flag2);

    /*!
     *  \brief Add argument with 3 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *  \param flag3 Third value
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag1,
            std::string const& flag2,
            std::string const& flag3);

    /*!
     *  \brief Add argument with 4 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *  \param flag3 Third value
     *  \param flag4 Fourth value
     *
     *  \since v1.7.2
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::string const& flag1,
            std::string const& flag2,
            std::string const& flag3,
            std::string const& flag4);
#endif  // C++11+

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    Argument&
    add_argument(
            std::vector<std::string> const& flags);

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current parser reference
     */
    ArgumentParser&
    add_argument(
            Argument const& argument);

    /*!
     *  \brief Add argument group
     *
     *  \param title Group title (default: "")
     *  \param description Group description (default: "")
     *
     *  \return Current argument group reference
     */
    ArgumentGroup&
    add_argument_group(
            std::string const& title = std::string(),
            std::string const& description = std::string());

    /*!
     *  \brief Add mutually exclusive group
     *
     *  \param required Required flag (default: false)
     *
     *  \return Current mutually exclusive group reference
     */
    MutuallyExclusiveGroup&
    add_mutually_exclusive_group(
            bool required = false);

    /*!
     *  \brief Add subparsers. Only arguments of one subparser are allowed
     *
     *  \param title Subparsers title (default: "")
     *  \param description Subparsers description (default: "")
     *
     *  \return Current subparsers reference
     */
    SubParsers&
    add_subparsers(
            std::string const& title = std::string(),
            std::string const& description = std::string());

    /*!
     *  \brief Get subparsers. If no subparsers was added, returns nullptr
     *
     *  \return Current subparsers pointer or nullptr
     */
    ARGPARSE_ATTR_NODISCARD
    SubParsers*
    subparsers() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Check if subparsers was added
     *
     *  \since v1.8.3
     *
     *  \return True if subparsers was added, false otherwise
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    has_subparsers() const ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set argument parser 'handle' function.
     *  Called when the parser is executed and passed the value of the
     *  parser
     *
     *  \param func Handle function
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    handle(detail::func1<std::string const&>::type func) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set argument parser 'handle' function.
     *  Called when the parser is executed and passed the namespace
     *  of the parser
     *
     *  \param func Parse handle function
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    handle(detail::func1<Namespace const&>::type func) ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get the default value for a specific argument.
     *  If argument not found, returns empty string.
     *
     *  \param dest Argument destination name or flag
     *
     *  \return Default value for a specific argument
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    get_default(
            std::string const& dest) const;

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Set default values for a specific arguments
     *
     *  \param pairs List of pairs: { 'argument flag', 'default value' }
     *
     *  \since v1.7.2
     */
    void
    set_defaults(
      std::initializer_list<std::pair<std::string, std::string> > const& pairs);
#endif  // C++11+

    /*!
     *  \brief Set default values for a specific arguments
     *
     *  \param pairs Vector of pairs: { 'argument flag', 'default value' }
     */
    void
    set_defaults(
            std::vector<std::pair<std::string, std::string> > const& pairs);

    /*!
     *  \brief Parse command line arguments
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known command line arguments
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse intermixed command line arguments
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known intermixed command line arguments
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

#ifdef ARGPARSE_CXX_11
    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;
#endif  // C++11+

#ifdef ARGPARSE_HAS_OPTIONAL
    /*!
     *  \brief Try parse command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse intermixed command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_intermixed_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_intermixed_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_intermixed_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known intermixed command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            std::string const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const;

    /*!
     *  \brief Try parse known intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;
#endif  // ARGPARSE_HAS_OPTIONAL

    /*!
     *  \brief Check if environment variable with name exists (from envp[])
     *
     *  \param name Environment variable name
     *
     *  \since v1.8.3
     *
     *  \return True if environment variable name with exists, false otherwise
     */
    ARGPARSE_ATTR_NODISCARD
    bool
    has_env(std::string const& name) const;

    /*!
     *  \brief Get environment variable value (from envp[])
     *
     *  \param name Environment variable name
     *
     *  \since v1.8.0
     *
     *  \return Environment variable value if exists, empty string otherwise
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    get_env(std::string const& name) const;

    /*!
     *  \brief Get environment variables list (from envp[])
     *
     *  \since v1.8.2
     *
     *  \return Environment variable values list
     */
    ARGPARSE_ATTR_NODISCARD
    std::list<std::pair<std::string, std::string> > const&
    list_env() const;

    /*!
     *  \brief Run self-test and print report to output stream
     *  for default language
     *
     *  \param os Output stream (default: std::cout)
     *
     *  \since v1.7.2
     *
     *  \return True if no warnings or errors were found, false otherwise
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    bool
    self_test(
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Run self-test and print report for selected language
     *  to output stream
     *
     *  \param lang Language value
     *  \param os Output stream (default: std::cout)
     *
     *  \since v1.7.2
     *
     *  \return True if no warnings or errors were found, false otherwise
     */
    ARGPARSE_ATTR_MAYBE_UNUSED
    bool
    self_test(
            std::string const& lang,
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Print a bash completion to output stream.
     *  Copy the contents to ~/.bashrc or create a script file and use it
     *
     *  \param os Output stream (default: std::cout)
     *
     *  \since v1.7.2
     */
    void
    print_bash_completion(
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Print a program usage for default language to output stream
     *
     *  \param os Output stream (default: std::cout)
     */
    void
    print_usage(
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Print a program usage for selected language to output stream
     *
     *  \param lang Language value
     *  \param os Output stream (default: std::cout)
     *
     *  \since v1.7.1
     */
    void
    print_usage(
            std::string const& lang,
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Print a help message for default language to output stream
     *
     *  \param os Output stream (default: std::cout)
     */
    void
    print_help(
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Print a help message for selected language to output stream
     *
     *  \param lang Language value
     *  \param os Output stream (default: std::cout)
     *
     *  \since v1.7.1
     */
    void
    print_help(
            std::string const& lang,
            std::ostream& os = std::cout) const;

    /*!
     *  \brief Return a string containing a bash completion.
     *  Copy the contents to ~/.bashrc or create a script file and use it
     *
     *  \param lang Language value
     *
     *  \since v1.7.2
     *
     *  \return Bash completion
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    format_bash_completion() const;

    /*!
     *  \brief Return a string containing a program usage for selected language
     *
     *  \param lang Language value (default: "")
     *
     *  \return Program usage for selected language
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    format_usage(
            std::string const& lang = std::string()) const;

    /*!
     *  \brief Return a string containing a help message for selected language
     *
     *  \param lang Language value (default: "")
     *
     *  \return Help message for selected language
     */
    ARGPARSE_ATTR_NODISCARD
    std::string
    format_help(
            std::string const& lang = std::string()) const;

    /*!
     *  \brief Terminates the program, exiting with the specified status and,
     *  if given, prints a message before that
     *
     *  \param status Status code (default: 0)
     *  \param message Error message (default: "")
     */
    virtual void
    exit(int status = 0,
            std::string const& message = std::string()) const;

    /*!
     *  \brief Prints a usage message including the message to the
     *  standard error and terminates the program with a status code of 2
     *
     *  \param message Error message
     */
    void
    error(std::string const& message) const;

    /*!
     *  \brief Arguments that are read from a file
     *  (see the fromfile_prefix_chars function)
     *  are read one argument per line.
     *  convert_arg_line_to_args() can be overridden for fancier reading
     *
     *  \param arg_line Line with arguments
     *
     *  \return Arguments to parse
     */
    virtual std::vector<std::string>
    convert_arg_line_to_args(
            std::string const& arg_line) const;

private:
    std::string
    default_language() const;

    struct ParserInfo
    {
        explicit
        ParserInfo(
                ArgumentParser const* parser,
                _Storage const& storage,
                SubParsersInfo const& subparsers,
                pArguments const& optional,
                pArguments const& operand);

        ParserInfo(
                ParserInfo const& orig);

        ParserInfo&
        operator =(
                ParserInfo const& rhs);

        // -- data ------------------------------------------------------------
        ArgumentParser const* parser;
        pArguments optional;
        pArguments operand;
        _Storage storage;
        SubParsersInfo subparsers;
        std::string lang;
        bool have_negative_args;
    };
    typedef std::list<ParserInfo> Parsers;
    typedef std::list<ParserInfo>::iterator pi_iterator;

    static ParserInfo
    parser_info(
            ArgumentParser const* parser,
            _Storage const& storage,
            SubParsersInfo const& subparsers);

    void
    throw_error(
            std::string const& message,
            std::string const& lang = std::string(),
            std::ostream& os = std::cerr) const;

    std::string
    despecify(
            std::string const& str) const;

    void
    process_add_argument();

    Namespace
    on_parse_arguments(
            std::vector<std::string> const& args,
            bool only_known,
            bool intermixed,
            Namespace const& space) const;

#ifdef ARGPARSE_HAS_OPTIONAL
    std::optional<Namespace>
    on_try_parse_arguments(
            std::vector<std::string> const& args,
            bool only_known,
            bool intermixed,
            Namespace const& space) const;
#endif  // ARGPARSE_HAS_OPTIONAL

    std::vector<std::string>
    read_args_from_file(
            std::vector<std::string> const& arguments) const;

    Namespace
    parse_arguments(
            std::vector<std::string> const& arguments,
            bool only_known,
            bool intermixed,
            Namespace const& space) const;

    void
    check_namespace(
            Namespace const& space) const;

    void
    check_mutex_arguments() const;

    static void
    check_intermixed_remainder(
            bool intermixed,
            pArguments const& positional);

    static Namespace
    create_namespace(
            bool only_known,
            detail::rval<_Storage>::type storage,
            detail::rval<std::vector<std::string> >::type unrecognized_args);

    static void
    validate_arguments(
            pArguments const& args);

    void
    validate_argument_value(
            ArgumentParser const* parser,
            Argument const& arg,
            std::string const& value) const;

    void
    storage_have_value(
            Parsers& parsers,
            pArgument const& arg) const;

    void
    storage_store_value(
            Parsers& parsers,
            pArgument const& arg,
            std::string const& val) const;

    void
    storage_store_values(
            Parsers& parsers,
            pArgument const& arg,
            std::vector<std::string> const& values) const;

    void
    storage_store_n_values(
            Parsers& parsers,
            pArgument const& arg,
            std::list<std::string>& arguments,
            std::size_t n) const;

    void
    storage_store_default_value(
            Parsers& parsers,
            pArgument const& arg) const;

    bool
    storage_is_positional_arg_stored(
            Parsers& parsers,
            pArgument const& arg) const;

    void
    storage_optional_store_func(
            Parsers& parsers,
            std::string const& arg,
            pArgument const& tmp,
            std::size_t n) const;

    void
    storage_optional_store(
            Parsers& parsers,
            std::vector<std::string> const& equals,
            std::vector<std::string> const& args,
            std::size_t& i,
            bool was_pseudo_arg,
            std::string const& arg,
            pArgument const& tmp) const;

    void
    storage_optional_store_const(
            Parsers& parsers,
            std::vector<std::string> const& equals,
            std::string const& arg,
            pArgument const& tmp) const;

    void
    process_optional_help(
            ParserInfo const& info,
            std::vector<std::string> const& equals,
            std::string const& arg,
            pArgument const& tmp) const;

    void
    process_optional_version(
            ParserInfo const& info,
            std::vector<std::string> const& equals,
            std::string const& arg,
            pArgument const& tmp) const;

    void
    match_positional_minimum(
            Parsers& parsers,
            std::list<std::string>& arguments,
            pArgument const& arg) const;

    void
    match_positional_more_zero(
            Parsers& parsers,
            std::list<std::string>& arguments,
            pArgument const& arg,
            std::size_t& over_args) const;

    void
    match_positional_optional(
            Parsers& parsers,
            std::list<std::string>& arguments,
            pArgument const& arg,
            std::size_t& over_args,
            std::size_t one_args) const;

    void
    match_positional_default(
            Parsers& parsers,
            std::list<std::string>& arguments,
            pArgument const& arg) const;

    void
    match_positionals(
            Parsers& parsers,
            std::size_t& pos,
            pArguments const& positional,
            std::list<std::string>& arguments,
            std::size_t finish,
            std::size_t min_args,
            std::size_t one_args,
            bool more_args) const;

    static bool
    finish_analyze_positional(
            pArgument const& arg,
            std::list<std::string> const& args,
            std::size_t& one_args,
            bool& more_args,
            std::size_t& min_args,
            bool first,
            bool read_all_args);

    void
    match_args_partial(
            Parsers& parsers,
            std::size_t& pos,
            pArguments const& positional,
            std::vector<std::string>& unrecognized_args,
            std::list<std::string>& args,
            bool read_all_args = true) const;

    bool
    try_capture_parser(
            Parsers& parsers,
            std::size_t& pos,
            pArguments& positional,
            std::vector<std::string>& unrecognized_args,
            std::list<std::string>& args,
            bool read_all_args) const;

    void
    check_abbreviations(
            Parsers const& parsers,
            bool was_pseudo_arg,
            std::vector<std::string>& arguments,
            std::size_t i) const;

    static bool
    is_remainder_positional(
            std::size_t pos,
            pArguments const& positional,
            Parsers const& parsers);

    static pArgument const
    get_optional_arg_by_flag(
            bool was_pseudo_arg,
            ParserInfo const& info,
            std::string const& key);

    void
    separate_arg_abbrev(
            ParserInfo const& info,
            std::vector<std::string>& temp,
            std::string const& arg,
            std::string const& name,
            pArguments const& args) const;

    void
    process_positional_args(
            std::vector<std::string> const& parsed_arguments,
            std::size_t& i,
            Parsers& parsers,
            bool was_pseudo_arg,
            bool intermixed,
            std::list<std::string>& intermixed_args,
            std::size_t& pos,
            pArguments& positional,
            std::vector<std::string>& unrecognized_args) const;

    static void
    check_mutex_groups(
            Parsers const& parsers);

    static void
    process_required_arguments(
            std::vector<std::string>& required_args,
            pArguments const& arguments,
            _Storage const& storage);

    static void
    process_required_subparsers(
            bool required,
            std::size_t pos,
            SubParsersInfo const& info,
            std::string& res);

    static void
    process_required_check(
            ParserInfo const& info,
            _Storage const& storage);

    bool
    skip_positional_required_check(
            Parsers& parsers,
            pArgument const& arg) const;

    void
    check_required_args(
            Parsers& parsers,
            std::size_t& pos,
            pArguments const& positional) const;

    void
    check_unrecognized_args(
            bool only_known,
            std::vector<std::string> const& args) const;

    void
    default_values_post_trigger(
            _Storage& storage) const;

    static void
    namespace_post_trigger(
            Parsers& parsers,
            bool only_known,
            std::vector<std::string> const& unrecognized_args);

    static bool
    is_default_value_stored(
            std::list<pArgument>& arguments,
            std::string const& dest,
            std::string const& value);

    std::size_t
    argument_help_limit() const;

    SubParsersInfo
    subparsers_info(
            bool add_suppress,
            std::size_t offset = 0) const;

    std::string
    subparsers_prog_args() const;

    static void
    print_subparsers(
            bool need_print,
            SubParsersInfo const& info,
            std::size_t index,
            HelpFormatter const& formatter,
            std::string const& prog,
            std::size_t size,
            std::size_t width,
            std::string const& lang,
            std::ostream& os);

    void
    update_prog(
            std::string const& parent_prog);

    void
    test_overview(
            std::string const& lang,
            std::ostream& os) const;

    static void
    overview_subparsers(
            ArgumentParser const* parser,
            std::string const& indent,
            std::ostream& os);

    bool
    test_diagnostics(
            std::string const& lang,
            std::ostream& os) const;

    static void
    test_argument_parser(
            std::string const& lang,
            std::ostream& os,
            ArgumentParser const* parser,
            std::pair<std::size_t, std::size_t>& diagnostics);

    void
    handle(std::string const& str) const;

    void
    parse_handle(
            bool only_known,
            _Storage const& storage,
            std::vector<std::string> const& unrecognized_args) const;

    // -- data ----------------------------------------------------------------
    detail::shared_ptr<_ArgumentData> m_data;
    std::string m_name;
    std::string m_prog;
    detail::SValue<detail::TranslationPack> m_usage;
    detail::TranslationPack m_usage_title;
    detail::TranslationPack m_description;
    detail::TranslationPack m_positionals_title;
    detail::TranslationPack m_operands_title;
    detail::TranslationPack m_optionals_title;
    detail::TranslationPack m_epilog;
    detail::TranslationPack m_help;
    std::vector<std::string> m_aliases;
    detail::shared_ptr<HelpFormatter> m_formatter;
    std::string m_prefix_chars;
    std::string m_fromfile_prefix_chars;
    std::string m_comment_prefix_chars;
    detail::SValue<std::string> m_argument_default;
    std::size_t m_output_width;
    std::list<pGroup> m_groups;
    std::list<MutuallyExclusiveGroup> m_mutex_groups;
    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
    std::list<std::pair<std::string, std::string> > m_env_variables;
    pSubParsers m_subparsers;
    std::size_t m_subparsers_position;
    detail::func1<std::string const&>::type m_handle;
    detail::func1<Namespace const&>::type m_parse_handle;
    bool m_allow_abbrev;
    bool m_exit_on_error;
};

// -- implementation ----------------------------------------------------------
#ifdef ARGPARSE_INL
namespace detail {
ARGPARSE_INLINE_VARIABLE std::size_t ARGPARSE_USE_CONSTEXPR _min_width     = 33;
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _prefix_char         = '-';
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _prefix_chars[]      = "-";
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _pseudo_arg[]       = "--";
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _space               = ' ';
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _equals[]            = "=";
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _none[]           = "None";
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR _prog[]       = "untitled";
ARGPARSE_INLINE_VARIABLE char ARGPARSE_USE_CONSTEXPR
                                                   _suppress[] = "==SUPPRESS==";

enum ConflictHandler ARGPARSE_ENUM_TYPE(uint8_t)
{
    _conflict_error   = 0,  // "error"
    _conflict_resolve = 1,  // "resolve"
};

ARGPARSE_INLINE_VARIABLE int32_t ARGPARSE_USE_CONSTEXPR
_bool_action = argparse::store_true | argparse::store_false;
ARGPARSE_INLINE_VARIABLE int32_t ARGPARSE_USE_CONSTEXPR
_const_action = argparse::store_const | argparse::append_const;
ARGPARSE_INLINE_VARIABLE int32_t ARGPARSE_USE_CONSTEXPR
_store_const_action = _store_action | _const_action;

typedef detail::shared_ptr<Argument> pArgument;
typedef std::vector<pArgument> pArguments;
typedef detail::shared_ptr<SubParsers> pSubParsers;

ARGPARSE_INL void
_erase_remove(
        pArguments& args,
        pArgument arg)
{
    pArguments::iterator it = args.begin();
    for ( ; it != args.end() && (*it) != arg; ++it) {
    }
    if (it != args.end()) {
        for (pArguments::iterator i = it; ++i != args.end(); ) {
            if (!(*i == arg)) {
                *it++ = ARGPARSE_MOVE(*i);
            }
        }
        args.erase(it, args.end());
    }
}

// -- templates ---------------------------------------------------------------
#ifdef ARGPARSE_CXX_11
template <class T = std::string, class... Args>
inline std::vector<T>
_vector(Args... args)
{
    return std::vector<T>{ std::move(args)... };
}
#else
template <class T>
std::vector<T>
_vector(T const& arg1)
{
    std::vector<T> res;
    res.push_back(arg1);
    return res;
}

template <class T>
std::vector<T>
_vector(T const& arg1,
        T const& arg2)
{
    std::vector<T> res;
    res.push_back(arg1);
    res.push_back(arg2);
    return res;
}

template <class T>
std::vector<T>
_vector(T const& arg1,
        T const& arg2,
        T const& arg3)
{
    std::vector<T> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    return res;
}

template <class T>
std::vector<T>
_vector(T const& arg1,
        T const& arg2,
        T const& arg3,
        T const& arg4)
{
    std::vector<T> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    return res;
}
#endif  // C++11+

template <class T>
void
_insert_to_end(
        std::list<T> const& from,
        std::list<T>& to)
{
    to.insert(to.end(), from.begin(), from.end());
}

template <class T>
void
_insert_to_end(
        std::list<T> const& from,
        std::vector<T>& to)
{
    to.reserve(to.size() + from.size());
    to.insert(to.end(), from.begin(), from.end());
}

template <class T>
void
_insert_to_end(
        std::vector<T> const& from,
        std::vector<T>& to)
{
    to.reserve(to.size() + from.size());
    to.insert(to.end(), from.begin(), from.end());
}

template <class T>
void
_move_insert_to_end(
        std::list<T>& from,
        std::vector<T>& to)
{
#ifdef ARGPARSE_CXX_11
    to.reserve(to.size() + from.size());
    to.insert(to.end(),
              std::make_move_iterator(from.begin()),
              std::make_move_iterator(from.end()));
#else
    _insert_to_end(from, to);
#endif  // C++11+
}

template <class T>
void
_move_insert_to_end(
        std::vector<T>& from,
        std::vector<T>& to)
{
#ifdef ARGPARSE_CXX_11
    to.reserve(to.size() + from.size());
    to.insert(to.end(),
              std::make_move_iterator(from.begin()),
              std::make_move_iterator(from.end()));
#else
    _insert_to_end(from, to);
#endif  // C++11+
}

template <class T>
void
_move_insert_to(
        std::vector<T>& from,
        std::vector<T>& to,
        std::size_t i)
{
    to.reserve(to.size() + from.size());
    typedef typename std::vector<T>::difference_type dtype;
#ifdef ARGPARSE_CXX_11
    to.insert(std::next(to.begin(), static_cast<dtype>(i)),
              std::make_move_iterator(from.begin()),
              std::make_move_iterator(from.end()));
#else
    to.insert(to.begin() + static_cast<dtype>(i), from.begin(), from.end());
#endif  // C++11+
}

template <class T>
void
_move_replace_at(
        std::vector<T>& from,
        std::vector<T>& to,
        std::size_t i)
{
    typedef typename std::vector<T>::difference_type dtype;
#ifdef ARGPARSE_CXX_11
    to.erase(std::next(to.begin(), static_cast<dtype>(i)));
    to.insert(std::next(to.begin(), static_cast<dtype>(i)),
              std::make_move_iterator(from.begin()),
              std::make_move_iterator(from.end()));
#else
    to.erase(to.begin() + static_cast<dtype>(i));
    to.insert(to.begin() + static_cast<dtype>(i), from.begin(), from.end());
#endif  // C++11+
}

template <class T>
bool
_exists(T const& value,
        std::vector<T> const& vec)
{
    typename std::vector<T>::const_iterator it = vec.begin();
    for ( ; it != vec.end() && (*it) != value; ++it) {
    }
    return it != vec.end();
}

ARGPARSE_INL bool
_exists(char value,
        std::string const& str)
{
    std::string::const_iterator it = str.begin();
    for ( ; it != str.end() && (*it) != value; ++it) {
    }
    return it != str.end();
}

// -- utf8 support ------------------------------------------------------------
// since v1.7.0
typedef uint32_t codepoint;

ARGPARSE_INL uint8_t ARGPARSE_CONSTEXPR
_char_to_u8(
        char c)
{
    return static_cast<uint8_t>(c);
}

ARGPARSE_INL char ARGPARSE_CONSTEXPR
_u8_to_char(
        uint8_t c)
{
    return static_cast<char>(c);
}

ARGPARSE_INL codepoint ARGPARSE_CONSTEXPR
_char_to_u32(
        char c)
{
    return static_cast<codepoint>(_char_to_u8(c));
}

ARGPARSE_INL char ARGPARSE_CONSTEXPR
_u32_to_char(
        codepoint c)
{
    return _u8_to_char(static_cast<uint8_t>(c));
}

ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_1b_mask = 0x80;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_1b_bits = 0x00;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_2b_mask = 0xe0;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_2b_bits = 0xc0;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_3b_mask = 0xf0;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_3b_bits = 0xe0;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_4b_mask = 0xf8;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_4b_bits = 0xf0;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_ct_mask = 0xc0;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_ct_bits = 0x80;
ARGPARSE_INLINE_VARIABLE uint32_t ARGPARSE_USE_CONSTEXPR   _utf8_ct_invm = 0x3f;

ARGPARSE_INL std::size_t ARGPARSE_CONSTEXPR
_utf8_codepoint_size(
        uint8_t byte)
{
    return (byte & _utf8_1b_mask) == _utf8_1b_bits ? 1
         : (byte & _utf8_2b_mask) == _utf8_2b_bits ? 2
         : (byte & _utf8_3b_mask) == _utf8_3b_bits ? 3
         : (byte & _utf8_4b_mask) == _utf8_4b_bits ? 4 : 0;
}

ARGPARSE_INL std::pair<bool, std::size_t>
_utf8_length(
        std::string const& str,
        std::ostream& err = std::cerr)
{
    std::size_t res = 0;
    std::size_t i = 0;
    while (i < str.size()) {
        std::size_t cp_size = _utf8_codepoint_size(_char_to_u8(str.at(i)));
        if (cp_size == 0) {
            err << "argparse error [skip]: invalid code point for string "
                << "'" << str << "'" << std::endl;
            return std::make_pair(false, str.size());
        }
        if (i + cp_size > str.size()) {
            err << "argparse error [skip]: code point for string '"
                << str << "' would be out of bounds" << std::endl;
            return std::make_pair(false, str.size());
        }
        for (std::size_t n = 1; n < cp_size; ++n) {
            if (str.at(i + n) == '\0') {
                err << "argparse error [skip]: string '" << str << "' "
                    << "is NUL-terminated in the middle of the code point"
                    << std::endl;
                return std::make_pair(false, str.size());
            } else if ((_char_to_u8(str.at(i + n)) & _utf8_ct_mask)
                                                  != _utf8_ct_bits) {
                err << "argparse error [skip]: invalid byte in code point"
                    << " for string '" << str << "'" << std::endl;
                return std::make_pair(false, str.size());
            }
        }
        i += cp_size;
        ++res;
    }
    return std::make_pair(true, res);
}

// since v1.7.3
ARGPARSE_INL bool
_is_utf8_string(
        std::string const& str)
{
    std::stringstream ss;
    return _utf8_length(str, ss).first;
}

// since v1.7.3
ARGPARSE_INL codepoint
_to_upper_codepoint(
        codepoint cp)
{
    // unicode 16 to upper case
    // BASIC LATIN
    if (0x0061 <= cp && cp <= 0x007a) return cp - 0x20;
    // LATIN-1 SUPPLEMENT
    if (cp == 0x00b5) return 0x039c;
    if ((0x00e0 <= cp && cp <= 0x00f6) || (0x00f8 <= cp && cp <= 0x00fe)) {
        return cp - 0x20;
    }
    if (cp == 0x00ff) return 0x0178;
    // LATIN EXTENDED-A
    if ((cp & 1)
        && ((0x0100 <= cp && cp <= 0x012f) || (0x0132 <= cp && cp <= 0x0137)
         || (0x014a <= cp && cp <= 0x0177))) {
        return cp - 0x1;
    }
    if (cp == 0x0131) return 0x0049;
    if (!(cp & 1)
        && ((0x0139 <= cp && cp <= 0x0148) || (0x0179 <= cp && cp <= 0x017e))) {
        return cp - 0x1;
    }
    if (cp == 0x017f) return 0x0053;
    // LATIN EXTENDED-B
    if (cp == 0x0180) return 0x0243;
    if ((cp & 1)
        && ((0x0182 <= cp && cp <= 0x0185) || (0x01a0 <= cp && cp <= 0x01a5)
         || (0x01de <= cp && cp <= 0x01ef) || (0x01fa <= cp && cp <= 0x021f)
         || (0x0222 <= cp && cp <= 0x0233) || (0x0246 <= cp && cp <= 0x024f))) {
        return cp - 0x1;
    }
    if (cp == 0x0188 || cp == 0x018c || cp == 0x0192 || cp == 0x0199) {
        return cp - 0x1;
    }
    if (cp == 0x0195) return 0x01f6;
    if (cp == 0x019a) return 0x023d;
    if (cp == 0x019b) return 0xa7dc;
    if (cp == 0x019e) return 0x0220;
    if (cp == 0x01a8 || cp == 0x01ad || cp == 0x01b0
     || cp == 0x01b9 || cp == 0x01bd) {
        return cp - 0x1;
    }
    if (!(cp & 1)
        && ((0x01b3 <= cp && cp <= 0x01b6) || (0x01cd <= cp && cp <= 0x01dc))) {
        return cp - 0x1;
    }
    if (cp == 0x01bf) return 0x01f7;
    if (cp == 0x01c5 || cp == 0x01c6) return 0x01c4;
    if (cp == 0x01c8 || cp == 0x01c9) return 0x01c7;
    if (cp == 0x01cb || cp == 0x01cc) return 0x01ca;
    if (cp == 0x01dd) return 0x018e;
    if (cp == 0x01f2 || cp == 0x01f3) return 0x01f1;
    if (cp == 0x01f5 || cp == 0x01f9) return cp - 0x1;
    if (cp == 0x023c || cp == 0x0242) return cp - 0x1;
    if (cp == 0x023f || cp == 0x0240) return cp + 0x2a3f;
    // IPA EXTENSIONS
    if (cp == 0x0250) return 0x2c6f;
    if (cp == 0x0251) return 0x2c6d;
    if (cp == 0x0252) return 0x2c70;
    if (cp == 0x0253) return 0x0181;
    if (cp == 0x0254) return 0x0186;
    if (cp == 0x0256 || cp == 0x0257) return cp - 0xcd;
    if (cp == 0x0259) return 0x018f;
    if (cp == 0x025b) return 0x0190;
    if (cp == 0x025c) return 0xa7ab;
    if (cp == 0x0260) return 0x0193;
    if (cp == 0x0261) return 0xa7ac;
    if (cp == 0x0263) return 0x0194;
    if (cp == 0x0264) return 0xa7cb;
    if (cp == 0x0265) return 0xa78d;
    if (cp == 0x0266) return 0xa7aa;
    if (cp == 0x0268) return 0x0197;
    if (cp == 0x0269) return 0x0196;
    if (cp == 0x026a) return 0xa7ae;
    if (cp == 0x026b) return 0x2c62;
    if (cp == 0x026c) return 0xa7ad;
    if (cp == 0x026f) return 0x019c;
    if (cp == 0x0271) return 0x2c6e;
    if (cp == 0x0272) return 0x019d;
    if (cp == 0x0275) return 0x019f;
    if (cp == 0x027d) return 0x2c64;
    if (cp == 0x0280 || cp == 0x0283 || cp == 0x0288) return cp - 0xda;
    if (cp == 0x0282) return 0xa7c5;
    if (cp == 0x0287) return 0xa7b1;
    if (cp == 0x0289) return 0x0244;
    if (cp == 0x028a || cp == 0x028b) return cp - 0xd9;
    if (cp == 0x028c) return 0x0245;
    if (cp == 0x0292) return 0x01b7;
    if (cp == 0x029d) return 0xa7b2;
    if (cp == 0x029e) return 0xa7b0;
    // COMBINING DIACRITICAL MARKS
    if (cp == 0x0345) return 0x0399;
    // BASIC GREEK
    if (cp == 0x0371 || cp == 0x0373 || cp == 0x0377) return cp - 0x1;
    if (0x037b <= cp && cp <= 0x037d) return cp + 0x82;
    if (cp == 0x03ac) return 0x0386;
    if (0x03ad <= cp && cp <= 0x03af) return cp - 0x25;
    if ((0x03b1 <= cp && cp <= 0x03c1) || (0x03c3 <= cp && cp <= 0x03cb)) {
        return cp - 0x20;
    }
    if (cp == 0x03c2) return 0x03a3;
    if (cp == 0x03cc) return 0x038c;
    if (cp == 0x03cd || cp == 0x03ce) return cp - 0x3f;
    // GREEK SYMBOLS AND COPTIC
    if (cp == 0x03d0) return 0x0392;
    if (cp == 0x03d1) return 0x0398;
    if (cp == 0x03d5) return 0x03a6;
    if (cp == 0x03d6) return 0x03a0;
    if (cp == 0x03d7) return 0x03cf;
    if ((cp & 1) && ((0x03d8 <= cp && cp <= 0x03ef))) return cp - 0x1;
    if (cp == 0x03f0) return 0x039a;
    if (cp == 0x03f1) return 0x03a1;
    if (cp == 0x03f2) return 0x03f9;
    if (cp == 0x03f3) return 0x037f;
    if (cp == 0x03f5) return 0x0395;
    if (cp == 0x03f8 || cp == 0x03fb) return cp - 0x1;
    // CYRILLIC
    if (0x0430 <= cp && cp <= 0x044f) return cp - 0x20;
    if (0x0450 <= cp && cp <= 0x045f) return cp - 0x50;
    if ((cp & 1)
        && ((0x0460 <= cp && cp <= 0x0481) || (0x048a <= cp && cp <= 0x04bf)
         || (0x04d0 <= cp && cp <= 0x04ff))) {
        return cp - 0x1;
    }
    if (!(cp & 1) && (0x04c1 <= cp && cp <= 0x04ce)) return cp - 0x1;
    if (cp == 0x04cf) return 0x04c0;
    // CYRILLIC SUPPLEMENT
    if ((cp & 1) && (0x0500 <= cp && cp <= 0x052f)) return cp - 0x1;
    // ARMENIAN
    if (0x0561 <= cp && cp <= 0x0586) return cp - 0x30;
    // GEORGIAN
    if ((0x10d0 <= cp && cp <= 0x10fa) || (0x10fd <= cp && cp <= 0x10ff)) {
        return cp + 0xbc0;
    }
    // CHEROKEE
    if (0x13f8 <= cp && cp <= 0x13fd) return cp - 0x8;
    // CYRILLIC
    if (cp == 0x1c80) return 0x0412;
    if (cp == 0x1c81) return 0x0414;
    if (cp == 0x1c82) return 0x041e;
    if (cp == 0x1c83) return 0x0421;
    if (cp == 0x1c84 || cp == 0x1c85) return 0x0422;
    if (cp == 0x1c86) return 0x042a;
    if (cp == 0x1c87) return 0x0462;
    if (cp == 0x1c88) return 0xa64a;
    if (cp == 0x1c8a) return 0x1c89;
    // PHONETIC EXTENSIONS
    if (cp == 0x1d79) return 0xa77d;
    if (cp == 0x1d7d) return 0x2c63;
    if (cp == 0x1d8e) return 0xa7c6;
    // LATIN EXTENDED ADDITIONAL
    if ((cp & 1)
        && ((0x1e00 <= cp && cp <= 0x1e95) || (0x1ea0 <= cp && cp <= 0x1ef9)
         || (0x1efa <= cp && cp <= 0x1eff))) {
        return cp - 0x1;
    }
    if (cp == 0x1e9b) return 0x1e60;
    // GREEK EXTENDED
    if (!(cp & 8)
        && ((0x1f00 <= cp && cp <= 0x1f15) || (0x1f20 <= cp && cp <= 0x1f45)
         || (0x1f60 <= cp && cp <= 0x1f6f) || (0x1f80 <= cp && cp <= 0x1faf))) {
        return cp + 0x8;
    }
    if (!(cp & 8) && (cp & 1) && (0x1f51 <= cp && cp <= 0x1f5f)) {
        return cp + 0x8;
    }
    if (cp == 0x1fb0 || cp == 0x1fb1 || cp == 0x1fd0 || cp == 0x1fd1
     || cp == 0x1fe0 || cp == 0x1fe1) {
        return cp + 0x8;
    }
    if (cp == 0x1f70 || cp == 0x1f71) return cp + 0x4a;
    if (0x1f72 <= cp && cp <= 0x1f75) return cp + 0x56;
    if (cp == 0x1f76 || cp == 0x1f77) return cp + 0x64;
    if (cp == 0x1f78 || cp == 0x1f79) return cp + 0x80;
    if (cp == 0x1f7a || cp == 0x1f7b) return cp + 0x70;
    if (cp == 0x1f7c || cp == 0x1f7d) return cp + 0x7e;
    if (cp == 0x1fb3 || cp == 0x1fc3 || cp == 0x1ff3) return cp + 0x9;
    if (cp == 0x1fbe) return 0x0399;
    if (cp == 0x1fe5) return 0x1fec;
    // LETTERLIKE SYMBOLS
    if (cp == 0x214e) return 0x2132;
    // NUMBER FORMS
    if (0x2170 <= cp && cp <= 0x217f) return cp - 0x10;
    if (cp == 0x2184) return cp - 0x1;
    // ENCLOSED ALPHANUMERICS : CIRCLED LATIN
    if (0x24d0 <= cp && cp <= 0x24e9) return cp - 0x1a;
    // GLAGOLITIC
    if (0x2c30 <= cp && cp <= 0x2c5f) return cp - 0x30;
    // LATIN EXTENDED-C
    if ((cp & 1)
        && ((0x2c60 <= cp && cp <= 0x2c61) || (0x2c72 <= cp && cp <= 0x2c73))) {
        return cp - 0x1;
    }
    if (cp == 0x2c65) return 0x023a;
    if (cp == 0x2c66) return 0x023e;
    if (!(cp & 1)
        && ((0x2c67 <= cp && cp <= 0x2c6c) || (0x2c75 <= cp && cp <= 0x2c76))) {
        return cp - 0x1;
    }
    // COPTIC
    if ((cp & 1)
        && ((0x2c80 <= cp && cp <= 0x2ce3) || (0x2cf2 <= cp && cp <= 0x2cf3))) {
        return cp - 0x1;
    }
    if (!(cp & 1) && (0x2ceb <= cp && cp <= 0x2cee)) return cp - 0x1;
    // GEORGIAN SUPPLEMENT
    if ((0x2d00 <= cp && cp <= 0x2d25) || cp == 0x2d27 || cp == 0x2d2d) {
        return cp - 0x1c60;
    }
    // CYRILLIC SUPPLEMENT 2
    if ((cp & 1)
        && ((0xa640 <= cp && cp <= 0xa66d) || (0xa680 <= cp && cp <= 0xa69b))) {
        return cp - 0x1;
    }
    // LATIN EXTENDED-D
    if ((cp & 1)
        && ((0xa722 <= cp && cp <= 0xa72f) || (0xa732 <= cp && cp <= 0xa76f)
         || (0xa77e <= cp && cp <= 0xa787) || (0xa790 <= cp && cp <= 0xa793)
         || (0xa796 <= cp && cp <= 0xa7a9) || (0xa7b4 <= cp && cp <= 0xa7c3)
         || (0xa7cd == cp)
         || (0xa7d0 <= cp && cp <= 0xa7d1) || (0xa7d6 <= cp && cp <= 0xa7db))) {
        return cp - 0x1;
    }
    if (!(cp & 1)
        && ((0xa779 <= cp && cp <= 0xa77c) || (0xa78b <= cp && cp <= 0xa78c)
         || (0xa7c7 <= cp && cp <= 0xa7ca) || (0xa7f5 <= cp && cp <= 0xa7f6))) {
        return cp - 0x1;
    }
    if (cp == 0xa794) return cp + 0x30;
    // LATIN SMALL LETTER CHI
    if (cp == 0xab53) return 0xa7b3;
    // CHEROKEE
    if (0xab70 <= cp && cp <= 0xabbf) return cp - 0x97d0;
    // HALFWIDTH AND FULLWIDTH FORMS
    if (0xff41 <= cp && cp <= 0xff5a) return cp - 0x20;
    // DESERET
    if (0x10428 <= cp && cp <= 0x1044f) return cp - 0x28;
    // OSAGE
    if (0x104d8 <= cp && cp <= 0x104fb) return cp - 0x28;
    // CAUCASIAN ALBANIAN
    if ((0x10597 <= cp && cp <= 0x105a1) || (0x105a3 <= cp && cp <= 0x105b1)
     || (0x105b3 <= cp && cp <= 0x105b9) || (0x105bb <= cp && cp <= 0x105bc)) {
        return cp - 0x27;
    }
    // OLD HUNGARIAN
    if (0x10cc0 <= cp && cp <= 0x10cf2) return cp - 0x40;
    // GARAY
    if (0x10d70 <= cp && cp <= 0x10d85) return cp - 0x20;
    // WARANG CITI
    if (0x118c0 <= cp && cp <= 0x118df) return cp - 0x20;
    // MEDEFAIDRIN
    if (0x16e60 <= cp && cp <= 0x16e7f) return cp - 0x20;
    // ADLAM
    if (0x1e922 <= cp && cp <= 0x1e943) return cp - 0x22;
    return cp;
}

ARGPARSE_INL std::string
_to_upper(
        std::string const& str)
{
    std::pair<bool, std::size_t> num_chars = _utf8_length(str);
    std::string res;
    if (!num_chars.first) {
        res = str;
        for (std::size_t i = 0; i < res.size(); ++i) {
            res.at(i) = static_cast<char>(
                           std::toupper(static_cast<unsigned char>(res.at(i))));
        }
        return res;
    }
    std::size_t i = 0;
    for (std::size_t n = 0; n < num_chars.second; ++n) {
        std::size_t cp_size = _utf8_codepoint_size(_char_to_u8(str.at(i)));
        codepoint cp = 0;
        switch (cp_size) {
            case 1:
                cp =  (_char_to_u32(str.at(i    )) & ~_utf8_1b_mask);
                break;
            case 2:
                cp = ((_char_to_u32(str.at(i    )) & ~_utf8_2b_mask) <<  6)
                   |  (_char_to_u32(str.at(i + 1)) & ~_utf8_ct_mask);
                break;
            case 3:
                cp = ((_char_to_u32(str.at(i    )) & ~_utf8_3b_mask) << 12)
                   | ((_char_to_u32(str.at(i + 1)) & ~_utf8_ct_mask) <<  6)
                   |  (_char_to_u32(str.at(i + 2)) & ~_utf8_ct_mask);
                break;
            case 4:
                cp = ((_char_to_u32(str.at(i    )) & ~_utf8_4b_mask) << 18)
                   | ((_char_to_u32(str.at(i + 1)) & ~_utf8_ct_mask) << 12)
                   | ((_char_to_u32(str.at(i + 2)) & ~_utf8_ct_mask) <<  6)
                   |  (_char_to_u32(str.at(i + 3)) & ~_utf8_ct_mask);
                break;
            default:
                // should never happen
                break;
        }
        i += cp_size;
        cp = _to_upper_codepoint(cp);
        if (cp < 0x80) {
            // one octet
            res += _u32_to_char(cp);
        } else if (cp < 0x800) {
            // two octets
            res += _u32_to_char((cp >> 6)                    | _utf8_2b_bits);
            res += _u32_to_char((cp         & _utf8_ct_invm) | _utf8_ct_bits);
        } else if (cp < 0x10000) {
            // three octets
            res += _u32_to_char((cp >> 12)                   | _utf8_3b_bits);
            res += _u32_to_char(((cp >> 6)  & _utf8_ct_invm) | _utf8_ct_bits);
            res += _u32_to_char((cp         & _utf8_ct_invm) | _utf8_ct_bits);
        } else {
            // four octets
            res += _u32_to_char((cp >> 18)                   | _utf8_4b_bits);
            res += _u32_to_char(((cp >> 12) & _utf8_ct_invm) | _utf8_ct_bits);
            res += _u32_to_char(((cp >>  6) & _utf8_ct_invm) | _utf8_ct_bits);
            res += _u32_to_char((cp         & _utf8_ct_invm) | _utf8_ct_bits);
        }
    }
    return res;
}
// ----------------------------------------------------------------------------

// -- translations support ----------------------------------------------------
// since v1.7.1
ARGPARSE_INL
TranslationPack::TranslationPack()
    : m_data()
{ }

ARGPARSE_INL TranslationPack::iterator
TranslationPack::begin() ARGPARSE_NOEXCEPT
{
    return m_data.begin();
}

ARGPARSE_INL TranslationPack::iterator
TranslationPack::end() ARGPARSE_NOEXCEPT
{
    return m_data.end();
}

ARGPARSE_INL TranslationPack::const_iterator
TranslationPack::begin() const ARGPARSE_NOEXCEPT
{
    return m_data.begin();
}

ARGPARSE_INL TranslationPack::const_iterator
TranslationPack::end() const ARGPARSE_NOEXCEPT
{
    return m_data.end();
}

ARGPARSE_INL TranslationPack::mapped_type&
TranslationPack::operator [](
        key_type const& key)
{
    iterator it = find(key);
    if (it == end()) {
        it = m_data.insert(end(), std::make_pair(key, mapped_type()));
    }
    return it->second;
}

ARGPARSE_INL TranslationPack::mapped_type&
TranslationPack::at(
        key_type const& key)
{
    iterator it = find(key);
    if (it == end()) {
        throw std::out_of_range("argparse: translation at '" + key + "'");
    }
    return it->second;
}

ARGPARSE_INL TranslationPack::mapped_type const&
TranslationPack::at(
        key_type const& key) const
{
    const_iterator it = find(key);
    if (it == end()) {
        throw std::out_of_range("argparse: translation at '" + key + "'");
    }
    return it->second;
}

ARGPARSE_INL TranslationPack::const_iterator
TranslationPack::find(
        key_type const& key) const
{
    const_iterator it = begin();
    for ( ; it != end() && it->first != key; ++it) {
    }
    return it;
}

ARGPARSE_INL TranslationPack::iterator
TranslationPack::find(
        key_type const& key)
{
    iterator it = begin();
    for ( ; it != end() && it->first != key; ++it) {
    }
    return it;
}

ARGPARSE_INL std::string
_tr(TranslationPack const& pack,
        std::string const& lang)
{
    TranslationPack::const_iterator it;
    if (!lang.empty()) {
        it = pack.find(lang);
        if (it != pack.end()) {
            return it->second;
        }
    }
    it = pack.find(std::string());
    return it != pack.end() ? it->second : std::string();
}

ARGPARSE_INL TranslationPack::mapped_type const&
_tr_at(TranslationPack const& map,
        TranslationPack::key_type const& key)
{
    return map.at(key);
}
// ----------------------------------------------------------------------------

ARGPARSE_INL std::pair<std::size_t, std::size_t>
_get_terminal_size(
        bool default_values = false)
{
    std::size_t width  = 80;
    std::size_t height = 24;
    if (default_values) {
        return std::make_pair(width, height);
    }
#ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
# if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = static_cast<std::size_t>
                (csbi.srWindow.Right - csbi.srWindow.Left + 1);
        height = static_cast<std::size_t>
                (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
        if (width < _min_width && width != 0) {
            width = _min_width;
        }
    }
# else  // UNIX
#  if defined(TIOCGSIZE)
    struct ttysize w;
    if (ioctl(STDOUT_FILENO, TIOCGSIZE, &w) >= 0) {
        width = static_cast<std::size_t>(w.ts_cols);
        height = static_cast<std::size_t>(w.ts_lines);
        if (width < _min_width && width != 0) {
            width = _min_width;
        }
    }
#  elif defined(TIOCGWINSZ)
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) >= 0) {
        width = static_cast<std::size_t>(w.ws_col);
        height = static_cast<std::size_t>(w.ws_row);
        if (width < _min_width && width != 0) {
            width = _min_width;
        }
    }
#  endif  // TIOCGSIZE
# endif  // _WIN32
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
    return std::make_pair(width, height);
}

ARGPARSE_INL void
_limit_to_min(
        std::size_t& value,
        std::size_t to) ARGPARSE_NOEXCEPT
{
    if (value < to) {
        value = to;
    }
}

ARGPARSE_INL void
_limit_to_max(
        std::size_t& value,
        std::size_t to) ARGPARSE_NOEXCEPT
{
    if (value > to) {
        value = to;
    }
}

ARGPARSE_INL std::string
_trim_copy(
        std::string const& str)
{
    std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isspace(static_cast<unsigned char>(*it))) {
        ++it;
    }
    std::string::const_reverse_iterator rit = str.rbegin();
    while (rit.base() != it && std::isspace(static_cast<unsigned char>(*rit))) {
        ++rit;
    }
    return std::string(it, rit.base());
}

#ifdef ARGPARSE_HAS_STRING_VIEW
ARGPARSE_INL std::string_view
_trim_sw(
        std::string const& str)
{
    std::string_view in = str;
    auto left = in.begin();
    for ( ; ; ++left) {
        if (left == in.end()) {
            return std::string_view();
        }
        if (!std::isspace(static_cast<unsigned char>(*left))) {
            break;
        }
    }
    auto right = std::prev(in.end(), 1);
    for ( ; right > left
          && std::isspace(static_cast<unsigned char>(*right)); --right) {
    }
    return in.substr(static_cast<std::size_t>(std::distance(in.begin(), left)),
                     static_cast<std::size_t>(1 + std::distance(left, right)));
}
#else
ARGPARSE_INL std::string
_trim_sw(
        std::string const& str)
{
    return _trim_copy(str);
}
#endif  // ARGPARSE_HAS_STRING_VIEW

ARGPARSE_INL std::string
_file_name(
        std::string const& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

ARGPARSE_INL void
_resolve_conflict(
        std::string const& str,
        std::vector<std::string>& values)
{
    std::vector<std::string>::iterator it = values.begin();
    while (it != values.end()) {
        if ((*it) == str) {
            it = values.erase(it);
        } else {
            ++it;
        }
    }
}

ARGPARSE_INL void
_resolve_conflict(
        std::vector<std::string> const& vec,
        std::vector<std::string>& values)
{
    for (std::size_t i = 0; i < vec.size(); ++i) {
        _resolve_conflict(vec.at(i), values);
    }
}

ARGPARSE_INL std::string
_get_punct(
        std::string const& str)
{
    std::string res;
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (std::ispunct(static_cast<unsigned char>(str.at(i)))) {
            res += str.at(i);
        }
    }
    return res;
}

ARGPARSE_INL bool
_have_quotes(
        std::string const& str)
{
    return str.size() > 1 && str.at(0) == str.at(str.size() - 1)
            && (str.at(0) == '\'' || str.at(0) == '\"');
}

ARGPARSE_INL std::string
_remove_quotes(
        std::string const& str)
{
    return _have_quotes(str) ? str.substr(1, str.size() - 2) : str;
}

ARGPARSE_INL std::string
_replace(
        std::string str,
        std::string const& old,
        std::string const& value)
{
    std::string::size_type pos = str.find(old);
    while (pos != std::string::npos) {
        str.replace(pos, old.length(), value);
        pos = str.find(old, pos + value.size());
    }
    return str;
}

ARGPARSE_INL bool
_contains_substr(
        std::string const& str,
        std::string const& substr)
{
    return str.find(substr) != std::string::npos;
}

ARGPARSE_INL bool
_starts_with(
        std::string const& str,
        std::string const& value)
{
    return str.compare(0, value.size(), value) == 0;
}

ARGPARSE_INL bool
_ends_with(
        std::string const& str,
        std::string const& value)
{
    return str.size() >= value.size()
            && 0 == str.compare(str.size() - value.size(), value.size(), value);
}

ARGPARSE_INL std::string
_flag_name(
        std::string const& str)
{
    char prefix = str.at(0);
    std::string::const_iterator it = str.begin();
    for ( ; it != str.end() && *it == prefix; ++it) {
    }
    return str.substr(static_cast<std::size_t>(std::distance(str.begin(), it)),
                      static_cast<std::size_t>(std::distance(it, str.end())));
}

#ifdef ARGPARSE_HAS_STRING_VIEW
ARGPARSE_INL std::string_view
_flag_name(
        std::string_view str)
{
    char prefix = str.front();
    auto it = str.begin();
    for ( ; it != str.end() && *it == prefix; ++it) {
    }
    return str.substr(static_cast<std::size_t>(std::distance(str.begin(), it)),
                      static_cast<std::size_t>(std::distance(it, str.end())));
}
#endif  // ARGPARSE_HAS_STRING_VIEW

ARGPARSE_INL bool
_is_flag_correct(
        std::string const& str,
        bool is_optional)
{
#ifdef ARGPARSE_HAS_STRING_VIEW
    std::string_view trimmed = _trim_sw(str);
#else
    std::string trimmed = _trim_copy(str);
#endif  // ARGPARSE_HAS_STRING_VIEW
    if (trimmed.size() != str.size()) {
        return false;
    }
    if (is_optional) {
        trimmed = _flag_name(trimmed);
    }
    for (std::size_t i = 0; i < trimmed.size(); ++i) {
        if (trimmed.at(i) == '-' || trimmed.at(i) == '_') {
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(trimmed.at(i)))
                || std::ispunct(static_cast<unsigned char>(trimmed.at(i)))) {
            return false;
        }
    }
    return true;
}

ARGPARSE_INL bool
_is_negative_number(
        std::string const& str)
{
    double value;
    std::stringstream ss(str);
    ss >> value;
    return !ss.fail() && ss.eof() && value < 0;
}

ARGPARSE_INL bool
_is_optional(
        std::string const& arg,
        std::string const& prefix_chars,
        bool have_negative_args,
        bool was_pseudo_arg)
{
    return !arg.empty() && _exists(arg.at(0), prefix_chars) && !was_pseudo_arg
            && (have_negative_args || !_is_negative_number(arg));
}

ARGPARSE_INL bool
_not_optional(
        std::string const& arg,
        std::string const& prefix_chars,
        bool have_negative_args,
        bool was_pseudo_arg)
{
    return arg.empty() || !_exists(arg.at(0), prefix_chars) || was_pseudo_arg
            || (!have_negative_args && _is_negative_number(arg));
}

ARGPARSE_INL std::string
_join(std::vector<std::string> const& values,
        std::string const& separator,
        std::string const& quotes)
{
    if (values.empty()) {
        return std::string();
    }
    std::string res = quotes + values.front() + quotes;
    for (std::size_t i = 1; i < values.size(); ++i) {
        res += separator + quotes + values.at(i) + quotes;
    }
    return res;
}

ARGPARSE_INL void
_append_value_to(
        std::string const& value,
        std::string& str,
        std::string const& separator = _spaces)
{
    if (!str.empty()) {
        str += separator;
    }
    str += value;
}

ARGPARSE_INL void
_store_value_to(
        std::string& value,
        std::vector<std::string>& res,
        bool force = false)
{
    if (!value.empty() || force) {
        res.push_back(value);
        value.clear();
    }
}

ARGPARSE_INL std::vector<std::string>
_split(std::string const& str,
        std::string const& sep,
        int32_t maxsplit)
{
    std::vector<std::string> res;
    std::string value;
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str.at(i);
        if (sep.empty()) {
            if (std::isspace(static_cast<unsigned char>(c))
                    && (maxsplit < 0
                        || static_cast<int32_t>(res.size()) < maxsplit)) {
                _store_value_to(value, res, true);
            } else {
                value += c;
            }
        } else {
            if (_exists(c, sep)
                    && (maxsplit < 0
                        || static_cast<int32_t>(res.size()) < maxsplit)) {
                _store_value_to(value, res, true);
            } else {
                value += c;
            }
        }
    }
    res.push_back(value);
    return res;
}

ARGPARSE_INL std::vector<std::string>
_split_lines(
        std::string const& str,
        bool keepends = false)
{
    std::vector<std::string> res;
    std::string value;
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str.at(i);
        if (c == '\n') {
            if (keepends) {
                value += c;
            }
            _store_value_to(value, res, true);
        } else {
            value += c;
        }
    }
    _store_value_to(value, res);
    return res;
}

ARGPARSE_INL std::vector<std::string>
_split_equal(
        std::string const& str,
        std::string const& prefix)
{
    std::string::size_type pos;
    if (_exists(_equal, prefix)) {
        std::string::const_iterator it = str.begin();
        for ( ; it != str.end() && *it == _equal; ++it) {
        }
        pos = str.find(_equal, static_cast<std::string::size_type>(
                           std::distance(str.begin(), it)));
    } else {
        pos = str.find(_equal);
    }
    if (pos != std::string::npos) {
        return _vector(str.substr(0, pos), str.substr(pos + 1));
    } else {
        return _vector(str);
    }
}

ARGPARSE_INL void
_process_quotes(
        std::list<char>& quotes,
        std::string const& value,
        std::string const& str,
        char c,
        std::size_t i)
{
    if (c == '\"' || c == '\'') {
        if (!quotes.empty()
                && quotes.back() == c
                && (i == str.size()
                    || std::isspace(static_cast<unsigned char>(str.at(i)))
                    || std::ispunct(static_cast<unsigned char>(str.at(i))))) {
            quotes.pop_back();
        } else if (value.empty()
                   || std::ispunct(static_cast<unsigned char>(
                                       value.at(value.size() - 1)))) {
            quotes.push_back(c);
        }
    }
}

ARGPARSE_INL bool
_string_to_bool(
        std::string const& str) ARGPARSE_NOEXCEPT
{
    return !str.empty();
}

ARGPARSE_INL std::string
_bool_to_string(
        bool value)
{
    return value ? "true" : "false";
}

ARGPARSE_INL std::string
_bool_to_string(
        std::string const& str)
{
    return _bool_to_string(_string_to_bool(str));
}

ARGPARSE_INL std::string
_vector_to_string(
        std::vector<std::string>::const_iterator begvec,
        std::vector<std::string>::const_iterator endvec,
        std::string const& separator = _spaces,
        std::string const& quotes = std::string(),
        bool replace_space = false,
        std::string const& none = std::string(),
        std::string const& begin = std::string(),
        std::string const& end = std::string())
{
    typedef std::vector<std::string>::const_iterator value_const_iterator;
    std::string res;
    for (value_const_iterator it = begvec; it != endvec; ++it) {
        std::string val = *it;
        if (quotes.empty() && replace_space && !_have_quotes(val)) {
            val = _replace(val, _spaces, "\\ ");
        }
        _append_value_to(quotes + val + quotes, res, separator);
    }
    return begin + (res.empty() ? none : res) + end;
}

ARGPARSE_INL std::string
_vector_to_string(
        std::vector<std::string> const& vec,
        std::string const& separator = _spaces,
        std::string const& quotes = std::string(),
        bool replace_space = false,
        std::string const& none = std::string(),
        std::string const& begin = std::string(),
        std::string const& end = std::string())
{
    return _vector_to_string(vec.begin(), vec.end(), separator, quotes,
                             replace_space, none, begin, end);
}

ARGPARSE_INL std::string
_matrix_to_string(
        std::vector<std::string> const& values,
        std::vector<std::size_t> const& indexes,
        std::string const& separator = _spaces,
        std::string const& quotes = std::string(),
        bool replace_space = false,
        std::string const& none = std::string(),
        std::string const& begin = std::string(),
        std::string const& end = std::string())
{
    std::string res;
    for (std::size_t i = 0; i < indexes.size(); ++i) {
        typedef std::vector<std::string>::difference_type dtype;
        _append_value_to(
                    _vector_to_string(
                        values.begin() + static_cast<dtype>(
                                           i == 0 ? 0 : indexes.at(i - 1)),
                        values.begin() + static_cast<dtype>(indexes.at(i)),
                        separator, quotes,replace_space, none, begin, end),
                    res, separator);
    }
    return begin + (res.empty() ? (begin + end) : res) + end;
}

ARGPARSE_INL std::string
_boolean_option_to_args(
        std::string const& key,
        _Storage::value_type const& args)
{
    if (args.second.empty()) {
        return _bool_to_string(args.first->default_value());
    }
    if (args.second.size() != 1) {
        throw TypeError("got a data-array for argument '" + key + "'");
    }
    return (args.first->action() != argparse::BooleanOptionalAction
            || args.second.front() == args.first->const_value())
           ? _bool_to_string(args.second.front()) : args.second.front();
}

ARGPARSE_INL std::string
_boolean_option_to_string(
        std::string const& key,
        _Storage::value_type const& args,
        std::string const& quotes)
{
    if (args.second.empty()) {
        return args.first->action() == argparse::BooleanOptionalAction
                ? _none : _bool_to_string(args.first->default_value());
    }
    if (args.second.size() != 1) {
        throw TypeError("got a data-array for argument '" + key + "'");
    }
    return args.second.is_default() ? quotes + args.second.front() + quotes
                                    : _bool_to_string(args.second.front());
}

ARGPARSE_INL std::string
_ignore_explicit(
        std::string const& arg,
        std::string const& value)
{
    return "argument " + arg + ": ignored explicit argument '" + value + "'";
}

ARGPARSE_INL void
_eat_ln(std::ostream& os,
        bool& eat_ln,
        std::string const& begin = std::string("\n"))
{
    if (eat_ln) {
        eat_ln = false;
    } else {
        os << "\n" << begin;
    }
}

ARGPARSE_INL void
_format_output_func(
        std::size_t indent,
        std::size_t width,
        std::vector<std::string>& res,
        std::string& value,
        std::string const& str)
{
    std::size_t size = _utf8_length(value).second;
    if (size > indent && size + 1 + _utf8_length(str).second > width) {
        _store_value_to(value, res);
        size = _utf8_length(value).second;
    }
    if (size < indent) {
        value.resize(value.size() + indent - size, _space);
        value += str;
    } else {
        value += _spaces + str;
    }
}

ARGPARSE_INL std::string
_format_output(
        std::string const& head,
        std::string const& body,
        std::size_t interlayer,
        std::size_t indent,
        std::size_t width,
        std::string const& sep = std::string("\n"))
{
    std::vector<std::string> res;
    std::string value = head;
    if (_utf8_length(value).second + interlayer > indent) {
        _store_value_to(value, res);
    }
    std::vector<std::string> split_str = _split_lines(body);
    for (std::size_t i = 0; i < split_str.size(); ++i) {
        std::string const& str = split_str.at(i);
        if (sep == "\n") {
            _format_output_func(indent, width, res, value, str);
        } else if (str.empty()) {
            value.resize(value.size() + indent - _utf8_length(value).second,
                         _space);
            _store_value_to(value, res, true);
        } else {
            std::vector<std::string> sub_split_str = _split(str, sep);
            for (std::size_t j = 0; j < sub_split_str.size(); ++j) {
                std::string const& sub = sub_split_str.at(j);
                _format_output_func(indent, width, res, value, sub);
            }
            _store_value_to(value, res);
        }
    }
    _store_value_to(value, res);
    return _join(res, "\n");
}

ARGPARSE_INL std::string
_help_formatter(
        std::string const& head,
        HelpFormatter const& formatter,
        std::string const& help,
        std::size_t width,
        std::size_t indent)
{
    std::size_t const interlayer = 2;
    std::vector<std::string> res;
    std::string value = head;
    if (_utf8_length(value).second + interlayer > indent) {
        _store_value_to(value, res);
    }
    if (!help.empty()) {
        std::vector<std::string> lines
                = formatter._split_lines(help, width - indent);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            std::size_t const size = _utf8_length(value).second;
            if (size < indent) {
                value.resize(value.size() + indent - size, _space);
            }
            value += lines.at(i);
            _store_value_to(value, res, true);
        }
    }
    _store_value_to(value, res);
    return _join(res, "\n");
}

ARGPARSE_INL void
_print_raw_text_formatter(
        HelpFormatter const& formatter,
        std::string const& text,
        std::size_t width,
        std::ostream& os,
        bool& eat_ln,
        std::string const& begin = std::string("\n"),
        std::size_t indent = 0,
        std::string const& end = std::string())
{
    if (!text.empty()) {
        _eat_ln(os, eat_ln, begin);
        os << formatter._fill_text(text, width, indent) << end;
    }
}

ARGPARSE_INL std::string
_filled_string(
        std::string const& str,
        std::size_t limit,
        char filler = '-')
{
    if (str.size() + 2 >= limit) {
        return _spaces + str;
    }
    std::string res = std::string((limit - str.size() - 2) / 2, filler)
                      + _spaces + str + _spaces;
    res.resize(limit, filler);
    return res;
}

ARGPARSE_INL bool
_is_type_correct(
        std::string const& expected,
        std::string const& received) ARGPARSE_NOEXCEPT
{
    return expected.empty() || received == expected;
}

ARGPARSE_INL void
_check_type(
        Value<std::string> const& expected,
        std::string const& received)
{
    if (expected.has_value() && !_is_type_correct(expected.value(), received)) {
        throw TypeError("type_name mismatch: expected '" + expected.value()
                        + "', received '" + received + "'");
    }
}

ARGPARSE_INL void
_check_non_count_action(
        std::string const& key,
        Action action)
{
    if (action == argparse::count) {
        throw TypeError("got an invalid type for count argument '" + key + "'");
    }
}

ARGPARSE_INL void
_check_argument_type(
        bool is_positional)
{
    if (is_positional) {
        // 'version', 'help' and 'language' actions cannot be positional
        throw TypeError("got an unexpected keyword argument 'action'");
    }
}

ARGPARSE_INL void
_check_flag_name(
        std::string const& flag)
{
    if (flag.empty()) {
        throw IndexError("string index out of range");
    }
}

ARGPARSE_INL void
_update_flag_name_func(
        std::string const& arg,
        std::string& flag,
        std::size_t& count)
{
    std::string name = _flag_name(arg);
    std::size_t count_prefixes = arg.size() - name.size();
    if (count < count_prefixes) {
        count = count_prefixes;
        flag = ARGPARSE_MOVE(name);
    }
}

ARGPARSE_INL void
_update_flag_name(
        std::vector<std::string>& flags,
        std::string const& prefix_chars,
        std::string& flag_name,
        std::size_t& prefixes,
        bool is_optional)
{
    if (is_optional) {
        _update_flag_name_func(flag_name, flag_name, prefixes);
    } else if (flags.size() > 1) {
        // no positional or operand multiflag
        throw ValueError("invalid option string " + flags.front() + ": must "
                         + "starts with a character '" + prefix_chars + "'");
    }
    for (std::size_t i = 1; i < flags.size(); ++i) {
        // check arguments
        std::string& flag = flags.at(i);
        _check_flag_name(flag);
        if (!_exists(flag.at(0), prefix_chars)) {
            // no positional or operand arguments with optional flags
            throw ValueError("invalid option string " + flag + ": must starts "
                             + "with a character '" + prefix_chars + "'");
        }
        _update_flag_name_func(flag, flag_name, prefixes);
    }
}

ARGPARSE_INL pArgument const
_find_arg_by_flag(
        pArguments const& args,
        std::string const& key)
{
    pArguments::const_iterator it = args.begin();
    for ( ; it != args.end() && !_exists(key, (*it)->flags()); ++it) {
    }
    return it != args.end() ? *it : ARGPARSE_NULLPTR;
}

ARGPARSE_INL bool
_is_not_operand(
        bool was_pseudo_arg,
        pArguments const& args,
        std::string const& key)
{
    return was_pseudo_arg || !_find_arg_by_flag(
                args, _split(key, _equals, 1).front());
}

ARGPARSE_INL void
_add_arg_usage(
        std::string& res,
        std::string const& str,
        bool required)
{
    _append_value_to(required ? str : "[" + str + "]", res, "\n");
}

ARGPARSE_INL void
_check_intermixed_subparsers(
        bool intermixed,
        pSubParsers const& subparsers)
{
    if (intermixed && subparsers) {
        throw
        TypeError("parse_intermixed_args: positional arg with nargs=A...");
    }
}

ARGPARSE_INL std::vector<std::string>
_process_split_equal(
        std::string& arg,
        std::string const& prefix_chars)
{
    std::vector<std::string> equals = _split_equal(arg, prefix_chars);
    if (equals.size() == 2 && !equals.front().empty()) {
        arg = equals.front();
    } else {
        equals.resize(1);
    }
    return equals;
}

ARGPARSE_INL bool
_negative_numbers_presented(
        pArguments const& optionals,
        std::string const& prefix_chars)
{
    if (_exists(_prefix_char, prefix_chars)) {
        for (std::size_t i = 0; i < optionals.size(); ++i) {
            pArgument const& arg = optionals.at(i);
            for (std::size_t j = 0; j < arg->flags().size(); ++j) {
                if (_is_negative_number(arg->flags().at(j))) {
                    return true;
                }
            }
        }
    }
    return false;
}

ARGPARSE_INL bool
_process_separate_arg_abbrev(
        std::string const& name,
        std::size_t i,
        std::size_t cp_size,
        std::vector<std::string>& flags,
        std::string const& arg,
        pArguments const& args)
{
    std::string abbrev = name.substr(i, cp_size);
    if (abbrev == _equals) {
        if (flags.empty()) {
            flags.push_back(std::string());
        }
        flags.back() += name.substr(i);
        return false;
    }
    Argument const* argument = ARGPARSE_NULLPTR;
    for (std::size_t j = 0; j < args.size() && !argument; ++j) {
        for (std::size_t k = 0; k < args.at(j)->flags().size(); ++k) {
            std::string const& flag = args.at(j)->flags().at(k);
            if (_utf8_length(flag).second == 2
                    && flag.substr(1) == abbrev
                    && flag.at(0) == arg.at(0)) {
                flags.push_back(flag);
                argument = args.at(j).get();
                break;
            }
        }
    }
    if (!argument && flags.empty()) {
        flags.push_back(arg);
        return false;
    } else if ((!argument && !flags.empty())
               || (argument && (argument->action() & _store_action))) {
        std::string str
                = name.substr(i + (static_cast<bool>(argument) ? cp_size : 0));
        if (!str.empty()) {
            flags.back() += _equals;
            flags.back() += str;
        }
        return false;
    }
    return true;
}
}  // namespace detail

// -- HelpFormatter -----------------------------------------------------------
ARGPARSE_INL std::string
HelpFormatter::_fill_text(
        std::string const& text,
        std::size_t width,
        std::size_t indent) const
{
    std::vector<std::string> res;
    std::string value;
    std::vector<std::string> lines = _split_lines(text, width - indent);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::size_t const size = detail::_utf8_length(value).second;
        if (size < indent) {
            value.resize(value.size() + indent - size, detail::_space);
        }
        value += lines.at(i);
        detail::_store_value_to(value, res, true);
    }
    detail::_store_value_to(value, res);
    return detail::_join(res, "\n");
}

ARGPARSE_INL std::string
HelpFormatter::_get_default_metavar_for_optional(
        Argument const* action) const
{
    return detail::_to_upper(action->get_dest());
}

ARGPARSE_INL std::string
HelpFormatter::_get_default_metavar_for_positional(
        Argument const* action) const
{
    return action->get_dest();
}

ARGPARSE_INL std::string
HelpFormatter::_get_help_string(
        Argument const* action,
        std::string const& lang) const
{
    return detail::_tr(action->m_help.value(), lang);
}

ARGPARSE_INL std::vector<std::string>
HelpFormatter::_split_lines(
        std::string const& text,
        std::size_t width) const
{
    std::string value;
    std::vector<std::string> res;
    std::vector<std::string> split_str = detail::_split(text, "");
    for (std::size_t i = 0; i < split_str.size(); ++i) {
        if (detail::_utf8_length(value).second + 1
                + detail::_utf8_length(split_str.at(i)).second > width) {
            detail::_store_value_to(value, res);
        }
        if (!value.empty() && !split_str.at(i).empty()) {
            value += detail::_spaces;
        }
        value += split_str.at(i);
    }
    detail::_store_value_to(value, res);
    return res;
}

ARGPARSE_INL HelpFormatter::CompletionInfo
HelpFormatter::_bash_completion_info(
        ArgumentParser const* parser) const
{
    CompletionInfo res;
    detail::pArguments optional = parser->m_data->get_optional(false, true);
    detail::pArguments operand = parser->m_data->get_operand(false, true);
    detail::pArguments positional = parser->m_data->get_positional(false, true);
    res.options.reserve(optional.size());
    std::vector<std::string> options;
    for (std::size_t i = 0; i < optional.size(); ++i) {
        pArgument const& arg = optional.at(i);
        detail::_insert_to_end(arg->flags(), options);
        res.options.push_back(std::make_pair(arg, std::string()));
        if (arg->m_nargs != Argument::SUPPRESSING
                && (arg->action()
                    & (detail::_store_action | argparse::language))) {
            if (!arg->choices().empty()) {
                res.options.back().second
                        = " -W \"" + detail::_join(arg->choices()) + "\"";
            } else {
                res.options.back().second = " -df";
            }
        }
    }
    for (std::size_t i = 0; i < operand.size(); ++i) {
        pArgument const& arg = operand.at(i);
        for (std::size_t j = 0; j < arg->flags().size(); ++j) {
            options.push_back(arg->flags().at(j) + "=");
            options.push_back(options.back() + "A");
        }
    }
    bool have_fs_args = false;
    for (std::size_t i = 0; i < positional.size(); ++i) {
        pArgument const& arg = positional.at(i);
        if (!(arg->action() & detail::_store_action)) {
            continue;
        }
        have_fs_args = have_fs_args || (arg->m_nargs != Argument::SUPPRESSING);
    }
    if (parser->has_subparsers()) {
        detail::_insert_to_end(parser->m_subparsers->parser_names(), options);
    }
    if (have_fs_args) {
        res.args += " -df";
    }
    if (!options.empty()) {
        res.args += " -W \"" + detail::_join(options) + "\"";
    }
    return res;
}

ARGPARSE_INL void
HelpFormatter::_print_parser_completion(
        std::ostream& os,
        ArgumentParser const* p,
        std::string const& prog,
        bool is_root) const
{
    typedef detail::shared_ptr<ArgumentParser> pParser;
    typedef std::list<pParser>::const_iterator prs_iterator;
    if (p->has_subparsers()) {
        std::list<pParser> const parsers = p->m_subparsers->list_parsers();
        for (prs_iterator it = parsers.begin(); it != parsers.end(); ++it) {
            _print_parser_completion(
                        os, (*it).get(), prog + "_" + (*it)->m_name, false);
        }
    }
    os << "_" << prog << "()\n";
    os << "{\n";
    if (p->has_subparsers()) {
        if (is_root) {
            os << "  for (( i=1; i < ${COMP_CWORD}; ((++i)) )); do\n";
        } else {
            os << "  for (( i=$1; i < ${COMP_CWORD}; ((++i)) )); do\n";
        }
        os << "    case \"${COMP_WORDS[$i]}\" in\n";
        std::list<pParser> const parsers = p->m_subparsers->list_parsers();
        for (prs_iterator it = parsers.begin(); it != parsers.end(); ++it) {
            std::string name = "\"" + (*it)->m_name + "\"";
            if (!(*it)->aliases().empty()) {
                name += "|" + detail::_join((*it)->aliases(), "|", "\"");
            }
            os << "      " << name << ")\n";
            os << "        _" << prog << "_" << (*it)->m_name << " $((i+1))\n";
            os << "        return;;\n";
        }
        os << "      *);;\n";
        os << "    esac\n";
        os << "  done\n";
    }
    CompletionInfo info = _bash_completion_info(p);
    if (!info.options.empty()) {
        os << "  case \"${COMP_WORDS[${COMP_CWORD}-1]}\" in\n";
        for (std::size_t j = 0; j < info.options.size(); ++j) {
            std::pair<pArgument, std::string> const& a = info.options.at(j);
            os << "    " << detail::_join(a.first->flags(), "|", "\"") << ")\n";
            if (a.first->action() & (detail::_const_action
                                     | detail::_bool_action
                                     | argparse::BooleanOptionalAction)) {
                os << "      ;;\n";
            } else {
                if (!a.second.empty()) {
                    os << "      COMPREPLY=($(compgen" << a.second
                       << " -- \"${COMP_WORDS[${COMP_CWORD}]}\"))\n";
                }
                os << "      return;;\n";
            }
        }
        os << "    *);;\n";
        os << "  esac\n";
    }
    if (!info.args.empty()) {
        os << "  COMPREPLY=($(compgen" << info.args
           << " -- \"${COMP_WORDS[${COMP_CWORD}]}\"))\n";
    }
    os << "  return\n";
    os << "}\n\n";
}

ARGPARSE_INL void
HelpFormatter::_print_custom_usage(
        std::ostream& os,
        ArgumentParser const* p,
        std::string const& usage_title) const
{
    typedef ArgumentParser::mtx_it _mt;
    typedef ArgumentParser::arg_iterator _arg;
    detail::pArguments positional = p->m_data->get_positional(false, true);
    detail::pArguments operand = p->m_data->get_operand(false, true);
    detail::pArguments options = p->m_data->get_optional(false, true);
    ArgumentParser::SubParsersInfo const info = p->subparsers_info(false);
    std::size_t const w = p->output_width();
    std::string head_prog = usage_title + " " + p->prog();
    std::size_t indent = 1 + detail::_utf8_length(
                w > detail::_min_width ? head_prog : usage_title).second;
    std::string res;
    for (_mt i = p->m_mutex_groups.begin(); i != p->m_mutex_groups.end(); ++i) {
        for (_arg j = (*i).m_data->m_arguments.begin();
             j != (*i).m_data->m_arguments.end(); ++j) {
            detail::_erase_remove(options, *j);
            detail::_erase_remove(operand, *j);
        }
    }
    for (std::size_t i = 0; i < options.size(); ++i) {
        detail::_add_arg_usage(res, options.at(i)->usage(*this),
                               options.at(i)->required());
    }
    for (std::size_t i = 0; i < operand.size(); ++i) {
        detail::_add_arg_usage(res, operand.at(i)->usage(*this),
                               operand.at(i)->required());
    }
    for (_mt i = p->m_mutex_groups.begin(); i != p->m_mutex_groups.end(); ++i) {
        detail::_add_arg_usage(res, (*i).usage(*this), true);
    }
    for (std::size_t i = 0; i < positional.size(); ++i) {
        if (info.first && info.second == i && !info.first->is_suppress()) {
            detail::_add_arg_usage(res, info.first->usage(), true);
        }
        std::string const str = positional.at(i)->usage(*this);
        if (str.empty()) {
            continue;
        }
        detail::_add_arg_usage(res, str, true);
    }
    if (info.first && info.second == positional.size()
            && !info.first->is_suppress()) {
        detail::_add_arg_usage(res, info.first->usage(), true);
    }
    os << detail::_format_output(head_prog, res, 1, indent, w);
}

ARGPARSE_INL std::string
HelpFormatter::_format_bash_completion(
        ArgumentParser const* p) const
{
    std::stringstream ss;
    ss << "# bash completion for " << p->prog() << "\n";
    ss << "# generated with cpp-argparse v"
       << ARGPARSE_VERSION_MAJOR << "."
       << ARGPARSE_VERSION_MINOR << "."
       << ARGPARSE_VERSION_PATCH << "\n\n";
    _print_parser_completion(ss, p, p->prog(), true);
    ss << "complete -F _" << p->prog() << " " << p->prog();
    return ss.str();
}

ARGPARSE_INL std::string
HelpFormatter::_format_usage(
        ArgumentParser const* p,
        std::string const& language) const
{
    if (p->m_usage.suppress()) {
        return std::string();
    }
    std::stringstream ss;
    std::string lang = !language.empty() ? language : p->default_language();
    std::string tr_usage_title = detail::_tr(p->m_usage_title, lang) + ":";
    std::string tr_usage = detail::_tr(p->m_usage.value(), lang);
    if (!tr_usage.empty()) {
        ss << tr_usage_title << " " << p->despecify(tr_usage);
    } else {
        _print_custom_usage(ss, p, tr_usage_title);
    }
    return ss.str();
}

ARGPARSE_INL std::string
HelpFormatter::_format_help(
        ArgumentParser const* p,
        std::string const& language) const
{
    typedef std::list<ArgumentParser::pGroup>::const_iterator grp_iterator;
    std::stringstream ss;
    std::string lang = !language.empty() ? language : p->default_language();
    detail::pArguments positional = p->m_data->get_positional(false, false);
    detail::pArguments operand = p->m_data->get_operand(false, false);
    detail::pArguments optional = p->m_data->get_optional(false, false);
    ArgumentParser::SubParsersInfo const sub_info = p->subparsers_info(false);
    bool eat_ln = p->m_usage.suppress();
    if (!eat_ln) {
        ss << _format_usage(p, lang);
    }
    std::size_t const width = p->output_width();
    detail::_print_raw_text_formatter(
                *this, p->despecify(detail::_tr(p->m_description, lang)),
                width, ss, eat_ln);
    std::size_t size = 0;
    detail::pSubParsers subparsers = sub_info.first;
    bool sub_positional = subparsers && subparsers->is_positional();
    for (std::size_t i = 0; i < positional.size(); ++i) {
        std::string str = positional.at(i)->flags_to_string(*this);
        detail::_limit_to_min(size, detail::_utf8_length(str).second);
    }
    for (std::size_t i = 0; i < operand.size(); ++i) {
        std::string str = operand.at(i)->flags_to_string(*this);
        detail::_limit_to_min(size, detail::_utf8_length(str).second);
    }
    for (std::size_t i = 0; i < optional.size(); ++i) {
        std::string str = optional.at(i)->flags_to_string(*this);
        detail::_limit_to_min(size, detail::_utf8_length(str).second);
    }
    for (grp_iterator it = p->m_groups.begin(); it != p->m_groups.end(); ++it) {
        (*it)->limit_help_flags(*this, size);
    }
    size += 4;
    detail::_limit_to_max(size, p->output_width() - p->argument_help_limit());
    if (!positional.empty() || sub_positional) {
        detail::_eat_ln(ss, eat_ln);
        ss << detail::_tr(p->m_positionals_title, lang) << ":";
        for (std::size_t i = 0; i < positional.size(); ++i) {
            p->print_subparsers(sub_positional, sub_info, i,
                                *this, p->prog(), size, width, lang, ss);
            ss << "\n" << p->despecify(positional.at(i)->print(
                                           *this, size, width, lang));
        }
        p->print_subparsers(sub_positional, sub_info, positional.size(),
                            *this, p->prog(), size, width, lang, ss);
    }
    if (!operand.empty()) {
        detail::_eat_ln(ss, eat_ln);
        ss << detail::_tr(p->m_operands_title, lang) << ":";
        for (std::size_t i = 0; i < operand.size(); ++i) {
            ss << "\n" << p->despecify(operand.at(i)->print(
                                           *this, size, width, lang));
        }
    }
    if (!optional.empty()) {
        detail::_eat_ln(ss, eat_ln);
        ss << detail::_tr(p->m_optionals_title, lang) << ":";
        for (std::size_t i = 0; i < optional.size(); ++i) {
            ss << "\n" << p->despecify(optional.at(i)->print(
                                           *this, size, width, lang));
        }
    }
    for (grp_iterator it = p->m_groups.begin(); it != p->m_groups.end(); ++it) {
        if (!subparsers || ((*it) != subparsers || !sub_positional)) {
            (*it)->print_help(ss, eat_ln, *this, p->prog(),
                              false, size, width, lang);
        }
    }
    detail::_print_raw_text_formatter(
                *this, p->despecify(detail::_tr(p->m_epilog, lang)),
                width, ss, eat_ln);
    return ss.str();
}

// -- _RawDescriptionHelpFormatter --------------------------------------------
ARGPARSE_INL std::string
_RawDescriptionHelpFormatter::_fill_text(
        std::string const& text,
        std::size_t width,
        std::size_t indent) const
{
    std::vector<std::string> res;
    std::string value;
    std::vector<std::string> lines = _split_lines_raw(text, width - indent);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::size_t const size = detail::_utf8_length(value).second;
        if (size < indent) {
            value.resize(value.size() + indent - size, detail::_space);
        }
        value += lines.at(i);
        detail::_store_value_to(value, res, true);
    }
    detail::_store_value_to(value, res);
    return detail::_join(res, "\n");
}

ARGPARSE_INL std::vector<std::string>
_RawDescriptionHelpFormatter::_split_lines_raw(
        std::string const& text,
        std::size_t width) const
{
    std::string value;
    std::vector<std::string> res;
    std::vector<std::string> split_str = detail::_split_lines(text);
    for (std::size_t i = 0; i < split_str.size(); ++i) {
        std::string const& str = split_str.at(i);
        if (str.empty()) {
            detail::_store_value_to(value, res, true);
        } else {
            std::vector<std::string> sub_split_str
                    = detail::_split(str, detail::_spaces);
            for (std::size_t j = 0; j < sub_split_str.size(); ++j) {
                if (j != 0) {
                    value += detail::_spaces;
                }
                std::vector<std::string> tab_split_str
                        = detail::_split(sub_split_str.at(j), "\t");
                for (std::size_t k = 0; k < tab_split_str.size(); ++k) {
                    if (k != 0) {
                        std::size_t size = detail::_utf8_length(value).second;
                        std::size_t tbsize = _tab_size() - (size % _tab_size());
                        if (size + 1 + tbsize > width) {
                            detail::_store_value_to(value, res);
                        } else {
                            value += std::string(tbsize, detail::_space);
                        }
                    }
                    std::string sub = tab_split_str.at(k);
                    if (detail::_utf8_length(value).second + 1
                            + detail::_utf8_length(sub).second > width) {
                        detail::_store_value_to(value, res);
                    }
                    value += sub;
                }
            }
            detail::_store_value_to(value, res);
        }
    }
    detail::_store_value_to(value, res);
    return res;
}

// -- _RawTextHelpFormatter ---------------------------------------------------
ARGPARSE_INL std::vector<std::string>
_RawTextHelpFormatter::_split_lines(
        std::string const& text,
        std::size_t width) const
{
    return _RawDescriptionHelpFormatter::_split_lines_raw(text, width);
}

// -- _ArgumentDefaultsHelpFormatter ------------------------------------------
ARGPARSE_INL std::string
_ArgumentDefaultsHelpFormatter::_get_help_string(
        Argument const* action,
        std::string const& lang) const
{
    std::string res = detail::_tr(action->m_help.value(), lang);
    if (!res.empty() && !detail::_contains_substr(res, "%(default)s")) {
        if (((action->m_type & (Argument::Optional | Argument::Operand))
             || (action->m_nargs & (Argument::ZERO_OR_ONE
                                    | Argument::ZERO_OR_MORE)))
                && !(action->action() & (argparse::help | argparse::version))) {
            res += " (default: %(default)s)";
        }
    }
    return res;
}

// -- _MetavarTypeHelpFormatter -----------------------------------------------
ARGPARSE_INL std::string
_MetavarTypeHelpFormatter::_get_default_metavar_for_optional(
        Argument const* action) const
{
    if (!action->type_name().empty()) {
        return action->type_name();
    }
    return HelpFormatter::_get_default_metavar_for_optional(action);
}

ARGPARSE_INL std::string
_MetavarTypeHelpFormatter::_get_default_metavar_for_positional(
        Argument const* action) const
{
    if (!action->type_name().empty()) {
        return action->type_name();
    }
    return HelpFormatter::_get_default_metavar_for_positional(action);
}

// -- standAlone functions ----------------------------------------------------
ARGPARSE_INL std::vector<std::string>
split_to_args(
        std::string const& str,
        std::ostream& err)
{
    std::vector<std::string> res;
    std::string value;
    std::list<char> quotes;
    bool skip = false;
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str.at(i);
        if (!skip && c == '\\') {
            // skip space
            skip = true;
            if (i + 1 == str.size()) {
                value += c;
                break;
            }
            if (str.at(i + 1) != detail::_space) {
                value += c;
            }
            continue;
        }
        if (((c == detail::_space && !skip)
             || (c != detail::_space
                 && std::isspace(static_cast<unsigned char>(c))))
                && quotes.empty()) {
            detail::_store_value_to(value, res);
        } else {
            detail::_process_quotes(quotes, value, str, c, i + 1);
            value += c;
        }
        skip = false;
    }
    detail::_store_value_to(value, res);
    if (!quotes.empty()) {
        err << "argparse error [skip]: possible incorrect string: '"
            << str << "'" << std::endl;
    }
    return res;
}

// -- Argument ----------------------------------------------------------------
ARGPARSE_INL
Argument::Argument(
        std::vector<std::string> const& flags,
        std::string const& name,
        Type type)
    : m_flags(flags),
      m_all_flags(m_flags),
      m_name(name),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(detail::_vector(std::string())),
      m_handle(ARGPARSE_NULLPTR),
      m_factory(ARGPARSE_NULLPTR),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(type),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
    m_required.reset(m_type != Optional);
}

ARGPARSE_INL detail::shared_ptr<Argument>
Argument::make_argument(
        std::vector<std::string> const& flags,
        std::string const& name,
        Type type)
{
    return detail::make_shared<Argument>(Argument(flags, name, type));
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL
Argument::Argument(
        std::vector<std::string>&& flags,
        std::string&& name,
        Type type)
    : m_flags(std::move(flags)),
      m_all_flags(m_flags),
      m_name(std::move(name)),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(std::vector<std::string>{ std::string() }),
      m_handle(nullptr),
      m_factory(nullptr),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(type),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
    m_required.reset(m_type != Optional);
}

ARGPARSE_INL detail::shared_ptr<Argument>
Argument::make_argument(
        std::vector<std::string>&& flags,
        std::string&& name,
        Type type)
{
    return detail::make_shared<Argument>(
                Argument(std::move(flags), std::move(name), type));
}
#else
ARGPARSE_INL
Argument::Argument(
        std::string const& flag)
    : m_flags(detail::_vector(flag)),
      m_all_flags(m_flags),
      m_name(),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(detail::_vector(std::string())),
      m_handle(ARGPARSE_NULLPTR),
      m_factory(ARGPARSE_NULLPTR),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(NoType),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
}

ARGPARSE_INL
Argument::Argument(
        std::string const& flag1,
        std::string const& flag2)
    : m_flags(detail::_vector(flag1, flag2)),
      m_all_flags(m_flags),
      m_name(),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(detail::_vector(std::string())),
      m_handle(ARGPARSE_NULLPTR),
      m_factory(ARGPARSE_NULLPTR),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(NoType),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
}

ARGPARSE_INL
Argument::Argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3)
    : m_flags(detail::_vector(flag1, flag2, flag3)),
      m_all_flags(m_flags),
      m_name(),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(detail::_vector(std::string())),
      m_handle(ARGPARSE_NULLPTR),
      m_factory(ARGPARSE_NULLPTR),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(NoType),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
}

ARGPARSE_INL
Argument::Argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3,
        std::string const& flag4)
    : m_flags(detail::_vector(flag1, flag2, flag3, flag4)),
      m_all_flags(m_flags),
      m_name(),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(detail::_vector(std::string())),
      m_handle(ARGPARSE_NULLPTR),
      m_factory(ARGPARSE_NULLPTR),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(NoType),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
}
#endif  // C++11+

ARGPARSE_INL
Argument::Argument(
        std::vector<std::string> const& flags)
    : m_flags(flags),
      m_all_flags(m_flags),
      m_name(),
      m_num_args(1),
      m_nargs_str("1"),
      m_const(),
      m_default(),
      m_implicit(),
      m_type_name(),
      m_choices(),
      m_help(),
      m_version(),
      m_metavar(),
      m_dest(detail::_vector(std::string())),
      m_handle(ARGPARSE_NULLPTR),
      m_factory(ARGPARSE_NULLPTR),
      m_post_trigger(),
      m_action(argparse::store),
      m_type(NoType),
      m_nargs(NARGS_DEF),
      m_required()
{
    m_help.will_have()[std::string()] = std::string();
}

ARGPARSE_INL
Argument::Argument(
        Argument const& orig)
    : m_flags(orig.m_flags),
      m_all_flags(orig.m_all_flags),
      m_name(orig.m_name),
      m_num_args(orig.m_num_args),
      m_nargs_str(orig.m_nargs_str),
      m_const(orig.m_const),
      m_default(orig.m_default),
      m_implicit(orig.m_implicit),
      m_type_name(orig.m_type_name),
      m_choices(orig.m_choices),
      m_help(orig.m_help),
      m_version(orig.m_version),
      m_metavar(orig.m_metavar),
      m_dest(orig.m_dest),
      m_handle(orig.m_handle),
      m_factory(orig.m_factory),
      m_post_trigger(orig.m_post_trigger),
      m_action(orig.m_action),
      m_type(orig.m_type),
      m_nargs(orig.m_nargs),
      m_required(orig.m_required)
{
}

ARGPARSE_INL Argument&
Argument::operator =(
        Argument const& rhs)
{
    if (this != &rhs) {
        this->m_flags       = rhs.m_flags;
        this->m_all_flags   = rhs.m_all_flags;
        this->m_name        = rhs.m_name;
        this->m_num_args    = rhs.m_num_args;
        this->m_nargs_str   = rhs.m_nargs_str;
        this->m_const       = rhs.m_const;
        this->m_default     = rhs.m_default;
        this->m_implicit    = rhs.m_implicit;
        this->m_type_name   = rhs.m_type_name;
        this->m_choices     = rhs.m_choices;
        this->m_help        = rhs.m_help;
        this->m_version     = rhs.m_version;
        this->m_metavar     = rhs.m_metavar;
        this->m_dest        = rhs.m_dest;
        this->m_handle      = rhs.m_handle;
        this->m_factory     = rhs.m_factory;
        this->m_post_trigger= rhs.m_post_trigger;
        this->m_action      = rhs.m_action;
        this->m_type        = rhs.m_type;
        this->m_nargs       = rhs.m_nargs;
        this->m_required    = rhs.m_required;
    }
    return *this;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL
Argument::Argument(
        Argument&& orig) ARGPARSE_NOEXCEPT
    : m_flags(std::move(orig.m_flags)),
      m_all_flags(std::move(orig.m_all_flags)),
      m_name(std::move(orig.m_name)),
      m_num_args(std::move(orig.m_num_args)),
      m_nargs_str(std::move(orig.m_nargs_str)),
      m_const(std::move(orig.m_const)),
      m_default(std::move(orig.m_default)),
      m_implicit(std::move(orig.m_implicit)),
      m_type_name(std::move(orig.m_type_name)),
      m_choices(std::move(orig.m_choices)),
      m_help(std::move(orig.m_help)),
      m_version(std::move(orig.m_version)),
      m_metavar(std::move(orig.m_metavar)),
      m_dest(std::move(orig.m_dest)),
      m_handle(std::move(orig.m_handle)),
      m_factory(std::move(orig.m_factory)),
      m_post_trigger(std::move(orig.m_post_trigger)),
      m_action(std::move(orig.m_action)),
      m_type(std::move(orig.m_type)),
      m_nargs(std::move(orig.m_nargs)),
      m_required(std::move(orig.m_required))
{
}

ARGPARSE_INL Argument&
Argument::operator =(
        Argument&& rhs) ARGPARSE_NOEXCEPT
{
    if (this != &rhs) {
        this->m_flags       = std::move(rhs.m_flags);
        this->m_all_flags   = std::move(rhs.m_all_flags);
        this->m_name        = std::move(rhs.m_name);
        this->m_num_args    = std::move(rhs.m_num_args);
        this->m_nargs_str   = std::move(rhs.m_nargs_str);
        this->m_const       = std::move(rhs.m_const);
        this->m_default     = std::move(rhs.m_default);
        this->m_implicit    = std::move(rhs.m_implicit);
        this->m_type_name   = std::move(rhs.m_type_name);
        this->m_choices     = std::move(rhs.m_choices);
        this->m_help        = std::move(rhs.m_help);
        this->m_version     = std::move(rhs.m_version);
        this->m_metavar     = std::move(rhs.m_metavar);
        this->m_dest        = std::move(rhs.m_dest);
        this->m_handle      = std::move(rhs.m_handle);
        this->m_factory     = std::move(rhs.m_factory);
        this->m_post_trigger= std::move(rhs.m_post_trigger);
        this->m_action      = std::move(rhs.m_action);
        this->m_type        = std::move(rhs.m_type);
        this->m_nargs       = std::move(rhs.m_nargs);
        this->m_required    = std::move(rhs.m_required);
    }
    return *this;
}
#endif  // C++11+

ARGPARSE_INL Argument&
Argument::action(
        std::string const& value)
{
    if (value == "store") {
        return action(argparse::store);
    } else if (value == "store_const") {
        return action(argparse::store_const);
    } else if (value == "store_true") {
        return action(argparse::store_true);
    } else if (value == "store_false") {
        return action(argparse::store_false);
    } else if (value == "append") {
        return action(argparse::append);
    } else if (value == "append_const") {
        return action(argparse::append_const);
    } else if (value == "count") {
        return action(argparse::count);
    } else if (value == "help") {
        return action(argparse::help);
    } else if (value == "version") {
        return action(argparse::version);
    } else if (value == "extend") {
        return action(argparse::extend);
    } else if (value == "language") {
        return action(argparse::language);
    }
    throw ValueError("unknown action '" + value + "'");
}

ARGPARSE_INL Argument&
Argument::action(
        Action value)
{
    if (m_type == Operand
            && !(value & (argparse::store | argparse::language))) {
        // only store and language actions can be operand
        throw TypeError("got an unexpected keyword argument 'action'");
    }
    prepare_action(value);
    switch (value) {
        case argparse::store_true :
            if (!m_default.has_value()) {
                m_default.reset();
            }
            // fallthrough
        case argparse::BooleanOptionalAction :
            m_const.reset("1");
            m_nargs = NARGS_NUM;
            m_nargs_str = std::string("0");
            m_num_args = 0;
            m_choices.reset();
            break;
        case argparse::store_false :
            if (!m_default.has_value()) {
                m_default.reset("1");
            }
            // fallthrough
        case argparse::store_const :
        case argparse::append_const :
            m_const.reset();
            m_nargs = NARGS_NUM;
            m_nargs_str = std::string("0");
            m_num_args = 0;
            m_choices.reset();
            break;
        case argparse::version :
        case argparse::help :
            detail::_check_argument_type(m_type == Positional);
            if (value == argparse::version) {
                this->help("show program's version number and exit");
            }
            // fallthrough
        case argparse::count :
            m_const.reset();
            m_nargs = NARGS_NUM;
            m_nargs_str = std::string("0");
            m_num_args = 0;
            m_choices.reset();
            break;
        case argparse::store :
        case argparse::append :
        case argparse::extend :
            m_const.reset();
            if (m_num_args == 0) {
                m_nargs = NARGS_DEF;
                m_nargs_str = std::string("1");
                m_num_args = 1;
            }
            break;
        case argparse::language :
            detail::_check_argument_type(m_type == Positional);
            m_const.reset();
            m_nargs = NARGS_DEF;
            m_nargs_str = std::string("1");
            m_num_args = 1;
            break;
        default :
            throw ValueError("unknown action");
    }
    if (action() & (argparse::version | argparse::help)) {
        m_handle = ARGPARSE_NULLPTR;
    }
    if (!(value & detail::_store_const_action)) {
        m_metavar.reset();
    }
    m_action = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::nargs(
        std::size_t num)
{
    if (m_type == Operand) {
        throw TypeError("got an unexpected keyword argument 'nargs'");
    }
    switch (action()) {
        case argparse::store_const :
        case argparse::store_true :
        case argparse::store_false :
        case argparse::append_const :
        case argparse::count :
        case argparse::help :
        case argparse::version :
        case argparse::language :
        case argparse::BooleanOptionalAction :
            throw TypeError("got an unexpected keyword argument 'nargs'");
        case argparse::store :
            if (num == 0) {
                throw
                ValueError("nargs for store actions must be != 0; if you "
                           "have nothing to store, actions such as store "
                           "true or store const may be more appropriate");
            }
            break;
        case argparse::append :
        case argparse::extend :
            if (num == 0) {
                throw
                ValueError("nargs for append actions must be != 0; if arg "
                           "strings are not supplying the value to append, "
                           "the append const action may be more appropriate");
            }
            break;
        default:
            throw ValueError("unknown action");
    }
    m_nargs = NARGS_NUM;
    m_nargs_str = detail::_to_string(num);
    m_num_args = num;
    return *this;
}

ARGPARSE_INL Argument&
Argument::nargs(
        std::string const& value,
        std::size_t num)
{
    if (!(action() & detail::_store_action) || m_type == Operand || num == 0) {
        throw TypeError("got an unexpected keyword argument 'nargs'");
    }
    if (value == "?") {
        m_nargs = ZERO_OR_ONE;
    } else if (value == "*") {
        m_nargs = ZERO_OR_MORE;
    } else if (value == "+") {
        m_nargs = ONE_OR_MORE;
    } else {
        throw ValueError("invalid nargs value '" + value + "'");
    }
    m_nargs_str = value;
    m_num_args = num;
    return *this;
}

ARGPARSE_INL Argument&
Argument::nargs(
        _REMAINDER)
{
    if (!(action() & detail::_store_action) || m_type == Operand) {
        throw TypeError("got an unexpected keyword argument 'nargs'");
    }
    m_nargs = REMAINDING;
    m_nargs_str = std::string("0");
    m_num_args = 0;
    return *this;
}

ARGPARSE_INL Argument&
Argument::nargs(
        _SUPPRESS)
{
    if (!(action() & detail::_store_action) || m_type == Operand) {
        throw TypeError("got an unexpected keyword argument 'nargs'");
    }
    m_nargs = SUPPRESSING;
    m_nargs_str = std::string("0");
    m_num_args = 0;
    return *this;
}

ARGPARSE_INL Argument&
Argument::const_value(
        std::string const& value)
{
    if ((action() & detail::_const_action)
            || (m_nargs == ZERO_OR_ONE && (action() & detail::_store_action))) {
        m_const = value;
    } else if (m_type == Optional && m_nargs != ZERO_OR_ONE
               && (action() & detail::_store_action)) {
        throw ValueError("nargs must be \"?\" to supply const");
    } else {
        throw TypeError("got an unexpected keyword argument 'const'");
    }
    return *this;
}

ARGPARSE_INL Argument&
Argument::default_value(
        std::string const& value)
{
    m_default = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::default_value(
        _SUPPRESS value)
{
    m_default = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::implicit_value(
        std::string const& value)
{
    m_implicit = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::type(
        std::string const& value)
{
    m_type_name.reset(value);
    return *this;
}

ARGPARSE_INL Argument&
Argument::type(
        detail::func2<std::string const&, void*>::type func) ARGPARSE_NOEXCEPT
{
    m_factory = ARGPARSE_MOVE(func);
    return *this;
}

ARGPARSE_INL Argument&
Argument::choice(
        std::string const& value)
{
    if (!(action() & (detail::_store_action | argparse::language))) {
        throw TypeError("got an unexpected keyword argument 'choices'");
    }
    std::vector<std::string> values;
    values.push_back(value);
    m_choices = ARGPARSE_MOVE(values);
    return *this;
}

ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value)
{
    if (!(action() & (detail::_store_action | argparse::language))) {
        throw TypeError("got an unexpected keyword argument 'choices'");
    }
    std::vector<std::string> values;
    values.reserve(value.size());
    for (std::size_t i = 0; i < value.size(); ++i) {
        values.push_back(std::string(1, value.at(i)));
    }
    m_choices = ARGPARSE_MOVE(values);
    return *this;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL Argument&
Argument::choices(
        std::initializer_list<std::string> const& value)
{
    return choices(std::vector<std::string>{ value });
}
#else
ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value1,
        std::string const& value2)
{
    return choices(detail::_vector(value1, value2));
}

ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3)
{
    return choices(detail::_vector(value1, value2, value3));
}

ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3,
        std::string const& value4)
{
    return choices(detail::_vector(value1, value2, value3, value4));
}
#endif  // C++11+

ARGPARSE_INL Argument&
Argument::choices(
        std::vector<std::string> const& value)
{
    if (!(action() & (detail::_store_action | argparse::language))) {
        throw TypeError("got an unexpected keyword argument 'choices'");
    }
    m_choices = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::required(
        bool value)
{
    if (m_type == Positional) {
        throw TypeError("'required' is an invalid argument for positionals");
    }
    m_required = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::help(
        std::string const& value,
        std::string const& lang)
{
    m_help.will_have()[lang] = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::help(
        _SUPPRESS value)
{
    m_help = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::version(
        std::string const& value)
{
    if (action() != argparse::version) {
        throw TypeError("got an unexpected keyword argument 'version'");
    }
    m_version = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::metavar(
        std::vector<std::string> const& value)
{
    if (m_type == Operand && value.size() != 1) {
        throw TypeError("got an invalid keyword argument 'metavar'");
    }
    if (!(action() & (detail::_store_const_action
                      | argparse::language
                      | argparse::BooleanOptionalAction))) {
        throw TypeError("got an unexpected keyword argument 'metavar'");
    }
    m_metavar = value;
    return *this;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL Argument&
Argument::metavar(
        std::initializer_list<std::string> const& value)
{
    return metavar(std::vector<std::string>{ value });
}
#else
ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value)
{
    return metavar(detail::_vector(value));
}

ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value1,
        std::string const& value2)
{
    return metavar(detail::_vector(value1, value2));
}

ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3)
{
    return metavar(detail::_vector(value1, value2, value3));
}

ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3,
        std::string const& value4)
{
    return metavar(detail::_vector(value1, value2, value3, value4));
}
#endif  // C++11+

ARGPARSE_INL Argument&
Argument::dest(
        std::string const& value)
{
    if (m_type == Positional && !m_flags.empty()) {
        throw ValueError("dest supplied twice for positional argument");
    }
    m_dest.front() = value;
    return *this;
}

ARGPARSE_INL Argument&
Argument::handle(
        detail::func1<std::string const&>::type func) ARGPARSE_NOEXCEPT
{
    m_handle = ARGPARSE_MOVE(func);
    return *this;
}

ARGPARSE_INL std::vector<std::string> const&
Argument::flags() const ARGPARSE_NOEXCEPT
{
    return m_all_flags;
}

ARGPARSE_INL Action
Argument::action() const ARGPARSE_NOEXCEPT
{
    return static_cast<Action>(m_action);
}

ARGPARSE_INL std::string const&
Argument::nargs() const ARGPARSE_NOEXCEPT
{
    return m_nargs_str;
}

ARGPARSE_INL std::string const&
Argument::const_value() const ARGPARSE_NOEXCEPT
{
    return m_const.value();
}

ARGPARSE_INL std::string const&
Argument::default_value() const ARGPARSE_NOEXCEPT
{
    return m_default.value();
}

ARGPARSE_INL std::string const&
Argument::implicit_value() const ARGPARSE_NOEXCEPT
{
    return m_implicit.value();
}

ARGPARSE_INL std::string const&
Argument::type_name() const ARGPARSE_NOEXCEPT
{
    return m_type_name.value();
}

ARGPARSE_INL std::vector<std::string> const&
Argument::choices() const ARGPARSE_NOEXCEPT
{
    return m_choices.value();
}

ARGPARSE_INL bool
Argument::required() const ARGPARSE_NOEXCEPT
{
    return m_required.value();
}

ARGPARSE_INL std::string const&
Argument::help() const
{
    return detail::_tr_at(m_help.value(), std::string());
}

ARGPARSE_INL std::string const&
Argument::version() const ARGPARSE_NOEXCEPT
{
    return m_version.value();
}

ARGPARSE_INL std::string
Argument::metavar() const
{
    std::string res = detail::_join(m_metavar.value(), ", ");
    return m_metavar.value().size() > 1 ? ("(" + res + ")") : res;
}

ARGPARSE_INL std::string const&
Argument::dest() const ARGPARSE_NOEXCEPT
{
    return m_dest.front();
}

ARGPARSE_INL void
Argument::handle(
        std::string const& str) const
{
    if (m_handle) {
        m_handle(detail::_remove_quotes(str));
    }
}

ARGPARSE_INL void
Argument::validate() const
{
    if (!dest().empty()) {
        return;
    }
    if (m_type == Positional && m_flags.empty()) {
        throw TypeError("missing 1 required positional argument: 'dest'");
    }
    for (std::size_t i = 0; i < m_flags.size(); ++i) {
        if (m_flags.at(i) == detail::_pseudo_arg) {
            throw ValueError("dest= is required for options like '--'");
        }
    }
}

ARGPARSE_INL void
Argument::prepare_action(
        Action value)
{
    if (m_type == Optional && value == argparse::BooleanOptionalAction) {
        make_no_flags();
        if (detail::shared_ptr<_ArgumentData> ptr = m_post_trigger.lock()) {
            ptr->check_conflict_arg(this);
        }
    } else {
        m_all_flags = m_flags;
    }
}

ARGPARSE_INL void
Argument::make_no_flags()
{
    m_all_flags.clear();
    for (std::size_t i = 0; i < m_flags.size(); ++i) {
        std::string const& flag = m_flags.at(i);
        m_all_flags.push_back(flag);
        char prefix = flag.at(0);
        std::string::const_iterator it = flag.begin();
        for ( ; it != flag.end() && *it == prefix; ++it) {
        }
        if (std::distance(flag.begin(), it) > 1) {
            m_all_flags.push_back(flag);
            m_all_flags.back().insert(static_cast<std::size_t>(2), "no-");
        }
    }
}

ARGPARSE_INL std::string
Argument::usage(
        HelpFormatter const& formatter) const
{
    std::string res;
    if (m_type == Optional) {
        res = action() == argparse::BooleanOptionalAction
                ? detail::_join(flags(), " | ") : m_flags.front();
    } else if (m_type == Operand) {
        res = m_flags.front();
    }
    process_nargs_suffix(res, formatter);
    return res;
}

ARGPARSE_INL std::string
Argument::flags_to_string(
        HelpFormatter const& formatter) const
{
    std::string res;
    if (m_type & (Optional | Operand)) {
        for (std::size_t i = 0; i < flags().size(); ++i) {
            detail::_append_value_to(flags().at(i), res, ", ");
            process_nargs_suffix(res, formatter);
        }
    } else {
        std::vector<std::string> names = get_argument_name(formatter);
        if (names.size() != 1) {
            throw ValueError("too many values to unpack (expected 1)");
        }
        res += names.front();
    }
    return res;
}

ARGPARSE_INL std::string
Argument::get_choices() const
{
    return m_choices.has_value()
            ? detail::_join(choices(), ", ") : detail::_none;
}

ARGPARSE_INL std::string
Argument::get_const() const
{
    return m_const.value_or(detail::_none);
}

ARGPARSE_INL std::string
Argument::get_default() const
{
    if (!m_default.has_value() && (action() & detail::_bool_action)) {
        return detail::_bool_to_string(m_default.value());
    } else {
        return (m_default.has_value() || !m_default.value().empty())
                ? m_default.value() : detail::_none;
    }
}

ARGPARSE_INL std::string const&
Argument::get_dest() const ARGPARSE_NOEXCEPT
{
    return dest().empty() ? m_name : dest();
}

ARGPARSE_INL std::string
Argument::get_metavar() const
{
    return m_metavar.has_value() ? metavar() : detail::_none;
}

ARGPARSE_INL std::string
Argument::get_nargs() const
{
    switch (m_nargs) {
        case NARGS_DEF :
            return detail::_none;
        case NARGS_NUM :
        case ONE_OR_MORE :
        case ZERO_OR_ONE :
        case ZERO_OR_MORE :
            return m_nargs_str;
        case REMAINDING :
            return "...";
        case SUPPRESSING :
            return "";
        default :
            return "Undefined";
    }
}

ARGPARSE_INL std::string
Argument::option_strings() const
{
    return detail::_vector_to_string(flags(), ", ", "'", false, "", "[", "]");
}

ARGPARSE_INL std::string
Argument::get_required() const
{
    return detail::_bool_to_string(required());
}

ARGPARSE_INL std::string
Argument::get_type() const
{
    return m_type_name.value_or(detail::_none);
}

ARGPARSE_INL std::string
Argument::print(
        HelpFormatter const& formatter,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    std::string help = formatter._get_help_string(this, lang);
    std::string res = help;
    std::string text;
#ifdef ARGPARSE_CXX_11
    std::regex const r("%[(]([a-z_]*)[)]s");
    std::smatch match;
    std::unordered_map<std::string, std::function<std::string()> > const
            specifiers =
    {
        { "%(choices)s",        [this]() { return get_choices();    } },
        { "%(const)s",          [this]() { return get_const();      } },
        { "%(default)s",        [this]() { return get_default();    } },
        { "%(dest)s",
            [this] () -> std::string const& { return get_dest();    } },
        { "%(help)s",
            [&help]() -> std::string const& { return help;          } },
        { "%(metavar)s",        [this]() { return get_metavar();    } },
        { "%(nargs)s",          [this]() { return get_nargs();      } },
        { "%(option_strings)s", [this]() { return option_strings(); } },
        { "%(required)s",       [this]() { return get_required();   } },
        { "%(type)s",           [this]() { return get_type();       } },
    };
    while (std::regex_search(res, match, r)) {
        text += match.prefix();
        auto specifier = std::string(match[0]);
        auto it = specifiers.find(specifier);
        text += (it != specifiers.end() ? it->second() : std::move(specifier));
        res = match.suffix();
    }
#else
    std::string const beg = "%(";
    std::string const end = ")s";
    std::string::size_type pos = res.find(beg);
    while (pos != std::string::npos) {
        std::string::size_type next = res.find(end, pos + beg.size());
        if (next == std::string::npos) {
            break;
        }
        text += res.substr(0, pos);
        std::string specifier = res.substr(pos, next + end.size() - pos);
        if (specifier == "%(choices)s") {
            text += get_choices();
        } else if (specifier == "%(const)s") {
            text += get_const();
        } else if (specifier == "%(default)s") {
            text += get_default();
        } else if (specifier == "%(dest)s") {
            text += get_dest();
        } else if (specifier == "%(metavar)s") {
            text += get_metavar();
        } else if (specifier == "%(nargs)s") {
            text += get_nargs();
        } else if (specifier == "%(option_strings)s") {
            text += option_strings();
        } else if (specifier == "%(required)s") {
            text += get_required();
        } else if (specifier == "%(type)s") {
            text += get_type();
        } else if (specifier == "%(help)s") {
            text += help;
        } else {
            text += specifier;
        }
        res = res.substr(next + end.size());
        pos = res.find(beg);
    }
#endif  // C++11+
    text += res;
    std::swap(res, text);
    return detail::_help_formatter("  " + flags_to_string(formatter),
                                   formatter, res, width, limit);
}

ARGPARSE_INL void
Argument::process_nargs_suffix(
        std::string& res,
        HelpFormatter const& formatter) const
{
    if (!(action() & (detail::_store_action
                      | argparse::append_const
                      | argparse::language))) {
        return;
    }
    std::vector<std::string> names = get_argument_name(formatter);
    bool nargs_check = (m_nargs & _NARGS_COMBINED);
    if (names.size() > 1 && (!nargs_check || names.size() != m_num_args)) {
        throw TypeError("length of metavar tuple does not match nargs");
    }
    if (names.size() == 1 && m_num_args > names.size() && nargs_check) {
        names.resize(m_num_args, names.front());
    }
    std::string const name = detail::_join(names);
    if (m_type == Optional && !name.empty() && m_nargs != SUPPRESSING) {
        res += detail::_spaces;
    }
    if (m_type == Operand && !name.empty()) {
        res += detail::_equal;
    }
    switch (m_nargs) {
        case NARGS_DEF :
        case NARGS_NUM :
            res += name;
            break;
        case ZERO_OR_ONE :
            res += "[" + name + "]";
            break;
        case ONE_OR_MORE :
            res += name + detail::_spaces;
            // fallthrough
        case ZERO_OR_MORE :
            res += "[" + name + " ...]";
            break;
        case REMAINDING :
            res += "...";
            break;
        case SUPPRESSING :
        default :
            break;
    }
}

ARGPARSE_INL std::vector<std::string>
Argument::get_argument_name(
        HelpFormatter const& formatter) const
{
    if (m_metavar.has_value()) {
        return m_metavar.value();
    }
    if (m_choices.has_value()) {
        return detail::_vector("{" + detail::_join(choices(), ",") + "}");
    }
    return detail::_vector(
                m_type == Positional
                  ? formatter._get_default_metavar_for_positional(this)
                  : formatter._get_default_metavar_for_optional(this));
}

ARGPARSE_INL std::vector<std::string> const&
Argument::get_argument_flags() const ARGPARSE_NOEXCEPT
{
    return dest().empty() ? m_flags : m_dest;
}

ARGPARSE_INL void
Argument::resolve_conflict_flags(
        std::vector<std::string> const& flags)
{
    detail::_resolve_conflict(flags, m_flags);
    detail::_resolve_conflict(flags, m_all_flags);
}

ARGPARSE_INL std::string
Argument::error_nargs(
        std::string const& arg) const
{
    switch (m_nargs) {
        case NARGS_DEF :
            return "argument " + arg + ": expected one argument";
        case NARGS_NUM :
            return "argument " + arg + ": expected " + nargs() + " arguments";
        case ONE_OR_MORE :
            if (m_num_args > 1) {
                return "argument " + arg + ": expected at least "
                        + detail::_to_string(m_num_args) + "x arguments";
            }
            return "argument " + arg + ": expected at least one argument";
        case ZERO_OR_ONE :
            return "argument " + arg + ": expected 0 or "
                    + detail::_to_string(m_num_args) + " arguments";
        case ZERO_OR_MORE :
            if (m_num_args > 1) {
                return "argument " + arg + ": expected 0 or at least "
                        + detail::_to_string(m_num_args) + "x arguments";
            }
            // fallthrough
        case REMAINDING :
        case SUPPRESSING :
        default :
            return std::string();
    }
}

ARGPARSE_INL bool
Argument::is_match_name(
        std::string const& value) const
{
    if (!dest().empty()) {
        return dest() == value;
    }
    switch (m_type) {
        case Positional :
            return m_name == value;
        case Operand :
            for (std::size_t j = 0; j < m_flags.size(); ++j) {
                if (m_flags.at(j) == value || (m_flags.at(j) + "=" == value)) {
                    return true;
                }
            }
            return false;
        case Optional :
            for (std::size_t j = 0; j < m_flags.size(); ++j) {
                if (m_flags.at(j) == value || m_name == value
                        || detail::_flag_name(m_flags.at(j)) == value) {
                    return true;
                }
            }
            return false;
        default :
            return false;
    }
}

ARGPARSE_INL bool
Argument::operator ==(
        Argument const& rhs) const
{
    return this == &rhs;
}

ARGPARSE_INL bool
Argument::operator ==(
        std::string const& rhs) const
{
    return !dest().empty() ? dest() == rhs : detail::_exists(rhs, m_flags);
}

// -- _Group ------------------------------------------------------------------
ARGPARSE_INL
_Group::_Group(
        std::string const& title,
        std::string const& description)
    : m_title(),
      m_description()
{
    m_title[std::string()] = title;
    m_description[std::string()] = description;
}

ARGPARSE_INL std::string const&
_Group::title() const
{
    return detail::_tr_at(m_title, std::string());
}

ARGPARSE_INL std::string const&
_Group::description() const
{
    return detail::_tr_at(m_description, std::string());
}

// -- _ArgumentData -----------------------------------------------------------
ARGPARSE_INL
_ArgumentData::_ArgumentData()
    : m_conflict_handler_str("error"),
      m_arguments(),
      m_optional(),
      m_operand(),
      m_positional(),
      m_conflict_handler(detail::_conflict_error),
      m_add_help(false),
      m_help_added(false)
{
}

ARGPARSE_INL detail::shared_ptr<_ArgumentData>
_ArgumentData::make_argument_data()
{
    return detail::make_shared<_ArgumentData>(_ArgumentData());
}

ARGPARSE_INL void
_ArgumentData::check_conflict_arg(
        Argument const* arg)
{
    for (sub_iterator it = m_optional.begin(); it != m_optional.end(); ++it) {
        if ((*it).first.get() == arg) {
            continue;
        }
        check_conflicting_option(arg, (*it).first->m_all_flags);
        check_conflicting_option(arg, (*it).first->m_flags);
    }
}

ARGPARSE_INL void
_ArgumentData::update_help(
        bool add_help,
        std::string const& prefix_chars)
{
    m_add_help = add_help;
    if (m_help_added) {
        m_arguments.pop_front();
        m_optional.pop_front();
        m_help_added = false;
    }
    if (m_add_help) {
        char prefix = detail::_exists(detail::_prefix_char, prefix_chars)
                ? detail::_prefix_char : prefix_chars.at(0);
        std::vector<std::string> help_flags
                = detail::_vector(std::string(1, prefix) + "h",
                                  std::string(2, prefix) + "help");
        if (m_conflict_handler == detail::_conflict_resolve) {
            for (sub_iterator it = m_optional.begin();
                 it != m_optional.end(); ++it) {
                detail::_resolve_conflict((*it).first->flags(), help_flags);
            }
        }
        if (!help_flags.empty()) {
            pArgument arg = Argument::make_argument(
                        ARGPARSE_MOVE(help_flags), "help", Argument::Optional);
            arg->help("show this help message and exit").action(argparse::help);
            m_arguments.push_front(arg);
            m_optional.push_front(std::make_pair(m_arguments.front(), false));
            m_help_added = true;
        }
    }
}

ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_arguments(
        bool add_suppress) const
{
    std::vector<pArgument> res;
    res.reserve(m_arguments.size());
    for (arg_iterator it = m_arguments.begin(); it != m_arguments.end(); ++it) {
        if ((add_suppress || !(*it)->m_help.suppress())
                && ((*it)->m_type != Argument::Optional
                    || !(*it)->flags().empty())) {
            res.push_back(*it);
        }
    }
    return res;
}

ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_optional(
        bool add_suppress,
        bool add_group) const
{
    std::vector<pArgument> res;
    res.reserve(m_optional.size());
    for (sub_iterator it = m_optional.begin(); it != m_optional.end(); ++it) {
        if ((add_suppress || !(*it).first->m_help.suppress())
                && (add_group || !(*it).second)
                && !(*it).first->flags().empty()) {
            res.push_back((*it).first);
        }
    }
    return res;
}

ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_operand(
        bool add_suppress,
        bool add_group) const
{
    std::vector<pArgument> res;
    res.reserve(m_operand.size());
    for (sub_iterator it = m_operand.begin(); it != m_operand.end(); ++it) {
        if ((add_suppress || !(*it).first->m_help.suppress())
                && (add_group || !(*it).second)) {
            res.push_back((*it).first);
        }
    }
    return res;
}

ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_positional(
        bool add_suppress,
        bool add_group) const
{
    std::vector<pArgument> res;
    res.reserve(m_positional.size());
    for (sub_iterator i = m_positional.begin(); i != m_positional.end(); ++i) {
        if ((add_suppress || !(*i).first->m_help.suppress())
                && (add_group || !(*i).second)) {
            res.push_back((*i).first);
        }
    }
    return res;
}

ARGPARSE_INL void
_ArgumentData::check_conflicting_option(
        Argument const* arg,
        std::vector<std::string>& flags) const
{
    if (m_conflict_handler == detail::_conflict_resolve) {
        detail::_resolve_conflict(arg->flags(), flags);
    } else {
        std::vector<std::string> conflict_options;
        for (std::size_t i = 0; i < arg->flags().size(); ++i) {
            std::string const& flag = arg->flags().at(i);
            if (detail::_exists(flag, flags)) {
                conflict_options.push_back(flag);
            }
        }
        if (conflict_options.size() == 1) {
            throw ArgumentError("argument " + detail::_join(arg->flags(), "/")
                                + ": conflicting option string: "
                                + conflict_options.front());
        } else if (conflict_options.size() > 1) {
            throw ArgumentError("argument " + detail::_join(arg->flags(), "/")
                                + ": conflicting option strings: "
                                + detail::_join(conflict_options, ", "));
        }
    }
}

ARGPARSE_INL void
_ArgumentData::merge_arguments(
        _ArgumentData const& d)
{
    for (sub_iterator i = d.m_optional.begin(); i != d.m_optional.end(); ++i) {
        check_conflict_arg((*i).first.get());
        m_optional.push_back(*i);
    }
    detail::_insert_to_end(d.m_operand, m_operand);
    detail::_insert_to_end(d.m_positional, m_positional);
    detail::_insert_to_end(d.m_arguments, m_arguments);
}

ARGPARSE_INL void
_ArgumentData::create_argument(
        detail::shared_ptr<_ArgumentData>& data,
        std::vector<std::string> const& in_flags,
        std::string const& prefix_chars)
{
    std::vector<std::string> flags = in_flags;
    if (flags.empty()) {
        data->m_arguments.push_back(
                    Argument::make_argument(
                        flags, std::string(), Argument::Positional));
        return;
    }
    flags.front() = detail::_trim_copy(flags.front());
    std::string flag = flags.front();
    detail::_check_flag_name(flag);
    std::size_t prefixes = 0;
    Argument::Type type = Argument::Positional;
    if (detail::_exists(flag.at(0), prefix_chars)) {
        type = Argument::Optional;
    } else if (detail::_ends_with(flag, detail::_equals)) {
        flags.front().resize(flags.front().size() - 1);
        flag = flags.front();
        detail::_check_flag_name(flag);
        type = Argument::Operand;
    }
    detail::_update_flag_name(flags, prefix_chars, flag, prefixes,
                              type == Argument::Optional);
    if (type == Argument::Optional) {
        flag = detail::_replace(flag, "-", "_");
    }
    pArgument arg = Argument::make_argument(
                ARGPARSE_MOVE(flags), ARGPARSE_MOVE(flag), type);
    if (type == Argument::Optional) {
        data->check_conflict_arg(arg.get());
    }
    data->m_arguments.push_back(arg);
    if (type == Argument::Optional) {
        data->m_arguments.back()->m_post_trigger = data;
    }
}

ARGPARSE_INL void
_ArgumentData::validate_argument(
        Argument const& argument,
        std::string const& prefix_chars)
{
    Argument arg = argument;
    std::vector<std::string>& flags = arg.m_flags;
    arg.m_type = Argument::Positional;
    if (flags.empty()) {
        arg.m_name = arg.dest();
    } else {
        std::string flag = flags.front();
        detail::_check_flag_name(flag);
        std::size_t prefixes = 0;
        if (detail::_exists(flag.at(0), prefix_chars)) {
            arg.m_type = Argument::Optional;
        } else if (detail::_ends_with(flag, detail::_equals)) {
            flags.front().resize(flags.front().size() - 1);
            flag = flags.front();
            detail::_check_flag_name(flag);
            arg.m_type = Argument::Operand;
            arg.m_all_flags = arg.m_flags;
            if (!arg.m_required.has_value()) {
                arg.m_required.reset(true);
            }
        }
        detail::_update_flag_name(flags, prefix_chars, flag, prefixes,
                                  arg.m_type == Argument::Optional);
        arg.m_name = arg.m_type == Argument::Optional
                ? detail::_replace(ARGPARSE_MOVE(flag), "-", "_")
                : ARGPARSE_MOVE(flag);
    }
    // check
    if (arg.m_type == Argument::Positional) {
        if (arg.dest().empty() && flags.empty()) {
            throw TypeError("missing 1 required positional argument: 'dest'");
        }
        if (arg.action()
              & (argparse::version | argparse::help | argparse::language)) {
            // 'version', 'help' and 'language' actions cannot be positional
            throw TypeError("got an unexpected keyword argument 'action'");
        }
        if (arg.m_required.has_value()) {
            throw
            TypeError("'required' is an invalid argument for positionals");
        }
        if (!arg.dest().empty() && !flags.empty()) {
            throw ValueError("dest supplied twice for positional argument");
        }
        if (arg.m_const.has_value()
                && !(arg.m_nargs == Argument::ZERO_OR_ONE
                     && (arg.action() & detail::_store_action))
                && !(arg.action() & detail::_const_action)) {
            throw TypeError("got an unexpected keyword argument 'const'");
        }
    } else if (arg.m_type == Argument::Operand) {
        if (!(arg.action() & (argparse::store | argparse::language))) {
            // only store and language actions can be operand
            throw TypeError("got an unexpected keyword argument 'action'");
        }
        if (arg.m_nargs != Argument::NARGS_DEF) {
            throw TypeError("got an unexpected keyword argument 'nargs'");
        }
        if (arg.m_metavar.has_value() && arg.m_metavar.value().size() != 1) {
            throw ValueError("got an invalid keyword argument 'metavar'");
        }
    } else if (arg.m_type == Argument::Optional) {
        if (arg.action() == argparse::BooleanOptionalAction) {
            arg.make_no_flags();
        }
        check_conflict_arg(&arg);
    }
    m_arguments.push_back(detail::make_shared<Argument>(arg));
}

// -- _ArgumentGroup ----------------------------------------------------------
ARGPARSE_INL
_ArgumentGroup::_ArgumentGroup(
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::SValue<std::string>& argument_default,
        bool is_mutex_group)
    : m_data(_ArgumentData::make_argument_data()),
      m_prefix_chars(prefix_chars),
      m_parent_data(parent_data),
      m_argument_default(argument_default),
      m_is_mutex_group(is_mutex_group)
{
}

ARGPARSE_INL
_ArgumentGroup::_ArgumentGroup(
        _ArgumentGroup const& orig)
    : m_data(orig.m_data),
      m_prefix_chars(orig.m_prefix_chars),
      m_parent_data(orig.m_parent_data),
      m_argument_default(orig.m_argument_default),
      m_is_mutex_group(orig.m_is_mutex_group)
{
}

ARGPARSE_INL _ArgumentGroup&
_ArgumentGroup::operator =(
        _ArgumentGroup const& rhs)
{
    if (this != &rhs) {
        m_data              = rhs.m_data;
        m_prefix_chars      = rhs.m_prefix_chars;
        m_parent_data       = rhs.m_parent_data;
        m_argument_default  = rhs.m_argument_default;
        m_is_mutex_group    = rhs.m_is_mutex_group;
    }
    return *this;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::initializer_list<std::string> const& flags)
{
    return add_argument(std::vector<std::string>{ flags });
}
#else
ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag)
{
    return add_argument(detail::_vector(flag));
}

ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag1,
        std::string const& flag2)
{
    return add_argument(detail::_vector(flag1, flag2));
}

ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3)
{
    return add_argument(detail::_vector(flag1, flag2, flag3));
}

ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3,
        std::string const& flag4)
{
    return add_argument(detail::_vector(flag1, flag2, flag3, flag4));
}
#endif  // C++11+

ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::vector<std::string> const& flags)
{
    m_data->create_argument(m_data, flags, m_prefix_chars);
    process_add_argument();
    return *m_data->m_arguments.back();
}

ARGPARSE_INL void
_ArgumentGroup::process_add_argument()
{
    Argument::Type type
            = static_cast<Argument::Type>(m_data->m_arguments.back()->m_type);
    if (type == Argument::Optional
            && m_parent_data->m_conflict_handler == detail::_conflict_resolve) {
        for (sub_iterator it = m_parent_data->m_optional.begin();
             it != m_parent_data->m_optional.end(); ++it) {
            (*it).first->resolve_conflict_flags(
                        m_data->m_arguments.back()->flags());
        }
    }
    if (m_argument_default.has_value()
            && !m_data->m_arguments.back()->m_default.has_value()
            && !m_data->m_arguments.back()->m_default.suppress()) {
        m_data->m_arguments.back()->default_value(m_argument_default.value());
    }
    if (m_argument_default.suppress()) {
        m_data->m_arguments.back()->default_value(argparse::SUPPRESS);
    }
    m_parent_data->m_arguments.push_back(m_data->m_arguments.back());
    switch (type) {
        case Argument::Positional :
            m_data->m_positional
                    .push_back(std::make_pair(m_data->m_arguments.back(),
                                              !m_is_mutex_group));
            m_parent_data->m_positional
                    .push_back(std::make_pair(m_data->m_arguments.back(),
                                              !m_is_mutex_group));
            break;
        case Argument::Operand :
            m_data->m_operand
                    .push_back(std::make_pair(m_data->m_arguments.back(),
                                              !m_is_mutex_group));
            m_parent_data->m_operand
                    .push_back(std::make_pair(m_data->m_arguments.back(),
                                              !m_is_mutex_group));
            break;
        case Argument::Optional :
            m_data->m_optional
                    .push_back(std::make_pair(m_data->m_arguments.back(),
                                              !m_is_mutex_group));
            m_parent_data->m_optional
                    .push_back(std::make_pair(m_data->m_arguments.back(),
                                              !m_is_mutex_group));
            break;
        default :
            break;
    }
}

// -- ArgumentGroup -----------------------------------------------------------
ARGPARSE_INL
ArgumentGroup::ArgumentGroup(
        std::string const& title,
        std::string const& description,
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::SValue<std::string>& argument_default)
    : _Group(title, description),
      _ArgumentGroup(prefix_chars, parent_data, argument_default, false)
{
}

ARGPARSE_INL detail::shared_ptr<ArgumentGroup>
ArgumentGroup::make_argument_group(
        std::string const& title,
        std::string const& description,
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::SValue<std::string>& argument_default)
{
    return detail::make_shared<ArgumentGroup>(
                ArgumentGroup(title, description, prefix_chars,
                              parent_data, argument_default));
}

ARGPARSE_INL
ArgumentGroup::ArgumentGroup(
        ArgumentGroup const& orig)
    : _Group(orig),
      _ArgumentGroup(orig)
{
}

ARGPARSE_INL ArgumentGroup&
ArgumentGroup::operator =(
        ArgumentGroup const& rhs)
{
    if (this != &rhs) {
        m_title             = rhs.m_title;
        m_description       = rhs.m_description;
        m_data              = rhs.m_data;
        m_prefix_chars      = rhs.m_prefix_chars;
        m_argument_default  = rhs.m_argument_default;
        m_parent_data       = rhs.m_parent_data;
    }
    return *this;
}

ARGPARSE_INL ArgumentGroup&
ArgumentGroup::title(
        std::string const& value,
        std::string const& lang)
{
    m_title[lang] = value;
    return *this;
}

ARGPARSE_INL ArgumentGroup&
ArgumentGroup::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

ARGPARSE_INL ArgumentGroup&
ArgumentGroup::add_argument(
        Argument const& argument)
{
    m_data->validate_argument(Argument(argument), m_prefix_chars);
    process_add_argument();
    return *this;
}

ARGPARSE_INL void
ArgumentGroup::limit_help_flags(
        HelpFormatter const& formatter,
        std::size_t& limit) const
{
    for (arg_iterator it = m_data->m_arguments.begin();
         it != m_data->m_arguments.end(); ++it) {
        detail::_limit_to_min(limit, (*it)->flags_to_string(formatter).size());
    }
}

ARGPARSE_INL void
ArgumentGroup::print_help(
        std::ostream& os,
        bool& eat_ln,
        HelpFormatter const& formatter,
        std::string const& prog,
        bool /*required*/,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    std::string const description = detail::_tr(m_description, lang);
    if (!description.empty() || !m_data->m_arguments.empty()) {
        std::string const title = detail::_tr(m_title, lang);
        if (!title.empty()) {
            detail::_eat_ln(os, eat_ln);
            os << title << ":";
        }
        detail::_print_raw_text_formatter(
                    formatter,
                    detail::_replace(description, "%(prog)s", prog),
                    width, os, eat_ln, "\n", 2);
        for (arg_iterator it = m_data->m_arguments.begin();
             it != m_data->m_arguments.end(); ++it) {
            os << "\n" << (*it)->print(formatter, limit, width, lang);
        }
    }
}

// -- MutuallyExclusiveGroup --------------------------------------------------
ARGPARSE_INL
MutuallyExclusiveGroup::MutuallyExclusiveGroup(
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::SValue<std::string>& argument_default)
    : _ArgumentGroup(prefix_chars, parent_data, argument_default, true),
      m_required(false)
{
}

ARGPARSE_INL MutuallyExclusiveGroup
MutuallyExclusiveGroup::make_mutex_group(
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::SValue<std::string>& argument_default)
{
    return MutuallyExclusiveGroup(prefix_chars, parent_data, argument_default);
}

ARGPARSE_INL
MutuallyExclusiveGroup::MutuallyExclusiveGroup(
        MutuallyExclusiveGroup const& orig)
    : _ArgumentGroup(orig),
      m_required(orig.m_required)
{
}

ARGPARSE_INL MutuallyExclusiveGroup&
MutuallyExclusiveGroup::operator =(
        MutuallyExclusiveGroup const& rhs)
{
    if (this != &rhs) {
        m_data              = rhs.m_data;
        m_prefix_chars      = rhs.m_prefix_chars;
        m_parent_data       = rhs.m_parent_data;
        m_argument_default  = rhs.m_argument_default;
        m_required          = rhs.m_required;
    }
    return *this;
}

ARGPARSE_INL MutuallyExclusiveGroup&
MutuallyExclusiveGroup::required(
        bool value) ARGPARSE_NOEXCEPT
{
    m_required = value;
    return *this;
}

ARGPARSE_INL bool
MutuallyExclusiveGroup::required() const ARGPARSE_NOEXCEPT
{
    return m_required;
}

ARGPARSE_INL MutuallyExclusiveGroup&
MutuallyExclusiveGroup::add_argument(
        Argument const& argument)
{
    m_data->validate_argument(Argument(argument), m_prefix_chars);
    process_add_argument();
    return *this;
}

ARGPARSE_INL std::string
MutuallyExclusiveGroup::usage(
        HelpFormatter const& formatter) const
{
    std::string res;
    for (arg_iterator it = m_data->m_arguments.begin();
         it != m_data->m_arguments.end(); ++it) {
        detail::_append_value_to((*it)->usage(formatter), res, " | ");
    }
    return res.empty() ? std::string()
                       : (m_required ? "(" + res + ")" : "[" + res + "]");
}

// -- _Storage::mapped_type ---------------------------------------------------
ARGPARSE_INL
_Storage::mapped_type::mapped_type()
    : m_exists(),
      m_is_default(),
      m_values(),
      m_indexes()
{ }

ARGPARSE_INL
_Storage::mapped_type::mapped_type(
        std::vector<std::string> const& values)
    : m_exists(true),
      m_is_default(true),
      m_values(values),
      m_indexes()
{
    m_indexes.push_back(m_values.size());
}

ARGPARSE_INL void
_Storage::mapped_type::clear()
{
    m_values.clear();
    m_indexes.clear();
    m_exists = false;
    m_is_default = false;
}

ARGPARSE_INL bool
_Storage::mapped_type::exists() const ARGPARSE_NOEXCEPT
{
    return m_exists;
}

ARGPARSE_INL bool
_Storage::mapped_type::is_default() const ARGPARSE_NOEXCEPT
{
    return m_is_default;
}

ARGPARSE_INL std::vector<std::string> const&
_Storage::mapped_type::operator ()() const ARGPARSE_NOEXCEPT
{
    return m_values;
}

ARGPARSE_INL std::size_t
_Storage::mapped_type::size() const ARGPARSE_NOEXCEPT
{
    return m_values.size();
}

ARGPARSE_INL bool
_Storage::mapped_type::empty() const ARGPARSE_NOEXCEPT
{
    return m_values.empty();
}

ARGPARSE_INL std::string const&
_Storage::mapped_type::front() const ARGPARSE_NOEXCEPT
{
    return m_values.front();
}

ARGPARSE_INL std::string const&
_Storage::mapped_type::at(
        std::size_t i) const
{
    return m_values.at(i);
}

ARGPARSE_INL void
_Storage::mapped_type::push_back(
        std::string const& value,
        bool is_default)
{
    m_is_default = is_default;
    m_values.push_back(value);
    m_indexes.push_back(m_values.size());
    m_exists = true;
}

ARGPARSE_INL void
_Storage::mapped_type::push_values(
        std::vector<std::string> const& values)
{
    detail::_insert_to_end(values, m_values);
    m_indexes.push_back(m_values.size());
    m_exists = true;
}

ARGPARSE_INL std::vector<std::size_t> const&
_Storage::mapped_type::indexes() const ARGPARSE_NOEXCEPT
{
    return m_indexes;
}

ARGPARSE_INL std::vector<std::string>
_Storage::mapped_type::sub_values(
        std::size_t i) const
{
    return std::vector<std::string>(
                m_values.begin() + static_cast<dtype>(
                    i == 0 ? 0 : m_indexes.at(i - 1)),
                m_values.begin() + static_cast<dtype>(m_indexes.at(i)));
}
// -- _Storage ----------------------------------------------------------------
ARGPARSE_INL
_Storage::_Storage()
    : m_data()
{ }

ARGPARSE_INL void
_Storage::create(
        key_type const& key,
        mapped_type const& value)
{
    if (key->action() & (argparse::version | argparse::help)) {
        return;
    }
    bool have_key = false;
    for (const_iterator it = m_data.begin(); it != m_data.end(); ++it) {
        have_key = have_key || (key == (*it).first);
        if (key != (*it).first) {
            (*it).first->resolve_conflict_flags(key->flags());
        }
    }
    if (!have_key) {
        m_data.push_back(std::make_pair(key, value));
    }
}

ARGPARSE_INL void
_Storage::create(
        std::vector<key_type> const& keys)
{
    for (std::size_t i = 0; i < keys.size(); ++i) {
        create(keys.at(i));
    }
}

ARGPARSE_INL void
_Storage::have_value(
        key_type const& key)
{
    if (key->implicit_value().empty()) {
        at(key).push_values(std::vector<std::string>());
        on_process_store(key, std::vector<std::string>());
    } else {
        at(key).push_back(key->implicit_value());
        on_process_store(key, key->implicit_value());
        key->handle(key->implicit_value());
    }
}

ARGPARSE_INL void
_Storage::store_value(
        key_type const& key,
        std::string const& value)
{
    at(key).push_back(value);
    on_process_store(key, value);
    key->handle(value);
}

ARGPARSE_INL void
_Storage::store_values(
        key_type const& key,
        std::vector<std::string> const& values)
{
    at(key).push_values(values);
    on_process_store(key, values);
    for (std::size_t i = 0; i < values.size(); ++i) {
        key->handle(values.at(i));
    }
}

ARGPARSE_INL void
_Storage::store_default_value(
        key_type const& key)
{
    if (key->m_default.has_value()
            && (key->action()
                & (argparse::store | argparse::BooleanOptionalAction))) {
        mapped_type& arg_data = at(key);
        if (arg_data.empty()) {
            arg_data.push_back(key->m_default.value(), true);
            on_process_store(key, key->m_default.value());
            key->handle(key->m_default.value());
        }
    }
}

ARGPARSE_INL bool
_Storage::self_value_stored(
        key_type const& key)
{
    if (key->action() & (argparse::store_const | detail::_bool_action)) {
        mapped_type& arg_data = at(key);
        arg_data.clear();
        arg_data.push_back(key->const_value());
        on_process_store(key, key->const_value());
        key->handle(key->const_value());
        return true;
    } else if (key->action() == argparse::append_const) {
        at(key).push_back(key->const_value());
        on_process_store(key, key->const_value());
        key->handle(key->const_value());
        return true;
    } else if (key->action() == argparse::count) {
        at(key).push_back(std::string());
        on_process_store(key, std::string());
        key->handle(std::string());
        return true;
    }
    return false;
}

ARGPARSE_INL bool
_Storage::exists_arg(
        std::string const& key) const
{
    return find_arg(key) != end();
}

ARGPARSE_INL bool
_Storage::exists(
        std::string const& key) const
{
    return find(key) != end();
}

ARGPARSE_INL bool
_Storage::exists(
        key_type const& key) const
{
    return find(key) != end();
}

ARGPARSE_INL _Storage::value_type const&
_Storage::at(
        std::string const& key) const
{
    const_iterator it = find(key);
    if (it == end()) {
        throw std::out_of_range("key '" + key + "' not found");
    }
    return *it;
}

ARGPARSE_INL _Storage::mapped_type&
_Storage::at(
        key_type const& key)
{
    iterator it = find(key);
    if (it == end()) {
        throw std::out_of_range("key '" + key->m_name + "' not found");
    }
    return it->second;
}

ARGPARSE_INL _Storage::mapped_type const&
_Storage::at(
        key_type const& key) const
{
    const_iterator it = find(key);
    if (it == end()) {
        throw std::out_of_range("key '" + key->m_name + "' not found");
    }
    return it->second;
}

ARGPARSE_INL _Storage::iterator
_Storage::erase(
        iterator it)
{
    return m_data.erase(it);
}

ARGPARSE_INL _Storage::iterator
_Storage::begin() ARGPARSE_NOEXCEPT
{
    return m_data.begin();
}

ARGPARSE_INL _Storage::iterator
_Storage::end() ARGPARSE_NOEXCEPT
{
    return m_data.end();
}

ARGPARSE_INL _Storage::const_iterator
_Storage::begin() const ARGPARSE_NOEXCEPT
{
    return m_data.begin();
}

ARGPARSE_INL _Storage::const_iterator
_Storage::end() const ARGPARSE_NOEXCEPT
{
    return m_data.end();
}

ARGPARSE_INL _Storage::const_iterator
_Storage::find_arg(
        std::string const& key) const
{
    const_iterator it = find(key);
    if (it != end()) {
        return it;
    }
    for (it = begin(); it != end() && !it->first->is_match_name(key); ++it) {
    }
    return it;
}

ARGPARSE_INL _Storage::const_iterator
_Storage::find(
        std::string const& key) const
{
    const_iterator it = begin();
    for ( ; it != end() && !(*(it->first) == key); ++it) {
    }
    return it;
}

ARGPARSE_INL _Storage::const_iterator
_Storage::find(
        key_type const& key) const
{
    const_iterator it = begin();
    for ( ; it != end() && it->first != key; ++it) {
    }
    return it;
}

ARGPARSE_INL _Storage::iterator
_Storage::find(
        key_type const& key)
{
    iterator it = begin();
    for ( ; it != end() && it->first != key; ++it) {
    }
    return it;
}

ARGPARSE_INL void
_Storage::on_process_store(
        key_type const& key,
        std::string const& value)
{
    if (!key->dest().empty()) {
        for (iterator it = begin(); it != end(); ++it) {
            if (it->first != key && it->first->dest() == key->dest()) {
                if (it->first->action() & (argparse::store
                                           | argparse::store_const
                                           | detail::_bool_action
                                           | argparse::BooleanOptionalAction)) {
                    it->second.clear();
                }
                it->second.push_back(value);
            }
        }
    }
}

ARGPARSE_INL void
_Storage::on_process_store(
        key_type const& key,
        std::vector<std::string> const& values)
{
    if (!key->dest().empty()) {
        for (iterator it = begin(); it != end(); ++it) {
            if (it->first != key && it->first->dest() == key->dest()) {
                if (it->first->action() & (argparse::store
                                           | argparse::store_const
                                           | detail::_bool_action
                                           | argparse::BooleanOptionalAction)) {
                    it->second.clear();
                }
                it->second.push_values(values);
            }
        }
    }
}

// -- Namespace ---------------------------------------------------------------
ARGPARSE_INL
Namespace::Namespace(
        _Storage const& storage)
    : m_storage(storage),
      m_unrecognized_args()
{
}

ARGPARSE_INL
Namespace::Namespace(
        _Storage const& storage,
        std::vector<std::string> const& args)
    : m_storage(storage),
      m_unrecognized_args(args)
{
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL
Namespace::Namespace(
        _Storage&& storage) ARGPARSE_NOEXCEPT
    : m_storage(std::move(storage)),
      m_unrecognized_args()
{
}

ARGPARSE_INL
Namespace::Namespace(
        _Storage&& storage,
        std::vector<std::string>&& args) ARGPARSE_NOEXCEPT
    : m_storage(std::move(storage)),
      m_unrecognized_args(std::move(args))
{
}
#endif  // C++11+

ARGPARSE_INL bool
Namespace::contains(
        std::string const& key) const
{
    _Storage::const_iterator it = storage().find_arg(key);
    return it != storage().end()
            && (!it->second.empty() || it->first->action() == argparse::count);
}

ARGPARSE_INL bool
Namespace::exists(
        std::string const& key) const
{
    return contains(key);
}

ARGPARSE_INL void
Namespace::print(
        std::ostream& os) const
{
    os << to_string() << std::endl;
}

ARGPARSE_INL std::string
Namespace::to_args(
        std::string const& key) const
{
    _Storage::value_type const& args = data(key);
    switch (args.first->action()) {
        case argparse::store_const :
            if (args.second.empty()) {
                return std::string();
            }
            if (args.second.size() != 1) {
                throw TypeError("got a data-array for argument '" + key + "'");
            }
            return detail::_have_quotes(args.second.front())
                    ? args.second.front()
                    : detail::_replace(
                          args.second.front(), detail::_spaces, "\\ ");
        case argparse::store_true :
        case argparse::store_false :
        case argparse::BooleanOptionalAction :
            return detail::_boolean_option_to_args(key, args);
        case argparse::count :
            return detail::_to_string(args.second.size());
        case argparse::store :
        case argparse::append :
        case argparse::append_const :
        case argparse::extend :
        case argparse::language :
            return detail::_vector_to_string(
                        args.second(), detail::_spaces, std::string(), true);
        default :
            throw ValueError("action not supported");
    }
}

ARGPARSE_INL std::string
Namespace::to_string(
        std::string const& key,
        std::string const& quotes) const
{
    _Storage::value_type const& args = data(key);
    switch (args.first->action()) {
        case argparse::store_const :
            if (args.second.empty()) {
                return detail::_none;
            }
            if (args.second.size() != 1) {
                throw TypeError("got a data-array for argument '" + key + "'");
            }
            return quotes + args.second.front() + quotes;
        case argparse::store_true :
        case argparse::store_false :
        case argparse::BooleanOptionalAction :
            return detail::_boolean_option_to_string(key, args, quotes);
        case argparse::count :
            return args.second.empty()
                    ? detail::_none : detail::_to_string(args.second.size());
        case argparse::store :
        case argparse::append :
        case argparse::append_const :
        case argparse::extend :
        case argparse::language :
            return store_actions_to_string(args, quotes);
        default :
            throw ValueError("action not supported");
    }
}

ARGPARSE_INL std::string
Namespace::to_string() const
{
    std::string res;
    for (_Storage::const_iterator it
         = storage().begin(); it != storage().end(); ++it) {
        _Storage::value_type const& pair = *it;
        std::vector<std::string> const& str = pair.first->get_argument_flags();
        if (str.empty()) {
            continue;
        }
        detail::_append_value_to(pair.first->get_dest() + detail::_equals
                                 + to_string(str.front(), "'"), res, ", ");
    }
    if (!m_unrecognized_args.has_value()) {
        return "Namespace(" + res + ")";
    }
    std::string unknown_args = detail::_join(unrecognized_args(), ", ", "'");
    return "(Namespace(" + res + "), [" + unknown_args + "])";
}

ARGPARSE_INL std::vector<std::string> const&
Namespace::unrecognized_args() const ARGPARSE_NOEXCEPT
{
    return m_unrecognized_args.value();
}

ARGPARSE_INL std::string
Namespace::unrecognized_args_to_args() const
{
    return detail::_vector_to_string(
                unrecognized_args(), detail::_spaces, std::string(), true);
}

ARGPARSE_INL std::string
Namespace::store_actions_to_string(
        _Storage::value_type const& args,
        std::string const& quotes)
{
    if (((args.first->action() & (argparse::store | argparse::language))
         && (args.first->m_nargs
             & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE)))
            || (!args.second.exists()
                && args.first->m_type == Argument::Optional)
            || args.first->m_nargs == Argument::SUPPRESSING
            || args.second.is_default()) {
        return detail::_vector_to_string(
                    args.second(), ", ", quotes, false, detail::_none);
    }
    if (args.first->action() != argparse::append
            || (args.first->m_nargs
                & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE))) {
        std::string none
                = (args.first->m_nargs
                   & (Argument::ZERO_OR_MORE | Argument::REMAINDING))
                || (args.first->action() == argparse::extend
                    && args.first->m_nargs == Argument::ZERO_OR_ONE)
                ? std::string() : detail::_none;
        return detail::_vector_to_string(args.second(),
                                         ", ", quotes, false, none, "[", "]");
    } else {
        std::string none = (args.first->m_nargs
                            & (Argument::ZERO_OR_MORE | Argument::REMAINDING))
                ? std::string() : detail::_none;
        return detail::_matrix_to_string(args.second(), args.second.indexes(),
                                         ", ", quotes, false, none, "[", "]");
    }
}

ARGPARSE_INL _Storage::value_type const&
Namespace::data(
        std::string const& key) const
{
    _Storage::const_iterator it = storage().find_arg(key);
    if (it != storage().end()) {
        return *it;
    }
    throw AttributeError("'Namespace' object has no attribute '" + key + "'");
}

ARGPARSE_INL _Storage const&
Namespace::storage() const ARGPARSE_NOEXCEPT
{
    return m_storage;
}

#ifdef ARGPARSE_HAS_OPTIONAL
ARGPARSE_INL std::optional<_Storage::value_type>
Namespace::opt_data(
        std::string const& key) const
{
    auto it = storage().find_arg(key);
    if (it != storage().end()) {
        return *it;
    }
    return std::nullopt;
}
#endif  // ARGPARSE_HAS_OPTIONAL

// -- _ParserGroup ------------------------------------------------------------
ARGPARSE_INL
_ParserGroup::_ParserGroup(
        std::string const& title,
        std::string const& description)
    : _Group(title, description),
      m_help(),
      m_metavar(),
      m_parsers()
{
    m_help.will_have()[std::string()] = std::string();
}

ARGPARSE_INL _ParserGroup::pParser
_ParserGroup::make_parser(
        std::string const& name)
{
    pParser res = detail::make_shared<ArgumentParser>();
    res->m_prog.clear();
    res->m_name = name;
    return res;
}

ARGPARSE_INL std::string const&
_ParserGroup::help() const
{
    return detail::_tr_at(m_help.value(), std::string());
}

ARGPARSE_INL std::string const&
_ParserGroup::metavar() const ARGPARSE_NOEXCEPT
{
    return m_metavar.value();
}

ARGPARSE_INL void
_ParserGroup::limit_help_flags(
        HelpFormatter const&,
        std::size_t& limit) const
{
    if (m_help.suppress()) {
        return;
    }
    detail::_limit_to_min(limit, _flags_to_string().size());
    for (prs_iterator it = m_parsers.begin(); it != m_parsers.end(); ++it) {
        detail::_limit_to_min(limit, (*it)->m_name.size() + 2);
    }
}

ARGPARSE_INL void
_ParserGroup::print_help(
        std::ostream& os,
        bool& eat_ln,
        HelpFormatter const& formatter,
        std::string const& prog,
        bool required,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    if (m_help.suppress()) {
        return;
    }
    detail::_eat_ln(os, eat_ln);
    std::string const title = detail::_tr(m_title, lang);
    os << (title.empty() ? "subcommands" : title) << ":";
    detail::_print_raw_text_formatter(
                formatter,
                detail::_replace(
                    detail::_tr(m_description, lang), "%(prog)s", prog),
                width, os, eat_ln, std::string(), 2, "\n");
    print_parser_group(os, formatter, prog, required, limit, width, lang);
}

ARGPARSE_INL void
_ParserGroup::print_parser_group(
        std::ostream& os,
        HelpFormatter const& formatter,
        std::string const& prog,
        bool required,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    if (m_help.suppress()) {
        return;
    }
    // despecify parser group help
    std::string help = detail::_tr(m_help.value(), lang);
    std::string res = help;
    std::string text;
#ifdef ARGPARSE_CXX_11
    std::regex const r("%[(]([a-z_]*)[)]s");
    std::smatch match;
    std::unordered_map<std::string, std::function<std::string()> > const
            specifiers =
    {
        { "%(prog)s",[&prog]() -> std::string const& { return prog; } },
        { "%(choices)s",        [this]() { return get_choices();    } },
        { "%(help)s",[&help]() -> std::string const& { return help; } },
        { "%(metavar)s",        [this]() { return get_metavar();    } },
        { "%(option_strings)s", []    () { return "[]";             } },
        { "%(required)s",
            [required](){ return detail::_bool_to_string(required); } },
    };
    while (std::regex_search(res, match, r)) {
        text += match.prefix();
        auto specifier = std::string(match[0]);
        auto it = specifiers.find(specifier);
        text += (it != specifiers.end() ? it->second() : std::move(specifier));
        res = match.suffix();
    }
#else
    std::string const beg = "%(";
    std::string const end = ")s";
    std::string::size_type pos = res.find(beg);
    while (pos != std::string::npos) {
        std::string::size_type next = res.find(end, pos + beg.size());
        if (next == std::string::npos) {
            break;
        }
        text += res.substr(0, pos);
        std::string specifier = res.substr(pos, next + end.size() - pos);
        if (specifier == "%(prog)s") {
            text += prog;
        } else if (specifier == "%(choices)s") {
            text += get_choices();
        } else if (specifier == "%(metavar)s") {
            text += get_metavar();
        } else if (specifier == "%(option_strings)s") {
            text += "[]";
        } else if (specifier == "%(required)s") {
            text += detail::_bool_to_string(required);
        } else if (specifier == "%(help)s") {
            text += help;
        } else {
            text += specifier;
        }
        res = res.substr(next + end.size());
        pos = res.find(beg);
    }
#endif  // C++11+
    text += res;
    std::swap(res, text);
    os << "\n" << detail::_help_formatter(
              "  " + _flags_to_string(), formatter, res, width, limit);
    for (prs_iterator it = m_parsers.begin(); it != m_parsers.end(); ++it) {
        // despecify group's parser help
        help = detail::_tr((*it)->m_help, lang);
        if (!help.empty()) {
            std::string metavar = (*it)->m_name;
            std::string aliases = detail::_join((*it)->aliases(), ", ");
            if (!aliases.empty()) {
                metavar += " (" + aliases + ")";
            }
            std::string name = "    " + metavar;
            res = help;
            text.clear();
#ifdef ARGPARSE_CXX_11
            std::unordered_map<std::string, std::function<std::string()> > const
                    specifiers2 =
            {
                { "%(prog)s",
                    [&prog]   () -> std::string const& { return prog;   } },
                { "%(choices)s",        []() { return detail::_none;    } },
                { "%(help)s",
                    [&help]   () -> std::string const& { return help;   } },
                { "%(metavar)s",
                    [&metavar]() -> std::string const& { return metavar;} },
                { "%(option_strings)s", []() { return "[]";             } },
                { "%(required)s",       []() { return "False";          } },
            };
            while (std::regex_search(res, match, r)) {
                text += match.prefix();
                auto specifier = std::string(match[0]);
                auto it2 = specifiers2.find(specifier);
                text += (it2 != specifiers2.end() ? it2->second()
                                                  : std::move(specifier));
                res = match.suffix();
            }
#else
            pos = res.find(beg);
            while (pos != std::string::npos) {
                std::string::size_type next = res.find(end, pos + beg.size());
                if (next == std::string::npos) {
                    break;
                }
                text += res.substr(0, pos);
                std::string specifier
                        = res.substr(pos, next + end.size() - pos);
                if (specifier == "%(prog)s") {
                    text += prog;
                } else if (specifier == "%(choices)s") {
                    text += detail::_none;
                } else if (specifier == "%(metavar)s") {
                    text += metavar;
                } else if (specifier == "%(option_strings)s") {
                    text += "[]";
                } else if (specifier == "%(required)s") {
                    text += "False";
                } else if (specifier == "%(help)s") {
                    text += help;
                } else {
                    text += specifier;
                }
                res = res.substr(next + end.size());
                pos = res.find(beg);
            }
#endif  // C++11+
            text += res;
            std::swap(res, text);
            os << "\n"
               << detail::_help_formatter(name, formatter, res, width, limit);
        }
    }
}

ARGPARSE_INL std::vector<std::string>
_ParserGroup::_parser_names() const
{
    std::vector<std::string> res;
    res.reserve(2 * m_parsers.size());
    for (prs_iterator it = m_parsers.begin(); it != m_parsers.end(); ++it) {
        res.push_back((*it)->m_name);
        for (std::size_t a = 0; a < (*it)->aliases().size(); ++a) {
            res.push_back((*it)->aliases().at(a));
        }
    }
    return res;
}

ARGPARSE_INL std::string
_ParserGroup::_flags_to_string() const
{
    if (m_metavar.has_value()) {
        return metavar();
    }
    std::string res;
    for (prs_iterator it = m_parsers.begin(); it != m_parsers.end(); ++it) {
        detail::_append_value_to((*it)->m_name, res, ",");
        for (std::size_t a = 0; a < (*it)->aliases().size(); ++a) {
            detail::_append_value_to((*it)->aliases().at(a), res, ",");
        }
    }
    return "{" + res + "}";
}

ARGPARSE_INL bool
_ParserGroup::_is_positional() const
{
    return title().empty() && description().empty();
}

ARGPARSE_INL std::string
_ParserGroup::get_choices() const
{
    return detail::_join(_parser_names(), ", ");
}

ARGPARSE_INL std::string
_ParserGroup::get_metavar() const
{
    return m_metavar.has_value() ? metavar() : detail::_none;
}

// -- ParserGroup -------------------------------------------------------------
ARGPARSE_INL
ParserGroup::ParserGroup(
        SubParsers* parent,
        std::string const& title,
        std::string const& description)
    : _ParserGroup(title, description),
      m_parent(parent)
{
}

ARGPARSE_INL detail::shared_ptr<ParserGroup>
ParserGroup::make_parsergroup(
        SubParsers* parent,
        std::string const& title,
        std::string const& description)
{
    return detail::make_shared<ParserGroup>(
                ParserGroup(parent, title, description));
}

ARGPARSE_INL
ParserGroup::ParserGroup(
        ParserGroup const& orig)
    : _ParserGroup(orig),
      m_parent(orig.m_parent)
{
}

ARGPARSE_INL ParserGroup&
ParserGroup::operator =(
        ParserGroup const& rhs)
{
    if (this != &rhs) {
        m_title         = rhs.m_title;
        m_description   = rhs.m_description;
        m_parent        = rhs.m_parent;
        m_help          = rhs.m_help;
        m_metavar       = rhs.m_metavar;
        m_parsers       = rhs.m_parsers;
    }
    return *this;
}

ARGPARSE_INL ParserGroup&
ParserGroup::title(
        std::string const& value,
        std::string const& lang)
{
    m_title[lang] = value;
    return *this;
}

ARGPARSE_INL ParserGroup&
ParserGroup::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

ARGPARSE_INL ParserGroup&
ParserGroup::help(
        std::string const& value,
        std::string const& lang)
{
    m_help.will_have()[lang] = value;
    return *this;
}

ARGPARSE_INL ParserGroup&
ParserGroup::help(
        _SUPPRESS value)
{
    m_help = value;
    return *this;
}

ARGPARSE_INL ParserGroup&
ParserGroup::metavar(
        std::string const& value)
{
    m_metavar = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ParserGroup::add_parser(
        std::string const& name)
{
    if (name.empty()) {
        throw ValueError("parser name can't be empty");
    }
    m_parsers.push_back(make_parser(name));
    if (m_parent) {
        m_parsers.back()->update_prog(m_parent->prog_name());
    }
    return *m_parsers.back();
}

ARGPARSE_INL std::vector<std::string>
ParserGroup::parser_names() const
{
    return _parser_names();
}

// -- SubParsers --------------------------------------------------------------
ARGPARSE_INL
SubParsers::SubParsers(
        std::string const& title,
        std::string const& description)
    : _ParserGroup(title, description),
      m_parent_prog(),
      m_parent_args(),
      m_prog(),
      m_dest(),
      m_groups(),
      m_required(false)
{
}

ARGPARSE_INL detail::shared_ptr<SubParsers>
SubParsers::make_subparsers(
        std::string const& title,
        std::string const& description)
{
    return detail::make_shared<SubParsers>(SubParsers(title, description));
}

ARGPARSE_INL SubParsers&
SubParsers::title(
        std::string const& value,
        std::string const& lang)
{
    m_title[lang] = value;
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::prog(
        std::string const& value)
{
    m_prog = value;
    update_prog(m_parent_prog, m_parent_args);
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::dest(
        std::string const& value)
{
    m_dest = value;
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::required(
        bool value) ARGPARSE_NOEXCEPT
{
    m_required = value;
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::help(
        std::string const& value,
        std::string const& lang)
{
    m_help.will_have()[lang] = value;
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::help(
        _SUPPRESS value)
{
    m_help = value;
    return *this;
}

ARGPARSE_INL SubParsers&
SubParsers::metavar(
        std::string const& value)
{
    m_metavar = value;
    return *this;
}

ARGPARSE_INL std::string const&
SubParsers::prog() const ARGPARSE_NOEXCEPT
{
    return m_prog;
}

ARGPARSE_INL std::string const&
SubParsers::dest() const ARGPARSE_NOEXCEPT
{
    return m_dest;
}

ARGPARSE_INL bool
SubParsers::required() const ARGPARSE_NOEXCEPT
{
    return m_required;
}

ARGPARSE_INL ArgumentParser&
SubParsers::add_parser(
        std::string const& name)
{
    if (name.empty()) {
        throw ValueError("parser name can't be empty");
    }
    m_parsers.push_back(make_parser(name));
    m_parsers.back()->update_prog(prog_name());
    return *m_parsers.back();
}

ARGPARSE_INL ParserGroup&
SubParsers::add_parser_group(
        std::string const& title,
        std::string const& description)
{
    m_groups.push_back(ParserGroup::make_parsergroup(this, title, description));
    return *m_groups.back();
}

ARGPARSE_INL std::vector<std::string>
SubParsers::parser_names() const
{
    std::vector<std::string> res = _parser_names();
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        detail::_insert_to_end((*it)->_parser_names(), res);
    }
    return res;
}

ARGPARSE_INL void
SubParsers::limit_help_flags(
        HelpFormatter const& formatter,
        std::size_t& limit) const
{
    _ParserGroup::limit_help_flags(formatter, limit);
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        (*it)->limit_help_flags(formatter, limit);
    }
}

ARGPARSE_INL void
SubParsers::print_help(
        std::ostream& os,
        bool& eat_ln,
        HelpFormatter const& formatter,
        std::string const& prog,
        bool /*required*/,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    _ParserGroup::print_help(os, eat_ln, formatter, prog,
                             required(), limit, width, lang);
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        (*it)->print_help(os, eat_ln, formatter, prog,
                          required(), limit, width, lang);
    }
}

ARGPARSE_INL std::list<SubParsers::pParser>
SubParsers::list_parsers() const
{
    std::list<pParser> res = m_parsers;
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        res.insert(res.end(), (*it)->m_parsers.begin(), (*it)->m_parsers.end());
    }
    return res;
}

ARGPARSE_INL std::string
SubParsers::prog_name() const
{
    std::string program = m_prog.empty() ? m_parent_prog : m_prog;
    if (!m_parent_args.empty()) {
        program += " " + m_parent_args;
    }
    return program;
}

ARGPARSE_INL void
SubParsers::update_prog(
        std::string const& parent_prog,
        std::string const& parent_args)
{
    m_parent_prog = parent_prog;
    m_parent_args = parent_args;
    std::string const program = prog_name();
    std::list<SubParsers::pParser> parsers = list_parsers();
    for (prs_iterator it = parsers.begin(); it != parsers.end(); ++it) {
        (*it)->update_prog(program);
    }
}

ARGPARSE_INL std::string
SubParsers::usage() const
{
    return flags_to_string() + " ...";
}

ARGPARSE_INL std::string
SubParsers::flags_to_string() const
{
    std::string res;
    if (!m_help.suppress()) {
        res += _flags_to_string();
    }
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        if (!(*it)->m_help.suppress()) {
            detail::_append_value_to((*it)->_flags_to_string(), res, " | ");
        }
    }
    return res;
}

ARGPARSE_INL bool
SubParsers::is_positional() const
{
    bool res = _is_positional();
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        res = res && (*it)->_is_positional();
    }
    return res;
}

ARGPARSE_INL bool
SubParsers::is_suppress() const
{
    bool res = m_help.suppress();
    for (pgr_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        res = res && (*it)->m_help.suppress();
    }
    return res;
}

// -- ArgumentParser ----------------------------------------------------------
ARGPARSE_INL void
ArgumentParser::read_args(
        int argc,
        char const* const argv[])
{
    if (argc > 0 && argv && argv[0]) {
        m_prog = detail::_file_name(std::string(argv[0]));
        m_parsed_arguments.reserve(std::size_t(argc - 1));
        for (int i = 1; i < argc && argv[i]; ++i) {
            m_parsed_arguments.push_back(std::string(argv[i]));
        }
    }
}

ARGPARSE_INL void
ArgumentParser::read_env(
        char const* const envp[])
{
    if (envp) {
        for (int i = 0; envp[i]; ++i) {
            std::vector<std::string> pair
                    = detail::_split(std::string(envp[i]), detail::_equals, 1);
            pair.resize(2);
            m_env_variables.push_back(std::make_pair(pair.at(0), pair.at(1)));
        }
    }
}

ARGPARSE_INL void
ArgumentParser::initialize_parser()
{
    formatter_class(HelpFormatter());
    m_data->update_help(true, m_prefix_chars);
    // init translations
    m_usage.will_have()[std::string()] = std::string();
    m_usage_title[std::string()] = "usage";
    m_description[std::string()] = std::string();
    m_positionals_title[std::string()] = "positional arguments";
    m_operands_title[std::string()] = "operands";
    m_optionals_title[std::string()] = "options";
    m_epilog[std::string()] = std::string();
    m_help[std::string()] = std::string();
}

ARGPARSE_INL
ArgumentParser::ArgumentParser(
        std::string const& prog)
    : m_data(_ArgumentData::make_argument_data()),
      m_name(),
      m_prog(detail::_prog),
      m_usage(),
      m_usage_title(),
      m_description(),
      m_positionals_title(),
      m_operands_title(),
      m_optionals_title(),
      m_epilog(),
      m_help(),
      m_aliases(),
      m_formatter(),
      m_prefix_chars(detail::_prefix_chars),
      m_fromfile_prefix_chars(),
      m_comment_prefix_chars(),
      m_argument_default(),
      m_output_width(),
      m_groups(),
      m_mutex_groups(),
      m_default_values(),
      m_parsed_arguments(),
      m_env_variables(),
      m_subparsers(ARGPARSE_NULLPTR),
      m_subparsers_position(),
      m_handle(ARGPARSE_NULLPTR),
      m_parse_handle(ARGPARSE_NULLPTR),
      m_allow_abbrev(true),
      m_exit_on_error(true)
{
    initialize_parser();
    this->prog(prog);
}

ARGPARSE_INL
ArgumentParser::ArgumentParser(
        int argc,
        char const* const argv[],
        std::string const& prog)
    : m_data(_ArgumentData::make_argument_data()),
      m_name(),
      m_prog(detail::_prog),
      m_usage(),
      m_usage_title(),
      m_description(),
      m_positionals_title(),
      m_operands_title(),
      m_optionals_title(),
      m_epilog(),
      m_help(),
      m_aliases(),
      m_formatter(),
      m_prefix_chars(detail::_prefix_chars),
      m_fromfile_prefix_chars(),
      m_comment_prefix_chars(),
      m_argument_default(),
      m_output_width(),
      m_groups(),
      m_mutex_groups(),
      m_default_values(),
      m_parsed_arguments(),
      m_env_variables(),
      m_subparsers(ARGPARSE_NULLPTR),
      m_subparsers_position(),
      m_handle(ARGPARSE_NULLPTR),
      m_parse_handle(ARGPARSE_NULLPTR),
      m_allow_abbrev(true),
      m_exit_on_error(true)
{
    initialize_parser();
    read_args(argc, argv);
    this->prog(prog);
}

ARGPARSE_INL
ArgumentParser::ArgumentParser(
        int argc,
        char const* const argv[],
        char const* const envp[],
        std::string const& prog)
    : m_data(_ArgumentData::make_argument_data()),
      m_name(),
      m_prog(detail::_prog),
      m_usage(),
      m_usage_title(),
      m_description(),
      m_positionals_title(),
      m_operands_title(),
      m_optionals_title(),
      m_epilog(),
      m_help(),
      m_aliases(),
      m_formatter(),
      m_prefix_chars(detail::_prefix_chars),
      m_fromfile_prefix_chars(),
      m_comment_prefix_chars(),
      m_argument_default(),
      m_output_width(),
      m_groups(),
      m_mutex_groups(),
      m_default_values(),
      m_parsed_arguments(),
      m_env_variables(),
      m_subparsers(ARGPARSE_NULLPTR),
      m_subparsers_position(),
      m_handle(ARGPARSE_NULLPTR),
      m_parse_handle(ARGPARSE_NULLPTR),
      m_allow_abbrev(true),
      m_exit_on_error(true)
{
    initialize_parser();
    read_args(argc, argv);
    read_env(envp);
    this->prog(prog);
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::prog(
        std::string const& value)
{
    if (!value.empty()) {
        m_prog = value;
        if (m_subparsers) {
            m_subparsers->update_prog(prog(), subparsers_prog_args());
        }
    }
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::usage(
        std::string const& value,
        std::string const& lang)
{
    m_usage.will_have()[lang] = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::usage(
        _SUPPRESS value)
{
    m_usage = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::usage_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_usage_title[lang] = value;
    }
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::positionals_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_positionals_title[lang] = value;
    }
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::operands_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_operands_title[lang] = value;
    }
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::optionals_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_optionals_title[lang] = value;
    }
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::epilog(
        std::string const& value,
        std::string const& lang)
{
    m_epilog[lang] = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::help(
        std::string const& value,
        std::string const& lang)
{
    m_help[lang] = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::vector<std::string> const& value)
{
    m_aliases.clear();
    std::vector<std::string>::const_iterator it = value.begin();
    for ( ; it != value.end(); ++it) {
        if (!(*it).empty()) {
            m_aliases.push_back(*it);
        }
    }
    return *this;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::initializer_list<std::string> const& value)
{
    return aliases(std::vector<std::string>{ value });
}
#else
ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::string const& value)
{
    return aliases(detail::_vector(value));
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::string const& value1,
        std::string const& value2)
{
    return aliases(detail::_vector(value1, value2));
}
#endif  // C++11+

ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        std::vector<ArgumentParser> const& value)
{
    for (std::size_t i = 0; i < value.size(); ++i) {
        ArgumentParser const& parent = value.at(i);
        if (this == &parent) {
            continue;
        }
        if (parent.has_subparsers()) {
            if (has_subparsers()) {
                throw_error("cannot have multiple subparser arguments");
            }
            m_subparsers_position
                   = parent.m_subparsers_position + m_data->m_positional.size();
            m_subparsers = parent.m_subparsers;
            m_subparsers->update_prog(prog(), subparsers_prog_args());
        }
        m_data->merge_arguments(*parent.m_data.get());
        detail::_insert_to_end(parent.m_groups, m_groups);
        detail::_insert_to_end(parent.m_mutex_groups, m_mutex_groups);
        detail::_insert_to_end(parent.m_default_values, m_default_values);
    }
    return *this;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        std::initializer_list<ArgumentParser> const& value)
{
    return parents(std::vector<ArgumentParser>{ value });
}
#else
ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        ArgumentParser const& value)
{
    return parents(detail::_vector(value));
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        ArgumentParser const& value1,
        ArgumentParser const& value2)
{
    return parents(detail::_vector(value1, value2));
}
#endif  // C++11+

ARGPARSE_INL ArgumentParser&
ArgumentParser::prefix_chars(
        std::string const& value)
{
    std::string val = detail::_get_punct(value);
    if (!val.empty()) {
        m_prefix_chars = ARGPARSE_MOVE(val);
        m_data->update_help(m_data->m_add_help, m_prefix_chars);
    }
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::fromfile_prefix_chars(
        std::string const& value)
{
    m_fromfile_prefix_chars = detail::_get_punct(value);
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::comment_prefix_chars(
        std::string const& value)
{
    m_comment_prefix_chars = detail::_get_punct(value);
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::argument_default(
        std::string const& value)
{
    m_argument_default = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::argument_default(
        _SUPPRESS value)
{
    m_argument_default = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::conflict_handler(
        std::string const& value)
{
    if (value == "resolve") {
        m_data->m_conflict_handler = detail::_conflict_resolve;
    } else if (value == "error") {
        m_data->m_conflict_handler = detail::_conflict_error;
    } else {
        throw AttributeError("'ArgumentParser' object has no attribute "
                             "'_handle_conflict_" + value + "'");
    }
    m_data->m_conflict_handler_str = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::add_help(
        bool value)
{
    m_data->update_help(value, m_prefix_chars);
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::allow_abbrev(
        bool value) ARGPARSE_NOEXCEPT
{
    m_allow_abbrev = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::exit_on_error(
        bool value) ARGPARSE_NOEXCEPT
{
    m_exit_on_error = value;
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::output_width(
        std::size_t value) ARGPARSE_NOEXCEPT
{
    m_output_width = value < detail::_min_width ? detail::_min_width : value;
    return *this;
}

ARGPARSE_INL std::string const&
ArgumentParser::prog() const ARGPARSE_NOEXCEPT
{
    return m_prog;
}

ARGPARSE_INL std::string const&
ArgumentParser::usage() const
{
    return detail::_tr_at(m_usage.value(), default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::usage_title() const
{
    return detail::_tr_at(m_usage_title, default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::description() const
{
    return detail::_tr_at(m_description, default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::positionals_title() const
{
    return detail::_tr_at(m_positionals_title, default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::operands_title() const
{
    return detail::_tr_at(m_operands_title, default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::optionals_title() const
{
    return detail::_tr_at(m_optionals_title, default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::epilog() const
{
    return detail::_tr_at(m_epilog, default_language());
}

ARGPARSE_INL std::string const&
ArgumentParser::help() const
{
    return detail::_tr_at(m_help, default_language());
}

ARGPARSE_INL std::vector<std::string> const&
ArgumentParser::aliases() const ARGPARSE_NOEXCEPT
{
    return m_aliases;
}

ARGPARSE_INL HelpFormatter&
ArgumentParser::formatter_class() ARGPARSE_NOEXCEPT
{
    return *m_formatter;
}

ARGPARSE_INL std::string const&
ArgumentParser::prefix_chars() const ARGPARSE_NOEXCEPT
{
    return m_prefix_chars;
}

ARGPARSE_INL std::string const&
ArgumentParser::fromfile_prefix_chars() const ARGPARSE_NOEXCEPT
{
    return m_fromfile_prefix_chars;
}

ARGPARSE_INL std::string const&
ArgumentParser::comment_prefix_chars() const ARGPARSE_NOEXCEPT
{
    return m_comment_prefix_chars;
}

ARGPARSE_INL std::string const&
ArgumentParser::argument_default() const ARGPARSE_NOEXCEPT
{
    return m_argument_default.value();
}

ARGPARSE_INL std::string const&
ArgumentParser::conflict_handler() const ARGPARSE_NOEXCEPT
{
    return m_data->m_conflict_handler_str;
}

ARGPARSE_INL bool
ArgumentParser::add_help() const ARGPARSE_NOEXCEPT
{
    return m_data->m_add_help;
}

ARGPARSE_INL bool
ArgumentParser::allow_abbrev() const ARGPARSE_NOEXCEPT
{
    return m_allow_abbrev;
}

ARGPARSE_INL bool
ArgumentParser::exit_on_error() const ARGPARSE_NOEXCEPT
{
    return m_exit_on_error;
}

ARGPARSE_INL std::size_t
ArgumentParser::output_width() const
{
    return m_output_width != 0
            ? m_output_width : detail::_get_terminal_size().first;
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::initializer_list<std::string> const& flags)
{
    return add_argument(std::vector<std::string>{ flags });
}
#else
ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag)
{
    return add_argument(detail::_vector(flag));
}

ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag1,
        std::string const& flag2)
{
    return add_argument(detail::_vector(flag1, flag2));
}

ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3)
{
    return add_argument(detail::_vector(flag1, flag2, flag3));
}

ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3,
        std::string const& flag4)
{
    return add_argument(detail::_vector(flag1, flag2, flag3, flag4));
}
#endif  // C++11+

ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::vector<std::string> const& flags)
{
    m_data->create_argument(m_data, flags, prefix_chars());
    process_add_argument();
    return *m_data->m_arguments.back();
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::add_argument(
        Argument const& argument)
{
    m_data->validate_argument(Argument(argument), prefix_chars());
    process_add_argument();
    return *this;
}

ARGPARSE_INL ArgumentGroup&
ArgumentParser::add_argument_group(
        std::string const& title,
        std::string const& description)
{
    detail::shared_ptr<ArgumentGroup> group
            = ArgumentGroup::make_argument_group(
                title, description, m_prefix_chars, m_data, m_argument_default);
    m_groups.push_back(pGroup(group));
    return *group;
}

ARGPARSE_INL MutuallyExclusiveGroup&
ArgumentParser::add_mutually_exclusive_group(
        bool required)
{
    m_mutex_groups.push_back(
                MutuallyExclusiveGroup::make_mutex_group(
                    m_prefix_chars, m_data, m_argument_default));
    return m_mutex_groups.back().required(required);
}

ARGPARSE_INL SubParsers&
ArgumentParser::add_subparsers(
        std::string const& title,
        std::string const& description)
{
    if (has_subparsers()) {
        throw_error("cannot have multiple subparser arguments");
    }
    m_subparsers_position = m_data->m_positional.size();
    m_subparsers = SubParsers::make_subparsers(title, description);
    m_subparsers->update_prog(prog(), subparsers_prog_args());
    m_groups.push_back(pGroup(m_subparsers));
    return *m_subparsers;
}

ARGPARSE_INL SubParsers*
ArgumentParser::subparsers() const ARGPARSE_NOEXCEPT
{
    return m_subparsers.get();
}

ARGPARSE_INL bool
ArgumentParser::has_subparsers() const ARGPARSE_NOEXCEPT
{
    return m_subparsers.get();
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::handle(
        detail::func1<std::string const&>::type func) ARGPARSE_NOEXCEPT
{
    m_handle = ARGPARSE_MOVE(func);
    return *this;
}

ARGPARSE_INL ArgumentParser&
ArgumentParser::handle(
        detail::func1<Namespace const&>::type func) ARGPARSE_NOEXCEPT
{
    m_parse_handle = ARGPARSE_MOVE(func);
    return *this;
}

ARGPARSE_INL std::string
ArgumentParser::get_default(
        std::string const& dest) const
{
    pArguments const arguments = m_data->get_arguments(true);
    for (std::size_t i = 0; i < arguments.size(); ++i) {
        pArgument const& arg = arguments.at(i);
        if (arg->is_match_name(dest)) {
            return arg->m_default.suppress()
                    ? detail::_suppress : arg->m_default.value();
        }
    }
    for (std::size_t i = 0; i < m_default_values.size(); ++i) {
        if (m_default_values.at(i).first == dest) {
            return m_default_values.at(i).second;
        }
    }
    return std::string();
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL void
ArgumentParser::set_defaults(
       std::initializer_list<std::pair<std::string, std::string> > const& pairs)
{
    set_defaults(std::vector<std::pair<std::string, std::string> >{ pairs });
}
#endif  // C++11+

ARGPARSE_INL void
ArgumentParser::set_defaults(
        std::vector<std::pair<std::string, std::string> > const& pairs)
{
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        std::string const& dest = pairs.at(i).first;
        if (dest.empty()) {
            continue;
        }
        std::string const& value = pairs.at(i).second;
        if (!is_default_value_stored(m_data->m_arguments, dest, value)) {
            m_default_values.push_back(std::make_pair(dest, value));
        }
    }
}

ARGPARSE_INL Namespace
ArgumentParser::parse_args(
        Namespace const& space) const
{
    return parse_args(m_parsed_arguments, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_args(
        std::string const& args,
        Namespace const& space) const
{
    return parse_args(split_to_args(args), space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, false, false, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_args(
        Namespace const& space) const
{
    return parse_known_args(m_parsed_arguments, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_args(
        std::string const& args,
        Namespace const& space) const
{
    return parse_known_args(split_to_args(args), space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, true, false, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_intermixed_args(
        Namespace const& space) const
{
    return parse_intermixed_args(m_parsed_arguments, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_intermixed_args(
        std::string const& args,
        Namespace const& space) const
{
    return parse_intermixed_args(split_to_args(args), space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, false, true, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_intermixed_args(
        Namespace const& space) const
{
    return parse_known_intermixed_args(m_parsed_arguments, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_intermixed_args(
        std::string const& args,
        Namespace const& space) const
{
    return parse_known_intermixed_args(split_to_args(args), space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, true, true, space);
}

#ifdef ARGPARSE_CXX_11
ARGPARSE_INL Namespace
ArgumentParser::parse_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return parse_args(std::vector<std::string>{ args }, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return parse_known_args(std::vector<std::string>{ args }, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_intermixed_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return parse_intermixed_args(std::vector<std::string>{ args }, space);
}

ARGPARSE_INL Namespace
ArgumentParser::parse_known_intermixed_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return parse_known_intermixed_args(std::vector<std::string>{ args }, space);
}
#endif  // C++11+

#ifdef ARGPARSE_HAS_OPTIONAL
ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_args(
        Namespace const& space) const
{
    return try_parse_args(m_parsed_arguments, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_args(
        std::string const& args,
        Namespace const& space) const
{
    return try_parse_args(split_to_args(args), space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return try_parse_args(std::vector<std::string>{ args }, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, false, false, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_args(
        Namespace const& space) const
{
    return try_parse_known_args(m_parsed_arguments, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_args(
        std::string const& args,
        Namespace const& space) const
{
    return try_parse_known_args(split_to_args(args), space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return try_parse_known_args(std::vector<std::string>{ args }, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, true, false, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_intermixed_args(
        Namespace const& space) const
{
    return try_parse_intermixed_args(m_parsed_arguments, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_intermixed_args(
        std::string const& args,
        Namespace const& space) const
{
    return try_parse_intermixed_args(split_to_args(args), space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_intermixed_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return try_parse_intermixed_args(
                std::vector<std::string>{ args }, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, false, true, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_intermixed_args(
        Namespace const& space) const
{
    return try_parse_known_intermixed_args(m_parsed_arguments, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_intermixed_args(
        std::string const& args,
        Namespace const& space) const
{
    return try_parse_known_intermixed_args(split_to_args(args), space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_intermixed_args(
        std::initializer_list<std::string> const& args,
        Namespace const& space) const
{
    return try_parse_known_intermixed_args(
                std::vector<std::string>{ args }, space);
}

ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, true, true, space);
}
#endif  // ARGPARSE_HAS_OPTIONAL

ARGPARSE_INL bool
ArgumentParser::has_env(
        std::string const& name) const
{
    std::list<std::pair<std::string, std::string> >::const_iterator it
            = m_env_variables.begin();
    for ( ; it != m_env_variables.end() && it->first != name; ++it) {
    }
    return it != m_env_variables.end();
}

ARGPARSE_INL std::string
ArgumentParser::get_env(
        std::string const& name) const
{
    std::list<std::pair<std::string, std::string> >::const_iterator it
            = m_env_variables.begin();
    for ( ; it != m_env_variables.end() && it->first != name; ++it) {
    }
    return it != m_env_variables.end() ? it->second : std::string();
}

ARGPARSE_INL std::list<std::pair<std::string, std::string> > const&
ArgumentParser::list_env() const
{
    return m_env_variables;
}

ARGPARSE_INL bool
ArgumentParser::self_test(
        std::ostream& os) const
{
    return self_test(std::string(), os);
}

ARGPARSE_INL bool
ArgumentParser::self_test(
        std::string const& language,
        std::ostream& os) const
{
    std::string lang = !language.empty() ? language : default_language();
    std::size_t const limit = 79;
    char const filler = '-';
    std::stringstream ss;
    ss << "cpp-argparse v"
       << ARGPARSE_VERSION_MAJOR << "."
       << ARGPARSE_VERSION_MINOR << "."
       << ARGPARSE_VERSION_PATCH << " self-test report";
    os << detail::_filled_string(ss.str(), limit, filler) << "\n";
    os << detail::_filled_string(
              "This report contains parser information and displays "
              "diagnostic warnings", limit, detail::_space) << "\n";
    os << detail::_filled_string("overview", limit, filler) << "\n";
    test_overview(lang, os);
    os << detail::_filled_string("diagnostics", limit, filler) << "\n";
    bool res = test_diagnostics(lang, os);
    os << detail::_filled_string("end report", limit, filler) << std::endl;
    return res;
}

ARGPARSE_INL void
ArgumentParser::print_bash_completion(
        std::ostream& os) const
{
    os << format_bash_completion() << std::endl;
}

ARGPARSE_INL void
ArgumentParser::print_usage(
        std::ostream& os) const
{
    print_usage(std::string(), os);
}

ARGPARSE_INL void
ArgumentParser::print_usage(
        std::string const& language,
        std::ostream& os) const
{
    std::string res = format_usage(language);
    if (!res.empty()) {
        os << res << std::endl;
    }
}

ARGPARSE_INL void
ArgumentParser::print_help(
        std::ostream& os) const
{
    print_help(std::string(), os);
}

ARGPARSE_INL void
ArgumentParser::print_help(
        std::string const& language,
        std::ostream& os) const
{
    std::string res = format_help(language);
    if (!res.empty()) {
        os << res << std::endl;
    }
}

ARGPARSE_INL std::string
ArgumentParser::format_bash_completion() const
{
    return m_formatter->_format_bash_completion(this);
}

ARGPARSE_INL std::string
ArgumentParser::format_usage(
        std::string const& lang) const
{
    return m_formatter->_format_usage(this, lang);
}

ARGPARSE_INL std::string
ArgumentParser::format_help(
        std::string const& lang) const
{
    return m_formatter->_format_help(this, lang);
}

ARGPARSE_INL void
ArgumentParser::exit(
        int status,
        std::string const& message) const
{
    if (!message.empty()) {
        std::cerr << message << std::endl;
    }
    std::exit(status);
}

ARGPARSE_INL void
ArgumentParser::error(
        std::string const& message) const
{
    print_usage(std::cerr);
    exit(2, prog() + ": error: " + message);
}

ARGPARSE_INL std::vector<std::string>
ArgumentParser::convert_arg_line_to_args(
        std::string const& arg_line) const
{
    return detail::_vector(arg_line);
}

ARGPARSE_INL std::string
ArgumentParser::default_language() const
{
    std::string res;
    for (arg_iterator it = m_data->m_arguments.begin();
         it != m_data->m_arguments.end(); ++it) {
        if ((*it)->action() == argparse::language
                && (*it)->m_default.has_value()) {
            res = (*it)->m_default.value();
        }
    }
    return res;
}

ARGPARSE_INL
ArgumentParser::ParserInfo::ParserInfo(
        ArgumentParser const* parser,
        _Storage const& storage,
        SubParsersInfo const& subparsers,
        pArguments const& optional,
        pArguments const& operand)
    : parser(parser),
      optional(optional),
      operand(operand),
      storage(storage),
      subparsers(subparsers),
      lang(),
      have_negative_args()
{
    lang = parser->default_language();
    have_negative_args = detail::_negative_numbers_presented(
                optional, parser->prefix_chars());
}

ARGPARSE_INL
ArgumentParser::ParserInfo::ParserInfo(
        ParserInfo const& orig)
    : parser(orig.parser),
      optional(orig.optional),
      operand(orig.operand),
      storage(orig.storage),
      subparsers(orig.subparsers),
      lang(orig.lang),
      have_negative_args(orig.have_negative_args)
{ }

ARGPARSE_INL ArgumentParser::ParserInfo&
ArgumentParser::ParserInfo::operator =(
        ParserInfo const& rhs)
{
    if (this != &rhs) {
        parser              = rhs.parser;
        optional            = rhs.optional;
        operand             = rhs.operand;
        storage             = rhs.storage;
        subparsers          = rhs.subparsers;
        lang                = rhs.lang;
        have_negative_args  = rhs.have_negative_args;
    }
    return *this;
}

ARGPARSE_INL ArgumentParser::ParserInfo
ArgumentParser::parser_info(
        ArgumentParser const* parser,
        _Storage const& storage,
        SubParsersInfo const& subparsers)
{
    return ParserInfo(parser, storage, subparsers,
                      parser->m_data->get_optional(true, true),
                      parser->m_data->get_operand(true, true));
}

ARGPARSE_INL void
ArgumentParser::throw_error(
        std::string const& message,
        std::string const& lang,
        std::ostream& os) const
{
    print_usage(!lang.empty() ? lang : default_language(), os);
    throw std::logic_error(prog() + ": error: " + message);
}

ARGPARSE_INL std::string
ArgumentParser::despecify(
        std::string const& str) const
{
    return detail::_replace(str, "%(prog)s", prog());
}

ARGPARSE_INL void
ArgumentParser::process_add_argument()
{
    if (m_argument_default.has_value()
            && !m_data->m_arguments.back()->m_default.has_value()
            && !m_data->m_arguments.back()->m_default.suppress()) {
        m_data->m_arguments.back()->default_value(m_argument_default.value());
    }
    if (m_argument_default.suppress()) {
        m_data->m_arguments.back()->default_value(argparse::SUPPRESS);
    }
    switch (m_data->m_arguments.back()->m_type) {
        case Argument::Positional :
            m_data->m_positional.push_back(
                        std::make_pair(m_data->m_arguments.back(), false));
            break;
        case Argument::Operand :
            m_data->m_operand.push_back(
                        std::make_pair(m_data->m_arguments.back(), false));
            break;
        case Argument::Optional :
            m_data->m_optional.push_back(
                        std::make_pair(m_data->m_arguments.back(), false));
            break;
        default :
            break;
    }
}

ARGPARSE_INL Namespace
ArgumentParser::on_parse_arguments(
        std::vector<std::string> const& args,
        bool only_known,
        bool intermixed,
        Namespace const& space) const
{
    if (!m_exit_on_error) {
        return parse_arguments(args, only_known, intermixed, space);
    }
    try {
        return parse_arguments(args, only_known, intermixed, space);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "argparse error: unexpected error" << std::endl;
    }
    std::exit(1);
}

#ifdef ARGPARSE_HAS_OPTIONAL
ARGPARSE_INL std::optional<Namespace>
ArgumentParser::on_try_parse_arguments(
        std::vector<std::string> const& args,
        bool only_known,
        bool intermixed,
        Namespace const& space) const
{
    try {
        return parse_arguments(args, only_known, intermixed, space);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "argparse error: unexpected error" << std::endl;
    }
    return std::nullopt;
}
#endif  // ARGPARSE_HAS_OPTIONAL

ARGPARSE_INL std::vector<std::string>
ArgumentParser::read_args_from_file(
        std::vector<std::string> const& arguments) const
{
    std::vector<std::string> res = arguments;
    if (!fromfile_prefix_chars().empty()) {
        for (std::size_t i = 0; i < res.size(); ++i) {
            while (!res.at(i).empty()
                   && detail::_exists(res.at(i).at(0),
                                      fromfile_prefix_chars())) {
                std::string file = res.at(i).substr(1);
                std::ifstream is(file.c_str());
                if (!is.is_open()) {
                    throw_error("[Errno 2] No such file or directory: '"
                                + file + "'");
                }
                std::vector<std::string> args;
                std::string line;
                while (std::getline(is, line)) {
                    std::vector<std::string> line_args
                            = convert_arg_line_to_args(line);
                    detail::_move_insert_to_end(line_args, args);
                }
                is.close();
                detail::_move_replace_at(args, res, i);
            }
        }
    }
    if (!comment_prefix_chars().empty()) {
        // remove comments
        std::vector<std::string>::iterator first = res.begin();
        std::vector<std::string>::iterator it = first;
        while (first != res.end()) {
            if (first->empty()
                    || !detail::_exists(first->at(0), comment_prefix_chars())) {
                if (it != first) {
                    *it = ARGPARSE_MOVE(*first);
                }
                ++it;
            }
            ++first;
        }
        res.erase(it, res.end());
    }
    return res;
}

ARGPARSE_INL Namespace
ArgumentParser::parse_arguments(
        std::vector<std::string> const& in_args,
        bool only_known,
        bool intermixed,
        Namespace const& space) const
{
    handle(prog());
    check_namespace(space);
    std::vector<std::string> parsed_arguments = read_args_from_file(in_args);

    Parsers parsers;
    parsers.push_back(
                parser_info(this, space.storage(), subparsers_info(true)));

    check_mutex_arguments();
    detail::_check_intermixed_subparsers(
                intermixed, parsers.back().subparsers.first);

    pArguments positional = m_data->get_positional(true, true);
    check_intermixed_remainder(intermixed, positional);

    validate_arguments(m_data->get_arguments(true));
    parsers.back().storage.create(m_data->get_arguments(true));

    std::vector<std::string> unrecognized_args;
    std::list<std::string> intermixed_args;

    bool was_pseudo_arg = false;
    std::size_t pos = 0;
    for (std::size_t i = 0; i < parsed_arguments.size(); ++i) {
        if (parsed_arguments.at(i) == detail::_pseudo_arg && !was_pseudo_arg) {
            was_pseudo_arg = true;
            continue;
        }
        check_abbreviations(parsers, was_pseudo_arg, parsed_arguments, i);
        bool remainder = is_remainder_positional(pos, positional, parsers);
        std::string arg = parsed_arguments.at(i);
        std::vector<std::string> equals = detail::_process_split_equal(
                    arg, parsers.back().parser->prefix_chars());
        pArgument const tmp
                = get_optional_arg_by_flag(was_pseudo_arg, parsers.back(), arg);
        if (tmp && !remainder) {
            switch (tmp->action()) {
                case argparse::store :
                    parsers.front().storage.at(tmp).clear();
                    // fallthrough
                case argparse::append :
                case argparse::extend :
                    storage_optional_store(parsers, equals, parsed_arguments,
                                           i, was_pseudo_arg, arg, tmp);
                    break;
                case argparse::help :
                    process_optional_help(parsers.back(), equals, arg, tmp);
                    break;
                case argparse::version :
                    process_optional_version(parsers.back(), equals, arg, tmp);
                    break;
                case argparse::language :
                    parsers.front().storage.at(tmp).clear();
                    storage_optional_store(parsers, equals, parsed_arguments,
                                           i, was_pseudo_arg, arg, tmp);
                    parsers.back().lang
                            = parsers.front().storage.at(tmp).front();
                    break;
                default :
                    // argparse::store_const :
                    // argparse::store_true :
                    // argparse::store_false :
                    // argparse::append_const :
                    // argparse::count :
                    // argparse::BooleanOptionalAction :
                    storage_optional_store_const(parsers, equals, arg, tmp);
                    break;
            }
        } else if (!arg.empty()
                   && !remainder
                   && detail::_is_optional(
                       arg, parsers.back().parser->prefix_chars(),
                       parsers.back().have_negative_args, was_pseudo_arg)) {
            unrecognized_args.push_back(arg);
        } else {
            process_positional_args(parsed_arguments, i, parsers,
                                    was_pseudo_arg, intermixed, intermixed_args,
                                    pos, positional, unrecognized_args);
        }
    }
    if (!intermixed_args.empty()) {
        match_args_partial(parsers, pos, positional, unrecognized_args,
                           intermixed_args);
    }
    check_mutex_groups(parsers);
    check_required_args(parsers, pos, positional);
    check_unrecognized_args(only_known, unrecognized_args);
    default_values_post_trigger(parsers.front().storage);
    namespace_post_trigger(parsers, only_known, unrecognized_args);
    return create_namespace(only_known, ARGPARSE_MOVE(parsers.front().storage),
                            ARGPARSE_MOVE(unrecognized_args));
}

ARGPARSE_INL void
ArgumentParser::check_namespace(
        Namespace const& space) const
{
    if (space.m_unrecognized_args.has_value() && !m_data->m_arguments.empty()) {
        throw AttributeError("'tuple' object has no attribute '"
                             + m_data->m_arguments.front()->get_dest() + "'");
    }
}

ARGPARSE_INL void
ArgumentParser::check_mutex_arguments() const
{
    for (mtx_it i = m_mutex_groups.begin(); i != m_mutex_groups.end(); ++i) {
        for (arg_iterator j = (*i).m_data->m_arguments.begin();
             j != (*i).m_data->m_arguments.end(); ++j) {
            if ((*j)->required()) {
                throw
                ValueError("mutually exclusive arguments must be optional");
            }
        }
    }
}

ARGPARSE_INL void
ArgumentParser::check_intermixed_remainder(
        bool intermixed,
        pArguments const& positional)
{
    if (!intermixed) {
        return;
    }
    for (std::size_t i = 0; i < positional.size(); ++i) {
        if (positional.at(i)->m_nargs == Argument::REMAINDING) {
            throw TypeError(
                    "parse_intermixed_args: positional arg with nargs=...");
        }
    }
}

ARGPARSE_INL Namespace
ArgumentParser::create_namespace(
        bool only_known,
        detail::rval<_Storage>::type storage,
        detail::rval<std::vector<std::string> >::type unrecognized_args)
{
    return only_known ? Namespace(ARGPARSE_MOVE(storage),
                                  ARGPARSE_MOVE(unrecognized_args))
                      : Namespace(ARGPARSE_MOVE(storage));
}

ARGPARSE_INL void
ArgumentParser::validate_arguments(
        pArguments const& args)
{
    for (std::size_t i = 0; i < args.size(); ++i) {
        args.at(i)->validate();
    }
}

ARGPARSE_INL void
ArgumentParser::validate_argument_value(
        ArgumentParser const* parser,
        Argument const& arg,
        std::string const& value) const
{
    if (!(arg.m_nargs & (Argument::REMAINDING | Argument::SUPPRESSING))
            && arg.m_choices.has_value()) {
        std::string str = detail::_remove_quotes(value);
        if (!str.empty() && !detail::_exists(str, arg.m_choices.value())) {
            parser->throw_error(
                        "argument " + (arg.m_flags.empty()
                                       ? arg.dest() : arg.m_flags.front())
                        + ": invalid choice: '" + str + "' (choose from "
                       + detail::_join(arg.m_choices.value(), ", ", "'") + ")");
        }
    }
}

ARGPARSE_INL void
ArgumentParser::storage_have_value(
        Parsers& parsers,
        pArgument const& arg) const
{
    validate_argument_value(parsers.back().parser, *arg, std::string());
    parsers.front().storage.have_value(arg);
}

ARGPARSE_INL void
ArgumentParser::storage_store_value(
        Parsers& parsers,
        pArgument const& arg,
        std::string const& val) const
{
    validate_argument_value(parsers.back().parser, *arg, val);
    parsers.front().storage.store_value(arg, val);
}

ARGPARSE_INL void
ArgumentParser::storage_store_values(
        Parsers& parsers,
        pArgument const& arg,
        std::vector<std::string> const& values) const
{
    for (std::size_t i = 0; i < values.size(); ++i) {
        validate_argument_value(parsers.back().parser, *arg, values.at(i));
    }
    parsers.front().storage.store_values(arg, values);
}

ARGPARSE_INL void
ArgumentParser::storage_store_n_values(
        Parsers& parsers,
        pArgument const& arg,
        std::list<std::string>& arguments,
        std::size_t n) const
{
    if (n > arguments.size()) {
        throw ValueError("can't store " +  detail::_to_string(n) + " values");
    }
    std::vector<std::string> values;
    values.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        values.push_back(arguments.front());
        arguments.pop_front();
    }
    storage_store_values(parsers, arg, values);
}

ARGPARSE_INL void
ArgumentParser::storage_store_default_value(
        Parsers& parsers,
        pArgument const& arg) const
{
    parsers.front().storage.store_default_value(arg);
}

ARGPARSE_INL bool
ArgumentParser::storage_is_positional_arg_stored(
        Parsers& parsers,
        pArgument const& arg) const
{
    return parsers.front().storage.self_value_stored(arg);
}

ARGPARSE_INL void
ArgumentParser::storage_optional_store_func(
        Parsers& parsers,
        std::string const& arg,
        pArgument const& tmp,
        std::size_t n) const
{
    if (n == 0) {
        switch (tmp->m_nargs) {
            case Argument::NARGS_DEF :
            case Argument::NARGS_NUM :
            case Argument::ONE_OR_MORE :
                parsers.back().parser->throw_error(tmp->error_nargs(arg));
                break;
            case Argument::ZERO_OR_ONE :
                if (tmp->m_const.has_value()) {
                    if (tmp->action() == argparse::extend) {
                        if (tmp->const_value().empty()) {
                            storage_have_value(parsers, tmp);
                        } else {
                            std::vector<std::string> values;
                            values.reserve(tmp->const_value().size());
                            for (std::size_t i = 0;
                                 i < tmp->const_value().size(); ++i) {
                                values.push_back(
                                      std::string(1, tmp->const_value().at(i)));
                            }
                            storage_store_values(parsers, tmp, values);
                        }
                    } else {
                        storage_store_value(parsers, tmp, tmp->const_value());
                    }
                } else if (tmp->action() == argparse::extend) {
                    throw TypeError("'NoneType' object is not iterable");
                } else {
                    storage_have_value(parsers, tmp);
                }
                break;
            case Argument::SUPPRESSING :
                storage_store_default_value(parsers, tmp);
                break;
            case Argument::ZERO_OR_MORE :
            case Argument::REMAINDING :
            default :
                storage_have_value(parsers, tmp);
                break;
        }
    } else if ((tmp->m_nargs & Argument::_NARGS_COMBINED)
               && (n < tmp->m_num_args
                   || (tmp->m_nargs != Argument::NARGS_NUM
                       && tmp->m_num_args > 1 && n % tmp->m_num_args != 0))) {
        parsers.back().parser->throw_error(tmp->error_nargs(arg));
    }
}

ARGPARSE_INL void
ArgumentParser::storage_optional_store(
        Parsers& parsers,
        std::vector<std::string> const& equals,
        std::vector<std::string> const& args,
        std::size_t& i,
        bool was_pseudo_arg,
        std::string const& arg,
        pArgument const& tmp) const
{
    if (equals.size() == 1) {
        if (tmp->m_type == Argument::Operand) {
            parsers.back().parser->throw_error(
                        "argument " + arg + " is operand: expected A=...");
        }
        std::size_t n = 0;
        std::vector<std::string> values;
        bool read_next = true;
        do {
            if (++i == args.size()) {
                storage_optional_store_func(parsers, arg, tmp, n);
                break;
            } else {
                std::string const& next = args.at(i);
                if (tmp->m_nargs != Argument::SUPPRESSING
                        && (tmp->m_nargs == Argument::REMAINDING
                            || (detail::_is_not_operand(
                                   was_pseudo_arg, parsers.back().operand, next)
                                && detail::_not_optional(
                                    next,
                                    parsers.back().parser->prefix_chars(),
                                    parsers.back().have_negative_args,
                                    was_pseudo_arg)))) {
                    values.push_back(next);
                    ++n;
                } else {
                    --i;
                    storage_optional_store_func(parsers, arg, tmp, n);
                    break;
                }
            }
            switch (tmp->m_nargs) {
                case Argument::NARGS_DEF :
                    read_next = false;
                    break;
                case Argument::NARGS_NUM :
                case Argument::ZERO_OR_ONE :
                    read_next = tmp->m_num_args != n;
                    break;
                case Argument::ONE_OR_MORE :
                case Argument::ZERO_OR_MORE :
                case Argument::REMAINDING :
                case Argument::SUPPRESSING :
                    read_next = true;
                    break;
                default :
                    read_next = false;
                    break;
            }
        } while (read_next);
        if (!values.empty()) {
            storage_store_values(parsers, tmp, values);
        }
    } else {
        if (tmp->m_nargs == Argument::SUPPRESSING) {
            parsers.back().parser->throw_error(
                       detail::_ignore_explicit(equals.front(), equals.back()));
        }
        if (tmp->m_nargs != Argument::NARGS_DEF && tmp->m_num_args > 1) {
            parsers.back().parser->throw_error(tmp->error_nargs(arg));
        }
        storage_store_value(parsers, tmp, equals.back());
    }
}

ARGPARSE_INL void
ArgumentParser::storage_optional_store_const(
        Parsers& parsers,
        std::vector<std::string> const& equals,
        std::string const& arg,
        pArgument const& tmp) const
{
    if (equals.size() == 1) {
        if (tmp->action() == argparse::BooleanOptionalAction) {
            bool exist = detail::_exists(equals.front(), tmp->m_flags);
            storage_store_value(parsers, tmp,
                                exist ? tmp->const_value() : std::string());
        } else {
            parsers.front().storage.self_value_stored(tmp);
        }
    } else {
        parsers.back().parser->throw_error(
                    detail::_ignore_explicit(arg, equals.back()));
    }
}

ARGPARSE_INL void
ArgumentParser::process_optional_help(
        ParserInfo const& info,
        std::vector<std::string> const& equals,
        std::string const& arg,
        pArgument const& tmp) const
{
    if (equals.size() != 1) {
        info.parser->throw_error(
                    detail::_ignore_explicit(arg, equals.back()), info.lang);
    }
    // print help and exit
    tmp->handle(std::string());
    info.parser->print_help(info.lang);
    exit(0);
}

ARGPARSE_INL void
ArgumentParser::process_optional_version(
        ParserInfo const& info,
        std::vector<std::string> const& equals,
        std::string const& arg,
        pArgument const& tmp) const
{
    if (equals.size() != 1) {
        info.parser->throw_error(
                    detail::_ignore_explicit(arg, equals.back()), info.lang);
    }
    if (!tmp->m_version.has_value()) {
        throw
        AttributeError("'ArgumentParser' object has no attribute 'version'");
    }
    // print version and exit
    tmp->handle(std::string());
    std::cout << despecify(tmp->version()) << std::endl;
    exit(0);
}

ARGPARSE_INL void
ArgumentParser::match_positional_minimum(
        Parsers& parsers,
        std::list<std::string>& arguments,
        pArgument const& arg) const
{
    if (storage_is_positional_arg_stored(parsers, arg)) {
        return;
    }
    if (arg->action() == argparse::BooleanOptionalAction
            || arg->m_nargs == Argument::SUPPRESSING) {
        storage_store_default_value(parsers, arg);
        return;
    }
    if (arg->action() == argparse::store) {
        parsers.front().storage.at(arg).clear();
    }
    switch (arg->m_nargs) {
        case Argument::NARGS_DEF :
            storage_store_value(parsers, arg, arguments.front());
            arguments.pop_front();
            break;
        case Argument::ONE_OR_MORE :
        case Argument::NARGS_NUM :
            storage_store_n_values(parsers, arg, arguments, arg->m_num_args);
            break;
        case Argument::ZERO_OR_ONE :
        case Argument::ZERO_OR_MORE :
            storage_store_default_value(parsers, arg);
            break;
        case Argument::REMAINDING :
        case Argument::SUPPRESSING :
        default :
            break;
    }
}

ARGPARSE_INL void
ArgumentParser::match_positional_more_zero(
        Parsers& parsers,
        std::list<std::string>& arguments,
        pArgument const& arg,
        std::size_t& over_args) const
{
    if (storage_is_positional_arg_stored(parsers, arg)) {
        return;
    }
    if (arg->action() == argparse::BooleanOptionalAction
            || arg->m_nargs == Argument::SUPPRESSING) {
        storage_store_default_value(parsers, arg);
        return;
    }
    if (arg->action() == argparse::store) {
        parsers.front().storage.at(arg).clear();
    }
    switch (arg->m_nargs) {
        case Argument::NARGS_DEF :
            storage_store_value(parsers, arg, arguments.front());
            arguments.pop_front();
            break;
        case Argument::NARGS_NUM :
            storage_store_n_values(parsers, arg, arguments, arg->m_num_args);
            break;
        case Argument::ONE_OR_MORE :
            storage_store_n_values(
                        parsers, arg, arguments, arg->m_num_args + over_args);
            over_args = 0;
            break;
        case Argument::ZERO_OR_ONE :
            storage_store_default_value(parsers, arg);
            break;
        case Argument::ZERO_OR_MORE :
        case Argument::REMAINDING :
            if (over_args > 0) {
                storage_store_n_values(parsers, arg, arguments, over_args);
                over_args = 0;
            } else if (arg->m_nargs == Argument::ZERO_OR_MORE) {
                storage_store_default_value(parsers, arg);
            }
            break;
        case Argument::SUPPRESSING :
        default :
            break;
    }
}

ARGPARSE_INL void
ArgumentParser::match_positional_optional(
        Parsers& parsers,
        std::list<std::string>& arguments,
        pArgument const& arg,
        std::size_t& over_args,
        std::size_t one_args) const
{
    if (storage_is_positional_arg_stored(parsers, arg)) {
        return;
    }
    if (arg->action() == argparse::BooleanOptionalAction
            || arg->m_nargs == Argument::SUPPRESSING) {
        storage_store_default_value(parsers, arg);
        return;
    }
    if (arg->action() == argparse::store) {
        parsers.front().storage.at(arg).clear();
    }
    switch (arg->m_nargs) {
        case Argument::NARGS_DEF :
            storage_store_value(parsers, arg, arguments.front());
            arguments.pop_front();
            break;
        case Argument::NARGS_NUM :
            storage_store_n_values(parsers, arg, arguments, arg->m_num_args);
            break;
        case Argument::ZERO_OR_ONE :
            if (over_args + arg->m_num_args <= one_args) {
                storage_store_n_values(
                            parsers, arg, arguments, arg->m_num_args);
                over_args += arg->m_num_args;
            } else {
                storage_store_default_value(parsers, arg);
            }
            break;
        case Argument::ONE_OR_MORE :
        case Argument::ZERO_OR_MORE :
        case Argument::REMAINDING :
        case Argument::SUPPRESSING :
        default :
            break;
    }
}

ARGPARSE_INL void
ArgumentParser::match_positional_default(
        Parsers& parsers,
        std::list<std::string>& arguments,
        pArgument const& arg) const
{
    if (storage_is_positional_arg_stored(parsers, arg)) {
        return;
    }
    if (arg->action() == argparse::BooleanOptionalAction
            || arg->m_nargs == Argument::SUPPRESSING) {
        storage_store_default_value(parsers, arg);
        return;
    }
    if (arg->action() == argparse::store) {
        parsers.front().storage.at(arg).clear();
    }
    if (arg->m_nargs == Argument::NARGS_DEF) {
        storage_store_value(parsers, arg, arguments.front());
        arguments.pop_front();
    } else {
        storage_store_n_values(parsers, arg, arguments, arg->m_num_args);
    }
}

ARGPARSE_INL void
ArgumentParser::match_positionals(
        Parsers& parsers,
        std::size_t& pos,
        pArguments const& positional,
        std::list<std::string>& arguments,
        std::size_t finish,
        std::size_t min_args,
        std::size_t one_args,
        bool more_args) const
{
    if (finish == pos) {
        return;
    }
    if (min_args == arguments.size()) {
        for ( ; pos < finish; ++pos) {
            match_positional_minimum(parsers, arguments, positional.at(pos));
        }
    } else if (more_args) {
        std::size_t over_args = arguments.size() - min_args;
        for ( ; pos < finish; ++pos) {
            match_positional_more_zero(parsers, arguments, positional.at(pos),
                                       over_args);
        }
    } else if (min_args + one_args >= arguments.size()) {
        std::size_t over_args = min_args + one_args - arguments.size();
        for ( ; pos < finish; ++pos) {
            match_positional_optional(parsers, arguments, positional.at(pos),
                                      over_args, one_args);
        }
    } else {
        for ( ; pos < finish; ++pos) {
            match_positional_default(parsers, arguments, positional.at(pos));
        }
    }
}

ARGPARSE_INL bool
ArgumentParser::finish_analyze_positional(
        pArgument const& arg,
        std::list<std::string> const& args,
        std::size_t& one_args,
        bool& more_args,
        std::size_t& min_args,
        bool first,
        bool read_all_args)
{
    if (!(arg->action() & detail::_store_action)) {
        return false;
    }
    std::size_t min_amount = 0;
    switch (arg->m_nargs) {
        case Argument::NARGS_DEF :
        case Argument::NARGS_NUM :
            min_amount += arg->m_num_args;
            break;
        case Argument::ZERO_OR_ONE :
            one_args += arg->m_num_args;
            break;
        case Argument::ONE_OR_MORE :
            min_amount += arg->m_num_args;
            // fallthrough
        case Argument::ZERO_OR_MORE :
            more_args = true;
            break;
        case Argument::REMAINDING :
            more_args = true;
            break;
        case Argument::SUPPRESSING :
        default :
            break;
    }
    if (min_args + min_amount > args.size()) {
        return true;
    }
    min_args += min_amount;
    if (arg->m_nargs == Argument::REMAINDING && !first && !read_all_args) {
        return true;
    }
    return false;
}

ARGPARSE_INL void
ArgumentParser::match_args_partial(
        Parsers& parsers,
        std::size_t& pos,
        pArguments const& positional,
        std::vector<std::string>& unrecognized_args,
        std::list<std::string>& args,
        bool read_all_args) const
{
    if (pos < positional.size()) {
        std::size_t finish = pos;
        std::size_t min_args = 0;
        std::size_t one_args = 0;
        bool more_args = false;
        for ( ; finish < positional.size()
              && !finish_analyze_positional(positional.at(finish), args,
                                            one_args, more_args, min_args,
                                            finish == pos, read_all_args);
              ++finish) {
        }
        match_positionals(parsers, pos, positional, args,
                          finish, min_args, one_args, more_args);
    }
    if (!args.empty()) {
        detail::_move_insert_to_end(args, unrecognized_args);
    }
}

ARGPARSE_INL bool
ArgumentParser::try_capture_parser(
        Parsers& parsers,
        std::size_t& pos,
        pArguments& positional,
        std::vector<std::string>& unrecognized_args,
        std::list<std::string>& args,
        bool read_all_args) const
{
    std::size_t finish = pos;
    std::size_t min_args = 0;
    std::size_t one_args = 0;
    bool more_args = false;
    bool capture_need = false;
    for ( ; finish < positional.size(); ++finish) {
        if (finish == parsers.back().subparsers.second) {
            if (min_args + 1 > args.size()) {
                break;
            }
            capture_need = true;
            break;
        }
        if (finish_analyze_positional(positional.at(finish), args, one_args,
                                      more_args, min_args,
                                      finish == pos, read_all_args)) {
            break;
        }
    }
    if (!capture_need
            && (finish != positional.size() || min_args >= args.size())) {
        match_positionals(parsers, pos, positional, args,
                          finish, min_args, one_args, more_args);
        detail::_insert_to_end(args, unrecognized_args);
        return false;
    }
    match_positionals(parsers, pos, positional, args, finish,
                      ++min_args, one_args, more_args);
    std::string const& name = args.front();
    std::string const& dest = parsers.back().subparsers.first->dest();
    std::string choices;
    std::list<pParser> const lst_parsers
            = parsers.back().subparsers.first->list_parsers();
    for (prs_iterator it = lst_parsers.begin(); it != lst_parsers.end(); ++it) {
        detail::_append_value_to("'" + (*it)->m_name + "'", choices, ", ");
        for (std::size_t a = 0; a < (*it)->aliases().size(); ++a) {
            std::string const& alias = (*it)->aliases().at(a);
            detail::_append_value_to("'" + alias + "'", choices, ", ");
        }
        if ((*it)->m_name == name || detail::_exists(name, (*it)->aliases())) {
            std::string const& lang = parsers.back().lang;
            parsers.push_back(parser_info((*it).get(), _Storage(),
                                          (*it)->subparsers_info(true, pos)));
            if (!lang.empty()) {
                parsers.back().lang = lang;
            }
            parsers.back().parser->handle(parsers.back().parser->m_name);
            validate_arguments((*it)->m_data->get_arguments(true));
            if (!dest.empty()) {
                pArgument subparsers_arg = Argument::make_argument(
                            detail::_vector(dest), dest, Argument::Positional);
                for (pi_iterator j = parsers.begin(); j != parsers.end(); ++j) {
                    (*j).storage.create(subparsers_arg);
                }
                parsers.front().storage.at(subparsers_arg).push_back(name);
            }
            for (pi_iterator j = parsers.begin(); j != parsers.end(); ++j) {
                (*j).storage.create(
                            parsers.back().parser->m_data->get_arguments(true));
            }
            pArguments sub_positional
                    = parsers.back().parser->m_data->get_positional(true, true);
            detail::_move_insert_to(sub_positional, positional, pos);
            args.pop_front();
            return true;
        }
    }
    throw_error("argument " + parsers.back().subparsers.first->flags_to_string()
            + ": invalid choice: '" + name + "' (choose from " + choices + ")");
    return false;
}

ARGPARSE_INL void
ArgumentParser::check_abbreviations(
        Parsers const& parsers,
        bool was_pseudo_arg,
        std::vector<std::string>& arguments,
        std::size_t i) const
{
    std::string const& arg = arguments.at(i);
    if (!arg.empty() && !parsers.front().storage.exists(arg)
            && detail::_is_optional(arg,
                                    parsers.back().parser->prefix_chars(),
                                    parsers.back().have_negative_args,
                                    was_pseudo_arg)) {
        pArguments const& options = parsers.back().optional;
        std::vector<std::string> temp;
        if (m_allow_abbrev) {
            bool is_flag_added = false;
            std::string args;
            std::vector<std::string> keys;
            keys.reserve(4);
            for (std::size_t j = 0; j < options.size(); ++j) {
                pArgument const& opt = options.at(j);
                for (std::size_t k = 0; k < opt->flags().size(); ++k) {
                    std::string const& flag = opt->flags().at(k);
                    if (detail::_starts_with(flag, arg)) {
                        is_flag_added = true;
                        keys.push_back(flag);
                        detail::_append_value_to(detail::_spaces + flag,
                                                 args, ",");
                        break;
                    }
                    if (detail::_utf8_length(flag).second == 2
                            && detail::_starts_with(arg, flag)) {
                        keys.push_back(arg);
                        detail::_append_value_to(detail::_spaces + flag,
                                                 args, ",");
                        break;
                    }
                }
            }
            if (keys.size() > 1) {
                parsers.back().parser->throw_error(
                          "ambiguous option: '" + arg + "' could match" + args);
            }
            std::string const& flag = keys.empty() ? arg : keys.front();
            if (is_flag_added) {
                temp.push_back(flag);
            } else {
                separate_arg_abbrev(parsers.back(), temp, arg,
                                    detail::_flag_name(flag), options);
            }
        } else {
            separate_arg_abbrev(parsers.back(), temp, arg,
                                detail::_flag_name(arg), options);
        }
        detail::_move_replace_at(temp, arguments, i);
    }
}

ARGPARSE_INL bool
ArgumentParser::is_remainder_positional(
        std::size_t pos,
        pArguments const& positional,
        Parsers const& parsers)
{
    return pos < positional.size()
            && positional.at(pos)->m_nargs == Argument::REMAINDING
            && !parsers.front().storage.at(positional.at(pos)).empty();
}

ARGPARSE_INL ArgumentParser::pArgument const
ArgumentParser::get_optional_arg_by_flag(
        bool was_pseudo_arg,
        ParserInfo const& info,
        std::string const& key)
{
    if (was_pseudo_arg) {
        return ARGPARSE_NULLPTR;
    }
    pArgument const opt = detail::_find_arg_by_flag(info.optional, key);
    return opt ? opt : detail::_find_arg_by_flag(info.operand, key);
}

ARGPARSE_INL void
ArgumentParser::separate_arg_abbrev(
        ParserInfo const& info,
        std::vector<std::string>& temp,
        std::string const& arg,
        std::string const& name,
        pArguments const& args) const
{
    if (name.size() + 1 == arg.size()) {
        std::vector<std::string> const split
                = detail::_split_equal(arg, info.parser->prefix_chars());
        if (split.size() == 2 && !split.front().empty()
                && detail::_find_arg_by_flag(info.optional, split.front())) {
            temp.push_back(arg);
            return;
        }
        std::vector<std::string> flags;
        flags.reserve(8);
        for (std::size_t i = 0; i < name.size(); ) {
            std::size_t cp_size = detail::_utf8_codepoint_size(
                        detail::_char_to_u8(name.at(i)));
            if (cp_size == 0) {
                // invalid string
                cp_size = 1;
            }
            if (!detail::_process_separate_arg_abbrev(
                        name, i, cp_size, flags, arg, args)) {
                break;
            }
            i += cp_size;
        }
        detail::_move_insert_to_end(flags, temp);
    } else {
        temp.push_back(arg);
    }
}

ARGPARSE_INL void
ArgumentParser::process_positional_args(
        std::vector<std::string> const& parsed_arguments,
        std::size_t& i,
        Parsers& parsers,
        bool was_pseudo_arg,
        bool intermixed,
        std::list<std::string>& intermixed_args,
        std::size_t& pos,
        pArguments& positional,
        std::vector<std::string>& unrecognized_args) const
{
    std::list<std::string> args;
    args.push_back(parsed_arguments.at(i));
    bool remainder = pos < positional.size()
            && positional.at(pos)->m_nargs == Argument::REMAINDING;
    while (true) {
        if (++i == parsed_arguments.size()) {
            break;
        } else {
            std::string const& next = parsed_arguments.at(i);
            if (remainder || (detail::_is_not_operand(
                                  was_pseudo_arg, parsers.back().operand, next)
                              && detail::_not_optional(
                                  next,
                                  parsers.back().parser->prefix_chars(),
                                  parsers.back().have_negative_args,
                                  was_pseudo_arg))) {
                args.push_back(next);
            } else {
                --i;
                break;
            }
        }
    }
    if (intermixed) {
        detail::_insert_to_end(args, intermixed_args);
    } else {
        if (parsers.back().subparsers.first) {
            if (try_capture_parser(parsers, pos, positional, unrecognized_args,
                                   args, i == parsed_arguments.size())) {
                i -= (i == parsed_arguments.size());
                i -= args.size();
            }
        } else {
            match_args_partial(parsers, pos, positional, unrecognized_args,
                               args, i == parsed_arguments.size());
        }
    }
}

ARGPARSE_INL void
ArgumentParser::check_mutex_groups(
        Parsers const& parsers)
{
    for (Parsers::const_iterator i = parsers.begin(); i != parsers.end(); ++i) {
        for (mtx_it j = (*i).parser->m_mutex_groups.begin();
             j != (*i).parser->m_mutex_groups.end(); ++j) {
            std::string args;
            std::string found;
            for (arg_iterator k = (*j).m_data->m_arguments.begin();
                 k != (*j).m_data->m_arguments.end(); ++k) {
                std::string flags = detail::_join((*k)->flags(), "/");
                args += detail::_spaces + flags;
                if (!parsers.front().storage.at(*k).empty()) {
                    if (!found.empty()) {
                        (*i).parser->throw_error("argument " + flags + ": not "
                                            + "allowed with argument " + found);
                    }
                    found = ARGPARSE_MOVE(flags);
                }
            }
            if ((*j).required() && found.empty()) {
                if ((*j).m_data->m_arguments.empty()) {
                    throw IndexError("list index out of range");
                }
                (*i).parser->throw_error(
                            "one of the arguments" + args + " is required");
            }
        }
    }
}

ARGPARSE_INL void
ArgumentParser::process_required_arguments(
        std::vector<std::string>& required_args,
        pArguments const& arguments,
        _Storage const& storage)
{
    for (std::size_t i = 0; i < arguments.size(); ++i) {
        pArgument const& arg = arguments.at(i);
        if (arg->required() && storage.at(arg).empty()) {
            required_args.push_back(detail::_join(arg->flags(), "/"));
        }
    }
}

ARGPARSE_INL void
ArgumentParser::process_required_subparsers(
        bool required,
        std::size_t pos,
        SubParsersInfo const& info,
        std::string& res)
{
    if (required && pos == info.second) {
        if (!info.first->dest().empty()) {
            detail::_append_value_to(info.first->dest(), res, ", ");
        } else {
            detail::_append_value_to(info.first->flags_to_string(), res, ", ");
        }
    }
}

ARGPARSE_INL void
ArgumentParser::process_required_check(
        ParserInfo const& info,
        _Storage const& storage)
{
    std::vector<std::string> required;
    process_required_arguments(required, info.optional, storage);
    process_required_arguments(required, info.operand, storage);
    if (!required.empty()) {
        info.parser->throw_error("the following arguments are required: "
                                 + detail::_join(required, ", "));
    }
}

ARGPARSE_INL bool
ArgumentParser::skip_positional_required_check(
        Parsers& parsers,
        pArgument const& arg) const
{
    if (storage_is_positional_arg_stored(parsers, arg)) {
        return true;
    }
    if (arg->action() == argparse::extend
            && arg->m_nargs == Argument::ZERO_OR_ONE) {
        throw TypeError("'NoneType' object is not iterable");
    }
    if ((arg->m_nargs & (Argument::ZERO_OR_ONE | Argument::ZERO_OR_MORE
                         | Argument::SUPPRESSING))
            || arg->action() == argparse::BooleanOptionalAction) {
        storage_store_default_value(parsers, arg);
        return true;
    }
    if (arg->m_nargs == Argument::REMAINDING) {
        return true;
    }
    return false;
}

ARGPARSE_INL void
ArgumentParser::check_required_args(
        Parsers& parsers,
        std::size_t& pos,
        pArguments const& positional) const
{
    std::list<ParserInfo>::reverse_iterator it = parsers.rbegin();
    std::vector<std::string> required;
    process_required_arguments(required, it->optional, parsers.front().storage);
    process_required_arguments(required, it->operand, parsers.front().storage);
    SubParsersInfo const& info = it->subparsers;
    bool sub_required = info.first && info.first->required();
    if (!required.empty() || pos < positional.size() || sub_required) {
        std::string args;
        for ( ; pos < positional.size(); ++pos) {
            process_required_subparsers(sub_required, pos, info, args);
            pArgument const& arg = positional.at(pos);
            if (args.empty() && skip_positional_required_check(parsers, arg)) {
                continue;
            }
            detail::_append_value_to(arg->m_flags.front(), args, ", ");
        }
        process_required_subparsers(sub_required, pos, info, args);
        for (std::size_t i = 0; i < required.size(); ++i) {
            detail::_append_value_to(required.at(i), args, ", ");
        }
        if (!args.empty()) {
            it->parser->throw_error(
                        "the following arguments are required: " + args);
        }
    }
    while (++it != parsers.rend()) {
        process_required_check(*it, parsers.front().storage);
    }
}

ARGPARSE_INL void
ArgumentParser::check_unrecognized_args(
        bool only_known,
        std::vector<std::string> const& args) const
{
    if (!only_known && !args.empty()) {
        throw_error("unrecognized arguments: " + detail::_join(args));
    }
}

ARGPARSE_INL void
ArgumentParser::default_values_post_trigger(
        _Storage& storage) const
{
    for (_Storage::iterator it = storage.begin(); it != storage.end(); ) {
        if (!it->second.exists()) {
            if (it->first->m_default.suppress()) {
                it = storage.erase(it);
                continue;
            }
            if (it->first->action() != argparse::count
                    && (it->first->m_type
                        & (Argument::Optional | Argument::Operand))) {
                detail::SValue<std::string> const& dv = it->first->m_default;
                if (dv.has_value()
                        || (it->first->action() & detail::_bool_action)) {
                    it->second.push_back(dv.value(), dv.has_value());
                    storage.on_process_store(it->first, dv.value());
                }
            }
        }
        ++it;
    }
    for (std::size_t i = 0; i < m_default_values.size(); ++i) {
        std::pair<std::string, std::string> const& pair
                = m_default_values.at(i);
        if (!storage.exists_arg(pair.first)) {
            pArgument arg = Argument::make_argument(
                        detail::_vector(pair.first),
                        pair.first, Argument::Positional);
            arg->default_value(pair.second);
            storage.create(arg, _Storage::mapped_type(
                               detail::_vector(pair.second)));
        }
    }
}

ARGPARSE_INL void
ArgumentParser::namespace_post_trigger(
        Parsers& parsers,
        bool only_known,
        std::vector<std::string> const& unrecognized_args)
{
    parsers.front().parser->parse_handle(
                only_known, parsers.front().storage, unrecognized_args);
    pi_iterator p = parsers.begin();
    _Storage const* storage = &(*p).storage;
    for (++p; p != parsers.end(); ++p) {
        _Storage& current = (*p).storage;
        for (_Storage::iterator it = current.begin(); it != current.end(); ) {
            if (storage->exists(it->first)) {
                it->second = storage->at(it->first);
                ++it;
            } else {
                it = current.erase(it);
            }
        }
        (*p).parser->parse_handle(only_known, current, unrecognized_args);
        storage = &(*p).storage;
    }
}

ARGPARSE_INL bool
ArgumentParser::is_default_value_stored(
        std::list<pArgument>& arguments,
        std::string const& dest,
        std::string const& value)
{
    for (arg_iterator it = arguments.begin(); it != arguments.end(); ++it) {
        if ((*it)->is_match_name(dest)) {
            (*it)->default_value(value);
            return true;
        }
    }
    return false;
}

ARGPARSE_INL std::size_t
ArgumentParser::argument_help_limit() const
{
    std::size_t width = output_width();
    std::size_t const _name_maxwidth = 24;
    if (width >= 2 * _name_maxwidth) {
        return width - _name_maxwidth;
    }
    std::size_t const _help_minwidth = 22;
    if (width >= 2 * _help_minwidth) {
        return width >> 1;
    }
    return _help_minwidth;
}

ARGPARSE_INL ArgumentParser::SubParsersInfo
ArgumentParser::subparsers_info(
        bool add_suppress,
        std::size_t offset) const
{
    typedef std::list<std::pair<pArgument, bool> >::const_iterator sub_iterator;
    SubParsersInfo res = std::make_pair(m_subparsers, offset);
    std::size_t i = 0;
    for (sub_iterator it = m_data->m_positional.begin();
         it != m_data->m_positional.end() && i < m_subparsers_position;
         ++it, ++i) {
        res.second += (add_suppress || !(*it).first->m_help.suppress());
    }
    return res;
}

ARGPARSE_INL std::string
ArgumentParser::subparsers_prog_args() const
{
    std::string res;
    bool add_suppress = false;
    SubParsersInfo info = subparsers_info(add_suppress);
    pArguments args = m_data->get_positional(add_suppress, true);
    for (std::size_t i = 0; i < args.size() && i != info.second; ++i) {
        detail::_append_value_to(args.at(i)->usage(*m_formatter), res);
    }
    return res;
}

ARGPARSE_INL void
ArgumentParser::print_subparsers(
        bool need_print,
        SubParsersInfo const& info,
        std::size_t index,
        HelpFormatter const& formatter,
        std::string const& prog,
        std::size_t size,
        std::size_t width,
        std::string const& lang,
        std::ostream& os)
{
    if (need_print && info.second == index) {
        info.first->print_parser_group(
                os, formatter, prog, info.first->required(), size, width, lang);
        for (SubParsers::pgr_iterator it = info.first->m_groups.begin();
             it != info.first->m_groups.end(); ++it) {
            (*it)->print_parser_group(
                os, formatter, prog, info.first->required(), size, width, lang);
        }
    }
}

ARGPARSE_INL void
ArgumentParser::update_prog(
        std::string const& parent_prog)
{
    m_prog = parent_prog + detail::_spaces + m_name;
}

ARGPARSE_INL void
ArgumentParser::test_overview(
        std::string const& lang,
        std::ostream& os) const
{
    if (lang.empty()) {
        os << "default language\n";
    } else {
        os << "language: " << lang << "\n";
    }
    os << "prog: " << prog() << "\n";
    for (arg_iterator it = m_data->m_arguments.begin();
         it != m_data->m_arguments.end(); ++it) {
        if ((*it)->action() == argparse::version) {
            os << "version: " << (*it)->version() << "\n";
        }
    }
    if (!m_usage.suppress()) {
        std::string tr_usage = detail::_tr(m_usage.value(), lang);
        if (!tr_usage.empty()) {
            os << "usage: " << despecify(tr_usage) << "\n";
        }
    }
    std::string tr_description = detail::_tr(m_description, lang);
    if (!tr_description.empty()) {
        os << "description: " << despecify(tr_description) << "\n";
    }
    std::string tr_epilog = detail::_tr(m_epilog, lang);
    if (!tr_epilog.empty()) {
        os << "epilog: " << despecify(tr_epilog) << "\n";
    }
    os << "prefix_chars: '" << prefix_chars() << "'\n";
    if (!fromfile_prefix_chars().empty()) {
        os << "fromfile_prefix_chars: '" << fromfile_prefix_chars() << "'\n";
    }
    if (!comment_prefix_chars().empty()) {
        os << "comment_prefix_chars: '" << comment_prefix_chars() << "'\n";
    }
    if (m_argument_default.has_value()) {
        os << "argument_default: " << argument_default() << "\n";
    }
    if (!conflict_handler().empty()) {
        os << "conflict_handler: " << conflict_handler() << "\n";
    }
    os << "add_help: " << detail::_bool_to_string(add_help()) << "\n";
    os << "allow_abbrev: " << detail::_bool_to_string(allow_abbrev()) << "\n";
    os << "exit_on_error: " << detail::_bool_to_string(exit_on_error()) << "\n";
    if (m_output_width != 0) {
        os << "output_width [override]: " << output_width() << "\n";
    } else {
#ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
        os << "output_width [detected]: " << output_width() << "\n";
#else
        os << "output_width [default]: " << output_width() << "\n";
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
    }
    overview_subparsers(this, std::string(), os);
}

ARGPARSE_INL void
ArgumentParser::overview_subparsers(
        ArgumentParser const* parser,
        std::string const& indent,
        std::ostream& os)
{
    if (parser->has_subparsers()) {
        std::list<pParser> const parsers = parser->m_subparsers->list_parsers();
        os << indent << "subparsers list:\n";
        std::size_t i = 0;
        for (prs_iterator it = parsers.begin(); it != parsers.end(); ++it) {
            os << indent << "  " << (++i) << ". '" << (*it)->m_name << "'";
            std::string aliases = detail::_join((*it)->aliases(), ", ", "'");
            if (!aliases.empty()) {
                os << ", aliases: " << aliases;
            }
            os << "\n";
            overview_subparsers((*it).get(), indent + "  ", os);
        }
    }
}

ARGPARSE_INL bool
ArgumentParser::test_diagnostics(
        std::string const& lang,
        std::ostream& os) const
{
    std::string const _ok    = "[ OK ]";
    std::string const _warn  = "[WARN]";
    std::string const _error = "[FAIL]";
    typedef std::pair<std::size_t, std::size_t> WarnErrAmount;
    WarnErrAmount diagnostics;
    // check prog
    if (prog() == detail::_prog) {
        ++diagnostics.first;
        os << _warn << " used default `prog` value, "
           << "override it or pass command line options\n";
    }
    // check prefix chars
    if (!fromfile_prefix_chars().empty()) {
        for (std::size_t i = 0; i < fromfile_prefix_chars().size(); ++i) {
            char c = fromfile_prefix_chars().at(i);
            if (detail::_exists(c, prefix_chars())) {
                ++diagnostics.first;
                os << _warn << " fromfile prefix char '" << std::string(1, c)
                   << "' exists in prefix_chars\n";
            }
        }
        for (std::size_t i = 0; i < comment_prefix_chars().size(); ++i) {
            char c = comment_prefix_chars().at(i);
            if (detail::_exists(c, prefix_chars())) {
                ++diagnostics.first;
                os << _warn << " comment prefix char '" << std::string(1, c)
                   << "' exists in prefix_chars\n";
            }
            if (detail::_exists(c, fromfile_prefix_chars())) {
                ++diagnostics.first;
                os << _warn << " comment prefix char '" << std::string(1, c)
                   << "' exists in fromfile_prefix_chars\n";
            }
        }
    }
    // check parser
    test_argument_parser(lang, os, this, diagnostics);
    // end diagnostics
    bool res = diagnostics.first == 0 && diagnostics.second == 0;
    if (res) {
        os << _ok << " no warning or errors detected\n";
    } else {
        if (diagnostics.first != 0) {
            os << _warn << " detected warnings: " << diagnostics.first << "\n";
        }
        if (diagnostics.second != 0) {
            os << _error << " detected errors: " << diagnostics.second << "\n";
        }
    }
    return res;
}

ARGPARSE_INL void
ArgumentParser::test_argument_parser(
        std::string const& lang,
        std::ostream& os,
        ArgumentParser const* p,
        std::pair<std::size_t, std::size_t>& diagnostics)
{
    std::string const _info  = "[INFO]";
    std::string const _warn  = "[WARN]";
    std::string const _error = "[FAIL]";
    std::vector<std::string> dest_args;
    // check arguments
    for (arg_iterator i = p->m_data->m_arguments.begin();
         i != p->m_data->m_arguments.end(); ++i) {
        pArgument const& arg = (*i);
        std::string argument = "argument with ";
        argument += std::string(arg->dest().empty() ? "options" : "dest");
        argument += " " + detail::_join(arg->get_argument_flags(), ", ", "'");
        // check flags
        bool is_optional = arg->m_type == Argument::Optional;
        for (std::size_t j = 0; j < arg->flags().size(); ++j) {
            std::string const& flag = arg->flags().at(j);
            if (!detail::_is_utf8_string(flag)) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": flag '"
                   << flag << "' is not utf-8\n";
            }
            if (!detail::_is_flag_correct(flag, is_optional)) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": flag '"
                   << flag << "' can be incorrect\n";
            }
            if (flag == detail::_pseudo_arg && arg->dest().empty()) {
                ++diagnostics.second;
                os << _error << " " << argument << ": dest= "
                   << "is required for options like '--'\n";
            }
        }
        // check dest
        if (arg->m_type == Argument::Positional) {
            if (arg->dest().empty() && arg->flags().empty()) {
                ++diagnostics.second;
                os << _error << " " << argument << ": missing 1 "
                   << "required positional argument: 'dest'\n";
            }
            if (!arg->dest().empty() && !arg->flags().empty()) {
                ++diagnostics.second;
                os << _error << " " << argument << ": dest supplied "
                   << "twice for positional argument\n";
            }
        }
        if (!arg->dest().empty()) {
            std::string const& flag = arg->dest();
            if (!detail::_is_utf8_string(flag)) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": dest '"
                   << flag << "' is not utf-8\n";
            }
            if (!detail::_is_flag_correct(flag, is_optional)) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": dest '"
                   << flag << "' can be incorrect\n";
            }
        }
        for (std::size_t j = 0; j < arg->get_argument_flags().size(); ++j) {
            std::string const& flag = arg->get_argument_flags().at(j);
            if (!flag.empty()) {
                if (detail::_exists(flag, dest_args)) {
                    if (p->conflict_handler() == "resolve") {
                        ++diagnostics.first;
                        os << _warn << " " << argument << " resolve"
                           << ": conflicting option string: '" << flag << "'\n";
                    } else {
                        ++diagnostics.second;
                        os << _error << " " << argument
                           << ": conflicting option string: '" << flag << "'\n";
                    }
                }
                dest_args.push_back(flag);
            }
        }
        // check choices
        for (std::size_t j = 0; j < arg->choices().size(); ++j) {
            std::string const& str = arg->choices().at(j);
            if (str.size() > 1 && str.size() != detail::_trim_sw(str).size()) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": choice '"
                   << str << "' can be incorrect\n";
            } else if (str.empty()) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": empty choice\n";
            }
        }
        if (arg->m_choices.has_value() && arg->choices().empty()) {
            ++diagnostics.first;
            os << _warn << " " << argument << ": empty choices\n";
        }
        // check help
        if (detail::_tr(arg->m_help.value(), lang).empty()
                && !arg->m_help.suppress()) {
            ++diagnostics.first;
            os << _warn << " " << argument << ": help is not set\n";
        }
        // check metavar
        if ((arg->action() & (detail::_store_action
                              | argparse::append_const
                              | argparse::language))) {
            std::size_t size = arg->get_argument_name(*(p->m_formatter)).size();
            if (size > 1 && (!(arg->m_nargs & Argument::_NARGS_COMBINED)
                             || size != arg->m_num_args)) {
                ++diagnostics.second;
                os << _error << " " << argument << ": length of "
                   << "metavar tuple does not match nargs\n";
            }
        }
        // check type
        if (arg->m_type_name.has_value()) {
            if (arg->m_type_name.value() == detail::Type::basic<bool>()
                    && !(arg->action() & (detail::_bool_action
                                          | argparse::BooleanOptionalAction))) {
                ++diagnostics.first;
                os << _warn << " " << argument << ": danger to use `bool` type "
                   << "for non-boolean actions\n";
            }
        }
    }
    // check mutually exclusive arguments
    for (mtx_it i = p->m_mutex_groups.begin();
         i != p->m_mutex_groups.end(); ++i) {
        for (arg_iterator j = (*i).m_data->m_arguments.begin();
             j != (*i).m_data->m_arguments.end(); ++j) {
            std::string argument = "argument with ";
            argument += std::string((*j)->dest().empty() ? "options" : "dest");
            argument += " " + detail::_join(
                            (*j)->get_argument_flags(), ", ", "'");
            if ((*j)->required()) {
                ++diagnostics.second;
                os << _error << " " << argument << ": mutually "
                   << "exclusive arguments must be optional\n";
            }
        }
    }
    // check subparsers
    if (p->has_subparsers()) {
        std::list<pParser> const parsers = p->m_subparsers->list_parsers();
        if (parsers.empty()) {
            ++diagnostics.first;
            os << _warn << " subparsers created but no parsers were added\n";
        }
        // check dest
        std::string const& flag = p->m_subparsers->dest();
        if (flag.empty()) {
            os << _info << " you can specify `dest` for subparsers "
               << "to determine used parser\n";
        } else {
            if (!detail::_is_utf8_string(flag)) {
                ++diagnostics.first;
                os << _warn << " subparsers dest '"
                   << flag << "' is not utf-8\n";
            }
            if (!detail::_is_flag_correct(flag, false)) {
                ++diagnostics.first;
                os << _warn << " subparsers dest '"
                   << flag << "' can be incorrect\n";
            }
            if (detail::_exists(flag, dest_args)) {
                if (p->conflict_handler() == "resolve") {
                    ++diagnostics.first;
                    os << _warn << " subparsers dest '" << flag << " resolve"
                       << "': conflicting option string: '" << flag << "'\n";
                } else {
                    ++diagnostics.second;
                    os << _error << " subparsers dest '" << flag
                       << "': conflicting option string: '" << flag << "'\n";
                }
            }
            dest_args.push_back(flag);
        }
        // check help
        if (detail::_tr(p->m_subparsers->m_help.value(), lang).empty()
                && !p->m_subparsers->is_suppress()) {
            ++diagnostics.first;
            os << _warn << " help for subparsers is not set\n";
        }
        for (prs_iterator it = parsers.begin(); it != parsers.end(); ++it) {
            std::string const& name = (*it)->m_name;
            // check name
            if (!detail::_is_utf8_string(name)) {
                ++diagnostics.first;
                os << _warn << " name for parser '"
                   << name << "' is not utf-8\n";
            }
            if (!name.empty()
                    && detail::_exists(name.at(0), p->m_prefix_chars)) {
                ++diagnostics.first;
                os << _warn << " name for parser '" << name << "' incorrect, "
                   << "started with prefix chars '" << p->m_prefix_chars
                   << "'\n";
            } else if (!detail::_is_flag_correct(name, false)) {
                ++diagnostics.first;
                os << _warn << " name for parser '"
                   << name << "' can be incorrect\n";
            }
            // check help
            if (detail::_tr((*it)->m_help, lang).empty()) {
                ++diagnostics.first;
                os << _warn << " help for parser '" << name << "' is not set\n";
            }
            // check parser
            test_argument_parser(lang, os, (*it).get(), diagnostics);
        }
    }
}

ARGPARSE_INL void
ArgumentParser::handle(
        std::string const& str) const
{
    if (m_handle) {
        m_handle(detail::_remove_quotes(str));
    }
}

ARGPARSE_INL void
ArgumentParser::parse_handle(
        bool only_known,
        _Storage const& storage,
        std::vector<std::string> const& unrecognized_args) const
{
    if (m_parse_handle) {
        m_parse_handle(only_known ? Namespace(storage, unrecognized_args)
                                  : Namespace(storage));
    }
}
#endif  // ARGPARSE_INL
}  // namespace argparse

// -- #undef ------------------------------------------------------------------
#undef ARGPARSE_INL
#ifndef ARGPARSE_KEEP_MACROS
// -- standard ----------------------------------------------------------------
# undef ARGPARSE_CXX_98
# undef ARGPARSE_CXX_11
# undef ARGPARSE_CXX_14
# undef ARGPARSE_CXX_17
# undef ARGPARSE_CXX_20
# undef ARGPARSE_CXX_23
// -- features ----------------------------------------------------------------
# undef ARGPARSE_HAS_OPTIONAL
# undef ARGPARSE_HAS_STRING_VIEW
#endif  // ARGPARSE_KEEP_MACROS
// -- attributes --------------------------------------------------------------
// C++11+
#undef ARGPARSE_ATTR_NORETURN
#undef ARGPARSE_ATTR_CARRIES_DEPENDENCY
// C++14+
#undef ARGPARSE_ATTR_DEPRECATED
#undef ARGPARSE_ATTR_DEPRECATED_REASON
// C++17+
#undef ARGPARSE_ATTR_FALLTHROUGH
#undef ARGPARSE_ATTR_MAYBE_UNUSED
#undef ARGPARSE_ATTR_NODISCARD
// C++20+
#undef ARGPARSE_ATTR_LIKELY
#undef ARGPARSE_ATTR_NODISCARD_REASON
#undef ARGPARSE_ATTR_NO_UNIQUE_ADDRESS
#undef ARGPARSE_ATTR_UNLIKELY
// C++23+
#undef ARGPARSE_ATTR_ASSUME
// -- specifiers --------------------------------------------------------------
#undef ARGPARSE_CONSTEXPR
#undef ARGPARSE_ENUM_TYPE
#undef ARGPARSE_EXPORT
#undef ARGPARSE_FINAL
#undef ARGPARSE_INLINE_VARIABLE
#undef ARGPARSE_MOVE
#undef ARGPARSE_NOEXCEPT
#undef ARGPARSE_NULLPTR
#undef ARGPARSE_OVERRIDE
#undef ARGPARSE_USE_CONSTEXPR

#endif  // _ARGPARSE_ARGUMENT_PARSER_HPP_
