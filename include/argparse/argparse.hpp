/*
* MIT License
*
* Argument parser for C++11 (ArgumentParser v1.4.0)
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
#define _ARGPARSE_VERSION_MINOR 4
#define _ARGPARSE_VERSION_PATCH 0

#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_USE_CONSTEXPR
#undef _ARGPARSE_USE_FILESYSTEM
#undef _ARGPARSE_USE_OPTIONAL

#include <algorithm>
#include <array>
#include <cctype>
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
#if __cplusplus >= 201703L // C++17+
#include <string_view>
#endif // C++17+
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#else // UNIX
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#endif // _WIN32

// filesystem
#if __cplusplus >= 201703L // C++17+
#if (defined(_MSC_VER) && _MSC_VER >= 1914) \
    || (defined(__clang__) && (__clang_major__ > 8)) \
    || (defined(__GNUC__) && (__GNUC__ > 8))
#include <filesystem>

#define _ARGPARSE_USE_FILESYSTEM 1
#endif //
#endif // C++17+

// optional
#if __cplusplus >= 201703L // C++17+
#include <optional>

#define _ARGPARSE_USE_OPTIONAL 1
#elif __cplusplus >= 201402L // C++14
#if defined(__GNUC__) && (defined(__linux__) || !defined(__clang__))
#include <experimental/optional>
namespace std {
using experimental::optional;
using experimental::fundamentals_v1::nullopt;
} // std

#define _ARGPARSE_USE_OPTIONAL 1
#endif // __GNUC__
#endif // C++14+

#define _ARGPARSE_EXPORT

#if __cplusplus >= 201103L // C++11+
#define _ARGPARSE_CONSTEXPR constexpr
#define _ARGPARSE_USE_CONSTEXPR constexpr
#else
#define _ARGPARSE_CONSTEXPR
#define _ARGPARSE_USE_CONSTEXPR const
#endif // C++11+

#if __cplusplus >= 201703L // C++17+
#define _ARGPARSE_INLINE_VARIABLE inline
#else
#define _ARGPARSE_INLINE_VARIABLE
#endif // C++17+

#if __cplusplus >= 201103L // C++11+
#define _ARGPARSE_NOEXCEPT noexcept
#else
#define _ARGPARSE_NOEXCEPT
#endif // C++11+

namespace argparse {
template <class T>  struct is_byte_type { enum{value = false}; };
template <>         struct is_byte_type<char> { enum{value = true}; };
template <>         struct is_byte_type<signed char> { enum{value = true}; };
template <>         struct is_byte_type<unsigned char> { enum{value = true}; };
#if __cplusplus >= 201703L // C++17+
template <>         struct is_byte_type<std::byte> { enum{value = true}; };
#endif // C++17+
#if __cplusplus >= 202002L // C++20+
template <>         struct is_byte_type<char8_t> { enum{value = true}; };
#endif // C++20+

/*!
 * \brief Action values
 *
 * \enum Action
 */
_ARGPARSE_EXPORT enum Action : uint16_t
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
 * \brief Help formatter values
 *
 * \enum HelpFormatter
 */
_ARGPARSE_EXPORT enum HelpFormatter
{
    RawDescriptionHelpFormatter     = 0x00000001,
    RawTextHelpFormatter            = 0x00000002,
    ArgumentDefaultsHelpFormatter   = 0x00000004,
    MetavarTypeHelpFormatter        = 0x00000008,
};

/*!
 * \brief ArgumentError handler
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
 * \brief AttributeError handler
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
 * \brief ValueError handler
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
 * \brief IndexError handler
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
 * \brief TypeError handler
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

namespace detail {
std::size_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _output_width =80;
std::size_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _minimum_width=33;
std::size_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _output_height=24;
std::size_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _help_minwidth=22;
std::size_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _name_maxwidth=24;
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _default_prefix_char='-';
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
                                                  _default_prefix_chars[] = "-";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _pseudo_argument[] ="--";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _space = ' ';
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _equal = '=';
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _spaces[] = " ";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR _equals[] = "=";
char _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
                                                   _suppress[] = "==SUPPRESS==";

uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_bool_action = Action::store_true | Action::store_false;
uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_store_action = Action::store | Action::append | Action::extend;
uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_const_action = Action::store_const | Action::append_const;
uint32_t _ARGPARSE_INLINE_VARIABLE _ARGPARSE_USE_CONSTEXPR
_store_const_action = _store_action | _const_action;

inline std::pair<std::size_t, std::size_t>
_get_terminal_size()
{
    std::size_t width  = _output_width;
    std::size_t height = _output_height;
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = static_cast<std::size_t>
                (csbi.srWindow.Right - csbi.srWindow.Left + 1);
        height = static_cast<std::size_t>
                (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
        if (width < _minimum_width) {
            width = _minimum_width;
        }
    }
#else // UNIX
#if defined(TIOCGSIZE)
    struct ttysize w;
    if (ioctl(STDOUT_FILENO, TIOCGSIZE, &w) >= 0) {
        width = static_cast<std::size_t>(w.ts_cols);
        height = static_cast<std::size_t>(w.ts_lines);
        if (width < _minimum_width) {
            width = _minimum_width;
        }
    }
#elif defined(TIOCGWINSZ)
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) >= 0) {
        width = static_cast<std::size_t>(w.ws_col);
        height = static_cast<std::size_t>(w.ws_row);
        if (width < _minimum_width) {
            width = _minimum_width;
        }
    }
#endif // TIOCGSIZE
#endif // _WIN32
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
#endif // _ARGPARSE_USE_FILESYSTEM
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

inline std::string
_replace(std::string s, char c, std::string const& value)
{
    auto pos = s.find(c);
    while (pos != std::string::npos) {
        s.replace(pos, 1, value);
        pos = s.find(c, pos + value.size());
    }
    return s;
}

inline bool
_starts_with(std::string const& s, std::string const& value)
{
#if __cplusplus >= 202002L // C++20+
    return s.starts_with(value);
#else
    return s.compare(0, value.size(), value) == 0;
#endif // C++20+
}

template <class T>
bool
_is_value_exists(T const& value, std::vector<T> const& vec) _ARGPARSE_NOEXCEPT
{
    for (auto const& el : vec) {
        if (el == value) {
            return true;
        }
    }
    return false;
}

inline bool
_is_value_exists(char value, std::string const& str) _ARGPARSE_NOEXCEPT
{
    for (auto const& el : str) {
        if (el == value) {
            return true;
        }
    }
    return false;
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
    auto const& prefix
            = _is_value_exists(_default_prefix_char, prefix_chars)
            ? _default_prefix_char : prefix_chars.front();
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
                           [prefix] (char c) -> bool { return c != prefix; });
    if (it != std::end(str)) {
        str.insert(static_cast<std::string::size_type>(
                       std::distance(std::begin(str), it)), "no-");
    } else {
        throw ValueError("can't create no- boolean option");
    }
    return str;
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
_split(std::string const& str, char delim, bool force = false)
{
    std::vector<std::string> result;
    std::string value;
    for (auto c : str) {
        if (c == delim) {
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
_ignore_default(std::string const& arg, std::string const& value)
{
    return "argument " + arg + ": ignored default value '" + value + "'";
}

inline std::string
_ignore_explicit(std::string const& arg, std::string const& value)
{
    return "argument " + arg + ": ignored explicit argument '" + value + "'";
}

inline std::string
_format_output(std::string const& head, std::string const& body,
               std::size_t interlayer, std::size_t indent, std::size_t limit,
               char delimiter = '\n')
{
    std::vector<std::string> result;
    std::string value = head;
    if (value.size() + interlayer > indent) {
        _store_value_to(value, result);
    }
    auto _func = [indent, limit, &result, &value] (std::string const& str)
    {
        if (value.size() > indent && value.size() + 1 + str.size() > limit) {
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
_help_formatter(HelpFormatter formatter,
                std::string const& help)
{
    if (help.empty()) {
        return std::string();
    }
    if (formatter & RawTextHelpFormatter) {
        return help;
    } else {
        std::string help_formatted;
        auto lines = _split(help, '\n');
        for (auto& line : lines) {
            _trim(line);
            if (!line.empty()) {
                if (!help_formatted.empty()) {
                    help_formatted += _space;
                }
                help_formatted += line;
            }
        }
        return help_formatted;
    }
}

inline std::string
_raw_text_formatter(HelpFormatter formatter, std::string const& text)
{
    if (formatter & (RawDescriptionHelpFormatter | RawTextHelpFormatter)) {
        return text;
    }
    auto res = text;
    _trim(res);
    auto lines = _split(res, '\n');
    if (lines.size() > 1) {
        for (auto& line : lines) {
            _trim(line);
        }
        return _vector_to_string(lines);
    }
    return res;
}

inline void
_print_raw_text_formatter(HelpFormatter formatter, std::string const& text,
                          std::ostream& os)
{
    auto formatted_text = _raw_text_formatter(formatter, text);
    if (!formatted_text.empty()) {
        os << "\n" << formatted_text << std::endl;
    }
}

template <class T>
std::string
_type_name()
{
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
    std::string res = __FUNCSIG__;
    auto pos = res.find("__cdecl");
    return res.substr(pos + 27, res.size() - pos - 27 - 7);
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
#endif // __PRETTY_FUNCTION__
}

template <class T>
class Value
{
public:
    explicit Value()
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
} // details

/*!
 * \brief Unspecified values
 *
 * \enum Enum
 */
_ARGPARSE_EXPORT enum Enum : uint8_t
{
    SUPPRESS,
};

/*!
 * \brief Argument class
 */
