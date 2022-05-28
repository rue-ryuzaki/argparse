/*
* MIT License
*
* Argument parser for C++11 (ArgumentParser v1.6.1)
*
* Copyright (c) 2021-2022 Golubchikov Mihail <https://github.com/rue-ryuzaki>
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

#define _ARGPARSE_VERSION_MAJOR 1
#define _ARGPARSE_VERSION_MINOR 6
#define _ARGPARSE_VERSION_PATCH 1

#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_EXPERIMENTAL_OPTIONAL
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_USE_CONSTEXPR
#undef _ARGPARSE_USE_FILESYSTEM
#undef _ARGPARSE_USE_OPTIONAL

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <initializer_list>
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
#if __cplusplus >= 201703L  // C++17+
#include <string_view>
#endif  // C++17+
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifndef ARGPARSE_NO_AUTODETECT
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

#include <Windows.h>

#ifdef _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif  // _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN

#ifdef _ARGPARSE_DEFINE_VC_EXTRALEAN
#undef VC_EXTRALEAN
#endif  // _ARGPARSE_DEFINE_VC_EXTRALEAN

#undef _ARGPARSE_DEFINE_WIN32_LEAN_AND_MEAN
#undef _ARGPARSE_DEFINE_VC_EXTRALEAN
#else  // UNIX
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#endif  // _WIN32
#endif  // ARGPARSE_NO_AUTODETECT

// filesystem
#if __cplusplus >= 201703L  // C++17+
#if (defined(_MSC_VER) && _MSC_VER >= 1914) \
    || (defined(__clang__) && (__clang_major__ > 8)) \
    || (defined(__GNUC__) && (__GNUC__ > 8))
#include <filesystem>

#define _ARGPARSE_USE_FILESYSTEM 1
#endif  //
#endif  // C++17+

// optional
#if __cplusplus >= 201703L  // C++17+
#include <optional>

#define _ARGPARSE_USE_OPTIONAL 1
#elif __cplusplus >= 201402L  // C++14
#if defined(__GNUC__) && (defined(__linux__) || !defined(__clang__))
#include <experimental/optional>
namespace std {
using experimental::optional;
using experimental::fundamentals_v1::nullopt;
}  // namespace std

#define _ARGPARSE_EXPERIMENTAL_OPTIONAL 1
#define _ARGPARSE_USE_OPTIONAL 1
#endif  // __GNUC__
#endif  // C++14+

#define _ARGPARSE_EXPORT

#if __cplusplus >= 201103L  // C++11+
#define _ARGPARSE_CONSTEXPR constexpr
#define _ARGPARSE_USE_CONSTEXPR constexpr
#else
#define _ARGPARSE_CONSTEXPR
#define _ARGPARSE_USE_CONSTEXPR const
#endif  // C++11+

#if __cplusplus >= 201703L  // C++17+
#define _ARGPARSE_INLINE_VARIABLE inline
#else
#define _ARGPARSE_INLINE_VARIABLE static
#endif  // C++17+

#if __cplusplus >= 201103L  // C++11+
#define _ARGPARSE_NOEXCEPT noexcept
#else
#define _ARGPARSE_NOEXCEPT
#endif  // C++11+

namespace argparse {
/*!
 *  \brief Action values
 *
 *  \enum Action
 */
_ARGPARSE_EXPORT enum Action : std::uint16_t
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
_ARGPARSE_EXPORT class IndexError : public std::logic_error
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
        : std::logic_error("IndexError: " + error)
    { }
};

/*!
 *  \brief TypeError handler
 */
_ARGPARSE_EXPORT class TypeError : public std::logic_error
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
        : std::logic_error("TypeError: " + error)
    { }
};

//  Forward declaration
class Argument;

/*!
 *  \brief _HelpFormatter class
 */
struct _HelpFormatter
{
    std::string (*_fill_text)(std::string const& text, std::size_t width,
                              std::string const& indent);
    std::string (*_get_default_metavar_for_optional)(Argument const* action);
    std::string (*_get_default_metavar_for_positional)(Argument const* action);
    std::string (*_get_help_string)(Argument const* action);
    std::vector<std::string> (*_split_lines)(std::string const& text,
                                             std::size_t width);
};

namespace detail {
std::size_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _min_width = 33;
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _prefix_char     = '-';
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _prefix_chars[]  = "-";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _pseudo_arg[]    = "--";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _space           = ' ';
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _equal           = '=';
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _spaces[]        = " ";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _equals[]        = "=";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
                                                   _suppress[] = "==SUPPRESS==";

std::uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_bool_action = Action::store_true | Action::store_false;
std::uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_store_action = Action::store | Action::append | Action::extend;
std::uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_const_action = Action::store_const | Action::append_const;
std::uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_store_const_action = _store_action | _const_action;

template <class T> struct is_byte_type              { enum { value = false }; };
template <>        struct is_byte_type<char>         { enum { value = true }; };
template <>        struct is_byte_type<signed char>  { enum { value = true }; };
template <>        struct is_byte_type<unsigned char>{ enum { value = true }; };
#if __cplusplus >= 201703L  // C++17+
template <>        struct is_byte_type<std::byte>    { enum { value = true }; };
#endif  // C++17+
#if __cplusplus >= 202002L  // C++20+
template <>        struct is_byte_type<char8_t>      { enum { value = true }; };
#endif  // C++20+

template <class T>
struct is_stl_array:std::false_type{};
template <class T, std::size_t N>
struct is_stl_array<std::array                     <T, N> >   :std::true_type{};

template <class T>
struct is_stl_container:std::false_type{};
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
struct is_stl_container_paired:std::false_type{};

template <class _1st, class _2nd, template <class...> class container>
struct is_stl_container_paired<container<std::pair<_1st, _2nd> > >
                                                              :std::true_type{};

template <class T, typename U = void>
struct is_stl_container_tupled:std::false_type{};

template <class... Args, template <class...> class container>
struct is_stl_container_tupled<container<std::tuple<Args...> > >
                                                              :std::true_type{};

template <class...>
struct voider { using type = void; };
template <class... T>
using void_t = typename voider<T...>::type;
template <class T, typename U = void>
struct is_stl_map:std::false_type{};
template <class T>
struct is_stl_map<T, void_t<typename T::key_type,
                                    typename T::mapped_type> >:std::true_type{};

template <class T>
struct is_stl_matrix:std::false_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::deque       <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::forward_list<Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::list        <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::multiset    <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::priority_queue<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::set         <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::vector      <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::unordered_multiset<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::deque<std::unordered_set<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::deque        <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::forward_list<Args...> > > :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::list         <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::multiset     <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::priority_queue<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::set          <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::vector       <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::unordered_multiset<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::list<std::unordered_set<Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::deque      <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::forward_list<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::list       <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::multiset<Args...> > >   :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::priority_queue<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::set        <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::vector     <Args...> > >:std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::unordered_multiset<Args...> > >
                                                              :std::true_type{};
template <class... Args>
struct is_stl_matrix<std::vector<std::unordered_set<Args...> > >
                                                              :std::true_type{};

template <class T>
struct is_stl_matrix_queue:std::false_type{};
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
struct is_stl_pair:std::false_type{};
template <class T>
struct is_stl_pair<T, void_t<typename T::first_type,
                                    typename T::second_type> >:std::true_type{};

template <class T>
struct is_stl_queue:std::false_type{};
template <class... Args>
struct is_stl_queue<std::stack                     <Args...> >:std::true_type{};
template <class... Args>
struct is_stl_queue<std::queue                     <Args...> >:std::true_type{};

template <class T>
struct is_stl_tuple:std::false_type{};
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
    struct tmp<integer_sequence<T, Prev...>>
    { using type = integer_sequence<T, Prev..., N - 1>; };

    using type
      = typename tmp<typename make_integer_sequence_impl<T, N - 1>::type>::type;
};

template <class T, T N>
struct make_integer_sequence_impl<T, N, typename std::enable_if<N == 0>::type>
{ using type = integer_sequence<T>; };

template <class T, T N>
using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

inline std::pair<std::size_t, std::size_t>
_get_terminal_size(bool default_values = false)
{
    std::size_t width  = 80;
    std::size_t height = 24;
    if (default_values) {
        return std::make_pair(width, height);
    }
#ifndef ARGPARSE_NO_AUTODETECT
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = static_cast<std::size_t>
                (csbi.srWindow.Right - csbi.srWindow.Left + 1);
        height = static_cast<std::size_t>
                (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
        if (width < _min_width) {
            width = _min_width;
        }
    }
#else  // UNIX
#if defined(TIOCGSIZE)
    struct ttysize w;
    if (ioctl(STDOUT_FILENO, TIOCGSIZE, &w) >= 0) {
        width = static_cast<std::size_t>(w.ts_cols);
        height = static_cast<std::size_t>(w.ts_lines);
        if (width < _min_width) {
            width = _min_width;
        }
    }
#elif defined(TIOCGWINSZ)
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) >= 0) {
        width = static_cast<std::size_t>(w.ws_col);
        height = static_cast<std::size_t>(w.ws_row);
        if (width < _min_width) {
            width = _min_width;
        }
    }
#endif  // TIOCGSIZE
#endif  // _WIN32
#endif  // ARGPARSE_NO_AUTODETECT
    return std::make_pair(width, height);
}

inline void
_ltrim(std::string& s)
{
    s.erase(std::begin(s),
            std::find_if(std::begin(s), std::end(s),
                         [] (unsigned char c) { return !std::isspace(c); }));
}

inline void
_rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [] (unsigned char c)
    { return !std::isspace(c); }).base(), s.end());
}

inline void
_trim(std::string& s)
{
    _ltrim(s);
    _rtrim(s);
}

inline std::string
_trim_copy(std::string s)
{
    _trim(s);
    return s;
}

inline std::string
_to_lower(std::string s)
{
    std::transform(std::begin(s), std::end(s), std::begin(s),
                   [] (unsigned char c)
    { return static_cast<char>(std::tolower(c)); });
    return s;
}

inline std::string
_to_upper(std::string s)
{
    std::transform(std::begin(s), std::end(s), std::begin(s),
                   [] (unsigned char c)
    { return static_cast<char>(std::toupper(c)); });
    return s;
}

inline std::string
_file_name(std::string const& path)
{
#ifdef _ARGPARSE_USE_FILESYSTEM
    return std::filesystem::path(path.c_str()).filename().string();
#else
    return path.substr(path.find_last_of("/\\") + 1);
#endif  // _ARGPARSE_USE_FILESYSTEM
}

inline bool
_have_quotes(std::string const& str)
{
    return str.size() > 1 && str.front() == str.back()
            && (str.front() == '\'' || str.front() == '\"');
}

inline void
_resolve_conflict(std::string const& s, std::vector<std::string>& values)
{
    auto it = std::find(std::begin(values), std::end(values), s);
    if (it != std::end(values)) {
        values.erase(it);
    }
}

inline void
_resolve_conflict(std::vector<std::string> const& vec,
                  std::vector<std::string>& values)
{
    for (auto const& s : vec) {
        _resolve_conflict(s, values);
    }
}

template <class T = std::string>
typename std::enable_if<std::is_constructible<std::string, T>::value, T>::type
_remove_quotes(std::string const& s)
{
    return _have_quotes(s) ? T(s).substr(1, s.size() - 2) : T(s);
}

inline bool
_contains_substr(std::string const& str, std::string const& substr)
{
    //  C++23+
    //  return str.contains(substr);
    return str.find(substr) != std::string::npos;
}

inline std::string
_replace(std::string s, char old, std::string const& value)
{
    auto pos = s.find(old);
    while (pos != std::string::npos) {
        s.replace(pos, 1, value);
        pos = s.find(old, pos + value.size());
    }
    return s;
}

inline std::string
_replace(std::string s, std::string const& old, std::string const& value)
{
    auto pos = s.find(old);
    while (pos != std::string::npos) {
        s.replace(pos, old.length(), value);
        pos = s.find(old, pos + value.size());
    }
    return s;
}

inline std::string
_replace(std::string const& s,
         std::function<bool(unsigned char)> func, std::string const& value)
{
    std::string res;
    for (auto c : s) {
        if (func(static_cast<unsigned char>(c))) {
            res += value;
        } else {
            res += c;
        }
    }
    return res;
}

inline std::string
_format_name(std::string const& s)
{
    auto res = _replace(s, [] (unsigned char c) { return std::iscntrl(c)
                                                   || std::isspace(c); }, "");
    _trim(res);
    res = _replace(res, [] (unsigned char c) { return std::isblank(c); }, "_");
    return res;
}

inline bool
_starts_with(std::string const& s, std::string const& value)
{
#if __cplusplus >= 202002L  // C++20+
    return s.starts_with(value);
#else
    return s.compare(0, value.size(), value) == 0;
#endif  // C++20+
}

template <class T>
bool
_is_value_exists(T const& value, std::vector<T> const& vec)
{
    return std::any_of(std::begin(vec), std::end(vec),
                       [&value] (T const& el) { return el == value; });
}

inline bool
_is_value_exists(char value, std::string const& str)
{
    return std::any_of(std::begin(str), std::end(str),
                       [&value] (char el) { return el == value; });
}

template <class T>
void
_insert_vector_to_end(std::deque<T> const& from, std::deque<T>& to)
{
    if (!from.empty()) {
        to.insert(std::end(to), std::begin(from), std::end(from));
    }
}

template <class T>
void
_insert_vector_to_end(std::deque<T> const& from, std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(std::end(to), std::begin(from), std::end(from));
    }
}

template <class T>
void
_insert_vector_to_end(std::vector<T> const& from, std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(std::end(to), std::begin(from), std::end(from));
    }
}

template <class T>
void
_move_vector_insert_to_end(std::deque<T>& from, std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(std::end(to),
                  std::make_move_iterator(std::begin(from)),
                  std::make_move_iterator(std::end(from)));
    }
}

template <class T>
void
_move_vector_insert_to_end(std::vector<T>& from, std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(std::end(to),
                  std::make_move_iterator(std::begin(from)),
                  std::make_move_iterator(std::end(from)));
    }
}

template <class T>
void
_move_vector_insert_to(std::vector<T>& from, std::vector<T>& to, std::size_t i)
{
    if (!from.empty()) {
        using dtype = typename std::vector<T>::difference_type;
        to.insert(std::next(std::begin(to), static_cast<dtype>(i)),
                  std::make_move_iterator(std::begin(from)),
                  std::make_move_iterator(std::end(from)));
    }
}

template <class T>
void
_move_vector_replace_at(std::vector<T>& from, std::vector<T>& to, std::size_t i)
{
    using dtype = typename std::vector<T>::difference_type;
    to.erase(std::next(std::begin(to), static_cast<dtype>(i)));
    to.insert(std::next(std::begin(to), static_cast<dtype>(i)),
              std::make_move_iterator(std::begin(from)),
              std::make_move_iterator(std::end(from)));
}

inline std::string
_flag_name(std::string str)
{
    auto prefix = str.front();
    str.erase(std::begin(str),
              std::find_if(std::begin(str), std::end(str),
                           [prefix] (char c) { return c != prefix; }));
    return str;
}

inline std::vector<std::string>
_help_flags(std::string const& prefix_chars)
{
    auto const& prefix = _is_value_exists(_prefix_char, prefix_chars)
            ? _prefix_char : prefix_chars.front();
    return { std::string(1, prefix) + "h", std::string(2, prefix) + "help" };
}

inline bool
_is_negative_number(std::string const& str)
{
    double value;
    std::stringstream ss(str);
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        return false;
    }
    return value < 0;
}

inline bool
_is_optional(std::string const& arg,
             std::string const& prefix_chars,
             bool have_negative_args,
             bool was_pseudo_arg)
{
    return _is_value_exists(arg.front(), prefix_chars) && !was_pseudo_arg
            && (have_negative_args || !_is_negative_number(arg));
}

inline bool
_not_optional(std::string const& arg,
              std::string const& prefix_chars,
              bool have_negative_args,
              bool was_pseudo_arg)
{
    return !_is_value_exists(arg.front(), prefix_chars) || was_pseudo_arg
            || (!have_negative_args && _is_negative_number(arg));
}

inline std::string
_make_no_flag(std::string str)
{
    auto prefix = str.front();
    auto it = std::find_if(std::begin(str), std::end(str),
                           [prefix] (char c) { return c != prefix; });
    if (it == std::end(str)) {
        throw ValueError("can't create no- boolean option");
    }
    str.insert(static_cast<std::string::size_type>(
                   std::distance(std::begin(str), it)), "no-");
    return str;
}

inline void
_append_value_to(std::string const& value, std::string& str,
                 std::string const& separator = _spaces, bool force = false)
{
    if (!str.empty() || force) {
        str += separator;
    }
    str += value;
}

inline void
_store_value_to(std::string& value, std::vector<std::string>& result,
                bool force = false)
{
    if (!value.empty() || force) {
        result.push_back(value);
        value.clear();
    }
}

inline std::vector<std::string>
_split(std::string const& str, char delim,
       bool force = false, bool add_delim = false)
{
    std::vector<std::string> result;
    std::string value;
    for (auto c : str) {
        if (c == delim) {
            _store_value_to(value, result, force);
            if (add_delim) {
                value = std::string(1, delim);
                _store_value_to(value, result, true);
            }
        } else {
            value += c;
        }
    }
    _store_value_to(value, result);
    return result;
}

inline std::vector<std::string>
_split_whitespace(std::string const& str, bool force = false)
{
    std::vector<std::string> result;
    std::string value;
    for (auto c : str) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            _store_value_to(value, result, force);
        } else {
            value += c;
        }
    }
    _store_value_to(value, result);
    return result;
}

inline std::pair<std::string, std::string>
_split_delimiter(std::string const& s, char delim)
{
    auto pos = s.find(delim);
    if (pos != std::string::npos) {
        return std::make_pair(s.substr(0, pos), s.substr(pos + 1));
    } else {
        return std::make_pair(s, std::string());
    }
}

inline std::vector<std::string>
_split_equal(std::string const& s, std::string const& prefix)
{
    auto pos = _is_value_exists(_equal, prefix)
            ? s.find(_equal, static_cast<std::string::size_type>(
                         std::distance(std::begin(s),
                                       std::find_if(std::begin(s), std::end(s),
                                                    [] (char c)
    { return c != _equal; }))))
            : s.find(_equal);
    if (pos != std::string::npos) {
        return { s.substr(0, pos), s.substr(pos + 1) };
    } else {
        return { s };
    }
}

inline void
_process_quotes(std::deque<char>& quotes, std::string const& value,
                std::string const& str, char c, std::size_t i)
{
    if (c == '\"' || c == '\'') {
        if (!quotes.empty()
                && quotes.back() == c
                && (i == str.size()
                    || std::isspace(static_cast<unsigned char>(str.at(i)))
                    || std::ispunct(static_cast<unsigned char>(str.at(i))))) {
            quotes.pop_back();
        } else if (value.empty()
                   || std::ispunct(static_cast<unsigned char>(value.back()))) {
            quotes.push_back(c);
        }
    }
}

inline std::vector<std::string>
_split_to_args(std::string const& str)
{
    std::vector<std::string> result;
    std::string value;
    std::deque<char> quotes;
    for (std::size_t i = 0; i < str.size(); ++i) {
        auto c = str.at(i);
        bool skip = false;
        if (c == '\\') {
            // skip space
            skip = true;
            if (++i == str.size()) {
                value += c;
                break;
            } else {
                if (str.at(i) != _space) {
                    value += c;
                }
                c = str.at(i);
            }
        }
        if (((c == _space && !skip)
             || (c != _space && std::isspace(static_cast<unsigned char>(c))))
                && quotes.empty()) {
            _store_value_to(value, result);
        } else {
            _process_quotes(quotes, value, str, c, i + 1);
            value += c;
        }
    }
    _store_value_to(value, result);
    if (!quotes.empty()) {
        std::cerr << "possible incorrect string: '" << str << "'" << std::endl;
    }
    return result;
}

inline bool
_string_to_bool(std::string const& str) _ARGPARSE_NOEXCEPT
{
    return !str.empty();
}

inline std::string
_bool_to_string(std::string const& str)
{
    return _string_to_bool(str) ? "true" : "false";
}

inline std::string
_vector_to_string(std::vector<std::string> const& vec,
                  std::string const& separator = _spaces,
                  std::string const& quotes = std::string(),
                  bool replace_space = false,
                  std::string const& none = std::string(),
                  std::string const& begin = std::string(),
                  std::string const& end = std::string())
{
    std::string res;
    for (auto const& el : vec) {
        if (!res.empty()) {
            res += separator;
        }
        auto val = el;
        if (quotes.empty() && replace_space && !_have_quotes(val)) {
            val = _replace(val, _space, "\\ ");
        }
        res += quotes + val + quotes;
    }
    return begin + (res.empty() ? none : res) + end;
}

inline std::string
_matrix_to_string(std::vector<std::vector<std::string> > const& matrix,
                  std::string const& separator = _spaces,
                  std::string const& quotes = std::string(),
                  bool replace_space = false,
                  std::string const& none = std::string(),
                  std::string const& begin = std::string(),
                  std::string const& end = std::string())
{
    std::string res;
    for (auto const& vec : matrix) {
        if (!res.empty()) {
            res += separator;
        }
        res += _vector_to_string(vec, separator, quotes, replace_space,
                                 none, begin, end);
    }
    return begin + (res.empty() ? (begin + res + end) : res) + end;
}

inline std::string
_ignore_explicit(std::string const& arg, std::string const& value)
{
    return "argument " + arg + ": ignored explicit argument '" + value + "'";
}

inline std::string
_format_output(std::string const& head, std::string const& body,
               std::size_t interlayer, std::size_t indent, std::size_t width,
               char delimiter = '\n')
{
    std::vector<std::string> result;
    std::string value = head;
    if (value.size() + interlayer > indent) {
        _store_value_to(value, result);
    }
    auto _func = [indent, width, &result, &value] (std::string const& str)
    {
        if (value.size() > indent && value.size() + 1 + str.size() > width) {
            _store_value_to(value, result);
        }
        if (value.size() < indent) {
            value.resize(indent, _space);
            value += str;
        } else {
            value += _spaces + str;
        }
    };
    auto split_str = _split(body, '\n', true);
    for (auto const& str : split_str) {
        if (delimiter == '\n') {
            _func(str);
        } else if (str.empty()) {
            value.resize(indent, _space);
            _store_value_to(value, result, true);
        } else {
            auto sub_split_str = _split(str, delimiter, true);
            for (auto const& sub : sub_split_str) {
                _func(sub);
            }
            _store_value_to(value, result);
        }
    }
    _store_value_to(value, result);
    return _vector_to_string(result, "\n");
}

inline std::string
_help_formatter(std::string const& head,
                _HelpFormatter const& formatter,
                std::string const& help,
                std::size_t width,
                std::string const& indent = std::string())
{
    std::size_t const interlayer = 2;

    std::vector<std::string> result;
    std::string value = head;
    if (value.size() + interlayer > indent.size()) {
        _store_value_to(value, result);
    }
    if (!help.empty()) {
        auto lines = formatter._split_lines(help, width - indent.size());
        for (auto const& line : lines) {
            if (value.size() < indent.size()) {
                value.resize(indent.size(), _space);
            }
            value += line;
            _store_value_to(value, result, true);
        }
    }
    _store_value_to(value, result);
    return _vector_to_string(result, "\n");
}

inline void
_print_raw_text_formatter(_HelpFormatter const& formatter,
                          std::string const& text,
                          std::size_t width,
                          std::ostream& os,
                          std::string const& begin = "\n",
                          std::string const& indent = std::string(),
                          std::string const& end = std::string())
{
    if (!text.empty()) {
        os << begin
           << formatter._fill_text(text, width, indent) << end << std::endl;
    }
}

