/*
*  __            __   __  __   __   __   __  __   __
* |   _|_ _|_   |  | |   |  | |  | |  | |   |__  |__|
* |__  |   |    \_/| |   \__| |_/  \_/| |    __| \__
*                         __| |
* MIT License
*
* Argument parser for C++ (ArgumentParser v1.8.1)
*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
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
#undef _ARGPARSE_CXX_98
#undef _ARGPARSE_CXX_11
#undef _ARGPARSE_CXX_14
#undef _ARGPARSE_CXX_17
#undef _ARGPARSE_CXX_20
#undef _ARGPARSE_CXX_23
// -- attributes --------------------------------------------------------------
// C++11+
#undef _ARGPARSE_ATTR_NORETURN
#undef _ARGPARSE_ATTR_CARRIES_DEPENDENCY
// C++14+
#undef _ARGPARSE_ATTR_DEPRECATED
#undef _ARGPARSE_ATTR_DEPRECATED_REASON
// C++17+
#undef _ARGPARSE_ATTR_FALLTHROUGH
#undef _ARGPARSE_ATTR_MAYBE_UNUSED
#undef _ARGPARSE_ATTR_NODISCARD
// C++20+
#undef _ARGPARSE_ATTR_LIKELY
#undef _ARGPARSE_ATTR_NODISCARD_REASON
#undef _ARGPARSE_ATTR_NO_UNIQUE_ADDRESS
#undef _ARGPARSE_ATTR_UNLIKELY
// C++23+
#undef _ARGPARSE_ATTR_ASSUME
// -- specifiers --------------------------------------------------------------
#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_ENUM_TYPE
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_FINAL
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_MOVE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_NULLPTR
#undef _ARGPARSE_OVERRIDE
#undef _ARGPARSE_USE_CONSTEXPR

// -- #define -----------------------------------------------------------------
// -- version -----------------------------------------------------------------
#define ARGPARSE_VERSION_MAJOR 1
#define ARGPARSE_VERSION_MINOR 8
#define ARGPARSE_VERSION_PATCH 1

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
#if _MSVC_LANG >= 202302L
#define _ARGPARSE_CXX_23
#endif  // C++23+
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
#if __cplusplus >= 202302L
#define _ARGPARSE_CXX_23
#endif  // C++23+
#endif  // _MSVC_LANG

// -- static building ---------------------------------------------------------
#undef _ARGPARSE_INL
#ifdef ARGPARSE_DECLARATION
// header
#undef ARGPARSE_IMPLEMENTATION
#else
#ifdef ARGPARSE_IMPLEMENTATION
// static build
#define _ARGPARSE_INL
#else
// single-header
#define _ARGPARSE_INL inline
#endif  // ARGPARSE_IMPLEMENTATION
#endif  // ARGPARSE_DECLARATION

// -- terminal size detection -------------------------------------------------
#undef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#if !defined ARGPARSE_DISABLE_TERMINAL_SIZE_DETECTION
#define ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION

#ifdef _ARGPARSE_INL
#ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#if defined(_WIN32)
#undef _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#undef _ARGPARSE_DEFINE_VC_EXTRALEAN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#endif  // WIN32_LEAN_AND_MEAN

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#define _ARGPARSE_DEFINE_VC_EXTRALEAN
#endif  // VC_EXTRALEAN

#ifndef NOMINMAX
#define NOMINMAX
#define _ARGPARSE_DEFINE_NOMINMAX
#endif  // NOMINMAX

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <windows.h>
#endif  // _MSC_VER

#ifdef _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif  // _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN

#ifdef _ARGPARSE_DEFINE_VC_EXTRALEAN
#undef VC_EXTRALEAN
#endif  // _ARGPARSE_DEFINE_VC_EXTRALEAN

#ifdef _ARGPARSE_DEFINE_NOMINMAX
#undef NOMINMAX
#endif  // _ARGPARSE_DEFINE_NOMINMAX

#undef _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#undef _ARGPARSE_DEFINE_VC_EXTRALEAN
#undef _ARGPARSE_DEFINE_NOMINMAX
#else  // UNIX
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#endif  // _WIN32
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#endif  // _ARGPARSE_INL

// -- #include ----------------------------------------------------------------
#ifdef _ARGPARSE_CXX_11
#include <array>
#include <cstdint>
#include <forward_list>
#include <initializer_list>
#include <regex>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#else
#include <stdint.h>

#include <cassert>
#endif  // C++11+

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _ARGPARSE_CXX_17
#include <optional>
#include <string_view>
#endif  // C++17+

// -- attributes --------------------------------------------------------------
#ifdef _ARGPARSE_CXX_11
#define _ARGPARSE_ATTR_NORETURN             [[noreturn]]
#define _ARGPARSE_ATTR_CARRIES_DEPENDENCY   [[carries_dependency]]
#else
#define _ARGPARSE_ATTR_NORETURN
#define _ARGPARSE_ATTR_CARRIES_DEPENDENCY
#endif  // C++11+

#ifdef _ARGPARSE_CXX_14
#define _ARGPARSE_ATTR_DEPRECATED           [[deprecated]]
#define _ARGPARSE_ATTR_DEPRECATED_REASON(X) [[deprecated(X)]]
#else
#define _ARGPARSE_ATTR_DEPRECATED
#define _ARGPARSE_ATTR_DEPRECATED_REASON(X)
#endif  // C++14+

#ifdef _ARGPARSE_CXX_17
#define _ARGPARSE_ATTR_FALLTHROUGH          [[fallthrough]]
#define _ARGPARSE_ATTR_MAYBE_UNUSED         [[maybe_unused]]
#define _ARGPARSE_ATTR_NODISCARD            [[nodiscard]]
#define _ARGPARSE_ATTR_NODISCARD_REASON(X)  [[nodiscard]]
#else
#define _ARGPARSE_ATTR_FALLTHROUGH
#define _ARGPARSE_ATTR_MAYBE_UNUSED
#define _ARGPARSE_ATTR_NODISCARD
#define _ARGPARSE_ATTR_NODISCARD_REASON(X)
#endif  // C++17+

#ifdef _ARGPARSE_CXX_20
#undef _ARGPARSE_ATTR_NODISCARD_REASON

#define _ARGPARSE_ATTR_LIKELY               [[likely]]
#define _ARGPARSE_ATTR_NODISCARD_REASON(X)  [[nodiscard(X)]]
#define _ARGPARSE_ATTR_NO_UNIQUE_ADDRESS    [[no_unique_address]]
#define _ARGPARSE_ATTR_UNLIKELY             [[unlikely]]
#else
#define _ARGPARSE_ATTR_LIKELY
#define _ARGPARSE_ATTR_NO_UNIQUE_ADDRESS
#define _ARGPARSE_ATTR_UNLIKELY
#endif  // C++20+

#ifdef _ARGPARSE_CXX_23
#define _ARGPARSE_ATTR_ASSUME(X)            [[assume(X)]]
#else
#define _ARGPARSE_ATTR_ASSUME(X)
#endif  // C++23+

// -- specifiers --------------------------------------------------------------
#define _ARGPARSE_EXPORT

#ifdef _ARGPARSE_CXX_11
#define _ARGPARSE_FINAL final
#define _ARGPARSE_NOEXCEPT noexcept
#define _ARGPARSE_OVERRIDE override
#define _ARGPARSE_CONSTEXPR constexpr
#define _ARGPARSE_USE_CONSTEXPR constexpr
#define _ARGPARSE_ENUM_TYPE(X) : X
#define _ARGPARSE_MOVE(X) std::move(X)
#else
#define _ARGPARSE_FINAL
#define _ARGPARSE_NOEXCEPT
#define _ARGPARSE_OVERRIDE
#define _ARGPARSE_CONSTEXPR
#define _ARGPARSE_USE_CONSTEXPR const
#define _ARGPARSE_ENUM_TYPE(X)
#define _ARGPARSE_MOVE(X) X
#endif  // C++11+

#ifdef _ARGPARSE_CXX_17
#define _ARGPARSE_INLINE_VARIABLE inline
#else
#define _ARGPARSE_INLINE_VARIABLE static
#endif  // C++17+

namespace argparse {
#ifdef _ARGPARSE_CXX_11
#define _ARGPARSE_NULLPTR nullptr
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

#define _ARGPARSE_NULLPTR _nullptr
#endif  // C++11+

/*!
 *  \brief Action values
 *
 *  \enum Action
 */
_ARGPARSE_EXPORT enum Action _ARGPARSE_ENUM_TYPE(uint16_t)
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
_ARGPARSE_EXPORT class ArgumentError : public std::invalid_argument
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
    ArgumentError(std::string const& error)
        : std::invalid_argument("argparse::ArgumentError: " + error)
    { }
};

/*!
 *  \brief AttributeError handler
 */
_ARGPARSE_EXPORT class AttributeError : public std::invalid_argument
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
    AttributeError(std::string const& error)
        : std::invalid_argument("AttributeError: " + error)
    { }
};

/*!
 *  \brief ValueError handler
 */
_ARGPARSE_EXPORT class ValueError : public std::invalid_argument
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
    ValueError(std::string const& error)
        : std::invalid_argument("ValueError: " + error)
    { }
};

/*!
 *  \brief IndexError handler
 */
_ARGPARSE_EXPORT class IndexError : public std::out_of_range
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
    IndexError(std::string const& error)
        : std::out_of_range("IndexError: " + error)
    { }
};

/*!
 *  \brief NameError handler
 *
 *  \since v1.7.1
 */
_ARGPARSE_EXPORT class NameError : public std::invalid_argument
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
    NameError(std::string const& error)
        : std::invalid_argument("NameError: " + error)
    { }
};

/*!
 *  \brief TypeError handler
 */
_ARGPARSE_EXPORT class TypeError : public std::invalid_argument
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
    TypeError(std::string const& error)
        : std::invalid_argument("TypeError: " + error)
    { }
};

/*!
 *  \brief Don't use this enum name! use argparse::SUPPRESS value directly
 *
 *  \enum _SUPPRESS
 */
_ARGPARSE_EXPORT _ARGPARSE_INLINE_VARIABLE
enum _SUPPRESS _ARGPARSE_ENUM_TYPE(uint8_t) {} SUPPRESS;

/*!
 *  \brief Don't use this enum name! use argparse::REMAINDER value directly
 *
 *  \enum _REMAINDER
 */
_ARGPARSE_EXPORT _ARGPARSE_INLINE_VARIABLE
enum _REMAINDER _ARGPARSE_ENUM_TYPE(uint8_t) {} REMAINDER;

// Forward declaration
class Argument;

/*!
 *  \brief Formatter for generating usage messages and argument help strings
 */
_ARGPARSE_EXPORT class HelpFormatter
{
    static std::size_t const c_default_tab_size = 4;

public:
    HelpFormatter()
        : m_tab_size(c_default_tab_size)
    { }
    virtual ~HelpFormatter() _ARGPARSE_NOEXCEPT { }

    inline void
    _tab_size(std::size_t value) _ARGPARSE_NOEXCEPT
    {
        m_tab_size = value != 0 ? value : c_default_tab_size;
    }

    inline std::size_t
    _tab_size() const _ARGPARSE_NOEXCEPT
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
    // -- data ----------------------------------------------------------------
    std::size_t m_tab_size;
};

/*!
 *  \brief Help message formatter which retains any formatting in descriptions
 */
_ARGPARSE_EXPORT
class _RawDescriptionHelpFormatter : virtual public HelpFormatter
{
public:
    ~_RawDescriptionHelpFormatter() _ARGPARSE_NOEXCEPT _ARGPARSE_OVERRIDE { }

    std::string
    _fill_text(
            std::string const& text,
            std::size_t width,
            std::size_t indent) const _ARGPARSE_OVERRIDE;

protected:
    std::vector<std::string>
    _split_lines_raw(
            std::string const& text,
            std::size_t width) const;
} _ARGPARSE_INLINE_VARIABLE RawDescriptionHelpFormatter;

/*!
 *  \brief Help message formatter which retains formatting of all help text
 */
_ARGPARSE_EXPORT
class _RawTextHelpFormatter : virtual public _RawDescriptionHelpFormatter
{
public:
    ~_RawTextHelpFormatter() _ARGPARSE_NOEXCEPT _ARGPARSE_OVERRIDE { }

    std::vector<std::string>
    _split_lines(
            std::string const& text,
            std::size_t width) const _ARGPARSE_OVERRIDE;
} _ARGPARSE_INLINE_VARIABLE RawTextHelpFormatter;

/*!
 *  \brief Help message formatter which adds default values to argument help
 */
_ARGPARSE_EXPORT
class _ArgumentDefaultsHelpFormatter : virtual public HelpFormatter
{
public:
    ~_ArgumentDefaultsHelpFormatter() _ARGPARSE_NOEXCEPT _ARGPARSE_OVERRIDE { }

    std::string
    _get_help_string(
            Argument const* action,
            std::string const& lang) const _ARGPARSE_OVERRIDE;
} _ARGPARSE_INLINE_VARIABLE ArgumentDefaultsHelpFormatter;

/*!
 *  \brief Help message formatter which uses the argument 'type' as the default
 *  metavar value (instead of the argument 'dest')
 */
_ARGPARSE_EXPORT
class _MetavarTypeHelpFormatter : virtual public HelpFormatter
{
public:
    ~_MetavarTypeHelpFormatter() _ARGPARSE_NOEXCEPT _ARGPARSE_OVERRIDE { }

    std::string
    _get_default_metavar_for_optional(
            Argument const* action) const _ARGPARSE_OVERRIDE;

    std::string
    _get_default_metavar_for_positional(
            Argument const* action) const _ARGPARSE_OVERRIDE;
} _ARGPARSE_INLINE_VARIABLE MetavarTypeHelpFormatter;

namespace detail {
_ARGPARSE_INLINE_VARIABLE std::size_t _ARGPARSE_USE_CONSTEXPR _min_width = 33;
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _prefix_char      = '-';
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _prefix_chars[]   = "-";
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _pseudo_arg[]     = "--";
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _space            = ' ';
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _equal            = '=';
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _spaces[]         = " ";
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR _equals[]         = "=";
_ARGPARSE_INLINE_VARIABLE char _ARGPARSE_USE_CONSTEXPR
                                                   _suppress[] = "==SUPPRESS==";

_ARGPARSE_INLINE_VARIABLE int32_t _ARGPARSE_USE_CONSTEXPR
_bool_action = argparse::store_true | argparse::store_false;
_ARGPARSE_INLINE_VARIABLE int32_t _ARGPARSE_USE_CONSTEXPR
_store_action = argparse::store | argparse::append | argparse::extend;
_ARGPARSE_INLINE_VARIABLE int32_t _ARGPARSE_USE_CONSTEXPR
_const_action = argparse::store_const | argparse::append_const;
_ARGPARSE_INLINE_VARIABLE int32_t _ARGPARSE_USE_CONSTEXPR
_store_const_action = _store_action | _const_action;

template <class K, class V, class C, class A>
V const&
_map_at(std::map<K, V, C, A> const& m,
        K const& k)
{
    typename std::map<K, V, C, A>::const_iterator it(m.find(k));
    if (it == m.end()) {
        throw std::out_of_range("map::at");
    }
    return it->second;
}

template <class T> struct is_byte_type              { enum { value = false }; };
template <>        struct is_byte_type<char>         { enum { value = true }; };
template <>        struct is_byte_type<signed char>  { enum { value = true }; };
template <>        struct is_byte_type<unsigned char>{ enum { value = true }; };
#ifdef _ARGPARSE_CXX_17
template <>        struct is_byte_type<std::byte>    { enum { value = true }; };
#endif  // C++17+
#ifdef _ARGPARSE_CXX_20
template <>        struct is_byte_type<char8_t>      { enum { value = true }; };
#endif  // C++20+

#ifdef _ARGPARSE_CXX_11
using std::decay;
using std::enable_if;
using std::is_constructible;
using std::is_floating_point;
using std::is_integral;
using std::is_same;
using std::is_base_of;

template <class T>
struct is_stl_array                                          :std::false_type{};
template <class T, std::size_t N>
struct is_stl_array<std::array                     <T, N> >   :std::true_type{};

template <class T>
struct is_stl_container                                      :std::false_type{};
template <class... Args>
struct is_stl_container<std::deque                 <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::forward_list          <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::list                  <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::multiset              <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::priority_queue        <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::set                   <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::vector                <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::unordered_multiset    <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_container<std::unordered_set         <Args...> >:std::true_type{};

template <class T, typename U = void>
struct is_stl_container_paired                               :std::false_type{};
template <class _1st, class _2nd, template <class...> class container>
struct is_stl_container_paired<container<std::pair<_1st, _2nd> > >
                                                              :std::true_type{};

template <class T, typename U = void>
struct is_stl_container_tupled                               :std::false_type{};
template <class... Args, template <class...> class container>
struct is_stl_container_tupled<container<std::tuple<Args...> > >
                                                              :std::true_type{};

template <class...>
struct voider { using type = void; };
template <class... T>
using void_t = typename voider<T...>::type;

template <class T, typename U = void>
struct is_stl_map                                            :std::false_type{};
template <class T>
struct is_stl_map<T, void_t<typename T::key_type,
                                    typename T::mapped_type> >:std::true_type{};

template <class T>
struct is_stl_matrix                                         :std::false_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::deque       <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::forward_list<Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::list        <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::multiset    <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::priority_queue
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::set         <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::vector      <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::unordered_multiset
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::unordered_set
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::deque        <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::forward_list <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::list         <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::multiset     <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::priority_queue
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::set          <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::vector       <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::unordered_multiset
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::unordered_set<Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::deque      <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::forward_list
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::list       <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::multiset   <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::priority_queue
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::set        <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::vector     <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::unordered_multiset
                                                 <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::unordered_set
                                                 <Args...> > >:std::true_type{};

template <class T>
struct is_stl_matrix_queue                                   :std::false_type{};
template <class... Args>
struct is_stl_matrix_queue<std::deque<std::stack <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix_queue<std::deque<std::queue <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix_queue<std::list<std::stack  <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix_queue<std::list<std::queue  <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix_queue<std::vector<std::stack<Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix_queue<std::vector<std::queue<Args...> > >:std::true_type{};

template <class T, typename U = void>
struct is_stl_pair                                           :std::false_type{};
template <class T>
struct is_stl_pair<T, void_t<typename T::first_type,
                                    typename T::second_type> >:std::true_type{};

template <class T>
struct is_stl_queue                                          :std::false_type{};
template <class... Args>
struct is_stl_queue<std::stack                     <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_queue<std::queue                     <Args...> >:std::true_type{};

template <class T>
struct is_stl_tuple                                          :std::false_type{};
template <class... Args>
struct is_stl_tuple<std::tuple                     <Args...> >:std::true_type{};

template <class T>
struct type_tag{};

template <class T, T... Ints>
struct integer_sequence{};

template <class T, T N, typename = void>
struct make_integer_sequence_impl
{
    template <class>
    struct tmp;

    template <T... Prev>
    struct tmp<integer_sequence<T, Prev...> >
    { using type = integer_sequence<T, Prev..., N - 1>; };

    using type
      = typename tmp<typename make_integer_sequence_impl<T, N - 1>::type>::type;
};

template <class T, T N>
struct make_integer_sequence_impl<T, N, typename std::enable_if<N == 0>::type>
{ using type = integer_sequence<T>; };

template <class T, T N>
using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;
#else
template <bool B, class T, class F>
struct conditional                  { typedef T type; };
template <class T, class F>
struct conditional<false, T, F>     { typedef F type; };

template <bool, typename T = void>
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

template <class T>
struct _is_pointer_helper           { static const bool value = false; };
template <class T>
struct _is_pointer_helper<T*>       { static const bool value = true; };

template <class T>
struct is_pointer
{
    static const bool value
                       = _is_pointer_helper<typename remove_cv<T>::type>::value;
};

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

template <class B, class D>
struct Host
{
    operator B*() const;
    operator D*();
};

template <class B, class D>
struct is_base_of
{
    typedef char yes;
    typedef int no;

    template <class T>
    static yes check(D*, T);
    static no check(B*, int);

    static const bool value = sizeof(check(Host<B, D>(), int())) == sizeof(yes);
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
    static bool test(
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
    static int test(...);

    static const bool value
              = (sizeof(test<T, AU_1, AU_2, AU_3, AU_4>(NULL)) == sizeof(bool));
};

template <class T, class AT_1, class AT_2, class AT_3>
struct _is_constructible_impl<T, AT_1, AT_2, AT_3, void>
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;
    typedef typename _replace_array_with_pointer<AT_2>::type AU_2;
    typedef typename _replace_array_with_pointer<AT_3>::type AU_3;

    template <class C_T, class C_AT_1, class C_AT_2, class C_AT_3>
    static bool test(
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
    static int test(...);

    static const bool value
                    = (sizeof(test<T, AU_1, AU_2, AU_3>(NULL)) == sizeof(bool));
};

template <class T, class AT_1, class AT_2>
struct _is_constructible_impl<T, AT_1, AT_2, void, void>
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;
    typedef typename _replace_array_with_pointer<AT_2>::type AU_2;

    template <class C_T, class C_AT_1, class C_AT_2>
    static bool test(
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
    static int test(...);

    static const bool value
                          = (sizeof(test<T, AU_1, AU_2>(NULL)) == sizeof(bool));
};

template <class T, class AT_1>
struct _is_constructible_impl<T, AT_1, void, void, void>
{
    typedef typename _replace_array_with_pointer<AT_1>::type AU_1;

    template <class C_T, class C_AT_1>
    static bool test(
        typename enable_if<
            sizeof(C_T) ==
            sizeof(C_T(
                static_cast<C_AT_1>(
                   *static_cast<typename remove_reference<C_AT_1>::type*>(NULL))
            ))
        >::type*
    );

    template <class, class>
    static int test(...);

    static const bool value = (sizeof(test<T, AU_1>(NULL)) == sizeof(bool));
};

template <class T>
struct _is_constructible_impl<T, void, void, void, void>
{
    template <class C_T>
    static C_T testFun(C_T);

    template <class C_T>
    static bool test(
            typename enable_if<sizeof(C_T)
                    == sizeof(_is_constructible_impl::testFun(C_T()))>::type*);

    template <class>
    static int test(...);

    static const bool value = (sizeof(test<T>(NULL)) == sizeof(bool));
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
struct _is_constructible_impl_ptr
{
    static const bool value = false;
};

template <class T, class AT_1>
struct _is_constructible_impl_ptr<T, AT_1,
        typename enable_if<is_pointer<
            typename remove_reference<T>::type>::value, void>::type, void, void>
{
    template <class C_T>
    static bool test(C_T);

    template <class>
    static int test(...);

    static const bool value
                   = (sizeof(test<T>(static_cast<AT_1>(NULL))) == sizeof(bool));
};

template <class T>
struct _is_constructible_impl_ptr<T, void, void, void, void>
{
    static const bool value = true;
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
struct is_constructible
{
    typedef typename _replace_array_with_pointer<T>::type U;

    static const bool value = (
        is_pointer<typename remove_reference<U>::type>::value
                ? _is_constructible_impl_ptr<U, AT_1, AT_2, AT_3, AT_4>::value
                : _is_constructible_impl<U, AT_1, AT_2, AT_3, AT_4>::value
    );
};

template <>
struct is_constructible<std::string, _SUPPRESS >
{
    static const bool value = false;
};

template <>
struct is_constructible<std::string, std::vector<std::string> >
{
    static const bool value = false;
};

template <class T>
struct is_stl_container                                           :false_type{};
template <class T>
struct is_stl_container<std::deque                            <T> >:true_type{};
template <class T>
struct is_stl_container<std::list                             <T> >:true_type{};
template <class T>
struct is_stl_container<std::multiset                         <T> >:true_type{};
template <class T>
struct is_stl_container<std::priority_queue                   <T> >:true_type{};
template <class T>
struct is_stl_container<std::set                              <T> >:true_type{};
template <class T>
struct is_stl_container<std::vector                           <T> >:true_type{};

template <class T, typename U = void>
struct is_stl_container_paired                                    :false_type{};
template <class _1st, class _2nd, template <class T, class U> class container>
struct is_stl_container_paired<container<std::pair<_1st, _2nd>,
                         std::allocator<std::pair<_1st, _2nd> > > >:true_type{};

template <class T, class U>
struct voider { typedef void type; };

template <class T, typename U = void>
struct is_stl_map                                                 :false_type{};
template <class T>
struct is_stl_map<T, typename voider<typename T::key_type,
                                    typename T::mapped_type>::type>:true_type{};

template <class T, typename U = void>
struct is_stl_pair                                                :false_type{};
template <class T>
struct is_stl_pair<T, typename voider<typename T::first_type,
                                    typename T::second_type>::type>:true_type{};

template <class T>
struct is_stl_queue                                               :false_type{};
template <class T>
struct is_stl_queue<std::stack                                <T> >:true_type{};
template <class T>
struct is_stl_queue<std::queue                                <T> >:true_type{};

template <class T>
struct is_stl_matrix                                              :false_type{};
template <class T>
struct is_stl_matrix<std::deque<std::deque                  <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::deque<std::list                   <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::deque<std::multiset               <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::deque<std::priority_queue         <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::deque<std::set                    <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::deque<std::vector                 <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::list<std::deque                   <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::list<std::list                    <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::list<std::multiset                <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::list<std::priority_queue          <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::list<std::set                     <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::list<std::vector                  <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::vector<std::deque                 <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::vector<std::list                  <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::vector<std::multiset              <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::vector<std::priority_queue        <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::vector<std::set                   <T> > >:true_type{};
template <class T>
struct is_stl_matrix<std::vector<std::vector                <T> > >:true_type{};

template <class T>
struct is_stl_matrix_queue                                        :false_type{};
template <class T>
struct is_stl_matrix_queue<std::deque<std::stack            <T> > >:true_type{};
template <class T>
struct is_stl_matrix_queue<std::deque<std::queue            <T> > >:true_type{};
template <class T>
struct is_stl_matrix_queue<std::list<std::stack             <T> > >:true_type{};
template <class T>
struct is_stl_matrix_queue<std::list<std::queue             <T> > >:true_type{};
template <class T>
struct is_stl_matrix_queue<std::vector<std::stack           <T> > >:true_type{};
template <class T>
struct is_stl_matrix_queue<std::vector<std::queue           <T> > >:true_type{};
#endif  // C++11+

#ifdef _ARGPARSE_CXX_11
template <class T>  struct rval { typedef T&& type; };

using std::shared_ptr;
using std::make_shared;
#else
template <class T>  struct rval { typedef T const& type; };

// Slightly modified version of the shared_ptr implementation for C++98
// from Sébastien Rombauts which is licensed under the MIT License.
// See https://github.com/SRombauts/shared_ptr
class _shared_ptr_count
{
public:
    _shared_ptr_count()
        : pn(NULL)
    { }

    _shared_ptr_count(
            _shared_ptr_count const& orig)
        : pn(orig.pn)
    { }

    inline void
    swap(_shared_ptr_count& lhs) throw()
    {
        std::swap(pn, lhs.pn);
    }

    inline std::size_t
    use_count() const throw()
    {
        std::size_t count = 0;
        if (pn) {
            count = *pn;
        }
        return count;
    }

    template <class U>
    void
    acquire(U*& p)
    {
        if (p) {
            if (!pn) {
                try {
                    pn = new std::size_t(1);
                } catch (std::bad_alloc&) {
                    delete p;
                    p = NULL;
                    throw;
                }
            } else {
                ++(*pn);
            }
        }
    }

    template <class U>
    void
    release(U*& p) throw()
    {
        if (pn) {
            --(*pn);
            if (*pn == 0) {
                delete p;
                delete pn;
                p = NULL;
            }
            pn = NULL;
        }
    }

public:
    std::size_t* pn;
};

class _shared_ptr_base
{
protected:
    _shared_ptr_base()
        : pn()
    { }

    _shared_ptr_base(
            _shared_ptr_base const& orig)
        : pn(orig.pn)
    { }

    _shared_ptr_count pn;
};

template <class T>
class shared_ptr : public _shared_ptr_base
{
public:
    typedef typename remove_extent<T>::type element_type;

    shared_ptr() throw()
        : _shared_ptr_base(),
          px(NULL)
    { }

    explicit
    shared_ptr(
            T* p)
        : _shared_ptr_base(),
          px(NULL)
    {
        acquire(p);
    }

    shared_ptr(
            nullptr_t) throw()
        : _shared_ptr_base(),
          px(NULL)
    { }

    template <class U>
    explicit
    shared_ptr(
            shared_ptr<U> const& ptr,
            T* p)
        : _shared_ptr_base(ptr),
          px(NULL)
    {
        acquire(p);
    }

    template <class U>
    explicit
    shared_ptr(
            shared_ptr<U> const& orig) throw()
        : _shared_ptr_base(orig),
          px(NULL)
    {
        assert(!orig.get() || (orig.use_count() != 0));
        acquire(static_cast<typename shared_ptr<T>::element_type*>(orig.get()));
    }

    shared_ptr(
            shared_ptr const& orig) throw()
        : _shared_ptr_base(orig),
          px(NULL)
    {
        assert(!orig.px || (orig.use_count() != 0));
        acquire(orig.px);
    }

    inline shared_ptr&
    operator =(shared_ptr ptr) throw()
    {
        swap(ptr);
        return *this;
    }