_ARGPARSE_EXPORT class Argument
{
    friend class ArgumentData;
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class BaseArgumentGroup;
    friend class BaseParser;
    friend class MutuallyExclusiveGroup;
    friend class Namespace;

    enum Nargs : uint8_t
    {
        NARGS_DEF   = 0x01, // ""
        NARGS_NUM   = 0x02, // "N"
        ONE_OR_MORE = 0x04, // "+"
        ZERO_OR_ONE = 0x08, // "?"
        ZERO_OR_MORE= 0x10, // "*"
    };

    enum Type : uint8_t
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
            this->m_flags       = rhs.m_flags;
            this->m_all_flags   = rhs.m_all_flags;
            this->m_name        = rhs.m_name;
            this->m_action      = rhs.m_action;
            this->m_type        = rhs.m_type;
            this->m_default_type= rhs.m_default_type;
            this->m_help_type   = rhs.m_help_type;
            this->m_nargs       = rhs.m_nargs;
            this->m_num_args    = rhs.m_num_args;
            this->m_nargs_str   = rhs.m_nargs_str;
            this->m_const       = rhs.m_const;
            this->m_default     = rhs.m_default;
            this->m_type_name   = rhs.m_type_name;
            this->m_choices     = rhs.m_choices;
            this->m_required    = rhs.m_required;
            this->m_help        = rhs.m_help;
            this->m_metavar     = rhs.m_metavar;
            this->m_dest        = rhs.m_dest;
            this->m_version     = rhs.m_version;
            this->m_handle      = rhs.m_handle;
            this->m_post_trigger= rhs.m_post_trigger;
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
            this->m_flags       = std::move(rhs.m_flags);
            this->m_all_flags   = std::move(rhs.m_all_flags);
            this->m_name        = std::move(rhs.m_name);
            this->m_action      = std::move(rhs.m_action);
            this->m_type        = std::move(rhs.m_type);
            this->m_default_type= std::move(rhs.m_default_type);
            this->m_help_type   = std::move(rhs.m_help_type);
            this->m_nargs       = std::move(rhs.m_nargs);
            this->m_num_args    = std::move(rhs.m_num_args);
            this->m_nargs_str   = std::move(rhs.m_nargs_str);
            this->m_const       = std::move(rhs.m_const);
            this->m_default     = std::move(rhs.m_default);
            this->m_type_name   = std::move(rhs.m_type_name);
            this->m_choices     = std::move(rhs.m_choices);
            this->m_required    = std::move(rhs.m_required);
            this->m_help        = std::move(rhs.m_help);
            this->m_metavar     = std::move(rhs.m_metavar);
            this->m_dest        = std::move(rhs.m_dest);
            this->m_version     = std::move(rhs.m_version);
            this->m_handle      = std::move(rhs.m_handle);
            this->m_post_trigger= std::move(rhs.m_post_trigger);
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
     *  \brief Set argument 'action' value
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
     *  \brief Set argument 'action' value
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
                m_default.clear();
                // fallthrough
            case Action::BooleanOptionalAction :
                m_const = "1";
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::store_false :
                m_default.clear("1");
                // fallthrough
            case Action::store_const :
            case Action::append_const :
                m_const = std::string();
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
    Argument& nargs(uint32_t value)
    {
        switch (m_action) {
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
                               "the append const action may be more appropriate"
                               );
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
    Argument& nargs(std::string const& value)
    {
        if (!(m_action & detail::_store_action)) {
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
     *  \brief Set argument 'const' value
     *
     *  \param value Const value
     *
     *  \return Current argument reference
     */
    Argument& const_value(std::string const& value)
    {
        if ((m_action & detail::_const_action)
                || (m_nargs == ZERO_OR_ONE
                    && (m_action & detail::_store_action))) {
            m_const = detail::_trim_copy(value);
        } else if (m_type == Optional && m_nargs != ZERO_OR_ONE
                   && (m_action & detail::_store_action)) {
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
        return const_value(ss.str());
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
        m_default_type.clear();
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
        m_default_type.clear();
        return *this;
    }

    /*!
     *  \brief Suppress argument 'default' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument reference
     */
    inline Argument& default_value(Enum value)
    {
        if (value != SUPPRESS) {
            throw TypeError("got an unexpected keyword argument 'default'");
        }
        m_default_type = value;
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
        m_type_name = detail::_type_name<T>();
        return *this;
    }

    /*!
     *  \brief Set argument 'type' name (for MetavarTypeHelpFormatter)
     *
     *  \param value Type name value
     *
     *  \return Current argument reference
     */
    Argument& type(std::string const& value)
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
    Argument& choices(std::string const& value)
    {
        if (!(m_action & detail::_store_action)) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        std::vector<std::string> values;
        values.reserve(value.size());
        for (auto const& c : value) {
            values.emplace_back(std::string(1, c));
        }
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
    Argument& choices(std::vector<std::string> const& value)
    {
        if (!(m_action & (detail::_store_action
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
    inline Argument& help(Enum value)
    {
        if (value != SUPPRESS) {
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
    inline Argument& metavar(std::string const& value)
    {
        if (!(m_action & (detail::_store_const_action
                          | Action::BooleanOptionalAction))){
            throw TypeError("got an unexpected keyword argument 'metavar'");
        }
        m_metavar = detail::_trim_copy(value);
        return *this;
    }

    /*!
     *  \brief Set argument 'dest' value for optional arguments
     *
     *  \param value Destination value
     *
     *  \return Current argument reference
     */
    Argument& dest(std::string const& value)
    {
        if (m_type == Positional && !m_flags.empty()) {
            throw ValueError("dest supplied twice for positional argument");
        }
        m_dest.front() = detail::_trim_copy(value);
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
        if (m_action == Action::version) {
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
    inline Argument& handle(std::function<void(std::string)> func)
    {
        if (m_action & (Action::version | Action::help)) {
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
    Argument& handle(std::function<void()> func)
    {
        if (m_action & (Action::version | Action::help)) {
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
     *  \brief Get argument 'action' value
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
    inline std::string const& metavar() const _ARGPARSE_NOEXCEPT
    {
        return m_metavar();
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
        if ((m_action & detail::_const_action) && !m_const.has_value()) {
            throw TypeError("missing 1 required positional argument: 'const'");
        }
        if (std::any_of(std::begin(m_flags), std::end(m_flags),
                        [] (std::string const& flag)
        { return flag == detail::_pseudo_argument; }) && dest().empty()) {
            throw ValueError("dest= is required for options like '--'");
        }
    }

    inline void prepare_action(Action value)
    {
        if (m_action & (Action::version | Action::help)) {
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

    std::string usage(HelpFormatter formatter) const
    {
        std::string res;
        if (m_type == Optional) {
            if (m_action == Action::BooleanOptionalAction) {
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
        if (m_action & (detail::_store_action | Action::append_const)) {
            res += get_nargs_suffix(formatter);
        }
        return res;
    }

    std::string flags_to_string(HelpFormatter formatter) const
    {
        std::string res;
        if (m_type == Optional) {
            for (auto const& flag : flags()) {
                if (!res.empty()) {
                    res += ", ";
                }
                res += flag;
                if (m_action & (detail::_store_action | Action::append_const)) {
                    res += get_nargs_suffix(formatter);
                }
            }
        } else {
            res += get_argument_name(formatter);
        }
        return res;
    }

    std::string print(bool suppress_default,
                      detail::Value<std::string> const& argument_default,
                      HelpFormatter formatter,
                      std::size_t limit,
                      std::size_t width) const
    {
        std::string res = "  " + flags_to_string(formatter);
        auto formatted = detail::_help_formatter(formatter, help());
        if (!formatted.empty()) {
            if (m_type == Optional
                    && (formatter & ArgumentDefaultsHelpFormatter)
                    && !(m_action & (Action::help | Action::version))) {
                auto const& def = (m_default.has_value()
                                   || !argument_default.has_value())
                        ? m_default : argument_default;
                if (m_default_type != SUPPRESS
                        && !(suppress_default && !def.has_value())) {
                    if (!def.has_value() && (m_action & detail::_bool_action)) {
                        formatted += " (default: "
                                + detail::_bool_to_string(def()) + ")";
                    } else {
                        formatted += " (default: "
                                + ((def.has_value() || !def().empty())
                                   ? def() : "None") + ")";
                    }
                }
            }
        }
        return detail::_format_output(res, formatted, 2, limit,
                                      width, detail::_space);
    }

    std::string get_nargs_suffix(HelpFormatter formatter) const
    {
        auto const name = get_argument_name(formatter);
        std::string res;
        if (m_type == Optional && !name.empty()) {
            res += detail::_spaces;
        }
        switch (m_nargs) {
            case ZERO_OR_ONE :
                res += "[" +  name + "]";
                break;
            case ONE_OR_MORE :
                res += name + detail::_spaces;
                // fallthrough
            case ZERO_OR_MORE :
                res += "[" +  name + " ...]";
                break;
            case NARGS_NUM :
                for (uint32_t i = 0; i < m_num_args; ++i) {
                    if (i != 0) {
                        res += detail::_spaces;
                    }
                    res += name;
                }
                break;
            default :
                res += name;
                break;
        }
        return res;
    }

    std::string get_argument_name(HelpFormatter formatter) const
    {
        if ((formatter & MetavarTypeHelpFormatter) && !type_name().empty()) {
            return type_name();
        }
        if (m_metavar.has_value()) {
            return metavar();
        }
        if (m_choices.has_value()) {
            return "{" + detail::_vector_to_string(choices(), ",") + "}";
        }
        auto const& res = dest().empty() ? m_name : dest();
        return m_type == Optional ? detail::_to_upper(res) : res;
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

    inline bool operator ==(Argument const& rhs) const
    {
        return m_flags == rhs.m_flags
                && m_name == rhs.m_name
                && m_type == rhs.m_type
                && m_action == rhs.m_action
                && m_const == rhs.m_const
                && dest() == rhs.dest();
    }

    inline bool operator ==(std::string const& rhs) const _ARGPARSE_NOEXCEPT
    {
        return !dest().empty() ? dest() == rhs
                               : detail::_is_value_exists(rhs, m_flags);
    }

    std::vector<std::string> m_flags;
    std::vector<std::string> m_all_flags;
    std::string m_name;
    Action      m_action;
    Type        m_type;
    detail::Value<Enum> m_default_type;
    detail::Value<Enum> m_help_type;
    Nargs       m_nargs;
    uint32_t    m_num_args;
    std::string m_nargs_str;
    detail::Value<std::string> m_const;
    detail::Value<std::string> m_default;
    detail::Value<std::string> m_type_name;
    detail::Value<std::vector<std::string> > m_choices;
    detail::Value<bool> m_required;
    std::string m_help;
    detail::Value<std::string> m_metavar;
    std::vector<std::string> m_dest;
    detail::Value<std::string> m_version;
    std::function<void(std::string)> m_handle;
    std::function<void(Argument const*)> m_post_trigger;
};

/*!
 * \brief Group class
 */
class Group
{
    friend class ArgumentParser;

protected:
    explicit
    Group()
        : m_title(),
          m_description(),
          m_position()
    { }

    explicit
    Group(std::string const& title, std::string const& description)
        : m_title(title),
          m_description(description),
          m_position()
    { }

public:
    /*!
     *  \brief Destroy group
     */
    virtual ~Group() _ARGPARSE_NOEXCEPT = default;

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
    virtual void limit_help_flags(HelpFormatter formatter,
                                  std::size_t& limit)                 const = 0;
    virtual void print_help(std::ostream& os,
                            bool show_default_value,
                            detail::Value<std::string> const& argument_default,
                            HelpFormatter formatter,
                            std::size_t limit,
                            std::size_t width)                        const = 0;

    std::string m_title;
    std::string m_description;
    std::size_t m_position;
};

/*!
 * \brief ArgumentData class
 */
class ArgumentData
{
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class BaseArgumentGroup;
    friend class BaseParser;
    friend class MutuallyExclusiveGroup;

    explicit
    ArgumentData()
        : m_conflict_handler(),
          m_arguments(),
          m_optional(),
          m_positional()
    { }

protected:
    typedef std::shared_ptr<Argument> pArgument;

public:
    /*!
     *  \brief Destroy argument data
     */
    virtual ~ArgumentData() _ARGPARSE_NOEXCEPT = default;

protected:
    std::vector<pArgument> get_optional(bool add_group) const
    {
        std::vector<pArgument> result;
        result.reserve(m_optional.size());
        for (auto const& pair : m_optional) {
            if (add_group || !pair.second) {
                result.push_back(pair.first);
            }
        }
        return result;
    }

    std::vector<pArgument>
    get_optional_with_help(bool add_group,
                           bool add_help,
                           std::string const& prefix_chars) const
    {
        std::vector<pArgument> result;
        result.reserve(m_optional.size() + add_help);
        if (add_help) {
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
                result.emplace_back(std::move(help));
            }
        }
        for (auto const& pair : m_optional) {
            if (add_group || !pair.second) {
                result.push_back(pair.first);
            }
        }
        return result;
    }

    std::vector<pArgument> get_positional(bool add_group) const
    {
        std::vector<pArgument> result;
        result.reserve(m_positional.size());
        for (auto const& pair : m_positional) {
            if (add_group || !pair.second) {
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
            if (count < count_prefixes) {
                count = count_prefixes;
                flag = std::move(name);
            }
        };
        if (is_optional) {
            _update_flag_name(flag_name, flag_name, prefixes);
        } else if (flags.size() > 1) {
            // no positional multiflag
            throw
            ValueError("invalid option string " + flags.front()
                       + ": must starts with a character '"
                       + prefix_chars + "'");
        }
        for (std::size_t i = 1; i < flags.size(); ++i) {
            // check arguments
            auto& flag = flags.at(i);
            detail::_trim(flag);
            if (flag.empty()) {
                throw IndexError("string index out of range");
            }
            if (!detail::_is_value_exists(flag.front(), prefix_chars)) {
                // no positional and optional args
                throw
                ValueError("invalid option string " + flag
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

    inline void check_conflict_arg(Argument const* arg)
    {
        auto _check_conflict = [this, &arg]
                (std::string const& flag, std::vector<std::string>& flags)
        {
            auto it = std::find(std::begin(flags), std::end(flags), flag);
            if (it != std::end(flags)) {
                if (m_conflict_handler == "resolve") {
                    flags.erase(it);
                } else {
                    throw
                    ArgumentError("argument "
                                  + detail::_vector_to_string(arg->flags(), "/")
                                  + ": conflicting option string: " + flag);
                }
            }
        };
        for (auto const& flag : arg->flags()) {
            for (auto& opt : m_optional) {
                if (opt.first.get() == arg) {
                    continue;
                }
                _check_conflict(flag, opt.first->m_flags);
                _check_conflict(flag, opt.first->m_all_flags);
            }
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
        detail::_trim(flags.front());
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
            detail::_trim(flags.front());
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
            if (arg.m_action & (Action::version | Action::help)) {
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
                         && (arg.m_action & detail::_store_action))
                    && !(arg.m_action & detail::_const_action)) {
                throw TypeError("got an unexpected keyword argument 'const'");
            }
        } else {
            if (arg.m_action == Action::BooleanOptionalAction) {
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
};

/*!
 * \brief BaseArgumentGroup class
 */
class BaseArgumentGroup
{
protected:
    BaseArgumentGroup(std::string& prefix_chars,
                      ArgumentData& parent_data,
                      bool is_mutex_group)
        : m_data(),
          m_prefix_chars(prefix_chars),
          m_parent_data(parent_data),
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
    BaseArgumentGroup(BaseArgumentGroup const& orig)
        : m_data(orig.m_data),
          m_prefix_chars(orig.m_prefix_chars),
          m_parent_data(orig.m_parent_data),
          m_is_mutex_group(orig.m_is_mutex_group)
    { }

    /*!
     *  \brief Destroy basea argument group object
     */
    virtual ~BaseArgumentGroup() _ARGPARSE_NOEXCEPT = default;

    /*!
     *  \brief Copy base argument group object from another argument group
     *
     *  \param rhs Base argument group object to copy
     *
     *  \return Current base argument group reference
     */
    BaseArgumentGroup& operator =(BaseArgumentGroup const& rhs)
    {
        if (this != &rhs) {
            m_data          = rhs.m_data;
            m_prefix_chars  = rhs.m_prefix_chars;
            m_parent_data   = rhs.m_parent_data;
            m_is_mutex_group= rhs.m_is_mutex_group;
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
        m_parent_data.m_arguments.push_back(m_data.m_arguments.back());
        (optional ? m_data.m_optional : m_data.m_positional)
                .push_back(std::make_pair(m_data.m_arguments.back(), true));
        (optional ? m_parent_data.m_optional : m_parent_data.m_positional)
                .push_back(std::make_pair(m_data.m_arguments.back(), true));
    }

    ArgumentData m_data;
    std::string& m_prefix_chars;
    ArgumentData& m_parent_data;

private:
    bool m_is_mutex_group;
};

/*!
 * \brief ArgumentGroup class
 */
_ARGPARSE_EXPORT class ArgumentGroup : public Group, public BaseArgumentGroup
{
    friend class BaseParser;

    explicit
    ArgumentGroup(std::string const& title,
                  std::string const& description,
                  std::string& prefix_chars,
                  ArgumentData& parent_data)
        : Group(title, description),
          BaseArgumentGroup(prefix_chars, parent_data, false)
    { }

    static inline std::shared_ptr<ArgumentGroup>
    make_argument_group(std::string const& title,
                        std::string const& description,
                        std::string& prefix_chars,
                        ArgumentData& parent_data)
    {
        return std::make_shared<ArgumentGroup>
                (ArgumentGroup(title, description, prefix_chars, parent_data));
    }

public:
    using BaseArgumentGroup::add_argument;

    /*!
     *  \brief Create argument group object from another argument group
     *
     *  \param orig Argument group object to copy
     *
     *  \return Argument group object
     */
    ArgumentGroup(ArgumentGroup const& orig)
        : Group(orig),
          BaseArgumentGroup(orig)
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
            m_title         = rhs.m_title;
            m_description   = rhs.m_description;
            m_position      = rhs.m_position;
            m_data          = rhs.m_data;
            m_prefix_chars  = rhs.m_prefix_chars;
            m_parent_data   = rhs.m_parent_data;
        }
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
    limit_help_flags(HelpFormatter formatter, std::size_t& limit) const override
    {
        for (auto const& arg : m_data.m_arguments) {
            auto size = arg->flags_to_string(formatter).size();
            if (limit < size) {
                limit = size;
            }
        }
    }

    void print_help(std::ostream& os,
                    bool suppress_default,
                    detail::Value<std::string> const& argument_default,
                    HelpFormatter formatter,
                    std::size_t limit,
                    std::size_t width) const override
    {
        if (!description().empty() || !m_data.m_arguments.empty()) {
            os << "\n" << title() << ":" << std::endl;
            if (!description().empty()) {
                os << "  " << description() << std::endl;
            }
            if (!m_data.m_arguments.empty()) {
                os << std::endl;
            }
            for (auto const& arg : m_data.m_arguments) {
                os << arg->print(suppress_default, argument_default,
                                 formatter, limit, width) << std::endl;
            }
        }
    }
};

/*!
 * \brief MutuallyExclusiveGroup class
 */
_ARGPARSE_EXPORT class MutuallyExclusiveGroup : public BaseArgumentGroup
{
    friend class ArgumentParser;
    friend class BaseParser;

    explicit
    MutuallyExclusiveGroup(std::string& prefix_chars, ArgumentData& parent_data)
        : BaseArgumentGroup(prefix_chars, parent_data, true),
          m_required(false)
    { }

    static inline MutuallyExclusiveGroup
    make_mutex_group(std::string& prefix_chars, ArgumentData& parent_data)
    {
        return MutuallyExclusiveGroup(prefix_chars, parent_data);
    }

public:
    using BaseArgumentGroup::add_argument;

    /*!
     *  \brief Create mutually exclusive group object from another
     *   mutually exclusive group
     *
     *  \param orig Mutually exclusive group object to copy
     *
     *  \return Mutually exclusive group object
     */
    MutuallyExclusiveGroup(MutuallyExclusiveGroup const& orig)
        : BaseArgumentGroup(orig),
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
            m_data          = rhs.m_data;
            m_prefix_chars  = rhs.m_prefix_chars;
            m_parent_data   = rhs.m_parent_data;
            m_required      = rhs.m_required;
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
    std::string usage(HelpFormatter formatter) const
    {
        std::string res;
        for (auto const& arg : m_data.m_arguments) {
            if (!res.empty()) {
                res += " | ";
            }
            res += arg->usage(formatter);
        }
        return res.empty() ? res : "(" + res + ")";
    }

    bool m_required;
};

// compatibility for version v1.3.8 and earlier
using ExclusiveGroup
#if __cplusplus >= 201402L // C++14+
[[deprecated("Use argparse::MutuallyExclusiveGroup instead.")]]
#endif // C++14
    = MutuallyExclusiveGroup;

/*!
 * \brief BaseParser class
 */
class BaseParser
{
protected:
    typedef std::shared_ptr<Group> pGroup;

    explicit
    BaseParser()
        : m_data(),
          m_usage(),
          m_description(),
          m_epilog(),
          m_prefix_chars(detail::_default_prefix_chars),
          m_groups(),
          m_mutex_groups()
    { }

public:
    /*!
     *  \brief Destroy base parser
     */
    virtual ~BaseParser() _ARGPARSE_NOEXCEPT = default;

    /*!
     *  \brief Get base parser 'usage' value
     *
     *  \return Base parser 'usage' value
     */
    inline std::string const& usage() const _ARGPARSE_NOEXCEPT
    {
        return m_usage;
    }

    /*!
     *  \brief Get base parser 'description' value
     *
     *  \return Base parser 'description' value
     */
    inline std::string const& description() const _ARGPARSE_NOEXCEPT
    {
        return m_description;
    }

    /*!
     *  \brief Get base parser 'epilog' value
     *
     *  \return Base parser 'epilog' value
     */
    inline std::string const& epilog() const _ARGPARSE_NOEXCEPT
    {
        return m_epilog;
    }

    /*!
     *  \brief Get base parser 'prefix_chars' value
     *
     *  \return Base parser 'prefix_chars' value
     */
    inline std::string const& prefix_chars() const _ARGPARSE_NOEXCEPT
    {
        return m_prefix_chars;
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
        auto group = ArgumentGroup::make_argument_group(title, description,
                                                        m_prefix_chars, m_data);
        m_groups.push_back(group);
        return *group;
    }

    /*!
     *  \brief Add mutually exclusive group
     *
     *  \return Current mutually exclusive group reference
     */
    inline MutuallyExclusiveGroup& add_mutually_exclusive_group()
    {
        m_mutex_groups.emplace_back(
              MutuallyExclusiveGroup::make_mutex_group(m_prefix_chars, m_data));
        return m_mutex_groups.back();
    }

protected:
    inline void process_add_argument()
    {
        bool optional = m_data.m_arguments.back()->m_type == Argument::Optional;
        (optional ? m_data.m_optional : m_data.m_positional)
                .push_back(std::make_pair(m_data.m_arguments.back(), false));
    }

    ArgumentData m_data;
    std::string m_usage;
    std::string m_description;
    std::string m_epilog;
    std::string m_prefix_chars;
    std::deque<pGroup> m_groups;
    std::deque<MutuallyExclusiveGroup> m_mutex_groups;
};

/*!
 * \brief Object with parsed arguments
 */
_ARGPARSE_EXPORT class Namespace
{
    typedef std::shared_ptr<Argument> pArgument;

    class Storage
    {
        friend class ArgumentParser;

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

        explicit
        Storage()
            : m_data(),
              m_conflict_arg()
        { }

        void
        force_add(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->m_action & (Action::version | Action::help)) {
                return;
            }
            auto const& arg_flags = key->get_argument_flags();
            for (auto& pair : m_data) {
                pair.first->resolve_conflict_flags(arg_flags);
            }
            m_data.push_back(std::make_pair(key, value));
        }

        template <class T>
        void force_add(T const& arguments)
        {
            for (auto const& arg : arguments) {
                force_add(arg);
            }
        }

        void
        try_add(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->m_action & (Action::version | Action::help)) {
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

        void
        create(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->m_action & (Action::version | Action::help)) {
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
            at(arg).push_values({ });
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

        void store_default_value(key_type const& arg, std::string const& value)
        {
            if (arg->m_action
                    & (Action::store | Action::BooleanOptionalAction)) {
                auto& arg_data = at(arg);
                if (arg_data.empty()) {
                    arg_data.push_default(value);
                }
            }
        }

        bool self_value_stored(key_type const& arg)
        {
            if (arg->m_action & (Action::store_const | detail::_bool_action)) {
                auto& arg_data = at(arg);
                if (arg_data.empty()) {
                    arg_data.push_back(arg->m_const());
                }
                arg->handle(arg->m_const());
                return true;
            } else if (arg->m_action == Action::append_const) {
                at(arg).push_back(arg->m_const());
                arg->handle(arg->m_const());
                return true;
            } else if (arg->m_action == Action::count) {
                at(arg).emplace_back(std::string());
                arg->handle(std::string());
                return true;
            }
            return false;
        }

        inline bool exists(std::string const& key) const
        {
            return std::find_if(std::begin(m_data), std::end(m_data),
                                [key] (value_type const& pair) -> bool
            { return *(pair.first) == key; }) != std::end(m_data);
        }

        inline bool exists(key_type const& key) const
        {
            auto it = std::find_if(std::begin(m_data), std::end(m_data),
                                   [key] (value_type const& pair) -> bool
            { return pair.first == key; });
            return it != std::end(m_data);
        }

        value_type const& at(std::string const& key) const
        {
            auto it = std::find_if(std::begin(m_data), std::end(m_data),
                                   [key] (value_type const& pair) -> bool
            { return *(pair.first) == key; });
            if (it == std::end(m_data)) {
                throw std::logic_error("key '" + key + "' not found");
            }
            return *it;
        }

        inline mapped_type& at(key_type const& key)
        {
            auto it = std::find_if(std::begin(m_data), std::end(m_data),
                                   [key] (value_type const& pair) -> bool
            { return pair.first == key; });
            if (it == std::end(m_data)) {
                throw std::logic_error("key '" + key->m_name + "' not found");
            }
            return it->second;
        }

        inline mapped_type const& at(key_type const& key) const
        {
            auto it = std::find_if(std::begin(m_data), std::end(m_data),
                                   [key] (value_type const& pair) -> bool
            { return pair.first == key; });
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
        std::string const&
        conflict_arg(key_type const& arg) const _ARGPARSE_NOEXCEPT
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

    template <class T>
    struct is_stl_array:std::false_type{};
    template <class T, std::size_t N>
    struct is_stl_array<std::array                 <T, N> >   :std::true_type{};

    template <class T>
    struct is_stl_container:std::false_type{};
    template <class... Args>
    struct is_stl_container<std::deque             <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::forward_list      <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::list              <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::multiset          <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::priority_queue    <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::set               <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::vector            <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::unordered_multiset<Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_container<std::unordered_set     <Args...> >:std::true_type{};

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
    struct is_stl_matrix<std::deque<std::deque   <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::forward_list<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::list    <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::multiset<Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::priority_queue<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::set     <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::vector  <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::unordered_multiset<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::deque<std::unordered_set<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::deque    <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::forward_list<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::list     <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::multiset <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::priority_queue<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::set      <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::vector   <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::unordered_multiset<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::list<std::unordered_set<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::deque  <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::forward_list<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::list   <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::multiset<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::priority_queue<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::set    <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::vector <Args...> > >:std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::unordered_multiset<Args...> > >
                                                              :std::true_type{};
    template <class... Args>
    struct is_stl_matrix<std::vector<std::unordered_set<Args...> > >
                                                              :std::true_type{};

    template <class T, typename U = void>
    struct is_stl_pair:std::false_type{};
    template <class T>
    struct is_stl_pair<T, void_t<typename T::first_type,
                                    typename T::second_type> >:std::true_type{};

    template <class T>
    struct is_stl_queue:std::false_type{};
    template <class... Args>
    struct is_stl_queue<std::stack                 <Args...> >:std::true_type{};
    template <class... Args>
    struct is_stl_queue<std::queue                 <Args...> >:std::true_type{};

    template <class T>
    struct is_stl_tuple:std::false_type{};
    template <class... Args>
    struct is_stl_tuple<std::tuple                 <Args...> >:std::true_type{};

    template <class T> struct type_tag {};

    template <std::size_t... Is>
    struct seq{};

    template <std::size_t N, std::size_t... Is>
    struct gen_seq:gen_seq<N - 1, N - 1, Is...>{};

    template <std::size_t... Is>
    struct gen_seq<0, Is...>:seq<Is...>{};

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
    bool exists(std::string const& key) const
    {
        if (m_storage.exists(key)) {
            return !m_storage.at(key).second.empty()
                    || m_storage.at(key).first->m_action == Action::count;
        }
        for (auto const& pair : m_storage) {
            if (pair.first->m_type == Argument::Optional
                    && pair.first->dest().empty()) {
                for (auto const& flag : pair.first->m_flags) {
                    if (detail::_flag_name(flag) == key) {
                        return !pair.second.empty()
                                || pair.first->m_action == Action::count;
                    }
                }
            }
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
                            || is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<T>());
        if (args.first->m_action == Action::count) {
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
                            && !is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<T>());
        if (args.first->m_action == Action::count) {
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
        is_stl_array<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<typename T::value_type>());
        if (args.first->m_action == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_vector<typename T::value_type>(args.second());
        T res{};
        if (res.size() != vector.size()) {
            std::cerr << "error: array size mismatch: " << res.size()
                      << ", expected " << vector.size() << std::endl;
        }
#ifdef min
        std::copy_n(std::begin(vector),
                    min(res.size(), vector.size()), std::begin(res));
#else
        std::copy_n(std::begin(vector),
                    std::min(res.size(), vector.size()), std::begin(res));
#endif // min
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
        is_stl_container<typename std::decay<T>::type>::value
        && !is_stl_matrix<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<typename T::value_type>());
        if (args.first->m_action == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_vector<typename T::value_type>(args.second());
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
        is_stl_map<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<typename T::mapped_type>());
        if (args.first->m_action == Action::count) {
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
     *  \brief Get parsed argument value for 2D deque, list, vector
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        is_stl_matrix<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<typename T::value_type>());
        if (args.first->m_action != Action::append
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
     *  \brief Get parsed argument value for pair types
     *
     *  \param key Argument name
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        is_stl_pair<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<T>());
        if (args.first->m_action == Action::count) {
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
        is_stl_queue<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<typename T::value_type>());
        if (args.first->m_action == Action::count) {
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
        is_stl_tuple<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<T>());
        if (args.first->m_action == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        if (args.second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            return to_tuple(type_tag<T>{}, args.second());
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return to_tuple(type_tag<T>{},
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
        && !is_byte_type<T>::value
        && !is_stl_array<typename std::decay<T>::type>::value
        && !is_stl_container<typename std::decay<T>::type>::value
        && !is_stl_map<typename std::decay<T>::type>::value
        && !is_stl_matrix<typename std::decay<T>::type>::value
        && !is_stl_pair<typename std::decay<T>::type>::value
        && !is_stl_queue<typename std::decay<T>::type>::value
        && !is_stl_tuple<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::_type_name<T>());
        if (args.first->m_action == Action::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        return to_type<T>(detail::_vector_to_string(args.second()));
    }

    /*!
     *  \brief Print namespace to output stream
     *
     *  \param os Output stream
     */
    void print(std::ostream& os = std::cout) const
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
        switch (args.first->m_action) {
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
                    return detail::_bool_to_string(args.first->m_default());
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
        switch (args.first->m_action) {
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
    std::string to_string() const
    {
        std::string result;
        for (auto const& pair : m_storage) {
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
                = detail::_vector_to_string(m_unrecognized_args(), ", ", "'");
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
    std::optional<typename std::enable_if<
        std::is_constructible<std::string, T>::value
        || std::is_floating_point<T>::value
        || std::is_same<bool, T>::value
        || is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || args->second.empty()
                || args->second.size() != 1
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::_type_name<T>())) {
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
    std::optional<typename std::enable_if<std::is_integral<T>::value
                                          && !std::is_same<bool, T>::value
                                          && !is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::_type_name<T>())) {
            return {};
        }
        if (args->first->m_action == Action::count) {
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
    std::optional<typename std::enable_if<
        is_stl_array<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(),
                        detail::_type_name<typename T::value_type>())) {
            return {};
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
        if (!vector.operator bool()) {
            return {};
        }
        T res{};
        if (res.size() != vector->size()) {
            std::cerr << "error: array size mismatch: " << res.size()
                      << ", expected " << vector.size() << std::endl;
        }
#ifdef min
        std::copy_n(std::begin(vector.value()),
                    min(res.size(), vector->size()), std::begin(res));
#else
        std::copy_n(std::begin(vector.value()),
                    std::min(res.size(), vector->size()), std::begin(res));
#endif // min
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
    std::optional<typename std::enable_if<
        is_stl_container<typename std::decay<T>::type>::value
        && !is_stl_matrix<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(),
                        detail::_type_name<typename T::value_type>())) {
            return {};
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
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
    std::optional<typename std::enable_if<
        is_stl_map<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(),
                        detail::_type_name<typename T::mapped_type>())) {
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
     *  \brief Try get parsed argument value for 2D deque, list, vector.
     *  If invalid type, argument not exists, not parsed or can't be parsed,
     *  returns std::nullopt.
     *
     *  \param key Argument name
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    std::optional<typename std::enable_if<
        is_stl_matrix<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action != Action::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_name_correct(
                        args->first->type_name(),
                        detail::_type_name<typename T::value_type>())) {
            return {};
        }
        T res{};
        for (auto const& vec : args->second.matrix()) {
            auto vector= try_to_vector<typename T::value_type::value_type>(vec);
            if (!vector.operator bool()) {
                return {};
            }
            res.push_back(typename T::value_type(std::begin(vector.value()),
                                                 std::end(vector.value())));
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
    std::optional<typename std::enable_if<
        is_stl_pair<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::_type_name<T>())) {
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
    std::optional<typename std::enable_if<
        is_stl_queue<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(),
                        detail::_type_name<typename T::value_type>())) {
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
    std::optional<typename std::enable_if<
        is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::_type_name<T>())) {
            return {};
        }
        if (args->second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            return try_to_tuple(type_tag<T>{}, args->second());
        }
        if (args->second.size() != 1) {
            return {};
        }
        return try_to_tuple(type_tag<T>{},
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
    std::optional<typename std::enable_if<
        !std::is_constructible<std::string, T>::value
        && !std::is_floating_point<T>::value
        && !std::is_integral<T>::value
        && !std::is_same<bool, T>::value
        && !is_byte_type<T>::value
        && !is_stl_array<typename std::decay<T>::type>::value
        && !is_stl_container<typename std::decay<T>::type>::value
        && !is_stl_map<typename std::decay<T>::type>::value
        && !is_stl_matrix<typename std::decay<T>::type>::value
        && !is_stl_pair<typename std::decay<T>::type>::value
        && !is_stl_queue<typename std::decay<T>::type>::value
        && !is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->m_action == Action::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::_type_name<T>())) {
            return {};
        }
        return try_to_type<T>(detail::_vector_to_string(args->second()));
    }
#endif // _ARGPARSE_USE_OPTIONAL

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
        return detail::_vector_to_string(m_unrecognized_args(), detail::_spaces,
                                         std::string(), true);
    }

private:
    inline std::string
    boolean_option_to_args(std::string const& key,
                           Storage::value_type const& args) const
    {
        if (args.second.empty()) {
            return detail::_bool_to_string(args.first->m_default());
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return args.second.front() == args.first->m_const()
                ? detail::_bool_to_string(args.second.front())
                : args.second.front();
    }

    inline std::string
    boolean_option_to_string(std::string const& key,
                             Storage::value_type const& args,
                             std::string const& quotes) const
    {
        if (args.second.empty()) {
            if (args.first->m_action == Action::BooleanOptionalAction) {
                return std::string("None");
            } else {
                return detail::_bool_to_string(args.first->m_default());
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
        if ((args.first->m_action == Action::store
             && (args.first->m_nargs
                 & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE)))
                || (!args.second.exists()
                    && args.first->m_type == Argument::Optional)
                || args.second.is_default()) {
            return detail::_vector_to_string(args.second(), ", ",
                                             quotes, false, "None");
        }
        if (args.first->m_action != Action::append
                || (args.first->m_nargs
                    & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE))) {
            std::string none
                    = args.first->m_nargs == Argument::ZERO_OR_MORE
                    || (args.first->m_action == Action::extend
                        && args.first->m_nargs == Argument::ZERO_OR_ONE)
                    ? "" : "None";
            return detail::_vector_to_string(args.second(), ", ",
                                             quotes, false, none, "[", "]");
        } else {
            std::string none = args.first->m_nargs
                                == Argument::ZERO_OR_MORE ? "" : "None";
            return detail::_matrix_to_string(args.second.matrix(), ", ",
                                             quotes, false, none, "[", "]");
        }
    }

    Storage::value_type const& data(std::string const& key) const
    {
        if (m_storage.exists(key)) {
            return m_storage.at(key);
        }
        for (auto const& pair : m_storage) {
            if (pair.first->m_type == Argument::Optional
                    && pair.first->dest().empty()) {
                for (auto const& flag : pair.first->m_flags) {
                    if (detail::_flag_name(flag) == key) {
                        return pair;
                    }
                }
            }
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
        for (auto const& arg : args) {
            vec.emplace_back(to_type<T>(arg));
        }
        return vec;
    }

    template <class... Ts, std::size_t... Idxs>
    std::tuple<Ts...>
    parse_tuple(std::vector<std::string> const& values, seq<Idxs...>) const
    {
        return std::make_tuple(to_type<Ts>(values[Idxs])...);
    }

    template <class... Ts>
    std::tuple<Ts...>
    to_tuple(type_tag<std::tuple<Ts...>>,
             std::vector<std::string> const& values) const
    {
        return parse_tuple<Ts...>(values, gen_seq<sizeof...(Ts)>());
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
    typename std::enable_if<is_byte_type<T>::value, T>::type
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
                            && !is_byte_type<T>::value, T>::type
    to_type(std::string const& data) const
    {
        if (data.empty()) {
            return T();
        }
        T result;
        std::stringstream ss(detail::_remove_quotes(data));
        ss >> result;
        if (ss.fail() || !ss.eof()) {
            throw TypeError("can't convert value '" + data + "'");
        }
        return result;
    }

#ifdef _ARGPARSE_USE_OPTIONAL
    std::optional<Storage::value_type>
    try_get_data(std::string const& key) const
    {
        if (m_storage.exists(key)) {
            return m_storage.at(key);
        }
        for (auto const& pair : m_storage) {
            if (pair.first->m_type == Argument::Optional
                    && pair.first->dest().empty()) {
                for (auto const& flag : pair.first->m_flags) {
                    if (detail::_flag_name(flag) == key) {
                        return pair;
                    }
                }
            }
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
        vec.reserve(args.size());
        for (auto const& arg : args) {
            auto pair = try_to_pair<T, U>(arg, delim);
            if (pair.operator bool()) {
                vec.emplace_back(pair.value());
            } else {
                return {};
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
    try_to_tuple(type_tag<std::tuple<Ts...>>,
                 std::vector<std::string> const& values) const
    {
        try {
            auto res = parse_tuple<Ts...>(values, gen_seq<sizeof...(Ts)>());
            return res;
        } catch (...) {
            return {};
        }
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
    std::optional<typename std::enable_if<is_byte_type<T>::value, T>::type>
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
        && !is_byte_type<T>::value, T>::type>
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
#endif // _ARGPARSE_USE_OPTIONAL

    Storage m_storage;
    detail::Value<std::vector<std::string> > m_unrecognized_args;
};

/*!
 * \brief ArgumentParser objects
 */
_ARGPARSE_EXPORT class ArgumentParser : public BaseParser
{
    typedef std::shared_ptr<Argument> pArgument;

public:
    using BaseParser::usage;
    using BaseParser::description;
    using BaseParser::epilog;
    using BaseParser::prefix_chars;
    using BaseParser::add_argument;

    // compatibility for version v1.3.3 and earlier
    using Namespace
#if __cplusplus >= 201402L // C++14+
    [[deprecated("Use argparse::Namespace instead.")]]
#endif // C++14
        = argparse::Namespace;

    /*!
     * \brief Parser class
     */
    class Parser : public BaseParser
    {
        friend class ArgumentParser;

        explicit
        Parser(std::string const& name)
            : BaseParser(),
              m_name(name),
              m_help(),
              m_prog(),
              m_handle(),
              m_parse_handle()
        { }

    public:
        using BaseParser::usage;
        using BaseParser::description;
        using BaseParser::epilog;
        using BaseParser::prefix_chars;
        using BaseParser::add_argument;

        /*!
         *  \brief Set parser 'usage' value
         *
         *  \param param Usage value
         *
         *  \return Current parser reference
         */
        inline Parser& usage(std::string const& param)
        {
            m_usage = detail::_trim_copy(param);
            return *this;
        }

        /*!
         *  \brief Set parser 'description' value
         *
         *  \param param Description value
         *
         *  \return Current parser reference
         */
        inline Parser& description(std::string const& param)
        {
            m_description = param;
            return *this;
        }

        /*!
         *  \brief Set parser 'epilog' value
         *
         *  \param param Epilog value
         *
         *  \return Current parser reference
         */
        inline Parser& epilog(std::string const& param)
        {
            m_epilog = param;
            return *this;
        }

        /*!
         *  \brief Set parser 'prefix_chars' value
         *
         *  \param param Prefix chars values
         *
         *  \return Current parser reference
         */
        inline Parser& prefix_chars(std::string const& param)
        {
            auto value = detail::_trim_copy(param);
            if (!value.empty()) {
                m_prefix_chars = std::move(value);
            }
            return *this;
        }

        /*!
         *  \brief Set parser 'help' message
         *
         *  \param value Help message
         *
         *  \return Current parser reference
         */
        inline Parser& help(std::string const& value)
        {
            m_help = value;
            return *this;
        }

        /*!
         *  \brief Set parser 'handle' function.
         *  Called when the parser is executed and passed the value of the
         *  parser
         *
         *  \param func Handle function
         *
         *  \return Current parser reference
         */
        inline Parser&
        handle(std::function<void(std::string)> func) _ARGPARSE_NOEXCEPT
        {
            m_handle = func;
            return *this;
        }

        /*!
         *  \brief Set parser 'handle' function.
         *  Called when the parser is executed.
         *
         *  \param func Handle function
         *
         *  \return Current parser reference
         */
        inline Parser& handle(std::function<void()> func) _ARGPARSE_NOEXCEPT
        {
            m_handle = [func] (std::string const&) { func(); };
            return *this;
        }

        /*!
         *  \brief Set parser 'handle' function.
         *  Called when the parser is executed and passed the namespace
         *  of the parser
         *
         *  \param func Parse handle function
         *
         *  \return Current parser reference
         */
        inline Parser&
        handle(std::function<void(argparse::Namespace const&)> func)
                                                              _ARGPARSE_NOEXCEPT
        {
            m_parse_handle = func;
            return *this;
        }

        /*!
         *  \brief Add argument
         *
         *  \param argument Argument
         *
         *  \return Current parser reference
         */
        template <typename = void>
        inline Parser& add_argument(Argument const& argument)
        {
            m_data.validate_argument(Argument(argument), m_prefix_chars);
            process_add_argument();
            return *this;
        }

        /*!
         *  \brief Get parser 'help' message
         *
         *  \return Parser 'help' message
         */
        inline std::string const& help() const _ARGPARSE_NOEXCEPT
        {
            return m_help;
        }

    private:
        inline void handle(std::string const& str) const
        {
            if (m_handle) {
                m_handle(detail::_remove_quotes(str));
            }
        }

        inline std::string print(HelpFormatter formatter,
                                 std::size_t limit, std::size_t width) const
        {
            return detail::_format_output(
                        "    " + m_name,
                        detail::_help_formatter(formatter, help()),
                        2, limit, width, detail::_space);
        }

        std::string m_name;
        std::string m_help;
        std::string m_prog;
        std::function<void(std::string)> m_handle;
        std::function<void(argparse::Namespace const&)> m_parse_handle;
    };

    /*!
     * \brief Subparser class
     */
    class Subparser : public Group
    {
        friend class ArgumentParser;

        explicit
        Subparser()
            : Group(),
              m_prog(),
              m_dest(),
              m_required(false),
              m_help(),
              m_metavar(),
              m_parsers()
        { }

        static inline std::shared_ptr<Subparser>
        make_subparser()
        {
            return std::make_shared<Subparser>(Subparser());
        }

    public:
        using Group::title;
        using Group::description;

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
            m_dest = detail::_trim_copy(value);
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
         *  \brief Add parser with name
         *
         *  \param name Parser name
         *
         *  \return Current parser reference
         */
        inline Parser& add_parser(std::string const& name)
        {
            m_parsers.emplace_back(Parser(name));
            return m_parsers.back();
        }

    private:
        void limit_help_flags(HelpFormatter, std::size_t& limit) const override
        {
            auto size = flags_to_string().size();
            if (limit < size) {
                limit = size;
            }
            for (auto const& arg : m_parsers) {
                auto s = arg.m_name.size() + 2;
                if (limit < s) {
                    limit = s;
                }
            }
        }

        void print_help(std::ostream& os,
                        bool,
                        detail::Value<std::string> const&,
                        HelpFormatter formatter,
                        std::size_t limit,
                        std::size_t width) const override
        {
            os << "\n" << (title().empty() ? "subcommands" : title()) << ":\n";
            if (!description().empty()) {
                os << "  " << description() << "\n\n";
            }
            os << print(formatter, limit, width) << std::endl;
        }

        inline std::string usage() const
        {
            return flags_to_string() + " ...";
        }

        std::string flags_to_string() const
        {
            if (m_metavar.has_value()) {
                return metavar();
            }
            std::string res;
            for (auto const& parser : m_parsers) {
                if (!res.empty()) {
                    res += ",";
                }
                res += parser.m_name;
            }
            return "{" + res + "}";
        }

        inline std::string print(HelpFormatter formatter,
                                 std::size_t limit, std::size_t width) const
        {
            auto res = detail::_format_output(
                        "  " + flags_to_string(),
                        detail::_help_formatter(formatter, help()),
                        2, limit, width, detail::_space);
            return std::accumulate(std::begin(m_parsers), std::end(m_parsers),
                                   res, [formatter, limit, width]
                                   (std::string const& str, Parser const& p)
            { return str + "\n" + p.print(formatter, limit, width); });
        }

        std::string m_prog;
        std::string m_dest;
        bool        m_required;
        std::string m_help;
        detail::Value<std::string> m_metavar;
        std::deque<Parser> m_parsers;
    };

private:
    typedef std::pair<std::shared_ptr<Subparser>, std::size_t> SubparserInfo;

public:
    /*!
     *  \brief Construct argument parser with concrete program name
     *
     *  \param prog Program value
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(std::string const& prog = "untitled")
        : BaseParser(),
          m_prog(prog),
          m_parents(),
          m_formatter_class(),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_argument_default_type(),
          m_output_width(),
          m_add_help(true),
          m_allow_abbrev(true),
          m_exit_on_error(true),
          m_default_values(),
          m_parsed_arguments(),
          m_subparsers(nullptr)
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
     *  \brief Destroy argument parser
     */
    ~ArgumentParser() _ARGPARSE_NOEXCEPT = default;

    /*!
     *  \brief Set argument parser 'prog' value
     *
     *  \param param Program value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& prog(std::string const& param)
    {
        auto value = detail::_trim_copy(param);
        if (!value.empty()) {
            m_prog = value;
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
     *  \brief Set argument parser 'parents' value
     *
     *  \param param Parents values
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& parents(std::vector<ArgumentParser> const& param)
    {
        m_parents = param;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param param HelpFormatter value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    formatter_class(HelpFormatter param) _ARGPARSE_NOEXCEPT
    {
        m_formatter_class = param;
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
    formatter_class(HelpFormatter param, Args... args) _ARGPARSE_NOEXCEPT
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
    add_formatter_class(HelpFormatter param) _ARGPARSE_NOEXCEPT
    {
        m_formatter_class
                = static_cast<HelpFormatter>(m_formatter_class | param);
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
    add_formatter_class(HelpFormatter param, Args... args) _ARGPARSE_NOEXCEPT
    {
        add_formatter_class(param);
        return add_formatter_class(args...);
    }

    /*!
     *  \brief Set argument parser 'prefix_chars' value
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
        m_argument_default_type.clear();
        return *this;
    }

    /*!
     *  \brief Suppress argument parser 'argument_default' value
     *
     *  \param value argparse::SUPPRESS
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& argument_default(Enum value)
    {
        if (value != SUPPRESS) {
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
     *  \brief Set output width value (default auto-detected or 80, min 33)
     *
     *  \param value Output width
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& output_width(std::size_t value) _ARGPARSE_NOEXCEPT
    {
        m_output_width = value;
        if (m_output_width() < detail::_minimum_width) {
            m_output_width = detail::_minimum_width;
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'add_help' value
     *
     *  \param value Add help flag
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& add_help(bool value) _ARGPARSE_NOEXCEPT
    {
        m_add_help = value;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'allow_abbrev' value
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
     *  \brief Set argument parser 'exit_on_error' value
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
     *  \brief Get argument parser 'prog' value
     *
     *  \return Argument parser 'prog' value
     */
    inline std::string const& prog() const _ARGPARSE_NOEXCEPT
    {
        return m_prog;
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
     *  \brief Get output width value (default auto-detected or 80, min 33)
     *
     *  \return Output width value
     */
    inline std::size_t output_width() const
    {
        return m_output_width.has_value() ? m_output_width()
                                          : detail::_get_terminal_size().first;
    }

    /*!
     *  \brief Get argument parser 'add_help' value
     *
     *  \return Argument parser 'add_help' value
     */
    inline bool add_help() const _ARGPARSE_NOEXCEPT
    {
        return m_add_help;
    }

    /*!
     *  \brief Get argument parser 'allow_abbrev' value
     *
     *  \return Argument parser 'allow_abbrev' value
     */
    inline bool allow_abbrev() const _ARGPARSE_NOEXCEPT
    {
        return m_allow_abbrev;
    }

    /*!
     *  \brief Get argument parser 'exit_on_error' value
     *
     *  \return Argument parser 'exit_on_error' value
     */
    inline bool exit_on_error() const _ARGPARSE_NOEXCEPT
    {
        return m_exit_on_error;
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
        m_data.validate_argument(Argument(argument), m_prefix_chars);
        process_add_argument();
        return *this;
    }

    /*!
     *  \brief Add subparsers
     *
     *  \return Current subparser reference
     */
    Subparser& add_subparsers()
    {
        if (m_subparsers) {
            throw_error("cannot have multiple subparser arguments");
        }
        for (auto const& parent : m_parents) {
            if (parent.m_subparsers) {
                throw_error("cannot have multiple subparser arguments");
            }
        }
        m_subparsers = Subparser::make_subparser();
        m_subparsers->m_position = m_data.m_positional.size();
        m_groups.push_back(m_subparsers);
        return *m_subparsers;
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
        auto const positional = positional_arguments(true, true);
        auto const optional = optional_arguments(true, true).second;
        for (auto const& arg : positional) {
            if (detail::_is_value_exists(dest, arg->m_flags)) {
                return default_argument_value(*arg)();
            }
        }
        bool suppress_default = m_argument_default_type == SUPPRESS;
        auto _arg_suppressed = [this, suppress_default] (pArgument const& arg)
        {
            return arg->m_default_type == argparse::SUPPRESS
                    || (suppress_default
                        && !default_argument_value(*arg).has_value());
        };
        for (auto const& arg : optional) {
            if (!arg->dest().empty()) {
                if (arg->dest() == dest) {
                    if (_arg_suppressed(arg)) {
                        return detail::_suppress;
                    }
                    return default_argument_value(*arg)();
                }
            } else {
                for (auto const& flag : arg->m_flags) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        if (_arg_suppressed(arg)) {
                            return detail::_suppress;
                        }
                        return default_argument_value(*arg)();
                    }
                }
            }
        }
        auto it = std::find_if(
                    std::begin(m_default_values), std::end(m_default_values),
                    [dest] (std::pair<std::string, std::string> const& pair)
        { return pair.first == dest; });
        return it != std::end(m_default_values) ? it->second :std::string();
    }

    /*!
     *  \brief Set default values for certain arguments
     *
     *  \param pairs Vector of pairs: { 'argument flag', 'default value' }
     */
    void
    set_defaults(std::vector<std::pair<std::string, std::string> > const& pairs)
    {
        for (auto const& pair : pairs) {
            auto dest = detail::_trim_copy(pair.first);
            if (dest.empty()) {
                continue;
            }
            auto value = detail::_trim_copy(pair.second);
            auto it = std::begin(m_parents);
            for ( ; it != std::end(m_parents)
                  && !is_default_stored(it->m_data.m_arguments, dest, value);
                  ++it) {
            }
            if (it == std::end(m_parents)
                    && !is_default_stored(m_data.m_arguments, dest, value)) {
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
    argparse::Namespace
    parse_args(argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_args(T const& args,
               argparse::Namespace const& space = argparse::Namespace()) const
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
    inline argparse::Namespace
    parse_args(std::vector<std::string> const& args,
               argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_known_args(
            argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_known_args(
            T const& args,
            argparse::Namespace const& space = argparse::Namespace()) const
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
    inline argparse::Namespace
    parse_known_args(
            std::vector<std::string> const& args,
            argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_intermixed_args(
            argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_intermixed_args(
            T const& args,
            argparse::Namespace const& space = argparse::Namespace()) const
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
    inline argparse::Namespace
    parse_intermixed_args(
            std::vector<std::string> const& args,
            argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_known_intermixed_args(
            argparse::Namespace const& space = argparse::Namespace()) const
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
    argparse::Namespace
    parse_known_intermixed_args(
            T const& args,
            argparse::Namespace const& space = argparse::Namespace()) const
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
    inline argparse::Namespace
    parse_known_intermixed_args(
            std::vector<std::string> const& args,
            argparse::Namespace const& space = argparse::Namespace()) const
    {
        return on_parse_arguments(args, true, true, space);
    }

    /*!
     *  \brief Print a brief description of how the ArgumentParser
     *  should be invoked on the command line
     *
     *  \param os Output stream
     */
    void print_usage(std::ostream& os = std::cout) const
    {
        if (!usage().empty()) {
            os << "usage: " << usage() << std::endl;
        } else {
            auto const positional = positional_arguments(false, true);
            auto const optional = optional_arguments(false, true).second;
            print_custom_usage(positional, optional, m_mutex_groups,
                               subpurser_info(false), m_prog, os);
        }
    }

    /*!
     *  \brief Print a help message, including the program usage and information
     *  about the arguments registered with the ArgumentParser
     *
     *  \param os Output stream
     */
    void print_help(std::ostream& os = std::cout) const
    {
        auto const positional_all = positional_arguments(false, true);
        auto const optional_all = optional_arguments(false, true).second;
        auto const positional = positional_arguments(false, false);
        auto const optional = optional_arguments(false, false);
        print_custom_help(positional_all, optional_all, positional,
                          optional.second, optional.first, m_groups,
                          m_mutex_groups, subpurser_info(false), m_prog,
                          m_usage, m_description, m_epilog, os);
    }

    /*!
     *  \brief Return a string containing a brief description of how
     *  the ArgumentParser should be invoked on the command line
     *
     *  \return Usage format
     */
    inline std::string format_usage() const
    {
        std::stringstream ss;
        print_usage(ss);
        return detail::_trim_copy(ss.str());
    }

    /*!
     *  \brief Return a string containing a help message, including the program
     *  usage and information about the arguments registered with the
     *  ArgumentParser
     *
     *  \return Help format
     */
    inline std::string format_help() const
    {
        std::stringstream ss;
        print_help(ss);
        return detail::_trim_copy(ss.str());
    }

    /*!
     *  \brief This method terminates the program, exiting with the specified
     *  status and, if given, it prints a message before that
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
     *  \brief This method prints a usage message including the message to the
     *  standard error and terminates the program with a status code of 2
     *
     *  \param message Error message
     */
    inline void error(std::string const& message) const
    {
        print_usage(std::cerr);
        std::cerr << m_prog << ": error: " << message << std::endl;
        ::exit(2);
    }

    /*!
     *  \brief Arguments that are read from a file
     *  (see the fromfile_prefix_chars function)
     *  are read one argument per line.
     *  convert_arg_line_to_args() can be overridden for fancier reading
     *
     *  \param file File name
     *
     *  \return Arguments to parse
     */
    virtual std::vector<std::string>
    convert_arg_line_to_args(std::string const& file) const
    {
        std::ifstream is(file);
        if (!is.is_open()) {
            throw_error("[Errno 2] No such file or directory: '" + file + "'");
        }
        std::vector<std::string> res;
        std::string line;
        while (std::getline(is, line)) {
            res.push_back(line);
        }
        is.close();
        return res;
    }

protected:
    inline void
    throw_error(std::string const& message, std::ostream& os = std::cerr) const
    {
        print_usage(os);
        throw std::logic_error(m_prog + ": error: " + message);
    }

private:
    argparse::Namespace
    on_parse_arguments(std::vector<std::string> const& args,
                       bool only_known,
                       bool intermixed,
                       argparse::Namespace const& space) const
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

    argparse::Namespace
    parse_arguments(std::vector<std::string> const& in_parsed_arguments,
                    bool only_known,
                    bool intermixed,
                    argparse::Namespace const& space) const
    {
        auto parsed_arguments = in_parsed_arguments;
        if (!space.unrecognized_args().empty()) {
            auto const& args = space.unrecognized_args();
            parsed_arguments.insert(std::end(parsed_arguments),
                                    std::begin(args), std::end(args));
        }
        auto const subparser = subpurser_info();
        if (intermixed && subparser.first) {
            throw
            TypeError("parse_intermixed_args: positional arg with nargs=A...");
        }
        Parser* parser = nullptr;
        auto _custom_error = [this]
         (Parser const* p, std::string const& err, std::ostream& os = std::cerr)
        {
            if (p) {
                if (!p->m_usage.empty()) {
                    os << "usage: " << p->m_usage << std::endl;
                } else {
                    print_custom_usage(p->m_data.get_positional(true),
                                       p->m_data.get_optional_with_help(
                                           true, m_add_help, p->m_prefix_chars),
                                       p->m_mutex_groups,
                                       std::make_pair(nullptr, 0),
                                       p->m_prog, os);
                }
                throw std::logic_error(p->m_prog + ": error: " + err);
            } else {
                throw_error(err, os);
            }
        };
        auto _throw_error = [_custom_error, &parser]
                (std::string const& error, std::ostream& os = std::cerr)
        {
            _custom_error(parser, error, os);
        };
        auto _validate_argument_value = [_throw_error]
                (Argument const& arg, std::string const& value)
        {
            auto const& choices = arg.m_choices;
            if (choices.has_value()) {
                auto str = detail::_remove_quotes(value);
                if (!str.empty() && !detail::_is_value_exists(str, choices())) {
                    auto values
                            = detail::_vector_to_string(choices(), ", ", "'");
                    _throw_error("argument " + arg.m_flags.front()
                                 + ": invalid choice: '" + str
                                 + "' (choose from " + values + ")");
                }
            }
        };
        auto _negative_numbers_presented = []
                (std::vector<pArgument> const& optionals,
                std::string const& prefix_chars)
        {
            if (detail::_is_value_exists(detail::_default_prefix_char,
                                         prefix_chars)) {
                for (auto const& arg : optionals) {
                    for (auto const& flag : arg->flags()) {
                        if (detail::_is_negative_number(flag)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        auto positional = positional_arguments(true, true);
        auto const optional = optional_arguments(true, true).second;
        std::vector<pArgument> sub_optional;
        std::string subparser_dest;
        std::vector<std::string> subparser_flags;

        validate_arguments(positional);
        validate_arguments(optional);
        validate_subparser(subparser.first, subparser_dest, subparser_flags);

        bool have_negative_args
                = _negative_numbers_presented(optional, m_prefix_chars);
        bool was_pseudo_arg = false;

        auto subparser_name = subparser_dest;
        auto subparser_arg = Argument::make_argument(std::move(subparser_flags),
                                                     std::move(subparser_name),
                                                     Argument::Positional);
        argparse::Namespace::Storage storage = space.storage();
        if (space.m_storage.m_data.empty()) {
            storage.force_add(positional);
            storage.create(optional);
        } else {
            storage.force_add(positional);
            storage.try_add(optional);
        }
        argparse::Namespace::Storage sub_storage;

        auto _optional_arg_by_flag = [&optional, &sub_optional, &parser]
                (std::string const& key) -> pArgument const
        {
            for (auto const& arg : (parser ? sub_optional : optional)) {
                if (detail::_is_value_exists(key, arg->flags())) {
                    return arg;
                }
            }
            return nullptr;
        };
        auto _prefix_chars = [this, &parser] () -> std::string const&
        {
            return parser ? parser->m_prefix_chars : m_prefix_chars;
        };

        std::vector<std::string> unrecognized_args;
        std::deque<std::string> intermixed_args;

        std::size_t pos = 0;
        auto _have_value
                = [_validate_argument_value, &storage] (pArgument const& arg)
        {
            _validate_argument_value(*arg, std::string());
            storage.have_value(arg);
        };
        auto _store_value = [_validate_argument_value, &storage]
                (pArgument const& arg, std::string const& val)
        {
            _validate_argument_value(*arg, val);
            storage.store_value(arg, val);
        };
        auto _store_values = [_validate_argument_value, &storage]
                (pArgument const& arg, std::vector<std::string> const& values)
        {
            for (auto const& val : values) {
                _validate_argument_value(*arg, val);
            }
            storage.store_values(arg, values);
        };
        auto _store_default_value = [this, &storage] (pArgument const& arg)
        {
            auto const& value = default_argument_value(*arg);
            if (value.has_value()) {
                storage.store_default_value(arg, value());
            }
        };
        auto _is_positional_arg_stored = [_throw_error, &storage]
                (pArgument const& arg) -> bool
        {
            if (arg->m_action == Action::append_const
                    && arg->m_default.has_value()) {
                _throw_error(detail::_ignore_default(arg->m_flags.front(),
                                                     arg->m_default()));
            }
            return storage.self_value_stored(arg);
        };
        auto _match_positionals
                = [_is_positional_arg_stored, _store_value, _store_values,
                   _store_default_value, &pos]
                (std::vector<pArgument> const& positional,
                std::deque<std::string>& arguments, std::size_t finish,
                std::size_t min_args, std::size_t one_args, bool more_args)
        {
            auto _store_first_value = [_store_value]
                    (pArgument const& arg, std::deque<std::string>& arguments)
            {
                _store_value(arg, arguments.front());
                arguments.pop_front();
            };
            auto _store_n_values = [_store_values] (pArgument const& arg,
                    std::deque<std::string>& arguments, std::size_t n)
            {
                std::vector<std::string> values;
                values.reserve(n);
                for (std::size_t i = 0; i < n; ++i) {
                    values.push_back(arguments.front());
                    arguments.pop_front();
                }
                 _store_values(arg, values);
            };
            if (min_args == arguments.size()) {
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg->m_action == Action::BooleanOptionalAction) {
                        _store_default_value(arg);
                        continue;
                    }
                    switch (arg->m_nargs) {
                        case Argument::NARGS_DEF :
                        case Argument::ONE_OR_MORE :
                            _store_first_value(arg, arguments);
                            break;
                        case Argument::ZERO_OR_ONE :
                        case Argument::ZERO_OR_MORE :
                            _store_default_value(arg);
                            break;
                        case Argument::NARGS_NUM :
                            _store_n_values(arg, arguments, arg->m_num_args);
                            break;
                        default :
                            break;
                    }
                }
            } else if (more_args) {
                std::size_t over_args = arguments.size() - min_args;
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg->m_action == Action::BooleanOptionalAction) {
                        _store_default_value(arg);
                        continue;
                    }
                    switch (arg->m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_first_value(arg, arguments);
                            break;
                        case Argument::ONE_OR_MORE :
                            _store_n_values(arg, arguments, 1 + over_args);
                            over_args = 0;
                            break;
                        case Argument::ZERO_OR_ONE :
                            _store_default_value(arg);
                            break;
                        case Argument::ZERO_OR_MORE :
                            if (over_args > 0) {
                                _store_n_values(arg, arguments, over_args);
                                over_args = 0;
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_NUM :
                            _store_n_values(arg, arguments, arg->m_num_args);
                            break;
                        default :
                            break;
                    }
                }
            } else if (min_args + one_args >= arguments.size()) {
                std::size_t over_args = min_args + one_args - arguments.size();
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg->m_action == Action::BooleanOptionalAction) {
                        _store_default_value(arg);
                        continue;
                    }
                    switch (arg->m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_first_value(arg, arguments);
                            break;
                        case Argument::ZERO_OR_ONE :
                            if (over_args < one_args) {
                                _store_first_value(arg, arguments);
                                ++over_args;
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_NUM :
                            _store_n_values(arg, arguments, arg->m_num_args);
                            break;
                        default :
                            break;
                    }
                }
            } else {
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg->m_action == Action::BooleanOptionalAction) {
                        _store_default_value(arg);
                        continue;
                    }
                    if (arg->m_nargs == Argument::NARGS_DEF) {
                        _store_first_value(arg, arguments);
                    } else {
                        _store_n_values(arg, arguments, arg->m_num_args);
                    }
                }
            }
        };
        auto _try_capture_parser
                = [this, _match_positionals, _negative_numbers_presented,
                &have_negative_args, &pos, &positional, &storage, &subparser,
                &subparser_arg, &subparser_dest, &sub_optional, &sub_storage,
                &unrecognized_args]
                (std::deque<std::string>& args, Parser*& parser)
        {
            std::size_t finish = pos;
            std::size_t min_args = 0;
            std::size_t one_args = 0;
            bool more_args = false;
            bool capture_need = false;
            for ( ; finish < positional.size(); ++finish) {
                if (finish == subparser.second) {
                    if (min_args + 1 > args.size()) {
                        break;
                    }
                    capture_need = true;
                    break;
                }
                auto const& arg = positional.at(finish);
                if (!(arg->m_action & detail::_store_action)) {
                    continue;
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
                    default :
                        min_amount += arg->m_num_args;
                        break;
                }
                if (min_args + min_amount > args.size()) {
                    break;
                }
                min_args += min_amount;
            }
            if (!capture_need && (finish != positional.size()
                                  || min_args >= args.size())) {
                if (finish != pos) {
                    _match_positionals(positional, args, finish, min_args,
                                       one_args, more_args);
                }
                if (!args.empty()) {
                    unrecognized_args.insert(std::end(unrecognized_args),
                                             std::begin(args), std::end(args));
                }
                return;
            }
            _match_positionals(positional, args, finish, ++min_args,
                               one_args, more_args);
            auto const& name = args.front();
            std::string choices;
            for (auto& p : subparser.first->m_parsers) {
                if (!choices.empty()) {
                    choices += ", ";
                }
                choices += "'" + p.m_name + "'";
                if (p.m_name == name) {
                    parser = &p;
                    if (!subparser_dest.empty()) {
                        storage.force_add(subparser_arg);
                        storage.at(subparser_arg).push_back(name);
                        sub_storage.force_add(subparser_arg);
                    }
                    break;
                }
            }
            if (parser) {
                sub_optional
                        = parser->m_data
                            .get_optional_with_help(true, m_add_help,
                                                    parser->m_prefix_chars);
                auto sub_positional = parser->m_data.get_positional(true);
                if (!sub_positional.empty()) {
                    using dtype = std::vector<pArgument>::difference_type;
                    positional.insert(std::next(std::begin(positional),
                                                static_cast<dtype>(
                                                    subparser.second)),
                                      std::make_move_iterator(
                                          std::begin(sub_positional)),
                                      std::make_move_iterator(
                                          std::end(sub_positional)));
                }
                args.pop_front();
                have_negative_args
                        = _negative_numbers_presented(sub_optional,
                                                      parser->m_prefix_chars);

                bool add_suppress = false;
                auto info = subpurser_info(add_suppress);
                auto program = m_subparsers->m_prog;
                if (program.empty()) {
                    program = m_prog;
                }
                auto const positional_args
                        = positional_arguments(add_suppress, true);
                for (std::size_t i = 0; i < positional_args.size(); ++i) {
                    if (info.second == i) {
                        break;
                    }
                    auto str = positional_args.at(i)->usage(m_formatter_class);
                    if (!str.empty()) {
                        program += detail::_spaces + str;
                    }
                }
                parser->m_prog = program + detail::_spaces + parser->m_name;
                parser->handle(parser->m_name);
                storage.force_add(parser->m_data.m_arguments);
                sub_storage.force_add(parser->m_data.m_arguments);
            } else {
                throw_error("invalid choice: '" + name
                            + "' (choose from " + choices + ")");
            }
        };
        auto _match_args_partial
                = [_match_positionals, &pos, &positional, &unrecognized_args]
                (std::deque<std::string>& args)
        {
            if (pos < positional.size()) {
                std::size_t finish = pos;
                std::size_t min_args = 0;
                std::size_t one_args = 0;
                bool more_args = false;
                for ( ; finish < positional.size(); ++finish) {
                    auto const& arg = positional.at(finish);
                    if (!(arg->m_action & detail::_store_action)) {
                        continue;
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
                        default :
                            min_amount += arg->m_num_args;
                            break;
                    }
                    if (min_args + min_amount > args.size()) {
                        break;
                    }
                    min_args += min_amount;
                }
                if (finish != pos) {
                    _match_positionals(positional, args, finish,
                                       min_args, one_args, more_args);
                }
            }
            if (!args.empty()) {
                unrecognized_args.insert(std::end(unrecognized_args),
                                         std::begin(args), std::end(args));
            }
        };
        auto _separate_arg_abbrev = [_optional_arg_by_flag, _prefix_chars]
                (std::vector<std::string>& temp, std::string const& arg,
                std::string const& name, std::vector<pArgument> const& args)
        {
            if (name.size() + 1 == arg.size()) {
                auto const split = detail::_split_equal(arg, _prefix_chars());
                if (split.size() == 2 && !split.front().empty()
                        && _optional_arg_by_flag(split.front())) {
                    temp.push_back(arg);
                    return;
                }
                std::vector<std::string> flags;
                flags.reserve(8);
                for (std::size_t i = 0; i < name.size(); ++i) {
                    if (name.at(i) == detail::_equal) {
                        if (flags.empty()) {
                            flags.emplace_back(std::string());
                        }
                        flags.back() += name.substr(i);
                        break;
                    }
                    Argument const* argument = nullptr;
                    for (auto const& opt : args) {
                        for (auto const& flag : opt->flags()) {
                            if (flag.size() == 2 && flag.back() == name.at(i)) {
                                flags.push_back(flag);
                                argument = opt.get();
                                break;
                            }
                        }
                    }
                    if (!argument && flags.empty()) {
                        flags.push_back(arg);
                        break;
                    } else if ((!argument && !flags.empty())
                               || (argument && (argument->m_action
                                                & detail::_store_action))) {
                        auto str = name.substr(i + bool(argument));
                        if (!str.empty()) {
                            if (!detail::_starts_with(str, detail::_equals)) {
                                flags.back() += detail::_equals;
                            }
                            flags.back() += str;
                        }
                        break;
                    }
                }
                temp.insert(std::end(temp),
                            std::make_move_iterator(std::begin(flags)),
                            std::make_move_iterator(std::end(flags)));
            } else {
                temp.push_back(arg);
            }
        };
        auto _check_abbreviations
                = [this, _separate_arg_abbrev, _throw_error, _prefix_chars,
                &storage, &have_negative_args, &was_pseudo_arg]
                (std::vector<std::string>& arguments, std::size_t i,
                std::vector<pArgument> const& optionals)
        {
            auto& arg = arguments.at(i);
            if (!arg.empty() && !storage.exists(arg)
                    && detail::_is_optional(arg,
                                            _prefix_chars(),
                                            have_negative_args,
                                            was_pseudo_arg)) {
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
                                if (!args.empty()) {
                                    args += ",";
                                }
                                args += detail::_spaces + flag;
                                break;
                            }
                            if (flag.size() == 2
                                    && detail::_starts_with(arg, flag)) {
                                keys.push_back(arg);
                                if (!args.empty()) {
                                    args += ",";
                                }
                                args += detail::_spaces + flag;
                                break;
                            }
                        }
                    }
                    if (keys.size() > 1) {
                        _throw_error("ambiguous option: '" + arg
                                     + "' could match" + args);
                    }
                    if (is_flag_added) {
                        temp.push_back(keys.empty() ? arg : keys.front());
                    } else {
                        auto name = detail::_flag_name(keys.empty()
                                                       ? arg : keys.front());
                        _separate_arg_abbrev(temp, arg, name, optionals);
                    }
                } else {
                    _separate_arg_abbrev(temp, arg, detail::_flag_name(arg),
                                         optionals);
                }
                using dtype = std::vector<std::string>::difference_type;
                arguments.erase(std::next(std::begin(arguments),
                                          static_cast<dtype>(i)));
                arguments.insert(std::next(std::begin(arguments),
                                           static_cast<dtype>(i)),
                                 std::make_move_iterator(std::begin(temp)),
                                 std::make_move_iterator(std::end(temp)));
            }
        };
        for (std::size_t i = 0; i < parsed_arguments.size(); ++i) {
            if (parsed_arguments.at(i) == detail::_pseudo_argument
                    && !was_pseudo_arg) {
                was_pseudo_arg = true;
                continue;
            }
            check_load_args(parsed_arguments, i, was_pseudo_arg);
            _check_abbreviations(parsed_arguments, i, parser ? sub_optional
                                                             : optional);
            auto arg = parsed_arguments.at(i);
            auto splitted = detail::_split_equal(arg, _prefix_chars());
            if (splitted.size() == 2 && !splitted.front().empty()) {
                arg = splitted.front();
            } else {
                splitted.resize(1);
            }
            auto const tmp = (was_pseudo_arg ? nullptr
                                             : _optional_arg_by_flag(arg));
            auto _store_func = [_throw_error, _have_value,
                    _store_value, _store_values, &arg, &tmp] (uint32_t n)
            {
                if (n == 0) {
                    switch (tmp->m_nargs) {
                        case Argument::NARGS_DEF :
                        case Argument::NARGS_NUM :
                        case Argument::ONE_OR_MORE :
                            _throw_error(tmp->error_nargs(arg));
                            break;
                        case Argument::ZERO_OR_ONE :
                            if (tmp->m_const.has_value()) {
                                if (tmp->m_action == Action::extend) {
                                    if (tmp->m_const().empty()) {
                                        _have_value(tmp);
                                    } else {
                                        std::vector<std::string> values;
                                        values.reserve(tmp->m_const().size());
                                        for (auto c : tmp->m_const()) {
                                            values.push_back(std::string(1, c));
                                        }
                                        _store_values(tmp, values);
                                    }
                                } else {
                                    _store_value(tmp, tmp->const_value());
                                }
                            } else if (tmp->m_action == Action::extend) {
                                throw
                                TypeError("'NoneType' object is not iterable");
                            } else {
                                _have_value(tmp);
                            }
                            break;
                        default :
                            _have_value(tmp);
                            break;
                    }
                } else if (tmp->m_nargs == Argument::NARGS_NUM
                           && n < tmp->m_num_args) {
                    _throw_error(tmp->error_nargs(arg));
                }
            };
            if (tmp) {
                switch (tmp->m_action) {
                    case Action::store :
                        storage.at(tmp).clear();
                        // fallthrough
                    case Action::append :
                    case Action::extend :
                        if (splitted.size() == 1) {
                            uint32_t n = 0;
                            std::vector<std::string> values;
                            do {
                                if (++i == parsed_arguments.size()) {
                                    _store_func(n);
                                    break;
                                } else {
                                    auto const& next = parsed_arguments.at(i);
                                    if (next.empty()
                                            || detail::_not_optional(
                                                next,
                                                _prefix_chars(),
                                                have_negative_args,
                                                was_pseudo_arg)) {
                                        values.push_back(next);
                                        ++n;
                                    } else {
                                        --i;
                                        _store_func(n);
                                        break;
                                    }
                                }
                            } while ((tmp->m_nargs != Argument::NARGS_DEF
                                      && tmp->m_nargs != Argument::ZERO_OR_ONE
                                      && (tmp->m_nargs != Argument::NARGS_NUM
                                          || n != tmp->m_num_args)));
                            if (!values.empty()) {
                                _store_values(tmp, values);
                            }
                        } else {
                            if (tmp->m_nargs != Argument::NARGS_DEF
                                    && tmp->m_num_args > 1) {
                                _throw_error(tmp->error_nargs(arg));
                            }
                            _store_value(tmp, splitted.back());
                        }
                        break;
                    case Action::store_const :
                    case Action::store_true :
                    case Action::store_false :
                    case Action::append_const :
                    case Action::count :
                    case Action::BooleanOptionalAction :
                        if (splitted.size() == 1) {
                            if (tmp->m_action == Action::append_const
                                    && !tmp->m_default().empty()) {
                                _throw_error(detail::_ignore_default(
                                                 tmp->m_flags.front(),
                                                 tmp->m_default()));
                            }
                            if (tmp->m_action == Action::BooleanOptionalAction){
                                bool exist = detail::_is_value_exists(
                                            splitted.front(), tmp->m_flags);
                                storage.store_value(tmp, exist ? tmp->m_const()
                                                               : std::string());
                            } else {
                                storage.self_value_stored(tmp);
                            }
                        } else {
                            _throw_error(detail::_ignore_explicit(
                                             arg, splitted.back()));
                        }
                        break;
                    case Action::help :
                        if (splitted.size() == 1) {
                            print_help_and_exit(parser);
                        } else {
                            _throw_error(detail::_ignore_explicit(
                                             arg, splitted.back()));
                        }
                        break;
                    case Action::version :
                        if (splitted.size() == 1) {
                            if (!tmp->m_version.has_value()) {
                                throw
                                AttributeError("'ArgumentParser' object has no "
                                               "attribute 'version'");
                            }
                            std::cout << tmp->version() << std::endl;
                            ::exit(0);
                        } else {
                            _throw_error(detail::_ignore_explicit(
                                             arg, splitted.back()));
                        }
                        break;
                    default :
                        _throw_error("action not supported");
                        break;
                }
            } else if (!arg.empty()
                       && detail::_is_optional(arg,
                                               _prefix_chars(),
                                               have_negative_args,
                                               was_pseudo_arg)) {
                unrecognized_args.push_back(arg);
            } else {
                std::deque<std::string> args;
                args.push_back(parsed_arguments.at(i));
                while (true) {
                    if (++i == parsed_arguments.size()) {
                        break;
                    } else {
                        auto const& next = parsed_arguments.at(i);
                        if (next.empty()
                                || detail::_not_optional(next,
                                                         _prefix_chars(),
                                                         have_negative_args,
                                                         was_pseudo_arg)) {
                            args.push_back(next);
                        } else {
                            --i;
                            break;
                        }
                    }
                }
                if (intermixed) {
                    if (!args.empty()) {
                        intermixed_args.insert(std::end(intermixed_args),
                                               std::begin(args),
                                               std::end(args));
                    }
                } else {
                    if (subparser.first && !parser) {
                        _try_capture_parser(args, parser);
                        if (parser) {
                            i -= (i == parsed_arguments.size());
                            i -= args.size();
                        }
                    } else {
                        _match_args_partial(args);
                    }
                }
            }
        }
        if (!intermixed_args.empty()) {
            _match_args_partial(intermixed_args);
        }
        auto _check_mutex_groups = [_custom_error, &storage]
             (Parser const* p, std::deque<MutuallyExclusiveGroup> const& groups)
        {
            for (auto const& ex : groups) {
                std::string args;
                std::string found;
                for (auto const& arg : ex.m_data.m_arguments) {
                    auto flags = detail::_vector_to_string(arg->flags(), "/");
                    args += detail::_spaces + flags;
                    if (!storage.at(arg).empty()) {
                        if (!found.empty()) {
                            _custom_error(p, "argument " + flags
                                          + ": not allowed with argument "
                                          + found);
                        }
                        found = flags;
                    }
                }
                if (ex.m_required && found.empty()) {
                    if (ex.m_data.m_arguments.empty()) {
                        throw IndexError("list index out of range");
                    }
                    _custom_error(p, "one of the arguments"
                                  + args + " is required");
                }
            }
        };
        _check_mutex_groups(nullptr, m_mutex_groups);
        if (parser) {
            _check_mutex_groups(parser, parser->m_mutex_groups);
        }
        std::vector<std::string> required_args;
        for (auto const& arg : optional) {
            if (arg->required() && storage.at(arg).empty()) {
                auto args = detail::_vector_to_string(arg->flags(), "/");
                required_args.emplace_back(std::move(args));
            }
        }
        bool subparser_required
                = subparser.first && !parser && subparser.first->required();
        if (!required_args.empty()
                || pos < positional.size() || subparser_required) {
            std::string args;
            auto _append_value = [] (std::string const& val, std::string& args)
            {
                if (!args.empty()) {
                    args += ", ";
                }
                args += val;
            };
            for ( ; pos < positional.size(); ++pos) {
                if (subparser_required && pos == subparser.second) {
                    _append_value(subparser.first->flags_to_string(), args);
                }
                auto const& arg = positional.at(pos);
                if (args.empty()) {
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg->m_action == Action::extend
                            && arg->m_nargs == Argument::ZERO_OR_ONE) {
                        throw TypeError("'NoneType' object is not iterable");
                    }
                    if ((arg->m_nargs
                         & (Argument::ZERO_OR_ONE | Argument::ZERO_OR_MORE))
                            || arg->m_action == Action::BooleanOptionalAction) {
                        _store_default_value(arg);
                        continue;
                    }
                }
                _append_value(arg->m_flags.front(), args);
            }
            if (subparser_required && pos == subparser.second) {
                _append_value(subparser.first->flags_to_string(), args);
            }
            for (auto const& arg : required_args) {
                _append_value(arg, args);
            }
            if (!args.empty()) {
                throw_error("the following arguments are required: " + args);
            }
        }
        check_unrecognized_args(only_known, unrecognized_args);
        default_values_post_trigger(storage);
        parser_post_trigger(parser, sub_storage, storage);
        if (only_known) {
            return argparse::Namespace(std::move(storage),
                                       std::move(unrecognized_args));
        } else {
            return argparse::Namespace(std::move(storage));
        }
    }

    static void validate_arguments(std::vector<pArgument> const& args)
    {
        for (auto const& arg : args) {
            arg->validate();
        }
    }

    static void validate_subparser(std::shared_ptr<Subparser> const& subparser,
                                   std::string& subparser_dest,
                                   std::vector<std::string>& subparser_flags)
    {
        if (subparser) {
            for (auto const& p : subparser->m_parsers) {
                for (auto const& arg : p.m_data.m_arguments) {
                    arg->validate();
                }
            }
            subparser_dest = subparser->m_dest;
            if (!subparser_dest.empty()) {
                subparser_flags.push_back(subparser_dest);
            }
        }
    }

    inline void check_load_args(std::vector<std::string>& arguments,
                                std::size_t i, bool was_pseudo_arg) const
    {
        if (!m_fromfile_prefix_chars.empty() && !was_pseudo_arg) {
            while (!arguments.at(i).empty()
                   && detail::_is_value_exists(arguments.at(i).front(),
                                               m_fromfile_prefix_chars)) {
                auto args = convert_arg_line_to_args(arguments.at(i).substr(1));
                using dtype = std::vector<std::string>::difference_type;
                arguments.erase(std::next(std::begin(arguments),
                                          static_cast<dtype>(i)));
                arguments.insert(std::next(std::begin(arguments),
                                           static_cast<dtype>(i)),
                                 std::make_move_iterator(std::begin(args)),
                                 std::make_move_iterator(std::end(args)));
            }
        }
    }

    void print_help_and_exit(Parser const* parser) const
    {
        if (parser) {
            bool add_help = false;
            auto opt_all = parser->m_data.get_optional(true);
            auto opt = parser->m_data.get_optional(false);
            if (m_add_help) {
                auto help_flags
                        = detail::_help_flags(parser->m_prefix_chars);
                if (m_data.m_conflict_handler == "resolve") {
                    for (auto const& arg : opt_all) {
                        detail::_resolve_conflict(arg->flags(), help_flags);
                    }
                }
                if (!help_flags.empty()) {
                    auto help = Argument::make_argument(
                                std::move(help_flags),
                                "help", Argument::Optional);
                    help->help("show this help message and exit")
                            .action(Action::help);
                    opt_all.insert(std::begin(opt_all), help);
                    opt.insert(std::begin(opt), help);
                    add_help = true;
                }
            }
            print_custom_help(parser->m_data.get_positional(true), opt_all,
                              parser->m_data.get_positional(false), opt,
                              add_help, parser->m_groups,
                              parser->m_mutex_groups,
                              std::make_pair(nullptr, 0),
                              parser->m_prog, parser->usage(),
                              parser->description(), parser->epilog());
        } else {
            print_help();
        }
        ::exit(0);
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
    default_values_post_trigger(argparse::Namespace::Storage& storage) const
    {
        bool suppress_default = m_argument_default_type == SUPPRESS;
        for (auto it = storage.begin(); it != storage.end(); ) {
            if (!it->second.exists() && it->first->m_action != Action::count
                    && it->first->m_type == Argument::Optional) {
                auto const& value = default_argument_value(*(it->first));
                if (it->first->m_default_type == SUPPRESS
                        || (suppress_default && !value.has_value())) {
                    it = storage.erase(it);
                    continue;
                }
                if (value.has_value()) {
                    it->second.push_default(value());
                } else if (it->first->action() & detail::_bool_action) {
                    it->second.push_back(value());
                }
            }
            ++it;
        }
        for (auto const& pair : m_default_values) {
            if (!storage.exists(pair.first)) {
                auto arg = Argument::make_argument(
                            std::vector<std::string>{ pair.first },
                            pair.first, Argument::Positional);
                arg->default_value(pair.second);
                storage.create(arg, { true, { pair.second } });
            }
        }
    }

    static void
    parser_post_trigger(Parser* parser,
                        argparse::Namespace::Storage& sub_storage,
                        argparse::Namespace::Storage const& storage)
    {
        if (parser && parser->m_parse_handle) {
            for (auto it = sub_storage.begin(); it != sub_storage.end(); ) {
                if (storage.exists(it->first)) {
                    it->second = storage.at(it->first);
                    ++it;
                } else {
                    it = sub_storage.erase(it);
                }
            }
            parser->m_parse_handle(argparse::Namespace(sub_storage));
        }
    }

    inline detail::Value<std::string> const&
    default_argument_value(Argument const& arg) const _ARGPARSE_NOEXCEPT
    {
        return (arg.m_default.has_value()
                || !m_argument_default.has_value()) ? arg.m_default
                                                    : m_argument_default;
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
        if (width >= 2 * detail::_name_maxwidth) {
            return width - detail::_name_maxwidth;
        }
        if (width >= 2 * detail::_help_minwidth) {
            return width >> 1;
        }
        return detail::_help_minwidth;
    }

    std::vector<pArgument>
    positional_arguments(bool add_suppress = true, bool add_groups = true) const
    {
        std::vector<pArgument> result;
        result.reserve(m_data.m_positional.size());
        for (auto const& parent : m_parents) {
            auto args = parent.positional_arguments(add_suppress, add_groups);
            if (!args.empty()) {
                result.insert(std::end(result),
                              std::make_move_iterator(std::begin(args)),
                              std::make_move_iterator(std::end(args)));
            }
        }
        for (auto const& arg : m_data.m_positional) {
            if ((add_suppress || !arg.first->m_help_type.has_value())
                    && (add_groups || !arg.second)) {
                result.push_back(arg.first);
            }
        }
        return result;
    }

    std::pair<bool, std::vector<pArgument> >
    optional_arguments(bool add_suppress = true, bool add_groups = true) const
    {
        bool help_added = false;
        std::vector<pArgument> result;
        result.reserve(m_data.m_optional.size() + 1);
        if (m_add_help) {
            auto help_flags = detail::_help_flags(m_prefix_chars);
            if (m_data.m_conflict_handler == "resolve") {
                for (auto const& pair : m_data.m_optional) {
                    detail::_resolve_conflict(pair.first->flags(), help_flags);
                }
            }
            if (!help_flags.empty()) {
                auto help = Argument::make_argument(
                            std::move(help_flags), "help", Argument::Optional);
                help->help("show this help message and exit")
                        .action(Action::help);
                result.emplace_back(std::move(help));
                help_added = true;
            }
        }
        for (auto const& parent : m_parents) {
            auto args
                   = parent.optional_arguments(add_suppress, add_groups).second;
            if (!args.empty()) {
                result.insert(std::end(result),
                              std::make_move_iterator(std::begin(args)),
                              std::make_move_iterator(std::end(args)));
            }
        }
        for (auto const& arg : m_data.m_optional) {
            if ((add_suppress || !arg.first->m_help_type.has_value())
                    && (add_groups || !arg.second)
                    && !arg.first->flags().empty()) {
                result.push_back(arg.first);
            }
        }
        return std::make_pair(help_added, result);
    }

    SubparserInfo subpurser_info(bool add_suppress = true) const
    {
        SubparserInfo res = std::make_pair(m_subparsers, 0);
        auto _func = [&res, add_suppress] (ArgumentParser const& parser)
        {
#ifdef min
            std::size_t size = min(parser.m_subparsers->m_position,
                                   parser.m_data.m_positional.size());
#else
            std::size_t size = std::min(parser.m_subparsers->m_position,
                                        parser.m_data.m_positional.size());
#endif // min
            for (std::size_t i = 0; i < size; ++i) {
                res.second
                        += (add_suppress
                            || !parser.m_data.m_positional.at(i)
                                               .first->m_help_type.has_value());
            }
        };
        if (m_subparsers) {
            for (auto const& parent : m_parents) {
                res.second
                      += parent.positional_arguments(add_suppress, true).size();
            }
            _func(*this);
        } else {
            for (std::size_t i = 0; i < m_parents.size(); ++i) {
                auto const& parent = m_parents.at(i);
                if (parent.m_subparsers) {
                    res.first = parent.m_subparsers;
                    for (std::size_t j = 0; j < i; ++j) {
                        res.second += m_parents.at(j)
                               .positional_arguments(add_suppress, true).size();
                    }
                    _func(parent);
                    break;
                }
            }
        }
        return res;
    }

    static std::string
    custom_usage(HelpFormatter formatter,
                 std::vector<pArgument> const& positional,
                 std::vector<pArgument> const& optional,
                 std::deque<MutuallyExclusiveGroup> const& mutex_groups,
                 SubparserInfo const& subparser)
    {
        std::string res;
        auto ex_opt = optional;
        for (auto const& ex : mutex_groups) {
            for (auto arg : ex.m_data.m_arguments) {
                ex_opt.erase(std::remove(std::begin(ex_opt),
                                         std::end(ex_opt), arg),
                             std::end(ex_opt));
            }
        }
        auto _arg_usage = [&res] (std::string const& str, bool bkt)
        {
            if (!res.empty()) {
                res += '\n';
            }
            res += (bkt ? "[" + str + "]" : str);
        };
        for (auto const& arg : ex_opt) {
            _arg_usage(arg->usage(formatter), true);
        }
        for (auto const& ex : mutex_groups) {
            _arg_usage(ex.usage(formatter), false);
        }
        for (std::size_t i = 0; i < positional.size(); ++i) {
            if (subparser.first && subparser.second == i) {
                _arg_usage(subparser.first->usage(), false);
            }
            auto const str = positional.at(i)->usage(formatter);
            if (str.empty()) {
                continue;
            }
            _arg_usage(str, false);
        }
        if (subparser.first && subparser.second == positional.size()) {
            _arg_usage(subparser.first->usage(), false);
        }
        return res;
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
                = 1 + (w > detail::_minimum_width ? head_prog : head).size();
        os << detail::_format_output(
                  head_prog,
                  custom_usage(m_formatter_class, positional, optional,
                               mutex_groups, subparser),
                  1, indent, w)
           << std::endl;
    }

    static bool is_subparser_positional(std::shared_ptr<Subparser> const& sub)
    {
        return sub && sub->title().empty() && sub->description().empty();
    }

    static void
    print_subparser(bool need_print, SubparserInfo const& subparser,
                    std::size_t index, HelpFormatter formatter,
                    std::size_t size, std::size_t width, std::ostream& os)
    {
        if (need_print && subparser.second == index) {
            os << subparser.first->print(formatter, size, width) << std::endl;
        }
    }

    static void
    print_group(pGroup const& group, std::shared_ptr<Subparser>const& subparser,
                bool sub_positional, bool suppress_default,
                detail::Value<std::string> const& argument_default,
                HelpFormatter formatter, std::size_t size, std::size_t width,
                std::ostream& os)
    {
        if ((subparser && (group != subparser || !sub_positional))
                || (!subparser && group != subparser)) {
            group->print_help(os, suppress_default, argument_default,
                              formatter, size, width);
        }
    }

    void
    print_custom_help(std::vector<pArgument> const& positional_all,
                      std::vector<pArgument> const& optional_all,
                      std::vector<pArgument> const& positional,
                      std::vector<pArgument> const& optional,
                      bool help_added,
                      std::deque<pGroup> const& groups,
                      std::deque<MutuallyExclusiveGroup> const& mutex_groups,
                      SubparserInfo const& subparser_info,
                      std::string const& prog,
                      std::string const& usage,
                      std::string const& description,
                      std::string const& epilog,
                      std::ostream& os = std::cout) const
    {
        if (!usage.empty()) {
            os << "usage: " << usage << std::endl;
        } else {
            print_custom_usage(positional_all, optional_all,
                               mutex_groups, subparser_info, prog, os);
        }
        detail::_print_raw_text_formatter(m_formatter_class, description, os);
        std::size_t size = 0;
        auto _update_size = [&size] (std::size_t value)
        {
            if (size < value) {
                size = value;
            }
        };
        std::size_t width = output_width();
        bool suppress_default = m_argument_default_type == SUPPRESS;
        auto subparser = subparser_info.first;
        bool sub_positional = is_subparser_positional(subparser);
        for (auto const& arg : positional) {
            _update_size(arg->flags_to_string(m_formatter_class).size());
        }
        for (auto const& arg : optional) {
            _update_size(arg->flags_to_string(m_formatter_class).size());
        }
        for (auto const& group : groups) {
            group->limit_help_flags(m_formatter_class, size);
        }
#ifdef min
        size = min(size + 4, argument_name_limit());
#else
        size = std::min(size + 4, argument_name_limit());
#endif // min
        if (!positional.empty() || sub_positional) {
            os << "\npositional arguments:\n";
            for (std::size_t i = 0; i < positional.size(); ++i) {
                print_subparser(sub_positional, subparser_info, i,
                                m_formatter_class, size, width, os);
                os << positional.at(i)->print(suppress_default,
                                              m_argument_default,
                                              m_formatter_class,
                                              size, width) << std::endl;
            }
            print_subparser(sub_positional, subparser_info, positional.size(),
                            m_formatter_class, size, width, os);
        }
        if (!optional.empty()) {
            os << "\noptional arguments:\n";
            for (auto i = std::begin(optional); i != std::end(optional); ++i) {
                os << (*i)->print(suppress_default
                                  && !(help_added && i == std::begin(optional)),
                                  m_argument_default,
                                  m_formatter_class, size, width) << std::endl;
            }
        }
        for (auto const& group : groups) {
            print_group(group, subparser, sub_positional, suppress_default,
                        m_argument_default, m_formatter_class, size, width, os);
        }
        detail::_print_raw_text_formatter(m_formatter_class, epilog, os);
    }

    std::string m_prog;
    std::vector<ArgumentParser> m_parents;
    HelpFormatter m_formatter_class;
    std::string m_fromfile_prefix_chars;
    detail::Value<std::string> m_argument_default;
    detail::Value<Enum> m_argument_default_type;
    detail::Value<std::size_t> m_output_width;
    bool m_add_help;
    bool m_allow_abbrev;
    bool m_exit_on_error;
    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
    std::shared_ptr<Subparser> m_subparsers;
};
} // argparse

#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_USE_CONSTEXPR
#undef _ARGPARSE_USE_FILESYSTEM

#endif // _ARGPARSE_ARGUMENT_PARSER_HPP_