template <class T>
std::string
_get_type_name()
{
#if defined(_MSC_VER) && !defined(__clang__)
    std::string res = __FUNCSIG__;
    auto pos = res.find("__cdecl") + 8;
    return _replace(
                res.substr(pos + 33, res.size() - pos - 33 - 7), "class ", "");
#elif defined(__clang__)
    std::string res = __PRETTY_FUNCTION__;
    auto pos = res.find('=') + 2;
    return res.substr(pos, res.size() - pos - 1);
#elif defined(__GNUC__)
    std::string res = __PRETTY_FUNCTION__;
    auto pos = res.find('=') + 2;
    return res.substr(pos, res.find(';', pos) - pos);
#else
    return std::string();
#endif  // _MSC_VER
}

class Type
{
public:
    template <class T, typename std::enable_if<
                  std::is_same<std::string, T>::value
#if __cplusplus >= 201703L  // C++17+
                  || std::is_same<std::string_view, T>::value
#endif  // C++17+
                  || is_stl_pair<T>::value
                  || is_stl_tuple<T>::value>::type* = nullptr>
    std::string static
    basic()
    {
        return name<T>();
    }

    template <class T, typename std::enable_if<
                  is_stl_array<T>::value
                  || is_stl_container<T>::value
                  || is_stl_queue<T>::value>::type* = nullptr>
    std::string static
    basic()
    {
        return basic<typename T::value_type>();
    }