    inline shared_ptr&
    operator =(nullptr_t) throw()
    {
        reset();
        return *this;
    }

    template <class U>
    inline shared_ptr&
    operator =(shared_ptr<U> const& ptr) throw()
    {
        *this = shared_ptr(ptr);
        return *this;
    }

    ~shared_ptr() throw()
    {
        release();
    }

    inline void
    reset() throw()
    {
        release();
    }

    inline void
    reset(T* p)
    {
        assert(!p || (px != p));
        release();
        acquire(p);
    }

    inline void
    swap(shared_ptr& lhs) throw()
    {
        std::swap(px, lhs.px);
        pn.swap(lhs.pn);
    }

    inline operator
    bool() const throw()
    {
        return use_count() > 0;
    }

    inline bool
    unique() const throw()
    {
        return use_count() == 1;
    }

    inline std::size_t
    use_count() const throw()
    {
        return pn.use_count();
    }

    inline T&
    operator *() const throw()
    {
        assert(NULL != px);
        return *px;
    }

    inline T*
    operator ->() const throw()
    {
        assert(px);
        return px;
    }

    inline T*
    get() const throw()
    {
        return px;
    }

private:
    inline void
    acquire(T* p)
    {
        pn.acquire(p);
        px = p;
    }

    inline void
    release() throw()
    {
        pn.release(px);
    }

private:
    T* px;
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
make_shared(U const& u)
{
    return shared_ptr<T>(shared_ptr<U>(new U(u)));
}
#endif  // C++11+

#ifdef _ARGPARSE_CXX_11
template <class T = std::string, class... Args>
std::vector<T>
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

// -- translations support ----------------------------------------------------
// since v1.7.1
typedef std::map<std::string, std::string> TranslationPack;
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

bool
_have_quotes(
        std::string const& str);

template <class T>
typename enable_if<is_constructible<std::string, T>::value, T>::type
_remove_quotes(
        std::string const& str)
{
    return _have_quotes(str) ? T(str).substr(1, str.size() - 2) : T(str);
}

std::string
_replace(std::string str,
        char old,
        std::string const& value);

std::string
_replace(std::string str,
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

std::vector<std::string>
_split_lines(
        std::string const& str,
        bool keepends = false);

std::vector<std::string>
_split_to_args(
        std::string const& str,
        std::ostream& err = std::cerr);

bool
_string_to_bool(
        std::string const& str) _ARGPARSE_NOEXCEPT;

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
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6))
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
#ifdef _ARGPARSE_CXX_11
    template <class T, typename std::enable_if<
                  is_constructible<std::string, T>::value
                  || is_stl_pair<T>::value
                  || is_stl_tuple<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return name<T>();
    }

    template <class T, typename std::enable_if<
                  is_stl_array<T>::value
                  || is_stl_container<T>::value
                  || is_stl_queue<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return basic<typename T::value_type>();
    }

    template <class T, typename std::enable_if<
                  is_stl_map<T>::value>::type* = nullptr>
    static std::string
    basic()
    {
        return "std::pair<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  !is_constructible<std::string, T>::value
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
    basic(typename enable_if<is_same<std::string, T>::value
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
    basic(typename enable_if<!is_same<std::string, T>::value
                             && !is_stl_container<T>::value
                             && !is_stl_map<T>::value
                             && !is_stl_pair<T>::value
                             && !is_stl_queue<T>::value, bool>::type = true)
    {
        return name<T>();
    }
#endif  // C++11+

#ifdef _ARGPARSE_CXX_11
    template <class T, typename std::enable_if<
                  is_constructible<std::string, T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return "std::string";
    }

    template <class T, typename std::enable_if<
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

    template <class T, typename std::enable_if<
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

    template <class T, typename std::enable_if<
                  is_stl_map<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        auto str = _replace(_get_type_name<T>(), "__1::", std::string());
        return str.substr(0, str.find('<')) + "<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  is_stl_pair<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return "std::pair<" + name<typename T::first_type>()
                + ", " + name<typename T::second_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  is_stl_tuple<T>::value>::type* = nullptr>
    static std::string
    name()
    {
        return tuple_as_string(type_tag<T>{});
    }

    template <class T, typename std::enable_if<
                  !is_constructible<std::string, T>::value
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
    name(typename enable_if<is_same<std::string, T>::value, bool>::type = true)
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
    name(typename enable_if<!is_same<std::string, T>::value
                            && !is_stl_container<T>::value
                            && !is_stl_map<T>::value
                            && !is_stl_pair<T>::value
                            && !is_stl_queue<T>::value, bool>::type = true)
    {
        return _get_type_name<T>();
    }
#endif  // C++11+

private:
#ifdef _ARGPARSE_CXX_11
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
    operator =(Value const& rhs)
    {
        if (this != &rhs) {
            m_value     = rhs.m_value;
            m_has_value = rhs.m_has_value;
        }
        return *this;
    }

    inline Value&
    operator =(T const& rhs)
    {
        m_value     = rhs;
        m_has_value = true;
        return *this;
    }

#ifdef _ARGPARSE_CXX_11
    Value(Value&& orig) _ARGPARSE_NOEXCEPT
        : m_value(std::move(orig.m_value)),
          m_has_value(std::move(orig.m_has_value))
    { }

    explicit
    Value(T&& orig) _ARGPARSE_NOEXCEPT
        : m_value(std::move(orig)),
          m_has_value(true)
    { }

    inline Value&
    operator =(Value&& rhs) _ARGPARSE_NOEXCEPT
    {
        if (this != &rhs) {
            m_value     = std::move(rhs.m_value);
            m_has_value = std::move(rhs.m_has_value);
        }
        return *this;
    }

    inline Value&
    operator =(T&& rhs) _ARGPARSE_NOEXCEPT
    {
        m_value     = std::move(rhs);
        m_has_value = true;
        return *this;
    }
#endif  // C++11+

    inline bool
    operator ==(Value const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return m_has_value == rhs.m_has_value && m_value == rhs.m_value;
    }

    inline bool
    operator ==(T const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return m_has_value && m_value == rhs;
    }

    inline bool
    operator !=(Value const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return !(*this == rhs);
    }

    inline bool
    operator !=(T const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return !(*this == rhs);
    }

    inline void
    reset(T const& value = T())
    {
        m_value     = value;
        m_has_value = false;
    }

    inline bool
    has_value() const _ARGPARSE_NOEXCEPT
    {
        return m_has_value;
    }

    inline T const&
    value() const _ARGPARSE_NOEXCEPT
    {
        return m_value;
    }

    inline T const&
    operator()() const _ARGPARSE_NOEXCEPT
    {
        return m_value;
    }

    inline T const
    value_or(T const& value) const _ARGPARSE_NOEXCEPT
    {
        return has_value() ? this->value() : value;
    }

private:
    // -- data ----------------------------------------------------------------
    T       m_value;
    bool    m_has_value;
};

bool
_is_type_correct(
        std::string const& expected,
        std::string const& received) _ARGPARSE_NOEXCEPT;

void
_check_type(
        Value<std::string> const& expected,
        std::string const& received);

void
_check_non_count_action(
        std::string const& key,
        Action action);
}  // namespace detail

/*!
 *  \brief _ConflictResolver class
 */
struct _ConflictResolver
{
    virtual ~_ConflictResolver() _ARGPARSE_NOEXCEPT { }

    virtual void
    check_conflict_arg(
            Argument const* arg)                                            = 0;
};

/*!
 *  \brief Argument class
 */
_ARGPARSE_EXPORT class Argument
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

    enum Nargs _ARGPARSE_ENUM_TYPE(uint8_t)
    {
        NARGS_DEF       = 0x01,  // ""
        NARGS_NUM       = 0x02,  // "N"
        ONE_OR_MORE     = 0x04,  // "+"
        ZERO_OR_ONE     = 0x08,  // "?"
        ZERO_OR_MORE    = 0x10,  // "*"
        REMAINDING      = 0x20,  // argparse::REMAINDER
        SUPPRESSING     = 0x40,  // argparse::SUPPRESS
    };

    enum Type _ARGPARSE_ENUM_TYPE(uint8_t)
    {
        NoType,
        Positional,
        Operand,
        Optional
    };

    explicit
    Argument(std::vector<std::string> const& flags,
            std::string const& name,
            Type type);

    static detail::shared_ptr<Argument>
    make_argument(
            std::vector<std::string> const& flags,
            std::string const& name,
            Type type);

#ifdef _ARGPARSE_CXX_11
    explicit
    Argument(std::vector<std::string>&& flags,
            std::string&& name,
            Type type);

    static detail::shared_ptr<Argument>
    make_argument(
            std::vector<std::string>&& flags,
            std::string&& name,
            Type type);
#endif  // C++11+

public:
#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
    template <class... Args>
    explicit
    Argument(Args... flags)
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
    Argument(std::initializer_list<std::string> const& flags)
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
    Argument(std::string const& flag);

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flag1 First flag
     *  \param flag2 Second flag
     *
     *  \return Argument object
     */
    explicit
    Argument(std::string const& flag1,
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
    Argument(std::string const& flag1,
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
    Argument(std::string const& flag1,
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
    Argument(std::vector<std::string> const& flags);

    /*!
     *  \brief Construct argument object from another argument
     *
     *  \param orig Argument object to copy
     *
     *  \return Argument object
     */
    Argument(Argument const& orig);

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

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Construct argument object from another argument
     *
     *  \param orig Argument object to move
     *
     *  \return Argument object
     */
    Argument(Argument&& orig) _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Move argument object from another argument
     *
     *  \param rhs Argument object to move
     *
     *  \return Current argument reference
     */
    Argument&
    operator =(
            Argument&& rhs) _ARGPARSE_NOEXCEPT;
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
     *  \param value Nargs value
     *
     *  \return Current argument reference
     */
    Argument&
    nargs(std::size_t value);

    /*!
     *  \brief Set argument 'nargs' value
     *
     *  \param value Nargs value: "?", "*", "+"
     *
     *  \return Current argument reference
     */
    Argument&
    nargs(std::string const& value);

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
     *
     *  \return Current argument reference
     */
    inline Argument&
    optional()
    {
        return nargs("?");
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_one ("?") value
     *
     *  \return Current argument reference
     */
    inline Argument&
    zero_or_one()
    {
        return nargs("?");
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_more ("*") value
     *
     *  \return Current argument reference
     */
    inline Argument&
    zero_or_more()
    {
        return nargs("*");
    }

    /*!
     *  \brief Set argument 'nargs' one_or_more ("+") value
     *
     *  \return Current argument reference
     */
    inline Argument&
    one_or_more()
    {
        return nargs("+");
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
     *  \brief Set argument 'const' value
     *
     *  \param value Const value
     *
     *  \return Current argument reference
     */
    Argument&
    const_value(std::string const& value);

    /*!
     *  \brief Set custom argument 'const' value
     *
     *  \param value Const value
     *
     *  \return Current argument reference
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                !std::is_constructible<std::string, T>::value>::type* = nullptr>
    inline Argument&
    const_value(
            T const& value)
#else
    template <class T>
    inline Argument&
    const_value(
            T const& value,
            typename detail::enable_if<
                !detail::is_constructible<std::string, T>::value, bool
            >::type = true)
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
     *  \brief Set custom argument 'default' value
     *
     *  \param value Default value
     *
     *  \return Current argument reference
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                !std::is_constructible<std::string, T>::value>::type* = nullptr>
    inline Argument&
    default_value(
            T const& value)
#else
    template <class T>
    inline Argument&
    default_value(
            T const& value,
            typename detail::enable_if<
                !detail::is_constructible<std::string, T>::value, bool
            >::type = true)
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
     *
     *  \param value Implicit value
     *
     *  \return Current argument reference
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                !std::is_constructible<std::string, T>::value>::type* = nullptr>
    inline Argument&
    implicit_value(
            T const& value)
#else
    template <class T>
    inline Argument&
    implicit_value(
            T const& value,
            typename detail::enable_if<
                !detail::is_constructible<std::string, T>::value, bool
            >::type = true)
#endif  // C++11+
    {
        m_implicit = detail::_to_string<T>(value);
        m_type_name = detail::Type::basic<T>();
        return *this;
    }

    /*!
     *  \brief Set argument 'type' name
     *  (for MetavarTypeHelpFormatter and Namespace::get<T> type check)
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

#ifdef _ARGPARSE_CXX_11
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
    required(bool value);

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

#ifdef _ARGPARSE_CXX_11
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

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Set argument 'handle' value.
     *  Called when the argument is present and passed the value of the argument
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    Argument&
    handle(std::function<void(std::string const&)> func);

    /*!
     *  \brief Set argument 'handle' value.
     *  Called when the argument is present
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    Argument&
    handle(std::function<void()> func);
#endif  // C++11+

    /*!
     *  \brief Get argument flags values
     *
     *  \return Argument flags values
     */
    _ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    flags() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'action' value (default: argparse::store)
     *
     *  \return Argument 'action' value
     */
    _ARGPARSE_ATTR_NODISCARD
    Action
    action() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'nargs' value
     *
     *  \return Argument 'nargs' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    nargs() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'const' value
     *
     *  \return Argument 'const' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    const_value() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'default' value
     *
     *  \return Argument 'default' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    default_value() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'implicit' value (used with nargs="?" or "*",
     *  const_value alternative for optional arguments with nargs="?")
     *
     *  \return Argument 'implicit' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    implicit_value() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'type' name (for MetavarTypeHelpFormatter)
     *
     *  \return Argument 'type' name
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    type_name() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'choices' value
     *
     *  \return Argument 'choices' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    choices() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'required' value
     *
     *  \return Argument 'required' value
     */
    _ARGPARSE_ATTR_NODISCARD
    bool
    required() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'help' message for default language
     *
     *  \return Argument 'help' message
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    help() const;

    /*!
     *  \brief Get argument 'version' value
     *
     *  \return Argument 'version' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    version() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument 'metavar' value
     *
     *  \return Argument 'metavar' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string
    metavar() const;

    /*!
     *  \brief Get argument 'dest' value
     *
     *  \return Argument 'dest' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    dest() const _ARGPARSE_NOEXCEPT;

private:
    void
    handle(std::string const& str) const;

    void
    validate() const;

    void
    check_action() const;

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
    get_dest() const _ARGPARSE_NOEXCEPT;

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
    get_argument_flags() const _ARGPARSE_NOEXCEPT;

    void
    resolve_conflict_flags(
            std::vector<std::string> const& flags);

    std::string
    error_nargs(
            std::string const& arg) const;

    bool
    is_match_name(
            std::string const& value) const;

    inline bool
    is_suppressed() const _ARGPARSE_NOEXCEPT
    {
        return m_default_type == argparse::SUPPRESS && !m_default.has_value();
    }

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
    Action                      m_action;
    detail::Value<_SUPPRESS>    m_default_type;
    detail::Value<_SUPPRESS>    m_help_type;
    Type                        m_type;
    Nargs                       m_nargs;
    std::size_t                 m_num_args;
    std::string                 m_nargs_str;
    detail::Value<std::string>  m_const;
    detail::Value<std::string>  m_default;
    detail::Value<std::string>  m_implicit;
    detail::Value<std::string>  m_type_name;
    detail::Value<std::vector<std::string> > m_choices;
    detail::TranslationPack     m_help;
    detail::Value<std::string>  m_version;
    detail::Value<std::vector<std::string> > m_metavar;
    std::vector<std::string>    m_dest;
#ifdef _ARGPARSE_CXX_11
    std::function<void(std::string const&)> m_handle;
#endif  // C++11+
    detail::shared_ptr<_ConflictResolver> m_post_trigger;
    detail::Value<bool>         m_required;
};

/*!
 *  \brief _Group class
 */
class _Group
{
    friend class ArgumentParser;

protected:
    explicit
    _Group(std::string const& title,
            std::string const& description);

public:
    /*!
     *  \brief Destroy group
     */
    virtual ~_Group() _ARGPARSE_NOEXCEPT { }

    /*!
     *  \brief Get group 'title' value for default language
     *
     *  \return Group 'title' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    title() const;

    /*!
     *  \brief Get group 'description' value for default language
     *
     *  \return Group 'description' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    description() const;

protected:
    virtual void
    limit_help_flags(
            HelpFormatter const& formatter,
            std::size_t& limit) const                                       = 0;
    virtual void
    print_help(std::ostream& os,
            HelpFormatter const& formatter,
            std::string const& prog,
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
class _ArgumentData : public _ConflictResolver
{
    friend class _ArgumentGroup;
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class MutuallyExclusiveGroup;

    typedef detail::shared_ptr<Argument> pArgument;
    typedef std::list<pArgument>::const_iterator arg_iterator;
    typedef std::list<std::pair<pArgument, bool> >::const_iterator sub_iterator;

    _ArgumentData();

    static detail::shared_ptr<_ArgumentData>
    make_argument_data();

    void
    check_conflict_arg(
            Argument const* arg) _ARGPARSE_OVERRIDE _ARGPARSE_FINAL;

    void
    update_help(bool add_help,
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
    std::string m_conflict_handler;
    std::list<pArgument> m_arguments;
    std::list<std::pair<pArgument, bool> > m_optional;
    std::list<std::pair<pArgument, bool> > m_operand;
    std::list<std::pair<pArgument, bool> > m_positional;
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
            detail::Value<std::string>& argument_default,
            detail::Value<_SUPPRESS>& argument_default_type,
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
    virtual ~_ArgumentGroup() _ARGPARSE_NOEXCEPT { }

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

#ifdef _ARGPARSE_CXX_11
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
    detail::Value<std::string>& m_argument_default;
    detail::Value<_SUPPRESS>& m_argument_default_type;

private:
    bool m_is_mutex_group;
};

/*!
 *  \brief ArgumentGroup class
 */
_ARGPARSE_EXPORT class ArgumentGroup : public _Group, public _ArgumentGroup
{
    friend class ArgumentParser;

    explicit
    ArgumentGroup(
            std::string const& title,
            std::string const& description,
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::Value<std::string>& argument_default,
            detail::Value<_SUPPRESS>& argument_default_type);

    static detail::shared_ptr<ArgumentGroup>
    make_argument_group(
            std::string const& title,
            std::string const& description,
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::Value<std::string>& argument_default,
            detail::Value<_SUPPRESS>& argument_default_type);

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
            std::size_t& limit) const _ARGPARSE_OVERRIDE;

    void
    print_help(
            std::ostream& os,
            HelpFormatter const& formatter,
            std::string const& prog,
            std::size_t limit,
            std::size_t width,
            std::string const& lang) const _ARGPARSE_OVERRIDE;
};

/*!
 *  \brief MutuallyExclusiveGroup class
 */
_ARGPARSE_EXPORT class MutuallyExclusiveGroup : public _ArgumentGroup
{
    friend class ArgumentParser;

    explicit
    MutuallyExclusiveGroup(
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::Value<std::string>& argument_default,
            detail::Value<_SUPPRESS>& argument_default_type);

    static MutuallyExclusiveGroup
    make_mutex_group(
            std::string& prefix_chars,
            pArgumentData& parent_data,
            detail::Value<std::string>& argument_default,
            detail::Value<_SUPPRESS>& argument_default_type);

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
    required(bool value) _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get mutually exclusive group 'required' value
     *
     *  \return Mutually exclusive group 'required' value
     */
    _ARGPARSE_ATTR_NODISCARD
    bool
    required() const _ARGPARSE_NOEXCEPT;

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
    typedef detail::shared_ptr<Argument> pArgument;

    friend class ArgumentParser;
    friend class Namespace;

    class mapped_type
    {
    public:
        mapped_type()
            : m_exists(),
              m_is_default(),
              m_values(),
              m_indexes()
        { }

        explicit
        mapped_type(
                std::vector<std::string> const& values)
            : m_exists(true),
              m_is_default(true),
              m_values(values),
              m_indexes()
        {
            m_indexes.push_back(m_values.size());
        }

        inline void
        clear()
        {
            m_values.clear();
            m_indexes.clear();
            m_exists = false;
            m_is_default = false;
        }

        inline bool
        exists() const _ARGPARSE_NOEXCEPT
        {
            return m_exists;
        }

        inline void
        push_default(
                std::string const& value)
        {
            m_is_default = true;
            push_back(value);
        }

        inline bool
        is_default() const _ARGPARSE_NOEXCEPT
        {
            return m_is_default;
        }

        inline std::vector<std::string> const&
        operator ()() const _ARGPARSE_NOEXCEPT
        {
            return m_values;
        }

        inline std::size_t
        size() const _ARGPARSE_NOEXCEPT
        {
            return m_values.size();
        }

        inline bool
        empty() const _ARGPARSE_NOEXCEPT
        {
            return m_values.empty();
        }

        inline std::string const&
        front() const _ARGPARSE_NOEXCEPT
        {
            return m_values.front();
        }

        inline std::string const&
        at(std::size_t i) const
        {
            return m_values.at(i);
        }

#ifdef _ARGPARSE_CXX_11
        inline void
        emplace_back(
                std::string&& value)
        {
            m_values.emplace_back(std::move(value));
            m_indexes.push_back(m_values.size());
            m_exists = true;
        }
#endif  // C++11+

        inline void
        push_back(
                std::string const& value)
        {
            m_values.push_back(value);
            m_indexes.push_back(m_values.size());
            m_exists = true;
        }

        inline void
        push_values(
                std::vector<std::string> const& values)
        {
            m_values.reserve(m_values.size() + values.size());
            m_values.insert(m_values.end(), values.begin(), values.end());
            m_indexes.push_back(m_values.size());
            m_exists = true;
        }

        inline std::vector<std::size_t> const&
        indexes() const _ARGPARSE_NOEXCEPT
        {
            return m_indexes;
        }

    private:
        // -- data ------------------------------------------------------------
        bool m_exists;
        bool m_is_default;
        std::vector<std::string> m_values;
        std::vector<std::size_t> m_indexes;
    };

    typedef pArgument                               key_type;
    typedef std::pair<key_type, mapped_type>        value_type;
    typedef std::list<value_type>                   map_type;
    typedef map_type::iterator                      iterator;
    typedef map_type::const_iterator                const_iterator;

    _Storage()
        : m_data()
    { }

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

    inline bool
    exists_arg(
            std::string const& key) const
    {
        return find_arg(key) != end();
    }

    inline bool
    exists(std::string const& key) const
    {
        return find(key) != end();
    }

    inline bool
    exists(key_type const& key) const
    {
        return find(key) != end();
    }

    inline value_type const&
    at(std::string const& key) const
    {
        const_iterator it = find(key);
        if (it == end()) {
            throw std::logic_error("key '" + key + "' not found");
        }
        return *it;
    }

    inline mapped_type&
    at(key_type const& key)
    {
        iterator it = find(key);
        if (it == end()) {
            throw std::logic_error("key '" + key->m_name + "' not found");
        }
        return it->second;
    }

    inline mapped_type const&
    at(key_type const& key) const
    {
        const_iterator it = find(key);
        if (it == end()) {
            throw std::logic_error("key '" + key->m_name + "' not found");
        }
        return it->second;
    }

    inline iterator
    erase(iterator it)
    {
        return m_data.erase(it);
    }

    inline iterator
    begin() _ARGPARSE_NOEXCEPT
    {
        return m_data.begin();
    }

    inline iterator
    end() _ARGPARSE_NOEXCEPT
    {
        return m_data.end();
    }

    inline const_iterator
    begin() const _ARGPARSE_NOEXCEPT
    {
        return m_data.begin();
    }

    inline const_iterator
    end() const _ARGPARSE_NOEXCEPT
    {
        return m_data.end();
    }

    inline const_iterator
    find_arg(std::string const& key) const
    {
        const_iterator it = find(key);
        if (it != end()) {
            return it;
        }
        for (it = begin(); it != end(); ++it) {
            if (it->first->is_match_name(key)) {
                return it;
            }
        }
        return end();
    }

    inline const_iterator
    find(std::string const& key) const
    {
        for (const_iterator it = begin(); it != end(); ++it) {
            if (*(it->first) == key) {
                return it;
            }
        }
        return end();
    }

    inline const_iterator
    find(key_type const& key) const
    {
        for (const_iterator it = begin(); it != end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return end();
    }

    inline iterator
    find(key_type const& key)
    {
        for (iterator it = begin(); it != end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return end();
    }

    void
    on_process_store(
            key_type const& key,
            std::string const& value);

    void
    on_process_store(
            key_type const& key,
            std::vector<std::string> const& values);

    // -- data ----------------------------------------------------------------
    map_type m_data;
};

/*!
 *  \brief Object with parsed arguments
 */
_ARGPARSE_EXPORT class Namespace
{
    friend class ArgumentParser;

    explicit
    Namespace(
            _Storage const& storage = _Storage());

    explicit
    Namespace(
            _Storage const& storage,
            std::vector<std::string> const& args);

#ifdef _ARGPARSE_CXX_11
    explicit
    Namespace(
            _Storage&& storage) _ARGPARSE_NOEXCEPT;

    explicit
    Namespace(
            _Storage&& storage,
            std::vector<std::string>&& args) _ARGPARSE_NOEXCEPT;
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
    _ARGPARSE_ATTR_NODISCARD
    bool
    contains(std::string const& key) const;

    /*!
     *  \brief Check if argument name exists and specified in parsed arguments
     *
     *  \param key Argument destination name or flag
     *
     *  \return True if argument name exists and specified, otherwise false
     */
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<detail::is_constructible<std::string, T>::value
                               || detail::is_floating_point<T>::value
                               || detail::is_same<bool, T>::value
                               || detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::name<T>());
        detail::_check_non_count_action(key, args.first->action());
        if (args.second.empty()) {
            return T();
        }
        if (args.second.size() != 1) {
            throw TypeError("got a data-array for argument '" + key + "'");
        }
        return to_type<T>(args.second.front());
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
    _ARGPARSE_ATTR_NODISCARD
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
        if (args.second.empty()) {
            return T();
        }
        if (args.second.size() != 1) {
            throw TypeError("got a data-array for argument '" + key + "'");
        }
        return to_type<T>(args.second.front());
    }

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Get parsed argument value as std array type.
     *  If argument not parsed, returns array of default values.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    typename std::enable_if<
        detail::is_stl_array<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        auto vector = to_vector<typename T::value_type>(args.second());
        T res{};
        if (res.size() != vector.size()) {
            std::cerr << "argparse error [skip]: array size mismatch: was "
                      << res.size() << ", expected " << vector.size()
                      << std::endl;
        }
        auto size = res.size();
        if (size > vector.size()) {
            size = vector.size();
        }
        std::copy_n(vector.begin(), size, res.begin());
        return res;
    }
#endif  // C++11+

    /*!
     *  \brief Get parsed argument value as std containers types.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_container<typename detail::decay<T>::type>::value
#ifdef _ARGPARSE_CXX_11
     && !detail::is_stl_container_tupled<typename std::decay<T>::type>::value
#endif  // C++11+
     && !detail::is_stl_container_paired<typename detail::decay<T>::type>::value
     && !detail::is_stl_matrix<typename detail::decay<T>::type>::value
     && !detail::is_stl_matrix_queue<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        typedef typename T::value_type V;
        std::vector<V> vector = to_vector<V>(args.second().begin(),
                                             args.second().end());
        return T(vector.begin(), vector.end());
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
    _ARGPARSE_ATTR_NODISCARD
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
                = to_paired_vector<K, V>(args.second(), sep);
        return T(vector.begin(), vector.end());
    }

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_NODISCARD
    typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value, T
    >::type
    get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        auto vector = to_tupled_vector<
                typename T::value_type>(args.second(), sep);
        return T(vector.begin(), vector.end());
    }
#endif  // C++11+

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
    _ARGPARSE_ATTR_NODISCARD
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
                = to_paired_vector<K, V>(args.second(), sep);
        for (std::size_t i = 0; i < vector.size(); ++i) {
            res.insert(std::make_pair(vector.at(i).first, vector.at(i).second));
        }
        return res;
    }

    /*!
     *  \brief Get parsed argument value as 2D deque, list, vector of not queue.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_matrix<typename detail::decay<T>::type>::value
     && !detail::is_stl_matrix_queue<typename detail::decay<T>::type>::value, T
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
        typedef typename T::value_type V;
        typedef typename T::value_type::value_type VV;
        T res = T();
        for (std::size_t i = 0; i < args.second.indexes().size(); ++i) {
            typedef std::vector<std::string>::difference_type dtype;
            std::vector<VV> vector = to_vector<VV>(
                        args.second().begin()
                            + static_cast<dtype>(
                                i == 0 ? 0 : args.second.indexes().at(i - 1)),
                        args.second().begin()
                            + static_cast<dtype>(args.second.indexes().at(i)));
            res.push_back(V(vector.begin(), vector.end()));
        }
        return res;
    }

    /*!
     *  \brief Get parsed argument value as 2D deque, list, vector of queues.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_matrix_queue<typename detail::decay<T>::type>::value, T
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
        typedef typename T::value_type V;
        typedef typename T::value_type::value_type VV;
        T res = T();
        for (std::size_t i = 0; i < args.second.indexes().size(); ++i) {
            typedef std::vector<std::string>::difference_type dtype;
            std::vector<VV> vector = to_vector<VV>(
                        args.second().begin()
                            + static_cast<dtype>(
                                i == 0 ? 0 : args.second.indexes().at(i - 1)),
                        args.second().begin()
                            + static_cast<dtype>(args.second.indexes().at(i)));
            res.push_back(V(std::deque<VV>(vector.begin(), vector.end())));
        }
        return res;
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
    _ARGPARSE_ATTR_NODISCARD
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
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (args.second.size() != 2) {
                throw
                TypeError("invalid data for paired argument '" + key + "'");
            }
            return std::make_pair(to_type<K>(args.second.front()),
                                  to_type<V>(args.second.at(1)));
        }
        if (args.second.size() != 1) {
            throw TypeError("got a data-array for argument '" + key + "'");
        }
        return to_pair<K, V>(args.second.front(), sep);
    }

    /*!
     *  \brief Get parsed argument value as queue types.
     *  If argument not parsed, returns empty queue.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        detail::is_stl_queue<typename detail::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type(args.first->m_type_name, detail::Type::basic<T>());
        detail::_check_non_count_action(key, args.first->action());
        typedef typename T::value_type V;
        std::vector<V> vector = to_vector<V>(args.second());
        return T(std::deque<V>(vector.begin(), vector.end()));
    }

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_NODISCARD
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
        if (std::isspace(static_cast<unsigned char>(sep))) {
            return to_tuple(detail::type_tag<T>{}, args.second());
        }
        if (args.second.size() != 1) {
            throw TypeError("got a data-array for argument '" + key + "'");
        }
        return to_tuple(
                    detail::type_tag<T>{},
                    detail::_split(args.second.front(), std::string(1, sep)));
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
    _ARGPARSE_ATTR_NODISCARD
    typename detail::enable_if<
        !detail::is_constructible<std::string, T>::value
        && !detail::is_floating_point<T>::value
        && !detail::is_integral<T>::value
#ifdef _ARGPARSE_CXX_11
        && !detail::is_stl_array<typename std::decay<T>::type>::value
        && !detail::is_stl_tuple<typename std::decay<T>::type>::value
#endif  // C++11+
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
        return to_type<T>(detail::_join(args.second()));
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
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_NODISCARD
    std::string
    to_string(std::string const& key,
                std::string const& quotes = std::string()) const;

    /*!
     *  \brief Get namespace as string
     *
     *  \return Namespace as string
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string
    to_string() const;

#ifdef _ARGPARSE_CXX_17
    /*!
     *  \brief Try get parsed argument value as boolean, byte, floating point
     *  or string types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        std::is_constructible<std::string, T>::value
        || std::is_floating_point<T>::value
        || std::is_same<bool, T>::value
        || detail::is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || args->second.size() != 1
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::name<T>())) {
            return std::nullopt;
        }
        return try_to_type<T>(args->second.front());
    }

    /*!
     *  \brief Try get parsed argument value as integer types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        std::is_integral<T>::value
        && !std::is_same<bool, T>::value
        && !detail::is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::name<T>())) {
            return std::nullopt;
        }
        if (args->first->action() == argparse::count) {
            return static_cast<T>(args->second.size());
        }
        if (args->second.size() != 1) {
            return std::nullopt;
        }
        return try_to_type<T>(args->second.front());
    }

    /*!
     *  \brief Try get parsed argument value as std array type.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_array<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
        if (!vector.has_value()) {
            return std::nullopt;
        }
        T res{};
        if (res.size() != vector->size()) {
            std::cerr << "argparse error [skip]: array size mismatch: was "
                      << res.size() << ", expected " << vector->size()
                      << std::endl;
        }
        auto size = res.size();
        if (size > vector->size()) {
            size = vector->size();
        }
        std::copy_n(vector.value().begin(), size, res.begin());
        return res;
    }

    /*!
     *  \brief Try get parsed argument value as std containers types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_container<typename std::decay<T>::type>::value
        && !detail::is_stl_container_paired<typename std::decay<T>::type>::value
        && !detail::is_stl_container_tupled<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        auto vector = try_to_vector<typename T::value_type>(
                    args->second().begin(), args->second().end());
        if (!vector.has_value()) {
            return std::nullopt;
        }
        return T(vector.value().begin(), vector.value().end());
    }

    /*!
     *  \brief Try get parsed argument value as paired container types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_container_paired<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::value_type::first_type K;
        typedef typename T::value_type::second_type V;
        auto vector = try_to_paired_vector<K, V>(args->second(), sep);
        if (!vector.has_value()) {
            return std::nullopt;
        }
        return T(vector.value().begin(), vector.value().end());
    }

    /*!
     *  \brief Try get parsed argument value as tupled container types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        auto vector = try_to_tupled_vector<
                typename T::value_type>(args->second(), sep);
        if (!vector.has_value()) {
            return std::nullopt;
        }
        return T(vector.value().begin(), vector.value().end());
    }

    /*!
     *  \brief Try get parsed argument value as mapped types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_map<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::key_type K;
        typedef typename T::mapped_type V;
        T res{};
        auto vector = try_to_paired_vector<K, V>(args->second(), sep);
        if (!vector.has_value()) {
            return std::nullopt;
        }
        for (auto const& pair : vector.value()) {
            res.emplace(std::make_pair(pair.first, pair.second));
        }
        return res;
    }

    /*!
     *  \brief Try get parsed argument value as 2D deque, list, vector
     *  of not queues.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() != argparse::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::value_type V;
        typedef typename T::value_type::value_type VV;
        T res{};
        for (std::size_t i = 0; i < args->second.indexes().size(); ++i) {
            typedef std::vector<std::string>::difference_type dtype;
            auto vector = try_to_vector<VV>(
                        args->second().begin()
                            + static_cast<dtype>(
                                i == 0 ? 0 : args->second.indexes().at(i - 1)),
                        args->second().begin()
                            + static_cast<dtype>(args->second.indexes().at(i)));
            if (!vector.has_value()) {
                return std::nullopt;
            }
            res.push_back(V(vector.value().begin(), vector.value().end()));
        }
        return res;
    }

    /*!
     *  \brief Try get parsed argument value as 2D deque, list, vector
     *  of queues.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() != argparse::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::value_type V;
        typedef typename T::value_type::value_type VV;
        T res{};
        for (std::size_t i = 0; i < args->second.indexes().size(); ++i) {
            typedef std::vector<std::string>::difference_type dtype;
            auto vector = try_to_vector<VV>(
                        args->second().begin()
                            + static_cast<dtype>(
                                i == 0 ? 0 : args->second.indexes().at(i - 1)),
                        args->second().begin()
                            + static_cast<dtype>(args->second.indexes().at(i)));
            if (!vector.has_value()) {
                return std::nullopt;
            }
            res.push_back(V(std::deque<VV>(vector.value().begin(),
                                           vector.value().end())));
        }
        return res;
    }

    /*!
     *  \brief Try get parsed argument value as paired types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_pair<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || args->second.empty()
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::name<T>())) {
            return std::nullopt;
        }
        typedef typename T::first_type K;
        typedef typename T::second_type V;
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (args->second.size() != 2) {
                return std::nullopt;
            }
            auto el1 = try_to_type<K>(args->second.at(0));
            auto el2 = try_to_type<V>(args->second.at(1));
            if (el1.has_value() && el2.has_value()) {
                return std::make_pair(el1.value(), el2.value());
            } else {
                return std::nullopt;
            }
        }
        if (args->second.size() != 1) {
            return std::nullopt;
        }
        return try_to_pair<K, V>(args->second.front(), sep);
    }

    /*!
     *  \brief Try get parsed argument value as queue types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_queue<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::value_type V;
        auto vector = try_to_vector<V>(args->second());
        if (!vector.has_value()) {
            return std::nullopt;
        }
        return T(std::deque<V>(vector.value().begin(), vector.value().end()));
    }

    /*!
     *  \brief Try get parsed argument value as tuple types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *  \param sep Separator (default: '=')
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key,
            char sep = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || args->second.empty()
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::name<T>())) {
            return std::nullopt;
        }
        if (std::isspace(static_cast<unsigned char>(sep))) {
            return try_to_tuple(detail::type_tag<T>{}, args->second());
        }
        if (args->second.size() != 1) {
            return std::nullopt;
        }
        return try_to_tuple(
                    detail::type_tag<T>{},
                    detail::_split(args->second.front(), std::string(1, sep)));
    }

    /*!
     *  \brief Try get parsed argument value as custom type.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    _ARGPARSE_ATTR_NODISCARD
    std::optional<typename std::enable_if<
        !std::is_constructible<std::string, T>::value
        && !std::is_floating_point<T>::value
        && !std::is_integral<T>::value
        && !detail::is_stl_array<typename std::decay<T>::type>::value
        && !detail::is_stl_container<typename std::decay<T>::type>::value
        && !detail::is_stl_map<typename std::decay<T>::type>::value
        && !detail::is_stl_pair<typename std::decay<T>::type>::value
        && !detail::is_stl_queue<typename std::decay<T>::type>::value
        && !detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.has_value()
                || args->first->action() == argparse::count
                || args->second.empty()
                || !detail::_is_type_correct(args->first->type_name(),
                                             detail::Type::name<T>())) {
            return std::nullopt;
        }
        return try_to_type<T>(detail::_join(args->second()));
    }
#endif  // C++17+

    /*!
     *  \brief Get unrecognized arguments
     *
     *  \return Unrecognized arguments
     */
    _ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    unrecognized_args() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get unrecognized arguments as args string
     *
     *  \return Unrecognized arguments as args string
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string
    unrecognized_args_to_args() const;

private:
    std::string
    boolean_option_to_args(
            std::string const& key,
            _Storage::value_type const& args) const;

    std::string
    boolean_option_to_string(
            std::string const& key,
            _Storage::value_type const& args,
            std::string const& quotes) const;

    std::string
    store_actions_to_string(
            _Storage::value_type const& args,
            std::string const& quotes) const;

    _Storage::value_type const&
    data(std::string const& key) const;

    _Storage const&
    storage() const _ARGPARSE_NOEXCEPT;

    template <class T, class U>
    std::pair<T, U>
    to_pair(std::string const& data,
            char sep) const
    {
        std::vector<std::string> pair
                = detail::_split(data, std::string(1, sep), 1);
        pair.resize(2);
        return std::make_pair(to_type<T>(pair.at(0)), to_type<U>(pair.at(1)));
    }

    template <class T, class U>
    std::vector<std::pair<T, U> >
    to_paired_vector(
            std::vector<std::string> const& args,
            char sep) const
    {
        std::vector<std::pair<T, U> > vec;
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (args.size() & 1) {
                throw TypeError("invalid stored argument amount");
            }
            vec.reserve(args.size() / 2);
            for (std::size_t i = 0; i < args.size(); i += 2) {
                vec.push_back(std::make_pair(to_type<T>(args.at(i)),
                                             to_type<U>(args.at(i + 1))));
            }
        } else {
            vec.reserve(args.size());
            for (std::size_t i = 0; i < args.size(); ++i) {
                vec.push_back(to_pair<T, U>(args.at(i), sep));
            }
        }
        return vec;
    }

    typedef std::vector<std::string>::const_iterator value_const_iterator;

#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                  std::is_constructible<std::string, T>::value
                  || std::is_floating_point<T>::value
                  || std::is_integral<T>::value>::type* = nullptr>
    std::vector<T>
    to_vector(
            value_const_iterator beg,
            value_const_iterator end) const
#else
    template <class T>
    std::vector<T>
    to_vector(
            value_const_iterator beg,
            value_const_iterator end,
            typename detail::enable_if<
                detail::is_constructible<std::string, T>::value
                || detail::is_floating_point<T>::value
                || detail::is_integral<T>::value, bool>::type = true) const
#endif  // C++11+
    {
        std::vector<T> vec;
        vec.reserve(static_cast<std::size_t>(end - beg));
        for (value_const_iterator it = beg; it != end; ++it) {
            vec.push_back(to_type<T>(*it));
        }
        return vec;
    }

#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                  !std::is_constructible<std::string, T>::value
                  && !std::is_floating_point<T>::value
                  && !std::is_integral<T>::value>::type* = nullptr>
    std::vector<T>
    to_vector(
            value_const_iterator beg,
            value_const_iterator end) const
#else
    template <class T>
    std::vector<T>
    to_vector(
            value_const_iterator beg,
            value_const_iterator end,
            typename detail::enable_if<
                !detail::is_constructible<std::string, T>::value
                && !detail::is_floating_point<T>::value
                && !detail::is_integral<T>::value, bool>::type = true) const
#endif  // C++11+
    {
        std::vector<T> vec;
        if (end == beg) {
            return vec;
        }
        std::string data;
        for (value_const_iterator it = beg; it != end; ++it) {
            std::string value = detail::_remove_quotes<std::string>(*it);
            if (!data.empty() && !value.empty()) {
                data += detail::_spaces;
            }
            data += value;
        }
        T res = T();
        std::stringstream ss(data);
        while (!ss.eof()) {
            ss >> res;
            if (ss.fail()) {
                throw TypeError("invalid " + detail::Type::name<T>()
                                + " value: '" + data + "'");
            }
            vec.push_back(res);
        }
        return vec;
    }

#ifdef _ARGPARSE_CXX_11
    template <class... Ts, std::size_t... Idxs>
    std::tuple<Ts...>
    parse_tuple(
            std::vector<std::string> const& values,
            detail::integer_sequence<std::size_t, Idxs...>) const
    {
        return std::make_tuple(to_type<Ts>(values[Idxs])...);
    }

    template <class... Ts>
    std::tuple<Ts...>
    to_tuple(detail::type_tag<std::tuple<Ts...> >,
            std::vector<std::string> const& values) const
    {
        return parse_tuple<Ts...>(
                    values, detail::make_integer_sequence<std::size_t,
                                                              sizeof...(Ts)>());
    }

    template <class T>
    std::vector<T>
    to_tupled_vector(
            std::vector<std::string> const& args,
            char sep) const
    {
        std::vector<T> vec;
        if (std::isspace(static_cast<unsigned char>(sep))) {
            auto const size = std::tuple_size<T>{};
            if (size == 0 || args.size() % size != 0) {
                throw TypeError("invalid stored argument amount");
            }
            vec.reserve(args.size() / size);
            for (std::size_t i = 0; i < args.size(); i += size) {
                std::vector<std::string> temp
                        = { args.begin() + i, args.begin() + i + size };
                vec.emplace_back(to_tuple(detail::type_tag<T>{}, temp));
            }
        } else {
            vec.reserve(args.size());
            std::transform(args.begin(), args.end(), std::back_inserter(vec),
                           [this, sep] (std::string const& a)
            { return to_tuple(detail::type_tag<T>{},
                              detail::_split(a, std::string(1, sep))); });
        }
        return vec;
    }
#endif  // C++11+

    template <class T>
    typename detail::enable_if<
        detail::is_constructible<std::string, T>::value, T>::type
    to_type(std::string const& data) const
    {
        return detail::_remove_quotes<T>(data);
    }

    template <class T>
    typename detail::enable_if<detail::is_same<bool, T>::value, T>::type
    to_type(std::string const& data) const _ARGPARSE_NOEXCEPT
    {
        return detail::_string_to_bool(data);
    }

    template <class T>
    typename detail::enable_if<detail::is_byte_type<T>::value, T>::type
    to_type(std::string const& data) const
    {
        if (data.empty()) {
            return T();
        }
        if (data.size() != 1) {
            throw TypeError("got a data-array in value '" + data + "'");
        }
        return static_cast<T>(data.at(0));
    }

    template <class T>
    typename detail::enable_if<!detail::is_constructible<std::string, T>::value
                               && !detail::is_same<bool, T>::value
                               && !detail::is_byte_type<T>::value, T>::type
    to_type(std::string const& data) const
    {
        if (data.empty()) {
            return T();
        }
        T res = T();
        std::stringstream ss(detail::_remove_quotes<std::string>(data));
        ss >> res;
        if (ss.fail() || !ss.eof()) {
            throw TypeError("invalid " + detail::Type::name<T>()
                            + " value: '" + data + "'");
        }
        return res;
    }

#ifdef _ARGPARSE_CXX_17
    inline std::optional<_Storage::value_type>
    try_get_data(
            std::string const& key) const
    {
        auto it = storage().find_arg(key);
        if (it != storage().end()) {
            return *it;
        }
        return std::nullopt;
    }

    template <class T, class U>
    std::optional<std::pair<T, U> >
    try_to_pair(
            std::string const& data,
            char sep) const
    {
        auto pair = detail::_split(data, std::string(1, sep), 1);
        pair.resize(2);
        auto el1 = try_to_type<T>(pair.at(0));
        auto el2 = try_to_type<U>(pair.at(1));
        if (el1.has_value() && el2.has_value()) {
            return std::make_pair(el1.value(), el2.value());
        } else {
            return std::nullopt;
        }
    }

    template <class T, class U>
    std::optional<std::vector<std::pair<T, U> > >
    try_to_paired_vector(
            std::vector<std::string> const& args,
            char sep) const
    {
        std::vector<std::pair<T, U> > vec;
        if (std::isspace(static_cast<unsigned char>(sep))) {
            if (args.size() & 1) {
                return std::nullopt;
            }
            vec.reserve(args.size() / 2);
            for (std::size_t i = 0; i < args.size(); i += 2) {
                auto el1 = try_to_type<T>(args.at(i));
                auto el2 = try_to_type<U>(args.at(i + 1));
                if (el1.has_value() && el2.has_value()) {
                    vec.emplace_back(std::make_pair(el1.value(), el2.value()));
                } else {
                    return std::nullopt;
                }
            }
        } else {
            vec.reserve(args.size());
            for (auto const& arg : args) {
                auto pair = try_to_pair<T, U>(arg, sep);
                if (!pair.has_value()) {
                    return std::nullopt;
                }
                vec.emplace_back(pair.value());
            }
        }
        return vec;
    }

    template <class T,
              typename std::enable_if<
                  std::is_constructible<std::string, T>::value
                  || std::is_floating_point<T>::value
                  || std::is_integral<T>::value>::type* = nullptr>
    std::optional<std::vector<T> >
    try_to_vector(
            value_const_iterator beg,
            value_const_iterator end) const
    {
        std::vector<T> vec;
        vec.reserve(static_cast<std::size_t>(end - beg));
        for (value_const_iterator it = beg; it != end; ++it) {
            auto el = try_to_type<T>(*it);
            if (!el.has_value()) {
                return std::nullopt;
            }
            vec.emplace_back(el.value());
        }
        return vec;
    }

    template <class T,
              typename std::enable_if<
                  !std::is_constructible<std::string, T>::value
                  && !std::is_floating_point<T>::value
                  && !std::is_integral<T>::value>::type* = nullptr>
    std::optional<std::vector<T> >
    try_to_vector(
            value_const_iterator beg,
            value_const_iterator end) const
    {
        std::vector<T> vec;
        if (end == beg) {
            return vec;
        }
        std::string data;
        for (value_const_iterator it = beg; it != end; ++it) {
            auto value = detail::_remove_quotes<std::string>(*it);
            if (!data.empty() && !value.empty()) {
                data += detail::_spaces;
            }
            data += value;
        }
        T res{};
        std::stringstream ss(data);
        while (!ss.eof()) {
            ss >> res;
            if (ss.fail()) {
                return std::nullopt;
            }
            vec.push_back(res);
        }
        return vec;
    }

    template <class... Ts>
    std::optional<std::tuple<Ts...> >
    try_to_tuple(
            detail::type_tag<std::tuple<Ts...> >,
            std::vector<std::string> const& values) const
    {
        try {
            auto res = parse_tuple<Ts...>(
                        values,
                        detail::make_integer_sequence<std::size_t,
                                                              sizeof...(Ts)>());
            return res;
        } catch (...) {
            return std::nullopt;
        }
    }

    template <class T>
    std::optional<std::vector<T> >
    try_to_tupled_vector(
            std::vector<std::string> const& args,
            char sep) const
    {
        std::vector<T> vec;
        if (std::isspace(static_cast<unsigned char>(sep))) {
            auto const size = std::tuple_size<T>{};
            if (size == 0 || args.size() % size != 0) {
                return std::nullopt;
            }
            vec.reserve(args.size() / size);
            for (std::size_t i = 0; i < args.size(); i += size) {
                std::vector<std::string> temp
                        = { args.begin() + i, args.begin() + i + size };
                auto tuple = try_to_tuple(detail::type_tag<T>{}, temp);
                if (!tuple.has_value()) {
                    return std::nullopt;
                }
                vec.emplace_back(tuple.value());
            }
        } else {
            vec.reserve(args.size());
            for (auto const& arg : args) {
                auto tuple = try_to_tuple(
                            detail::type_tag<T>{},
                            detail::_split(arg, std::string(1, sep)));
                if (!tuple.has_value()) {
                    return std::nullopt;
                }
                vec.emplace_back(tuple.value());
            }
        }
        return vec;
    }

    template <class T>
    std::optional<typename std::enable_if<
        std::is_constructible<std::string, T>::value, T>::type>
    try_to_type(
            std::string const& data) const
    {
        return detail::_remove_quotes<T>(data);
    }

    template <class T>
    std::optional<typename std::enable_if<
        std::is_same<bool, T>::value, T>::type>
    try_to_type(
            std::string const& data) const _ARGPARSE_NOEXCEPT
    {
        return detail::_string_to_bool(data);
    }

    template <class T>
    std::optional<typename std::enable_if<
        detail::is_byte_type<T>::value, T>::type>
    try_to_type(
            std::string const& data) const _ARGPARSE_NOEXCEPT
    {
        if (data.size() != 1) {
            return std::nullopt;
        }
        return static_cast<T>(data.front());
    }

    template <class T>
    std::optional<typename std::enable_if<
        !std::is_constructible<std::string, T>::value
        && !std::is_same<bool, T>::value
        && !detail::is_byte_type<T>::value, T>::type>
    try_to_type(
            std::string const& data) const
    {
        if (data.empty()) {
            return T{};
        }
        T res{};
        std::stringstream ss(detail::_remove_quotes<std::string>(data));
        ss >> res;
        if (ss.fail() || !ss.eof()) {
            return std::nullopt;
        }
        return res;
    }
#endif  // C++17+

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
 *  \brief ArgumentParser objects
 */
_ARGPARSE_EXPORT class ArgumentParser
{
    typedef detail::shared_ptr<Argument> pArgument;
    typedef std::vector<pArgument> pArguments;
    typedef detail::shared_ptr<_ArgumentData> pArgumentData;
    typedef detail::shared_ptr<_Group> pGroup;
    typedef detail::shared_ptr<ArgumentParser> pParser;
    typedef std::list<pArgument>::const_iterator arg_iterator;
    typedef std::list<std::pair<pArgument, bool> >::const_iterator sub_iterator;
    typedef std::list<pGroup>::const_iterator grp_iterator;
    typedef std::list<MutuallyExclusiveGroup>::const_iterator mtx_it;

public:
    /*!
     *  \brief Subparser class
     */
    class Subparser : public _Group
    {
        friend class ArgumentParser;

        explicit
        Subparser(
                std::string const& title,
                std::string const& description);

        static detail::shared_ptr<Subparser>
        make_subparser(
                std::string const& title,
                std::string const& description);

    public:
        using _Group::title;
        using _Group::description;

        /*!
         *  \brief Set subparser 'title' value for selected language
         *
         *  \param value Title value
         *  \param lang Language value (default: "")
         *
         *  \return Current subparser reference
         */
        Subparser&
        title(std::string const& value,
                std::string const& lang = std::string());

        /*!
         *  \brief Set subparser 'description' value for selected language
         *
         *  \param value Description value
         *  \param lang Language value (default: "")
         *
         *  \return Current subparser reference
         */
        Subparser&
        description(
                std::string const& value,
                std::string const& lang = std::string());

        /*!
         *  \brief Set subparser 'prog' value
         *
         *  \param value Program value
         *
         *  \return Current subparser reference
         */
        Subparser&
        prog(std::string const& value);

        /*!
         *  \brief Set subparser 'dest' value
         *
         *  \param value Destination value
         *
         *  \return Current subparser reference
         */
        Subparser&
        dest(std::string const& value);

        /*!
         *  \brief Set subparser 'required' value
         *
         *  \param value Required flag
         *
         *  \return Current subparser reference
         */
        Subparser&
        required(bool value) _ARGPARSE_NOEXCEPT;

        /*!
         *  \brief Set subparser 'help' message for selected language
         *
         *  \param value Help message
         *  \param lang Language value (default: "")
         *
         *  \return Current subparser reference
         */
        Subparser&
        help(std::string const& value,
                std::string const& lang = std::string());

        /*!
         *  \brief Suppress subparser 'help' message
         *
         *  \param value argparse::SUPPRESS
         *
         *  \since v1.7.3
         *
         *  \return Current subparser reference
         */
        Subparser&
        help(_SUPPRESS value);

        /*!
         *  \brief Set subparser 'metavar' value
         *
         *  \param value Metavar value
         *
         *  \return Current subparser reference
         */
        Subparser&
        metavar(std::string const& value);

        /*!
         *  \brief Get subparser 'prog' value
         *
         *  \return Subparser 'prog' value
         */
        _ARGPARSE_ATTR_NODISCARD
        std::string const&
        prog() const _ARGPARSE_NOEXCEPT;

        /*!
         *  \brief Get subparser 'dest' value
         *
         *  \return Subparser 'dest' value
         */
        _ARGPARSE_ATTR_NODISCARD
        std::string const&
        dest() const _ARGPARSE_NOEXCEPT;

        /*!
         *  \brief Get subparser 'required' value
         *
         *  \return Subparser 'required' value
         */
        _ARGPARSE_ATTR_NODISCARD
        bool
        required() const _ARGPARSE_NOEXCEPT;

        /*!
         *  \brief Get subparser 'help' message for default language
         *
         *  \return Subparser 'help' message
         */
        _ARGPARSE_ATTR_NODISCARD
        std::string const&
        help() const;

        /*!
         *  \brief Get subparser 'metavar' value
         *
         *  \return Subparser 'metavar' value
         */
        _ARGPARSE_ATTR_NODISCARD
        std::string const&
        metavar() const _ARGPARSE_NOEXCEPT;

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
        _ARGPARSE_ATTR_NODISCARD
        std::vector<std::string>
        parser_names() const;

    private:
        void
        limit_help_flags(
                HelpFormatter const&,
                std::size_t& limit) const _ARGPARSE_OVERRIDE;

        void
        print_help(
                std::ostream& os,
                HelpFormatter const& formatter,
                std::string const& prog,
                std::size_t limit,
                std::size_t width,
                std::string const& lang) const _ARGPARSE_OVERRIDE;

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

        std::string
        print(HelpFormatter const& formatter,
                std::size_t limit,
                std::size_t width,
                std::string const& lang) const;

        // -- data ------------------------------------------------------------
        std::string m_parent_prog;
        std::string m_parent_args;
        std::string m_prog;
        std::string m_dest;
        detail::TranslationPack m_help;
        detail::Value<std::string> m_metavar;
        std::deque<pParser> m_parsers;
        detail::Value<_SUPPRESS> m_help_type;
        bool        m_required;
    };

private:
    typedef detail::shared_ptr<Subparser> pSubparser;
    typedef std::pair<pSubparser, std::size_t> SubparserInfo;

    static pParser
    make_parser(
            std::string const& name);

    void
    read_args(int argc,
            char const* const argv[]);

    void
    read_env(char const* const envp[]);

    void
    initialize_parser();

public:
    /*!
     *  \brief Construct argument parser with concrete program name
     *
     *  \param prog Program name (default: "untitled")
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(
            std::string const& prog = "untitled");

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
    virtual ~ArgumentParser() _ARGPARSE_NOEXCEPT { }

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

#ifdef _ARGPARSE_CXX_11
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

#ifdef _ARGPARSE_CXX_11
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
#ifdef _ARGPARSE_CXX_11
    template <class T, typename detail::enable_if<
                  detail::is_base_of<HelpFormatter, T>::value>::type* = nullptr>
    inline ArgumentParser&
    formatter_class(
            T const& value)
#else
    template <class T>
    inline ArgumentParser&
    formatter_class(
            T const& value, typename detail::enable_if<
                detail::is_base_of<HelpFormatter, T>::value
                || detail::is_same<HelpFormatter, T>::value, bool
            >::type = true)
#endif  // C++11+
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
            bool value) _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set argument parser 'exit_on_error' value (default: true)
     *
     *  \param value Exit on error flag
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    exit_on_error(
            bool value) _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set output width value (default: auto-detected or 80, min 33)
     *
     *  \param value Output width
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    output_width(
            std::size_t value) _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'prog' value (default: argv[0] or "untitled")
     *
     *  \return Argument parser 'prog' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const& prog() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'usage' value for default language
     *
     *  \return Argument parser 'usage' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const& usage() const;

    /*!
     *  \brief Get title for argument parser 'usage' for default language
     *  (default: "usage")
     *
     *  \since v1.7.1
     *
     *  \return Title for argument parser 'usage'
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    usage_title() const;

    /*!
     *  \brief Get argument parser 'description' value for default language
     *
     *  \return Argument parser 'description' value
     */
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    optionals_title() const;

    /*!
     *  \brief Get argument parser 'epilog' value for default language
     *
     *  \return Argument parser 'epilog' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    epilog() const;

    /*!
     *  \brief Get argument parser 'help' message for default language
     *  (for subparsers)
     *
     *  \return Argument parser 'help' message
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    help() const;

    /*!
     *  \brief Get argument parser 'aliases' value (for subparsers)
     *
     *  \return Argument parser 'aliases' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::vector<std::string> const&
    aliases() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'formatter_class' value
     *
     *  \since v1.7.5
     *
     *  \return Argument parser 'formatter_class' value
     */
    _ARGPARSE_ATTR_NODISCARD
    HelpFormatter&
    formatter_class() _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'prefix_chars' value (default: "-")
     *
     *  \return Argument parser 'prefix_chars' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    prefix_chars() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'fromfile_prefix_chars' value
     *
     *  \return Argument parser 'fromfile_prefix_chars' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    fromfile_prefix_chars() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'comment_prefix_chars' value
     *
     *  \since v1.8.1
     *
     *  \return Argument parser 'comment_prefix_chars' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    comment_prefix_chars() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'argument_default' value
     *
     *  \return Argument parser 'argument_default' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    argument_default() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'conflict_handler' value
     *
     *  \return Argument parser 'conflict_handler' value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string const&
    conflict_handler() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'add_help' value (default: true)
     *
     *  \return Argument parser 'add_help' value
     */
    _ARGPARSE_ATTR_NODISCARD
    bool
    add_help() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'allow_abbrev' value (default: true)
     *
     *  \return Argument parser 'allow_abbrev' value
     */
    _ARGPARSE_ATTR_NODISCARD
    bool
    allow_abbrev() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get argument parser 'exit_on_error' value (default: true)
     *
     *  \return Argument parser 'exit_on_error' value
     */
    _ARGPARSE_ATTR_NODISCARD
    bool
    exit_on_error() const _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Get output width value (default: auto-detected or 80, min 33)
     *
     *  \return Output width value
     */
    _ARGPARSE_ATTR_NODISCARD
    std::size_t
    output_width() const;

#ifdef _ARGPARSE_CXX_11
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
     *  \return Current subparser reference
     */
    Subparser&
    add_subparsers(
            std::string const& title = std::string(),
            std::string const& description = std::string());

    /*!
     *  \brief Get subparsers.
     *  If no subparsers are present, returns nullptr
     *
     *  \return Current subparser pointer or nullptr
     */
    _ARGPARSE_ATTR_NODISCARD
    Subparser*
    subparsers() const _ARGPARSE_NOEXCEPT;

#ifdef _ARGPARSE_CXX_11
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
    handle(std::function<void(std::string const&)> func) _ARGPARSE_NOEXCEPT;

    /*!
     *  \brief Set argument parser 'handle' function.
     *  Called when the parser is executed.
     *
     *  \param func Handle function
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    handle(std::function<void()> func) _ARGPARSE_NOEXCEPT;

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
    handle(std::function<void(Namespace const&)> func) _ARGPARSE_NOEXCEPT;
#endif  // C++11+

    /*!
     *  \brief Get the default value for a specific argument.
     *  If argument not found, returns empty string.
     *
     *  \param dest Argument destination name or flag
     *
     *  \return Default value for a specific argument
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string
    get_default(
            std::string const& dest) const;

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_args(
            Namespace const& space = Namespace()) const
    {
        return parse_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_args(
            T const& args,
            Namespace const& space = Namespace()) const
#else
    template <class T>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace parse_args(
            T const& args, Namespace const& space = Namespace(),
            typename detail::enable_if<
                detail::is_constructible<std::string, T>::value, bool
            >::type = true) const
#endif  // C++11+
    {
        return parse_args(detail::_split_to_args(args), space);
    }

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline Namespace
    parse_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return parse_args(std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            Namespace const& space = Namespace()) const
    {
        return parse_known_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            T const& args,
            Namespace const& space = Namespace()) const
#else
    template <class T>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_args(
            T const& args, Namespace const& space = Namespace(),
            typename detail::enable_if<
                detail::is_constructible<std::string, T>::value, bool
            >::type = true) const
#endif  // C++11+
    {
        return parse_known_args(detail::_split_to_args(args), space);
    }

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline Namespace
    parse_known_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return parse_known_args(std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            Namespace const& space = Namespace()) const
    {
        return parse_intermixed_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            T const& args,
            Namespace const& space = Namespace()) const
#else
    template <class T>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_intermixed_args(
            T const& args, Namespace const& space = Namespace(),
            typename detail::enable_if<
                detail::is_constructible<std::string, T>::value, bool
            >::type = true) const
#endif  // C++11+
    {
        return parse_intermixed_args(detail::_split_to_args(args), space);
    }

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline Namespace
    parse_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return parse_intermixed_args(std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            Namespace const& space = Namespace()) const
    {
        return parse_known_intermixed_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            T const& args,
            Namespace const& space = Namespace()) const
#else
    template <class T>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace parse_known_intermixed_args(
            T const& args, Namespace const& space = Namespace(),
            typename detail::enable_if<
                detail::is_constructible<std::string, T>::value, bool
            >::type = true) const
#endif  // C++11+
    {
        return parse_known_intermixed_args(detail::_split_to_args(args), space);
    }

#ifdef _ARGPARSE_CXX_11
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline Namespace
    parse_known_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return parse_known_intermixed_args(
                    std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace (default: none)
     *
     *  \return Object with parsed arguments
     */
    _ARGPARSE_ATTR_MAYBE_UNUSED
    Namespace
    parse_known_intermixed_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;

#ifdef _ARGPARSE_CXX_17
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
    template <typename = void>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_args(
            Namespace const& space = Namespace()) const
    {
        return try_parse_args(m_parsed_arguments, space);
    }

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
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_args(
            T const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_args(detail::_split_to_args(args), space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline std::optional<Namespace>
    try_parse_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_args(std::vector<std::string>{ args }, space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    template <typename = void>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_args(
            Namespace const& space = Namespace()) const
    {
        return try_parse_known_args(m_parsed_arguments, space);
    }

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
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_args(
            T const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_known_args(detail::_split_to_args(args), space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline std::optional<Namespace>
    try_parse_known_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_known_args(std::vector<std::string>{ args }, space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    template <typename = void>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_intermixed_args(
            Namespace const& space = Namespace()) const
    {
        return try_parse_intermixed_args(m_parsed_arguments, space);
    }

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
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_intermixed_args(
            T const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_intermixed_args(detail::_split_to_args(args), space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline std::optional<Namespace>
    try_parse_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_intermixed_args(
                    std::vector<std::string>{ args }, space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    template <typename = void>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            Namespace const& space = Namespace()) const
    {
        return try_parse_known_intermixed_args(m_parsed_arguments, space);
    }

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
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            T const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_known_intermixed_args(
                    detail::_split_to_args(args), space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    inline std::optional<Namespace>
    try_parse_known_intermixed_args(
            std::initializer_list<std::string> const& args,
            Namespace const& space = Namespace()) const
    {
        return try_parse_known_intermixed_args(
                    std::vector<std::string>{ args }, space);
    }

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
    _ARGPARSE_ATTR_MAYBE_UNUSED
    std::optional<Namespace>
    try_parse_known_intermixed_args(
            std::vector<std::string> const& args,
            Namespace const& space = Namespace()) const;
#endif  // C++17+

    /*!
     *  \brief Check if environment variable with name exists (from envp[])
     *
     *  \param name Environment variable name
     *
     *  \since v1.8.0
     *
     *  \return True if environment variable name with exists, false otherwise
     */
    _ARGPARSE_ATTR_NODISCARD
    bool
    have_env(std::string const& name) const;

    /*!
     *  \brief Get environment variable value (from envp[])
     *
     *  \param name Environment variable name
     *
     *  \since v1.8.0
     *
     *  \return Environment variable value if exists, empty string otherwise
     */
    _ARGPARSE_ATTR_NODISCARD
    std::string
    get_env(std::string const& name) const;

    /*!
     *  \brief Get environment variables list (from envp[])
     *
     *  \since NEXT_RELEASE
     *
     *  \return Environment variable values list
     */
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    _ARGPARSE_ATTR_MAYBE_UNUSED
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
    print_help(std::string const& lang,
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
    _ARGPARSE_ATTR_NODISCARD
    std::string
    format_bash_completion() const;

    /*!
     *  \brief Return a string containing a program usage for selected language
     *
     *  \param lang Language value (default: "")
     *
     *  \return Program usage for selected language
     */
    _ARGPARSE_ATTR_NODISCARD
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
    _ARGPARSE_ATTR_NODISCARD
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
    struct CompletionInfo
    {
        CompletionInfo();

        // -- data ------------------------------------------------------------
        std::string args;
        std::vector<std::pair<pArgument, std::string> > options;
    };

    static CompletionInfo
    bash_completion_info(
            ArgumentParser const* parser);

    static void
    print_parser_completion(
            ArgumentParser const* parser,
            std::string const& prog,
            bool is_root,
            std::ostream& os = std::cout);

    static std::string
    default_language(
            ArgumentParser const* parser);

    struct ParserInfo
    {
        explicit
        ParserInfo(
                ArgumentParser const* parser,
                _Storage const& storage,
                SubparserInfo const& subparser,
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
        SubparserInfo subparser;
        std::string lang;
        bool have_negative_args;
    };
    typedef std::deque<ParserInfo> Parsers;

    static ParserInfo
    parser_info(
            ArgumentParser const* parser,
            _Storage const& storage,
            SubparserInfo const& subparser);

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

#ifdef _ARGPARSE_CXX_17
    std::optional<Namespace>
    on_try_parse_arguments(
            std::vector<std::string> const& args,
            bool only_known,
            bool intermixed,
            Namespace const& space) const;
#endif  // C++17+

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
    check_intermixed_subparser(
            bool intermixed,
            pSubparser const& subparser);

    static void
    check_intermixed_remainder(
            bool intermixed,
            pArguments const& positional);

    std::vector<std::string>
    process_split_equal(
            std::string& arg,
            std::string const& prefix_chars) const;

    static Namespace
    create_namespace(
            bool only_known,
            detail::rval<_Storage>::type storage,
            detail::rval<std::vector<std::string> >::type unrecognized_args);

    static bool
    negative_numbers_presented(
            pArguments const& optionals,
            std::string const& prefix_chars);

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
            std::deque<std::string>& arguments,
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
            std::string const& arg) const;

    void
    process_optional_version(
            ParserInfo const& info,
            std::vector<std::string> const& equals,
            std::string const& arg,
            pArgument const& tmp) const;

    void
    match_positional_minimum(
            Parsers& parsers,
            std::deque<std::string>& arguments,
            pArgument const& arg) const;

    void
    match_positional_more_zero(
            Parsers& parsers,
            std::deque<std::string>& arguments,
            pArgument const& arg,
            std::size_t& over_args) const;

    void
    match_positional_optional(
            Parsers& parsers,
            std::deque<std::string>& arguments,
            pArgument const& arg,
            std::size_t& over_args,
            std::size_t one_args) const;

    void
    match_positional_default(
            Parsers& parsers,
            std::deque<std::string>& arguments,
            pArgument const& arg) const;

    void
    match_positionals(
            Parsers& parsers,
            std::size_t& pos,
            pArguments const& positional,
            std::deque<std::string>& arguments,
            std::size_t finish,
            std::size_t min_args,
            std::size_t one_args,
            bool more_args) const;

    static bool
    finish_analyze_positional(
            pArgument const& arg,
            std::deque<std::string> const& args,
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
            std::deque<std::string>& args,
            bool read_all_args = true) const;

    bool
    try_capture_parser(
            Parsers& parsers,
            std::size_t& pos,
            pArguments& positional,
            std::vector<std::string>& unrecognized_args,
            std::deque<std::string>& args,
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
    find_arg_by_flag(
            pArguments const& args,
            std::string const& key);

    static pArgument const
    get_optional_arg_by_flag(
            bool was_pseudo_arg,
            ParserInfo const& info,
            std::string const& key);

    static bool
    is_not_operand(
            bool was_pseudo_arg,
            pArguments const& args,
            std::string const& key);

    static bool
    process_separate_arg_abbrev(
            std::string const& name,
            std::size_t i,
            std::size_t cp_size,
            std::vector<std::string>& flags,
            std::string const& arg,
            pArguments const& args);

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
            std::deque<std::string>& intermixed_args,
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
    process_required_subparser(
            bool required,
            std::size_t pos,
            SubparserInfo const& info,
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

    SubparserInfo
    subparser_info(
            bool add_suppress,
            std::size_t offset = 0) const;

    std::string
    subparser_prog_args() const;

    void
    add_arg_usage(
            std::string& res,
            std::string const& str,
            bool required) const;

    void
    print_custom_usage(
            pArguments const& positional,
            pArguments const& operand,
            pArguments const& optional,
            std::list<MutuallyExclusiveGroup> const& mutex_groups,
            SubparserInfo const& info,
            std::string const& prog,
            std::string const& usage_title,
            std::ostream& os) const;

    static bool
    is_subparser_positional(
            pSubparser const& sub);

    static void
    print_subparser(
            bool need_print,
            SubparserInfo const& info,
            std::size_t index,
            HelpFormatter const& formatter,
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

    bool
    test_diagnostics(
            std::string const& lang,
            std::ostream& os) const;

    void
    handle(std::string const& str) const;

#ifdef _ARGPARSE_CXX_11
    void
    parse_handle(
            bool only_known,
            _Storage const& storage,
            std::vector<std::string> const& unrecognized_args) const;
#endif  // C++11+

    // -- data ----------------------------------------------------------------
    pArgumentData m_data;
    std::string m_name;
    std::string m_prog;
    detail::TranslationPack m_usage;
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
    detail::Value<std::string> m_argument_default;
    detail::Value<std::size_t> m_output_width;
    std::list<pGroup> m_groups;
    std::list<MutuallyExclusiveGroup> m_mutex_groups;
    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
    std::list<std::pair<std::string, std::string> > m_env_variables;
    pSubparser m_subparsers;
    std::size_t m_subparsers_position;
#ifdef _ARGPARSE_CXX_11
    std::function<void(std::string const&)> m_handle;
    std::function<void(Namespace const&)> m_parse_handle;
#endif  // C++11+
    detail::Value<_SUPPRESS> m_argument_default_type;
    bool m_allow_abbrev;
    bool m_exit_on_error;
};

// -- implementation ----------------------------------------------------------
#ifdef _ARGPARSE_INL
namespace detail {
// -- templates ---------------------------------------------------------------

template <class T>
void
_insert_to_end(
        std::deque<T> const& from,
        std::deque<T>& to)
{
    if (!from.empty()) {
        to.insert(to.end(), from.begin(), from.end());
    }
}

template <class T>
void
_insert_to_end(
        std::deque<T> const& from,
        std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(to.end(), from.begin(), from.end());
    }
}

template <class T>
void
_insert_to_end(
        std::vector<T> const& from,
        std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(to.end(), from.begin(), from.end());
    }
}

template <class T>
void
_move_insert_to_end(
        std::deque<T>& from,
        std::vector<T>& to)
{
#ifdef _ARGPARSE_CXX_11
    if (!from.empty()) {
        to.insert(to.end(),
                  std::make_move_iterator(from.begin()),
                  std::make_move_iterator(from.end()));
    }
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
#ifdef _ARGPARSE_CXX_11
    if (!from.empty()) {
        to.insert(to.end(),
                  std::make_move_iterator(from.begin()),
                  std::make_move_iterator(from.end()));
    }
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
    if (!from.empty()) {
        typedef typename std::vector<T>::difference_type dtype;
#ifdef _ARGPARSE_CXX_11
        to.insert(std::next(to.begin(), static_cast<dtype>(i)),
                  std::make_move_iterator(from.begin()),
                  std::make_move_iterator(from.end()));
#else
        to.insert(to.begin() + static_cast<dtype>(i), from.begin(), from.end());
#endif  // C++11+
    }
}

template <class T>
void
_move_replace_at(
        std::vector<T>& from,
        std::vector<T>& to,
        std::size_t i)
{
    typedef typename std::vector<T>::difference_type dtype;
#ifdef _ARGPARSE_CXX_11
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
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (vec.at(i) == value) {
            return true;
        }
    }
    return false;
}

_ARGPARSE_INL bool
_exists(char value,
        std::string const& str)
{
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str.at(i) == value) {
            return true;
        }
    }
    return false;
}

// -- utf8 support ------------------------------------------------------------
// since v1.7.0
typedef uint32_t codepoint;

_ARGPARSE_INL uint8_t _ARGPARSE_CONSTEXPR
_char_to_u8(
        char c)
{
    return static_cast<uint8_t>(c);
}

_ARGPARSE_INL char _ARGPARSE_CONSTEXPR
_u8_to_char(
        uint8_t c)
{
    return static_cast<char>(c);
}

_ARGPARSE_INL codepoint _ARGPARSE_CONSTEXPR
_char_to_u32(
        char c)
{
    return static_cast<codepoint>(_char_to_u8(c));
}

_ARGPARSE_INL char _ARGPARSE_CONSTEXPR
_u32_to_char(
        codepoint c)
{
    return _u8_to_char(static_cast<uint8_t>(c));
}

_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_1b_mask = 0x80;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_1b_bits = 0x00;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_2b_mask = 0xe0;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_2b_bits = 0xc0;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_3b_mask = 0xf0;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_3b_bits = 0xe0;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_4b_mask = 0xf8;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_4b_bits = 0xf0;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_ct_mask = 0xc0;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_ct_bits = 0x80;
_ARGPARSE_INLINE_VARIABLE uint32_t _ARGPARSE_USE_CONSTEXPR _utf8_ct_invm = 0x3f;

_ARGPARSE_INL std::size_t _ARGPARSE_CONSTEXPR
_utf8_codepoint_size(
        uint8_t byte)
{
    return (byte & _utf8_1b_mask) == _utf8_1b_bits ? 1
         : (byte & _utf8_2b_mask) == _utf8_2b_bits ? 2
         : (byte & _utf8_3b_mask) == _utf8_3b_bits ? 3
         : (byte & _utf8_4b_mask) == _utf8_4b_bits ? 4 : 0;
}

_ARGPARSE_INL std::pair<bool, std::size_t>
_utf8_length(
        std::string const& str,
        std::ostream& err = std::cerr)
{
    std::size_t res = 0;
    std::size_t i = 0;
    while (i < str.size()) {
        std::size_t cp_size = _utf8_codepoint_size(_char_to_u8(str[i]));
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
            if (str[i + n] == '\0') {
                err << "argparse error [skip]: string '" << str << "' "
                    << "is NUL-terminated in the middle of the code point"
                    << std::endl;
                return std::make_pair(false, str.size());
            } else if ((_char_to_u8(str[i + n]) & _utf8_ct_mask)
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
_ARGPARSE_INL bool
_is_utf8_string(
        std::string const& str)
{
    std::stringstream ss;
    return _utf8_length(str, ss).first;
}

// since v1.7.3
_ARGPARSE_INL codepoint
_to_upper_codepoint(
        codepoint cp)
{
    // unicode 15 to upper case
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
         || (0xa7d0 <= cp && cp <= 0xa7d1) || (0xa7d6 <= cp && cp <= 0xa7d9))) {
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
    // WARANG CITI
    if (0x118c0 <= cp && cp <= 0x118df) return cp - 0x20;
    // MEDEFAIDRIN
    if (0x16e60 <= cp && cp <= 0x16e7f) return cp - 0x20;
    // ADLAM
    if (0x1e922 <= cp && cp <= 0x1e943) return cp - 0x22;
    return cp;
}

_ARGPARSE_INL std::string
_to_upper(std::string const& str)
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
        std::size_t cp_size = _utf8_codepoint_size(_char_to_u8(str[i]));
        codepoint cp = 0;
        switch (cp_size) {
            case 1:
                cp =  (_char_to_u32(str[i    ]) & ~_utf8_1b_mask);
                break;
            case 2:
                cp = ((_char_to_u32(str[i    ]) & ~_utf8_2b_mask) <<  6)
                   |  (_char_to_u32(str[i + 1]) & ~_utf8_ct_mask);
                break;
            case 3:
                cp = ((_char_to_u32(str[i    ]) & ~_utf8_3b_mask) << 12)
                   | ((_char_to_u32(str[i + 1]) & ~_utf8_ct_mask) <<  6)
                   |  (_char_to_u32(str[i + 2]) & ~_utf8_ct_mask);
                break;
            case 4:
                cp = ((_char_to_u32(str[i    ]) & ~_utf8_4b_mask) << 18)
                   | ((_char_to_u32(str[i + 1]) & ~_utf8_ct_mask) << 12)
                   | ((_char_to_u32(str[i + 2]) & ~_utf8_ct_mask) <<  6)
                   |  (_char_to_u32(str[i + 3]) & ~_utf8_ct_mask);
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
_ARGPARSE_INL std::string
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
// ----------------------------------------------------------------------------

_ARGPARSE_INL std::pair<std::size_t, std::size_t>
_get_terminal_size(
        bool default_values = false)
{
    std::size_t width  = 80;
    std::size_t height = 24;
    if (default_values) {
        return std::make_pair(width, height);
    }
#ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#if defined(_WIN32)
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
#else  // UNIX
#if defined(TIOCGSIZE)
    struct ttysize w;
    if (ioctl(STDOUT_FILENO, TIOCGSIZE, &w) >= 0) {
        width = static_cast<std::size_t>(w.ts_cols);
        height = static_cast<std::size_t>(w.ts_lines);
        if (width < _min_width && width != 0) {
            width = _min_width;
        }
    }
#elif defined(TIOCGWINSZ)
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) >= 0) {
        width = static_cast<std::size_t>(w.ws_col);
        height = static_cast<std::size_t>(w.ws_row);
        if (width < _min_width && width != 0) {
            width = _min_width;
        }
    }
#endif  // TIOCGSIZE
#endif  // _WIN32
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
    return std::make_pair(width, height);
}

_ARGPARSE_INL void
_limit_to_min(
        std::size_t& value,
        std::size_t to) _ARGPARSE_NOEXCEPT
{
    if (value < to) {
        value = to;
    }
}

_ARGPARSE_INL void
_limit_to_max(
        std::size_t& value,
        std::size_t to) _ARGPARSE_NOEXCEPT
{
    if (value > to) {
        value = to;
    }
}

_ARGPARSE_INL void
_ltrim(std::string& str)
{
    std::string::iterator it = str.begin();
    for ( ; it != str.end()
          && std::isspace(static_cast<unsigned char>(*it)); ++it) {
    }
    str.erase(str.begin(), it);
}

_ARGPARSE_INL void
_rtrim(std::string& str)
{
    std::string::reverse_iterator it = str.rbegin();
    for ( ; it != str.rend()
          && std::isspace(static_cast<unsigned char>(*it)); ++it) {
    }
    str.erase(it.base(), str.end());
}

_ARGPARSE_INL void
_trim(std::string& str)
{
    _ltrim(str);
    _rtrim(str);
}

_ARGPARSE_INL std::string
_trim_copy(
        std::string const& str)
{
    std::string res = str;
    _trim(res);
    return res;
}

#ifdef _ARGPARSE_CXX_17
_ARGPARSE_INL std::string_view
_trim_sw(std::string const& str)
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
_ARGPARSE_INL std::string
_trim_sw(std::string const& str)
{
    return _trim_copy(str);
}
#endif  // C++17+

_ARGPARSE_INL std::string
_file_name(
        std::string const& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

_ARGPARSE_INL void
_resolve_conflict(
        std::string const& str,
        std::vector<std::string>& values)
{
    std::vector<std::string>::iterator it
            = std::find(values.begin(), values.end(), str);
    if (it != values.end()) {
        values.erase(it);
    }
}

_ARGPARSE_INL void
_resolve_conflict(
        std::vector<std::string> const& vec,
        std::vector<std::string>& values)
{
    for (std::size_t i = 0; i < vec.size(); ++i) {
        _resolve_conflict(vec[i], values);
    }
}

_ARGPARSE_INL std::string
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

_ARGPARSE_INL bool
_have_quotes(
        std::string const& str)
{
    return str.size() > 1 && str.at(0) == str.at(str.size() - 1)
            && (str.at(0) == '\'' || str.at(0) == '\"');
}

_ARGPARSE_INL std::string
_replace(std::string str,
        char old,
        std::string const& value)
{
    std::string::size_type pos = str.find(old);
    while (pos != std::string::npos) {
        str.replace(pos, 1, value);
        pos = str.find(old, pos + value.size());
    }
    return str;
}

_ARGPARSE_INL std::string
_replace(std::string str,
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

_ARGPARSE_INL bool
_contains_substr(
        std::string const& str,
        std::string const& substr)
{
#ifdef _ARGPARSE_CXX_23
    return str.contains(substr);
#else
    return str.find(substr) != std::string::npos;
#endif  // C++23+
}

_ARGPARSE_INL bool
_starts_with(
        std::string const& str,
        std::string const& value)
{
#ifdef _ARGPARSE_CXX_20
    return str.starts_with(value);
#else
    return str.compare(0, value.size(), value) == 0;
#endif  // C++20+
}

_ARGPARSE_INL bool
_ends_with(
        std::string const& str,
        std::string const& value)
{
#ifdef _ARGPARSE_CXX_20
    return str.ends_with(value);
#else
    return str.size() >= value.size()
            && 0 == str.compare(str.size() - value.size(), value.size(), value);
#endif  // C++20+
}

_ARGPARSE_INL std::string
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

#ifdef _ARGPARSE_CXX_17
_ARGPARSE_INL std::string_view
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
#endif  // C++17+

_ARGPARSE_INL bool
_is_flag_correct(
        std::string const& str,
        bool is_optional)
{
#ifdef _ARGPARSE_CXX_17
    std::string_view trimmed = _trim_sw(str);
#else
    std::string trimmed = _trim_copy(str);
#endif  // C++17+
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

_ARGPARSE_INL bool
_is_negative_number(
        std::string const& str)
{
    double value;
    std::stringstream ss(str);
    ss >> value;
    return !ss.fail() && ss.eof() && value < 0;
}

_ARGPARSE_INL bool
_is_optional(
        std::string const& arg,
        std::string const& prefix_chars,
        bool have_negative_args,
        bool was_pseudo_arg)
{
    return !arg.empty() && _exists(arg.at(0), prefix_chars) && !was_pseudo_arg
            && (have_negative_args || !_is_negative_number(arg));
}

_ARGPARSE_INL bool
_not_optional(
        std::string const& arg,
        std::string const& prefix_chars,
        bool have_negative_args,
        bool was_pseudo_arg)
{
    return arg.empty() || !_exists(arg.at(0), prefix_chars) || was_pseudo_arg
            || (!have_negative_args && _is_negative_number(arg));
}

_ARGPARSE_INL std::string
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

_ARGPARSE_INL void
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

_ARGPARSE_INL void
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

_ARGPARSE_INL std::vector<std::string>
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

_ARGPARSE_INL std::vector<std::string>
_split_lines(
        std::string const& str,
        bool keepends)
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

_ARGPARSE_INL std::vector<std::string>
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

_ARGPARSE_INL void
_process_quotes(
        std::deque<char>& quotes,
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

_ARGPARSE_INL std::vector<std::string>
_split_to_args(
        std::string const& str,
        std::ostream& err)
{
    std::vector<std::string> res;
    std::string value;
    std::deque<char> quotes;
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
            if (str[i + 1] != _space) {
                value += c;
            }
            continue;
        }
        if (((c == _space && !skip)
             || (c != _space && std::isspace(static_cast<unsigned char>(c))))
                && quotes.empty()) {
            _store_value_to(value, res);
        } else {
            _process_quotes(quotes, value, str, c, i + 1);
            value += c;
        }
        skip = false;
    }
    _store_value_to(value, res);
    if (!quotes.empty()) {
        err << "argparse error [skip]: possible incorrect string: '"
            << str << "'" << std::endl;
    }
    return res;
}

_ARGPARSE_INL bool
_string_to_bool(
        std::string const& str) _ARGPARSE_NOEXCEPT
{
    return !str.empty();
}

_ARGPARSE_INL std::string
_bool_to_string(
        bool value)
{
    return value ? "true" : "false";
}

_ARGPARSE_INL std::string
_bool_to_string(
        std::string const& str)
{
    return _bool_to_string(_string_to_bool(str));
}

_ARGPARSE_INL std::string
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
            val = _replace(val, _space, "\\ ");
        }
        _append_value_to(quotes + val + quotes, res, separator);
    }
    return begin + (res.empty() ? none : res) + end;
}

_ARGPARSE_INL std::string
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

_ARGPARSE_INL std::string
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
    return begin + (res.empty() ? (begin + res + end) : res) + end;
}

_ARGPARSE_INL std::string
_ignore_explicit(
            std::string const& arg,
            std::string const& value)
{
    return "argument " + arg + ": ignored explicit argument '" + value + "'";
}

_ARGPARSE_INL void
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
    }
    size = _utf8_length(value).second;
    if (size < indent) {
        value.resize(value.size() + indent - size, _space);
        value += str;
    } else {
        value += _spaces + str;
    }
}

_ARGPARSE_INL std::string
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

_ARGPARSE_INL std::string
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

_ARGPARSE_INL void
_print_raw_text_formatter(
        HelpFormatter const& formatter,
        std::string const& text,
        std::size_t width,
        std::ostream& os,
        std::string const& begin = std::string("\n"),
        std::size_t indent = 0,
        std::string const& end = std::string())
{
    if (!text.empty()) {
        os << begin
           << formatter._fill_text(text, width, indent) << end << std::endl;
    }
}

_ARGPARSE_INL std::string
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

_ARGPARSE_INL bool
_is_type_correct(
        std::string const& expected,
        std::string const& received) _ARGPARSE_NOEXCEPT
{
    return expected.empty() || received == expected;
}

_ARGPARSE_INL void
_check_type(
        Value<std::string> const& expected,
        std::string const& received)
{
    if (expected.has_value() && !_is_type_correct(expected(), received)) {
        throw TypeError("type_name mismatch: expected '" + expected.value()
                        + "', received '" + received + "'");
    }
}

_ARGPARSE_INL void
_check_non_count_action(
        std::string const& key,
        Action action)
{
    if (action == argparse::count) {
        throw TypeError("got an invalid type for count argument '" + key + "'");
    }
}

_ARGPARSE_INL void
_check_flag_name(
            std::string const& flag)
{
    if (flag.empty()) {
        throw IndexError("string index out of range");
    }
}

_ARGPARSE_INL void
_update_flag_name_func(
        std::string const& arg,
        std::string& flag,
        std::size_t& count)
{
    std::string name = _flag_name(arg);
    std::size_t count_prefixes = arg.size() - name.size();
    if (count < count_prefixes) {
        count = count_prefixes;
        flag = _ARGPARSE_MOVE(name);
    }
}

_ARGPARSE_INL void
_update_flag_name(
        std::vector<std::string>& flags,
        std::string const& prefix_chars,
        bool is_optional,
        std::string& flag_name,
        std::size_t& prefixes)
{
    if (is_optional) {
        _update_flag_name_func(flag_name, flag_name, prefixes);
    } else if (flags.size() > 1) {
        // no positional multiflag
        throw ValueError("invalid option string " + flags.front() + ": must "
                         + "starts with a character '" + prefix_chars + "'");
    }
    for (std::size_t i = 1; i < flags.size(); ++i) {
        // check arguments
        std::string& flag = flags.at(i);
        _check_flag_name(flag);
        if (!_exists(flag.at(0), prefix_chars)) {
            // no positional and optional args
            throw ValueError("invalid option string " + flag + ": must starts "
                             + "with a character '" + prefix_chars + "'");
        }
        _update_flag_name_func(flag, flag_name, prefixes);
    }
}
}  // namespace detail

// -- HelpFormatter -----------------------------------------------------------
_ARGPARSE_INL std::string
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

_ARGPARSE_INL std::string
HelpFormatter::_get_default_metavar_for_optional(
        Argument const* action) const
{
    return detail::_to_upper(action->get_dest());
}

_ARGPARSE_INL std::string
HelpFormatter::_get_default_metavar_for_positional(
        Argument const* action) const
{
    return action->get_dest();
}

_ARGPARSE_INL std::string
HelpFormatter::_get_help_string(
        Argument const* action,
        std::string const& lang) const
{
    return detail::_tr(action->m_help, lang);
}

_ARGPARSE_INL std::vector<std::string>
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

// -- _RawDescriptionHelpFormatter --------------------------------------------
_ARGPARSE_INL std::string
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

_ARGPARSE_INL std::vector<std::string>
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
                    value += " ";
                }
                std::vector<std::string> tab_split_str
                        = detail::_split(sub_split_str.at(j), "\t");
                for (std::size_t k = 0; k < tab_split_str.size(); ++k) {
                    if (k != 0) {
                        std::string sub = std::string(
                                    _tab_size() - (detail::_utf8_length(
                                                   value).second % _tab_size()),
                                    detail::_space);
                        if (detail::_utf8_length(value).second + 1
                                + detail::_utf8_length(sub).second > width) {
                            detail::_store_value_to(value, res);
                            sub = std::string(_tab_size(), detail::_space);
                        }
                        value += sub;
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
_ARGPARSE_INL std::vector<std::string>
_RawTextHelpFormatter::_split_lines(
        std::string const& text,
        std::size_t width) const
{
    return _RawDescriptionHelpFormatter::_split_lines_raw(text, width);
}

// -- _ArgumentDefaultsHelpFormatter ------------------------------------------
_ARGPARSE_INL std::string
_ArgumentDefaultsHelpFormatter::_get_help_string(
        Argument const* action,
        std::string const& lang) const
{
    std::string res = detail::_tr(action->m_help, lang);
    if (!res.empty() && !detail::_contains_substr(res, "%(default)s")) {
        if (((action->m_type == Argument::Optional
              || action->m_type == Argument::Operand)
             || (action->m_nargs & (Argument::ZERO_OR_ONE
                                    | Argument::ZERO_OR_MORE)))
                && !(action->action() & (argparse::help | argparse::version))) {
            res += " (default: %(default)s)";
        }
    }
    return res;
}

// -- _MetavarTypeHelpFormatter -----------------------------------------------
_ARGPARSE_INL std::string
_MetavarTypeHelpFormatter::_get_default_metavar_for_optional(
        Argument const* action) const
{
    if (!action->type_name().empty()) {
        return action->type_name();
    }
    return HelpFormatter::_get_default_metavar_for_optional(action);
}

_ARGPARSE_INL std::string
_MetavarTypeHelpFormatter::_get_default_metavar_for_positional(
        Argument const* action) const
{
    if (!action->type_name().empty()) {
        return action->type_name();
    }
    return HelpFormatter::_get_default_metavar_for_positional(action);
}

// -- Argument ----------------------------------------------------------------
_ARGPARSE_INL
Argument::Argument(
        std::vector<std::string> const& flags,
        std::string const& name,
        Type type)
    : m_flags(flags),
      m_all_flags(m_flags),
      m_name(name),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(type),
      m_nargs(NARGS_DEF),
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
#ifdef _ARGPARSE_CXX_11
      m_dest(std::vector<std::string>{ std::string() }),
      m_handle(nullptr),
#else
      m_dest(detail::_vector(std::string())),
#endif  // C++11+
      m_post_trigger(_ARGPARSE_NULLPTR),
      m_required()
{
    m_help[std::string()] = std::string();
    m_required.reset(m_type != Optional);
}

_ARGPARSE_INL detail::shared_ptr<Argument>
Argument::make_argument(
        std::vector<std::string> const& flags,
        std::string const& name,
        Type type)
{
    return detail::make_shared<Argument>(Argument(flags, name, type));
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL
Argument::Argument(
        std::vector<std::string>&& flags,
        std::string&& name,
        Type type)
    : m_flags(std::move(flags)),
      m_all_flags(m_flags),
      m_name(std::move(name)),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(type),
      m_nargs(NARGS_DEF),
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
      m_post_trigger(nullptr),
      m_required()
{
    m_help[std::string()] = std::string();
    m_required.reset(m_type != Optional);
}

_ARGPARSE_INL detail::shared_ptr<Argument>
Argument::make_argument(
        std::vector<std::string>&& flags,
        std::string&& name,
        Type type)
{
    return detail::make_shared<Argument>(
                Argument(std::move(flags), std::move(name), type));
}
#endif  // C++11+

#ifndef _ARGPARSE_CXX_11
_ARGPARSE_INL
Argument::Argument(
        std::string const& flag)
    : m_flags(detail::_vector(flag)),
      m_all_flags(m_flags),
      m_name(),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(NoType),
      m_nargs(NARGS_DEF),
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
      m_post_trigger(_ARGPARSE_NULLPTR),
      m_required()
{
    m_help[std::string()] = std::string();
}

_ARGPARSE_INL
Argument::Argument(
        std::string const& flag1,
        std::string const& flag2)
    : m_flags(detail::_vector(flag1, flag2)),
      m_all_flags(m_flags),
      m_name(),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(NoType),
      m_nargs(NARGS_DEF),
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
      m_post_trigger(_ARGPARSE_NULLPTR),
      m_required()
{
    m_help[std::string()] = std::string();
}

_ARGPARSE_INL
Argument::Argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3)
    : m_flags(detail::_vector(flag1, flag2, flag3)),
      m_all_flags(m_flags),
      m_name(),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(NoType),
      m_nargs(NARGS_DEF),
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
      m_post_trigger(_ARGPARSE_NULLPTR),
      m_required()
{
    m_help[std::string()] = std::string();
}

_ARGPARSE_INL
Argument::Argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3,
        std::string const& flag4)
    : m_flags(detail::_vector(flag1, flag2, flag3, flag4)),
      m_all_flags(m_flags),
      m_name(),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(NoType),
      m_nargs(NARGS_DEF),
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
      m_post_trigger(_ARGPARSE_NULLPTR),
      m_required()
{
    m_help[std::string()] = std::string();
}
#endif  // C++11-

_ARGPARSE_INL
Argument::Argument(
        std::vector<std::string> const& flags)
    : m_flags(flags),
      m_all_flags(m_flags),
      m_name(),
      m_action(argparse::store),
      m_default_type(),
      m_help_type(),
      m_type(NoType),
      m_nargs(NARGS_DEF),
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
#ifdef _ARGPARSE_CXX_11
      m_dest(std::vector<std::string>{ std::string() }),
      m_handle(nullptr),
#else
      m_dest(detail::_vector(std::string())),
#endif  // C++11+
      m_post_trigger(_ARGPARSE_NULLPTR),
      m_required()
{
    m_help[std::string()] = std::string();
}

_ARGPARSE_INL
Argument::Argument(
        Argument const& orig)
    : m_flags(orig.m_flags),
      m_all_flags(orig.m_all_flags),
      m_name(orig.m_name),
      m_action(orig.m_action),
      m_default_type(orig.m_default_type),
      m_help_type(orig.m_help_type),
      m_type(orig.m_type),
      m_nargs(orig.m_nargs),
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
#ifdef _ARGPARSE_CXX_11
      m_handle(orig.m_handle),
#endif  // C++11+
      m_post_trigger(orig.m_post_trigger),
      m_required(orig.m_required)
{
}

_ARGPARSE_INL Argument&
Argument::operator =(
        Argument const& rhs)
{
    if (this != &rhs) {
        this->m_flags           = rhs.m_flags;
        this->m_all_flags       = rhs.m_all_flags;
        this->m_name            = rhs.m_name;
        this->m_action          = rhs.m_action;
        this->m_default_type    = rhs.m_default_type;
        this->m_help_type       = rhs.m_help_type;
        this->m_type            = rhs.m_type;
        this->m_nargs           = rhs.m_nargs;
        this->m_num_args        = rhs.m_num_args;
        this->m_nargs_str       = rhs.m_nargs_str;
        this->m_const           = rhs.m_const;
        this->m_default         = rhs.m_default;
        this->m_implicit        = rhs.m_implicit;
        this->m_type_name       = rhs.m_type_name;
        this->m_choices         = rhs.m_choices;
        this->m_help            = rhs.m_help;
        this->m_version         = rhs.m_version;
        this->m_metavar         = rhs.m_metavar;
        this->m_dest            = rhs.m_dest;
#ifdef _ARGPARSE_CXX_11
        this->m_handle          = rhs.m_handle;
#endif  // C++11+
        this->m_post_trigger    = rhs.m_post_trigger;
        this->m_required        = rhs.m_required;
    }
    return *this;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL
Argument::Argument(
        Argument&& orig) _ARGPARSE_NOEXCEPT
    : m_flags(std::move(orig.m_flags)),
      m_all_flags(std::move(orig.m_all_flags)),
      m_name(std::move(orig.m_name)),
      m_action(std::move(orig.m_action)),
      m_default_type(std::move(orig.m_default_type)),
      m_help_type(std::move(orig.m_help_type)),
      m_type(std::move(orig.m_type)),
      m_nargs(std::move(orig.m_nargs)),
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
      m_post_trigger(std::move(orig.m_post_trigger)),
      m_required(std::move(orig.m_required))
{
}

_ARGPARSE_INL Argument&
Argument::operator =(
        Argument&& rhs) _ARGPARSE_NOEXCEPT
{
    if (this != &rhs) {
        this->m_flags           = std::move(rhs.m_flags);
        this->m_all_flags       = std::move(rhs.m_all_flags);
        this->m_name            = std::move(rhs.m_name);
        this->m_action          = std::move(rhs.m_action);
        this->m_default_type    = std::move(rhs.m_default_type);
        this->m_help_type       = std::move(rhs.m_help_type);
        this->m_type            = std::move(rhs.m_type);
        this->m_nargs           = std::move(rhs.m_nargs);
        this->m_num_args        = std::move(rhs.m_num_args);
        this->m_nargs_str       = std::move(rhs.m_nargs_str);
        this->m_const           = std::move(rhs.m_const);
        this->m_default         = std::move(rhs.m_default);
        this->m_implicit        = std::move(rhs.m_implicit);
        this->m_type_name       = std::move(rhs.m_type_name);
        this->m_choices         = std::move(rhs.m_choices);
        this->m_help            = std::move(rhs.m_help);
        this->m_version         = std::move(rhs.m_version);
        this->m_metavar         = std::move(rhs.m_metavar);
        this->m_dest            = std::move(rhs.m_dest);
        this->m_handle          = std::move(rhs.m_handle);
        this->m_post_trigger    = std::move(rhs.m_post_trigger);
        this->m_required        = std::move(rhs.m_required);
    }
    return *this;
}
#endif  // C++11+

_ARGPARSE_INL Argument&
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

_ARGPARSE_INL Argument&
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
            check_action();
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
            check_action();
            m_const.reset();
            m_nargs = NARGS_DEF;
            m_nargs_str = std::string("1");
            m_num_args = 1;
            break;
        default :
            throw ValueError("unknown action");
    }
#ifdef _ARGPARSE_CXX_11
    if (action() & (argparse::version | argparse::help)) {
        m_handle = nullptr;
    }
#endif  // C++11+
    if (!(value & detail::_store_const_action)) {
        m_metavar.reset();
    }
    m_action = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::nargs(
        std::size_t value)
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
            if (value == 0) {
                throw
                ValueError("nargs for store actions must be != 0; if you "
                           "have nothing to store, actions such as store "
                           "true or store const may be more appropriate");
            }
            break;
        case argparse::append :
        case argparse::extend :
            if (value == 0) {
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
    m_nargs_str = detail::_to_string(value);
    m_num_args = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::nargs(
        std::string const& value)
{
    if (!(action() & detail::_store_action) || m_type == Operand) {
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
    m_num_args = 1;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::nargs(
        _REMAINDER value)
{
    if (!(action() & detail::_store_action) || m_type == Operand
            || value != argparse::REMAINDER) {
        throw TypeError("got an unexpected keyword argument 'nargs'");
    }
    m_nargs = REMAINDING;
    m_nargs_str = std::string("0");
    m_num_args = 0;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::nargs(
        _SUPPRESS value)
{
    if (!(action() & detail::_store_action) || m_type == Operand
            || value != argparse::SUPPRESS) {
        throw TypeError("got an unexpected keyword argument 'nargs'");
    }
    m_nargs = SUPPRESSING;
    m_nargs_str = std::string("0");
    m_num_args = 0;
    return *this;
}

_ARGPARSE_INL Argument&
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

_ARGPARSE_INL Argument&
Argument::default_value(
        std::string const& value)
{
    m_default = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::default_value(
        _SUPPRESS value)
{
    if (value != argparse::SUPPRESS) {
        throw TypeError("got an unexpected keyword argument 'default'");
    }
    if (action() == argparse::store_false) {
        m_default.reset("1");
    } else {
        m_default.reset();
    }
    m_default_type = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::implicit_value(
        std::string const& value)
{
    m_implicit = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::type(
        std::string const& value)
{
    m_type_name.reset(value);
    return *this;
}

_ARGPARSE_INL Argument&
Argument::choice(
        std::string const& value)
{
    if (!(action() & (detail::_store_action | argparse::language))) {
        throw TypeError("got an unexpected keyword argument 'choices'");
    }
    std::vector<std::string> values;
    values.push_back(value);
    m_choices = _ARGPARSE_MOVE(values);
    return *this;
}

_ARGPARSE_INL Argument&
Argument::choices(std::string const& value)
{
    if (!(action() & (detail::_store_action | argparse::language))) {
        throw TypeError("got an unexpected keyword argument 'choices'");
    }
    std::vector<std::string> values;
    values.reserve(value.size());
    for (std::size_t i = 0; i < value.size(); ++i) {
        values.push_back(std::string(1, value.at(i)));
    }
    m_choices = _ARGPARSE_MOVE(values);
    return *this;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL Argument&
Argument::choices(
        std::initializer_list<std::string> const& value)
{
    return choices(std::vector<std::string>{ value });
}
#else
_ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value1,
        std::string const& value2)
{
    return choices(detail::_vector(value1, value2));
}

_ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3)
{
    return choices(detail::_vector(value1, value2, value3));
}

_ARGPARSE_INL Argument&
Argument::choices(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3,
        std::string const& value4)
{
    return choices(detail::_vector(value1, value2, value3, value4));
}
#endif  // C++11+

_ARGPARSE_INL Argument&
Argument::choices(
        std::vector<std::string> const& value)
{
    if (!(action() & (detail::_store_action | argparse::language))) {
        throw TypeError("got an unexpected keyword argument 'choices'");
    }
    m_choices = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::required(
        bool value)
{
    if (m_type == Positional) {
        throw TypeError("'required' is an invalid argument for positionals");
    }
    m_required = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::help(
        std::string const& value,
        std::string const& lang)
{
    if (lang.empty()) {
        m_help_type.reset();
    }
    m_help[lang] = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::help(
        _SUPPRESS value)
{
    if (value != argparse::SUPPRESS) {
        throw TypeError("got an unexpected keyword argument 'help'");
    }
    m_help_type = value;
    return *this;
}

_ARGPARSE_INL Argument&
Argument::version(
        std::string const& value)
{
    if (action() != argparse::version) {
        throw TypeError("got an unexpected keyword argument 'version'");
    }
    m_version = value;
    return *this;
}

_ARGPARSE_INL Argument&
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

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL Argument&
Argument::metavar(
        std::initializer_list<std::string> const& value)
{
    return metavar(std::vector<std::string>{ value });
}
#else
_ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value)
{
    return metavar(detail::_vector(value));
}

_ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value1,
        std::string const& value2)
{
    return metavar(detail::_vector(value1, value2));
}

_ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3)
{
    return metavar(detail::_vector(value1, value2, value3));
}

_ARGPARSE_INL Argument&
Argument::metavar(
        std::string const& value1,
        std::string const& value2,
        std::string const& value3,
        std::string const& value4)
{
    return metavar(detail::_vector(value1, value2, value3, value4));
}
#endif  // C++11+

_ARGPARSE_INL Argument&
Argument::dest(
        std::string const& value)
{
    if (m_type == Positional && !m_flags.empty()) {
        throw ValueError("dest supplied twice for positional argument");
    }
    m_dest.front() = value;
    return *this;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL Argument&
Argument::handle(
        std::function<void(std::string const&)> func)
{
    if (action() & (argparse::version | argparse::help)) {
        throw TypeError("got an unexpected keyword argument 'handle'");
    }
    m_handle = std::move(func);
    return *this;
}

_ARGPARSE_INL Argument&
Argument::handle(
        std::function<void()> func)
{
    if (action() & (argparse::version | argparse::help)) {
        throw TypeError("got an unexpected keyword argument 'handle'");
    }
    m_handle = std::bind([](std::function<void()>& f){ f(); }, std::move(func));
    return *this;
}
#endif  // C++11+

_ARGPARSE_INL std::vector<std::string> const&
Argument::flags() const _ARGPARSE_NOEXCEPT
{
    return m_all_flags;
}

_ARGPARSE_INL Action
Argument::action() const _ARGPARSE_NOEXCEPT
{
    return m_action;
}

_ARGPARSE_INL std::string const&
Argument::nargs() const _ARGPARSE_NOEXCEPT
{
    return m_nargs_str;
}

_ARGPARSE_INL std::string const&
Argument::const_value() const _ARGPARSE_NOEXCEPT
{
    return m_const();
}

_ARGPARSE_INL std::string const&
Argument::default_value() const _ARGPARSE_NOEXCEPT
{
    return m_default();
}

_ARGPARSE_INL std::string const&
Argument::implicit_value() const _ARGPARSE_NOEXCEPT
{
    return m_implicit();
}

_ARGPARSE_INL std::string const&
Argument::type_name() const _ARGPARSE_NOEXCEPT
{
    return m_type_name();
}

_ARGPARSE_INL std::vector<std::string> const&
Argument::choices() const _ARGPARSE_NOEXCEPT
{
    return m_choices();
}

_ARGPARSE_INL bool
Argument::required() const _ARGPARSE_NOEXCEPT
{
    return m_required();
}

_ARGPARSE_INL std::string const&
Argument::help() const
{
    return detail::_map_at(m_help, std::string());
}

_ARGPARSE_INL std::string const&
Argument::version() const _ARGPARSE_NOEXCEPT
{
    return m_version();
}

_ARGPARSE_INL std::string
Argument::metavar() const
{
    std::string res = detail::_join(m_metavar(), ", ");
    return m_metavar().size() > 1 ? ("[" + res + "]") : res;
}

_ARGPARSE_INL std::string const&
Argument::dest() const _ARGPARSE_NOEXCEPT
{
    return m_dest.front();
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL void
Argument::handle(
        std::string const& str) const
{
    if (m_handle) {
        m_handle(detail::_remove_quotes<std::string>(str));
    }
}
#else
_ARGPARSE_INL void
Argument::handle(std::string const&) const { /* stub */ }
#endif  // C++11+

_ARGPARSE_INL void
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

_ARGPARSE_INL void
Argument::check_action() const
{
    if (m_type == Positional) {
        // version, help and language actions cannot be positional
        throw TypeError("got an unexpected keyword argument 'action'");
    }
}

_ARGPARSE_INL void
Argument::prepare_action(
        Action value)
{
    if (m_type == Optional && value == argparse::BooleanOptionalAction) {
        make_no_flags();
        if (m_post_trigger) {
            m_post_trigger->check_conflict_arg(this);
        }
    } else {
        m_all_flags = m_flags;
    }
}

_ARGPARSE_INL void
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

_ARGPARSE_INL std::string
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

_ARGPARSE_INL std::string
Argument::flags_to_string(
        HelpFormatter const& formatter) const
{
    std::string res;
    if (m_type == Optional || m_type == Operand) {
        for (std::size_t i = 0; i < flags().size(); ++i) {
            detail::_append_value_to(flags().at(i), res, ", ");
            process_nargs_suffix(res, formatter);
        }
    } else {
        std::vector<std::string> names = get_argument_name(formatter);
        res += names.size() > 1
              ? ("[" + detail::_join(names, ", ") + "]") : detail::_join(names);
    }
    return res;
}

_ARGPARSE_INL std::string
Argument::get_choices() const
{
    return m_choices.has_value() ? detail::_join(choices(), ", ") : "None";
}

_ARGPARSE_INL std::string
Argument::get_const() const
{
    return m_const.value_or("None");
}

_ARGPARSE_INL std::string
Argument::get_default() const
{
    if (!m_default.has_value() && (action() & detail::_bool_action)) {
        return detail::_bool_to_string(m_default());
    } else {
        return (m_default.has_value() || !m_default().empty())
                ? m_default.value() : "None";
    }
}

_ARGPARSE_INL std::string const&
Argument::get_dest() const _ARGPARSE_NOEXCEPT
{
    return dest().empty() ? m_name : dest();
}

_ARGPARSE_INL std::string
Argument::get_metavar() const
{
    return m_metavar.has_value() ? metavar() : "None";
}

_ARGPARSE_INL std::string
Argument::get_nargs() const
{
    switch (m_nargs) {
        case NARGS_DEF :
            return "None";
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

_ARGPARSE_INL std::string
Argument::option_strings() const
{
    return detail::_vector_to_string(flags(), ", ", "'", false, "", "[", "]");
}

_ARGPARSE_INL std::string
Argument::get_required() const
{
    return detail::_bool_to_string(required());
}

_ARGPARSE_INL std::string
Argument::get_type() const
{
    return m_type_name.value_or("None");
}

_ARGPARSE_INL std::string
Argument::print(
        HelpFormatter const& formatter,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    std::string res = formatter._get_help_string(this, lang);
#ifdef _ARGPARSE_CXX_11
    std::regex const r("%[(]([a-z_]*)[)]s");
    std::smatch match;
    std::string text;
    std::unordered_map<std::string, std::function<std::string()> > const
            specifiers =
    {
        { "%(choices)s",        [this] () { return get_choices();       } },
        { "%(const)s",          [this] () { return get_const();         } },
        { "%(default)s",        [this] () { return get_default();       } },
        { "%(dest)s",           [this] () { return get_dest();          } },
        { "%(help)s",           [this] () { return help();              } },
        { "%(metavar)s",        [this] () { return get_metavar();       } },
        { "%(nargs)s",          [this] () { return get_nargs();         } },
        { "%(option_strings)s", [this] () { return option_strings();    } },
        { "%(required)s",       [this] () { return get_required();      } },
        { "%(type)s",           [this] () { return get_type();          } },
    };
    while (std::regex_search(res, match, r)) {
        text += match.prefix();
        auto specifier = std::string(match[0]);
        auto it = specifiers.find(specifier);
        text += (it != specifiers.end() ? it->second() : specifier);
        res = match.suffix();
    }
    text += res;
    std::swap(res, text);
#else
    res = detail::_replace(res, "%(choices)s",          get_choices()   );
    res = detail::_replace(res, "%(const)s",            get_const()     );
    res = detail::_replace(res, "%(default)s",          get_default()   );
    res = detail::_replace(res, "%(dest)s",             get_dest()      );
    res = detail::_replace(res, "%(metavar)s",          get_metavar()   );
    res = detail::_replace(res, "%(nargs)s",            get_nargs()     );
    res = detail::_replace(res, "%(option_strings)s",   option_strings());
    res = detail::_replace(res, "%(required)s",         get_required()  );
    res = detail::_replace(res, "%(type)s",             get_type()      );
    res = detail::_replace(res, "%(help)s",             help()          );
#endif  // C++11+
    return detail::_help_formatter("  " + flags_to_string(formatter),
                                   formatter, res, width, limit);
}

_ARGPARSE_INL void
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
    std::size_t const names_size = names.size();
    if (names.size() > 1
            && (m_nargs != NARGS_NUM || names.size() != m_num_args)) {
        throw TypeError("length of metavar tuple does not match nargs");
    }
    if (names.size() == 1
            && m_nargs == NARGS_NUM && names.size() != m_num_args) {
        names.resize(m_num_args, names.front());
    }
    std::string const name = names_size > 1
            ? ("[" + detail::_join(names, ", ") + "]") : detail::_join(names);
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

_ARGPARSE_INL std::vector<std::string>
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

_ARGPARSE_INL std::vector<std::string> const&
Argument::get_argument_flags() const _ARGPARSE_NOEXCEPT
{
    return dest().empty() ? m_flags : m_dest;
}

_ARGPARSE_INL void
Argument::resolve_conflict_flags(
        std::vector<std::string> const& flags)
{
    detail::_resolve_conflict(flags, m_flags);
    detail::_resolve_conflict(flags, m_all_flags);
}

_ARGPARSE_INL std::string
Argument::error_nargs(
        std::string const& arg) const
{
    switch (m_nargs) {
        case NARGS_DEF :
            return "argument " + arg + ": expected one argument";
        case NARGS_NUM :
            return "argument " + arg + ": expected " + nargs() + " arguments";
        case ONE_OR_MORE :
            return "argument " + arg + ": expected at least one argument";
        case ZERO_OR_ONE :
        case ZERO_OR_MORE :
        case REMAINDING :
        case SUPPRESSING :
        default :
            return std::string();
    }
}

_ARGPARSE_INL bool
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
                std::string const& flag = m_flags.at(j);
                if (flag == value || (flag + "=" == value)) {
                    return true;
                }
            }
            return false;
        case Optional :
            for (std::size_t j = 0; j < m_flags.size(); ++j) {
                std::string const& flag = m_flags.at(j);
                if (flag == value || m_name == value
                        || detail::_flag_name(flag) == value) {
                    return true;
                }
            }
            return false;
        default :
            return false;
    }
}

_ARGPARSE_INL bool
Argument::operator ==(
        Argument const& rhs) const
{
    return m_flags == rhs.m_flags
            && m_name == rhs.m_name
            && m_type == rhs.m_type
            && m_action == rhs.m_action
            && m_const == rhs.m_const
            && dest() == rhs.dest();
}

_ARGPARSE_INL bool
Argument::operator ==(
        std::string const& rhs) const
{
    return !dest().empty() ? dest() == rhs : detail::_exists(rhs, m_flags);
}

// -- _Group ------------------------------------------------------------------
_ARGPARSE_INL
_Group::_Group(
        std::string const& title,
        std::string const& description)
    : m_title(),
      m_description()
{
    m_title[std::string()] = title;
    m_description[std::string()] = description;
}

_ARGPARSE_INL std::string const&
_Group::title() const
{
    return detail::_map_at(m_title, std::string());
}

_ARGPARSE_INL std::string const&
_Group::description() const
{
    return detail::_map_at(m_description, std::string());
}

// -- _ArgumentData -----------------------------------------------------------
_ARGPARSE_INL
_ArgumentData::_ArgumentData()
    : m_conflict_handler(),
      m_arguments(),
      m_optional(),
      m_operand(),
      m_positional(),
      m_add_help(false),
      m_help_added(false)
{
}

_ARGPARSE_INL detail::shared_ptr<_ArgumentData>
_ArgumentData::make_argument_data()
{
    return detail::make_shared<_ArgumentData>(_ArgumentData());
}

_ARGPARSE_INL void
_ArgumentData::check_conflict_arg(
        Argument const* arg)
{
    for (sub_iterator it = m_optional.begin(); it != m_optional.end(); ++it) {
        if ((*it).first.get() == arg) {
            return;
        }
        check_conflicting_option(arg, (*it).first->m_all_flags);
        check_conflicting_option(arg, (*it).first->m_flags);
    }
}

_ARGPARSE_INL void
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
        if (m_conflict_handler == "resolve") {
            for (sub_iterator it = m_optional.begin();
                 it != m_optional.end(); ++it) {
                detail::_resolve_conflict((*it).first->flags(), help_flags);
            }
        }
        if (!help_flags.empty()) {
            pArgument arg = Argument::make_argument(
                        _ARGPARSE_MOVE(help_flags), "help", Argument::Optional);
            arg->help("show this help message and exit").action(argparse::help);
            m_arguments.push_front(arg);
            m_optional.push_front(std::make_pair(m_arguments.front(), false));
            m_help_added = true;
        }
    }
}

_ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_arguments(
        bool add_suppress) const
{
    std::vector<pArgument> res;
    res.reserve(m_arguments.size());
    for (arg_iterator it = m_arguments.begin(); it != m_arguments.end(); ++it) {
        if ((add_suppress || !(*it)->m_help_type.has_value())
                && ((*it)->m_type != Argument::Optional
                    || !(*it)->flags().empty())) {
            res.push_back(*it);
        }
    }
    return res;
}

_ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_optional(
        bool add_suppress,
        bool add_group) const
{
    std::vector<pArgument> res;
    res.reserve(m_optional.size());
    for (sub_iterator it = m_optional.begin(); it != m_optional.end(); ++it) {
        if ((add_suppress || !(*it).first->m_help_type.has_value())
                && (add_group || !(*it).second)
                && !(*it).first->flags().empty()) {
            res.push_back((*it).first);
        }
    }
    return res;
}

_ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_operand(
        bool add_suppress,
        bool add_group) const
{
    std::vector<pArgument> res;
    res.reserve(m_operand.size());
    for (sub_iterator it = m_operand.begin(); it != m_operand.end(); ++it) {
        if ((add_suppress || !(*it).first->m_help_type.has_value())
                && (add_group || !(*it).second)) {
            res.push_back((*it).first);
        }
    }
    return res;
}

_ARGPARSE_INL std::vector<_ArgumentData::pArgument>
_ArgumentData::get_positional(
        bool add_suppress,
        bool add_group) const
{
    std::vector<pArgument> res;
    res.reserve(m_positional.size());
    for (sub_iterator i = m_positional.begin(); i != m_positional.end(); ++i) {
        if ((add_suppress || !(*i).first->m_help_type.has_value())
                && (add_group || !(*i).second)) {
            res.push_back((*i).first);
        }
    }
    return res;
}

_ARGPARSE_INL void
_ArgumentData::check_conflicting_option(
        Argument const* arg,
        std::vector<std::string>& flags) const
{
    std::vector<std::string> conflict_options;
    for (std::size_t i = 0; i < arg->flags().size(); ++i) {
        std::string const& flag = arg->flags().at(i);
        std::vector<std::string>::iterator it
                = std::find(flags.begin(), flags.end(), flag);
        if (it != flags.end()) {
            if (m_conflict_handler == "resolve") {
                flags.erase(it);
            } else {
                conflict_options.push_back(flag);
            }
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

_ARGPARSE_INL void
_ArgumentData::merge_arguments(
        _ArgumentData const& d)
{
    for (sub_iterator i = d.m_optional.begin(); i != d.m_optional.end(); ++i) {
        check_conflict_arg((*i).first.get());
        m_optional.push_back(*i);
    }
    m_operand.insert(m_operand.end(), d.m_operand.begin(), d.m_operand.end());
    m_positional.insert(m_positional.end(),
                        d.m_positional.begin(), d.m_positional.end());
    m_arguments.insert(m_arguments.end(),
                       d.m_arguments.begin(), d.m_arguments.end());
}

_ARGPARSE_INL void
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
    detail::_update_flag_name(flags, prefix_chars,
                              type == Argument::Optional, flag, prefixes);
    if (type == Argument::Optional) {
        flag = detail::_replace(flag, '-', "_");
    }
    pArgument arg = Argument::make_argument(
                _ARGPARSE_MOVE(flags), _ARGPARSE_MOVE(flag), type);
    if (type == Argument::Optional) {
        data->check_conflict_arg(arg.get());
    }
    data->m_arguments.push_back(arg);
    if (type == Argument::Optional) {
        data->m_arguments.back()->m_post_trigger = data;
    }
}

_ARGPARSE_INL void
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
        detail::_update_flag_name(
                    flags, prefix_chars,
                    arg.m_type == Argument::Optional, flag, prefixes);
        arg.m_name = arg.m_type == Argument::Optional
                ? detail::_replace(flag, '-', "_") : flag;
    }
    // check
    if (arg.m_type == Argument::Positional) {
        if (arg.dest().empty() && flags.empty()) {
            throw TypeError("missing 1 required positional argument: 'dest'");
        }
        if (arg.action()
              & (argparse::version | argparse::help | argparse::language)) {
            // version, help and language actions cannot be positional
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
        if (arg.m_metavar.has_value() && arg.m_metavar().size() != 1) {
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
_ARGPARSE_INL
_ArgumentGroup::_ArgumentGroup(
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::Value<std::string>& argument_default,
        detail::Value<_SUPPRESS>& argument_default_type,
        bool is_mutex_group)
    : m_data(_ArgumentData::make_argument_data()),
      m_prefix_chars(prefix_chars),
      m_parent_data(parent_data),
      m_argument_default(argument_default),
      m_argument_default_type(argument_default_type),
      m_is_mutex_group(is_mutex_group)
{
}

_ARGPARSE_INL
_ArgumentGroup::_ArgumentGroup(
        _ArgumentGroup const& orig)
    : m_data(orig.m_data),
      m_prefix_chars(orig.m_prefix_chars),
      m_parent_data(orig.m_parent_data),
      m_argument_default(orig.m_argument_default),
      m_argument_default_type(orig.m_argument_default_type),
      m_is_mutex_group(orig.m_is_mutex_group)
{
}

_ARGPARSE_INL _ArgumentGroup&
_ArgumentGroup::operator =(
        _ArgumentGroup const& rhs)
{
    if (this != &rhs) {
        m_data                  = rhs.m_data;
        m_prefix_chars          = rhs.m_prefix_chars;
        m_parent_data           = rhs.m_parent_data;
        m_argument_default      = rhs.m_argument_default;
        m_argument_default_type = rhs.m_argument_default_type;
        m_is_mutex_group        = rhs.m_is_mutex_group;
    }
    return *this;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::initializer_list<std::string> const& flags)
{
    return add_argument(std::vector<std::string>{ flags });
}
#else
_ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag)
{
    return add_argument(detail::_vector(flag));
}

_ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag1,
        std::string const& flag2)
{
    return add_argument(detail::_vector(flag1, flag2));
}

_ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3)
{
    return add_argument(detail::_vector(flag1, flag2, flag3));
}

_ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3,
        std::string const& flag4)
{
    return add_argument(detail::_vector(flag1, flag2, flag3, flag4));
}
#endif  // C++11+

_ARGPARSE_INL Argument&
_ArgumentGroup::add_argument(
        std::vector<std::string> const& flags)
{
    m_data->create_argument(m_data, flags, m_prefix_chars);
    process_add_argument();
    return *m_data->m_arguments.back();
}

_ARGPARSE_INL void
_ArgumentGroup::process_add_argument()
{
    Argument::Type type = m_data->m_arguments.back()->m_type;
    if (type == Argument::Optional
            && m_parent_data->m_conflict_handler == "resolve") {
        for (sub_iterator it = m_parent_data->m_optional.begin();
             it != m_parent_data->m_optional.end(); ++it) {
            (*it).first->resolve_conflict_flags(
                        m_data->m_arguments.back()->flags());
        }
    }
    if (m_argument_default.has_value()
            && !m_data->m_arguments.back()->m_default.has_value()
            && !m_data->m_arguments.back()->m_default_type.has_value()) {
        m_data->m_arguments.back()->default_value(m_argument_default.value());
    }
    if (m_argument_default_type.has_value()) {
        m_data->m_arguments.back()->default_value(m_argument_default_type());
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
_ARGPARSE_INL
ArgumentGroup::ArgumentGroup(
        std::string const& title,
        std::string const& description,
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::Value<std::string>& argument_default,
        detail::Value<_SUPPRESS>& argument_default_type)
    : _Group(title, description),
      _ArgumentGroup(prefix_chars, parent_data,
                     argument_default, argument_default_type, false)
{
}

_ARGPARSE_INL detail::shared_ptr<ArgumentGroup>
ArgumentGroup::make_argument_group(
        std::string const& title,
        std::string const& description,
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::Value<std::string>& argument_default,
        detail::Value<_SUPPRESS>& argument_default_type)
{
    return detail::make_shared<ArgumentGroup>(
                ArgumentGroup(title, description, prefix_chars, parent_data,
                              argument_default, argument_default_type));
}

_ARGPARSE_INL
ArgumentGroup::ArgumentGroup(
        ArgumentGroup const& orig)
    : _Group(orig),
      _ArgumentGroup(orig)
{
}

_ARGPARSE_INL ArgumentGroup&
ArgumentGroup::operator =(
        ArgumentGroup const& rhs)
{
    if (this != &rhs) {
        m_title                 = rhs.m_title;
        m_description           = rhs.m_description;
        m_data                  = rhs.m_data;
        m_prefix_chars          = rhs.m_prefix_chars;
        m_argument_default      = rhs.m_argument_default;
        m_argument_default_type = rhs.m_argument_default_type;
        m_parent_data           = rhs.m_parent_data;
    }
    return *this;
}

_ARGPARSE_INL ArgumentGroup&
ArgumentGroup::title(
        std::string const& value,
        std::string const& lang)
{
    m_title[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentGroup&
ArgumentGroup::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentGroup&
ArgumentGroup::add_argument(
        Argument const& argument)
{
    m_data->validate_argument(Argument(argument), m_prefix_chars);
    process_add_argument();
    return *this;
}

_ARGPARSE_INL void
ArgumentGroup::limit_help_flags(
        HelpFormatter const& formatter,
        std::size_t& limit) const
{
    for (arg_iterator it = m_data->m_arguments.begin();
         it != m_data->m_arguments.end(); ++it) {
        detail::_limit_to_min(limit, (*it)->flags_to_string(formatter).size());
    }
}

_ARGPARSE_INL void
ArgumentGroup::print_help(
        std::ostream& os,
        HelpFormatter const& formatter,
        std::string const& prog,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    std::string const description = detail::_tr(m_description, lang);
    if (!description.empty() || !m_data->m_arguments.empty()) {
        std::string const title = detail::_tr(m_title, lang);
        if (!title.empty()) {
            os << "\n" << title << ":";
        }
        detail::_print_raw_text_formatter(
                    formatter,
                    detail::_replace(description, "%(prog)s", prog),
                    width, os, "\n", 2);
        if (!m_data->m_arguments.empty()) {
            os << std::endl;
        }
        for (arg_iterator it = m_data->m_arguments.begin();
             it != m_data->m_arguments.end(); ++it) {
            os << (*it)->print(formatter, limit, width, lang) << std::endl;
        }
    }
}

// -- MutuallyExclusiveGroup --------------------------------------------------
_ARGPARSE_INL
MutuallyExclusiveGroup::MutuallyExclusiveGroup(
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::Value<std::string>& argument_default,
        detail::Value<_SUPPRESS>& argument_default_type)
    : _ArgumentGroup(prefix_chars, parent_data,
                     argument_default, argument_default_type, true),
      m_required(false)
{
}

_ARGPARSE_INL MutuallyExclusiveGroup
MutuallyExclusiveGroup::make_mutex_group(
        std::string& prefix_chars,
        pArgumentData& parent_data,
        detail::Value<std::string>& argument_default,
        detail::Value<_SUPPRESS>& argument_default_type)
{
    return MutuallyExclusiveGroup(prefix_chars, parent_data,
                                  argument_default, argument_default_type);
}

_ARGPARSE_INL
MutuallyExclusiveGroup::MutuallyExclusiveGroup(
        MutuallyExclusiveGroup const& orig)
    : _ArgumentGroup(orig),
      m_required(orig.m_required)
{
}

_ARGPARSE_INL MutuallyExclusiveGroup&
MutuallyExclusiveGroup::operator =(
        MutuallyExclusiveGroup const& rhs)
{
    if (this != &rhs) {
        m_data                  = rhs.m_data;
        m_prefix_chars          = rhs.m_prefix_chars;
        m_parent_data           = rhs.m_parent_data;
        m_argument_default      = rhs.m_argument_default;
        m_argument_default_type = rhs.m_argument_default_type;
        m_required              = rhs.m_required;
    }
    return *this;
}

_ARGPARSE_INL MutuallyExclusiveGroup&
MutuallyExclusiveGroup::required(bool value) _ARGPARSE_NOEXCEPT
{
    m_required = value;
    return *this;
}

_ARGPARSE_INL bool
MutuallyExclusiveGroup::required() const _ARGPARSE_NOEXCEPT
{
    return m_required;
}

_ARGPARSE_INL MutuallyExclusiveGroup&
MutuallyExclusiveGroup::add_argument(
        Argument const& argument)
{
    m_data->validate_argument(Argument(argument), m_prefix_chars);
    process_add_argument();
    return *this;
}

_ARGPARSE_INL std::string
MutuallyExclusiveGroup::usage(
        HelpFormatter const& formatter) const
{
    std::string res;
    for (arg_iterator it = m_data->m_arguments.begin();
         it != m_data->m_arguments.end(); ++it) {
        detail::_append_value_to((*it)->usage(formatter), res, " | ");
    }
    return res.empty() ? res : (m_required ? "(" + res + ")" : "[" + res + "]");
}

// -- _Storage ----------------------------------------------------------------
_ARGPARSE_INL void
_Storage::create(
        key_type const& key,
        mapped_type const& value)
{
    if (key->action() & (argparse::version | argparse::help)) {
        return;
    }
    std::vector<std::string> const& arg_flags = key->flags();
    bool have_key = false;
    for (const_iterator it = m_data.begin(); it != m_data.end(); ++it) {
        have_key |= (key == (*it).first);
        if (key != (*it).first) {
            (*it).first->resolve_conflict_flags(arg_flags);
        }
    }
    if (!have_key) {
        m_data.push_back(std::make_pair(key, value));
    }
}

_ARGPARSE_INL void
_Storage::create(
        std::vector<key_type> const& keys)
{
    for (std::size_t i = 0; i < keys.size(); ++i) {
        create(keys.at(i));
    }
}

_ARGPARSE_INL void
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

_ARGPARSE_INL void
_Storage::store_value(
        key_type const& key,
        std::string const& value)
{
    at(key).push_back(value);
    on_process_store(key, value);
    key->handle(value);
}

_ARGPARSE_INL void
_Storage::store_values(
        key_type const& key,
        std::vector<std::string> const& values)
{
    at(key).push_values(values);
    on_process_store(key, values);
#ifdef _ARGPARSE_CXX_11
    for (auto const& value : values) {
        key->handle(value);
    }
#endif  // C++11+
}

_ARGPARSE_INL void
_Storage::store_default_value(
        key_type const& key)
{
    if (key->m_default.has_value()
            && (key->action()
                & (argparse::store | argparse::BooleanOptionalAction))) {
        mapped_type& arg_data = at(key);
        if (arg_data.empty()) {
            arg_data.push_default(key->m_default.value());
            on_process_store(key, key->m_default.value());
            key->handle(key->m_default.value());
        }
    }
}

_ARGPARSE_INL bool
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

_ARGPARSE_INL void
_Storage::on_process_store(
        key_type const& key,
        std::string const& value)
{
    std::string const& dest = key->dest();
    if (!dest.empty()) {
        for (iterator it = begin(); it != end(); ++it) {
            if (it->first != key && it->first->dest() == dest) {
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

_ARGPARSE_INL void
_Storage::on_process_store(
        key_type const& key,
        std::vector<std::string> const& values)
{
    std::string const& dest = key->dest();
    if (!dest.empty()) {
        for (iterator it = begin(); it != end(); ++it) {
            if (it->first != key && it->first->dest() == dest) {
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
_ARGPARSE_INL
Namespace::Namespace(
        _Storage const& storage)
    : m_storage(storage),
      m_unrecognized_args()
{
}

_ARGPARSE_INL
Namespace::Namespace(
        _Storage const& storage,
        std::vector<std::string> const& args)
    : m_storage(storage),
      m_unrecognized_args(args)
{
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL
Namespace::Namespace(
        _Storage&& storage) _ARGPARSE_NOEXCEPT
    : m_storage(std::move(storage)),
      m_unrecognized_args()
{
}

_ARGPARSE_INL
Namespace::Namespace(
        _Storage&& storage,
        std::vector<std::string>&& args) _ARGPARSE_NOEXCEPT
    : m_storage(std::move(storage)),
      m_unrecognized_args(std::move(args))
{
}
#endif  // C++11+

_ARGPARSE_INL bool
Namespace::contains(
        std::string const& key) const
{
    _Storage::const_iterator it = storage().find_arg(key);
    if (it != storage().end()) {
        return !it->second.empty() || it->first->action() == argparse::count;
    }
    return false;
}

_ARGPARSE_INL bool
Namespace::exists(
        std::string const& key) const
{
    return contains(key);
}

_ARGPARSE_INL void
Namespace::print(
        std::ostream& os) const
{
    os << to_string() << std::endl;
}

_ARGPARSE_INL std::string
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
                          args.second.front(), detail::_space, "\\ ");
        case argparse::store_true :
        case argparse::store_false :
        case argparse::BooleanOptionalAction :
            return boolean_option_to_args(key, args);
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

_ARGPARSE_INL std::string
Namespace::to_string(
        std::string const& key,
        std::string const& quotes) const
{
    _Storage::value_type const& args = data(key);
    switch (args.first->action()) {
        case argparse::store_const :
            if (args.second.empty()) {
                return std::string("None");
            }
            if (args.second.size() != 1) {
                throw TypeError("got a data-array for argument '" + key + "'");
            }
            return quotes + args.second.front() + quotes;
        case argparse::store_true :
        case argparse::store_false :
        case argparse::BooleanOptionalAction :
            return boolean_option_to_string(key, args, quotes);
        case argparse::count :
            return args.second.empty()
                    ? "None" : detail::_to_string(args.second.size());
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

_ARGPARSE_INL std::string
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

_ARGPARSE_INL std::vector<std::string> const&
Namespace::unrecognized_args() const _ARGPARSE_NOEXCEPT
{
    return m_unrecognized_args();
}

_ARGPARSE_INL std::string
Namespace::unrecognized_args_to_args() const
{
    return detail::_vector_to_string(
                unrecognized_args(), detail::_spaces, std::string(), true);
}

_ARGPARSE_INL std::string
Namespace::boolean_option_to_args(
        std::string const& key,
        _Storage::value_type const& args) const
{
    if (args.second.empty()) {
        return detail::_bool_to_string(args.first->default_value());
    }
    if (args.second.size() != 1) {
        throw TypeError("got a data-array for argument '" + key + "'");
    }
    return (args.first->action() != argparse::BooleanOptionalAction
            || args.second.front() == args.first->const_value())
           ? detail::_bool_to_string(args.second.front()) : args.second.front();
}

_ARGPARSE_INL std::string
Namespace::boolean_option_to_string(
        std::string const& key,
        _Storage::value_type const& args,
        std::string const& quotes) const
{
    if (args.second.empty()) {
        return args.first->action() == argparse::BooleanOptionalAction
                ? "None" : detail::_bool_to_string(args.first->default_value());
    }
    if (args.second.size() != 1) {
        throw TypeError("got a data-array for argument '" + key + "'");
    }
    return args.second.is_default()
            ? quotes + args.second.front() + quotes
            : detail::_bool_to_string(args.second.front());
}

_ARGPARSE_INL std::string
Namespace::store_actions_to_string(
        _Storage::value_type const& args,
        std::string const& quotes) const
{
    if (((args.first->action() & (argparse::store | argparse::language))
         && (args.first->m_nargs
             & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE)))
            || (!args.second.exists()
                && args.first->m_type == Argument::Optional)
            || args.first->m_nargs == Argument::SUPPRESSING
            || args.second.is_default()) {
        return detail::_vector_to_string(args.second(), ", ",
                                         quotes, false, "None");
    }
    if (args.first->action() != argparse::append
            || (args.first->m_nargs
                & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE))) {
        std::string none
                = (args.first->m_nargs
                   & (Argument::ZERO_OR_MORE | Argument::REMAINDING))
                || (args.first->action() == argparse::extend
                    && args.first->m_nargs == Argument::ZERO_OR_ONE)
                ? std::string() : "None";
        return detail::_vector_to_string(args.second(),
                                         ", ", quotes, false, none, "[", "]");
    } else {
        std::string none = (args.first->m_nargs
                            & (Argument::ZERO_OR_MORE | Argument::REMAINDING))
                ? std::string() : "None";
        return detail::_matrix_to_string(args.second(), args.second.indexes(),
                                         ", ", quotes, false, none, "[", "]");
    }
}

_ARGPARSE_INL _Storage::value_type const&
Namespace::data(
        std::string const& key) const
{
    _Storage::const_iterator it = storage().find_arg(key);
    if (it != storage().end()) {
        return *it;
    }
    throw AttributeError("'Namespace' object has no attribute '" + key + "'");
}

_ARGPARSE_INL _Storage const&
Namespace::storage() const _ARGPARSE_NOEXCEPT
{
    return m_storage;
}

// -- ArgumentParser::Subparser -----------------------------------------------
_ARGPARSE_INL
ArgumentParser::Subparser::Subparser(
        std::string const& title,
        std::string const& description)
    : _Group(title, description),
      m_parent_prog(),
      m_parent_args(),
      m_prog(),
      m_dest(),
      m_help(),
      m_metavar(),
      m_parsers(),
      m_help_type(),
      m_required(false)
{
    m_help[std::string()] = std::string();
}

_ARGPARSE_INL detail::shared_ptr<ArgumentParser::Subparser>
ArgumentParser::Subparser::make_subparser(
        std::string const& title,
        std::string const& description)
{
    return detail::make_shared<Subparser>(Subparser(title, description));
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::title(
        std::string const& value,
        std::string const& lang)
{
    m_title[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::prog(
        std::string const& value)
{
    m_prog = value;
    update_prog(m_parent_prog, m_parent_args);
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::dest(
        std::string const& value)
{
    m_dest = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::required(
        bool value) _ARGPARSE_NOEXCEPT
{
    m_required = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::help(
        std::string const& value,
        std::string const& lang)
{
    if (lang.empty()) {
        m_help_type.reset();
    }
    m_help[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::help(
        _SUPPRESS value)
{
    if (value != argparse::SUPPRESS) {
        throw TypeError("got an unexpected keyword argument 'help'");
    }
    m_help_type = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::Subparser::metavar(
        std::string const& value)
{
    m_metavar = value;
    return *this;
}

_ARGPARSE_INL std::string const&
ArgumentParser::Subparser::prog() const _ARGPARSE_NOEXCEPT
{
    return m_prog;
}

_ARGPARSE_INL std::string const&
ArgumentParser::Subparser::dest() const _ARGPARSE_NOEXCEPT
{
    return m_dest;
}

_ARGPARSE_INL bool
ArgumentParser::Subparser::required() const _ARGPARSE_NOEXCEPT
{
    return m_required;
}

_ARGPARSE_INL std::string const&
ArgumentParser::Subparser::help() const
{
    return detail::_map_at(m_help, std::string());
}

_ARGPARSE_INL std::string const&
ArgumentParser::Subparser::metavar() const _ARGPARSE_NOEXCEPT
{
    return m_metavar();
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::Subparser::add_parser(
        std::string const& name)
{
    if (name.empty()) {
        throw ValueError("parser name can't be empty");
    }
    m_parsers.push_back(make_parser(name));
    m_parsers.back()->update_prog(prog_name());
    return *m_parsers.back();
}

_ARGPARSE_INL std::vector<std::string>
ArgumentParser::Subparser::parser_names() const
{
    std::vector<std::string> res;
    res.reserve(2 * m_parsers.size());
    for (std::size_t i = 0; i < m_parsers.size(); ++i) {
        res.push_back(m_parsers.at(i)->m_name);
        for (std::size_t j = 0; j < m_parsers.at(i)->aliases().size(); ++j) {
            res.push_back(m_parsers.at(i)->aliases().at(j));
        }
    }
    return res;
}

_ARGPARSE_INL void
ArgumentParser::Subparser::limit_help_flags(
        HelpFormatter const&,
        std::size_t& limit) const
{
    detail::_limit_to_min(limit, flags_to_string().size());
    for (std::size_t i = 0; i < m_parsers.size(); ++i) {
        detail::_limit_to_min(limit, m_parsers.at(i)->m_name.size() + 2);
    }
}

_ARGPARSE_INL void
ArgumentParser::Subparser::print_help(
        std::ostream& os,
        HelpFormatter const& formatter,
        std::string const& prog,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    std::string const title = detail::_tr(m_title, lang);
    os << "\n" << (title.empty() ? "subcommands" : title) << ":\n";
    detail::_print_raw_text_formatter(
                formatter,
                detail::_replace(
                    detail::_tr(m_description, lang), "%(prog)s", prog),
                width, os, std::string(), 2, "\n");
    os << print(formatter, limit, width, lang) << std::endl;
}

_ARGPARSE_INL std::string
ArgumentParser::Subparser::prog_name() const
{
    std::string program = m_prog.empty() ? m_parent_prog : m_prog;
    if (!m_parent_args.empty()) {
        program += " " + m_parent_args;
    }
    return program;
}

_ARGPARSE_INL void
ArgumentParser::Subparser::update_prog(
        std::string const& parent_prog,
        std::string const& parent_args)
{
    m_parent_prog = parent_prog;
    m_parent_args = parent_args;
    std::string const program = prog_name();
    for (std::size_t i = 0; i < m_parsers.size(); ++i) {
        m_parsers.at(i)->update_prog(program);
    }
}

_ARGPARSE_INL std::string
ArgumentParser::Subparser::usage() const
{
    return flags_to_string() + " ...";
}

_ARGPARSE_INL std::string
ArgumentParser::Subparser::flags_to_string() const
{
    if (m_metavar.has_value()) {
        return metavar();
    }
    std::string res;
    for (std::size_t i = 0; i < m_parsers.size(); ++i) {
        pParser const& p = m_parsers.at(i);
        detail::_append_value_to(p->m_name, res, ",");
        for (std::size_t j = 0; j < p->aliases().size(); ++j) {
            detail::_append_value_to(p->aliases().at(j), res, ",");
        }
    }
    return "{" + res + "}";
}

_ARGPARSE_INL std::string
ArgumentParser::Subparser::print(
        HelpFormatter const& formatter,
        std::size_t limit,
        std::size_t width,
        std::string const& lang) const
{
    std::string res = detail::_help_formatter(
                "  " + flags_to_string(), formatter,
                detail::_tr(m_help, lang), width, limit);
    for (std::size_t i = 0; i < m_parsers.size(); ++i) {
        pParser const& p = m_parsers.at(i);
        std::string help = detail::_tr(p->m_help, lang);
        if (!help.empty()) {
            std::string name = "    " + p->m_name;
            std::string alias = detail::_join(p->aliases(), ", ");
            if (!alias.empty()) {
                name += " (" + alias + ")";
            }
            res += "\n" + detail::_help_formatter(
                        name, formatter, help, width, limit);
        }
    }
    return res;
}

// -- ArgumentParser ----------------------------------------------------------
_ARGPARSE_INL ArgumentParser::pParser
ArgumentParser::make_parser(
        std::string const& name)
{
    pParser res = detail::make_shared<ArgumentParser>();
    res->m_prog.clear();
    res->m_name = name;
    return res;
}

_ARGPARSE_INL void
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

_ARGPARSE_INL void
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

_ARGPARSE_INL void
ArgumentParser::initialize_parser()
{
    formatter_class(HelpFormatter());
    m_data->update_help(true, m_prefix_chars);
    // init translations
    m_usage[std::string()] = std::string();
    m_usage_title[std::string()] = "usage";
    m_description[std::string()] = std::string();
    m_positionals_title[std::string()] = "positional arguments";
    m_operands_title[std::string()] = "operands";
    m_optionals_title[std::string()] = "options";
    m_epilog[std::string()] = std::string();
    m_help[std::string()] = std::string();
}

_ARGPARSE_INL
ArgumentParser::ArgumentParser(
        std::string const& prog)
    : m_data(_ArgumentData::make_argument_data()),
      m_name(),
      m_prog("untitled"),
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
      m_subparsers(_ARGPARSE_NULLPTR),
      m_subparsers_position(),
#ifdef _ARGPARSE_CXX_11
      m_handle(nullptr),
      m_parse_handle(nullptr),
#endif  // C++11+
      m_argument_default_type(),
      m_allow_abbrev(true),
      m_exit_on_error(true)
{
    initialize_parser();
    this->prog(prog);
}

_ARGPARSE_INL
ArgumentParser::ArgumentParser(
        int argc,
        char const* const argv[],
        std::string const& prog)
    : m_data(_ArgumentData::make_argument_data()),
      m_name(),
      m_prog("untitled"),
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
      m_subparsers(_ARGPARSE_NULLPTR),
      m_subparsers_position(),
#ifdef _ARGPARSE_CXX_11
      m_handle(nullptr),
      m_parse_handle(nullptr),
#endif  // C++11+
      m_argument_default_type(),
      m_allow_abbrev(true),
      m_exit_on_error(true)
{
    initialize_parser();
    read_args(argc, argv);
    this->prog(prog);
}

_ARGPARSE_INL
ArgumentParser::ArgumentParser(
        int argc,
        char const* const argv[],
        char const* const envp[],
        std::string const& prog)
    : m_data(_ArgumentData::make_argument_data()),
      m_name(),
      m_prog("untitled"),
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
      m_subparsers(_ARGPARSE_NULLPTR),
      m_subparsers_position(),
#ifdef _ARGPARSE_CXX_11
      m_handle(nullptr),
      m_parse_handle(nullptr),
#endif  // C++11+
      m_argument_default_type(),
      m_allow_abbrev(true),
      m_exit_on_error(true)
{
    initialize_parser();
    read_args(argc, argv);
    read_env(envp);
    this->prog(prog);
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::prog(
        std::string const& value)
{
    if (!value.empty()) {
        m_prog = value;
        if (m_subparsers) {
            m_subparsers->update_prog(prog(), subparser_prog_args());
        }
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::usage(
        std::string const& value,
        std::string const& lang)
{
    m_usage[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::usage_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_usage_title[lang] = value;
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::description(
        std::string const& value,
        std::string const& lang)
{
    m_description[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::positionals_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_positionals_title[lang] = value;
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::operands_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_operands_title[lang] = value;
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::optionals_title(
        std::string const& value,
        std::string const& lang)
{
    if (!value.empty()) {
        m_optionals_title[lang] = value;
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::epilog(
        std::string const& value,
        std::string const& lang)
{
    m_epilog[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::help(
        std::string const& value,
        std::string const& lang)
{
    m_help[lang] = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::vector<std::string> const& value)
{
    m_aliases.clear();
    for (std::size_t i = 0; i < value.size(); ++i) {
        std::string const& val = value.at(i);
        if (!val.empty()) {
            m_aliases.push_back(val);
        }
    }
    return *this;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::initializer_list<std::string> const& value)
{
    return aliases(std::vector<std::string>{ value });
}
#else
_ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
            std::string const& value)
{
    return aliases(detail::_vector(value));
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::aliases(
        std::string const& value1,
        std::string const& value2)
{
    return aliases(detail::_vector(value1, value2));
}
#endif  // C++11+

_ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        std::vector<ArgumentParser> const& value)
{
    for (std::size_t i = 0; i < value.size(); ++i) {
        ArgumentParser const& parent = value.at(i);
        if (this == &parent) {
            continue;
        }
        if (parent.m_subparsers) {
            if (m_subparsers) {
                throw_error("cannot have multiple subparser arguments");
            }
            m_subparsers_position
                   = parent.m_subparsers_position + m_data->m_positional.size();
            m_subparsers = parent.m_subparsers;
            m_subparsers->update_prog(prog(), subparser_prog_args());
        }
        m_data->merge_arguments(*parent.m_data.get());
        m_groups.insert(m_groups.end(),
                        parent.m_groups.begin(), parent.m_groups.end());
        m_mutex_groups.insert(m_mutex_groups.end(),
                              parent.m_mutex_groups.begin(),
                              parent.m_mutex_groups.end());
        for (std::size_t j = 0; j < parent.m_default_values.size(); ++j) {
            m_default_values.push_back(parent.m_default_values.at(j));
        }
    }
    return *this;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        std::initializer_list<ArgumentParser> const& value)
{
    return parents(std::vector<ArgumentParser>{ value });
}
#else
_ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        ArgumentParser const& value)
{
    return parents(detail::_vector(value));
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::parents(
        ArgumentParser const& value1,
        ArgumentParser const& value2)
{
    return parents(detail::_vector(value1, value2));
}
#endif  // C++11+

_ARGPARSE_INL ArgumentParser&
ArgumentParser::prefix_chars(
        std::string const& value)
{
    std::string val = detail::_get_punct(value);
    if (!val.empty()) {
        m_prefix_chars = val;
        m_data->update_help(m_data->m_add_help, m_prefix_chars);
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::fromfile_prefix_chars(
        std::string const& value)
{
    m_fromfile_prefix_chars = detail::_get_punct(value);
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::comment_prefix_chars(
        std::string const& value)
{
    m_comment_prefix_chars = detail::_get_punct(value);
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::argument_default(
        std::string const& value)
{
    m_argument_default = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::argument_default(
        _SUPPRESS value)
{
    if (value != argparse::SUPPRESS) {
        throw
        TypeError("got an unexpected keyword argument 'argument_default'");
    }
    m_argument_default_type = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::conflict_handler(
        std::string const& value)
{
    if (value != "resolve") {
        throw AttributeError("'ArgumentParser' object has no attribute "
                             "'_handle_conflict_" + value + "'");
    }
    m_data->m_conflict_handler = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::add_help(
        bool value)
{
    m_data->update_help(value, m_prefix_chars);
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::allow_abbrev(
        bool value) _ARGPARSE_NOEXCEPT
{
    m_allow_abbrev = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::exit_on_error(
        bool value) _ARGPARSE_NOEXCEPT
{
    m_exit_on_error = value;
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::output_width(
        std::size_t value) _ARGPARSE_NOEXCEPT
{
    m_output_width = value < detail::_min_width ? detail::_min_width : value;
    return *this;
}

_ARGPARSE_INL std::string const&
ArgumentParser::prog() const _ARGPARSE_NOEXCEPT
{
    return m_prog;
}

_ARGPARSE_INL std::string const&
ArgumentParser::usage() const
{
    return detail::_map_at(m_usage, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::usage_title() const
{
    return detail::_map_at(m_usage_title, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::description() const
{
    return detail::_map_at(m_description, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::positionals_title() const
{
    return detail::_map_at(m_positionals_title, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::operands_title() const
{
    return detail::_map_at(m_operands_title, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::optionals_title() const
{
    return detail::_map_at(m_optionals_title, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::epilog() const
{
    return detail::_map_at(m_epilog, default_language(this));
}

_ARGPARSE_INL std::string const&
ArgumentParser::help() const
{
    return detail::_map_at(m_help, default_language(this));
}

_ARGPARSE_INL std::vector<std::string> const&
ArgumentParser::aliases() const _ARGPARSE_NOEXCEPT
{
    return m_aliases;
}

_ARGPARSE_INL HelpFormatter&
ArgumentParser::formatter_class() _ARGPARSE_NOEXCEPT
{
    return *m_formatter;
}

_ARGPARSE_INL std::string const&
ArgumentParser::prefix_chars() const _ARGPARSE_NOEXCEPT
{
    return m_prefix_chars;
}

_ARGPARSE_INL std::string const&
ArgumentParser::fromfile_prefix_chars() const _ARGPARSE_NOEXCEPT
{
    return m_fromfile_prefix_chars;
}

_ARGPARSE_INL std::string const&
ArgumentParser::comment_prefix_chars() const _ARGPARSE_NOEXCEPT
{
    return m_comment_prefix_chars;
}

_ARGPARSE_INL std::string const&
ArgumentParser::argument_default() const _ARGPARSE_NOEXCEPT
{
    return m_argument_default();
}

_ARGPARSE_INL std::string const&
ArgumentParser::conflict_handler() const _ARGPARSE_NOEXCEPT
{
    return m_data->m_conflict_handler;
}

_ARGPARSE_INL bool
ArgumentParser::add_help() const _ARGPARSE_NOEXCEPT
{
    return m_data->m_add_help;
}

_ARGPARSE_INL bool
ArgumentParser::allow_abbrev() const _ARGPARSE_NOEXCEPT
{
    return m_allow_abbrev;
}

_ARGPARSE_INL bool
ArgumentParser::exit_on_error() const _ARGPARSE_NOEXCEPT
{
    return m_exit_on_error;
}

_ARGPARSE_INL std::size_t
ArgumentParser::output_width() const
{
    return m_output_width.has_value()
            ? m_output_width.value() : detail::_get_terminal_size().first;
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::initializer_list<std::string> const& flags)
{
    return add_argument(std::vector<std::string>{ flags });
}
#else
_ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag)
{
    return add_argument(detail::_vector(flag));
}

_ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag1,
        std::string const& flag2)
{
    return add_argument(detail::_vector(flag1, flag2));
}

_ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3)
{
    return add_argument(detail::_vector(flag1, flag2, flag3));
}

_ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::string const& flag1,
        std::string const& flag2,
        std::string const& flag3,
        std::string const& flag4)
{
    return add_argument(detail::_vector(flag1, flag2, flag3, flag4));
}
#endif  // C++11+

_ARGPARSE_INL Argument&
ArgumentParser::add_argument(
        std::vector<std::string> const& flags)
{
    m_data->create_argument(m_data, flags, prefix_chars());
    process_add_argument();
    return *m_data->m_arguments.back();
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::add_argument(
        Argument const& argument)
{
    m_data->validate_argument(Argument(argument), prefix_chars());
    process_add_argument();
    return *this;
}

_ARGPARSE_INL ArgumentGroup&
ArgumentParser::add_argument_group(
        std::string const& title,
        std::string const& description)
{
    detail::shared_ptr<ArgumentGroup> group
            = ArgumentGroup::make_argument_group(
                title, description, m_prefix_chars, m_data,
                m_argument_default, m_argument_default_type);
    m_groups.push_back(pGroup(group));
    return *group;
}

_ARGPARSE_INL MutuallyExclusiveGroup&
ArgumentParser::add_mutually_exclusive_group(
        bool required)
{
    m_mutex_groups.push_back(
                MutuallyExclusiveGroup::make_mutex_group(
                    m_prefix_chars, m_data,
                    m_argument_default, m_argument_default_type));
    return m_mutex_groups.back().required(required);
}

_ARGPARSE_INL ArgumentParser::Subparser&
ArgumentParser::add_subparsers(
        std::string const& title,
        std::string const& description)
{
    if (m_subparsers) {
        throw_error("cannot have multiple subparser arguments");
    }
    m_subparsers_position = m_data->m_positional.size();
    m_subparsers = Subparser::make_subparser(title, description);
    m_subparsers->update_prog(prog(), subparser_prog_args());
    m_groups.push_back(pGroup(m_subparsers));
    return *m_subparsers;
}

_ARGPARSE_INL ArgumentParser::Subparser*
ArgumentParser::subparsers() const _ARGPARSE_NOEXCEPT
{
    return m_subparsers.get();
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL ArgumentParser&
ArgumentParser::handle(
        std::function<void(std::string const&)> func) _ARGPARSE_NOEXCEPT
{
    m_handle = std::move(func);
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::handle(
        std::function<void()> func) _ARGPARSE_NOEXCEPT
{
    m_handle = std::bind([](std::function<void()>& f){ f(); }, std::move(func));
    return *this;
}

_ARGPARSE_INL ArgumentParser&
ArgumentParser::handle(
        std::function<void(Namespace const&)> func) _ARGPARSE_NOEXCEPT
{
    m_parse_handle = std::move(func);
    return *this;
}
#endif  // C++11+

_ARGPARSE_INL std::string
ArgumentParser::get_default(
        std::string const& dest) const
{
    pArguments const arguments = m_data->get_arguments(true);
    for (std::size_t i = 0; i < arguments.size(); ++i) {
        pArgument const& arg = arguments.at(i);
        if (arg->is_match_name(dest)) {
            return arg->is_suppressed() ? detail::_suppress : arg->m_default();
        }
    }
    for (std::size_t i = 0; i < m_default_values.size(); ++i) {
        if (m_default_values.at(i).first == dest) {
            return m_default_values.at(i).second;
        }
    }
    return std::string();
}

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL void
ArgumentParser::set_defaults(
       std::initializer_list<std::pair<std::string, std::string> > const& pairs)
{
    set_defaults(std::vector<std::pair<std::string, std::string> >{ pairs });
}
#endif  // C++11+

_ARGPARSE_INL void
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

_ARGPARSE_INL Namespace
ArgumentParser::parse_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, false, false, space);
}

_ARGPARSE_INL Namespace
ArgumentParser::parse_known_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, true, false, space);
}

_ARGPARSE_INL Namespace
ArgumentParser::parse_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, false, true, space);
}

_ARGPARSE_INL Namespace
ArgumentParser::parse_known_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_parse_arguments(args, true, true, space);
}

#ifdef _ARGPARSE_CXX_17
_ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, false, false, space);
}

_ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, true, false, space);
}

_ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, false, true, space);
}

_ARGPARSE_INL std::optional<Namespace>
ArgumentParser::try_parse_known_intermixed_args(
        std::vector<std::string> const& args,
        Namespace const& space) const
{
    return on_try_parse_arguments(args, true, true, space);
}
#endif  // C++17+

_ARGPARSE_INL bool
ArgumentParser::have_env(
        std::string const& name) const
{
    std::list<std::pair<std::string, std::string> >::const_iterator it
            = m_env_variables.begin();
    for ( ; it != m_env_variables.end(); ++it) {
        if (it->first == name) {
            break;
        }
    }
    return it != m_env_variables.end();
}

_ARGPARSE_INL std::string
ArgumentParser::get_env(
        std::string const& name) const
{
    std::list<std::pair<std::string, std::string> >::const_iterator it
            = m_env_variables.begin();
    for ( ; it != m_env_variables.end(); ++it) {
        if (it->first == name) {
            break;
        }
    }
    return it != m_env_variables.end() ? it->second : std::string();
}

_ARGPARSE_INL std::list<std::pair<std::string, std::string> > const&
ArgumentParser::list_env() const
{
    return m_env_variables;
}

_ARGPARSE_INL bool
ArgumentParser::self_test(
        std::ostream& os) const
{
    return self_test(std::string(), os);
}

_ARGPARSE_INL bool
ArgumentParser::self_test(
        std::string const& language,
        std::ostream& os) const
{
    std::string lang = !language.empty() ? language : default_language(this);
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

_ARGPARSE_INL void
ArgumentParser::print_bash_completion(
        std::ostream& os) const
{
    os << "# bash completion for " << prog() << "\n";
    os << "# generated with cpp-argparse v"
       << ARGPARSE_VERSION_MAJOR << "."
       << ARGPARSE_VERSION_MINOR << "."
       << ARGPARSE_VERSION_PATCH << "\n\n";
    print_parser_completion(this, prog(), true, os);
    os << "complete -F _" << prog() << " " << prog() << std::endl;
}

_ARGPARSE_INL void
ArgumentParser::print_usage(
        std::ostream& os) const
{
    print_usage(std::string(), os);
}

_ARGPARSE_INL void
ArgumentParser::print_usage(
        std::string const& language,
        std::ostream& os) const
{
    std::string lang = !language.empty() ? language : default_language(this);
    std::string tr_usage_title = detail::_tr(m_usage_title, lang) + ":";
    std::string tr_usage = detail::_tr(m_usage, lang);
    if (!tr_usage.empty()) {
        os << tr_usage_title << " " << despecify(tr_usage) << std::endl;
    } else {
        pArguments const positional = m_data->get_positional(false, true);
        pArguments const operand = m_data->get_operand(false, true);
        pArguments const optional = m_data->get_optional(false, true);
        print_custom_usage(positional, operand, optional, m_mutex_groups,
                           subparser_info(false), prog(), tr_usage_title, os);
    }
}

_ARGPARSE_INL void
ArgumentParser::print_help(
        std::ostream& os) const
{
    print_help(std::string(), os);
}

_ARGPARSE_INL void
ArgumentParser::print_help(
        std::string const& language,
        std::ostream& os) const
{
    std::string lang = !language.empty() ? language : default_language(this);
    pArguments const positional_all = m_data->get_positional(false, true);
    pArguments const operand_all = m_data->get_operand(false, true);
    pArguments const optional_all = m_data->get_optional(false, true);
    pArguments const positional = m_data->get_positional(false, false);
    pArguments const operand = m_data->get_operand(false, false);
    pArguments const optional = m_data->get_optional(false, false);
    SubparserInfo const sub_info = subparser_info(false);
    std::string tr_usage_title = detail::_tr(m_usage_title, lang) + ":";
    std::string tr_usage = detail::_tr(m_usage, lang);
    if (!tr_usage.empty()) {
        os << tr_usage_title << " " << despecify(tr_usage) << std::endl;
    } else {
        print_custom_usage(positional_all, operand_all, optional_all,
                          m_mutex_groups, sub_info, prog(), tr_usage_title, os);
    }
    std::size_t const width = output_width();
    detail::_print_raw_text_formatter(
                *m_formatter,
                despecify(detail::_tr(m_description, lang)), width, os);
    std::size_t size = 0;
    pSubparser subparser = sub_info.first;
    bool sub_positional = is_subparser_positional(subparser);
    for (std::size_t i = 0; i < positional.size(); ++i) {
        std::string str = positional.at(i)->flags_to_string(*m_formatter);
        detail::_limit_to_min(size, detail::_utf8_length(str).second);
    }
    for (std::size_t i = 0; i < operand.size(); ++i) {
        std::string str = operand.at(i)->flags_to_string(*m_formatter);
        detail::_limit_to_min(size, detail::_utf8_length(str).second);
    }
    for (std::size_t i = 0; i < optional.size(); ++i) {
        std::string str = optional.at(i)->flags_to_string(*m_formatter);
        detail::_limit_to_min(size, detail::_utf8_length(str).second);
    }
    for (grp_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        (*it)->limit_help_flags(*m_formatter, size);
    }
    size += 4;
    detail::_limit_to_max(size, output_width() - argument_help_limit());
    if (!positional.empty() || sub_positional) {
        os << "\n" << detail::_tr(m_positionals_title, lang) << ":\n";
        for (std::size_t i = 0; i < positional.size(); ++i) {
            print_subparser(sub_positional, sub_info, i,
                            *m_formatter, size, width, lang, os);
            os << despecify(positional.at(i)->print(
                                *m_formatter, size, width, lang)) << std::endl;
        }
        print_subparser(sub_positional, sub_info, positional.size(),
                        *m_formatter, size, width, lang, os);
    }
    if (!operand.empty()) {
        os << "\n" << detail::_tr(m_operands_title, lang) << ":\n";
        for (std::size_t i = 0; i < operand.size(); ++i) {
            os << despecify(operand.at(i)->print(
                                *m_formatter, size, width, lang)) << std::endl;
        }
    }
    if (!optional.empty()) {
        os << "\n" << detail::_tr(m_optionals_title, lang) << ":\n";
        for (std::size_t i = 0; i < optional.size(); ++i) {
            os << despecify(optional.at(i)->print(
                                *m_formatter, size, width, lang)) << std::endl;
        }
    }
    for (grp_iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        if (!subparser || ((*it) != subparser
                           || (!sub_positional
                               && !subparser->m_help_type.has_value()))) {
            (*it)->print_help(os, *m_formatter, prog(), size, width, lang);
        }
    }
    detail::_print_raw_text_formatter(
               *m_formatter, despecify(detail::_tr(m_epilog, lang)), width, os);
}

_ARGPARSE_INL std::string
ArgumentParser::format_bash_completion() const
{
    std::stringstream ss;
    print_bash_completion(ss);
    return detail::_trim_copy(ss.str());
}

_ARGPARSE_INL std::string
ArgumentParser::format_usage(
        std::string const& lang) const
{
    std::stringstream ss;
    print_usage(lang, ss);
    return detail::_trim_copy(ss.str());
}

_ARGPARSE_INL std::string
ArgumentParser::format_help(
        std::string const& lang) const
{
    std::stringstream ss;
    print_help(lang, ss);
    return detail::_trim_copy(ss.str());
}

_ARGPARSE_INL void
ArgumentParser::exit(
        int status,
        std::string const& message) const
{
    if (!message.empty()) {
        std::cerr << message << std::endl;
    }
    ::exit(status);
}

_ARGPARSE_INL void
ArgumentParser::error(
        std::string const& message) const
{
    print_usage(std::cerr);
    std::cerr << prog() << ": error: " << message << std::endl;
    ::exit(2);
}

_ARGPARSE_INL std::vector<std::string>
ArgumentParser::convert_arg_line_to_args(
        std::string const& arg_line) const
{
    return detail::_vector(arg_line);
}

_ARGPARSE_INL
ArgumentParser::CompletionInfo::CompletionInfo()
    : args(),
      options()
{ }

_ARGPARSE_INL ArgumentParser::CompletionInfo
ArgumentParser::bash_completion_info(
        ArgumentParser const* parser)
{
    CompletionInfo res;
    pArguments const optional = parser->m_data->get_optional(false, true);
    pArguments const operand = parser->m_data->get_operand(false, true);
    pArguments const positional = parser->m_data->get_positional(false, true);
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
    if (parser->m_subparsers) {
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

_ARGPARSE_INL void
ArgumentParser::print_parser_completion(
        ArgumentParser const* p,
        std::string const& prog,
        bool is_root,
        std::ostream& os)
{
    if (p->m_subparsers) {
        for (std::size_t i = 0; i < p->m_subparsers->m_parsers.size(); ++i) {
            pParser const& parser = p->m_subparsers->m_parsers.at(i);
            print_parser_completion(
                        parser.get(), prog + "_" + parser->m_name, false, os);
        }
    }
    os << "function _" << prog << "()\n";
    os << "{\n";
    if (p->m_subparsers) {
        if (is_root) {
            os << "  for (( i=1; i < ${COMP_CWORD}; ((++i)) )); do\n";
        } else {
            os << "  for (( i=$1; i < ${COMP_CWORD}; ((++i)) )); do\n";
        }
        os << "    case \"${COMP_WORDS[$i]}\" in\n";
        for (std::size_t i = 0; i < p->m_subparsers->m_parsers.size(); ++i) {
            pParser const& parser = p->m_subparsers->m_parsers.at(i);
            for (std::size_t j = 0; j < parser->aliases().size(); ++j) {
                os << "      \"" << parser->aliases().at(j) << "\")\n";
                os << "        ;&\n";
            }
            os << "      \"" << parser->m_name << "\")\n";
            os << "        _" << prog << "_" << parser->m_name << " $((i+1))\n";
            os << "        return\n";
            os << "        ;;\n";
        }
        os << "      *)\n";
        os << "        ;;\n";
        os << "    esac\n";
        os << "  done\n";
    }
    CompletionInfo info = bash_completion_info(p);
    if (!info.options.empty()) {
        os << "  case \"${COMP_WORDS[${COMP_CWORD}-1]}\" in\n";
        for (std::size_t j = 0; j < info.options.size(); ++j) {
            std::pair<pArgument, std::string> const& pr = info.options.at(j);
            for (std::size_t k = 0; k < pr.first->flags().size(); ++k) {
                os << "    \"" << pr.first->flags().at(k) << "\")\n";
                if (k + 1 != pr.first->flags().size()) {
                    os << "      ;&\n";
                } else {
                    if (!pr.second.empty()) {
                        os << "      COMPREPLY=($(compgen" << pr.second
                           << " -- \"${COMP_WORDS[${COMP_CWORD}]}\"))\n";
                    }
                    os << "      return\n";
                    os << "      ;;\n";
                }
            }
        }
        os << "    *)\n";
        os << "      ;;\n";
        os << "  esac\n";
    }
    if (!info.args.empty()) {
        os << "  COMPREPLY=($(compgen" << info.args
           << " -- \"${COMP_WORDS[${COMP_CWORD}]}\"))\n";
    }
    os << "  return\n";
    os << "}\n\n";
}

_ARGPARSE_INL std::string
ArgumentParser::default_language(
        ArgumentParser const* parser)
{
    std::string res;
    if (parser) {
        for (arg_iterator it = parser->m_data->m_arguments.begin();
             it != parser->m_data->m_arguments.end(); ++it) {
            if ((*it)->action() == argparse::language
                    && (*it)->m_default.has_value()) {
                res = (*it)->m_default.value();
            }
        }
    }
    return res;
}

_ARGPARSE_INL
ArgumentParser::ParserInfo::ParserInfo(
        ArgumentParser const* parser,
        _Storage const& storage,
        SubparserInfo const& subparser,
        pArguments const& optional,
        pArguments const& operand)
    : parser(parser),
      optional(optional),
      operand(operand),
      storage(storage),
      subparser(subparser),
      lang(),
      have_negative_args()
{
    lang = default_language(parser);
    have_negative_args
            = negative_numbers_presented(optional, parser->prefix_chars());
}

_ARGPARSE_INL
ArgumentParser::ParserInfo::ParserInfo(
        ParserInfo const& orig)
    : parser(orig.parser),
      optional(orig.optional),
      operand(orig.operand),
      storage(orig.storage),
      subparser(orig.subparser),
      lang(orig.lang),
      have_negative_args(orig.have_negative_args)
{ }

_ARGPARSE_INL ArgumentParser::ParserInfo&
ArgumentParser::ParserInfo::operator =(
        ParserInfo const& rhs)
{
    if (this != &rhs) {
        parser              = rhs.parser;
        optional            = rhs.optional;
        operand             = rhs.operand;
        storage             = rhs.storage;
        subparser           = rhs.subparser;
        lang                = rhs.lang;
        have_negative_args  = rhs.have_negative_args;
    }
    return *this;
}

_ARGPARSE_INL ArgumentParser::ParserInfo
ArgumentParser::parser_info(
        ArgumentParser const* parser,
        _Storage const& storage,
        SubparserInfo const& subparser)
{
    return ParserInfo(parser, storage, subparser,
                      parser->m_data->get_optional(true, true),
                      parser->m_data->get_operand(true, true));
}

_ARGPARSE_INL void
ArgumentParser::throw_error(
        std::string const& message,
        std::string const& lang,
        std::ostream& os) const
{
    print_usage(!lang.empty() ? lang : default_language(this), os);
    throw std::logic_error(prog() + ": error: " + message);
}

_ARGPARSE_INL std::string
ArgumentParser::despecify(
        std::string const& str) const
{
    return detail::_replace(str, "%(prog)s", prog());
}

_ARGPARSE_INL void
ArgumentParser::process_add_argument()
{
    if (m_argument_default.has_value()
            && !m_data->m_arguments.back()->m_default.has_value()
            && !m_data->m_arguments.back()->m_default_type.has_value()) {
        m_data->m_arguments.back()->default_value(m_argument_default.value());
    }
    if (m_argument_default_type.has_value()) {
        m_data->m_arguments.back()->default_value(m_argument_default_type());
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

_ARGPARSE_INL Namespace
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
    ::exit(1);
}

#ifdef _ARGPARSE_CXX_17
_ARGPARSE_INL std::optional<Namespace>
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
#endif  // C++17+

_ARGPARSE_INL std::vector<std::string>
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
                    *it = _ARGPARSE_MOVE(*first);
                }
                ++it;
            }
            ++first;
        }
        res.erase(it, res.end());
    }
    return res;
}

_ARGPARSE_INL Namespace
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
    parsers.push_back(parser_info(this, space.storage(), subparser_info(true)));

    check_mutex_arguments();
    check_intermixed_subparser(intermixed, parsers.back().subparser.first);

    pArguments positional = m_data->get_positional(true, true);
    check_intermixed_remainder(intermixed, positional);

    validate_arguments(m_data->get_arguments(true));
    parsers.back().storage.create(m_data->get_arguments(true));

    std::vector<std::string> unrecognized_args;
    std::deque<std::string> intermixed_args;

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
        std::vector<std::string> equals
              = process_split_equal(arg, parsers.back().parser->prefix_chars());
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
                    process_optional_help(parsers.back(), equals, arg);
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
    return create_namespace(only_known, _ARGPARSE_MOVE(parsers.front().storage),
                            _ARGPARSE_MOVE(unrecognized_args));
}

_ARGPARSE_INL void
ArgumentParser::check_namespace(
        Namespace const& space) const
{
    if (space.m_unrecognized_args.has_value() && !m_data->m_arguments.empty()) {
        throw AttributeError("'tuple' object has no attribute '"
                             + m_data->m_arguments.front()->get_dest() + "'");
    }
}

_ARGPARSE_INL void
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

_ARGPARSE_INL void
ArgumentParser::check_intermixed_subparser(
        bool intermixed,
        pSubparser const& subparser)
{
    if (intermixed && subparser) {
        throw
        TypeError("parse_intermixed_args: positional arg with nargs=A...");
    }
}

_ARGPARSE_INL void
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

_ARGPARSE_INL std::vector<std::string>
ArgumentParser::process_split_equal(
        std::string& arg,
        std::string const& prefix_chars) const
{
    std::vector<std::string> equals = detail::_split_equal(arg, prefix_chars);
    if (equals.size() == 2 && !equals.front().empty()) {
        arg = equals.front();
    } else {
        equals.resize(1);
    }
    return equals;
}

_ARGPARSE_INL Namespace
ArgumentParser::create_namespace(
        bool only_known,
        detail::rval<_Storage>::type storage,
        detail::rval<std::vector<std::string> >::type unrecognized_args)
{
    if (only_known) {
        return Namespace(_ARGPARSE_MOVE(storage),
                         _ARGPARSE_MOVE(unrecognized_args));
    } else {
        return Namespace(_ARGPARSE_MOVE(storage));
    }
}

_ARGPARSE_INL bool
ArgumentParser::negative_numbers_presented(
        pArguments const& optionals,
        std::string const& prefix_chars)
{
    if (detail::_exists(detail::_prefix_char, prefix_chars)) {
        for (std::size_t i = 0; i < optionals.size(); ++i) {
            pArgument const& arg = optionals.at(i);
            for (std::size_t j = 0; j < arg->flags().size(); ++j) {
                if (detail::_is_negative_number(arg->flags().at(j))) {
                    return true;
                }
            }
        }
    }
    return false;
}

_ARGPARSE_INL void
ArgumentParser::validate_arguments(
        pArguments const& args)
{
    for (std::size_t i = 0; i < args.size(); ++i) {
        args.at(i)->validate();
    }
}

_ARGPARSE_INL void
ArgumentParser::validate_argument_value(
        ArgumentParser const* parser,
        Argument const& arg,
        std::string const& value) const
{
    detail::Value<std::vector<std::string> > const& choices = arg.m_choices;
    if (!(arg.m_nargs & (Argument::REMAINDING | Argument::SUPPRESSING))
            && choices.has_value()) {
        std::string str = detail::_remove_quotes<std::string>(value);
        if (!str.empty() && !detail::_exists(str, choices.value())) {
            parser->throw_error(
                        "argument " + (arg.m_flags.empty()
                                       ? arg.dest() : arg.m_flags.front())
                        + ": invalid choice: '" + str + "' (choose from "
                        + detail::_join(choices.value(), ", ", "'") + ")");
        }
    }
}

_ARGPARSE_INL void
ArgumentParser::storage_have_value(
        Parsers& parsers,
        pArgument const& arg) const
{
    validate_argument_value(parsers.back().parser, *arg, std::string());
    parsers.front().storage.have_value(arg);
}

_ARGPARSE_INL void
ArgumentParser::storage_store_value(
        Parsers& parsers,
        pArgument const& arg,
        std::string const& val) const
{
    validate_argument_value(parsers.back().parser, *arg, val);
    parsers.front().storage.store_value(arg, val);
}

_ARGPARSE_INL void
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

_ARGPARSE_INL void
ArgumentParser::storage_store_n_values(
        Parsers& parsers,
        pArgument const& arg,
        std::deque<std::string>& arguments,
        std::size_t n) const
{
    std::vector<std::string> values;
    values.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        values.push_back(arguments.front());
        arguments.pop_front();
    }
    storage_store_values(parsers, arg, values);
}

_ARGPARSE_INL void
ArgumentParser::storage_store_default_value(
        Parsers& parsers,
        pArgument const& arg) const
{
    parsers.front().storage.store_default_value(arg);
}

_ARGPARSE_INL bool
ArgumentParser::storage_is_positional_arg_stored(
        Parsers& parsers,
        pArgument const& arg) const
{
    return parsers.front().storage.self_value_stored(arg);
}

_ARGPARSE_INL void
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
    } else if (tmp->m_nargs == Argument::NARGS_NUM && n < tmp->m_num_args) {
        parsers.back().parser->throw_error(tmp->error_nargs(arg));
    }
}

_ARGPARSE_INL void
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
        do {
            if (++i == args.size()) {
                storage_optional_store_func(parsers, arg, tmp, n);
                break;
            } else {
                std::string const& next = args.at(i);
                if (tmp->m_nargs != Argument::SUPPRESSING
                        && (tmp->m_nargs == Argument::REMAINDING
                            || (is_not_operand(
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
        } while (!(tmp->m_nargs & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE))
                 && (tmp->m_nargs != Argument::NARGS_NUM
                     || n != tmp->m_num_args));
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

_ARGPARSE_INL void
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

_ARGPARSE_INL void
ArgumentParser::process_optional_help(
        ParserInfo const& info,
        std::vector<std::string> const& equals,
        std::string const& arg) const
{
    if (equals.size() != 1) {
        info.parser->throw_error(
                    detail::_ignore_explicit(arg, equals.back()), info.lang);
    }
    // print help and exit
    info.parser->print_help(info.lang);
    ::exit(0);
}

_ARGPARSE_INL void
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
    std::cout << despecify(tmp->version()) << std::endl;
    ::exit(0);
}

_ARGPARSE_INL void
ArgumentParser::match_positional_minimum(
        Parsers& parsers,
        std::deque<std::string>& arguments,
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
        case Argument::ONE_OR_MORE :
            storage_store_value(parsers, arg, arguments.front());
            arguments.pop_front();
            break;
        case Argument::NARGS_NUM :
            storage_store_n_values(parsers, arg, arguments, arg->m_num_args);
            break;
        case Argument::ZERO_OR_ONE :
        case Argument::ZERO_OR_MORE :
            storage_store_default_value(parsers, arg);
            break;
        case Argument::REMAINDING :
        default :
            break;
    }
}

_ARGPARSE_INL void
ArgumentParser::match_positional_more_zero(
        Parsers& parsers,
        std::deque<std::string>& arguments,
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
            storage_store_n_values(parsers, arg, arguments, 1 + over_args);
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
        default :
            break;
    }
}

_ARGPARSE_INL void
ArgumentParser::match_positional_optional(
        Parsers& parsers,
        std::deque<std::string>& arguments,
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
            if (over_args < one_args) {
                storage_store_value(parsers, arg, arguments.front());
                arguments.pop_front();
                ++over_args;
            } else {
                storage_store_default_value(parsers, arg);
            }
            break;
        case Argument::ONE_OR_MORE :
        case Argument::ZERO_OR_MORE :
        case Argument::REMAINDING :
        default :
            break;
    }
}

_ARGPARSE_INL void
ArgumentParser::match_positional_default(
        Parsers& parsers,
        std::deque<std::string>& arguments,
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

_ARGPARSE_INL void
ArgumentParser::match_positionals(
        Parsers& parsers,
        std::size_t& pos,
        pArguments const& positional,
        std::deque<std::string>& arguments,
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

_ARGPARSE_INL bool
ArgumentParser::finish_analyze_positional(
        pArgument const& arg,
        std::deque<std::string> const& args,
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
            ++one_args;
            break;
        case Argument::ONE_OR_MORE :
            ++min_amount;
            // fallthrough
        case Argument::ZERO_OR_MORE :
            more_args = true;
            break;
        case Argument::REMAINDING :
            more_args = true;
            break;
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

_ARGPARSE_INL void
ArgumentParser::match_args_partial(
        Parsers& parsers,
        std::size_t& pos,
        pArguments const& positional,
        std::vector<std::string>& unrecognized_args,
        std::deque<std::string>& args,
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

_ARGPARSE_INL bool
ArgumentParser::try_capture_parser(
        Parsers& parsers,
        std::size_t& pos,
        pArguments& positional,
        std::vector<std::string>& unrecognized_args,
        std::deque<std::string>& args,
        bool read_all_args) const
{
    std::size_t finish = pos;
    std::size_t min_args = 0;
    std::size_t one_args = 0;
    bool more_args = false;
    bool capture_need = false;
    for ( ; finish < positional.size(); ++finish) {
        if (finish == parsers.back().subparser.second) {
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
    std::string const& dest = parsers.back().subparser.first->dest();
    std::string choices;
    for (std::size_t i = 0;
         i < parsers.back().subparser.first->m_parsers.size(); ++i) {
        pParser const& p = parsers.back().subparser.first->m_parsers.at(i);
        detail::_append_value_to("'" + p->m_name + "'", choices, ", ");
        for (std::size_t j = 0; j < p->aliases().size(); ++j) {
            std::string const& alias = p->aliases().at(j);
            detail::_append_value_to("'" + alias + "'", choices, ", ");
        }
        if (p->m_name == name || detail::_exists(name, p->aliases())) {
            std::string lang = parsers.back().lang;
            parsers.push_back(parser_info(p.get(), _Storage(),
                                          p->subparser_info(true, pos)));
            if (!lang.empty()) {
                parsers.back().lang = lang;
            }
            parsers.back().parser->handle(parsers.back().parser->m_name);
            validate_arguments(p.get()->m_data->get_arguments(true));
            if (!dest.empty()) {
                pArgument subparser_arg
                        = Argument::make_argument(detail::_vector(dest),
                                                  dest, Argument::Positional);
                for (std::size_t j = 0; j < parsers.size(); ++j) {
                    parsers.at(j).storage.create(subparser_arg);
                }
                parsers.front().storage.at(subparser_arg).push_back(name);
            }
            for (std::size_t j = 0; j < parsers.size(); ++j) {
                parsers.at(j).storage.create(
                            parsers.back().parser->m_data->get_arguments(true));
            }
            pArguments sub_positional
                    = parsers.back().parser->m_data->get_positional(true, true);
            detail::_move_insert_to(sub_positional, positional, pos);
            args.pop_front();
            return true;
        }
    }
    throw_error("argument " + parsers.back().subparser.first->flags_to_string()
            + ": invalid choice: '" + name + "' (choose from " + choices + ")");
    return false;
}

_ARGPARSE_INL void
ArgumentParser::check_abbreviations(
        Parsers const& parsers,
        bool was_pseudo_arg,
        std::vector<std::string>& arguments,
        std::size_t i) const
{
    std::string& arg = arguments.at(i);
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

_ARGPARSE_INL bool
ArgumentParser::is_remainder_positional(
        std::size_t pos,
        pArguments const& positional,
        Parsers const& parsers)
{
    return pos < positional.size()
            && positional.at(pos)->m_nargs == Argument::REMAINDING
            && !parsers.front().storage.at(positional.at(pos)).empty();
}

_ARGPARSE_INL ArgumentParser::pArgument const
ArgumentParser::find_arg_by_flag(
        pArguments const& args,
        std::string const& key)
{
    pArguments::const_iterator it = args.begin();
    for ( ; it != args.end() && !detail::_exists(key, (*it)->flags()); ++it) {
    }
    return it != args.end() ? *it : _ARGPARSE_NULLPTR;
}

_ARGPARSE_INL ArgumentParser::pArgument const
ArgumentParser::get_optional_arg_by_flag(
        bool was_pseudo_arg,
        ParserInfo const& info,
        std::string const& key)
{
    if (was_pseudo_arg) {
        return _ARGPARSE_NULLPTR;
    }
    pArgument const opt = find_arg_by_flag(info.optional, key);
    return opt ? opt : find_arg_by_flag(info.operand, key);
}

_ARGPARSE_INL bool
ArgumentParser::is_not_operand(
        bool was_pseudo_arg,
        pArguments const& args,
        std::string const& key)
{
    return was_pseudo_arg || !find_arg_by_flag(
                args, detail::_split(key, detail::_equals, 1).front());
}

_ARGPARSE_INL bool
ArgumentParser::process_separate_arg_abbrev(
        std::string const& name,
        std::size_t i,
        std::size_t cp_size,
        std::vector<std::string>& flags,
        std::string const& arg,
        pArguments const& args)
{
    std::string abbrev = name.substr(i, cp_size);
    if (abbrev == detail::_equals) {
        if (flags.empty()) {
            flags.push_back(std::string());
        }
        flags.back() += name.substr(i);
        return false;
    }
    Argument const* argument = _ARGPARSE_NULLPTR;
    for (std::size_t j = 0; j < args.size() && !argument; ++j) {
        for (std::size_t k = 0; k < args.at(j)->flags().size(); ++k) {
            std::string const& flag = args.at(j)->flags().at(k);
            if (detail::_utf8_length(flag).second == 2
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
               || (argument && (argument->action() & detail::_store_action))) {
        std::string str
             = name.substr(i + (static_cast<bool>(argument) ? cp_size : 0));
        if (!str.empty()) {
            flags.back() += detail::_equals;
            flags.back() += str;
        }
        return false;
    }
    return true;
}

_ARGPARSE_INL void
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
              && find_arg_by_flag(info.optional, split.front())) {
            temp.push_back(arg);
            return;
        }
        std::vector<std::string> flags;
        flags.reserve(8);
        for (std::size_t i = 0; i < name.size(); ) {
            std::size_t cp_size
                   = detail::_utf8_codepoint_size(detail::_char_to_u8(name[i]));
            if (cp_size == 0) {
                // invalid string
                cp_size = 1;
            }
            if (!process_separate_arg_abbrev(
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

_ARGPARSE_INL void
ArgumentParser::process_positional_args(
        std::vector<std::string> const& parsed_arguments,
        std::size_t& i,
        Parsers& parsers,
        bool was_pseudo_arg,
        bool intermixed,
        std::deque<std::string>& intermixed_args,
        std::size_t& pos,
        pArguments& positional,
        std::vector<std::string>& unrecognized_args) const
{
    std::deque<std::string> args;
    args.push_back(parsed_arguments.at(i));
    bool remainder = pos < positional.size()
            && positional.at(pos)->m_nargs == Argument::REMAINDING;
    while (true) {
        if (++i == parsed_arguments.size()) {
            break;
        } else {
            std::string const& next = parsed_arguments.at(i);
            if (remainder || (is_not_operand(
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
        if (parsers.back().subparser.first) {
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

_ARGPARSE_INL void
ArgumentParser::check_mutex_groups(
        Parsers const& parsers)
{
    for (std::size_t i = 0; i < parsers.size(); ++i) {
        ArgumentParser const* parser = parsers.at(i).parser;
        for (mtx_it j = parser->m_mutex_groups.begin();
             j != parser->m_mutex_groups.end(); ++j) {
            std::string args;
            std::string found;
            for (arg_iterator k = (*j).m_data->m_arguments.begin();
                 k != (*j).m_data->m_arguments.end(); ++k) {
                std::string flags = detail::_join((*k)->flags(), "/");
                args += detail::_spaces + flags;
                if (!parsers.front().storage.at(*k).empty()) {
                    if (!found.empty()) {
                        parser->throw_error("argument " + flags + ": not "
                                            + "allowed with argument " + found);
                    }
                    found = flags;
                }
            }
            if ((*j).required() && found.empty()) {
                if ((*j).m_data->m_arguments.empty()) {
                    throw IndexError("list index out of range");
                }
                parser->throw_error(
                            "one of the arguments" + args + " is required");
            }
        }
    }
}

_ARGPARSE_INL void
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

_ARGPARSE_INL void
ArgumentParser::process_required_subparser(
        bool required,
        std::size_t pos,
        SubparserInfo const& info,
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

_ARGPARSE_INL void
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

_ARGPARSE_INL bool
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

_ARGPARSE_INL void
ArgumentParser::check_required_args(
        Parsers& parsers,
        std::size_t& pos,
        pArguments const& positional) const
{
    std::deque<ParserInfo>::reverse_iterator it = parsers.rbegin();
    std::vector<std::string> required;
    process_required_arguments(required, it->optional, parsers.front().storage);
    process_required_arguments(required, it->operand, parsers.front().storage);
    SubparserInfo const& info = it->subparser;
    bool sub_required = info.first && info.first->required();
    if (!required.empty() || pos < positional.size() || sub_required) {
        std::string args;
        for ( ; pos < positional.size(); ++pos) {
            process_required_subparser(sub_required, pos, info, args);
            pArgument const& arg = positional.at(pos);
            if (args.empty() && skip_positional_required_check(parsers, arg)) {
                continue;
            }
            detail::_append_value_to(arg->m_flags.front(), args, ", ");
        }
        process_required_subparser(sub_required, pos, info, args);
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

_ARGPARSE_INL void
ArgumentParser::check_unrecognized_args(
        bool only_known,
        std::vector<std::string> const& args) const
{
    if (!only_known && !args.empty()) {
        throw_error("unrecognized arguments: " + detail::_join(args));
    }
}

_ARGPARSE_INL void
ArgumentParser::default_values_post_trigger(
        _Storage& storage) const
{
    for (_Storage::iterator it = storage.begin(); it != storage.end(); ) {
        if (!it->second.exists()) {
            if (it->first->is_suppressed()) {
                it = storage.erase(it);
                continue;
            }
            if (it->first->action() != argparse::count
                    && (it->first->m_type == Argument::Optional
                        || it->first->m_type == Argument::Operand)) {
                detail::Value<std::string> const& dv = it->first->m_default;
                if (dv.has_value()) {
                    it->second.push_default(dv.value());
                    storage.on_process_store(it->first, dv.value());
                } else if (it->first->action() & detail::_bool_action) {
                    it->second.push_back(dv.value());
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

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL void
ArgumentParser::namespace_post_trigger(
        Parsers& parsers,
        bool only_known,
        std::vector<std::string> const& unrecognized_args)
{
    parsers.front().parser->parse_handle(
                only_known, parsers.front().storage, unrecognized_args);
    for (std::size_t i = 1; i < parsers.size(); ++i) {
        auto const& storage = parsers.at(i - 1).storage;
        auto& sub_storage = parsers.at(i).storage;
        for (auto it = sub_storage.begin(); it != sub_storage.end(); ) {
            if (storage.exists(it->first)) {
                it->second = storage.at(it->first);
                ++it;
            } else {
                it = sub_storage.erase(it);
            }
        }
        parsers.at(i).parser->parse_handle(
                    only_known, sub_storage, unrecognized_args);
    }
}
#else
_ARGPARSE_INL void
ArgumentParser::namespace_post_trigger(
        Parsers&, bool, std::vector<std::string> const&) { /* stub */ }
#endif  // C++11+

_ARGPARSE_INL bool
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

_ARGPARSE_INL std::size_t
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

_ARGPARSE_INL ArgumentParser::SubparserInfo
ArgumentParser::subparser_info(
        bool add_suppress,
        std::size_t offset) const
{
    SubparserInfo res = std::make_pair(m_subparsers, offset);
    std::size_t i = 0;
    for (sub_iterator it = m_data->m_positional.begin();
         it != m_data->m_positional.end() && i < m_subparsers_position;
         ++it, ++i) {
        res.second += (add_suppress || !(*it).first->m_help_type.has_value());
    }
    return res;
}

_ARGPARSE_INL std::string
ArgumentParser::subparser_prog_args() const
{
    std::string res;
    bool add_suppress = false;
    SubparserInfo info = subparser_info(add_suppress);
    pArguments args = m_data->get_positional(add_suppress, true);
    for (std::size_t i = 0; i < args.size() && i != info.second; ++i) {
        detail::_append_value_to(args.at(i)->usage(*m_formatter), res);
    }
    return res;
}

_ARGPARSE_INL void
ArgumentParser::add_arg_usage(
        std::string& res,
        std::string const& str,
        bool required) const
{
    detail::_append_value_to(required ? str : "[" + str + "]", res, "\n");
}

_ARGPARSE_INL void
ArgumentParser::print_custom_usage(
        pArguments const& positional,
        pArguments const& operand,
        pArguments const& options,
        std::list<MutuallyExclusiveGroup> const& mutex_groups,
        SubparserInfo const& info,
        std::string const& prog,
        std::string const& usage_title,
        std::ostream& os) const
{
    std::size_t const w = output_width();
    std::string head_prog = usage_title + " " + prog;
    std::size_t indent = 1 + detail::_utf8_length(
                w > detail::_min_width ? head_prog : usage_title).second;
    std::string res;
    pArguments ex_options = options;
    pArguments ex_operand = operand;
    for (mtx_it i = mutex_groups.begin(); i != mutex_groups.end(); ++i) {
        for (arg_iterator j = (*i).m_data->m_arguments.begin();
             j != (*i).m_data->m_arguments.end(); ++j) {
            ex_options.erase(std::remove(
                                 ex_options.begin(), ex_options.end(), *j),
                             ex_options.end());
            ex_operand.erase(std::remove(
                                 ex_operand.begin(), ex_operand.end(), *j),
                             ex_operand.end());
        }
    }
    for (std::size_t i = 0; i < ex_options.size(); ++i) {
        add_arg_usage(res, ex_options.at(i)->usage(*m_formatter),
                      ex_options.at(i)->required());
    }
    for (std::size_t i = 0; i < ex_operand.size(); ++i) {
        add_arg_usage(res, ex_operand.at(i)->usage(*m_formatter),
                      ex_operand.at(i)->required());
    }
    for (mtx_it i = mutex_groups.begin(); i != mutex_groups.end(); ++i) {
        add_arg_usage(res, (*i).usage(*m_formatter), true);
    }
    for (std::size_t i = 0; i < positional.size(); ++i) {
        if (info.first && info.second == i
                && !info.first->m_help_type.has_value()) {
            add_arg_usage(res, info.first->usage(), true);
        }
        std::string const str = positional.at(i)->usage(*m_formatter);
        if (str.empty()) {
            continue;
        }
        add_arg_usage(res, str, true);
    }
    if (info.first && info.second == positional.size()
            && !info.first->m_help_type.has_value()) {
        add_arg_usage(res, info.first->usage(), true);
    }
    os << detail::_format_output(head_prog, res, 1, indent, w) << std::endl;
}

_ARGPARSE_INL bool
ArgumentParser::is_subparser_positional(
        pSubparser const& sub)
{
    return sub && sub->title().empty() && sub->description().empty()
            && !sub->m_help_type.has_value();
}

_ARGPARSE_INL void
ArgumentParser::print_subparser(
        bool need_print,
        SubparserInfo const& info,
        std::size_t index,
        HelpFormatter const& formatter,
        std::size_t size,
        std::size_t width,
        std::string const& lang,
        std::ostream& os)
{
    if (need_print && info.second == index) {
        os << info.first->print(formatter, size, width, lang) << std::endl;
    }
}

_ARGPARSE_INL void
ArgumentParser::update_prog(
        std::string const& parent_prog)
{
    m_prog = parent_prog + detail::_spaces + m_name;
}

_ARGPARSE_INL void
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
    std::string tr_usage = detail::_tr(m_usage, lang);
    if (!tr_usage.empty()) {
        os << "usage: " << despecify(tr_usage) << "\n";
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
    if (m_output_width.has_value()) {
        os << "output_width [override]: " << output_width() << "\n";
    } else {
#ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
        os << "output_width [detected]: " << output_width() << "\n";
#else
        os << "output_width [default]: " << output_width() << "\n";
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
    }
    if (m_subparsers) {
        std::deque<pParser> const& parsers = m_subparsers->m_parsers;
        os << "subparsers list:\n";
        for (std::size_t i = 0; i < parsers.size(); ++i) {
            pParser const& parser = parsers.at(i);
            os << "  " << (i + 1) << ". '" << parser->m_name << "'";
            std::string aliases = detail::_join(parser->aliases(), ", ", "'");
            if (!aliases.empty()) {
                os << ", aliases: " << aliases;
            }
            os << "\n";
        }
    }
}

_ARGPARSE_INL bool
ArgumentParser::test_diagnostics(
        std::string const& lang,
        std::ostream& os) const
{
    std::string const _ok    = "[ OK ]";
    std::string const _info  = "[INFO]";
    std::string const _warn  = "[WARN]";
    std::string const _error = "[FAIL]";
    typedef std::pair<std::size_t, std::size_t> WarnErrAmount;
    WarnErrAmount diagnostics;
    // check prog
    if (prog() == "untitled") {
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
    std::map<std::string, std::size_t> dest_args;
    // check arguments
    for (arg_iterator i = m_data->m_arguments.begin();
         i != m_data->m_arguments.end(); ++i) {
        pArgument const& arg = *i;
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
                if (dest_args.count(flag) != 0) {
                    if (conflict_handler() == "resolve") {
                        ++diagnostics.first;
                        os << _warn << " " << argument << " resolve"
                           << ": conflicting option string: '" << flag << "'\n";
                    } else {
                        ++diagnostics.second;
                        os << _error << " " << argument
                           << ": conflicting option string: '" << flag << "'\n";
                    }
                }
                ++dest_args[flag];
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
        if (detail::_tr(arg->m_help, lang).empty()
                && !arg->m_help_type.has_value()) {
            ++diagnostics.first;
            os << _warn << " " << argument << ": help is not set\n";
        }
        // check metavar
        if ((arg->action() & (detail::_store_action
                              | argparse::append_const
                              | argparse::language))) {
            std::size_t size = arg->get_argument_name(*m_formatter).size();
            if (size > 1 && (arg->m_nargs != Argument::NARGS_NUM
                             || size != arg->m_num_args)) {
                ++diagnostics.second;
                os << _error << " " << argument << ": length of "
                   << "metavar tuple does not match nargs\n";
            }
        }
    }
    // check mutually exclusive arguments
    for (mtx_it i = m_mutex_groups.begin(); i != m_mutex_groups.end(); ++i) {
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
    if (m_subparsers) {
        std::deque<pParser> const& parsers = m_subparsers->m_parsers;
        if (parsers.empty()) {
            ++diagnostics.first;
            os << _warn << " subparsers created but no parsers were added\n";
        }
        // check dest
        if (m_subparsers->dest().empty()) {
            os << _info << " you can specify `dest` for subparsers "
               << "to determine used parser\n";
        } else {
            if (!detail::_is_utf8_string(m_subparsers->dest())) {
                ++diagnostics.first;
                os << _warn << " subparsers dest '"
                   << m_subparsers->dest() << "' is not utf-8\n";
            }
            if (!detail::_is_flag_correct(m_subparsers->dest(), false)) {
                ++diagnostics.first;
                os << _warn << " subparsers dest '"
                   << m_subparsers->dest() << "' can be incorrect\n";
            }
            std::string const& flag = m_subparsers->dest();
            if (dest_args.count(flag) != 0) {
                if (conflict_handler() == "resolve") {
                    ++diagnostics.first;
                    os << _warn << " subparsers dest '" << flag << " resolve"
                       << "': conflicting option string: '" << flag << "'\n";
                } else {
                    ++diagnostics.second;
                    os << _error << " subparsers dest '" << flag
                       << "': conflicting option string: '" << flag << "'\n";
                }
            }
            ++dest_args[flag];
        }
        // check help
        if (detail::_tr(m_subparsers->m_help, lang).empty()
                && !m_subparsers->m_help_type.has_value()) {
            ++diagnostics.first;
            os << _warn << " help for subparsers is not set\n";
        }
        for (std::size_t i = 0; i < parsers.size(); ++i) {
            pParser const& parser = parsers.at(i);
            std::string const& name = parser->m_name;
            // check name
            if (!detail::_is_utf8_string(name)) {
                ++diagnostics.first;
                os << _warn << " name for parser '"
                   << name << "' is not utf-8\n";
            }
            if (!name.empty() && detail::_exists(name.at(0), m_prefix_chars)) {
                ++diagnostics.first;
                os << _warn << " name for parser '" << name << "' incorrect, "
                   << "started with prefix chars '" << m_prefix_chars << "'\n";
            } else if (!detail::_is_flag_correct(name, false)) {
                ++diagnostics.first;
                os << _warn << " name for parser '"
                   << name << "' can be incorrect\n";
            }
            // check help
            if (detail::_tr(parser->m_help, lang).empty()) {
                ++diagnostics.first;
                os << _warn << " help for parser '" << name << "' is not set\n";
            }
        }
    }
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

#ifdef _ARGPARSE_CXX_11
_ARGPARSE_INL void
ArgumentParser::handle(
        std::string const& str) const
{
    if (m_handle) {
        m_handle(detail::_remove_quotes<std::string>(str));
    }
}

_ARGPARSE_INL void
ArgumentParser::parse_handle(
        bool only_known,
        _Storage const& storage,
        std::vector<std::string> const& unrecognized_args) const
{
    if (m_parse_handle) {
        if (only_known) {
            m_parse_handle(Namespace(storage, unrecognized_args));
        } else {
            m_parse_handle(Namespace(storage));
        }
    }
}
#else
_ARGPARSE_INL void
ArgumentParser::handle(std::string const&) const { /* stub */ }
#endif  // C++11+
#endif  // _ARGPARSE_INL
}  // namespace argparse

// -- #undef ------------------------------------------------------------------
#undef _ARGPARSE_INL
// -- standard ----------------------------------------------------------------
#undef _ARGPARSE_CXX_98
#undef _ARGPARSE_CXX_11
#undef _ARGPARSE_CXX_14
#undef _ARGPARSE_CXX_17
#undef _ARGPARSE_CXX_20
#undef _ARGPARSE_CXX_23
// -- attributes --------------------------------------------------------------
// C++11+
#undef _ARGPARSE_ATTR_NORETURN
#undef _ARGPARSE_ATTR_CARRIES_DEPENDENCY
// C++14+
#undef _ARGPARSE_ATTR_DEPRECATED
#undef _ARGPARSE_ATTR_DEPRECATED_REASON
// C++17+
#undef _ARGPARSE_ATTR_FALLTHROUGH
#undef _ARGPARSE_ATTR_MAYBE_UNUSED
#undef _ARGPARSE_ATTR_NODISCARD
// C++20+
#undef _ARGPARSE_ATTR_LIKELY
#undef _ARGPARSE_ATTR_NODISCARD_REASON
#undef _ARGPARSE_ATTR_NO_UNIQUE_ADDRESS
#undef _ARGPARSE_ATTR_UNLIKELY
// C++23+
#undef _ARGPARSE_ATTR_ASSUME
// -- specifiers --------------------------------------------------------------
#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_ENUM_TYPE
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_FINAL
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_MOVE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_NULLPTR
#undef _ARGPARSE_OVERRIDE
#undef _ARGPARSE_USE_CONSTEXPR

#endif  // _ARGPARSE_ARGUMENT_PARSER_HPP_