    template <class T, typename std::enable_if<
                  is_stl_map<T>::value>::type* = nullptr>
    std::string static
    basic()
    {
        return "std::pair<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  !std::is_same<std::string, T>::value
#if __cplusplus >= 201703L  // C++17+
                  && !std::is_same<std::string_view, T>::value
#endif  // C++17+
                  && !is_stl_array<T>::value
                  && !is_stl_container<T>::value
                  && !is_stl_map<T>::value
                  && !is_stl_pair<T>::value
                  && !is_stl_queue<T>::value
                  && !is_stl_tuple<T>::value>::type* = nullptr>
    std::string static
    basic()
    {
        return _get_type_name<T>();
    }

    template <class T, typename std::enable_if<
#if __cplusplus >= 201703L  // C++17+
                  std::is_same<std::string_view, T>::value ||
#endif  // C++17+
                  std::is_same<std::string, T>::value>::type* = nullptr>
    std::string static
    name()
    {
        return "std::string";
    }

    template <class T, typename std::enable_if<
                  is_stl_array<T>::value>::type* = nullptr>
    std::string static
    name()
    {
        auto str = _replace(_get_type_name<T>(), "__cxx11::", "");
        return str.substr(0, str.find('<'))
                + "<" + name<typename T::value_type>()
                + ", " + std::to_string(std::tuple_size<T>::value) + ">";
    }

    template <class T, typename std::enable_if<
                  is_stl_container<T>::value
                  || is_stl_queue<T>::value>::type* = nullptr>
    std::string static
    name()
    {
        auto str = _replace(_get_type_name<T>(), "__cxx11::", "");
        return str.substr(0, str.find('<'))
                + "<" + name<typename T::value_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  is_stl_map<T>::value>::type* = nullptr>
    std::string static
    name()
    {
        auto str = _get_type_name<T>();
        return str.substr(0, str.find('<')) + "<" + name<typename T::key_type>()
                + ", " + name<typename T::mapped_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  is_stl_pair<T>::value>::type* = nullptr>
    std::string static
    name()
    {
        return "std::pair<" + name<typename T::first_type>()
                + ", " + name<typename T::second_type>() + ">";
    }

    template <class T, typename std::enable_if<
                  is_stl_tuple<T>::value>::type* = nullptr>
    std::string static
    name()
    {
        return tuple_as_string(type_tag<T>{});
    }

    template <class T, typename std::enable_if<
                  !std::is_same<std::string, T>::value
#if __cplusplus >= 201703L  // C++17+
                  && !std::is_same<std::string_view, T>::value
#endif  // C++17+
                  && !is_stl_array<T>::value
                  && !is_stl_container<T>::value
                  && !is_stl_map<T>::value
                  && !is_stl_pair<T>::value
                  && !is_stl_queue<T>::value
                  && !is_stl_tuple<T>::value>::type* = nullptr>
    std::string static
    name()
    {
        return _get_type_name<T>();
    }

private:
    template <std::size_t N>
    std::string static&
    tuple_type(std::string& s)
    {
        return s;
    }

    template <std::size_t N, class T, class... Ts>
    std::string static&
    tuple_type(std::string& s)
    {
        if (!s.empty()) {
            s += ", ";
        }
        s += name<T>();
        return tuple_type<N - 1, Ts...>(s);
    }

    template <class... Ts>
    std::string static
    tuple_as_string(type_tag<std::tuple<Ts...>>)
    {
        std::string result;
        tuple_type<sizeof...(Ts), Ts...>(result);
        return "std::tuple<" + result + ">";
    }
};

template <class T>
class Value
{
public:
    Value()
        : m_value(),
          m_has_value(false)
    { }

    Value(Value const& orig)
        : m_value(orig.m_value),
          m_has_value(orig.m_has_value)
    { }

    Value(Value&& orig) _ARGPARSE_NOEXCEPT
        : m_value(std::move(orig.m_value)),
          m_has_value(std::move(orig.m_has_value))
    { }

    explicit Value(T const& orig)
        : m_value(orig),
          m_has_value(true)
    { }

    explicit Value(T&& orig) _ARGPARSE_NOEXCEPT
        : m_value(std::move(orig)),
          m_has_value(true)
    { }

    inline Value& operator =(Value const& rhs)
    {
        if (this != &rhs) {
            m_value     = rhs.m_value;
            m_has_value = rhs.m_has_value;
        }
        return *this;
    }

    inline Value& operator =(Value&& rhs) _ARGPARSE_NOEXCEPT
    {
        if (this != &rhs) {
            m_value     = std::move(rhs.m_value);
            m_has_value = std::move(rhs.m_has_value);
        }
        return *this;
    }

    inline Value& operator =(T const& rhs)
    {
        m_value     = rhs;
        m_has_value = true;
        return *this;
    }

    inline Value& operator =(T&& rhs) _ARGPARSE_NOEXCEPT
    {
        m_value     = std::move(rhs);
        m_has_value = true;
        return *this;
    }

    inline bool operator ==(Value const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return m_has_value == rhs.m_has_value && m_value == rhs.m_value;
    }

    inline bool operator ==(T const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return m_has_value && m_value == rhs;
    }

    inline bool operator !=(Value const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return !(*this == rhs);
    }

    inline bool operator !=(T const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return !(*this == rhs);
    }

    inline void clear(T const& value = T())
    {
        m_value     = value;
        m_has_value = false;
    }

    inline bool     has_value() const _ARGPARSE_NOEXCEPT { return m_has_value; }
    inline T const& value()     const _ARGPARSE_NOEXCEPT { return m_value; }
    inline T const& operator()()const _ARGPARSE_NOEXCEPT { return m_value; }

private:
    T       m_value;
    bool    m_has_value;
};

inline bool
_is_type_name_correct(std::string const& expected,
                      std::string const& received) _ARGPARSE_NOEXCEPT
{
    return expected.empty() || received == expected;
}

inline void
_check_type_name(Value<std::string> const& expected,
                 std::string const& received)
{
    if (expected.has_value() && !_is_type_name_correct(expected(), received)) {
        throw TypeError("type_name missmatch: expected '" + expected() + "'"
                        + ", received '" + received + "'");
    }
}
}  // namespace detail

/*!
 *  \brief Don't use this enum name! use argparse::SUPPRESS value directly
 *
 *  \enum _SUPPRESS
 */
_ARGPARSE_EXPORT enum _SUPPRESS : uint8_t {} _ARGPARSE_INLINE_VARIABLE SUPPRESS;

/*!
 *  \brief Don't use this enum name! use argparse::REMAINDER value directly
 *
 *  \enum _REMAINDER
 */
_ARGPARSE_EXPORT enum _REMAINDER:uint8_t {} _ARGPARSE_INLINE_VARIABLE REMAINDER;

/*!
 *  \brief Argument class
 */
_ARGPARSE_EXPORT class Argument
{
    friend class _ArgumentData;
    friend class _ArgumentDefaultsHelpFormatter;
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class _BaseArgumentGroup;
    friend class _BaseParser;
    friend class HelpFormatter;
    friend class MutuallyExclusiveGroup;
    friend class Namespace;

    enum Nargs : std::uint8_t
    {
        NARGS_DEF       = 0x01,  // ""
        NARGS_NUM       = 0x02,  // "N"
        ONE_OR_MORE     = 0x04,  // "+"
        ZERO_OR_ONE     = 0x08,  // "?"
        ZERO_OR_MORE    = 0x10,  // "*"
        REMAINDER       = 0x20,  // argparse::REMAINDER
    };

    enum Type : std::uint8_t
    {
        NoType,
        Positional,
        Optional
    };

    explicit
    Argument(std::vector<std::string> const& flags,
             std::string const& name,
             Type type)
        : m_flags(flags),
          m_all_flags(m_flags),
          m_name(name),
          m_action(Action::store),
          m_type(type),
          m_default_type(),
          m_help_type(),
          m_nargs(NARGS_DEF),
          m_num_args(1),
          m_nargs_str("1"),
          m_const(),
          m_default(),
          m_implicit(),
          m_type_name(),
          m_choices(),
          m_required(),
          m_help(),
          m_metavar(),
          m_dest(std::vector<std::string>{ std::string() }),
          m_version(),
          m_handle(nullptr),
          m_post_trigger(nullptr)
    { }

    explicit
    Argument(std::vector<std::string>&& flags,
             std::string&& name,
             Type type)
        : m_flags(std::move(flags)),
          m_all_flags(m_flags),
          m_name(std::move(name)),
          m_action(Action::store),
          m_type(type),
          m_default_type(),
          m_help_type(),
          m_nargs(NARGS_DEF),
          m_num_args(1),
          m_nargs_str("1"),
          m_const(),
          m_default(),
          m_implicit(),
          m_type_name(),
          m_choices(),
          m_required(),
          m_help(),
          m_metavar(),
          m_dest(std::vector<std::string>{ std::string() }),
          m_version(),
          m_handle(nullptr),
          m_post_trigger(nullptr)
    { }

    static inline std::shared_ptr<Argument>
    make_argument(std::vector<std::string> const& flags,
                  std::string const& name,
                  Type type)
    {
        return std::make_shared<Argument>(Argument(flags, name, type));
    }

    static inline std::shared_ptr<Argument>
    make_argument(std::vector<std::string>&& flags,
                  std::string&& name,
                  Type type)
    {
        return std::make_shared<Argument>(
                    Argument(std::move(flags), std::move(name), type));
    }

public:
    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param args Argument flags
     *
     *  \return Argument object
     */
    template <class... Args>
    explicit
    Argument(Args... args)
        : Argument(std::vector<std::string>{ args... })
    { }

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
    explicit
    Argument(std::initializer_list<std::string> flags)
        : Argument(std::vector<std::string>{ flags })
    { }

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
    template <typename = void>
    explicit
    Argument(std::vector<std::string> const& flags)
        : m_flags(flags),
          m_all_flags(m_flags),
          m_name(),
          m_action(Action::store),
          m_type(NoType),
          m_default_type(),
          m_help_type(),
          m_nargs(NARGS_DEF),
          m_num_args(1),
          m_nargs_str("1"),
          m_const(),
          m_default(),
          m_implicit(),
          m_type_name(),
          m_choices(),
          m_required(),
          m_help(),
          m_metavar(),
          m_dest(std::vector<std::string>{ std::string() }),
          m_version(),
          m_handle(nullptr),
          m_post_trigger(nullptr)
    { }

    /*!
     *  \brief Construct argument object from another argument
     *
     *  \param orig Argument object to copy
     *
     *  \return Argument object
     */
    Argument(Argument const& orig)
        : m_flags(orig.m_flags),
          m_all_flags(orig.m_all_flags),
          m_name(orig.m_name),
          m_action(orig.m_action),
          m_type(orig.m_type),
          m_default_type(orig.m_default_type),
          m_help_type(orig.m_help_type),
          m_nargs(orig.m_nargs),
          m_num_args(orig.m_num_args),
          m_nargs_str(orig.m_nargs_str),
          m_const(orig.m_const),
          m_default(orig.m_default),
          m_implicit(orig.m_implicit),
          m_type_name(orig.m_type_name),
          m_choices(orig.m_choices),
          m_required(orig.m_required),
          m_help(orig.m_help),
          m_metavar(orig.m_metavar),
          m_dest(orig.m_dest),
          m_version(orig.m_version),
          m_handle(orig.m_handle),
          m_post_trigger(orig.m_post_trigger)
    { }

    /*!
     *  \brief Construct argument object from another argument
     *
     *  \param orig Argument object to move
     *
     *  \return Argument object
     */
    Argument(Argument&& orig) _ARGPARSE_NOEXCEPT
        : m_flags(std::move(orig.m_flags)),
          m_all_flags(std::move(orig.m_all_flags)),
          m_name(std::move(orig.m_name)),
          m_action(std::move(orig.m_action)),
          m_type(std::move(orig.m_type)),
          m_default_type(std::move(orig.m_default_type)),
          m_help_type(std::move(orig.m_help_type)),
          m_nargs(std::move(orig.m_nargs)),
          m_num_args(std::move(orig.m_num_args)),
          m_nargs_str(std::move(orig.m_nargs_str)),
          m_const(std::move(orig.m_const)),
          m_default(std::move(orig.m_default)),
          m_implicit(std::move(orig.m_implicit)),
          m_type_name(std::move(orig.m_type_name)),
          m_choices(std::move(orig.m_choices)),
          m_required(std::move(orig.m_required)),
          m_help(std::move(orig.m_help)),
          m_metavar(std::move(orig.m_metavar)),
          m_dest(std::move(orig.m_dest)),
          m_version(std::move(orig.m_version)),
          m_handle(std::move(orig.m_handle)),
          m_post_trigger(std::move(orig.m_post_trigger))
    { }

    /*!
     *  \brief Copy argument object from another argument
     *
     *  \param rhs Argument object to copy
     *
     *  \return Current argument reference
     */
    Argument& operator =(Argument const& rhs)
    {
        if (this != &rhs) {
            this->m_flags           = rhs.m_flags;
            this->m_all_flags       = rhs.m_all_flags;
            this->m_name            = rhs.m_name;
            this->m_action          = rhs.m_action;
            this->m_type            = rhs.m_type;
            this->m_default_type    = rhs.m_default_type;
            this->m_help_type       = rhs.m_help_type;
            this->m_nargs           = rhs.m_nargs;
            this->m_num_args        = rhs.m_num_args;
            this->m_nargs_str       = rhs.m_nargs_str;
            this->m_const           = rhs.m_const;
            this->m_default         = rhs.m_default;
            this->m_implicit        = rhs.m_implicit;
            this->m_type_name       = rhs.m_type_name;
            this->m_choices         = rhs.m_choices;
            this->m_required        = rhs.m_required;
            this->m_help            = rhs.m_help;
            this->m_metavar         = rhs.m_metavar;
            this->m_dest            = rhs.m_dest;
            this->m_version         = rhs.m_version;
            this->m_handle          = rhs.m_handle;
            this->m_post_trigger    = rhs.m_post_trigger;
        }
        return *this;
    }

    /*!
     *  \brief Move argument object from another argument
     *
     *  \param rhs Argument object to move
     *
     *  \return Current argument reference
     */
    Argument& operator =(Argument&& rhs) _ARGPARSE_NOEXCEPT
    {
        if (this != &rhs) {
            this->m_flags           = std::move(rhs.m_flags);
            this->m_all_flags       = std::move(rhs.m_all_flags);
            this->m_name            = std::move(rhs.m_name);
            this->m_action          = std::move(rhs.m_action);
            this->m_type            = std::move(rhs.m_type);
            this->m_default_type    = std::move(rhs.m_default_type);
            this->m_help_type       = std::move(rhs.m_help_type);
            this->m_nargs           = std::move(rhs.m_nargs);
            this->m_num_args        = std::move(rhs.m_num_args);
            this->m_nargs_str       = std::move(rhs.m_nargs_str);
            this->m_const           = std::move(rhs.m_const);
            this->m_default         = std::move(rhs.m_default);
            this->m_implicit        = std::move(rhs.m_implicit);
            this->m_type_name       = std::move(rhs.m_type_name);
            this->m_choices         = std::move(rhs.m_choices);
            this->m_required        = std::move(rhs.m_required);
            this->m_help            = std::move(rhs.m_help);
            this->m_metavar         = std::move(rhs.m_metavar);
            this->m_dest            = std::move(rhs.m_dest);
            this->m_version         = std::move(rhs.m_version);
            this->m_handle          = std::move(rhs.m_handle);
            this->m_post_trigger    = std::move(rhs.m_post_trigger);
        }
        return *this;
    }

    /*!
     *  \brief Compare current argument with another one
     *
     *  \param rhs Argument to compare
     *
     *  \return true if current argument lesser, otherwise false
     */
    inline bool operator <(Argument const& rhs) const
    {
        return m_flags < rhs.m_flags;
    }

    /*!
     *  \brief Set argument 'action' value (default: "store")
     *
     *  \param value Action value
     *
     *  \return Current argument reference
     */
    Argument& action(std::string const& value)
    {
        if (value == "store") {
            return action(Action::store);
        } else if (value == "store_const") {
            return action(Action::store_const);
        } else if (value == "store_true") {
            return action(Action::store_true);
        } else if (value == "store_false") {
            return action(Action::store_false);
        } else if (value == "append") {
            return action(Action::append);
        } else if (value == "append_const") {
            return action(Action::append_const);
        } else if (value == "count") {
            return action(Action::count);
        } else if (value == "help") {
            return action(Action::help);
        } else if (value == "version") {
            return action(Action::version);
        } else if (value == "extend") {
            return action(Action::extend);
        }
        throw ValueError("unknown action '" + value + "'");
    }

    /*!
     *  \brief Set argument 'action' value (default: argparse::store)
     *
     *  \param value Action value
     *
     *  \return Current argument reference
     */
    Argument& action(Action value)
    {
        prepare_action(value);
        switch (value) {
            case Action::store_true :
                if (!m_default.has_value()) {
                    m_default.clear();
                }
                // fallthrough
            case Action::BooleanOptionalAction :
                m_const.clear("1");
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::store_false :
                if (!m_default.has_value()) {
                    m_default.clear("1");
                }
                // fallthrough
            case Action::store_const :
            case Action::append_const :
                m_const.clear();
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::version :
                help("show program's version number and exit");
                // fallthrough
            case Action::help :
                if (m_type == Positional) {
                    // version and help actions cannot be positional
                    throw
                    TypeError("got an unexpected keyword argument 'required'");
                }
                // fallthrough
            case Action::count :
                m_const.clear();
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::store :
            case Action::append :
            case Action::extend :
                m_const.clear();
                if (m_num_args == 0) {
                    m_nargs = NARGS_DEF;
                    m_nargs_str = "1";
                    m_num_args = 1;
                }
                break;
            default :
                throw ValueError("unknown action");
        }
        m_action = value;
        return *this;
    }

    /*!
     *  \brief Set argument 'nargs' value
     *
     *  \param value Nargs value
     *
     *  \return Current argument reference
     */
    inline Argument& nargs(std::size_t value)
    {
        switch (action()) {
            case Action::store_const :
            case Action::store_true :
            case Action::store_false :
            case Action::append_const :
            case Action::help :
            case Action::version :
            case Action::count :
            case Action::BooleanOptionalAction :
                throw TypeError("got an unexpected keyword argument 'nargs'");
            case Action::store :
                if (value == 0) {
                    throw
                    ValueError("nargs for store actions must be != 0; if you "
                               "have nothing to store, actions such as store "
                               "true or store const may be more appropriate");
                }
                break;
            case Action::append :
            case Action::extend :
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
        m_nargs_str = std::to_string(value);
        m_num_args = value;
        return *this;
    }

    /*!
     *  \brief Set argument 'nargs' value
     *
     *  \param value Nargs value : "?", "*", "+"
     *
     *  \return Current argument reference
     */
    inline Argument& nargs(std::string const& value)
    {
        if (!(action() & detail::_store_action)) {
            throw TypeError("got an unexpected keyword argument 'nargs'");
        }
        auto param = detail::_trim_copy(value);
        if (param == "?") {
            m_nargs = ZERO_OR_ONE;
        } else if (param == "*") {
            m_nargs = ZERO_OR_MORE;
        } else if (param == "+") {
            m_nargs = ONE_OR_MORE;
        } else {
            throw ValueError("invalid nargs value '" + param + "'");
        }
        m_nargs_str = std::move(param);
        m_num_args = 1;
        return *this;
    }

    /*!
     *  \brief Set argument 'nargs' argparse::REMAINDER value
     *
     *  \return Current argument reference
     */
    inline Argument& nargs(_REMAINDER value)
    {
        if (!(action() & detail::_store_action)
                || value != argparse::REMAINDER) {
            throw TypeError("got an unexpected keyword argument 'nargs'");
        }
        m_nargs = REMAINDER;
        m_nargs_str = "0";
        m_num_args = 0;
        return *this;
    }

    /*!
     *  \brief Set argument 'nargs' optional ("?") value
     *
     *  \return Current argument reference
     */
    inline Argument& optional()
    {
        return nargs("?");
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_one ("?") value
     *
     *  \return Current argument reference
     */
    inline Argument& zero_or_one()
    {
        return nargs("?");
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_more ("*") value
     *
     *  \return Current argument reference
     */
    inline Argument& zero_or_more()
    {
        return nargs("*");
    }

    /*!
     *  \brief Set argument 'nargs' one_or_more ("+") value
     *
     *  \return Current argument reference
     */
    inline Argument& one_or_more()
    {
        return nargs("+");
    }

    /*!
     *  \brief Set argument 'nargs' argparse::REMAINDER value
     *
     *  \return Current argument reference
     */
    inline Argument& remainder()
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
    inline Argument& const_value(std::string const& value)
    {
        if ((action() & detail::_const_action)
                || (m_nargs == ZERO_OR_ONE
                    && (action() & detail::_store_action))) {
            m_const = detail::_trim_copy(value);
        } else if (m_type == Optional && m_nargs != ZERO_OR_ONE
                   && (action() & detail::_store_action)) {
            throw ValueError("nargs must be '?' to supply const");
        } else {
            throw TypeError("got an unexpected keyword argument 'const'");
        }
        return *this;
    }

    /*!
     *  \brief Set custom argument 'const' value
     *
     *  \param value Const value
     *
     *  \return Current argument reference
     */
    template <class T,
              class = typename std::enable_if<
                  !std::is_constructible<std::string, T>::value>::type>
    Argument& const_value(T const& value)
    {
        std::stringstream ss;
        ss << value;
        const_value(ss.str());
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
    inline Argument& default_value(std::string const& value)
    {
        m_default = detail::_trim_copy(value);
        return *this;
    }

    /*!
     *  \brief Set custom argument 'default' value
     *
     *  \param value Default value
     *
     *  \return Current argument reference
     */
    template <class T,
              class = typename std::enable_if<
                  !std::is_constructible<std::string, T>::value>::type>
    Argument& default_value(T const& value)
    {
        std::stringstream ss;
        ss << value;
        m_default = ss.str();
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
    inline Argument& default_value(_SUPPRESS value)
    {
        if (value != argparse::SUPPRESS) {
            throw TypeError("got an unexpected keyword argument 'default'");
        }
        if (action() == Action::store_false) {
            m_default.clear("1");
        } else {
            m_default.clear();
        }
        m_default_type = value;
        return *this;
    }

    /*!
     *  \brief Set argument 'implicit' value
     *
     *  \param value Implicit value
     *
     *  \return Current argument reference
     */
    inline Argument& implicit_value(std::string const& value)
    {
        m_implicit = detail::_trim_copy(value);
        return *this;
    }

    /*!
     *  \brief Set custom argument 'implicit' value
     *
     *  \param value Implicit value
     *
     *  \return Current argument reference
     */
    template <class T,
              class = typename std::enable_if<
                  !std::is_constructible<std::string, T>::value>::type>
    Argument& implicit_value(T const& value)
    {
        std::stringstream ss;
        ss << value;
        m_implicit = ss.str();
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
    Argument& type()
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
    inline Argument& type(std::string const& value)
    {
        m_type_name.clear(detail::_trim_copy(value));
        return *this;
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choices value
     *
     *  \return Current argument reference
     */
    template <typename = void>
    Argument& choices(std::string const& value)
    {
        if (!(action() & detail::_store_action)) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        std::vector<std::string> values;
        values.reserve(value.size());
        std::transform(std::begin(value), std::end(value),
                       std::back_inserter(values),
                       [] (char c) { return std::string(1, c); });
        m_choices = std::move(values);
        return *this;
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choices value
     *
     *  \return Current argument reference
     */
    inline Argument& choices(std::vector<std::string> const& value)
    {
        if (!(action() & (detail::_store_action
                          | Action::BooleanOptionalAction))) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        std::vector<std::string> values;
        values.reserve(value.size());
        for (auto const& str : value) {
            auto param = detail::_trim_copy(str);
            if (!param.empty()) {
                values.emplace_back(std::move(param));
            }
        }
        m_choices = std::move(values);
        return *this;
    }

    /*!
     *  \brief Set 'required' value for optional arguments
     *
     *  \param value Required flag
     *
     *  \return Current argument reference
     */
    inline Argument& required(bool value)
    {
        if (m_type == Positional) {
            throw
            TypeError("'required' is an invalid argument for positionals");
        }
        m_required = value;
        return *this;
    }

    /*!
     *  \brief Set argument 'help' message
     *
     *  \param value Help message
     *
     *  \return Current argument reference
     */
    inline Argument& help(std::string const& value)
    {
        m_help = value;
        m_help_type.clear();
        return *this;
    }

    /*!
     *  \brief Suppress argument 'help' message
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument reference
     */
    inline Argument& help(_SUPPRESS value)
    {
        if (value != argparse::SUPPRESS) {
            throw TypeError("got an unexpected keyword argument 'help'");
        }
        m_help_type = value;
        return *this;
    }

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar value
     *
     *  \return Current argument reference
     */
    template <typename = void>
    inline Argument& metavar(std::string const& value)
    {
        if (!(action() & (detail::_store_const_action
                          | Action::BooleanOptionalAction))) {
            throw TypeError("got an unexpected keyword argument 'metavar'");
        }
        m_metavar = { detail::_trim_copy(value) };
        return *this;
    }

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar value
     *
     *  \return Current argument reference
     */
    inline Argument& metavar(std::vector<std::string> const& values)
    {
        if (!(action() & (detail::_store_const_action
                          | Action::BooleanOptionalAction))) {
            throw TypeError("got an unexpected keyword argument 'metavar'");
        }
        auto _values = values;
        for (auto& value : _values) {
            detail::_trim(value);
        }
        m_metavar = std::move(_values);
        return *this;
    }

    /*!
     *  \brief Set argument 'dest' value for optional arguments
     *
     *  \param value Destination value
     *
     *  \return Current argument reference
     */
    inline Argument& dest(std::string const& value)
    {
        if (m_type == Positional && !m_flags.empty()) {
            throw ValueError("dest supplied twice for positional argument");
        }
        m_dest.front() = detail::_format_name(value);
        return *this;
    }

    /*!
     *  \brief Set argument 'version' for arguments with 'version' action
     *
     *  \param value Version value
     *
     *  \return Current argument reference
     */
    inline Argument& version(std::string const& value)
    {
        if (action() == Action::version) {
            m_version = detail::_trim_copy(value);
        } else {
            throw TypeError("got an unexpected keyword argument 'version'");
        }
        return *this;
    }

    /*!
     *  \brief Set argument 'handle' value.
     *  Called when the argument is present and passed the value of the argument
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    inline Argument& handle(std::function<void(std::string const&)> func)
    {
        if (action() & (Action::version | Action::help)) {
            throw TypeError("got an unexpected keyword argument 'handle'");
        }
        m_handle = func;
        return *this;
    }

    /*!
     *  \brief Set argument 'handle' value.
     *  Called when the argument is present
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    inline Argument& handle(std::function<void()> func)
    {
        if (action() & (Action::version | Action::help)) {
            throw TypeError("got an unexpected keyword argument 'handle'");
        }
        m_handle = [func] (std::string const&) { func(); };
        return *this;
    }

    /*!
     *  \brief Get argument flags values
     *
     *  \return Argument flags values
     */
    inline std::vector<std::string> const& flags() const _ARGPARSE_NOEXCEPT
    {
        return m_all_flags;
    }

    /*!
     *  \brief Get argument 'action' value (default: argparse::store)
     *
     *  \return Argument 'action' value
     */
    inline Action action() const _ARGPARSE_NOEXCEPT
    {
        return m_action;
    }

    /*!
     *  \brief Get argument 'nargs' value
     *
     *  \return Argument 'nargs' value
     */
    inline std::string const& nargs() const _ARGPARSE_NOEXCEPT
    {
        return m_nargs_str;
    }

    /*!
     *  \brief Get argument 'const' value
     *
     *  \return Argument 'const' value
     */
    inline std::string const& const_value() const _ARGPARSE_NOEXCEPT
    {
        return m_const();
    }

    /*!
     *  \brief Get argument 'default' value
     *
     *  \return Argument 'default' value
     */
    inline std::string const& default_value() const _ARGPARSE_NOEXCEPT
    {
        return m_default();
    }

    /*!
     *  \brief Get argument 'implicit' value
     *
     *  \return Argument 'implicit' value
     */
    inline std::string const& implicit_value() const _ARGPARSE_NOEXCEPT
    {
        return m_implicit();
    }

    /*!
     *  \brief Get argument 'type' name (for MetavarTypeHelpFormatter)
     *
     *  \return Argument 'type' name
     */
    inline std::string const& type_name() const _ARGPARSE_NOEXCEPT
    {
        return m_type_name();
    }

    /*!
     *  \brief Get argument 'choices' value
     *
     *  \return Argument 'choices' value
     */
    inline std::vector<std::string> const& choices() const _ARGPARSE_NOEXCEPT
    {
        return m_choices();
    }

    /*!
     *  \brief Get argument 'required' value
     *
     *  \return Argument 'required' value
     */
    inline bool required() const _ARGPARSE_NOEXCEPT
    {
        return m_required();
    }

    /*!
     *  \brief Get argument 'help' message
     *
     *  \return Argument 'help' message
     */
    inline std::string const& help() const _ARGPARSE_NOEXCEPT
    {
        return m_help;
    }

    /*!
     *  \brief Get argument 'metavar' value
     *
     *  \return Argument 'metavar' value
     */
    inline std::string metavar() const
    {
        return detail::_vector_to_string(m_metavar());
    }

    /*!
     *  \brief Get argument 'dest' value
     *
     *  \return Argument 'dest' value
     */
    inline std::string const& dest() const _ARGPARSE_NOEXCEPT
    {
        return m_dest.front();
    }

    /*!
     *  \brief Get argument 'version' value
     *
     *  \return Argument 'version' value
     */
    inline std::string const& version() const _ARGPARSE_NOEXCEPT
    {
        return m_version();
    }

private:
    inline void handle(std::string const& str) const
    {
        if (m_handle) {
            m_handle(detail::_remove_quotes(str));
        }
    }

    inline void validate() const
    {
        if (m_type == Positional && m_flags.empty() && dest().empty()) {
            throw TypeError("missing 1 required positional argument: 'dest'");
        }
        if (std::any_of(std::begin(m_flags), std::end(m_flags),
                        [] (std::string const& flag)
        { return flag == detail::_pseudo_arg; }) && dest().empty()) {
            throw ValueError("dest= is required for options like '--'");
        }
    }

    inline void prepare_action(Action value)
    {
        if (action() & (Action::version | Action::help)) {
            m_handle = nullptr;
        }
        if (!(value & detail::_store_const_action)) {
            m_metavar.clear();
        }
        if (m_type == Optional && value == Action::BooleanOptionalAction) {
            make_no_flags();
            if (m_post_trigger) {
                m_post_trigger(this);
            }
        } else {
            m_all_flags = m_flags;
        }
    }

    inline void make_no_flags()
    {
        m_all_flags.clear();
        for (auto const& flag : m_flags) {
            m_all_flags.push_back(flag);
            m_all_flags.push_back(detail::_make_no_flag(flag));
        }
    }

    inline std::string usage(_HelpFormatter const& formatter) const
    {
        std::string res;
        if (m_type == Optional) {
            if (action() == Action::BooleanOptionalAction) {
                for (auto const& flag : flags()) {
                    if (!res.empty()) {
                        res += " | ";
                    }
                    res += flag;
                }
            } else {
                res += m_flags.front();
            }
        }
        if (action() & (detail::_store_action | Action::append_const)) {
            res += get_nargs_suffix(formatter);
        }
        return res;
    }

    inline std::string flags_to_string(_HelpFormatter const& formatter) const
    {
        std::string res;
        if (m_type == Optional) {
            for (auto const& flag : flags()) {
                if (!res.empty()) {
                    res += ", ";
                }
                res += flag;
                if (action() & (detail::_store_action | Action::append_const)) {
                    res += get_nargs_suffix(formatter);
                }
            }
        } else {
            res += detail::_vector_to_string(get_argument_name(formatter));
        }
        return res;
    }

    inline std::string get_default() const
    {
        if (!m_default.has_value() && (action() & detail::_bool_action)) {
            return detail::_bool_to_string(m_default());
        } else {
            return (m_default.has_value() || !m_default().empty()) ? m_default()
                                                                   : "None";
        }
    }

    inline std::string print(_HelpFormatter const& formatter,
                             std::size_t limit,
                             std::size_t width) const
    {
        return detail::_help_formatter(
                            "  " + flags_to_string(formatter),
                            formatter,
                            detail::_replace(formatter._get_help_string(this),
                                             "%(default)s", get_default()),
                            width,
                            std::string(limit, detail::_space));
    }

    std::string get_nargs_suffix(_HelpFormatter const& formatter) const
    {
        auto names = get_argument_name(formatter);
        if (names.size() > 1
                && (m_nargs != NARGS_NUM || names.size() != m_num_args)) {
            throw TypeError("length of metavar tuple does not match nargs");
        }
        if (names.size() == 1
                && m_nargs == NARGS_NUM && names.size() != m_num_args) {
            names.resize(m_num_args, names.front());
        }
        auto const name = detail::_vector_to_string(names);
        std::string res;
        if (m_type == Optional && !name.empty()) {
            res += detail::_spaces;
        }
        switch (m_nargs) {
            case ZERO_OR_ONE :
                res += "[" + name + "]";
                break;
            case ONE_OR_MORE :
                res += name + detail::_spaces;
                // fallthrough
            case ZERO_OR_MORE :
                res += "[" + name + " ...]";
                break;
            case NARGS_NUM :
                res += name;
                break;
            case REMAINDER :
                res += "...";
                break;
            default :
                res += name;
                break;
        }
        return res;
    }

    inline std::vector<std::string>
    get_argument_name(_HelpFormatter const& formatter) const
    {
        if (m_metavar.has_value()) {
            return m_metavar();
        }
        if (m_choices.has_value()) {
            return { "{" + detail::_vector_to_string(choices(), ",") + "}" };
        }
        return { m_type == Optional
                    ? formatter._get_default_metavar_for_optional(this)
                    : formatter._get_default_metavar_for_positional(this) };
    }

    inline std::vector<std::string> const&
    get_argument_flags() const _ARGPARSE_NOEXCEPT
    {
        return dest().empty() ? m_flags : m_dest;
    }

    inline void resolve_conflict_flags(std::vector<std::string> const& flags)
    {
        detail::_resolve_conflict(flags, m_flags);
        detail::_resolve_conflict(flags, m_all_flags);
    }

    inline std::string error_nargs(std::string const& arg) const
    {
        switch (m_nargs) {
            case NARGS_DEF :
                return "argument " + arg + ": expected one argument";
            case ONE_OR_MORE :
                return "argument " + arg + ": expected at least one argument";
            case NARGS_NUM :
                return "argument " + arg + ": expected " + nargs()
                        + " arguments";
            default :
                return std::string();
        }
    }

    inline bool is_suppressed() const _ARGPARSE_NOEXCEPT
    {
        return m_default_type == argparse::SUPPRESS && !m_default.has_value();
    }

    inline bool operator ==(Argument const& rhs) const
    {
        return m_flags == rhs.m_flags
                && m_name == rhs.m_name
                && m_type == rhs.m_type
                && m_action == rhs.m_action
                && m_const == rhs.m_const
                && dest() == rhs.dest();
    }

    inline bool operator ==(std::string const& rhs) const
    {
        return !dest().empty() ? dest() == rhs
                               : detail::_is_value_exists(rhs, m_flags);
    }

    std::vector<std::string> m_flags;
    std::vector<std::string> m_all_flags;
    std::string m_name;
    Action      m_action;
    Type        m_type;
    detail::Value<_SUPPRESS> m_default_type;
    detail::Value<_SUPPRESS> m_help_type;
    Nargs       m_nargs;
    std::size_t m_num_args;
    std::string m_nargs_str;
    detail::Value<std::string> m_const;
    detail::Value<std::string> m_default;
    detail::Value<std::string> m_implicit;
    detail::Value<std::string> m_type_name;
    detail::Value<std::vector<std::string> > m_choices;
    detail::Value<bool> m_required;
    std::string m_help;
    detail::Value<std::vector<std::string> > m_metavar;
    std::vector<std::string> m_dest;
    detail::Value<std::string> m_version;
    std::function<void(std::string const&)> m_handle;
    std::function<void(Argument const*)> m_post_trigger;
};

/*!
 *  \brief Formatter for generating usage messages and argument help strings
 */
_ARGPARSE_EXPORT class HelpFormatter
{
public:
    virtual ~HelpFormatter() = default;

    virtual std::string
    (*_fill_text() const) (std::string const&, std::size_t, std::string const&)
    {
        return _fill_text_s;
    }

    virtual std::string
    (*_get_default_metavar_for_optional() const) (Argument const*)
    {
        return _get_default_metavar_for_optional_s;
    }

    virtual std::string
    (*_get_default_metavar_for_positional() const) (Argument const*)
    {
        return _get_default_metavar_for_positional_s;
    }

    virtual std::string
    (*_get_help_string() const) (Argument const*)
    {
        return _get_help_string_s;
    }

    virtual std::vector<std::string>
    (*_split_lines() const) (std::string const&, std::size_t)
    {
        return _split_lines_s;
    }

protected:
    static std::string
    _fill_text_s(std::string const& text, std::size_t width,
                 std::string const& indent)
    {
        std::vector<std::string> result;
        std::string value;
        auto lines = _split_lines_s(text, width - indent.size());
        for (auto const& line : lines) {
            if (value.size() < indent.size()) {
                value.resize(indent.size(), detail::_space);
            }
            value += line;
            detail::_store_value_to(value, result, true);
        }
        detail::_store_value_to(value, result);
        return detail::_vector_to_string(result, "\n");
    }

    static std::string
    _get_default_metavar_for_optional_s(Argument const* action)
    {
        return detail::_to_upper(action->dest().empty() ? action->m_name
                                                        : action->dest());
    }

    static std::string
    _get_default_metavar_for_positional_s(Argument const* action)
    {
        return action->dest().empty() ? action->m_name : action->dest();
    }

    static std::string
    _get_help_string_s(Argument const* action)
    {
        return action->help();
    }

    static std::vector<std::string>
    _split_lines_s(std::string const& text, std::size_t width)
    {
        std::string value;
        std::vector<std::string> result;
        auto _func = [width, &result, &value] (std::string const& str)
        {
            if (value.size() + 1 + str.size() > width) {
                detail::_store_value_to(value, result);
            }
            if (!value.empty()) {
                value += detail::_spaces;
            }
            value += str;
        };
        auto split_str = detail::_split_whitespace(text);
        for (auto const& str : split_str) {
            _func(str);
        }
        detail::_store_value_to(value, result);
        return result;
    }
};

/*!
 *  \brief Help message formatter which retains any formatting in descriptions
 */
_ARGPARSE_EXPORT
class _RawDescriptionHelpFormatter : public HelpFormatter
{
public:
    virtual ~_RawDescriptionHelpFormatter() = default;

    virtual std::string
    (*_fill_text() const) (std::string const&, std::size_t,
                           std::string const&) override
    {
        return _fill_text_s;
    }

protected:
    static std::string
    _fill_text_s(std::string const& text, std::size_t width,
                 std::string const& indent)
    {
        std::vector<std::string> result;
        std::string value;
        auto lines = _split_lines_s(text, width - indent.size());
        for (auto const& line : lines) {
            if (value.size() < indent.size()) {
                value.resize(indent.size(), detail::_space);
            }
            value += line;
            detail::_store_value_to(value, result, true);
        }
        detail::_store_value_to(value, result);
        return detail::_vector_to_string(result, "\n");
    }

    static std::vector<std::string>
    _split_lines_s(std::string const& text, std::size_t width)
    {
        auto body = detail::_replace(text, '\t', " ");
        std::string value;
        std::vector<std::string> result;
        auto _func = [width, &result, &value] (std::string const& str)
        {
            if (value.size() + 1 + str.size() > width) {
                detail::_store_value_to(value, result);
            }
            value += str;
        };
        auto split_str = detail::_split(body, '\n', true);
        for (auto const& str : split_str) {
            if (str.empty()) {
                detail::_store_value_to(value, result, true);
            } else {
                auto sub_split_str = detail::_split(str, detail::_space,
                                                    true, true);
                for (auto const& sub : sub_split_str) {
                    _func(sub);
                }
                detail::_store_value_to(value, result);
            }
        }
        detail::_store_value_to(value, result);
        return result;
    }
} _ARGPARSE_INLINE_VARIABLE RawDescriptionHelpFormatter;

/*!
 *  \brief Help message formatter which retains formatting of all help text
 */
_ARGPARSE_EXPORT
class _RawTextHelpFormatter : public _RawDescriptionHelpFormatter
{
public:
    virtual ~_RawTextHelpFormatter() = default;

    virtual std::vector<std::string>
    (*_split_lines() const) (std::string const&, std::size_t) override
    {
        return _RawDescriptionHelpFormatter::_split_lines_s;
    }
} _ARGPARSE_INLINE_VARIABLE RawTextHelpFormatter;

/*!
 *  \brief Help message formatter which adds default values to argument help
 */
_ARGPARSE_EXPORT
class _ArgumentDefaultsHelpFormatter : public HelpFormatter
{
public:
    virtual ~_ArgumentDefaultsHelpFormatter() = default;

    virtual std::string
    (*_get_help_string() const) (Argument const*) override
    {
        return _get_help_string_s;
    }

protected:
    static std::string
    _get_help_string_s(Argument const* action)
    {
        auto help = action->help();
        if (!help.empty()
                && !detail::_contains_substr(help, "%(default)s")
                && !action->is_suppressed()) {
            if ((action->m_type == Argument::Optional
                 || (action->m_nargs & (Argument::ZERO_OR_ONE
                                        | Argument::ZERO_OR_MORE)))
                    && !(action->action() & (Action::help | Action::version))) {
                help += " (default: %(default)s)";
            }
        }
        return help;
    }
} _ARGPARSE_INLINE_VARIABLE ArgumentDefaultsHelpFormatter;

/*!
 *  \brief Help message formatter which uses the argument 'type' as the default
 *  metavar value (instead of the argument 'dest')
 */
_ARGPARSE_EXPORT
class _MetavarTypeHelpFormatter : public HelpFormatter
{
public:
    virtual ~_MetavarTypeHelpFormatter() = default;

    virtual std::string
    (*_get_default_metavar_for_optional() const) (Argument const*) override
    {
        return _get_default_metavar_for_optional_s;
    }

    virtual std::string
    (*_get_default_metavar_for_positional() const) (Argument const*) override
    {
        return _get_default_metavar_for_positional_s;
    }

protected:
    static std::string
    _get_default_metavar_for_optional_s(Argument const* action)
    {
        if (!action->type_name().empty()) {
            return action->type_name();
        }
        return HelpFormatter::_get_default_metavar_for_optional_s(action);
    }

    static std::string
    _get_default_metavar_for_positional_s(Argument const* action)
    {
        if (!action->type_name().empty()) {
            return action->type_name();
        }
        return HelpFormatter::_get_default_metavar_for_positional_s(action);
    }
} _ARGPARSE_INLINE_VARIABLE MetavarTypeHelpFormatter;

/*!
 *  \brief Group class
 */
class _Group
{
    friend class ArgumentParser;

protected:
    explicit
    _Group(std::string const& title, std::string const& description)
        : m_title(title),
          m_description(description)
    { }

public:
    /*!
     *  \brief Destroy group
     */
    virtual ~_Group() _ARGPARSE_NOEXCEPT = default;

    /*!
     *  \brief Get group 'title' value
     *
     *  \return Group 'title' value
     */
    inline std::string const& title() const _ARGPARSE_NOEXCEPT
    {
        return m_title;
    }

    /*!
     *  \brief Get group 'description' value
     *
     *  \return Group 'description' value
     */
    inline std::string const& description() const _ARGPARSE_NOEXCEPT
    {
        return m_description;
    }

protected:
    virtual void limit_help_flags(_HelpFormatter const& formatter,
                                  std::size_t& limit)                 const = 0;
    virtual void print_help(std::ostream& os,
                            _HelpFormatter const& formatter,
                            std::string const& prog,
                            std::size_t limit,
                            std::size_t width)                        const = 0;

    std::string m_title;
    std::string m_description;
};

/*!
 *  \brief ArgumentData class
 */
class _ArgumentData
{
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class _BaseArgumentGroup;
    friend class _BaseParser;
    friend class MutuallyExclusiveGroup;

    _ArgumentData()
        : m_conflict_handler(),
          m_arguments(),
          m_optional(),
          m_positional(),
          m_add_help(false),
          m_help_added(false)
    { }

protected:
    typedef std::shared_ptr<Argument> pArgument;

public:
    /*!
     *  \brief Destroy argument data
     */
    virtual ~_ArgumentData() _ARGPARSE_NOEXCEPT = default;

protected:
    void update_help(bool add_help, std::string const& prefix_chars)
    {
        m_add_help = add_help;
        if (m_help_added) {
            m_arguments.pop_front();
            m_optional.pop_front();
            m_help_added = false;
        }
        if (m_add_help) {
            auto help_flags = detail::_help_flags(prefix_chars);
            if (m_conflict_handler == "resolve") {
                for (auto const& pair : m_optional) {
                    detail::_resolve_conflict(pair.first->flags(), help_flags);
                }
            }
            if (!help_flags.empty()) {
                auto help = Argument::make_argument(
                            std::move(help_flags), "help", Argument::Optional);
                help->help("show this help message and exit")
                        .action(Action::help);
                m_arguments.push_front(std::move(help));
                m_optional
                        .push_front(std::make_pair(m_arguments.front(), false));
                m_help_added = true;
            }
        }
    }

    inline std::vector<pArgument>
    get_arguments(bool add_suppress) const
    {
        std::vector<pArgument> result;
        result.reserve(m_arguments.size());
        for (auto const& arg : m_arguments) {
            if ((add_suppress || !arg->m_help_type.has_value())
                    && (arg->m_type != Argument::Optional
                        || !arg->flags().empty())) {
                result.push_back(arg);
            }
        }
        return result;
    }

    inline std::vector<pArgument>
    get_optional(bool add_suppress, bool add_group) const
    {
        std::vector<pArgument> result;
        result.reserve(m_optional.size());
        for (auto const& pair : m_optional) {
            if ((add_suppress || !pair.first->m_help_type.has_value())
                    && (add_group || !pair.second)
                    && !pair.first->flags().empty()) {
                result.push_back(pair.first);
            }
        }
        return result;
    }

    inline std::vector<pArgument>
    get_positional(bool add_suppress, bool add_group) const
    {
        std::vector<pArgument> result;
        result.reserve(m_positional.size());
        for (auto const& pair : m_positional) {
            if ((add_suppress || !pair.first->m_help_type.has_value())
                    && (add_group || !pair.second)) {
                result.push_back(pair.first);
            }
        }
        return result;
    }

    static void update_flag_name(std::vector<std::string>& flags,
                                 std::string const& prefix_chars,
                                 bool is_optional,
                                 std::string& flag_name, std::size_t& prefixes)
    {
        auto _update_flag_name = []
                (std::string const& arg, std::string& flag, std::size_t& count)
        {
            auto name = detail::_flag_name(arg);
            std::size_t count_prefixes = arg.size() - name.size();
            if (count < count_prefixes
                    || (count == count_prefixes
                        && count > 1
                        && flag.size() < name.size())) {
                count = count_prefixes;
                flag = std::move(name);
            }
        };
        if (is_optional) {
            _update_flag_name(flag_name, flag_name, prefixes);
        } else if (flags.size() > 1) {
            // no positional multiflag
            throw ValueError("invalid option string " + flags.front()
                             + ": must starts with a character '"
                             + prefix_chars + "'");
        }
        for (std::size_t i = 1; i < flags.size(); ++i) {
            // check arguments
            auto& flag = flags.at(i);
            flag = detail::_format_name(flag);
            if (flag.empty()) {
                throw IndexError("string index out of range");
            }
            if (!detail::_is_value_exists(flag.front(), prefix_chars)) {
                // no positional and optional args
                throw ValueError("invalid option string " + flag
                                 + ": must starts with a character '"
                                 + prefix_chars + "'");
            }
            _update_flag_name(flag, flag_name, prefixes);
        }
    }

    static void check_flag_name(std::string const& flag)
    {
        if (flag.empty()) {
            throw IndexError("string index out of range");
        }
    }

    void check_conflicting_option(Argument const* arg, pArgument& option) const
    {
        if (option.get() == arg) {
            return;
        }
        auto _check_conflict = [this, &arg] (std::vector<std::string>& flags)
        {
            std::vector<std::string> conflict_options;
            for (auto const& flag : arg->flags()) {
                auto it = std::find(std::begin(flags), std::end(flags), flag);
                if (it != std::end(flags)) {
                    if (m_conflict_handler == "resolve") {
                        flags.erase(it);
                    } else {
                        conflict_options.push_back(flag);
                    }
                }
            }
            if (conflict_options.size() == 1) {
                throw ArgumentError(
                            "argument "
                            + detail::_vector_to_string(arg->flags(), "/")
                            + ": conflicting option string: "
                            + conflict_options.front());
            } else if (conflict_options.size() > 1) {
                throw ArgumentError(
                            "argument "
                           + detail::_vector_to_string(arg->flags(), "/")
                           + ": conflicting option strings: "
                           + detail::_vector_to_string(conflict_options, ", "));
            }
        };
        _check_conflict(option->m_all_flags);
        _check_conflict(option->m_flags);
    }

    inline void check_conflict_arg(Argument const* arg)
    {
        for (auto& opt : m_optional) {
            check_conflicting_option(arg, opt.first);
        }
    }

    inline void merge_arguments(_ArgumentData const& data)
    {
        for (auto const& arg : data.m_optional) {
            for (auto& opt : m_optional) {
                check_conflicting_option(arg.first.get(), opt.first);
            }
            m_optional.push_back(arg);
        }
        for (auto const& arg : data.m_positional) {
            m_positional.push_back(arg);
        }
        for (auto const& arg : data.m_arguments) {
            m_arguments.push_back(arg);
        }
    }

    void create_argument(std::vector<std::string> flags,
                         std::string const& prefix_chars)
    {
        if (flags.empty()) {
            auto flag = std::string();
            auto arg = Argument::make_argument(std::move(flags),
                                               std::move(flag),
                                               Argument::Positional);
            m_arguments.emplace_back(std::move(arg));
            return;
        }
        flags.front() = detail::_format_name(flags.front());
        auto flag = flags.front();
        check_flag_name(flag);
        std::size_t prefixes = 0;
        bool is_optional = detail::_is_value_exists(flag.front(), prefix_chars);
        update_flag_name(flags, prefix_chars, is_optional, flag, prefixes);
        auto arg = Argument::make_argument(
                    std::move(flags), std::move(flag),
                    is_optional ? Argument::Optional : Argument::Positional);
        if (is_optional) {
            check_conflict_arg(arg.get());
        }
        m_arguments.emplace_back(std::move(arg));
        if (is_optional) {
            m_arguments.back()->m_post_trigger = [this] (Argument const* arg)
            {
                check_conflict_arg(arg);
            };
        }
    }

    void validate_argument(Argument const& ar, std::string const& prefix_chars)
    {
        auto arg = ar;
        auto& flags = arg.m_flags;
        bool is_optional = false;
        if (flags.empty()) {
            arg.m_name = arg.dest();
        } else {
            flags.front() = detail::_format_name(flags.front());
            auto flag = flags.front();
            check_flag_name(flag);
            std::size_t prefixes = 0;
            is_optional = detail::_is_value_exists(flag.front(), prefix_chars);
            update_flag_name(flags, prefix_chars, is_optional, flag, prefixes);
            arg.m_name = flag;
        }
        arg.m_type = is_optional ? Argument::Optional : Argument::Positional;
        // check
        if (!is_optional) {
            if (arg.dest().empty() && flags.empty()) {
                throw
                TypeError("missing 1 required positional argument: 'dest'");
            }
            if (arg.action() & (Action::version | Action::help)) {
                // version and help actions cannot be positional
                throw
                TypeError("got an unexpected keyword argument 'required'");
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
        } else {
            if (arg.action() == Action::BooleanOptionalAction) {
                arg.make_no_flags();
            }
            check_conflict_arg(&arg);
        }
        m_arguments.emplace_back(std::make_shared<Argument>(arg));
    }

    std::string m_conflict_handler;
    std::deque<pArgument> m_arguments;
    std::deque<std::pair<pArgument, bool> > m_optional;
    std::deque<std::pair<pArgument, bool> > m_positional;
    bool m_add_help;
    bool m_help_added;
};

/*!
 *  \brief BaseArgumentGroup class
 */
class _BaseArgumentGroup
{
protected:
    _BaseArgumentGroup(std::string& prefix_chars,
                       _ArgumentData& parent_data,
                       detail::Value<std::string>& argument_default,
                       detail::Value<_SUPPRESS>& argument_default_type,
                       bool is_mutex_group)
        : m_data(),
          m_prefix_chars(prefix_chars),
          m_parent_data(parent_data),
          m_argument_default(argument_default),
          m_argument_default_type(argument_default_type),
          m_is_mutex_group(is_mutex_group)
    { }

public:
    /*!
     *  \brief Create base argument group object from another argument group
     *
     *  \param orig Base argument group object to copy
     *
     *  \return Base rgument group object
     */
    _BaseArgumentGroup(_BaseArgumentGroup const& orig)
        : m_data(orig.m_data),
          m_prefix_chars(orig.m_prefix_chars),
          m_parent_data(orig.m_parent_data),
          m_argument_default(orig.m_argument_default),
          m_argument_default_type(orig.m_argument_default_type),
          m_is_mutex_group(orig.m_is_mutex_group)
    { }

    /*!
     *  \brief Destroy basea argument group object
     */
    virtual ~_BaseArgumentGroup() _ARGPARSE_NOEXCEPT = default;

    /*!
     *  \brief Copy base argument group object from another argument group
     *
     *  \param rhs Base argument group object to copy
     *
     *  \return Current base argument group reference
     */
    _BaseArgumentGroup& operator =(_BaseArgumentGroup const& rhs)
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

    /*!
     *  \brief Add argument with flags
     *
     *  \param args Flag values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    Argument& add_argument(Args... args)
    {
        return add_argument(std::vector<std::string>{ args... });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::vector<std::string> const& flags)
    {
        m_data.create_argument(flags, m_prefix_chars);
        process_add_argument();
        return *m_data.m_arguments.back();
    }

protected:
    inline void process_add_argument()
    {
        bool optional = m_data.m_arguments.back()->m_type == Argument::Optional;
        if (!optional) {
            if (m_is_mutex_group) {
                m_data.m_arguments.pop_back();
                throw
                ValueError("mutually exclusive arguments must be optional");
            }
        } else if (m_parent_data.m_conflict_handler == "resolve") {
            for (auto& arg : m_parent_data.m_optional) {
                arg.first->resolve_conflict_flags(
                            m_data.m_arguments.back()->flags());
            }
        }
        if (m_argument_default.has_value()
                && !m_data.m_arguments.back()->m_default.has_value()
                && !m_data.m_arguments.back()->m_default_type.has_value()) {
            m_data.m_arguments.back()->default_value(m_argument_default());
        }
        if (m_argument_default_type.has_value()) {
            m_data.m_arguments.back()->default_value(m_argument_default_type());
        }
        m_parent_data.m_arguments.push_back(m_data.m_arguments.back());
        (optional ? m_data.m_optional : m_data.m_positional)
                .push_back(std::make_pair(m_data.m_arguments.back(),
                                          !m_is_mutex_group));
        (optional ? m_parent_data.m_optional : m_parent_data.m_positional)
                .push_back(std::make_pair(m_data.m_arguments.back(),
                                          !m_is_mutex_group));
    }

    _ArgumentData m_data;
    std::string& m_prefix_chars;
    _ArgumentData& m_parent_data;
    detail::Value<std::string>& m_argument_default;
    detail::Value<_SUPPRESS>& m_argument_default_type;

private:
    bool m_is_mutex_group;
};

/*!
 *  \brief ArgumentGroup class
 */
_ARGPARSE_EXPORT class ArgumentGroup : public _Group, public _BaseArgumentGroup
{
    friend class ArgumentParser;

    explicit
    ArgumentGroup(std::string const& title,
                  std::string const& description,
                  std::string& prefix_chars,
                  _ArgumentData& parent_data,
                  detail::Value<std::string>& argument_default,
                  detail::Value<_SUPPRESS>& argument_default_type)
        : _Group(title, description),
          _BaseArgumentGroup(prefix_chars, parent_data,
                             argument_default, argument_default_type, false)
    { }

    static inline std::shared_ptr<ArgumentGroup>
    make_argument_group(std::string const& title,
                        std::string const& description,
                        std::string& prefix_chars,
                        _ArgumentData& parent_data,
                        detail::Value<std::string>& argument_default,
                        detail::Value<_SUPPRESS>& argument_default_type)
    {
        return std::make_shared<ArgumentGroup>
                (ArgumentGroup(title, description, prefix_chars, parent_data,
                               argument_default, argument_default_type));
    }

public:
    using _BaseArgumentGroup::add_argument;
    using _Group::title;
    using _Group::description;

    /*!
     *  \brief Create argument group object from another argument group
     *
     *  \param orig Argument group object to copy
     *
     *  \return Argument group object
     */
    ArgumentGroup(ArgumentGroup const& orig)
        : _Group(orig),
          _BaseArgumentGroup(orig)
    { }

    /*!
     *  \brief Copy argument group object from another argument group
     *
     *  \param rhs Argument group object to copy
     *
     *  \return Current argument group reference
     */
    ArgumentGroup& operator =(ArgumentGroup const& rhs)
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

    /*!
     *  \brief Set argument group 'title' value
     *
     *  \param value Title value
     *
     *  \return Current argument group reference
     */
    inline ArgumentGroup& title(std::string const& value)
    {
        m_title = detail::_trim_copy(value);
        return *this;
    }

    /*!
     *  \brief Set argument group 'description' value
     *
     *  \param param Description value
     *
     *  \return Current argument group reference
     */
    inline ArgumentGroup& description(std::string const& param)
    {
        m_description = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current argument group reference
     */
    template <typename = void>
    inline ArgumentGroup& add_argument(Argument const& argument)
    {
        m_data.validate_argument(Argument(argument), m_prefix_chars);
        process_add_argument();
        return *this;
    }

private:
    inline void
    limit_help_flags(_HelpFormatter const& formatter,
                     std::size_t& limit) const override
    {
        for (auto const& arg : m_data.m_arguments) {
            auto size = arg->flags_to_string(formatter).size();
            if (limit < size) {
                limit = size;
            }
        }
    }

    inline void print_help(std::ostream& os,
                           _HelpFormatter const& formatter,
                           std::string const& prog,
                           std::size_t limit,
                           std::size_t width) const override
    {
        if (!description().empty() || !m_data.m_arguments.empty()) {
            if (!title().empty()) {
                os << "\n" << title() << ":";
            }
            detail::_print_raw_text_formatter(
                        formatter,
                        detail::_replace(
                            description(), "%(prog)s", prog),
                        width, os, "\n", "  ");
            if (!m_data.m_arguments.empty()) {
                os << std::endl;
            }
            for (auto const& arg : m_data.m_arguments) {
                os << arg->print(formatter, limit, width) << std::endl;
            }
        }
    }
};

/*!
 *  \brief MutuallyExclusiveGroup class
 */
_ARGPARSE_EXPORT class MutuallyExclusiveGroup : public _BaseArgumentGroup
{
    friend class ArgumentParser;
    friend class _BaseParser;

    explicit
    MutuallyExclusiveGroup(std::string& prefix_chars,
                           _ArgumentData& parent_data,
                           detail::Value<std::string>& argument_default,
                           detail::Value<_SUPPRESS>& argument_default_type)
        : _BaseArgumentGroup(prefix_chars, parent_data,
                             argument_default, argument_default_type, true),
          m_required(false)
    { }

    static inline MutuallyExclusiveGroup
    make_mutex_group(std::string& prefix_chars,
                     _ArgumentData& parent_data,
                     detail::Value<std::string>& argument_default,
                     detail::Value<_SUPPRESS>& argument_default_type)
    {
        return MutuallyExclusiveGroup(prefix_chars, parent_data,
                                      argument_default, argument_default_type);
    }

public:
    using _BaseArgumentGroup::add_argument;

    /*!
     *  \brief Create mutually exclusive group object from another
     *   mutually exclusive group
     *
     *  \param orig Mutually exclusive group object to copy
     *
     *  \return Mutually exclusive group object
     */
    MutuallyExclusiveGroup(MutuallyExclusiveGroup const& orig)
        : _BaseArgumentGroup(orig),
          m_required(orig.m_required)
    { }

    /*!
     *  \brief Copy mutually exclusive group object from another
     *  mutually exclusive group
     *
     *  \param rhs Mutually exclusive group object to copy
     *
     *  \return Current mutually exclusive group reference
     */
    MutuallyExclusiveGroup& operator =(MutuallyExclusiveGroup const& rhs)
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

    /*!
     *  \brief Set mutually exclusive group 'required' value
     *
     *  \param value Required flag
     *
     *  \return Current mutually exclusive group reference
     */
    inline MutuallyExclusiveGroup& required(bool value) _ARGPARSE_NOEXCEPT
    {
        m_required = value;
        return *this;
    }

    /*!
     *  \brief Get mutually exclusive group 'required' value
     *
     *  \return Mutually exclusive group 'required' value
     */
    inline bool required() const _ARGPARSE_NOEXCEPT
    {
        return m_required;
    }

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current mutually exclusive group reference
     */
    template <typename = void>
    inline MutuallyExclusiveGroup& add_argument(Argument const& argument)
    {
        m_data.validate_argument(Argument(argument), m_prefix_chars);
        process_add_argument();
        return *this;
    }

private:
    inline std::string usage(_HelpFormatter const& formatter) const
    {
        std::string res;
        for (auto const& arg : m_data.m_arguments) {
            if (!res.empty()) {
                res += " | ";
            }
            res += arg->usage(formatter);
        }
        return res.empty() ? res
                           : (m_required ? "(" + res + ")" : "[" + res + "]");
    }

    bool m_required;
};

/*!
 *  \brief Object with parsed arguments
 */
_ARGPARSE_EXPORT class Namespace
{
    typedef std::shared_ptr<Argument> pArgument;

    class Storage
    {
        friend class ArgumentParser;
        friend class Namespace;

    public:
        class mapped_type
        {
        public:
            mapped_type()
                : m_exists(),
                  m_is_default(),
                  m_values(),
                  m_matrix()
            { }

            mapped_type(bool is_default, std::vector<std::string> const& values)
                : m_exists(true),
                  m_is_default(is_default),
                  m_values(values),
                  m_matrix({ m_values })
            { }

            inline void clear()
            {
                m_exists = false;
                m_is_default = false;
                m_values.clear();
                m_matrix.clear();
            }

            inline bool exists() const _ARGPARSE_NOEXCEPT
            {
                return m_exists;
            }

            inline void push_default(std::string const& value)
            {
                m_is_default = true;
                push_back(value);
            }

            inline bool is_default() const _ARGPARSE_NOEXCEPT
            {
                return m_is_default;
            }

            inline std::vector<std::string> const&
            operator ()() const _ARGPARSE_NOEXCEPT
            {
                return m_values;
            }

            inline std::size_t size() const _ARGPARSE_NOEXCEPT
            {
                return m_values.size();
            }

            inline bool empty() const _ARGPARSE_NOEXCEPT
            {
                return m_values.empty();
            }

            inline std::string const& front() const _ARGPARSE_NOEXCEPT
            {
                return m_values.front();
            }

            inline std::string const& at(std::size_t i) const
            {
                return m_values.at(i);
            }

            inline void emplace_back(std::string&& value)
            {
                m_exists = true;
                m_values.emplace_back(std::move(value));
                push_to_matrix();
            }

            inline void push_back(std::string const& value)
            {
                m_exists = true;
                m_values.push_back(value);
                push_to_matrix();
            }

            inline void push_values(std::vector<std::string> const& values)
            {
                m_exists = true;
                m_values.reserve(m_values.size() + values.size());
                m_values.insert(std::end(m_values),
                                std::begin(values), std::end(values));
                m_matrix.push_back(values);
            }

            inline std::vector<std::vector<std::string> > const&
            matrix() const _ARGPARSE_NOEXCEPT
            {
                return m_matrix;
            }

        private:
            inline void push_to_matrix()
            {
                m_matrix.push_back({ m_values.back() });
            }

            bool m_exists;
            bool m_is_default;
            std::vector<std::string> m_values;
            std::vector<std::vector<std::string> > m_matrix;
        };

        typedef pArgument                               key_type;
        typedef std::pair<key_type, mapped_type>        value_type;
        typedef std::deque<value_type>                  map_type;
        typedef map_type::iterator                      iterator;
        typedef map_type::const_iterator                const_iterator;

        Storage()
            : m_data(),
              m_conflict_arg()
        { }

        inline void
        force_add(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->action() & (Action::version | Action::help)) {
                return;
            }
            auto const& arg_flags = key->get_argument_flags();
            bool have_key = false;
            for (auto& pair : m_data) {
                if (key != pair.first) {
                    pair.first->resolve_conflict_flags(arg_flags);
                } else {
                    have_key = true;
                }
            }
            if (!have_key) {
                m_data.push_back(std::make_pair(key, value));
            }
        }

        template <class T>
        void force_add(T const& arguments)
        {
            for (auto const& arg : arguments) {
                force_add(arg);
            }
        }

        inline void
        try_add(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->action() & (Action::version | Action::help)) {
                return;
            }
            auto const& flag = conflict_arg(key);
            if (flag.empty()) {
                m_data.push_back(std::make_pair(key, value));
            }
        }

        template <class T>
        void try_add(T const& arguments)
        {
            for (auto const& arg : arguments) {
                try_add(arg);
            }
        }

        inline void
        create(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->action() & (Action::version | Action::help)) {
                return;
            }
            auto const& flag = conflict_arg(key);
            if (flag.empty()) {
                m_data.push_back(std::make_pair(key, value));
            } else {
                throw
                ArgumentError("argument "
                              + detail::_vector_to_string(key->m_flags, "/")
                              + ": conflicting dest string: " + flag);
            }
        }

        template <class T>
        void create(T const& arguments)
        {
            for (auto const& arg : arguments) {
                create(arg);
            }
        }

        inline void have_value(key_type const& arg)
        {
            if (arg->implicit_value().empty()) {
                at(arg).push_values({ });
            } else {
                at(arg).push_back(arg->implicit_value());
                arg->handle(arg->implicit_value());
            }
        }

        inline void store_value(key_type const& arg, std::string const& value)
        {
            at(arg).push_back(value);
            arg->handle(value);
        }

        inline void store_values(key_type const& arg,
                                 std::vector<std::string> const& values)
        {
            at(arg).push_values(values);
            for (auto const& value : values) {
                arg->handle(value);
            }
        }

        inline void store_default_value(key_type const& arg,
                                        std::string const& value)
        {
            if (arg->action()
                    & (Action::store | Action::BooleanOptionalAction)) {
                auto& arg_data = at(arg);
                if (arg_data.empty()) {
                    arg_data.push_default(value);
                    arg->handle(value);
                }
            }
        }

        inline bool self_value_stored(key_type const& arg)
        {
            if (arg->action() & (Action::store_const | detail::_bool_action)) {
                auto& arg_data = at(arg);
                if (arg_data.empty()) {
                    arg_data.push_back(arg->const_value());
                    arg->handle(arg->const_value());
                }
                return true;
            } else if (arg->action() == Action::append_const) {
                at(arg).push_back(arg->const_value());
                arg->handle(arg->const_value());
                return true;
            } else if (arg->action() == Action::count) {
                at(arg).emplace_back(std::string());
                arg->handle(std::string());
                return true;
            }
            return false;
        }

        inline bool exists_arg(std::string const& key) const
        {
            auto it = find_arg(key);
            return it != std::end(m_data);
        }

        inline bool exists(std::string const& key) const
        {
            auto it = find(key);
            return it != std::end(m_data);
        }

        inline bool exists(key_type const& key) const
        {
            auto it = find(key);
            return it != std::end(m_data);
        }

        inline value_type const& at(std::string const& key) const
        {
            auto it = find(key);
            if (it == std::end(m_data)) {
                throw std::logic_error("key '" + key + "' not found");
            }
            return *it;
        }

        inline mapped_type& at(key_type const& key)
        {
            auto it = find(key);
            if (it == std::end(m_data)) {
                throw std::logic_error("key '" + key->m_name + "' not found");
            }
            return it->second;
        }

        inline mapped_type const& at(key_type const& key) const
        {
            auto it = find(key);
            if (it == std::end(m_data)) {
                throw std::logic_error("key '" + key->m_name + "' not found");
            }
            return it->second;
        }

        inline iterator erase(iterator i)         { return m_data.erase(i); }

        inline iterator begin()_ARGPARSE_NOEXCEPT { return std::begin(m_data); }
        inline iterator end()  _ARGPARSE_NOEXCEPT { return std::end(m_data); }
        inline const_iterator
        begin()          const _ARGPARSE_NOEXCEPT { return std::begin(m_data); }
        inline const_iterator
        end()            const _ARGPARSE_NOEXCEPT { return std::end(m_data); }

    private:
        inline const_iterator find_arg(std::string const& key) const
        {
            auto it = find(key);
            if (it != end()) {
                return it;
            }
            for (it = begin(); it != end(); ++it) {
                if (it->first->m_type == Argument::Optional
                        && it->first->dest().empty()) {
                    if (std::any_of(std::begin(it->first->m_flags),
                                    std::end(it->first->m_flags),
                                    [&key] (std::string const& flag)
                    { return detail::_flag_name(flag) == key; })) {
                        return it;
                    }
                }
            }
            return end();
        }

        inline const_iterator find(std::string const& key) const
        {
            return std::find_if(std::begin(m_data), std::end(m_data),
                                [&key] (value_type const& pair)
            { return *(pair.first) == key; });
        }

        inline const_iterator find(key_type const& key) const
        {
            return std::find_if(std::begin(m_data), std::end(m_data),
                                [&key] (value_type const& pair)
            { return pair.first == key; });
        }

        inline iterator find(key_type const& key)
        {
            return std::find_if(std::begin(m_data), std::end(m_data),
                                [&key] (value_type const& pair)
            { return pair.first == key; });
        }

        inline std::string const&
        conflict_arg(key_type const& arg) const
        {
            auto const& arg_flags = arg->get_argument_flags();
            for (auto const& pair : m_data) {
                for (auto const& flag : pair.first->get_argument_flags()) {
                    if (detail::_is_value_exists(flag, arg_flags)) {
                        return flag;
                    }
                }
            }
            return m_conflict_arg;
        }

        map_type m_data;
        std::string m_conflict_arg;
    };

    friend class ArgumentParser;

    explicit
    Namespace(Storage const& storage = Storage())
        : m_storage(storage),
          m_unrecognized_args()
    { }

    explicit
    Namespace(Storage const& storage, std::vector<std::string> const& args)
        : m_storage(storage),
          m_unrecognized_args(args)
    { }

    explicit
    Namespace(Storage&& storage) _ARGPARSE_NOEXCEPT
        : m_storage(std::move(storage)),
          m_unrecognized_args()
    { }

    explicit
    Namespace(Storage&& storage,
              std::vector<std::string>&& args) _ARGPARSE_NOEXCEPT
        : m_storage(std::move(storage)),
          m_unrecognized_args(std::move(args))
    { }

public:
    /*!
     *  \brief Check if argument name exists and specified in parsed arguments
     *
     *  \param key Argument name
     *
     *  \return true if argument name exists and specified, otherwise false
     */
    inline bool exists(std::string const& key) const
    {
        auto it = storage().find_arg(key);
        if (it != storage().end()) {
            return !it->second.empty() || it->first->action() == Action::count;
        }
        return false;
    }

    /*!
     *  \brief Get parsed argument value for boolean, byte, floating point
     *  and string types.
     *  If argument not parsed, returns default value.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<std::is_constructible<std::string, T>::value
                            || std::is_floating_point<T>::value
                            || std::is_same<bool, T>::value
                            || detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        if (args.second.empty()) {
            return T();
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return to_type<T>(args.second.front());
    }

    /*!
     *  \brief Get parsed argument value for integer types.
     *  If argument not parsed, returns default value.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<std::is_integral<T>::value
                            && !std::is_same<bool, T>::value
                            && !detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == Action::count) {
            return T(args.second.size());
        }
        if (args.second.empty()) {
            return T();
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return to_type<T>(args.second.front());
    }

    /*!
     *  \brief Get parsed argument value std array type
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_array<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_vector<typename T::value_type>(args.second());
        T res{};
        if (res.size() != vector.size()) {
            std::cerr << "error: array size mismatch: " << res.size()
                      << ", expected " << vector.size() << std::endl;
        }
        auto size = res.size();
        if (size > vector.size()) {
            size = vector.size();
        }
        std::copy_n(std::begin(vector), size, std::begin(res));
        return res;
    }

    /*!
     *  \brief Get parsed argument value for std containers types
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_container<typename std::decay<T>::type>::value
        && !detail::is_stl_container_paired<typename std::decay<T>::type>::value
        && !detail::is_stl_container_tupled<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_vector<typename T::value_type>(args.second());
        return T(std::begin(vector), std::end(vector));
    }

    /*!
     *  \brief Get parsed argument value for paired container types
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_container_paired<typename std::decay<T>::type>::value,
    T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_paired_vector<
                typename T::value_type::first_type,
                typename T::value_type::second_type>(args.second(), delim);
        return T(std::begin(vector), std::end(vector));
    }

    /*!
     *  \brief Get parsed argument value for tupled container types
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value,
    T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_tupled_vector<
                typename T::value_type>(args.second(), delim);
        return T(std::begin(vector), std::end(vector));
    }

    /*!
     *  \brief Get parsed argument value for mapped types
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_map<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        T res{};
        auto vector = to_paired_vector<typename T::key_type,
                typename T::mapped_type>(args.second(), delim);
        for (auto const& pair : vector) {
            res.emplace(std::make_pair(pair.first, pair.second));
        }
        return res;
    }

    /*!
     *  \brief Get parsed argument value for 2D deque, list, vector of not queue
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() != Action::append
                || !(args.first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        T res{};
        for (auto const& vec : args.second.matrix()) {
            auto vector = to_vector<typename T::value_type::value_type>(vec);
            res.push_back(typename T::value_type(std::begin(vector),
                                                 std::end(vector)));
        }
        return res;
    }

    /*!
     *  \brief Get parsed argument value for 2D deque, list, vector of queue
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() != Action::append
                || !(args.first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        T res{};
        for (auto const& vec : args.second.matrix()) {
            auto vector = to_vector<typename T::value_type::value_type>(vec);
            res.push_back(typename T::value_type(
                              std::deque<typename T::value_type::value_type>(
                                  std::begin(vector), std::end(vector))));
        }
        return res;
    }

    /*!
     *  \brief Get parsed argument value for pair types
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_pair<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        if (args.second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args.second.size() != 2) {
                throw
                TypeError("invalid data for paired argument '" + key + "'");
            }
            return std::make_pair(
                        to_type<typename T::first_type>(args.second.front()),
                        to_type<typename T::second_type>(args.second.at(1)));
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return to_pair<typename T::first_type,
                typename T::second_type>(args.second.front(), delim);
    }

    /*!
     *  \brief Get parsed argument value for queue types
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_queue<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_vector<typename T::value_type>(args.second());
        return T(std::deque<typename T::value_type>(std::begin(vector),
                                                    std::end(vector)));
    }

    /*!
     *  \brief Get parsed argument value for tuple
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        if (args.second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            return to_tuple(detail::type_tag<T>{}, args.second());
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return to_tuple(detail::type_tag<T>{},
                        detail::_split(args.second.front(), delim));
    }

    /*!
     *  \brief Get parsed argument value for custom types
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        !std::is_constructible<std::string, T>::value
        && !std::is_floating_point<T>::value
        && !std::is_integral<T>::value
        && !std::is_same<bool, T>::value
        && !detail::is_byte_type<T>::value
        && !detail::is_stl_array<typename std::decay<T>::type>::value
        && !detail::is_stl_container<typename std::decay<T>::type>::value
        && !detail::is_stl_container_paired<typename std::decay<T>::type>::value
        && !detail::is_stl_container_tupled<typename std::decay<T>::type>::value
        && !detail::is_stl_map<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value
        && !detail::is_stl_pair<typename std::decay<T>::type>::value
        && !detail::is_stl_queue<typename std::decay<T>::type>::value
        && !detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        return to_type<T>(detail::_vector_to_string(args.second()));
    }

    /*!
     *  \brief Print namespace to output stream
     *
     *  \param os Output stream
     */
    inline void print(std::ostream& os = std::cout) const
    {
        os << to_string() << std::endl;
    }

    /*!
     *  \brief Get parsed argument value as args string
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value as args string
     */
    std::string to_args(std::string const& key) const
    {
        auto const& args = data(key);
        switch (args.first->action()) {
            case Action::store_const :
                if (args.second.empty()) {
                    return std::string();
                }
                if (args.second.size() != 1) {
                    throw TypeError("trying to get data from array argument '"
                                    + key + "'");
                }
                return detail::_have_quotes(args.second.front())
                        ? args.second.front()
                        : detail::_replace(args.second.front(),
                                           detail::_space, "\\ ");
            case Action::store_true :
            case Action::store_false :
                if (args.second.empty()) {
                    return detail::_bool_to_string(args.first->default_value());
                }
                if (args.second.size() != 1) {
                    throw TypeError("trying to get data from array argument '"
                                    + key + "'");
                }
                return detail::_bool_to_string(args.second.front());
            case Action::count :
                return std::to_string(args.second.size());
            case Action::store :
            case Action::append :
            case Action::append_const :
            case Action::extend :
                return detail::_vector_to_string(args.second(), detail::_spaces,
                                                 std::string(), true);
            case Action::BooleanOptionalAction :
                return boolean_option_to_args(key, args);
            default :
                throw ValueError("action not supported");
        }
    }

    /*!
     *  \brief Get parsed argument value as string
     *
     *  \param key Argument name
     *  \param quotes Value quotes
     *
     *  \return Parsed argument value as string
     */
    std::string to_string(std::string const& key,
                          std::string const& quotes = std::string()) const
    {
        auto const& args = data(key);
        switch (args.first->action()) {
            case Action::store_const :
                if (args.second.empty()) {
                    return std::string("None");
                }
                if (args.second.size() != 1) {
                    throw TypeError("trying to get data from array argument '"
                                    + key + "'");
                }
                return quotes + args.second.front() + quotes;
            case Action::store_true :
            case Action::store_false :
            case Action::BooleanOptionalAction :
                return boolean_option_to_string(key, args, quotes);
            case Action::count :
                if (args.second.empty()) {
                    return std::string("None");
                }
                return std::to_string(args.second.size());
            case Action::store :
            case Action::append :
            case Action::append_const :
            case Action::extend :
                return store_actions_to_string(args, quotes);
            default :
                throw ValueError("action not supported");
        }
    }

    /*!
     *  \brief Get namespace as string
     *
     *  \return Namespace as string
     */
    inline std::string to_string() const
    {
        std::string result;
        for (auto const& pair : storage()) {
            auto const& flags = pair.first->get_argument_flags();
            if (flags.empty()) {
                continue;
            }
            if (!result.empty()) {
                result += ", ";
            }
            auto const& name = !pair.first->dest().empty() ? pair.first->dest()
                                                           : pair.first->m_name;
            result += name + detail::_equals + to_string(flags.front(), "'");
        }
        if (!m_unrecognized_args.has_value()) {
            return "Namespace(" + result + ")";
        }
        auto unknown_args
                = detail::_vector_to_string(unrecognized_args(), ", ", "'");
        return "(Namespace(" + result + "), [" + unknown_args + "])";
    }

#ifdef _ARGPARSE_USE_OPTIONAL
    /*!
     *  \brief Try get parsed argument value for boolean, byte, floating point
     *  and string types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        std::is_constructible<std::string, T>::value
        || std::is_floating_point<T>::value
        || std::is_same<bool, T>::value
        || detail::is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || args->second.empty()
                || args->second.size() != 1
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return {};
        }
        return try_to_type<T>(args->second.front());
    }

    /*!
     *  \brief Try get parsed argument value for integer types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        std::is_integral<T>::value
        && !std::is_same<bool, T>::value
        && !detail::is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return {};
        }
        if (args->first->action() == Action::count) {
            return T(args->second.size());
        }
        if (args->second.empty() || args->second.size() != 1) {
            return {};
        }
        return try_to_type<T>(args->second.front());
    }

    /*!
     *  \brief Try get parsed argument value std array type.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_array<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
        if (!vector.operator bool()) {
            return {};
        }
        T res{};
        if (res.size() != vector->size()) {
            std::cerr << "error: array size mismatch: " << res.size()
                      << ", expected " << vector->size() << std::endl;
        }
        auto size = res.size();
        if (size > vector->size()) {
            size = vector->size();
        }
        std::copy_n(std::begin(vector.value()), size, std::begin(res));
        return res;
    }

    /*!
     *  \brief Try get parsed argument value for std containers types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
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
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
        if (!vector.operator bool()) {
            return {};
        }
        return T(std::begin(vector.value()), std::end(vector.value()));
    }

    /*!
     *  \brief Try get parsed argument value for paired container types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_container_paired<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        auto vector
                = try_to_paired_vector<
                typename T::value_type::first_type,
                typename T::value_type::second_type>(args->second(), delim);
        if (!vector.operator bool()) {
            return {};
        }
        return T(std::begin(vector.value()), std::end(vector.value()));
    }

    /*!
     *  \brief Try get parsed argument value for tupled container types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        auto vector = try_to_tupled_vector<
                typename T::value_type>(args->second(), delim);
        if (!vector.operator bool()) {
            return {};
        }
        return T(std::begin(vector.value()), std::end(vector.value()));
    }

    /*!
     *  \brief Try get parsed argument value for mapped types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_map<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        T res{};
        auto vector
                = try_to_paired_vector<typename T::key_type,
                                       typename T::mapped_type>(args->second(),
                                                                delim);
        if (!vector.operator bool()) {
            return {};
        }
        for (auto const& pair : vector.value()) {
            res.emplace(std::make_pair(pair.first, pair.second));
        }
        return res;
    }

    /*!
     *  \brief Try get parsed argument value for 2D deque, list, vector
     *  of not queue.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() != Action::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        T res{};
        for (auto const& v : args->second.matrix()) {
            auto vector = try_to_vector<typename T::value_type::value_type>(v);
            if (!vector.operator bool()) {
                return {};
            }
            res.push_back(typename T::value_type(std::begin(vector.value()),
                                                 std::end(vector.value())));
        }
        return res;
    }

    /*!
     *  \brief Try get parsed argument value for 2D deque, list, vector
     *  of queue.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() != Action::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        T res{};
        for (auto const& v : args->second.matrix()) {
            auto vector = try_to_vector<typename T::value_type::value_type>(v);
            if (!vector.operator bool()) {
                return {};
            }
            res.push_back(typename T::value_type(
                              std::deque<typename T::value_type::value_type>(
                                  std::begin(vector.value()),
                                  std::end(vector.value()))));
        }
        return res;
    }

    /*!
     *  \brief Try get parsed argument value for pair types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_pair<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return {};
        }
        if (args->second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args->second.size() != 2) {
                return {};
            }
            auto el1 = try_to_type<typename T::first_type>(args->second.at(0));
            auto el2 = try_to_type<typename T::second_type>(args->second.at(1));
            if (el1.operator bool() && el2.operator bool()) {
                return std::make_pair(el1.value(), el2.value());
            } else {
                return {};
            }
        }
        if (args->second.size() != 1) {
            return {};
        }
        return try_to_pair<typename T::first_type,
                typename T::second_type>(args->second.front(), delim);
    }

    /*!
     *  \brief Try get parsed argument value for queue types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_queue<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return {};
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
        if (!vector.operator bool()) {
            return {};
        }
        return T(std::deque<typename T::value_type>(std::begin(vector.value()),
                                                    std::end(vector.value())));
    }

    /*!
     *  \brief Try get parsed argument value for tuple.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return {};
        }
        if (args->second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            return try_to_tuple(detail::type_tag<T>{}, args->second());
        }
        if (args->second.size() != 1) {
            return {};
        }
        return try_to_tuple(detail::type_tag<T>{},
                            detail::_split(args->second.front(), delim));
    }

    /*!
     *  \brief Try get parsed argument value for custom types.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
#ifdef _ARGPARSE_EXPERIMENTAL_OPTIONAL
    [[deprecated("std::optional support is experimental, use C++17 or later")]]
#endif  // _ARGPARSE_EXPERIMENTAL_OPTIONAL
    std::optional<typename std::enable_if<
        !std::is_constructible<std::string, T>::value
        && !std::is_floating_point<T>::value
        && !std::is_integral<T>::value
        && !std::is_same<bool, T>::value
        && !detail::is_byte_type<T>::value
        && !detail::is_stl_array<typename std::decay<T>::type>::value
        && !detail::is_stl_container<typename std::decay<T>::type>::value
        && !detail::is_stl_container_paired<typename std::decay<T>::type>::value
        && !detail::is_stl_container_tupled<typename std::decay<T>::type>::value
        && !detail::is_stl_map<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value
        && !detail::is_stl_pair<typename std::decay<T>::type>::value
        && !detail::is_stl_queue<typename std::decay<T>::type>::value
        && !detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == Action::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return {};
        }
        return try_to_type<T>(detail::_vector_to_string(args->second()));
    }
#endif  // _ARGPARSE_USE_OPTIONAL

    /*!
     *  \brief Get unrecognized arguments
     *
     *  \return Unrecognized arguments
     */
    inline std::vector<std::string> const&
    unrecognized_args() const _ARGPARSE_NOEXCEPT
    {
        return m_unrecognized_args();
    }

    /*!
     *  \brief Get unrecognized arguments as args string
     *
     *  \return Unrecognized arguments as args string
     */
    inline std::string unrecognized_args_to_args() const
    {
        return detail::_vector_to_string(unrecognized_args(), detail::_spaces,
                                         std::string(), true);
    }

private:
    inline std::string
    boolean_option_to_args(std::string const& key,
                           Storage::value_type const& args) const
    {
        if (args.second.empty()) {
            return detail::_bool_to_string(args.first->default_value());
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return args.second.front() == args.first->const_value()
                ? detail::_bool_to_string(args.second.front())
                : args.second.front();
    }

    inline std::string
    boolean_option_to_string(std::string const& key,
                             Storage::value_type const& args,
                             std::string const& quotes) const
    {
        if (args.second.empty()) {
            if (args.first->action() == Action::BooleanOptionalAction) {
                return std::string("None");
            } else {
                return detail::_bool_to_string(args.first->default_value());
            }
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        if (args.second.is_default()) {
            return quotes + args.second.front() + quotes;
        }
        return detail::_bool_to_string(args.second.front());
    }

    inline std::string
    store_actions_to_string(Storage::value_type const& args,
                            std::string const& quotes) const
    {
        if ((args.first->action() == Action::store
             && (args.first->m_nargs
                 & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE)))
                || (!args.second.exists()
                    && args.first->m_type == Argument::Optional)
                || args.second.is_default()) {
            return detail::_vector_to_string(args.second(), ", ",
                                             quotes, false, "None");
        }
        if (args.first->action() != Action::append
                || (args.first->m_nargs
                    & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE))) {
            std::string none
                    = (args.first->m_nargs
                       & (Argument::ZERO_OR_MORE | Argument::REMAINDER))
                    || (args.first->action() == Action::extend
                        && args.first->m_nargs == Argument::ZERO_OR_ONE)
                    ? "" : "None";
            return detail::_vector_to_string(args.second(), ", ",
                                             quotes, false, none, "[", "]");
        } else {
            std::string none = (args.first->m_nargs
                               & (Argument::ZERO_OR_MORE | Argument::REMAINDER))
                    ? "" : "None";
            return detail::_matrix_to_string(args.second.matrix(), ", ",
                                             quotes, false, none, "[", "]");
        }
    }

    inline Storage::value_type const& data(std::string const& key) const
    {
        auto it = storage().find_arg(key);
        if (it != storage().end()) {
            return *it;
        }
        throw
        AttributeError("'Namespace' object has no attribute '" + key + "'");
    }

    inline Storage const& storage() const _ARGPARSE_NOEXCEPT
    {
        return m_storage;
    }

    template <class T, class U>
    std::pair<T, U> to_pair(std::string const& data, char delim) const
    {
        auto const pair = detail::_split_delimiter(data, delim);
        return std::make_pair(to_type<T>(pair.first), to_type<U>(pair.second));
    }

    template <class T, class U>
    std::vector<std::pair<T, U> >
    to_paired_vector(std::vector<std::string> const& args, char delim) const
    {
        std::vector<std::pair<T, U> > vec;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args.size() & 1) {
                throw TypeError("invalid stored argument amount");
            }
            vec.reserve(args.size() / 2);
            for (std::size_t i = 0; i < args.size(); i += 2) {
                vec.emplace_back(std::make_pair(to_type<T>(args.at(i)),
                                                to_type<U>(args.at(i + 1))));
            }
        } else {
            vec.reserve(args.size());
            std::transform(std::begin(args), std::end(args),
                           std::back_inserter(vec),
                           [this, delim] (std::string const& a)
            { return to_pair<T, U>(a, delim); });
        }
        return vec;
    }

    template <class T>
    std::vector<T> to_vector(std::vector<std::string> const& args) const
    {
        std::vector<T> vec;
        vec.reserve(args.size());
        std::transform(std::begin(args), std::end(args),
                       std::back_inserter(vec), [this] (std::string const& arg)
        { return to_type<T>(arg); });
        return vec;
    }

    template <class... Ts, std::size_t... Idxs>
    std::tuple<Ts...>
    parse_tuple(std::vector<std::string> const& values,
                detail::integer_sequence<std::size_t, Idxs...>) const
    {
        return std::make_tuple(to_type<Ts>(values[Idxs])...);
    }

    template <class... Ts>
    std::tuple<Ts...>
    to_tuple(detail::type_tag<std::tuple<Ts...>>,
             std::vector<std::string> const& values) const
    {
        return parse_tuple<Ts...>(
                    values, detail::make_integer_sequence<std::size_t,
                                                              sizeof...(Ts)>());
    }

    template <class T>
    std::vector<T>
    to_tupled_vector(std::vector<std::string> const& args, char delim) const
    {
        std::vector<T> vec;
        if (std::isspace(static_cast<unsigned char>(delim))) {
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
            std::transform(std::begin(args), std::end(args),
                           std::back_inserter(vec),
                           [this, delim] (std::string const& a)
            { return to_tuple(detail::type_tag<T>{},
                              detail::_split(a, delim)); });
        }
        return vec;
    }

    template <class T>
    typename std::enable_if<
        std::is_constructible<std::string, T>::value, T>::type
    to_type(std::string const& data) const
    {
        return detail::_remove_quotes<T>(data);
    }

    template <class T>
    typename std::enable_if<std::is_same<bool, T>::value, T>::type
    to_type(std::string const& data) const _ARGPARSE_NOEXCEPT
    {
        return detail::_string_to_bool(data);
    }

    template <class T>
    typename std::enable_if<detail::is_byte_type<T>::value, T>::type
    to_type(std::string const& data) const
    {
        if (data.empty()) {
            return T();
        }
        if (data.size() != 1) {
            throw TypeError("trying to get data from array argument value '"
                            + data + "'");
        }
        return T(data.front());
    }

    template <class T>
    typename std::enable_if<!std::is_constructible<std::string, T>::value
                            && !std::is_same<bool, T>::value
                            && !detail::is_byte_type<T>::value, T>::type
    to_type(std::string const& data) const
    {
        if (data.empty()) {
            return T();
        }
        T result;
        std::stringstream ss(detail::_remove_quotes(data));
        ss >> result;
        if (ss.fail() || !ss.eof()) {
            throw TypeError("invalid " + detail::Type::name<T>()
                            + " value: '" + data + "'");
        }
        return result;
    }

#ifdef _ARGPARSE_USE_OPTIONAL
    inline std::optional<Storage::value_type>
    try_get_data(std::string const& key) const
    {
        auto it = storage().find_arg(key);
        if (it != storage().end()) {
            return *it;
        }
        return {};
    }

    template <class T, class U>
    std::optional<std::pair<T, U> >
    try_to_pair(std::string const& data, char delim) const
    {
        auto const pair = detail::_split_delimiter(data, delim);
        auto el1 = try_to_type<T>(pair.first);
        auto el2 = try_to_type<U>(pair.second);
        if (el1.operator bool() && el2.operator bool()) {
            return std::make_pair(el1.value(), el2.value());
        } else {
            return {};
        }
    }

    template <class T, class U>
    std::optional<std::vector<std::pair<T, U> > >
    try_to_paired_vector(std::vector<std::string> const& args, char delim) const
    {
        std::vector<std::pair<T, U> > vec;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args.size() & 1) {
                return {};
            }
            vec.reserve(args.size() / 2);
            for (std::size_t i = 0; i < args.size(); i += 2) {
                auto el1 = try_to_type<T>(args.at(i));
                auto el2 = try_to_type<U>(args.at(i + 1));
                if (el1.operator bool() && el2.operator bool()) {
                    vec.emplace_back(std::make_pair(el1.value(), el2.value()));
                } else {
                    return {};
                }
            }
        } else {
            vec.reserve(args.size());
            for (auto const& arg : args) {
                auto pair = try_to_pair<T, U>(arg, delim);
                if (pair.operator bool()) {
                    vec.emplace_back(pair.value());
                } else {
                    return {};
                }
            }
        }
        return vec;
    }

    template <class T>
    std::optional<std::vector<T> >
    try_to_vector(std::vector<std::string> const& args) const
    {
        std::vector<T> vec;
        vec.reserve(args.size());
        for (auto const& arg : args) {
            auto el = try_to_type<T>(arg);
            if (el.operator bool()) {
                vec.emplace_back(el.value());
            } else {
                return {};
            }
        }
        return vec;
    }

    template <class... Ts>
    std::optional<std::tuple<Ts...> >
    try_to_tuple(detail::type_tag<std::tuple<Ts...>>,
                 std::vector<std::string> const& values) const
    {
        try {
            auto res = parse_tuple<Ts...>(
                        values,
                        detail::make_integer_sequence<std::size_t,
                                                              sizeof...(Ts)>());
            return res;
        } catch (...) {
            return {};
        }
    }

    template <class T>
    std::optional<std::vector<T> >
    try_to_tupled_vector(std::vector<std::string> const& args, char delim) const
    {
        std::vector<T> vec;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            auto const size = std::tuple_size<T>{};
            if (size == 0 || args.size() % size != 0) {
                return {};
            }
            vec.reserve(args.size() / size);
            for (std::size_t i = 0; i < args.size(); i += size) {
                std::vector<std::string> temp
                        = { args.begin() + i, args.begin() + i + size };
                auto tuple = try_to_tuple(detail::type_tag<T>{}, temp);
                if (tuple.operator bool()) {
                    vec.emplace_back(tuple.value());
                } else {
                    return {};
                }
            }
        } else {
            vec.reserve(args.size());
            for (auto const& arg : args) {
                auto tuple = try_to_tuple(detail::type_tag<T>{},
                                          detail::_split(arg, delim));
                if (tuple.operator bool()) {
                    vec.emplace_back(tuple.value());
                } else {
                    return {};
                }
            }
        }
        return vec;
    }

    template <class T>
    std::optional<typename std::enable_if<
        std::is_constructible<std::string, T>::value, T>::type>
    try_to_type(std::string const& data) const
    {
        return detail::_remove_quotes<T>(data);
    }

    template <class T>
    std::optional<typename std::enable_if<
        std::is_same<bool, T>::value, T>::type>
    try_to_type(std::string const& data) const _ARGPARSE_NOEXCEPT
    {
        return detail::_string_to_bool(data);
    }

    template <class T>
    std::optional<typename std::enable_if<
        detail::is_byte_type<T>::value, T>::type>
    try_to_type(std::string const& data) const _ARGPARSE_NOEXCEPT
    {
        if (data.empty() || data.size() != 1) {
            return {};
        }
        return T(data.front());
    }

    template <class T>
    std::optional<typename std::enable_if<
        !std::is_constructible<std::string, T>::value
        && !std::is_same<bool, T>::value
        && !detail::is_byte_type<T>::value, T>::type>
    try_to_type(std::string const& data) const
    {
        if (data.empty()) {
            return T();
        }
        T result;
        std::stringstream ss(detail::_remove_quotes(data));
        ss >> result;
        if (ss.fail() || !ss.eof()) {
            return {};
        }
        return result;
    }
#endif  // _ARGPARSE_USE_OPTIONAL

    Storage m_storage;
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
inline std::ostream& operator <<(std::ostream& os, Namespace const& obj)
{
    os << obj.to_string();
    return os;
}

/*!
 *  \brief ArgumentParser objects
 */
_ARGPARSE_EXPORT class ArgumentParser
{
    typedef std::shared_ptr<Argument> pArgument;
    typedef std::shared_ptr<_Group> pGroup;
    typedef std::shared_ptr<ArgumentParser> pParser;

public:
    /*!
     *  \brief Subparser class
     */
    class Subparser : public _Group
    {
        friend class ArgumentParser;

        Subparser(std::string const& title,
                  std::string const& description)
            : _Group(title, description),
              m_parent_prog(),
              m_parent_args(),
              m_prog(),
              m_dest(),
              m_required(false),
              m_help(),
              m_metavar(),
              m_parsers()
        { }

        static inline std::shared_ptr<Subparser>
        make_subparser(std::string const& title,
                       std::string const& description)
        {
            return std::make_shared<Subparser>(Subparser(title, description));
        }

    public:
        using _Group::title;
        using _Group::description;

        /*!
         *  \brief Set subparser 'title' value
         *
         *  \param value Title value
         *
         *  \return Current subparser reference
         */
        inline Subparser& title(std::string const& value)
        {
            m_title = detail::_trim_copy(value);
            return *this;
        }

        /*!
         *  \brief Set subparser 'description' value
         *
         *  \param param Description value
         *
         *  \return Current subparser reference
         */
        inline Subparser& description(std::string const& param)
        {
            m_description = detail::_trim_copy(param);
            return *this;
        }

        /*!
         *  \brief Set subparser 'prog' value
         *
         *  \param value Program value
         *
         *  \return Current subparser reference
         */
        inline Subparser& prog(std::string const& value)
        {
            m_prog = detail::_trim_copy(value);
            update_prog(m_parent_prog, m_parent_args);
            return *this;
        }

        /*!
         *  \brief Set subparser 'dest' value
         *
         *  \param value Destination value
         *
         *  \return Current subparser reference
         */
        inline Subparser& dest(std::string const& value)
        {
            m_dest = detail::_format_name(value);
            return *this;
        }

        /*!
         *  \brief Set subparser 'required' value
         *
         *  \param value Required flag
         *
         *  \return Current subparser reference
         */
        inline Subparser& required(bool value) _ARGPARSE_NOEXCEPT
        {
            m_required = value;
            return *this;
        }

        /*!
         *  \brief Set subparser 'help' message
         *
         *  \param value Help message
         *
         *  \return Current subparser reference
         */
        inline Subparser& help(std::string const& value)
        {
            m_help = value;
            return *this;
        }

        /*!
         *  \brief Set subparser 'metavar' value
         *
         *  \param value Metavar value
         *
         *  \return Current subparser reference
         */
        inline Subparser& metavar(std::string const& value)
        {
            m_metavar = detail::_trim_copy(value);
            return *this;
        }

        /*!
         *  \brief Get subparser 'prog' value
         *
         *  \return Subparser 'prog' value
         */
        inline std::string const& prog() const _ARGPARSE_NOEXCEPT
        {
            return m_prog;
        }

        /*!
         *  \brief Get subparser 'dest' value
         *
         *  \return Subparser 'dest' value
         */
        inline std::string const& dest() const _ARGPARSE_NOEXCEPT
        {
            return m_dest;
        }

        /*!
         *  \brief Get subparser 'required' value
         *
         *  \return Subparser 'required' value
         */
        inline bool required() const _ARGPARSE_NOEXCEPT
        {
            return m_required;
        }

        /*!
         *  \brief Get subparser 'help' message
         *
         *  \return Subparser 'help' message
         */
        inline std::string const& help() const _ARGPARSE_NOEXCEPT
        {
            return m_help;
        }

        /*!
         *  \brief Get subparser 'metavar' value
         *
         *  \return Subparser 'metavar' value
         */
        inline std::string const& metavar() const _ARGPARSE_NOEXCEPT
        {
            return m_metavar();
        }

        /*!
         *  \brief Add argument parser with name
         *
         *  \param name Parser name
         *
         *  \return Current argument parser reference
         */
        inline ArgumentParser& add_parser(std::string const& name)
        {
            auto value = detail::_format_name(name);
            if (value.empty()) {
                throw ValueError("parser name can't be empty");
            }
            m_parsers.emplace_back(make_parser(value));
            m_parsers.back()->update_prog(prog_name());
            return *m_parsers.back();
        }

    private:
        inline std::string prog_name() const
        {
            std::string program = m_prog.empty() ? m_parent_prog : m_prog;
            if (!m_parent_args.empty()) {
                program += " " + m_parent_args;
            }
            return program;
        }

        inline void update_prog(std::string const& parent_prog,
                                std::string const& parent_args)
        {
            m_parent_prog = parent_prog;
            m_parent_args = parent_args;
            auto program = prog_name();
            for (auto& parser : m_parsers) {
                parser->update_prog(program);
            }
        }

        inline void
        limit_help_flags(_HelpFormatter const&,
                         std::size_t& limit) const override
        {
            auto size = flags_to_string().size();
            if (limit < size) {
                limit = size;
            }
            for (auto const& arg : m_parsers) {
                auto s = arg->m_name.size() + 2;
                if (limit < s) {
                    limit = s;
                }
            }
        }

        inline void print_help(std::ostream& os,
                               _HelpFormatter const& formatter,
                               std::string const& prog,
                               std::size_t limit,
                               std::size_t width) const override
        {
            os << "\n" << (title().empty() ? "subcommands" : title()) << ":\n";
            detail::_print_raw_text_formatter(
                        formatter,
                        detail::_replace(
                            description(), "%(prog)s", prog),
                        width, os, "", "  ", "\n");
            os << print(formatter, limit, width) << std::endl;
        }

        inline std::string usage() const
        {
            return flags_to_string() + " ...";
        }

        inline std::string flags_to_string() const
        {
            if (m_metavar.has_value()) {
                return metavar();
            }
            std::string res;
            for (auto const& p : m_parsers) {
                detail::_append_value_to(p->m_name, res, ",");
                for (auto const& alias : p->aliases()) {
                    detail::_append_value_to(alias, res, ",");
                }
            }
            return "{" + res + "}";
        }

        inline std::string print(_HelpFormatter const& formatter,
                                 std::size_t limit, std::size_t width) const
        {
            auto res = detail::_help_formatter(
                        "  " + flags_to_string(),
                        formatter,
                        help(),
                        width,
                        std::string(limit, detail::_space));
            return std::accumulate(std::begin(m_parsers), std::end(m_parsers),
                                   res, [formatter, limit, width]
                                   (std::string const& str, pParser const& p)
            {
                if (!p->help().empty()) {
                    auto name = "    " + p->m_name;
                    auto alias = detail::_vector_to_string(p->aliases(), ", ");
                    if (!alias.empty()) {
                        name += " (" + alias + ")";
                    }
                    return str + "\n" + detail::_help_formatter(
                                name,
                                formatter,
                                p->help(),
                                width,
                                std::string(limit, detail::_space));
                }
                return str;
            });
        }

        std::string m_parent_prog;
        std::string m_parent_args;
        std::string m_prog;
        std::string m_dest;
        bool        m_required;
        std::string m_help;
        detail::Value<std::string> m_metavar;
        std::deque<pParser> m_parsers;
    };

private:
    typedef std::pair<std::shared_ptr<Subparser>, std::size_t> SubparserInfo;

    inline void
    apply_formatter_class(HelpFormatter const& param, bool force = false)
    {
        HelpFormatter sample;
        if (!m_formatter_class._fill_text
                || force
                || sample._fill_text() != param._fill_text()) {
            m_formatter_class._fill_text = param._fill_text();
        }
        if (!m_formatter_class._get_default_metavar_for_optional
                || force
                || sample._get_default_metavar_for_optional()
                    != param._get_default_metavar_for_optional()) {
            m_formatter_class._get_default_metavar_for_optional
                    =  param._get_default_metavar_for_optional();
        }
        if (!m_formatter_class._get_default_metavar_for_positional
                || force
                || sample._get_default_metavar_for_positional()
                    != param._get_default_metavar_for_positional()) {
            m_formatter_class._get_default_metavar_for_positional
                    = param._get_default_metavar_for_positional();
        }
        if (!m_formatter_class._get_help_string
                || force
                || sample._get_help_string() != param._get_help_string()) {
            m_formatter_class._get_help_string = param._get_help_string();
        }
        if (!m_formatter_class._split_lines
                || force
                || sample._split_lines() != param._split_lines()) {
            m_formatter_class._split_lines = param._split_lines();
        }
    }

public:
    /*!
     *  \brief Construct argument parser with concrete program name
     *
     *  \param prog Program name
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(std::string const& prog = "untitled")
        : m_data(),
          m_name(),
          m_prog("untitled"),
          m_usage(),
          m_description(),
          m_epilog(),
          m_help(),
          m_aliases(),
          m_formatter_class(),
          m_prefix_chars(detail::_prefix_chars),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_argument_default_type(),
          m_allow_abbrev(true),
          m_exit_on_error(true),
          m_output_width(),
          m_groups(),
          m_mutex_groups(),
          m_default_values(),
          m_parsed_arguments(),
          m_subparsers(nullptr),
          m_subparsers_position(),
          m_handle(nullptr),
          m_parse_handle(nullptr)
    {
        apply_formatter_class(HelpFormatter());
        this->prog(prog);
        m_data.update_help(true, m_prefix_chars);
    }

    /*!
     *  \brief Construct argument parser from command line arguments
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(int argc, char* argv[])
        : ArgumentParser(argc, const_cast<char const**>(argv))
    { }

    /*!
     *  \brief Construct argument parser from command line arguments
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(int argc, char const* argv[])
        : ArgumentParser("untitled")
    {
        if (argc > 0 && argv != nullptr && argv[0] != nullptr) {
            m_prog = detail::_file_name(argv[0]);
            m_parsed_arguments.reserve(std::size_t(argc - 1));
            for (int i = 1; i < argc; ++i) {
                if (argv[i] == nullptr) {
                    break;
                }
                m_parsed_arguments.emplace_back(std::string(argv[i]));
            }
        }
    }

    /*!
     *  \brief Construct argument parser from command line arguments
     *  with concrete program name
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *  \param prog Program name
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(int argc, char* argv[], std::string const& prog)
        : ArgumentParser(argc, const_cast<char const**>(argv), prog)
    { }

    /*!
     *  \brief Construct argument parser from command line arguments
     *  with concrete program name
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *  \param prog Program name
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(int argc, char const* argv[], std::string const& prog)
        : ArgumentParser(argc, argv)
    {
        this->prog(prog);
    }

    /*!
     *  \brief Destroy argument parser
     */
    virtual ~ArgumentParser() _ARGPARSE_NOEXCEPT = default;

private:
    pParser static make_parser(std::string const& name)
    {
        pParser result = std::make_shared<ArgumentParser>(ArgumentParser());
        result->m_prog = "";
        result->m_name = name;
        return result;
    }

public:
    /*!
     *  \brief Set argument parser 'prog' value (default: argv[0] or "untitled")
     *
     *  \param param Program name
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& prog(std::string const& param)
    {
        auto value = detail::_trim_copy(param);
        if (!value.empty()) {
            m_prog = value;
            if (m_subparsers) {
                m_subparsers->update_prog(prog(), subparser_prog_args());
            }
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'usage' value
     *
     *  \param param Usage value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& usage(std::string const& param)
    {
        m_usage = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'description' value
     *
     *  \param param Description value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& description(std::string const& param)
    {
        m_description = param;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'epilog' value
     *
     *  \param param Epilog value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& epilog(std::string const& param)
    {
        m_epilog = param;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'help' message (for subparsers)
     *
     *  \param value Help message
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& help(std::string const& value)
    {
        m_help = value;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param values Aliases value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& aliases(std::vector<std::string> const& value)
    {
        m_aliases.clear();
        auto values = value;
        for (auto& v : values) {
            v = detail::_format_name(v);
            if (!v.empty()) {
                m_aliases.push_back(v);
            }
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param param First value
     *  \param args Other values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser& aliases(std::string const& param, Args... args)
    {
        return aliases(std::vector<std::string>{ param, args... });
    }

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param param Parents values
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& parents(std::vector<ArgumentParser> const& param)
    {
        for (auto const& parent : param) {
            if (this == &parent) {
                continue;
            }
            if (parent.m_subparsers) {
                if (m_subparsers) {
                    throw_error("cannot have multiple subparser arguments");
                }
                m_subparsers_position
                    = parent.m_subparsers_position + m_data.m_positional.size();
                m_subparsers = parent.m_subparsers;
                m_subparsers->update_prog(prog(), subparser_prog_args());
            }
            m_data.merge_arguments(parent.m_data);
            for (auto const& group : parent.m_groups) {
                m_groups.push_back(group);
            }
            for (auto const& group : parent.m_mutex_groups) {
                m_mutex_groups.push_back(group);
            }
            for (auto const& pair : parent.m_default_values) {
                m_default_values.push_back(pair);
            }
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param param Parent value
     *  \param args Parents values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser& parents(ArgumentParser const& param, Args... args)
    {
        return parents(std::vector<ArgumentParser>{ param, args... });
    }

    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param param HelpFormatter value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    formatter_class(HelpFormatter const& param)
    {
        apply_formatter_class(param, true);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param param HelpFormatter value
     *  \param args HelpFormatter values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser&
    formatter_class(HelpFormatter const& param, Args... args)
    {
        formatter_class(param);
        return add_formatter_class(args...);
    }

    /*!
     *  \brief Add argument parser 'formatter_class' value
     *
     *  \param param HelpFormatter value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    add_formatter_class(HelpFormatter const& param)
    {
        apply_formatter_class(param);
        return *this;
    }

    /*!
     *  \brief Add argument parser 'formatter_class' value
     *
     *  \param param HelpFormatter value
     *  \param args HelpFormatter values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser&
    add_formatter_class(HelpFormatter const& param, Args... args)
    {
        add_formatter_class(param);
        return add_formatter_class(args...);
    }

    /*!
     *  \brief Set argument parser 'prefix_chars' value (default: "-")
     *
     *  \param param Prefix chars values
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& prefix_chars(std::string const& param)
    {
        auto value = detail::_trim_copy(param);
        if (!value.empty()) {
            m_prefix_chars = std::move(value);
            m_data.update_help(m_data.m_add_help, m_prefix_chars);
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'fromfile_prefix_chars' value
     *
     *  \param param Fromfile prefix chars values
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& fromfile_prefix_chars(std::string const& param)
    {
        m_fromfile_prefix_chars = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'argument_default' value
     *
     *  \param param Argument default value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& argument_default(std::string const& param)
    {
        m_argument_default = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Suppress argument parser 'argument_default' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& argument_default(_SUPPRESS value)
    {
        if (value != argparse::SUPPRESS) {
            throw
            TypeError("got an unexpected keyword argument 'argument_default'");
        }
        m_argument_default_type = value;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'conflict_handler' value
     *
     *  \param param Argument default value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& conflict_handler(std::string const& param)
    {
        if (param != "resolve") {
            throw AttributeError("'ArgumentParser' object has no attribute "
                                 "'_handle_conflict_" + param + "'");
        }
        m_data.m_conflict_handler = param;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'add_help' value (default: true)
     *
     *  \param value Add help flag
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& add_help(bool value)
    {
        m_data.update_help(value, m_prefix_chars);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'allow_abbrev' value (default: true)
     *
     *  \param value Allow abbrev flag
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& allow_abbrev(bool value) _ARGPARSE_NOEXCEPT
    {
        m_allow_abbrev = value;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'exit_on_error' value (default: true)
     *
     *  \param value Exit on error flag
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& exit_on_error(bool value) _ARGPARSE_NOEXCEPT
    {
        m_exit_on_error = value;
        return *this;
    }

    /*!
     *  \brief Set output width value (default: auto-detected or 80, min 33)
     *
     *  \param value Output width
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& output_width(std::size_t value) _ARGPARSE_NOEXCEPT
    {
        m_output_width = value;
        if (m_output_width() < detail::_min_width) {
            m_output_width = detail::_min_width;
        }
        return *this;
    }

    /*!
     *  \brief Get argument parser 'prog' value (default: argv[0] or "untitled")
     *
     *  \return Argument parser 'prog' value
     */
    inline std::string const& prog() const _ARGPARSE_NOEXCEPT
    {
        return m_prog;
    }

    /*!
     *  \brief Get argument parser 'usage' value
     *
     *  \return Argument parser 'usage' value
     */
    inline std::string const& usage() const _ARGPARSE_NOEXCEPT
    {
        return m_usage;
    }

    /*!
     *  \brief Get argument parser 'description' value
     *
     *  \return Argument parser 'description' value
     */
    inline std::string const& description() const _ARGPARSE_NOEXCEPT
    {
        return m_description;
    }

    /*!
     *  \brief Get argument parser 'epilog' value
     *
     *  \return Argument parser 'epilog' value
     */
    inline std::string const& epilog() const _ARGPARSE_NOEXCEPT
    {
        return m_epilog;
    }

    /*!
     *  \brief Get argument parser 'help' message (for subparsers)
     *
     *  \return Argument parser 'help' message
     */
    inline std::string const& help() const _ARGPARSE_NOEXCEPT
    {
        return m_help;
    }

    /*!
     *  \brief Get argument parser 'aliases' value (for subparsers)
     *
     *  \return Argument parser 'aliases' value
     */
    inline std::vector<std::string> const& aliases() const _ARGPARSE_NOEXCEPT
    {
        return m_aliases;
    }

    /*!
     *  \brief Get argument parser 'prefix_chars' value (default: "-")
     *
     *  \return Argument parser 'prefix_chars' value
     */
    inline std::string const& prefix_chars() const _ARGPARSE_NOEXCEPT
    {
        return m_prefix_chars;
    }

    /*!
     *  \brief Get argument parser 'fromfile_prefix_chars' value
     *
     *  \return Argument parser 'fromfile_prefix_chars' value
     */
    inline std::string const& fromfile_prefix_chars() const _ARGPARSE_NOEXCEPT
    {
        return m_fromfile_prefix_chars;
    }

    /*!
     *  \brief Get argument parser 'argument_default' value
     *
     *  \return Argument parser 'argument_default' value
     */
    inline std::string const& argument_default() const _ARGPARSE_NOEXCEPT
    {
        return m_argument_default();
    }

    /*!
     *  \brief Get argument parser 'conflict_handler' value
     *
     *  \return Argument parser 'conflict_handler' value
     */
    inline std::string const& conflict_handler() const _ARGPARSE_NOEXCEPT
    {
        return m_data.m_conflict_handler;
    }

    /*!
     *  \brief Get argument parser 'add_help' value (default: true)
     *
     *  \return Argument parser 'add_help' value
     */
    inline bool add_help() const _ARGPARSE_NOEXCEPT
    {
        return m_data.m_add_help;
    }

    /*!
     *  \brief Get argument parser 'allow_abbrev' value (default: true)
     *
     *  \return Argument parser 'allow_abbrev' value
     */
    inline bool allow_abbrev() const _ARGPARSE_NOEXCEPT
    {
        return m_allow_abbrev;
    }

    /*!
     *  \brief Get argument parser 'exit_on_error' value (default: true)
     *
     *  \return Argument parser 'exit_on_error' value
     */
    inline bool exit_on_error() const _ARGPARSE_NOEXCEPT
    {
        return m_exit_on_error;
    }

    /*!
     *  \brief Get output width value (default: auto-detected or 80, min 33)
     *
     *  \return Output width value
     */
    inline std::size_t output_width() const
    {
        return m_output_width.has_value() ? m_output_width()
                                          : detail::_get_terminal_size().first;
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param args Flag values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    Argument& add_argument(Args... args)
    {
        return add_argument(std::vector<std::string>{ args... });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::vector<std::string> const& flags)
    {
        m_data.create_argument(flags, prefix_chars());
        process_add_argument();
        return *m_data.m_arguments.back();
    }

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current parser reference
     */
    template <typename = void>
    inline ArgumentParser& add_argument(Argument const& argument)
    {
        m_data.validate_argument(Argument(argument), prefix_chars());
        process_add_argument();
        return *this;
    }

    /*!
     *  \brief Add argument group
     *
     *  \param title Group title
     *  \param description Group description
     *
     *  \return Current argument group reference
     */
    inline ArgumentGroup&
    add_argument_group(std::string const& title = std::string(),
                       std::string const& description = std::string())
    {
        auto group = ArgumentGroup::make_argument_group(
                    title, description, m_prefix_chars, m_data,
                    m_argument_default, m_argument_default_type);
        m_groups.push_back(group);
        return *group;
    }

    /*!
     *  \brief Add mutually exclusive group
     *
     *  \param required Required flag
     *
     *  \return Current mutually exclusive group reference
     */
    inline MutuallyExclusiveGroup&
    add_mutually_exclusive_group(bool required = false)
    {
        m_mutex_groups.emplace_back(
              MutuallyExclusiveGroup::make_mutex_group(
                        m_prefix_chars, m_data,
                        m_argument_default, m_argument_default_type));
        return m_mutex_groups.back().required(required);
    }

    /*!
     *  \brief Add subparsers
     *
     *  \return Current subparser reference
     */
    inline Subparser&
    add_subparsers(std::string const& title = std::string(),
                   std::string const& description = std::string())
    {
        if (m_subparsers) {
            throw_error("cannot have multiple subparser arguments");
        }
        m_subparsers_position = m_data.m_positional.size();
        m_subparsers = Subparser::make_subparser(title, description);
        m_subparsers->update_prog(prog(), subparser_prog_args());
        m_groups.push_back(m_subparsers);
        return *m_subparsers;
    }

    /*!
     *  \brief Set argument parser 'handle' function.
     *  Called when the parser is executed and passed the value of the
     *  parser
     *
     *  \param func Handle function
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    handle(std::function<void(std::string const&)> func) _ARGPARSE_NOEXCEPT
    {
        m_handle = func;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'handle' function.
     *  Called when the parser is executed.
     *
     *  \param func Handle function
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& handle(std::function<void()> func) _ARGPARSE_NOEXCEPT
    {
        m_handle = [func] (std::string const&) { func(); };
        return *this;
    }

    /*!
     *  \brief Set argument parser 'handle' function.
     *  Called when the parser is executed and passed the namespace
     *  of the parser
     *
     *  \param func Parse handle function
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    handle(std::function<void(Namespace const&)> func) _ARGPARSE_NOEXCEPT
    {
        m_parse_handle = func;
        return *this;
    }

    /*!
     *  \brief Get default value for certain argument
     *
     *  \param dest Argument destination name or flag
     *
     *  \return Default value for certain argument
     */
    std::string get_default(std::string const& dest) const
    {
        auto const positional = m_data.get_positional(true, true);
        auto const optional = m_data.get_optional(true, true);
        auto ip = std::find_if(std::begin(positional), std::end(positional),
                               [&dest] (pArgument const& arg)
        { return detail::_is_value_exists(dest, arg->m_flags); });
        if (ip != std::end(positional)) {
            return (*ip)->m_default();
        }
        for (auto const& arg : optional) {
            if (!arg->dest().empty()) {
                if (arg->dest() == dest) {
                    if (arg->is_suppressed()) {
                        return detail::_suppress;
                    }
                    return arg->m_default();
                }
            } else {
                for (auto const& flag : arg->m_flags) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        if (arg->is_suppressed()) {
                            return detail::_suppress;
                        }
                        return arg->m_default();
                    }
                }
            }
        }
        auto it = std::find_if(
                    std::begin(m_default_values), std::end(m_default_values),
                    [&dest] (std::pair<std::string, std::string> const& pair)
        { return pair.first == dest; });
        return it != std::end(m_default_values) ? it->second : std::string();
    }

    /*!
     *  \brief Set default values for certain arguments
     *
     *  \param pairs Vector of pairs: { 'argument flag', 'default value' }
     */
    inline void
    set_defaults(std::vector<std::pair<std::string, std::string> > const& pairs)
    {
        for (auto const& pair : pairs) {
            auto dest = detail::_format_name(pair.first);
            if (dest.empty()) {
                continue;
            }
            auto value = detail::_trim_copy(pair.second);
            if (!is_default_stored(m_data.m_arguments, dest, value)) {
                m_default_values.push_back(std::make_pair(dest, value));
            }
        }
    }

    /*!
     *  \brief Parse command line arguments
     *
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <typename = void>
    Namespace parse_args(Namespace const& space = Namespace()) const
    {
        return parse_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <class T,
              class = typename std::enable_if<
                  std::is_constructible<std::string, T>::value>::type>
    Namespace parse_args(T const& args,
                         Namespace const& space = Namespace()) const
    {
        return parse_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    inline Namespace parse_args(std::vector<std::string> const& args,
                                Namespace const& space = Namespace()) const
    {
        return on_parse_arguments(args, false, false, space);
    }

    /*!
     *  \brief Parse known command line arguments
     *
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <typename = void>
    Namespace parse_known_args(Namespace const& space = Namespace()) const
    {
        return parse_known_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <class T,
              class = typename std::enable_if<
                  std::is_constructible<std::string, T>::value>::type>
    Namespace parse_known_args(T const& args,
                               Namespace const& space = Namespace()) const
    {
        return parse_known_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    inline Namespace
    parse_known_args(std::vector<std::string> const& args,
                     Namespace const& space = Namespace()) const
    {
        return on_parse_arguments(args, true, false, space);
    }

    /*!
     *  \brief Parse intermixed command line arguments
     *
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <typename = void>
    Namespace parse_intermixed_args(Namespace const& space = Namespace()) const
    {
        return parse_intermixed_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <class T,
              class = typename std::enable_if<
                  std::is_constructible<std::string, T>::value>::type>
    Namespace parse_intermixed_args(T const& args,
                                    Namespace const& space = Namespace()) const
    {
        return parse_intermixed_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    inline Namespace
    parse_intermixed_args(std::vector<std::string> const& args,
                          Namespace const& space = Namespace()) const
    {
        return on_parse_arguments(args, false, true, space);
    }

    /*!
     *  \brief Parse known intermixed command line arguments
     *
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <typename = void>
    Namespace
    parse_known_intermixed_args(Namespace const& space = Namespace()) const
    {
        return parse_known_intermixed_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    template <class T,
              class = typename std::enable_if<
                  std::is_constructible<std::string, T>::value>::type>
    Namespace
    parse_known_intermixed_args(T const& args,
                                Namespace const& space = Namespace()) const
    {
        return parse_known_intermixed_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
    inline Namespace
    parse_known_intermixed_args(std::vector<std::string> const& args,
                                Namespace const& space = Namespace()) const
    {
        return on_parse_arguments(args, true, true, space);
    }

    /*!
     *  \brief Print a program usage to output stream (default: std::cout)
     *
     *  \param os Output stream
     */
    inline void print_usage(std::ostream& os = std::cout) const
    {
        if (!usage().empty()) {
            os << "usage: " << detail::_replace(usage(), "%(prog)s", prog())
               << std::endl;
        } else {
            auto const positional = m_data.get_positional(false, true);
            auto const optional = m_data.get_optional(false, true);
            print_custom_usage(positional, optional, m_mutex_groups,
                               subparser_info(false), prog(), os);
        }
    }

    /*!
     *  \brief Print a help message to output stream (default: std::cout)
     *
     *  \param os Output stream
     */
    void print_help(std::ostream& os = std::cout) const
    {
        auto const positional_all = m_data.get_positional(false, true);
        auto const optional_all = m_data.get_optional(false, true);
        auto const positional = m_data.get_positional(false, false);
        auto const optional = m_data.get_optional(false, false);
        auto const sub_info = subparser_info(false);
        if (!usage().empty()) {
            os << "usage: " << detail::_replace(usage(), "%(prog)s", prog())
               << std::endl;
        } else {
            print_custom_usage(positional_all, optional_all,
                               m_mutex_groups, sub_info, prog(), os);
        }
        std::size_t width = output_width();
        detail::_print_raw_text_formatter(
                    m_formatter_class,
                    detail::_replace(description(), "%(prog)s", prog()),
                    width, os);
        std::size_t size = 0;
        auto _update_size = [&size] (std::size_t value)
        {
            if (size < value) {
                size = value;
            }
        };
        auto subparser = sub_info.first;
        bool sub_positional = is_subparser_positional(subparser);
        for (auto const& arg : positional) {
            _update_size(arg->flags_to_string(m_formatter_class).size());
        }
        for (auto const& arg : optional) {
            _update_size(arg->flags_to_string(m_formatter_class).size());
        }
        for (auto const& group : m_groups) {
            group->limit_help_flags(m_formatter_class, size);
        }
        size += 4;
        auto const name_limit = argument_name_limit();
        if (size > name_limit) {
            size = name_limit;
        }
        if (!positional.empty() || sub_positional) {
            os << "\npositional arguments:\n";
            for (std::size_t i = 0; i < positional.size(); ++i) {
                print_subparser(sub_positional, sub_info, i,
                                m_formatter_class, size, width, os);
                os << detail::_replace(
                          positional.at(i)->print(m_formatter_class,
                                                  size, width),
                          "%(prog)s", prog()) << std::endl;
            }
            print_subparser(sub_positional, sub_info, positional.size(),
                            m_formatter_class, size, width, os);
        }
        if (!optional.empty()) {
            os << "\noptions:\n";
            for (auto const& arg : optional) {
                os << detail::_replace(
                          arg->print(m_formatter_class, size, width),
                          "%(prog)s", prog()) << std::endl;
            }
        }
        for (auto const& group : m_groups) {
            print_group(group, subparser, sub_positional, m_formatter_class,
                        prog(), size, width, os);
        }
        detail::_print_raw_text_formatter(
                    m_formatter_class,
                    detail::_replace(epilog(), "%(prog)s", prog()),
                    width, os);
    }

    /*!
     *  \brief Return a string containing a program usage
     *
     *  \return Program usage
     */
    inline std::string format_usage() const
    {
        std::stringstream ss;
        print_usage(ss);
        return detail::_trim_copy(ss.str());
    }

    /*!
     *  \brief Return a string containing a help message
     *
     *  \return Help message
     */
    inline std::string format_help() const
    {
        std::stringstream ss;
        print_help(ss);
        return detail::_trim_copy(ss.str());
    }

    /*!
     *  \brief Terminates the program, exiting with the specified status and,
     *  if given, prints a message before that
     *
     *  \param status Status code
     *  \param message Error message
     */
    virtual inline void
    exit(int status = 0, std::string const& message = std::string()) const
    {
        if (!message.empty()) {
            std::cerr << message << std::endl;
        }
        ::exit(status);
    }

    /*!
     *  \brief Prints a usage message including the message to the
     *  standard error and terminates the program with a status code of 2
     *
     *  \param message Error message
     */
    inline void error(std::string const& message) const
    {
        print_usage(std::cerr);
        std::cerr << prog() << ": error: " << message << std::endl;
        ::exit(2);
    }

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
    virtual inline std::vector<std::string>
    convert_arg_line_to_args(std::string const& arg_line) const
    {
        return { arg_line };
    }

private:
    inline void
    throw_error(std::string const& message, std::ostream& os = std::cerr) const
    {
        print_usage(os);
        throw std::logic_error(prog() + ": error: " + message);
    }

    inline void process_add_argument()
    {
        bool optional = m_data.m_arguments.back()->m_type == Argument::Optional;
        if (m_argument_default.has_value()
                && !m_data.m_arguments.back()->m_default.has_value()
                && !m_data.m_arguments.back()->m_default_type.has_value()) {
            m_data.m_arguments.back()->default_value(m_argument_default());
        }
        if (m_argument_default_type.has_value()) {
            m_data.m_arguments.back()->default_value(m_argument_default_type());
        }
        (optional ? m_data.m_optional : m_data.m_positional)
                .push_back(std::make_pair(m_data.m_arguments.back(), false));
    }

    inline Namespace on_parse_arguments(std::vector<std::string> const& args,
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
            std::cerr << "error: unexpected error" << std::endl;
        }
        ::exit(1);
    }

    inline std::vector<std::string>
    read_args_from_file(std::vector<std::string> const& arguments) const
    {
        std::vector<std::string> res = arguments;
        if (!fromfile_prefix_chars().empty()) {
            for (std::size_t i = 0; i < res.size(); ++i) {
                while (!res.at(i).empty()
                       && detail::_is_value_exists(res.at(i).front(),
                                                   fromfile_prefix_chars())) {
                    auto file = res.at(i).substr(1);
                    std::ifstream is(file);
                    if (!is.is_open()) {
                        throw_error("[Errno 2] No such file or directory: '"
                                    + file + "'");
                    }
                    std::vector<std::string> args;
                    std::string line;
                    while (std::getline(is, line)) {
                        auto line_args = convert_arg_line_to_args(line);
                        detail::_move_vector_insert_to_end(line_args, args);
                    }
                    is.close();
                    detail::_move_vector_replace_at(args, res, i);
                }
            }
        }
        return res;
    }

    struct ParserInfo
    {
        explicit
        ParserInfo(ArgumentParser const* parser,
                   Namespace::Storage const& storage,
                   SubparserInfo const& subparser)
            : parser(parser),
              optional(),
              storage(storage),
              subparser(subparser),
              have_negative_args()
        {
            optional = parser->m_data.get_optional(true, true);
            have_negative_args
                 = negative_numbers_presented(optional, parser->prefix_chars());
        }

        ParserInfo(ParserInfo const&) = default;
        ParserInfo& operator =(ParserInfo const&) = default;

        inline void validate() const
        {
            validate_arguments(parser->m_data.get_arguments(true));
        }

        inline void create_storage(bool force)
        {
            if (storage.m_data.empty() && !force) {
                storage.create(parser->m_data.get_arguments(true));
            } else {
                storage.force_add(parser->m_data.get_arguments(true));
            }
        }

        ArgumentParser const* parser;
        std::vector<pArgument> optional;
        Namespace::Storage storage;
        SubparserInfo subparser;
        bool have_negative_args;
    };

    using Parsers = std::deque<ParserInfo>;

    Namespace
    parse_arguments(std::vector<std::string> const& in_parsed_arguments,
                    bool only_known,
                    bool intermixed,
                    Namespace const& space) const
    {
        handle(prog());
        check_namespace(space);
        auto parsed_arguments = read_args_from_file(in_parsed_arguments);

        Parsers parsers;
        parsers.push_back(
                    ParserInfo(this, space.storage(), subparser_info(true)));

        check_mutex_arguments();
        check_intermixed_subparser(intermixed, parsers.back().subparser.first);

        auto positional = m_data.get_positional(true, true);
        check_intermixed_remainder(intermixed, positional);

        bool was_pseudo_arg = false;

        parsers.back().validate();
        parsers.back().create_storage(false);

        std::vector<std::string> unrecognized_args;
        std::deque<std::string> intermixed_args;

        std::size_t pos = 0;

        for (std::size_t i = 0; i < parsed_arguments.size(); ++i) {
            if (parsed_arguments.at(i) == detail::_pseudo_arg
                    && !was_pseudo_arg) {
                was_pseudo_arg = true;
                continue;
            }
            check_abbreviations(parsers, was_pseudo_arg, parsed_arguments, i);
            bool remainder = is_reminder_positional(pos, positional, parsers);
            auto arg = parsed_arguments.at(i);
            auto equals = process_split_equal(arg, parsers);
            auto const tmp
                    = get_optional_arg_by_flag(was_pseudo_arg, parsers, arg);
            if (tmp && !remainder) {
                switch (tmp->action()) {
                    case Action::store :
                        parsers.front().storage.at(tmp).clear();
                        // fallthrough
                    case Action::append :
                    case Action::extend :
                        storage_optional_store(parsers, equals,
                                               parsed_arguments, i,
                                               was_pseudo_arg, arg, tmp);
                        break;
                    case Action::help :
                        process_optional_help(parsers, equals, arg);
                        break;
                    case Action::version :
                        process_optional_version(parsers, equals, arg, tmp);
                        break;
                    default :
                        // Action::store_const :
                        // Action::store_true :
                        // Action::store_false :
                        // Action::append_const :
                        // Action::count :
                        // Action::BooleanOptionalAction :
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
                                        was_pseudo_arg,
                                        intermixed, intermixed_args,
                                        pos, positional, unrecognized_args);
            }
        }
        if (!intermixed_args.empty()) {
            match_args_partial(parsers, pos, positional,
                               unrecognized_args, intermixed_args);
        }
        check_mutex_groups(parsers);
        check_required_args(parsers, pos, positional);
        check_unrecognized_args(only_known, unrecognized_args);
        default_values_post_trigger(parsers.front().storage);
        namespace_post_trigger(parsers, only_known, unrecognized_args);
        return create_namespace(only_known, std::move(parsers.front().storage),
                                std::move(unrecognized_args));
    }

    inline void
    check_namespace(Namespace const& space) const
    {
        if (space.m_unrecognized_args.has_value()
                && !m_data.m_arguments.empty()) {
            auto const& arg = m_data.m_arguments.front();
            auto const& name = !arg->dest().empty() ? arg->dest() : arg->m_name;
            throw
            AttributeError("'tuple' object has no attribute '" + name + "'");
        }
    }

    inline void check_mutex_arguments() const
    {
        for (auto const& group : m_mutex_groups) {
            for (auto const& arg : group.m_data.m_arguments) {
                if (arg->required()) {
                    throw
                    ValueError("mutually exclusive arguments must be optional");
                }
            }
        }
    }

    static void
    check_intermixed_subparser(bool intermixed,
                               std::shared_ptr<Subparser> const& subparser)
    {
        if (intermixed && subparser) {
            throw
            TypeError("parse_intermixed_args: positional arg with nargs=A...");
        }
    }

    static void
    check_intermixed_remainder(bool intermixed,
                               std::vector<pArgument> const& positional)
    {
        if (intermixed
                && std::any_of(std::begin(positional), std::end(positional),
                               [] (pArgument const& arg)
        { return arg->m_nargs == Argument::REMAINDER; })) {
            throw
            TypeError("parse_intermixed_args: positional arg with nargs=...");
        }
    }

    inline std::vector<std::string>
    process_split_equal(std::string& arg, Parsers const& parsers) const
    {
        auto equals
             = detail::_split_equal(arg, parsers.back().parser->prefix_chars());
        if (equals.size() == 2 && !equals.front().empty()) {
            arg = equals.front();
        } else {
            equals.resize(1);
        }
        return equals;
    }

    static Namespace
    create_namespace(bool only_known,
                     Namespace::Storage&& storage,
                     std::vector<std::string>&& unrecognized_args)
    {
        if (only_known) {
            return Namespace(std::move(storage), std::move(unrecognized_args));
        } else {
            return Namespace(std::move(storage));
        }
    }

    static bool
    negative_numbers_presented(std::vector<pArgument> const& optionals,
                               std::string const& prefix_chars)
    {
        if (detail::_is_value_exists(detail::_prefix_char, prefix_chars)) {
            for (auto const& arg : optionals) {
                for (auto const& flag : arg->flags()) {
                    if (detail::_is_negative_number(flag)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    static void validate_arguments(std::vector<pArgument> const& args)
    {
        for (auto const& arg : args) {
            arg->validate();
        }
    }

    inline void validate_argument_value(Parsers const& parsers,
                                        Argument const& arg,
                                        std::string const& value) const
    {
        auto const& choices = arg.m_choices;
        if (choices.has_value()) {
            auto str = detail::_remove_quotes(value);
            if (!str.empty() && !detail::_is_value_exists(str, choices())) {
                auto values = detail::_vector_to_string(choices(), ", ", "'");
                parsers.back().parser->throw_error(
                            "argument " + arg.m_flags.front()
                            + ": invalid choice: '" + str
                            + "' (choose from " + values + ")");
            }
        }
    }

    inline void
    storage_have_value(Parsers& parsers,
                       pArgument const& arg) const
    {
        validate_argument_value(parsers, *arg, std::string());
        parsers.front().storage.have_value(arg);
    }

    inline void
    storage_store_value(Parsers& parsers,
                        pArgument const& arg,
                        std::string const& val) const
    {
        validate_argument_value(parsers, *arg, val);
        parsers.front().storage.store_value(arg, val);
    }

    inline void
    storage_store_values(Parsers& parsers,
                         pArgument const& arg,
                         std::vector<std::string> const& values) const
    {
        for (auto const& val : values) {
            validate_argument_value(parsers, *arg, val);
        }
        parsers.front().storage.store_values(arg, values);
    }

    inline void
    storage_store_n_values(Parsers& parsers,
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

    inline void
    storage_store_default_value(Parsers& parsers, pArgument const& arg) const
    {
        auto const& value = arg->m_default;
        if (value.has_value()) {
            parsers.front().storage.store_default_value(arg, value());
        }
    }

    inline bool
    storage_is_positional_arg_stored(Parsers& parsers,
                                     pArgument const& arg) const
    {
        return parsers.front().storage.self_value_stored(arg);
    }

    void storage_optional_store_func(Parsers& parsers,
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
                        if (tmp->action() == Action::extend) {
                            if (tmp->const_value().empty()) {
                                storage_have_value(parsers, tmp);
                            } else {
                                std::vector<std::string> values;
                                values.reserve(tmp->const_value().size());
                                std::transform(std::begin(tmp->const_value()),
                                               std::end(tmp->const_value()),
                                               std::back_inserter(values),
                                               [] (char c)
                                { return std::string(1, c); });
                                storage_store_values(parsers, tmp, values);
                            }
                        } else {
                            storage_store_value(parsers,
                                                tmp, tmp->const_value());
                        }
                    } else if (tmp->action() == Action::extend) {
                        throw TypeError("'NoneType' object is not iterable");
                    } else {
                        storage_have_value(parsers, tmp);
                    }
                    break;
                default :
                    storage_have_value(parsers, tmp);
                    break;
            }
        } else if (tmp->m_nargs == Argument::NARGS_NUM && n < tmp->m_num_args) {
            parsers.back().parser->throw_error(tmp->error_nargs(arg));
        }
    }

    void storage_optional_store(Parsers& parsers,
                                std::vector<std::string> const& equals,
                                std::vector<std::string> const& arguments,
                                std::size_t& i,
                                bool was_pseudo_arg,
                                std::string const& arg,
                                pArgument const& tmp) const
    {
        if (equals.size() == 1) {
            std::size_t n = 0;
            std::vector<std::string> values;
            do {
                if (++i == arguments.size()) {
                    storage_optional_store_func(parsers, arg, tmp, n);
                    break;
                } else {
                    auto const& next = arguments.at(i);
                    if (next.empty()
                            || tmp->m_nargs == Argument::REMAINDER
                            || detail::_not_optional(
                                next,
                                parsers.back().parser->prefix_chars(),
                                parsers.back().have_negative_args,
                                was_pseudo_arg)) {
                        values.push_back(next);
                        ++n;
                    } else {
                        --i;
                        storage_optional_store_func(parsers, arg, tmp, n);
                        break;
                    }
                }
            } while ((tmp->m_nargs != Argument::NARGS_DEF
                      && tmp->m_nargs != Argument::ZERO_OR_ONE
                      && (tmp->m_nargs != Argument::NARGS_NUM
                          || n != tmp->m_num_args)));
            if (!values.empty()) {
                storage_store_values(parsers, tmp, values);
            }
        } else {
            if (tmp->m_nargs != Argument::NARGS_DEF && tmp->m_num_args > 1) {
                parsers.back().parser->throw_error(tmp->error_nargs(arg));
            }
            storage_store_value(parsers, tmp, equals.back());
        }
    }

    void storage_optional_store_const(Parsers& parsers,
                                      std::vector<std::string> const& equals,
                                      std::string const& arg,
                                      pArgument const& tmp) const
    {
        if (equals.size() == 1) {
            if (tmp->action() == Action::BooleanOptionalAction) {
                bool exist = detail::_is_value_exists(equals.front(),
                                                      tmp->m_flags);
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

    inline void
    process_optional_help(Parsers const& parsers,
                          std::vector<std::string> const& equals,
                          std::string const& arg) const
    {
        if (equals.size() != 1) {
            parsers.back().parser->throw_error(
                        detail::_ignore_explicit(arg, equals.back()));
        }
        // print help and exit
        parsers.back().parser->print_help();
        ::exit(0);
    }

    inline void
    process_optional_version(Parsers const& parsers,
                             std::vector<std::string> const& equals,
                             std::string const& arg,
                             pArgument const& tmp) const
    {
        if (equals.size() != 1) {
            parsers.back().parser->throw_error(
                        detail::_ignore_explicit(arg, equals.back()));
        }
        if (!tmp->m_version.has_value()) {
            throw AttributeError("'ArgumentParser' object has no "
                                 "attribute 'version'");
        }
        std::cout << detail::_replace(tmp->version(), "%(prog)s", prog())
                  << std::endl;
        ::exit(0);
    }

    void match_positional_minimum(Parsers& parsers,
                                  std::deque<std::string>& arguments,
                                  pArgument const& arg) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == Action::BooleanOptionalAction) {
            storage_store_default_value(parsers, arg);
            return;
        }
        if (arg->action() == Action::store) {
            parsers.front().storage.at(arg).clear();
        }
        switch (arg->m_nargs) {
            case Argument::NARGS_DEF :
            case Argument::ONE_OR_MORE :
                storage_store_value(parsers, arg, arguments.front());
                arguments.pop_front();
                break;
            case Argument::ZERO_OR_ONE :
            case Argument::ZERO_OR_MORE :
                storage_store_default_value(parsers, arg);
                break;
            case Argument::NARGS_NUM :
                storage_store_n_values(parsers, arg, arguments,
                                       arg->m_num_args);
                break;
            default :
                break;
        }
    }

    void match_positional_more_zero(Parsers& parsers,
                                    std::deque<std::string>& arguments,
                                    pArgument const& arg,
                                    std::size_t& over_args) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == Action::BooleanOptionalAction) {
            storage_store_default_value(parsers, arg);
            return;
        }
        if (arg->action() == Action::store) {
            parsers.front().storage.at(arg).clear();
        }
        switch (arg->m_nargs) {
            case Argument::NARGS_DEF :
                storage_store_value(parsers, arg, arguments.front());
                arguments.pop_front();
                break;
            case Argument::ONE_OR_MORE :
                storage_store_n_values(parsers, arg, arguments, 1 + over_args);
                over_args = 0;
                break;
            case Argument::ZERO_OR_ONE :
                storage_store_default_value(parsers, arg);
                break;
            case Argument::ZERO_OR_MORE :
            case Argument::REMAINDER :
                if (over_args > 0) {
                    storage_store_n_values(parsers, arg, arguments, over_args);
                    over_args = 0;
                } else if (arg->m_nargs == Argument::ZERO_OR_MORE) {
                    storage_store_default_value(parsers, arg);
                }
                break;
            case Argument::NARGS_NUM :
                storage_store_n_values(parsers, arg, arguments,
                                       arg->m_num_args);
                break;
            default :
                break;
        }
    }

    void match_positional_optional(Parsers& parsers,
                                   std::deque<std::string>& arguments,
                                   pArgument const& arg,
                                   std::size_t& over_args,
                                   std::size_t one_args) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == Action::BooleanOptionalAction) {
            storage_store_default_value(parsers, arg);
            return;
        }
        if (arg->action() == Action::store) {
            parsers.front().storage.at(arg).clear();
        }
        switch (arg->m_nargs) {
            case Argument::NARGS_DEF :
                storage_store_value(parsers, arg, arguments.front());
                arguments.pop_front();
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
            case Argument::NARGS_NUM :
                storage_store_n_values(parsers, arg, arguments,
                                       arg->m_num_args);
                break;
            default :
                break;
        }
    }

    void match_positional_default(Parsers& parsers,
                                  std::deque<std::string>& arguments,
                                  pArgument const& arg) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == Action::BooleanOptionalAction) {
            storage_store_default_value(parsers, arg);
            return;
        }
        if (arg->action() == Action::store) {
            parsers.front().storage.at(arg).clear();
        }
        if (arg->m_nargs == Argument::NARGS_DEF) {
            storage_store_value(parsers, arg, arguments.front());
            arguments.pop_front();
        } else {
            storage_store_n_values(parsers, arg, arguments, arg->m_num_args);
        }
    }

    void match_positionals(Parsers& parsers,
                           std::size_t& pos,
                           std::vector<pArgument> const& positional,
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
                match_positional_minimum(parsers, arguments,
                                         positional.at(pos));
            }
        } else if (more_args) {
            std::size_t over_args = arguments.size() - min_args;
            for ( ; pos < finish; ++pos) {
                match_positional_more_zero(parsers, arguments,
                                           positional.at(pos), over_args);
            }
        } else if (min_args + one_args >= arguments.size()) {
            std::size_t over_args = min_args + one_args - arguments.size();
            for ( ; pos < finish; ++pos) {
                match_positional_optional(parsers, arguments,
                                          positional.at(pos), over_args,
                                          one_args);
            }
        } else {
            for ( ; pos < finish; ++pos) {
                match_positional_default(parsers, arguments,
                                         positional.at(pos));
            }
        }
    }

    static bool
    finish_analyze_positional(pArgument const& arg,
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
            case Argument::ZERO_OR_ONE :
                ++one_args;
                break;
            case Argument::ONE_OR_MORE :
                ++min_amount;
                // fallthrough
            case Argument::ZERO_OR_MORE :
                more_args = true;
                break;
            case Argument::REMAINDER :
                more_args = true;
                break;
            default :
                min_amount += arg->m_num_args;
                break;
        }
        if (min_args + min_amount > args.size()) {
            return true;
        }
        min_args += min_amount;
        if (arg->m_nargs == Argument::REMAINDER && !first && !read_all_args) {
            return true;
        }
        return false;
    }

    void match_args_partial(Parsers& parsers,
                            std::size_t& pos,
                            std::vector<pArgument> const& positional,
                            std::vector<std::string>& unrecognized_args,
                            std::deque<std::string>& args,
                            bool read_all_args = true) const
    {
        if (pos < positional.size()) {
            std::size_t finish = pos;
            std::size_t min_args = 0;
            std::size_t one_args = 0;
            bool more_args = false;
            for ( ; finish < positional.size(); ++finish) {
                if (finish_analyze_positional(positional.at(finish), args,
                                              one_args, more_args, min_args,
                                              finish == pos, read_all_args)) {
                    break;
                }
            }
            match_positionals(parsers, pos, positional, args,
                              finish, min_args, one_args, more_args);
        }
        if (!args.empty()) {
            unrecognized_args.insert(std::end(unrecognized_args),
                                     std::make_move_iterator(std::begin(args)),
                                     std::make_move_iterator(std::end(args)));
        }
    }

    bool try_capture_parser(Parsers& parsers,
                            std::size_t& pos,
                            std::vector<pArgument>& positional,
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
        if (!capture_need && (finish != positional.size()
                              || min_args >= args.size())) {
            match_positionals(parsers, pos, positional, args,
                              finish, min_args, one_args, more_args);
            detail::_insert_vector_to_end(args, unrecognized_args);
            return false;
        }
        match_positionals(parsers, pos, positional, args, finish,
                          ++min_args, one_args, more_args);
        auto const& name = args.front();
        std::string choices;
        auto const& dest = parsers.back().subparser.first->dest();
        for (auto& p : parsers.back().subparser.first->m_parsers) {
            detail::_append_value_to("'" + p->m_name + "'", choices, ", ");
            for (auto const& alias : p->aliases()) {
                detail::_append_value_to("'" + alias + "'", choices, ", ");
            }
            if (p->m_name == name
                    || detail::_is_value_exists(name, p->aliases())) {
                parsers.push_back(ParserInfo(p.get(),
                                             Namespace::Storage(),
                                             p->subparser_info(true, pos)));
                parsers.back().parser->handle(parsers.back().parser->m_name);
                parsers.back().validate();

                if (!dest.empty()) {
                    auto subparser_arg
                            = Argument::make_argument({ dest }, dest,
                                                      Argument::Positional);
                    for (auto& info : parsers) {
                        info.storage.force_add(subparser_arg);
                    }
                    parsers.front().storage.at(subparser_arg).push_back(name);
                }

                for (auto& info : parsers) {
                    info.storage.force_add(
                             parsers.back().parser->m_data.get_arguments(true));
                }

                auto sub_positional = parsers.back()
                        .parser->m_data.get_positional(true, true);
                detail::_move_vector_insert_to(sub_positional, positional, pos);
                args.pop_front();

                return true;
            }
        }
        throw_error("invalid choice: '" + name
                    + "' (choose from " + choices + ")");
        return false;
    }

    void check_abbreviations(Parsers const& parsers,
                             bool was_pseudo_arg,
                             std::vector<std::string>& arguments,
                             std::size_t i) const
    {
        auto& arg = arguments.at(i);
        if (!arg.empty() && !parsers.front().storage.exists(arg)
                && detail::_is_optional(arg,
                                        parsers.back().parser->prefix_chars(),
                                        parsers.back().have_negative_args,
                                        was_pseudo_arg)) {
            auto const& optionals = parsers.back().optional;
            std::vector<std::string> temp;
            if (m_allow_abbrev) {
                bool is_flag_added = false;
                std::string args;
                std::vector<std::string> keys;
                keys.reserve(4);
                for (auto const& opt : optionals) {
                    for (auto const& flag : opt->flags()) {
                        if (detail::_starts_with(flag, arg)) {
                            is_flag_added = true;
                            keys.push_back(flag);
                            detail::_append_value_to(detail::_spaces + flag,
                                                     args, ",");
                            break;
                        }
                        if (flag.size() == 2
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
                                "ambiguous option: '" + arg
                                + "' could match" + args);
                }
                auto const& flag = keys.empty() ? arg : keys.front();
                if (is_flag_added) {
                    temp.push_back(flag);
                } else {
                    auto name = detail::_flag_name(flag);
                    separate_arg_abbrev(parsers, temp, arg, name, optionals);
                }
            } else {
                separate_arg_abbrev(parsers, temp, arg,
                                    detail::_flag_name(arg), optionals);
            }
            detail::_move_vector_replace_at(temp, arguments, i);
        }
    }

    static bool is_reminder_positional(std::size_t pos,
                                       std::vector<pArgument> const& positional,
                                       Parsers const& parsers)
    {
        return pos < positional.size()
                && positional.at(pos)->m_nargs == Argument::REMAINDER
                && !parsers.front().storage.at(positional.at(pos)).empty();
    }

    static pArgument const
    optional_arg_by_flag(Parsers const& parsers, std::string const& key)
    {
        auto const& args = parsers.back().optional;
        auto it = std::find_if(std::begin(args), std::end(args),
                               [&key] (pArgument const& arg)
        { return detail::_is_value_exists(key, arg->flags()); });
        return it != std::end(args) ? *it : nullptr;
    }

    static pArgument const
    get_optional_arg_by_flag(bool was_pseudo_arg,
                             Parsers const& parsers,
                             std::string const& key)
    {
        return was_pseudo_arg ? nullptr : optional_arg_by_flag(parsers, key);
    }

    static bool
    process_separate_arg_abbrev(std::string const& name,
                                std::size_t i,
                                std::vector<std::string>& flags,
                                std::string const& arg,
                                std::vector<pArgument> const& args)
    {
        if (name.at(i) == detail::_equal) {
            if (flags.empty()) {
                flags.emplace_back(std::string());
            }
            flags.back() += name.substr(i);
            return false;
        }
        Argument const* argument = nullptr;
        for (auto const& opt : args) {
            auto it = std::find_if(std::begin(opt->flags()),
                                   std::end(opt->flags()),
                                   [&name, &arg, i] (std::string const& flag)
            {
                return flag.size() == 2
                        && flag.back() == name.at(i)
                        && flag.front() == arg.front();
            });
            if (it != std::end(opt->flags())) {
                flags.push_back(*it);
                argument = opt.get();
                break;
            }
        }
        if (!argument && flags.empty()) {
            flags.push_back(arg);
            return false;
        } else if ((!argument && !flags.empty())
                   || (argument
                       && (argument->action() & detail::_store_action))) {
            auto str = name.substr(i + static_cast<bool>(argument));
            if (!str.empty()) {
                if (!detail::_starts_with(str, detail::_equals)) {
                    flags.back() += detail::_equals;
                }
                flags.back() += str;
            }
            return false;
        }
        return true;
    }

    void separate_arg_abbrev(Parsers const& parsers,
                             std::vector<std::string>& temp,
                             std::string const& arg,
                             std::string const& name,
                             std::vector<pArgument> const& args) const
    {
        if (name.size() + 1 == arg.size()) {
            auto const split
                   = detail::_split_equal(arg,
                                         parsers.back().parser->prefix_chars());
            if (split.size() == 2 && !split.front().empty()
                    && optional_arg_by_flag(parsers, split.front())) {
                temp.push_back(arg);
                return;
            }
            std::vector<std::string> flags;
            flags.reserve(8);
            for (std::size_t i = 0; i < name.size(); ++i) {
                if (!process_separate_arg_abbrev(name, i, flags, arg, args)) {
                    break;
                }
            }
            temp.insert(std::end(temp),
                        std::make_move_iterator(std::begin(flags)),
                        std::make_move_iterator(std::end(flags)));
        } else {
            temp.push_back(arg);
        }
    }

    void
    process_positional_args(std::vector<std::string> const& parsed_arguments,
                            std::size_t& i,
                            Parsers& parsers,
                            bool was_pseudo_arg,
                            bool intermixed,
                            std::deque<std::string>& intermixed_args,
                            std::size_t& pos,
                            std::vector<pArgument>& positional,
                            std::vector<std::string>& unrecognized_args) const
    {
        std::deque<std::string> args;
        args.push_back(parsed_arguments.at(i));
        bool remainder = pos < positional.size()
                && positional.at(pos)->m_nargs == Argument::REMAINDER;
        while (true) {
            if (++i == parsed_arguments.size()) {
                break;
            } else {
                auto const& next = parsed_arguments.at(i);
                if (next.empty()
                        || remainder
                        || detail::_not_optional(
                            next,
                            parsers.back().parser->prefix_chars(),
                            parsers.back().have_negative_args,
                            was_pseudo_arg)) {
                    args.push_back(next);
                } else {
                    --i;
                    break;
                }
            }
        }
        if (intermixed) {
            detail::_insert_vector_to_end(args, intermixed_args);
        } else {
            if (parsers.back().subparser.first) {
                if (try_capture_parser(parsers, pos, positional,
                                       unrecognized_args, args,
                                       i == parsed_arguments.size())) {
                    i -= (i == parsed_arguments.size());
                    i -= args.size();
                }
            } else {
                match_args_partial(parsers, pos, positional, unrecognized_args,
                                   args, i == parsed_arguments.size());
            }
        }
    }

    static void check_mutex_groups(Parsers const& parsers)
    {
        auto _check_mutex_groups = []
               (ArgumentParser const* parser, Namespace::Storage const& storage)
        {
            for (auto const& ex : parser->m_mutex_groups) {
                std::string args;
                std::string found;
                for (auto const& arg : ex.m_data.m_arguments) {
                    auto flags = detail::_vector_to_string(arg->flags(), "/");
                    args += detail::_spaces + flags;
                    if (!storage.at(arg).empty()) {
                        if (!found.empty()) {
                            parser->throw_error("argument " + flags
                                                + ": not allowed with argument "
                                                + found);
                        }
                        found = flags;
                    }
                }
                if (ex.required() && found.empty()) {
                    if (ex.m_data.m_arguments.empty()) {
                        throw IndexError("list index out of range");
                    }
                    parser->throw_error("one of the arguments"
                                        + args + " is required");
                }
            }
        };
        for (auto const& info : parsers) {
            _check_mutex_groups(info.parser, parsers.front().storage);
        }
    }

    static void
    process_optionals_required(std::vector<std::string>& required_args,
                               std::vector<pArgument> const& optional,
                               Namespace::Storage const& storage)
    {
        for (auto const& arg : optional) {
            if (arg->required() && storage.at(arg).empty()) {
                auto args = detail::_vector_to_string(arg->flags(), "/");
                required_args.emplace_back(std::move(args));
            }
        }
    }

    static void
    process_subparser_required(bool required, std::size_t pos,
                               SubparserInfo const& subparser, std::string& res)
    {
        if (required && pos == subparser.second) {
            if (!subparser.first->dest().empty()) {
                detail::_append_value_to(subparser.first->dest(), res, ", ");
            } else {
                detail::_append_value_to(
                            subparser.first->flags_to_string(), res, ", ");
            }
        }
    }

    static void
    process_required_check(ParserInfo const& parser,
                           Namespace::Storage const& storage)
    {
        std::vector<std::string> required_args;
        process_optionals_required(required_args, parser.optional, storage);
        std::string args;
        for (auto const& arg : required_args) {
            detail::_append_value_to(arg, args, ", ");
        }
        if (!args.empty()) {
            parser.parser->throw_error(
                        "the following arguments are required: " + args);
        }
    }

    inline bool
    skip_positional_required_check(Parsers& parsers, pArgument const& arg) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return true;
        }
        if (arg->action() == Action::extend
                && arg->m_nargs == Argument::ZERO_OR_ONE) {
            throw TypeError("'NoneType' object is not iterable");
        }
        if ((arg->m_nargs
             & (Argument::ZERO_OR_ONE | Argument::ZERO_OR_MORE))
                || arg->action() == Action::BooleanOptionalAction) {
            storage_store_default_value(parsers, arg);
            return true;
        }
        if (arg->m_nargs == Argument::REMAINDER) {
            return true;
        }
        return false;
    }

    void
    check_required_args(Parsers& parsers,
                        std::size_t& pos,
                        std::vector<pArgument> const& positional) const
    {
        auto it = parsers.rbegin();
        std::vector<std::string> required_args;
        process_optionals_required(required_args, it->optional,
                                   parsers.front().storage);
        auto const& subparser = it->subparser;
        bool sub_required = subparser.first && subparser.first->required();
        if (!required_args.empty() || pos < positional.size() || sub_required) {
            std::string args;
            for ( ; pos < positional.size(); ++pos) {
                process_subparser_required(sub_required, pos, subparser, args);
                auto const& arg = positional.at(pos);
                if (args.empty()
                        && skip_positional_required_check(parsers, arg)) {
                    continue;
                }
                detail::_append_value_to(arg->m_flags.front(), args, ", ");
            }
            process_subparser_required(sub_required, pos, subparser, args);
            for (auto const& arg : required_args) {
                detail::_append_value_to(arg, args, ", ");
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

    inline void check_unrecognized_args(
            bool only_known, std::vector<std::string> const& args) const
    {
        if (!only_known && !args.empty()) {
            throw_error("unrecognized arguments: "
                        + detail::_vector_to_string(args));
        }
    }

    inline void
    default_values_post_trigger(Namespace::Storage& storage) const
    {
        for (auto it = storage.begin(); it != storage.end(); ) {
            if (!it->second.exists()) {
                auto const& value = it->first->m_default;
                if (it->first->is_suppressed()) {
                    it = storage.erase(it);
                    continue;
                }
                if (it->first->action() != Action::count
                        && it->first->m_type == Argument::Optional) {
                    if (value.has_value()) {
                        it->second.push_default(value());
                    } else if (it->first->action() & detail::_bool_action) {
                        it->second.push_back(value());
                    }
                }
            }
            ++it;
        }
        for (auto const& pair : m_default_values) {
            if (!storage.exists_arg(pair.first)) {
                auto arg = Argument::make_argument(
                            std::vector<std::string>{ pair.first },
                            pair.first, Argument::Positional);
                arg->default_value(pair.second);
                storage.create(arg, { true, { pair.second } });
            }
        }
    }

    static void
    namespace_post_trigger(Parsers& parsers,
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

    static bool
    is_default_stored(std::deque<pArgument>& arguments,
                      std::string const& dest, std::string const& val)
    {
        for (auto& arg : arguments) {
            if (arg->m_type == Argument::Positional) {
                if (detail::_is_value_exists(dest, arg->m_flags)) {
                    arg->default_value(val);
                    return true;
                }
            } else if (!arg->dest().empty()) {
                if (arg->dest() == dest) {
                    arg->default_value(val);
                    return true;
                }
            } else {
                for (auto const& flag : arg->m_flags) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        arg->default_value(val);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    inline std::size_t argument_name_limit() const
    {
        return output_width() - argument_help_limit();
    }

    inline std::size_t argument_help_limit() const
    {
        auto width = output_width();
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

    inline SubparserInfo
    subparser_info(bool add_suppress, std::size_t offset = 0) const
    {
        SubparserInfo res = std::make_pair(m_subparsers, offset);
        for (std::size_t i = 0; i < m_subparsers_position; ++i) {
            res.second += (add_suppress
                           || !m_data.m_positional.at(i)
                                               .first->m_help_type.has_value());
        }
        return res;
    }

    inline std::string subparser_prog_args() const
    {
        std::string result;
        bool add_suppress = false;
        auto info = subparser_info(add_suppress);
        auto const pos = m_data.get_positional(add_suppress, true);
        for (std::size_t i = 0; i < pos.size(); ++i) {
            if (info.second == i) {
                break;
            }
            auto str = pos.at(i)->usage(m_formatter_class);
            detail::_append_value_to(str, result);
        }
        return result;
    }

    inline void
    print_custom_usage(std::vector<pArgument> const& positional,
                       std::vector<pArgument> const& optional,
                       std::deque<MutuallyExclusiveGroup> const& mutex_groups,
                       SubparserInfo const& subparser,
                       std::string const& prog,
                       std::ostream& os) const
    {
        auto const w = output_width();
        std::string head = "usage:";
        std::string head_prog = head + " " + prog;
        std::size_t indent
                = 1 + (w > detail::_min_width ? head_prog : head).size();
        std::string res;
        auto ex_opt = optional;
        for (auto const& ex : mutex_groups) {
            for (auto arg : ex.m_data.m_arguments) {
                ex_opt.erase(std::remove(std::begin(ex_opt),
                                         std::end(ex_opt), arg),
                             std::end(ex_opt));
            }
        }
        auto _arg_usage = [&res] (std::string const& str, bool required)
        {
            if (!res.empty()) {
                res += '\n';
            }
            res += (required ? str : "[" + str + "]");
        };
        for (auto const& arg : ex_opt) {
            _arg_usage(arg->usage(m_formatter_class), arg->required());
        }
        for (auto const& ex : mutex_groups) {
            _arg_usage(ex.usage(m_formatter_class), true);
        }
        for (std::size_t i = 0; i < positional.size(); ++i) {
            if (subparser.first && subparser.second == i) {
                _arg_usage(subparser.first->usage(), true);
            }
            auto const str = positional.at(i)->usage(m_formatter_class);
            if (str.empty()) {
                continue;
            }
            _arg_usage(str, true);
        }
        if (subparser.first && subparser.second == positional.size()) {
            _arg_usage(subparser.first->usage(), true);
        }
        os << detail::_format_output(head_prog, res, 1, indent, w) << std::endl;
    }

    static bool is_subparser_positional(std::shared_ptr<Subparser> const& sub)
    {
        return sub && sub->title().empty() && sub->description().empty();
    }

    static void
    print_subparser(bool need_print, SubparserInfo const& subparser,
                    std::size_t index, _HelpFormatter const& formatter,
                    std::size_t size, std::size_t width, std::ostream& os)
    {
        if (need_print && subparser.second == index) {
            os << subparser.first->print(formatter, size, width) << std::endl;
        }
    }

    static void print_group(pGroup const& group,
                            std::shared_ptr<Subparser> const& subparser,
                            bool is_positional,
                            _HelpFormatter const& formatter,
                            std::string const& prog, std::size_t size,
                            std::size_t width, std::ostream& os)
    {
        if ((subparser && (group != subparser || !is_positional))
                || (!subparser && group != subparser)) {
            group->print_help(os, formatter, prog, size, width);
        }
    }

    inline void update_prog(std::string const& parent_prog)
    {
        m_prog = parent_prog + detail::_spaces + m_name;
    }

    inline void handle(std::string const& str) const
    {
        if (m_handle) {
            m_handle(detail::_remove_quotes(str));
        }
    }

    inline void
    parse_handle(bool only_known,
                 Namespace::Storage const& storage,
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

    _ArgumentData m_data;
    std::string m_name;
    std::string m_prog;
    std::string m_usage;
    std::string m_description;
    std::string m_epilog;
    std::string m_help;
    std::vector<std::string> m_aliases;
    _HelpFormatter m_formatter_class;
    std::string m_prefix_chars;
    std::string m_fromfile_prefix_chars;
    detail::Value<std::string> m_argument_default;
    detail::Value<_SUPPRESS> m_argument_default_type;
    bool m_allow_abbrev;
    bool m_exit_on_error;

    detail::Value<std::size_t> m_output_width;
    std::deque<pGroup> m_groups;
    std::deque<MutuallyExclusiveGroup> m_mutex_groups;
    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
    std::shared_ptr<Subparser> m_subparsers;
    std::size_t m_subparsers_position;

    std::function<void(std::string const&)> m_handle;
    std::function<void(Namespace const&)> m_parse_handle;
};
}  // namespace argparse

#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_EXPERIMENTAL_OPTIONAL
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_USE_CONSTEXPR
#undef _ARGPARSE_USE_FILESYSTEM
#undef _ARGPARSE_USE_OPTIONAL

#endif  // _ARGPARSE_ARGUMENT_PARSER_HPP_
