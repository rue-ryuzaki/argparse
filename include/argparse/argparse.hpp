/*
 * MIT License
 *
 * Argument parser for C++11 (ArgumentParser v1.0.1)
 *
 * Copyright (c) 2021 Golubchikov Mihail <https://github.com/rue-ryuzaki>
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

#define ARGPARSE_VERSION_MAJOR 1
#define ARGPARSE_VERSION_MINOR 0
#define ARGPARSE_VERSION_PATCH 1

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="

namespace argparse {
namespace detail {
std::size_t const _usage_limit = 80;
std::size_t const _argument_help_limit = 24;
char const _default_prefix_char = '-';
std::string const _default_prefix_chars = "-";
std::string const _pseudo_argument = "--";
char const _space = ' ';
char const _equal = '=';
std::string const _equals = "=";

static inline void _ltrim(std::string& s)
{
    s.erase(std::begin(s), std::find_if(std::begin(s), std::end(s),
                                        not1(std::ptr_fun<int, int>(isspace))));
}

static inline void _rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
}

static inline void _trim(std::string& s)
{
    _ltrim(s);
    _rtrim(s);
}

static inline std::string _trim_copy(std::string s)
{
    _trim(s);
    return s;
}

static inline std::string _to_lower(std::string s)
{
    std::transform(std::begin(s), std::end(s), std::begin(s), tolower);
    return s;
}

static inline std::string _to_upper(std::string s)
{
    std::transform(std::begin(s), std::end(s), std::begin(s), toupper);
    return s;
}

static inline std::string _file_name(std::string const& s)
{
    return s.substr(s.find_last_of("/\\") + 1);
}

static inline bool _have_quotes(std::string const& s)
{
    return s.size() > 1 && s.front() == s.back()
            && (s.front() == '\'' || s.front() == '\"');
}

static inline std::string _remove_quotes(std::string const& s)
{
    return _have_quotes(s) ? s.substr(1, s.size() - 2) : s;
}

static inline std::string _replace(std::string s, char c, std::string const& val)
{
    std::size_t pos = s.find(c);
    while (pos != std::string::npos) {
        s.replace(pos, 1, val);
        pos = s.find(c, pos + val.size());
    }
    return s;
}

static inline bool _starts_with(std::string const& s, std::string const& pattern)
{
    return s.compare(0, pattern.size(), pattern) == 0;
}

template <class T>
bool _is_value_exists(T const& value, std::vector<T> const& vec)
{
    for (auto const& el : vec) {
        if (el == value) {
            return true;
        }
    }
    return false;
}

static inline bool _is_value_exists(char value, std::string const& str)
{
    for (auto const& el : str) {
        if (el == value) {
            return true;
        }
    }
    return false;
}

static inline std::string _flag_name(std::string str)
{
    auto prefix = str.front();
    str.erase(std::begin(str),
              std::find_if(std::begin(str), std::end(str),
                           [prefix] (char c) -> bool { return c != prefix; }));
    return str;
}

static inline std::vector<std::string> _help_flags(std::string const& prefix_chars)
{
    auto const& prefix
            = _is_value_exists(_default_prefix_char, prefix_chars)
            ? _default_prefix_char : prefix_chars.front();
    return { std::string(1, prefix) + "h", std::string(2, prefix) + "help" };
}

static inline bool _is_negative_number(std::string const& str)
{
    double value;
    std::stringstream ss(str);
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        return false;
    }
    return value < 0;
}

static inline bool _is_optional(std::string const& arg, std::string const& prefix_chars,
                                bool have_negative_args, bool was_pseudo_arg)
{
    return _is_value_exists(arg.front(), prefix_chars) && !was_pseudo_arg
            && (have_negative_args || !_is_negative_number(arg));
}

static inline bool _not_optional(std::string const& arg, std::string const& prefix_chars,
                                 bool have_negative_args, bool was_pseudo_arg)
{
    return !_is_value_exists(arg.front(), prefix_chars) || was_pseudo_arg
            || (!have_negative_args && _is_negative_number(arg));
}

static std::vector<std::string> _split_to_args(std::string const& str)
{
    std::vector<std::string> result;
    auto _store_value = [&result] (std::string& value)
    {
        if (!value.empty()) {
            result.push_back(value);
            value.clear();
        }
    };
    std::string value;
    std::deque<char> quotes;
    for (std::size_t i = 0; i < str.size(); ++i) {
        bool skip = false;
        auto c = str.at(i);
        if (c == '\\') {
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
        if (((c == _space && !skip) || c == '\t' || c == '\n') && quotes.empty()) {
            _store_value(value);
        } else {
            if (c == '\"' || c == '\'') {
                if (!quotes.empty() && quotes.back() == c) {
                    quotes.pop_back();
                } else if (value.empty()) {
                    quotes.push_back(c);
                }
            }
            value += c;
        }
    }
    _store_value(value);
    if (!quotes.empty()) {
        std::cerr << "incorrect string: '" << str << "'" << std::endl;
    }
    return result;
}

static inline std::vector<std::string> _split_equal(std::string const& s,
                                                    std::string const& prefix)
{
    auto pos = _is_value_exists(_equal, prefix)
            ? s.find(_equal, std::distance(std::begin(s), std::find_if(std::begin(s), std::end(s),
              [] (char c) -> bool { return c != _equal; }))) : s.find(_equal);
    if (pos != std::string::npos) {
        return { s.substr(0, pos), s.substr(pos + 1) };
    } else {
        return { s };
    }
}

static inline bool _string_to_bool(std::string const& str)
{
    return !str.empty();
}

static inline std::string _bool_to_string(std::string const& str)
{
    return _string_to_bool(str) ? "true" : "false";
}

static inline std::string _vector_to_string(std::vector<std::string> const& vec,
                                            std::string const& separator = " ",
                                            std::string const& quotes = std::string(),
                                            bool replace_space = false,
                                            std::string const& none = std::string())
{
    std::string res;
    for (auto const& el : vec) {
        if (!res.empty()) {
            res += separator;
        }
        auto val = el;
        if (quotes.empty() && replace_space && !_have_quotes(val)) {
            val = _replace(val, ' ', "\\ ");
        }
        res += quotes + val + quotes;
    }
    return res.empty() ? none : res;
}

static inline std::string _ignore_default(std::string const& arg, std::string const& value)
{
    return "argument " + arg + ": ignored default value '" + value + "'";
}

static inline std::string _ignore_explicit(std::string const& arg, std::string const& value)
{
    return "argument " + arg + ": ignored explicit argument '" + value + "'";
}

template <class T>
class Value
{
public:
    Value()
        : m_status(false),
          m_value()
    { }
    Value(Value const& orig)
        : m_status(orig.m_status),
          m_value(orig.m_value)
    { }

    Value& operator =(Value const& rhs)
    {
        if (this != &rhs) {
            this->m_status = rhs.m_status;
            this->m_value  = rhs.m_value;
        }
        return *this;
    }

    Value& operator =(T const& rhs)
    {
        this->m_status = true;
        this->m_value  = rhs;
        return *this;
    }

    Value& operator =(T&& rhs)
    {
        this->m_status = true;
        this->m_value  = std::move(rhs);
        return *this;
    }

    bool operator ==(Value const& rhs) const
    {
        return this->m_status == rhs.m_status && this->m_value == rhs.m_value;
    }

    void clear(T const& value = T())
    {
        m_status = false;
        m_value = value;
    }

    bool        status() const { return m_status; }
    T const&    value()  const { return m_value; }
    T const& operator()()const { return m_value; }

private:
    bool    m_status;
    T       m_value;
};
} // details

/*!
 * \brief Action values
 *
 * \enum Action
 */
enum Action
{
    store           = 0x00000001,
    store_const     = 0x00000002,
    store_true      = 0x00000004,
    store_false     = 0x00000008,
    append          = 0x00000010,
    append_const    = 0x00000020,
    count           = 0x00000040,
    help            = 0x00000080,
    version         = 0x00000100,
    extend          = 0x00000200,
};

/*!
 * \brief Unspecified values
 *
 * \enum Enum
 */
enum Enum
{
    SUPPRESS,
};

/*!
 * \brief Help formatter values
 *
 * \enum HelpFormatter
 */
enum HelpFormatter
{
    ArgumentDefaultsHelpFormatter,
};

/*!
 * \brief ArgumentError handler
 */
class ArgumentError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct ArgumentError handler
     *
     *  \param error Error message
     *
     *  \return ArgumentError object
     */
    explicit ArgumentError(std::string const& error)
        : std::invalid_argument("argparse.ArgumentError: " + error)
    { }
};

/*!
 * \brief AttributeError handler
 */
class AttributeError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct AttributeError handler
     *
     *  \param error Error message
     *
     *  \return AttributeError object
     */
    explicit AttributeError(std::string const& error)
        : std::invalid_argument("AttributeError: " + error)
    { }
};

/*!
 * \brief ValueError handler
 */
class ValueError : public std::invalid_argument
{
public:
    /*!
     *  \brief Construct ValueError handler
     *
     *  \param error Error message
     *
     *  \return ValueError object
     */
    explicit ValueError(std::string const& error)
        : std::invalid_argument("ValueError: " + error)
    { }
};

/*!
 * \brief IndexError handler
 */
class IndexError : public std::logic_error
{
public:
    /*!
     *  \brief Construct IndexError handler
     *
     *  \param error Error message
     *
     *  \return IndexError object
     */
    explicit IndexError(std::string const& error)
        : std::logic_error("IndexError: " + error)
    { }
};

/*!
 * \brief TypeError handler
 */
class TypeError : public std::logic_error
{
public:
    /*!
     *  \brief Construct TypeError handler
     *
     *  \param error Error message
     *
     *  \return TypeError object
     */
    explicit TypeError(std::string const& error)
        : std::logic_error("TypeError: " + error)
    { }
};

/*!
 * \brief Argument class
 */
class Argument
{
    friend class ArgumentData;
    friend class ArgumentGroup;
    friend class ArgumentParser;
    friend class BaseParser;
    friend class ExclusiveGroup;

    enum Nargs
    {
        NARGS_DEF,      // ""
        NARGS_INT,      // "N"
        ONE_OR_MORE,    // "+"
        OPTIONAL,       // "*"
        ZERO_OR_MORE,   // "?"
    };

public:
    /*!
     * \brief Argument type
     *
     * \enum Type
     */
    enum Type
    {
        Positional,
        Optional
    };

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *  \param name Argument name
     *  \param type Argument type
     *
     *  \return Argument object
     */
    Argument(std::vector<std::string> const& flags,
             std::string const& name,
             Type type)
        : m_flags(flags),
          m_name(name),
          m_type(type),
          m_action(Action::store),
          m_nargs(NARGS_DEF),
          m_nargs_str("1"),
          m_num_args(1),
          m_const(),
          m_default(),
          m_choices(),
          m_required(false),
          m_help(),
          m_help_type(),
          m_metavar(),
          m_dest_str(),
          m_dest(),
          m_version(),
          m_handle_str(nullptr),
          m_handle(nullptr)
    { }

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *  \param name Argument name
     *  \param type Argument type
     *
     *  \return Argument object
     */
    Argument(std::vector<std::string>&& flags,
             std::string&& name,
             Type type)
        : m_flags(std::move(flags)),
          m_name(std::move(name)),
          m_type(type),
          m_action(Action::store),
          m_nargs(NARGS_DEF),
          m_nargs_str("1"),
          m_num_args(1),
          m_const(),
          m_default(),
          m_choices(),
          m_required(false),
          m_help(),
          m_help_type(),
          m_metavar(),
          m_dest_str(),
          m_dest(),
          m_version(),
          m_handle_str(nullptr),
          m_handle(nullptr)
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
          m_name(orig.m_name),
          m_type(orig.m_type),
          m_action(orig.m_action),
          m_nargs(orig.m_nargs),
          m_nargs_str(orig.m_nargs_str),
          m_num_args(orig.m_num_args),
          m_const(orig.m_const),
          m_default(orig.m_default),
          m_choices(orig.m_choices),
          m_required(orig.m_required),
          m_help(orig.m_help),
          m_help_type(orig.m_help_type),
          m_metavar(orig.m_metavar),
          m_dest_str(orig.m_dest_str),
          m_dest(orig.m_dest),
          m_version(orig.m_version),
          m_handle_str(orig.m_handle_str),
          m_handle(orig.m_handle)
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
            this->m_flags   = rhs.m_flags;
            this->m_name    = rhs.m_name;
            this->m_type    = rhs.m_type;
            this->m_action  = rhs.m_action;
            this->m_nargs   = rhs.m_nargs;
            this->m_nargs_str= rhs.m_nargs_str;
            this->m_num_args= rhs.m_num_args;
            this->m_const   = rhs.m_const;
            this->m_default = rhs.m_default;
            this->m_choices = rhs.m_choices;
            this->m_required= rhs.m_required;
            this->m_help    = rhs.m_help;
            this->m_help_type= rhs.m_help_type;
            this->m_metavar = rhs.m_metavar;
            this->m_dest_str= rhs.m_dest_str;
            this->m_dest    = rhs.m_dest;
            this->m_version = rhs.m_version;
            this->m_handle_str= rhs.m_handle_str;
            this->m_handle  = rhs.m_handle;
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
    bool operator <(Argument const& rhs) const
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
        if (!(value & (Action::store | Action::store_const | Action::append
                       | Action::append_const | Action::extend))) {
            m_handle_str = nullptr;
        }
        if (!(value & (Action::store_true | Action::store_false | Action::count))) {
            m_handle = nullptr;
        }
        if (!(value & (Action::store | Action::store_const | Action::append
                       | Action::append_const | Action::extend))) {
            m_metavar.clear();
        }
        switch (value) {
            case Action::store_true :
                m_default.clear();
                m_const = "1";
                m_nargs = NARGS_INT;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::store_false :
                m_default.clear("1");
            case Action::store_const :
            case Action::append_const :
                m_const = std::string();
                m_nargs = NARGS_INT;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::version :
                help("show program's version number and exit");
            case Action::help :
                if (m_type == Positional) {
                    // version and help actions cannot be positional
                    throw TypeError("got an unexpected keyword argument 'required'");
                }
            case Action::count :
                m_nargs = NARGS_INT;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::store :
            case Action::append :
            case Action::extend :
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
                throw TypeError("got an unexpected keyword argument 'nargs'");
            case Action::store :
                if (value == 0) {
                    throw ValueError("nargs for store actions must be != 0; "
                                     "if you have nothing to store, actions such as "
                                     "store true or store const may be more appropriate");
                }
                break;
            case Action::append :
            case Action::extend :
                if (value == 0) {
                    throw ValueError("nargs for append actions must be != 0; "
                                     "if arg strings are not supplying the value to append, "
                                     "the append const action may be more appropriate");
                }
                break;
            default:
                throw ValueError("unknown action");
        }
        m_nargs = NARGS_INT;
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
        if (!(m_action & (Action::store | Action::append | Action::extend))) {
            throw TypeError("got an unexpected keyword argument 'nargs'");
        }
        auto param = detail::_trim_copy(value);
        if (param == "?") {
            m_nargs = OPTIONAL;
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
    Argument& optional()
    {
        return nargs("?");
    }

    /*!
     *  \brief Set argument 'nargs' zero_or_more ("*") value
     *
     *  \return Current argument reference
     */
    Argument& zero_or_more()
    {
        return nargs("*");
    }

    /*!
     *  \brief Set argument 'nargs' one_or_more ("+") value
     *
     *  \return Current argument reference
     */
    Argument& one_or_more()
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
        if (m_action & (Action::store_const | Action::append_const)
                || (m_type == Optional && m_nargs == OPTIONAL
                    && (m_action & (Action::store | Action::append | Action::extend)))) {
            m_const = detail::_trim_copy(value);
        } else if (m_type == Optional && m_nargs != OPTIONAL
                   && (m_action & (Action::store | Action::append | Action::extend))) {
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
              typename std::enable_if<not std::is_constructible<std::string, T>::value>::type* = nullptr>
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
    Argument& default_value(std::string const& value)
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
              typename std::enable_if<not std::is_constructible<std::string, T>::value>::type* = nullptr>
    Argument& default_value(T const& value)
    {
        std::stringstream ss;
        ss << value;
        m_default = ss.str();
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
        std::vector<std::string> values;
        values.reserve(value.size());
        for (auto const& c : value) {
            values.emplace_back(std::string(1, c));
        }
        return choices(values);
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
        if (!(m_action & (Action::store | Action::append | Action::extend))) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        std::vector<std::string> values;
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
    Argument& required(bool value)
    {
        if (m_type == Positional) {
            throw TypeError("'required' is an invalid argument for positionals");
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
    Argument& help(std::string const& value)
    {
        m_help = detail::_trim_copy(value);
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
    Argument& help(Enum value)
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
    Argument& metavar(std::string const& value)
    {
        if (!(m_action & (Action::store | Action::store_const | Action::append
                          | Action::append_const | Action::extend))) {
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
        if (m_type == Positional) {
            throw ValueError("dest supplied twice for positional argument");
        }
        m_dest_str = detail::_trim_copy(value);
        if (m_dest_str.empty()) {
            m_dest.clear();
        } else if (m_dest.empty()) {
            m_dest.push_back(m_dest_str);
        } else {
            m_dest.front() = m_dest_str;
        }
        return *this;
    }

    /*!
     *  \brief Set argument 'version' for arguments with 'version' action
     *
     *  \param value Version value
     *
     *  \return Current argument reference
     */
    Argument& version(std::string const& value)
    {
        if (m_action == Action::version) {
            m_version = detail::_trim_copy(value);
        } else {
            throw TypeError("got an unexpected keyword argument 'version'");
        }
        return *this;
    }

    /*!
     *  \brief Set argument 'handle' for arguments with 'store/_const' or 'append/_const/extend' action
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    Argument& handle(std::function<void(std::string)> func)
    {
        if (m_action & (Action::store | Action::store_const | Action::append
                        | Action::append_const | Action::extend)) {
            m_handle_str = func;
        } else {
            throw TypeError("got an unexpected keyword argument 'handle'");
        }
        return *this;
    }

    /*!
     *  \brief Set argument 'handle' for arguments with 'store_true/_false' or 'count' action
     *
     *  \param func Handle function
     *
     *  \return Current argument reference
     */
    Argument& handle(std::function<void()> func)
    {
        if (m_action & (Action::store_true | Action::store_false | Action::count)) {
            m_handle = func;
        } else {
            throw TypeError("got an unexpected keyword argument 'handle'");
        }
        return *this;
    }

    /*!
     *  \brief Get argument flags values
     *
     *  \return Argument flags values
     */
    std::vector<std::string> const& flags() const
    {
        return m_flags;
    }

    /*!
     *  \brief Get argument 'action' value
     *
     *  \return Argument 'action' value
     */
    Action action() const
    {
        return m_action;
    }

    /*!
     *  \brief Get argument 'nargs' value
     *
     *  \return Argument 'nargs' value
     */
    std::string const& nargs() const
    {
        return m_nargs_str;
    }

    /*!
     *  \brief Get argument 'const' value
     *
     *  \return Argument 'const' value
     */
    std::string const& const_value() const
    {
        return m_const();
    }

    /*!
     *  \brief Get argument 'default' value
     *
     *  \return Argument 'default' value
     */
    std::string const& default_value() const
    {
        return m_default();
    }

    /*!
     *  \brief Get argument 'choices' value
     *
     *  \return Argument 'choices' value
     */
    std::vector<std::string> const& choices() const
    {
        return m_choices();
    }

    /*!
     *  \brief Get argument 'required' value
     *
     *  \return Argument 'required' value
     */
    bool required() const
    {
        return m_required;
    }

    /*!
     *  \brief Get argument 'help' message
     *
     *  \return Argument 'help' message
     */
    std::string const& help() const
    {
        return m_help;
    }

    /*!
     *  \brief Get argument 'metavar' value
     *
     *  \return Argument 'metavar' value
     */
    std::string const& metavar() const
    {
        return m_metavar();
    }

    /*!
     *  \brief Get argument 'dest' value
     *
     *  \return Argument 'dest' value
     */
    std::string const& dest() const
    {
        return m_dest_str;
    }

    /*!
     *  \brief Get argument 'version' value
     *
     *  \return Argument 'version' value
     */
    std::string const& version() const
    {
        return m_version();
    }

private:
    void handle(std::string const& str) const
    {
        if (m_handle_str) {
            m_handle_str(detail::_remove_quotes(str));
        }
    }

    void handle() const
    {
        if (m_handle) {
            m_handle();
        }
    }

    std::string usage() const
    {
        std::string res;
        if (m_type == Optional) {
            res += m_flags.front();
        }
        if (m_action & (Action::store | Action::append
                        | Action::extend | Action::append_const)) {
            res += get_nargs_suffix();
        }
        return res;
    }

    std::string flags_to_string() const
    {
        std::string res;
        if (m_type == Optional) {
            for (auto const& flag : m_flags) {
                if (!res.empty()) {
                    res += ", ";
                }
                res += flag;
                if (m_action & (Action::store | Action::append
                                | Action::extend | Action::append_const)) {
                    res += get_nargs_suffix();
                }
            }
        } else {
            res += get_argument_name();
        }
        return res;
    }

    std::string print(bool show_default_value, detail::Value<std::string> const& argument_default,
                      std::size_t limit = detail::_argument_help_limit) const
    {
        std::string res = "  " + flags_to_string();
        if (!help().empty()) {
            if (res.size() + 2 > limit) {
                res += "\n" + std::string(detail::_argument_help_limit, ' ') + help();
            } else {
                res += std::string(limit - res.size(), ' ') + help();
            }
            if (show_default_value && m_type == Optional) {
                auto const& def = (m_default.status() || !argument_default.status()) ? m_default
                                                                                     : argument_default;
                if (!def.status() && m_action & (Action::store_true | Action::store_false)) {
                    res += " (default: " + detail::_bool_to_string(def()) + ")";
                } else {
                    res += " (default: " + ((def.status() || !def().empty()) ? def() : "None") + ")";
                }
            }
        }
        return res;
    }

    std::string get_nargs_suffix() const
    {
        auto const name = get_argument_name();
        std::string res;
        if (m_type == Optional && !name.empty()) {
            res += " ";
        }
        switch (m_nargs) {
            case OPTIONAL :
                res += "[" +  name + "]";
                break;
            case ONE_OR_MORE :
                res += name + " ";
            case ZERO_OR_MORE :
                res += "[" +  name + " ...]";
                break;
            case NARGS_INT :
                for (uint32_t i = 0; i < m_num_args; ++i) {
                    if (i != 0) {
                        res += " ";
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

    std::string get_argument_name() const
    {
        if (m_metavar.status()) {
            return metavar();
        }
        if (m_choices.status()) {
            return "{" + detail::_vector_to_string(choices(), ",") + "}";
        }
        auto const& res = m_dest_str.empty() ? m_name : m_dest_str;
        return m_type == Optional ? detail::_to_upper(res) : res;
    }

    std::string error_nargs(std::string const& arg) const
    {
        switch (m_nargs) {
            case NARGS_DEF :
                return "argument " + arg + ": expected one argument";
            case ONE_OR_MORE :
                return "argument " + arg + ": expected at least one argument";
            case NARGS_INT :
                return "argument " + arg + ": expected " + nargs() + " arguments";
            default :
                return std::string();
        }
    }

    bool operator ==(Argument const& rhs) const
    {
        return m_flags == rhs.m_flags
                && m_name == rhs.m_name
                && m_type == rhs.m_type
                && m_action == rhs.m_action
                && m_const == rhs.m_const
                && m_dest_str == rhs.m_dest_str;
    }

    bool operator ==(std::string const& rhs) const
    {
        if (!m_dest_str.empty()) {
            return m_dest_str == rhs;
        }
        return detail::_is_value_exists(rhs, m_flags);
    }

    std::vector<std::string> m_flags;
    std::string m_name;
    Type        m_type;
    Action      m_action;
    Nargs       m_nargs;
    std::string m_nargs_str;
    uint32_t    m_num_args;
    detail::Value<std::string> m_const;
    detail::Value<std::string> m_default;
    detail::Value<std::vector<std::string> > m_choices;
    bool        m_required;
    std::string m_help;
    detail::Value<Enum> m_help_type;
    detail::Value<std::string> m_metavar;
    std::string m_dest_str;
    std::vector<std::string> m_dest;
    detail::Value<std::string> m_version;
    std::function<void(std::string)> m_handle_str;
    std::function<void()> m_handle;
};

typedef std::shared_ptr<Argument> pArgument;

/*!
 * \brief Group class
 */
class Group
{
    friend class ArgumentParser;

public:
    /*!
     *  \brief Construct group
     *
     *  \return Group object
     */
    Group()
        : m_title(),
          m_description(),
          m_position()
    { }

    /*!
     *  \brief Destroy group
     */
    virtual ~Group() = default;

    /*!
     *  \brief Get group 'title' value
     *
     *  \return Group 'title' value
     */
    std::string const& title() const
    {
        return m_title;
    }

    /*!
     *  \brief Get group 'description' value
     *
     *  \return Group 'description' value
     */
    std::string const& description() const
    {
        return m_description;
    }

protected:
    virtual void limit_usage(std::size_t& limit) const = 0;
    virtual void limit_help_flags(std::size_t& limit) const = 0;
    virtual void print_help(std::ostream& os, bool show_default_value,
                            detail::Value<std::string> const& argument_default, std::size_t limit) const = 0;

    std::string m_title;
    std::string m_description;
    std::size_t m_position;
};

typedef std::shared_ptr<Group> pGroup;

/*!
 * \brief ArgumentData class
 */
class ArgumentData
{
    friend class ArgumentGroup;
    friend class ExclusiveGroup;

public:
    /*!
     *  \brief Construct argument data
     *
     *  \return Argument data object
     */
    ArgumentData()
        : m_arguments(),
          m_optional(),
          m_positional()
    { }

    /*!
     *  \brief Destroy argument data
     */
    virtual ~ArgumentData() = default;

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

    std::vector<pArgument> get_optional_with_help(bool add_group, bool add_help,
                                                  std::string const& prefix_chars) const
    {
        std::vector<pArgument> result;
        result.reserve(m_optional.size() + add_help);
        if (add_help) {
            auto help = std::make_shared<Argument>(detail::_help_flags(prefix_chars),
                                                   "help", Argument::Optional);
            help->help("show this help message and exit").action(Action::help);
            result.emplace_back(std::move(help));
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

    void create_argument(std::vector<std::string> flags, std::string const& prefix_chars)
    {
        if (flags.empty()) {
            throw ValueError("empty options");
        }
        detail::_trim(flags.front());
        auto flag_name = flags.front();
        if (flag_name.empty()) {
            throw IndexError("string index out of range");
        }
        std::size_t prefixes = 0;
        auto _update_flag_name = [] (std::string const& arg, std::string& flag_name, std::size_t& prefixes)
        {
            auto name = detail::_flag_name(arg);
            std::size_t count_prefixes = arg.size() - name.size();
            if (prefixes < count_prefixes) {
                prefixes = count_prefixes;
                flag_name = std::move(name);
            }
        };
        bool is_optional = detail::_is_value_exists(flag_name.front(), prefix_chars);
        if (is_optional) {
            _update_flag_name(flag_name, flag_name, prefixes);
        } else if (flags.size() > 1) {
            // no positional multiflag
            throw ValueError("invalid option string " + flags.front()
                             + ": must starts with a character '" + prefix_chars + "'");
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
                throw ValueError("invalid option string " + flag
                                 + ": must starts with a character '" + prefix_chars + "'");
            }
            _update_flag_name(flag, flag_name, prefixes);
        }
        auto arg = std::make_shared<Argument>(std::move(flags), std::move(flag_name),
                                              is_optional ? Argument::Optional : Argument::Positional);
        if (is_optional) {
            for (auto const& arg_flag : arg->m_flags) {
                for (auto const& opt : m_optional) {
                    if (detail::_is_value_exists(arg_flag, opt.first->m_flags)) {
                        throw ArgumentError("argument " + detail::_vector_to_string(arg->flags(), "/")
                                            + ": conflicting option string: " + arg_flag);
                    }
                }
            }
        }
        m_arguments.emplace_back(std::move(arg));
    }

    std::vector<pArgument> m_arguments;
    std::vector<std::pair<pArgument, bool> > m_optional;
    std::vector<std::pair<pArgument, bool> > m_positional;
};

/*!
 * \brief ArgumentGroup class
 */
class ArgumentGroup : public ArgumentData, public Group
{
    friend class ArgumentParser;

public:
    /*!
     *  \brief Construct argument group
     *
     *  \param title Group title
     *  \param description Group description
     *  \param prefix_chars Parent prefix chars
     *  \param parent_data Parent argument data
     *
     *  \return Argument group object
     */
    ArgumentGroup(std::string const& title, std::string const& description,
                  std::string& prefix_chars, ArgumentData* parent_data)
        : ArgumentData(),
          Group(),
          m_prefix_chars(prefix_chars),
          m_parent_data(parent_data)
    {
        m_title = title;
        m_description = description;
    }

    /*!
     *  \brief Create argument group object from another argument group
     *
     *  \param rhs Argument group object to copy
     *
     *  \return Argument group object
     */
    ArgumentGroup(ArgumentGroup const& rhs)
        : ArgumentData(),
          Group(),
          m_prefix_chars(rhs.m_prefix_chars),
          m_parent_data(rhs.m_parent_data)
    {
        m_title         = rhs.m_title;
        m_description   = rhs.m_description;
        m_position      = rhs.m_position;
        m_arguments     = rhs.m_arguments;
        m_optional      = rhs.m_optional;
        m_positional    = rhs.m_positional;
    }

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
            m_prefix_chars  = rhs.m_prefix_chars;
            m_arguments     = rhs.m_arguments;
            m_optional      = rhs.m_optional;
            m_positional    = rhs.m_positional;
            m_parent_data   = rhs.m_parent_data;
        }
        return *this;
    }

    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Argument& add_argument(T const& flag)
    {
        return add_argument({ flag });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    Argument& add_argument(std::vector<std::string> const& flags)
    {
        create_argument(flags, m_prefix_chars);
        bool is_optional = m_arguments.back()->m_type == Argument::Optional;
        m_parent_data->m_arguments.push_back(m_arguments.back());
        (is_optional ? m_optional : m_positional).push_back(std::make_pair(m_arguments.back(), true));
        (is_optional ? m_parent_data->m_optional
                     : m_parent_data->m_positional).push_back(std::make_pair(m_arguments.back(), true));
        return *m_arguments.back();
    }

private:
    void limit_usage(std::size_t& limit) const override
    {
        for (auto const& arg : m_arguments) {
            auto const str = arg->usage();
            if (limit < str.size()) {
                limit = str.size();
            }
        }
    }

    void limit_help_flags(std::size_t& limit) const override
    {
        for (auto const& arg : m_arguments) {
            auto size = arg->flags_to_string().size();
            if (limit < size) {
                limit = size;
            }
        }
    }

    void print_help(std::ostream& os, bool show_default_value,
                    detail::Value<std::string> const& argument_default, std::size_t limit) const override
    {
        if (!description().empty() || !m_arguments.empty()) {
            os << std::endl << title() << ":" << std::endl;
            if (!description().empty()) {
                os << "  " << description() << std::endl;
            }
            if (!m_arguments.empty()) {
                os << std::endl;
            }
            for (auto const& arg : m_arguments) {
                os << arg->print(show_default_value, argument_default, limit) << std::endl;
            }
        }
    }

    std::string& m_prefix_chars;
    ArgumentData* m_parent_data;
};

class ExclusiveGroup : public ArgumentData
{
public:
    friend class ArgumentParser;

public:
    /*!
     *  \brief Construct exclusive group
     *
     *  \param prefix_chars Parent prefix chars
     *  \param parent_data Parent argument data
     *
     *  \return Exclusive group object
     */
    ExclusiveGroup(std::string& prefix_chars, ArgumentData* parent_data)
        : ArgumentData(),
          m_prefix_chars(prefix_chars),
          m_parent_data(parent_data),
          m_required(false)
    { }

    /*!
     *  \brief Create exclusive group object from another exclusive group
     *
     *  \param rhs Exclusive group object to copy
     *
     *  \return Exclusive group object
     */
    ExclusiveGroup(ExclusiveGroup const& rhs)
        : ArgumentData(),
          m_prefix_chars(rhs.m_prefix_chars),
          m_parent_data(rhs.m_parent_data),
          m_required(rhs.m_required)
    {
        m_arguments     = rhs.m_arguments;
        m_optional      = rhs.m_optional;
        m_positional    = rhs.m_positional;
    }

    /*!
     *  \brief Copy exclusive group object from another argument group
     *
     *  \param rhs Exclusive group object to copy
     *
     *  \return Current exclusive group reference
     */
    ExclusiveGroup& operator =(ExclusiveGroup const& rhs)
    {
        if (this != &rhs) {
            m_arguments     = rhs.m_arguments;
            m_optional      = rhs.m_optional;
            m_positional    = rhs.m_positional;
            m_prefix_chars  = rhs.m_prefix_chars;
            m_parent_data   = rhs.m_parent_data;
            m_required      = rhs.m_required;
        }
        return *this;
    }

    /*!
     *  \brief Set exclusive group 'required' value
     *
     *  \param value Required flag
     *
     *  \return Current exclusive group reference
     */
    ExclusiveGroup& required(bool value)
    {
        m_required = value;
        return *this;
    }

    /*!
     *  \brief Get exclusive group 'required' value
     *
     *  \return Exclusive group 'required' value
     */
    bool required() const
    {
        return m_required;
    }

    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Argument& add_argument(T const& flag)
    {
        return add_argument({ flag });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    Argument& add_argument(std::vector<std::string> const& flags)
    {
        create_argument(flags, m_prefix_chars);
        if (m_arguments.back()->m_type != Argument::Optional) {
            m_arguments.pop_back();
            throw ValueError("mutually exclusive arguments must be optional");
        }
        m_parent_data->m_arguments.push_back(m_arguments.back());
        m_optional.push_back(std::make_pair(m_arguments.back(), false));
        m_parent_data->m_optional.push_back(std::make_pair(m_arguments.back(), false));
        return *m_arguments.back();
    }

private:
    std::string usage() const
    {
        std::string res;
        for (auto const& arg : m_arguments) {
            if (!res.empty()) {
                res += " | ";
            }
            res += arg->usage();
        }
        return res.empty() ? res : "(" + res + ")";
    }

    std::string& m_prefix_chars;
    ArgumentData* m_parent_data;
    bool m_required;
};

/*!
 * \brief BaseParser class
 */
class BaseParser : public ArgumentData
{
public:
    /*!
     *  \brief Construct base parser
     *
     *  \return BaseParser object
     */
    BaseParser()
        : ArgumentData(),
          m_usage(),
          m_description(),
          m_epilog(),
          m_prefix_chars(detail::_default_prefix_chars),
          m_groups(),
          m_exclusive()
    { }

    /*!
     *  \brief Destroy base parser
     */
    virtual ~BaseParser() = default;

    /*!
     *  \brief Get base parser 'usage' value
     *
     *  \return Base parser 'usage' value
     */
    std::string const& usage() const
    {
        return m_usage;
    }

    /*!
     *  \brief Get base parser 'description' value
     *
     *  \return Base parser 'description' value
     */
    std::string const& description() const
    {
        return m_description;
    }

    /*!
     *  \brief Get base parser 'epilog' value
     *
     *  \return Base parser 'epilog' value
     */
    std::string const& epilog() const
    {
        return m_epilog;
    }

    /*!
     *  \brief Get base parser 'prefix_chars' value
     *
     *  \return Base parser 'prefix_chars' value
     */
    std::string const& prefix_chars() const
    {
        return m_prefix_chars;
    }

    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Argument& add_argument(T const& flag)
    {
        return add_argument({ flag });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    Argument& add_argument(std::vector<std::string> const& flags)
    {
        create_argument(flags, m_prefix_chars);
        bool is_optional = m_arguments.back()->m_type == Argument::Optional;
        (is_optional ? m_optional : m_positional).push_back(std::make_pair(m_arguments.back(), false));
        return *m_arguments.back();
    }

    /*!
     *  \brief Add argument group
     *
     *  \return Current argument group reference
     */
    ArgumentGroup& add_argument_group(std::string const& title = std::string(),
                                      std::string const& description = std::string())
    {
        auto group = std::make_shared<ArgumentGroup>(title, description, m_prefix_chars, this);
        m_groups.push_back(group);
        return *group;
    }

    /*!
     *  \brief Add mutually exclusive group
     *
     *  \return Current mutually exclusive group reference
     */
    ExclusiveGroup& add_mutually_exclusive_group()
    {
        m_exclusive.emplace_back(ExclusiveGroup(m_prefix_chars, this));
        return m_exclusive.back();
    }

protected:
    std::string m_usage;
    std::string m_description;
    std::string m_epilog;
    std::string m_prefix_chars;
    std::deque<pGroup> m_groups;
    std::deque<ExclusiveGroup> m_exclusive;
};

/*!
 * \brief ArgumentParser objects
 */
class ArgumentParser : public BaseParser
{
    class Storage
    {
    public:
        typedef pArgument                                       key_type;
        typedef std::vector<std::string>                        mapped_type;
        typedef std::pair<const key_type, mapped_type>          value_type;
        typedef std::map<key_type, mapped_type>::iterator       iterator;
        typedef std::map<key_type, mapped_type>::const_iterator const_iterator;

        Storage()
            : m_data()
        { }

        void create(key_type const& key, mapped_type const& value = mapped_type())
        {
            if (key->m_action & (Action::version | Action::help)) {
                return;
            }
            auto const& flag = conflict_arg(key);
            if (flag.empty()) {
                m_data.insert({ key, value });
            } else {
                throw ArgumentError("argument " + detail::_vector_to_string(key->m_flags, "/")
                                    + ": conflicting dest string: " + flag);
            }
        }

        void create(std::vector<key_type> const& arguments)
        {
            for (auto const& arg : arguments) {
                create(arg);
            }
        }

        void store_value(key_type const& arg, std::string const& value)
        {
            at(arg).push_back(value);
            arg->handle(value);
        }

        void store_default_value(key_type const& arg, std::string const& value)
        {
            if (arg->m_action == Action::store) {
                auto& storage = at(arg);
                if (storage.empty()) {
                    storage.push_back(value);
                }
            }
        }

        bool self_value_stored(key_type const& arg)
        {
            if (arg->m_action == Action::store_const) {
                auto& storage = at(arg);
                if (storage.empty()) {
                    storage.push_back(arg->m_const());
                }
                arg->handle(arg->m_const());
                return true;
            } else if (arg->m_action & (Action::store_true | Action::store_false)) {
                auto& storage = at(arg);
                if (storage.empty()) {
                    storage.push_back(arg->m_const());
                }
                arg->handle();
                return true;
            } else if (arg->m_action == Action::append_const) {
                at(arg).push_back(arg->m_const());
                arg->handle(arg->m_const());
                return true;
            } else if (arg->m_action == Action::count) {
                at(arg).emplace_back(std::string());
                arg->handle();
                return true;
            }
            return false;
        }

        bool exists(std::string const& key) const
        {
            return std::find_if(std::begin(m_data), std::end(m_data),
                                [key] (value_type const& pair) -> bool
            { return *(pair.first) == key; }) != std::end(m_data);
        }

        bool exists(key_type const& key) const
        {
            return m_data.count(key) != 0;
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

        mapped_type& at(key_type const& key)
        {
            return m_data.at(key);
        }

        mapped_type const& at(key_type const& key) const
        {
            return m_data.at(key);
        }

        inline iterator       begin()        { return std::begin(m_data); }
        inline iterator       end()          { return std::end(m_data); }
        inline const_iterator begin()  const { return std::begin(m_data); }
        inline const_iterator end()    const { return std::end(m_data); }

    private:
        std::string const& conflict_arg(key_type const& arg) const
        {
            auto _get_argument_flags = [] (key_type const& arg) -> std::vector<std::string> const&
            {
                return arg->m_dest_str.empty() ? arg->m_flags : arg->m_dest;
            };
            auto const& arg_flags = _get_argument_flags(arg);
            for (auto const& pair : m_data) {
                for (auto const& flag : _get_argument_flags(pair.first)) {
                    if (detail::_is_value_exists(flag, arg_flags)) {
                        return flag;
                    }
                }
            }
            static std::string res;
            return res;
        }

        std::map<key_type, mapped_type> m_data;
    };

public:
    using BaseParser::usage;
    using BaseParser::description;
    using BaseParser::epilog;
    using BaseParser::prefix_chars;

    /*!
     * \brief Parser class
     */
    class Parser : public BaseParser
    {
        friend class ArgumentParser;

    public:
        using BaseParser::usage;
        using BaseParser::description;
        using BaseParser::epilog;
        using BaseParser::prefix_chars;

        /*!
         *  \brief Construct parser with name
         *
         *  \param name Parser name
         *
         *  \return Parser object
         */
        Parser(std::string const& name)
            : BaseParser(),
              m_name(name),
              m_help(),
              m_prefix(),
              m_handle()
        { }

        /*!
         *  \brief Set parser 'usage' value
         *
         *  \param param Usage value
         *
         *  \return Current parser reference
         */
        Parser& usage(std::string const& param)
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
        Parser& description(std::string const& param)
        {
            m_description = detail::_trim_copy(param);
            return *this;
        }

        /*!
         *  \brief Set parser 'epilog' value
         *
         *  \param param Epilog value
         *
         *  \return Current parser reference
         */
        Parser& epilog(std::string const& param)
        {
            m_epilog = detail::_trim_copy(param);
            return *this;
        }

        /*!
         *  \brief Set parser 'prefix_chars' value
         *
         *  \param param Prefix chars values
         *
         *  \return Current parser reference
         */
        Parser& prefix_chars(std::string const& param)
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
        Parser& help(std::string const& value)
        {
            m_help = detail::_trim_copy(value);
            return *this;
        }

        /*!
         *  \brief Set parser 'handle' function
         *
         *  \param func Handle function
         *
         *  \return Current parser reference
         */
        Parser& handle(std::function<void()> func)
        {
            m_handle = func;
            return *this;
        }

        /*!
         *  \brief Get parser 'help' message
         *
         *  \return Parser 'help' message
         */
        std::string const& help() const
        {
            return m_help;
        }

    private:
        void handle() const
        {
            if (m_handle) {
                m_handle();
            }
        }

        std::string print(std::size_t limit = detail::_argument_help_limit) const
        {
            std::string res = "    " + m_name;
            if (!help().empty()) {
                if (res.size() + 2 > limit) {
                    res += "\n" + std::string(detail::_argument_help_limit, ' ') + help();
                } else {
                    res += std::string(limit - res.size(), ' ') + help();
                }
            }
            return res;
        }

        std::string m_name;
        std::string m_help;
        std::string m_prefix;
        std::function<void()> m_handle;
    };

    /*!
     * \brief Subparser class
     */
    class Subparser : public Group
    {
        friend class ArgumentParser;

    public:
        using Group::title;
        using Group::description;

        /*!
         *  \brief Construct subparser
         *
         *  \return Subparser object
         */
        Subparser()
            : Group(),
              m_prog(),
              m_dest(),
              m_required(false),
              m_help(),
              m_metavar(),
              m_parsers()
        { }

        /*!
         *  \brief Set subparser 'title' value
         *
         *  \param value Title value
         *
         *  \return Current subparser reference
         */
        Subparser& title(std::string const& value)
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
        Subparser& description(std::string const& param)
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
        Subparser& prog(std::string const& value)
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
        Subparser& dest(std::string const& value)
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
        Subparser& required(bool value)
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
        Subparser& help(std::string const& value)
        {
            m_help = detail::_trim_copy(value);
            return *this;
        }

        /*!
         *  \brief Set subparser 'metavar' value
         *
         *  \param value Metavar value
         *
         *  \return Current subparser reference
         */
        Subparser& metavar(std::string const& value)
        {
            m_metavar = detail::_trim_copy(value);
            return *this;
        }

        /*!
         *  \brief Get subparser 'prog' value
         *
         *  \return Subparser 'prog' value
         */
        std::string const& prog() const
        {
            return m_prog;
        }

        /*!
         *  \brief Get subparser 'dest' value
         *
         *  \return Subparser 'dest' value
         */
        std::string const& dest() const
        {
            return m_dest;
        }

        /*!
         *  \brief Get subparser 'required' value
         *
         *  \return Subparser 'required' value
         */
        bool required() const
        {
            return m_required;
        }

        /*!
         *  \brief Get subparser 'help' message
         *
         *  \return Subparser 'help' message
         */
        std::string const& help() const
        {
            return m_help;
        }

        /*!
         *  \brief Get subparser 'metavar' value
         *
         *  \return Subparser 'metavar' value
         */
        std::string const& metavar() const
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
        Parser& add_parser(std::string const& name)
        {
            m_parsers.emplace_back(Parser(name));
            return m_parsers.back();
        }

    private:
        void limit_usage(std::size_t& limit) const override
        {
            auto const str = usage();
            if (limit < str.size()) {
                limit = str.size();
            }
        }

        void limit_help_flags(std::size_t& limit) const override
        {
            auto size = flags_to_string().size();
            if (limit < size) {
                limit = size;
            }
            for (auto const& arg : m_parsers) {
                auto size = arg.m_name.size() + 2;
                if (limit < size) {
                    limit = size;
                }
            }
        }

        void print_help(std::ostream& os, bool, detail::Value<std::string> const&,
                        std::size_t limit) const override
        {
            os << std::endl << (title().empty() ? "subcommands" : title()) << ":" << std::endl;
            if (!description().empty()) {
                os << "  " << description() << std::endl << std::endl;
            }
            os << print(limit) << std::endl;
            for (auto const& arg : m_parsers) {
                os << arg.print(limit) << std::endl;
            }
        }

        std::string usage() const
        {
            return flags_to_string() + " ...";
        }

        std::string flags_to_string() const
        {
            if (m_metavar.status()) {
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

        std::string print(std::size_t limit = detail::_argument_help_limit) const
        {
            std::string res = "  " + flags_to_string();
            if (!help().empty()) {
                if (res.size() + 2 > limit) {
                    res += "\n" + std::string(detail::_argument_help_limit, ' ') + help();
                } else {
                    res += std::string(limit - res.size(), ' ') + help();
                }
            }
            return res;
        }

        std::string m_prog;
        std::string m_dest;
        bool        m_required;
        std::string m_help;
        detail::Value<std::string> m_metavar;
        std::deque<Parser> m_parsers;
    };

    /*!
     * \brief Object with parsed arguments
     */
    class Namespace
    {
        template <class T>       struct is_stl_container:std::false_type{};
        template <class... Args> struct is_stl_container<std::deque             <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::forward_list      <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::list              <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::multiset          <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::priority_queue    <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::set               <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::vector            <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::unordered_multiset<Args...> >:std::true_type{};
        template <class... Args> struct is_stl_container<std::unordered_set     <Args...> >:std::true_type{};

        template <class T>       struct is_stl_array:std::false_type{};
        template <class T,
                  std::size_t N> struct is_stl_array<std::array                 <T, N> >   :std::true_type{};

        template <class T>       struct is_stl_queue:std::false_type{};
        template <class... Args> struct is_stl_queue<std::stack                 <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_queue<std::queue                 <Args...> >:std::true_type{};

    public:
        /*!
         *  \brief Construct object with parsed arguments
         *
         *  \param arguments Parsed arguments
         *  \param unrecognized_args Unrecognized arguments
         *
         *  \return Object with parsed arguments
         */
        Namespace(Storage const& arguments,
                  std::vector<std::string> const& unrecognized_args = std::vector<std::string>())
            : m_arguments(arguments),
              m_unrecognized_args(unrecognized_args)
        { }

        /*!
         *  \brief Construct object with parsed arguments
         *
         *  \param arguments Parsed arguments
         *  \param unrecognized_args Unrecognized arguments
         *
         *  \return Object with parsed arguments
         */
        Namespace(Storage&& arguments, std::vector<std::string>&& unrecognized_args)
            : m_arguments(std::move(arguments)),
              m_unrecognized_args(std::move(unrecognized_args))
        { }

        /*!
         *  \brief Check if argument name exists and specified in parsed arguments
         *
         *  \param key Argument name
         *
         *  \return true if argument name exists and specified, otherwise false
         */
        bool exists(std::string const& key) const
        {
            if (m_arguments.exists(key)) {
                return !m_arguments.at(key).second.empty();
            }
            for (auto const& pair : m_arguments) {
                if (pair.first->m_type == Argument::Optional && pair.first->m_dest_str.empty()) {
                    for (auto const& flag : pair.first->m_flags) {
                        if (detail::_flag_name(flag) == key) {
                            return !pair.second.empty();
                        }
                    }
                }
            }
            return false;
        }

        /*!
         *  \brief Get parsed argument value for integer types
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value
         */
        template <class T, typename std::enable_if<std::is_integral<T>::value
                                                   and not std::is_same<bool, T>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto const& args = data(key);
            if (args.first->m_action == Action::count) {
                return T(args.second.size());
            }
            if (args.second.empty()) {
                return T();
            }
            if (args.second.size() != 1) {
                throw TypeError("trying to get data from array argument '" + key + "'");
            }
            return to_type<T>(args.second.front());
        }

        /*!
         *  \brief Get parsed argument value for boolean, floating point and string types
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value
         */
        template <class T, typename std::enable_if<std::is_same<bool, T>::value
                                                   or std::is_floating_point<T>::value
                                                   or std::is_same<std::string, T>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto const& args = data(key);
            if (args.first->m_action == Action::count) {
                throw TypeError("invalid get type for argument '" + key + "'");
            }
            if (args.second.empty()) {
                return T();
            }
            if (args.second.size() != 1) {
                throw TypeError("trying to get data from array argument '" + key + "'");
            }
            return to_type<T>(args.second.front());
        }

        /*!
         *  \brief Get parsed argument value for std containers types
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value
         */
        template <class T,
                  typename std::enable_if<is_stl_container<typename std::decay<T>::type>::value>::type*
                                                                                                    = nullptr>
        T get(std::string const& key) const
        {
            auto vector = to_vector<typename T::value_type>(data(key).second);
            return T(std::begin(vector), std::end(vector));
        }

        /*!
         *  \brief Get parsed argument value std array type
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value
         */
        template <class T,
                  typename std::enable_if<is_stl_array<typename std::decay<T>::type>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto vector = to_vector<typename T::value_type>(data(key).second);
            T res{};
            if (res.size() != vector.size()) {
                std::cerr << "error: array size mismatch: " << res.size()
                          << ", expected " << vector.size() << std::endl;
            }
            std::copy_n(std::begin(vector), std::min(res.size(), vector.size()), std::begin(res));
            return res;
        }

        /*!
         *  \brief Get parsed argument value for queue types
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value
         */
        template <class T,
                  typename std::enable_if<is_stl_queue<typename std::decay<T>::type>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto vector = to_vector<typename T::value_type>(data(key).second);
            return T(std::deque<typename T::value_type>(std::begin(vector),
                                                        std::end(vector)));
        }

        /*!
         *  \brief Get parsed argument value for custom types
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value
         */
        template <class T,
                  typename std::enable_if<not std::is_integral<T>::value
                                          and not std::is_same<bool, T>::value
                                          and not std::is_floating_point<T>::value
                                          and not std::is_same<std::string, T>::value
                                          and not is_stl_container<typename std::decay<T>::type>::value
                                          and not is_stl_array<typename std::decay<T>::type>::value
                                          and not is_stl_queue<typename std::decay<T>::type>::value>
                  ::type* = nullptr>
        T get(std::string const& key) const
        {
            auto const& args = data(key);
            return to_type<T>(detail::_vector_to_string(args.second));
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
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return detail::_have_quotes(args.second.front())
                            ? args.second.front()
                            : detail::_replace(args.second.front(), ' ', "\\ ");
                case Action::store_true :
                case Action::store_false :
                    if (args.second.empty()) {
                        return detail::_bool_to_string(args.first->m_default());
                    }
                    if (args.second.size() != 1) {
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return detail::_bool_to_string(args.second.front());
                case Action::count :
                    return std::to_string(args.second.size());
                case Action::store :
                case Action::append :
                case Action::append_const :
                case Action::extend :
                    return detail::_vector_to_string(args.second, " ", std::string(), true);
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
        std::string to_string(std::string const& key, std::string const& quotes = std::string()) const
        {
            auto const& args = data(key);
            switch (args.first->m_action) {
                case Action::store_const :
                    if (args.second.empty()) {
                        return std::string();
                    }
                    if (args.second.size() != 1) {
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return args.second.front();
                case Action::store_true :
                case Action::store_false :
                    if (args.second.empty()) {
                        return detail::_bool_to_string(args.first->m_default());
                    }
                    if (args.second.size() != 1) {
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return detail::_bool_to_string(args.second.front());
                case Action::count :
                    return std::to_string(args.second.size());
                case Action::store :
                case Action::append :
                case Action::append_const :
                case Action::extend :
                    return "[" + detail::_vector_to_string(args.second, ", ", quotes, false, "None") + "]";
                default :
                    throw ValueError("action not supported");
            }
        }

        /*!
         *  \brief Get unrecognized arguments
         *
         *  \return Unrecognized arguments
         */
        std::vector<std::string> const& unrecognized_args() const
        {
            return m_unrecognized_args;
        }

        /*!
         *  \brief Get unrecognized arguments as args string
         *
         *  \return Unrecognized arguments as args string
         */
        std::string unrecognized_args_to_args() const
        {
            return detail::_vector_to_string(m_unrecognized_args, " ", std::string(), true);
        }

    private:
        Storage::value_type const& data(std::string const& key) const
        {
            if (m_arguments.exists(key)) {
                return m_arguments.at(key);
            }
            for (auto const& pair : m_arguments) {
                if (pair.first->m_type == Argument::Optional && pair.first->m_dest_str.empty()) {
                    for (auto const& flag : pair.first->m_flags) {
                        if (detail::_flag_name(flag) == key) {
                            return pair;
                        }
                    }
                }
            }
            throw AttributeError("'Namespace' object has no attribute '" + key + "'");
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

        template <class T, typename std::enable_if<std::is_same<bool, T>::value>::type* = nullptr>
        T to_type(std::string const& data) const
        {
            return detail::_string_to_bool(data);
        }

        template <class T,
                  typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
        T to_type(std::string const& data) const
        {
            return detail::_remove_quotes(data);
        }

        template <class T,
                  typename std::enable_if<not std::is_constructible<std::string, T>::value
                                          and not std::is_same<bool, T>::value>::type* = nullptr>
        T to_type(std::string const& data) const
        {
            if (data.empty()) {
                return T();
            }
            T result;
            std::stringstream ss(data);
            ss >> result;
            if (ss.fail() || !ss.eof()) {
                throw TypeError("can't convert value '" + data + "'");
            }
            return result;
        }

        Namespace& operator =(Namespace const&) = delete;

        Storage m_arguments;
        std::vector<std::string> m_unrecognized_args;
    };

public:
    /*!
     *  \brief Construct argument parser with concrete program name
     *
     *  \param prog Program value
     *
     *  \return Argument parser object
     */
    explicit ArgumentParser(std::string const& prog = "untitled")
        : BaseParser(),
          m_prog(prog),
          m_parents(),
          m_formatter_class(),
          m_fromfile_prefix_chars(),
          m_argument_default(),
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
    ArgumentParser(int argc, char const* argv[])
        : ArgumentParser(detail::_file_name(argv[0]))
    {
        m_parsed_arguments.reserve(argc - 1);
        for (int i = 1; i < argc; ++i) {
            m_parsed_arguments.emplace_back(std::string(argv[i]));
        }
    }

    /*!
     *  \brief Destroy argument parser
     */
    ~ArgumentParser() = default;

    /*!
     *  \brief Set argument parser 'prog' value
     *
     *  \param param Program value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& prog(std::string const& param)
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
    ArgumentParser& usage(std::string const& param)
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
    ArgumentParser& description(std::string const& param)
    {
        m_description = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'epilog' value
     *
     *  \param param Epilog value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& epilog(std::string const& param)
    {
        m_epilog = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param param Parents values
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& parents(std::vector<ArgumentParser> const& param)
    {
        m_parents = param;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param param Formatter value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& formatter_class(HelpFormatter param)
    {
        m_formatter_class = param;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'prefix_chars' value
     *
     *  \param param Prefix chars values
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& prefix_chars(std::string const& param)
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
    ArgumentParser& fromfile_prefix_chars(std::string const& param)
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
    ArgumentParser& argument_default(std::string const& param)
    {
        m_argument_default = detail::_trim_copy(param);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'add_help' value
     *
     *  \param value Add help flag
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& add_help(bool value)
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
    ArgumentParser& allow_abbrev(bool value)
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
    ArgumentParser& exit_on_error(bool value)
    {
        m_exit_on_error = value;
        return *this;
    }

    /*!
     *  \brief Get argument parser 'prog' value
     *
     *  \return Argument parser 'prog' value
     */
    std::string const& prog() const
    {
        return m_prog;
    }

    /*!
     *  \brief Get argument parser 'fromfile_prefix_chars' value
     *
     *  \return Argument parser 'fromfile_prefix_chars' value
     */
    std::string const& fromfile_prefix_chars() const
    {
        return m_fromfile_prefix_chars;
    }

    /*!
     *  \brief Get argument parser 'argument_default' value
     *
     *  \return Argument parser 'argument_default' value
     */
    std::string const& argument_default() const
    {
        return m_argument_default();
    }

    /*!
     *  \brief Get argument parser 'add_help' value
     *
     *  \return Argument parser 'add_help' value
     */
    bool add_help() const
    {
        return m_add_help;
    }

    /*!
     *  \brief Get argument parser 'allow_abbrev' value
     *
     *  \return Argument parser 'allow_abbrev' value
     */
    bool allow_abbrev() const
    {
        return m_allow_abbrev;
    }

    /*!
     *  \brief Get argument parser 'exit_on_error' value
     *
     *  \return Argument parser 'exit_on_error' value
     */
    bool exit_on_error() const
    {
        return m_exit_on_error;
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
        m_subparsers = std::make_shared<Subparser>();
        m_subparsers->m_position = m_positional.size();
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
        auto const optional = optional_arguments(true, true);
        for (auto const& arg : positional) {
            if (detail::_is_value_exists(dest, arg->m_flags)) {
                return default_argument_value(*arg)();
            }
        }
        for (auto const& arg : optional) {
            if (!arg->m_dest_str.empty()) {
                if (arg->dest() == dest) {
                    return default_argument_value(*arg)();
                }
            } else {
                for (auto const& flag : arg->m_flags) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        return default_argument_value(*arg)();
                    }
                }
            }
        }
        for (auto const& pair : m_default_values) {
            if (pair.first == dest) {
                return pair.second;
            }
        }
        return std::string();
    }

    /*!
     *  \brief Set default values for certain arguments
     *
     *  \param values Vector of pairs: { 'argument flag', 'default value' }
     */
    void set_defaults(std::vector<std::pair<std::string, std::string> > const& values)
    {
        auto _set_value = [] (Argument& arg, std::string const& dest, std::string const& value)
        {
            if (arg.m_type == Argument::Positional) {
                if (detail::_is_value_exists(dest, arg.m_flags)) {
                    arg.default_value(value);
                    return true;
                }
            } else if (!arg.m_dest_str.empty()) {
                if (arg.m_dest_str == dest) {
                    arg.default_value(value);
                    return true;
                }
            } else {
                for (auto const& flag : arg.m_flags) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        arg.default_value(value);
                        return true;
                    }
                }
            }
            return false;
        };
        for (auto const& pair : values) {
            auto dest = detail::_trim_copy(pair.first);
            if (dest.empty()) {
                continue;
            }
            auto value = detail::_trim_copy(pair.second);
            bool found = false;
            for (auto& parent : m_parents) {
                for (auto& arg : parent.m_arguments) {
                    if (_set_value(*arg, dest, value)) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
            if (found) {
                continue;
            }
            for (auto& arg : m_arguments) {
                if (_set_value(*arg, dest, value)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                m_default_values.push_back({ dest, value });
            }
        }
    }

    /*!
     *  \brief Parse command line arguments
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_args() const
    {
        return parse_args(m_parsed_arguments);
    }

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_args(T const& args) const
    {
        return parse_args(detail::_split_to_args(args));
    }

    /*!
     *  \brief Parse concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_args(std::vector<std::string> const& args) const
    {
        if (m_exit_on_error) {
            try {
                return parse_arguments(args);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            } catch (...) {
                std::cerr << "error: unexpected error" << std::endl;
            }
            ::exit(1);
        } else {
            return parse_arguments(args);
        }
    }

    /*!
     *  \brief Parse known command line arguments
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_known_args() const
    {
        return parse_known_args(m_parsed_arguments);
    }

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_known_args(T const& args) const
    {
        return parse_known_args(detail::_split_to_args(args));
    }

    /*!
     *  \brief Parse known concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_known_args(std::vector<std::string> const& args) const
    {
        if (m_exit_on_error) {
            try {
                return parse_arguments(args, true);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            } catch (...) {
                std::cerr << "error: unexpected error" << std::endl;
            }
            ::exit(1);
        } else {
            return parse_arguments(args, true);
        }
    }

    /*!
     *  \brief Parse intermixed command line arguments
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_intermixed_args() const
    {
        return parse_intermixed_args(m_parsed_arguments);
    }

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_intermixed_args(T const& args) const
    {
        return parse_intermixed_args(detail::_split_to_args(args));
    }

    /*!
     *  \brief Parse intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_intermixed_args(std::vector<std::string> const& args) const
    {
        if (m_exit_on_error) {
            try {
                return parse_arguments(args, false, true);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            } catch (...) {
                std::cerr << "error: unexpected error" << std::endl;
            }
            ::exit(1);
        } else {
            return parse_arguments(args, false, true);
        }
    }

    /*!
     *  \brief Parse known intermixed command line arguments
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_known_intermixed_args() const
    {
        return parse_known_intermixed_args(m_parsed_arguments);
    }

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    template<class T,
             typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_known_intermixed_args(T const& args) const
    {
        return parse_known_intermixed_args(detail::_split_to_args(args));
    }

    /*!
     *  \brief Parse known intermixed concrete arguments
     *
     *  \param args Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_known_intermixed_args(std::vector<std::string> const& args) const
    {
        if (m_exit_on_error) {
            try {
                return parse_arguments(args, true, true);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            } catch (...) {
                std::cerr << "error: unexpected error" << std::endl;
            }
            ::exit(1);
        } else {
            return parse_arguments(args, true, true);
        }
    }

    /*!
     *  \brief Print program usage
     *
     *  \param os Output stream
     */
    void print_usage(std::ostream& os = std::cout) const
    {
        if (!usage().empty()) {
            os << "usage: " << usage() << std::endl;
        } else {
            auto const positional = positional_arguments(false, true);
            auto const optional = optional_arguments(false, true);
            auto const subparser = subpurser_info(false);
            print_custom_usage(positional, optional, m_groups, m_exclusive, subparser, m_prog, os);
        }
    }

    /*!
     *  \brief Print program help message
     *
     *  \param os Output stream
     */
    void print_help(std::ostream& os = std::cout) const
    {
        auto const positional_all = positional_arguments(false, true);
        auto const optional_all = optional_arguments(false, true);
        auto const positional = positional_arguments(false, false);
        auto const optional = optional_arguments(false, false);
        auto const subparser = subpurser_info(false);
        print_custom_help(positional_all, optional_all, positional, optional, m_groups, m_exclusive,
                          subparser, m_prog, m_usage, m_description, m_epilog, os);
    }

    /*!
     *  \brief This method terminates the program, exiting with the specified
     *  status and, if given, it prints a message before that
     *
     *  \param status Status code
     *  \param message Error message
     */
    void exit(int status = 0, std::string const& message = std::string()) const
    {
        if (status) {
            throw std::logic_error("Exiting because of an error: " + message);
        }
        ::exit(status);
    }

    /*!
     *  \brief This method prints a usage message including the message to the
     *  standard error and terminates the program with a status code of 2
     *
     *  \param message Error message
     */
    void error(std::string const& message) const
    {
        print_usage(std::cerr);
        std::cerr << m_prog << ": error: " << message << std::endl;
        ::exit(2);
    }

private:
    Namespace parse_arguments(std::vector<std::string> parsed_arguments,
                              bool only_known = false, bool intermixed = false) const
    {
        auto const subparser = subpurser_info();
        if (intermixed && subparser.first) {
            throw TypeError("parse_intermixed_args: positional arg with nargs=A...");
        }
        Parser const* parser = nullptr;
        auto _throw_error = [this, &parser] (std::string const& error, std::ostream& os = std::cerr)
        {
            if (parser) {
                if (!parser->m_usage.empty()) {
                    os << "usage: " << parser->m_usage << std::endl;
                } else {
                    print_custom_usage(parser->get_positional(true),
                                       parser->get_optional_with_help(true, m_add_help, m_prefix_chars),
                                       parser->m_groups, parser->m_exclusive,
                                       { nullptr, 0 }, parser->m_prefix, os);
                }
                throw std::logic_error(parser->m_prefix + ": error: " + error);
            } else {
                throw_error(error, os);
            }
        };
        auto _validate_arguments = [] (std::vector<pArgument> const& arguments)
        {
            for (auto const& arg : arguments) {
                if ((arg->m_action & (Action::store_const | Action::append_const))
                        && !arg->m_const.status()) {
                    throw TypeError("missing 1 required positional argument: 'const'");
                }
                for (auto const& flag : arg->m_flags) {
                    if (flag == detail::_pseudo_argument && arg->m_dest_str.empty()) {
                        throw ValueError("dest= is required for options like '--'");
                    }
                }
            }
        };
        auto _validate_argument_value = [_throw_error] (Argument const& arg, std::string const& value)
        {
            auto const& choices = arg.m_choices;
            if (choices.status()) {
                auto str = detail::_remove_quotes(value);
                if (!str.empty() && !detail::_is_value_exists(str, choices())) {
                    auto values = detail::_vector_to_string(choices(), ", ", "'");
                    _throw_error("argument " + arg.m_flags.front()
                                  + ": invalid choice: '" + str + "' (choose from " + values + ")");
                }
            }
        };
        auto _negative_numbers_presented = [] (std::vector<pArgument> const& optionals,
                std::string const& prefix_chars)
        {
            if (detail::_is_value_exists(detail::_default_prefix_char, prefix_chars)) {
                for (auto const& arg : optionals) {
                    for (auto const& flag : arg->m_flags) {
                        if (detail::_is_negative_number(flag)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        auto positional = positional_arguments(true, true);
        auto const optional = optional_arguments(true, true);
        std::vector<pArgument> sub_optional;
        std::string subparser_dest;
        std::vector<std::string> subparser_flags;

        _validate_arguments(positional);
        _validate_arguments(optional);
        if (subparser.first) {
            for (auto const& parser : subparser.first->m_parsers) {
                _validate_arguments(parser.m_arguments);
            }
            subparser_dest = subparser.first->m_dest;
            if (!subparser_dest.empty()) {
                subparser_flags.push_back(subparser_dest);
            }
        }

        bool have_negative_args = _negative_numbers_presented(optional, m_prefix_chars);
        bool was_pseudo_arg = false;

        auto subparser_name = subparser_dest;
        auto subparser_arg = std::make_shared<Argument>(std::move(subparser_flags),
                                                        std::move(subparser_name), Argument::Positional);
        Storage result;
        result.create(positional);
        result.create(optional);
        if (subparser.first) {
            if (!subparser_dest.empty()) {
                result.create(subparser_arg);
            }
            for (auto const& parser : subparser.first->m_parsers) {
                result.create(parser.m_arguments);
            }
        }

        auto _optional_arg_by_flag = [&optional, &sub_optional, &parser]
                (std::string const& key) -> pArgument const
        {
            for (auto const& arg : (parser ? sub_optional : optional)) {
                if (detail::_is_value_exists(key, arg->m_flags)) {
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
        auto _store_value = [_validate_argument_value, &result] (pArgument const& arg, std::string const& val)
        {
            _validate_argument_value(*arg, val);
            result.store_value(arg, val);
        };
        auto _store_default_value = [this, &result] (pArgument const& arg)
        {
            auto const& value = default_argument_value(*arg);
            if (value.status()) {
                result.store_default_value(arg, value());
            }
        };
        auto _is_positional_arg_stored = [_throw_error, &result] (pArgument const& arg) -> bool
        {
            if (arg->m_action == Action::append_const && arg->m_default.status()) {
                _throw_error(detail::_ignore_default(arg->m_flags.front(), arg->m_default()));
            }
            return result.self_value_stored(arg);
        };
        auto _match_positionals = [_is_positional_arg_stored, _store_value, _store_default_value, &pos]
                (std::vector<pArgument> const& positional, std::deque<std::string>& arguments,
                std::size_t finish, std::size_t min_args, std::size_t one_args, bool more_args)
        {
            auto _store_first_value = [_store_value] (pArgument const& arg, std::deque<std::string>& values)
            {
                _store_value(arg, values.front());
                values.pop_front();
            };
            if (min_args == arguments.size()) {
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    switch (arg->m_nargs) {
                        case Argument::NARGS_DEF :
                        case Argument::ONE_OR_MORE :
                            _store_first_value(arg, arguments);
                            break;
                        case Argument::OPTIONAL :
                        case Argument::ZERO_OR_MORE :
                            _store_default_value(arg);
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg->m_num_args; ++n) {
                                _store_first_value(arg, arguments);
                            }
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
                    switch (arg->m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_first_value(arg, arguments);
                            break;
                        case Argument::ONE_OR_MORE :
                            _store_first_value(arg, arguments);
                            while (over_args > 0) {
                                _store_first_value(arg, arguments);
                                --over_args;
                            }
                            break;
                        case Argument::OPTIONAL :
                            _store_default_value(arg);
                            break;
                        case Argument::ZERO_OR_MORE :
                            if (over_args > 0) {
                                while (over_args > 0) {
                                    _store_first_value(arg, arguments);
                                    --over_args;
                                }
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg->m_num_args; ++n) {
                                _store_first_value(arg, arguments);
                            }
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
                    switch (arg->m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_first_value(arg, arguments);
                            break;
                        case Argument::OPTIONAL :
                            if (over_args < one_args) {
                                _store_first_value(arg, arguments);
                                ++over_args;
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg->m_num_args; ++n) {
                                _store_first_value(arg, arguments);
                            }
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
                    if (arg->m_nargs == Argument::NARGS_DEF) {
                        _store_first_value(arg, arguments);
                    } else {
                        for (std::size_t n = 0; n < arg->m_num_args; ++n) {
                            _store_first_value(arg, arguments);
                        }
                    }
                }
            }
        };
        auto _try_capture_parser = [&] (std::deque<std::string>& arguments)
        {
            auto size = arguments.size();
            std::size_t finish = pos;
            std::size_t min_args = 0;
            std::size_t one_args = 0;
            bool more_args = false;
            bool capture_need = false;
            for ( ; finish < positional.size(); ++finish) {
                if (finish == subparser.second) {
                    if (min_args + 1 > size) {
                        break;
                    }
                    capture_need = true;
                    break;
                }
                auto const& arg = positional.at(finish);
                if (!(arg->m_action & (Action::store | Action::append | Action::extend))) {
                    continue;
                }
                std::size_t min_amount = 0;
                switch (arg->m_nargs) {
                    case Argument::OPTIONAL :
                        ++one_args;
                        break;
                    case Argument::ONE_OR_MORE :
                        ++min_amount;
                    case Argument::ZERO_OR_MORE :
                        more_args = true;
                        break;
                    default :
                        min_amount += arg->m_num_args;
                        break;
                }
                if (min_args + min_amount > size) {
                    break;
                }
                min_args += min_amount;
            }
            if (!capture_need && (finish != positional.size() || min_args >= size)) {
                if (finish != pos) {
                    _match_positionals(positional, arguments, finish, min_args, one_args, more_args);
                }
                unrecognized_args.insert(std::end(unrecognized_args),
                                         std::begin(arguments), std::end(arguments));
                return;
            }
            _match_positionals(positional, arguments, finish, ++min_args, one_args, more_args);
            auto const& name = arguments.front();
            std::string choices;
            for (auto const& p : subparser.first->m_parsers) {
                if (!choices.empty()) {
                    choices += ", ";
                }
                choices += "'" + p.m_name + "'";
                if (p.m_name == name) {
                    parser = &p;
                    if (!subparser_dest.empty()) {
                        result.at(subparser_arg).push_back(name);
                    }
                    break;
                }
            }
            if (parser) {
                sub_optional = parser->get_optional_with_help(true, m_add_help, parser->m_prefix_chars);
                auto sub_positional = parser->get_positional(true);
                positional.insert(std::begin(positional) + subparser.second,
                                  std::begin(sub_positional), std::end(sub_positional));
                arguments.pop_front();
                have_negative_args = _negative_numbers_presented(sub_optional, parser->m_prefix_chars);

                bool add_suppress = false;
                auto info = subpurser_info(add_suppress);
                auto program = m_subparsers->m_prog;
                if (program.empty()) {
                    program = m_prog;
                }
                auto const positional_args = positional_arguments(add_suppress, true);
                for (std::size_t i = 0; i < positional_args.size(); ++i) {
                    if (info.second == i) {
                        break;
                    }
                    auto const str = positional_args.at(i)->usage();
                    if (str.empty()) {
                        continue;
                    }
                    program += " " + str;
                }
                const_cast<std::string&>(parser->m_prefix) = program + " " + parser->m_name;
                parser->handle();
            } else {
                throw_error("invalid choice: '" + name + "' (choose from " + choices + ")");
            }
        };
        auto _match_args_partial = [_match_positionals, &pos, &positional, &unrecognized_args]
                (std::deque<std::string>& arguments)
        {
            if (pos >= positional.size()) {
                unrecognized_args.insert(std::end(unrecognized_args),
                                         std::begin(arguments), std::end(arguments));
                return;
            }
            std::size_t finish = pos;
            std::size_t min_args = 0;
            std::size_t one_args = 0;
            bool more_args = false;
            for ( ; finish < positional.size(); ++finish) {
                auto const& arg = positional.at(finish);
                if (!(arg->m_action & (Action::store | Action::append | Action::extend))) {
                    continue;
                }
                std::size_t min_amount = 0;
                switch (arg->m_nargs) {
                    case Argument::OPTIONAL :
                        ++one_args;
                        break;
                    case Argument::ONE_OR_MORE :
                        ++min_amount;
                    case Argument::ZERO_OR_MORE :
                        more_args = true;
                        break;
                    default :
                        min_amount += arg->m_num_args;
                        break;
                }
                if (min_args + min_amount > arguments.size()) {
                    break;
                }
                min_args += min_amount;
            }
            if (finish == pos) {
                unrecognized_args.insert(std::end(unrecognized_args),
                                         std::begin(arguments), std::end(arguments));
                return;
            }
            _match_positionals(positional, arguments, finish, min_args, one_args, more_args);
            unrecognized_args.insert(std::end(unrecognized_args),
                                     std::begin(arguments), std::end(arguments));
        };
        auto _separate_arg_abbrev = [_optional_arg_by_flag, _prefix_chars]
                (std::vector<std::string>& temp, std::string const& arg,
                std::string const& name, std::vector<pArgument> const& optionals)
        {
            if (name.size() + 1 == arg.size()) {
                auto const splitted = detail::_split_equal(arg, _prefix_chars());
                if (splitted.size() == 2 && !splitted.front().empty()
                        && _optional_arg_by_flag(splitted.front())) {
                    temp.push_back(arg);
                    return;
                }
                std::vector<std::string> flags;
                for (std::size_t i = 0; i < name.size(); ++i) {
                    if (name.at(i) == detail::_equal) {
                        if (flags.empty()) {
                            flags.push_back(name.substr(i));
                        } else {
                            flags.back() += name.substr(i);
                        }
                        break;
                    }
                    Argument const* argument = nullptr;
                    for (auto const& opt : optionals) {
                        for (auto const& flag : opt->m_flags) {
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
                                                & (Action::store | Action::append | Action::extend)))) {
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
                temp.insert(std::end(temp), std::begin(flags), std::end(flags));
            } else {
                temp.push_back(arg);
            }
        };
        auto _check_load_args = [this] (std::vector<std::string>& arguments, size_t i)
        {
            while (!arguments.at(i).empty()
                   && detail::_is_value_exists(arguments.at(i).front(), m_fromfile_prefix_chars)) {
                auto const load_args = convert_arg_line_to_args(arguments.at(i).substr(1));
                arguments.erase(std::begin(arguments) + i);
                arguments.insert(std::begin(arguments) + i, std::begin(load_args), std::end(load_args));
            }
        };
        auto _check_abbreviations = [this, _separate_arg_abbrev, &result,
                _throw_error, _prefix_chars, &have_negative_args, &was_pseudo_arg]
                (std::vector<std::string>& arguments, size_t i, std::vector<pArgument> const& optionals)
        {
            auto& arg = arguments.at(i);
            if (!arg.empty() && !result.exists(arg)
                    && detail::_is_optional(arg, _prefix_chars(), have_negative_args, was_pseudo_arg)) {
                std::vector<std::string> temp;
                if (m_allow_abbrev) {
                    bool is_flag_added = false;
                    std::string args;
                    std::vector<std::string> keys;
                    for (auto const& opt : optionals) {
                        for (auto const& flag : opt->m_flags) {
                            if (detail::_starts_with(flag, arg)) {
                                is_flag_added = true;
                                keys.push_back(flag);
                                if (!args.empty()) {
                                    args += ",";
                                }
                                args += " " + flag;
                                break;
                            }
                            if (flag.size() == 2 && detail::_starts_with(arg, flag)) {
                                keys.push_back(arg);
                                if (!args.empty()) {
                                    args += ",";
                                }
                                args += " " + flag;
                                break;
                            }
                        }
                    }
                    if (keys.size() > 1) {
                        _throw_error("ambiguous option: '" + arg + "' could match" + args);
                    }
                    if (is_flag_added) {
                        temp.push_back(keys.empty() ? arg : keys.front());
                    } else {
                        auto name = detail::_flag_name(keys.empty() ? arg : keys.front());
                        _separate_arg_abbrev(temp, arg, name, optionals);
                    }
                } else {
                    _separate_arg_abbrev(temp, arg, detail::_flag_name(arg), optionals);
                }
                arguments.erase(std::begin(arguments) + i);
                arguments.insert(std::begin(arguments) + i, std::begin(temp), std::end(temp));
            }
        };
        auto _print_help_and_exit = [this, &parser] ()
        {
            if (parser) {
                auto opt_all = parser->get_optional(true);
                auto opt = parser->get_optional(false);
                if (m_add_help) {
                    auto help = std::make_shared<Argument>(detail::_help_flags(parser->m_prefix_chars),
                                                           "help", Argument::Optional);
                    help->help("show this help message and exit").action(Action::help);
                    opt_all.insert(std::begin(opt_all), help);
                    opt.insert(std::begin(opt), help);
                }
                print_custom_help(parser->get_positional(true), opt_all, parser->get_positional(false), opt,
                                  parser->m_groups, parser->m_exclusive, { nullptr, 0 }, parser->m_prefix,
                                  parser->usage(), parser->description(), parser->epilog());
            } else {
                print_help();
            }
            ::exit(0);
        };
        for (std::size_t i = 0; i < parsed_arguments.size(); ++i) {
            if (parsed_arguments.at(i) == detail::_pseudo_argument && !was_pseudo_arg) {
                was_pseudo_arg = true;
                continue;
            }
            if (!m_fromfile_prefix_chars.empty() && !was_pseudo_arg) {
                _check_load_args(parsed_arguments, i);
            }
            _check_abbreviations(parsed_arguments, i, parser ? sub_optional : optional);
            auto arg = parsed_arguments.at(i);
            auto splitted = detail::_split_equal(arg, _prefix_chars());
            if (splitted.size() == 2 && !splitted.front().empty()) {
                arg = splitted.front();
            } else {
                splitted.resize(1);
            }
            auto const tmp = (was_pseudo_arg ? nullptr : _optional_arg_by_flag(arg));
            if (tmp) {
                switch (tmp->m_action) {
                    case Action::store :
                        result.at(tmp).clear();
                    case Action::append :
                    case Action::extend :
                        if (splitted.size() == 1) {
                            uint32_t n = 0;
                            uint32_t num_args = tmp->m_num_args;
                            while (true) {
                                if (++i == parsed_arguments.size()) {
                                    if (n == 0) {
                                        switch (tmp->m_nargs) {
                                            case Argument::NARGS_DEF :
                                            case Argument::NARGS_INT :
                                            case Argument::ONE_OR_MORE :
                                                _throw_error(tmp->error_nargs(arg));
                                                break;
                                            case Argument::OPTIONAL :
                                                _store_value(tmp, tmp->const_value());
                                                break;
                                            default :
                                                break;
                                        }
                                    } else if (tmp->m_nargs == Argument::NARGS_INT && n < num_args) {
                                        _throw_error(tmp->error_nargs(arg));
                                    }
                                    break;
                                } else {
                                    auto const& next = parsed_arguments.at(i);
                                    if (next.empty()
                                            || detail::_not_optional(next, _prefix_chars(),
                                                                     have_negative_args, was_pseudo_arg)) {
                                        _store_value(tmp, next);
                                        ++n;
                                    } else if (n == 0) {
                                        --i;
                                        switch (tmp->m_nargs) {
                                            case Argument::NARGS_DEF :
                                            case Argument::NARGS_INT :
                                            case Argument::ONE_OR_MORE :
                                                _throw_error(tmp->error_nargs(arg));
                                                break;
                                            case Argument::OPTIONAL :
                                                _store_value(tmp, tmp->m_const());
                                                break;
                                            default :
                                                break;
                                        }
                                        break;
                                    } else {
                                        if (tmp->m_nargs == Argument::NARGS_INT && n < num_args) {
                                            _throw_error(tmp->error_nargs(arg));
                                        }
                                        --i;
                                        break;
                                    }
                                }
                                if (tmp->m_nargs == Argument::NARGS_DEF
                                        || tmp->m_nargs == Argument::OPTIONAL
                                        || (tmp->m_nargs == Argument::NARGS_INT && n == num_args)) {
                                    break;
                                }
                            }
                        } else {
                            if (tmp->m_nargs != Argument::NARGS_DEF && tmp->m_num_args > 1) {
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
                        if (splitted.size() == 1) {
                            if (tmp->m_action == Action::append_const && !tmp->m_default().empty()) {
                                _throw_error(detail::_ignore_default(tmp->m_flags.front(),
                                                                     tmp->m_default()));
                            }
                            result.self_value_stored(tmp);
                        } else {
                            _throw_error(detail::_ignore_explicit(arg, splitted.back()));
                        }
                        break;
                    case Action::help :
                        if (splitted.size() == 1) {
                            _print_help_and_exit();
                        } else {
                            _throw_error(detail::_ignore_explicit(arg, splitted.back()));
                        }
                        break;
                    case Action::version :
                        if (splitted.size() == 1) {
                            if (!tmp->m_version.status()) {
                                throw AttributeError("'ArgumentParser' object has no attribute 'version'");
                            }
                            std::cout << tmp->version() << std::endl;
                            ::exit(0);
                        } else {
                            _throw_error(detail::_ignore_explicit(arg, splitted.back()));
                        }
                        break;
                    default :
                        _throw_error("action not supported");
                        break;
                }
            } else if (!arg.empty() && detail::_is_optional(arg, _prefix_chars(),
                                                            have_negative_args, was_pseudo_arg)) {
                unrecognized_args.push_back(arg);
            } else {
                std::deque<std::string> values;
                values.push_back(parsed_arguments.at(i));
                while (true) {
                    if (++i == parsed_arguments.size()) {
                        break;
                    } else {
                        auto const& next = parsed_arguments.at(i);
                        if (next.empty() || detail::_not_optional(next, _prefix_chars(),
                                                                  have_negative_args, was_pseudo_arg)) {
                            values.push_back(next);
                        } else {
                            --i;
                            break;
                        }
                    }
                }
                if (intermixed) {
                    intermixed_args.insert(std::end(intermixed_args), std::begin(values), std::end(values));
                } else {
                    if (subparser.first && !parser) {
                        _try_capture_parser(values);
                        if (parser) {
                            i -= (i == parsed_arguments.size());
                            i -= values.size();
                        }
                    } else {
                        _match_args_partial(values);
                    }
                }
            }
        }
        if (!intermixed_args.empty()) {
            _match_args_partial(intermixed_args);
        }
        for (auto const& ex : m_exclusive) {
            std::string args;
            std::string found;
            for (auto const& arg : ex.m_arguments) {
                args += " " + arg->m_flags.front();
                if (!result.at(arg).empty()) {
                    if (!found.empty()) {
                        throw_error("argument " + arg->m_flags.front()
                                     + ": not allowed with argument " + found);
                    }
                    found = arg->m_flags.front();
                }
            }
            if (ex.m_required && found.empty()) {
                if (ex.m_arguments.empty()) {
                    throw IndexError("list index out of range");
                }
                throw_error("one of the arguments" + args + " is required");
            }
        }
        if (parser) {
            for (auto const& ex : parser->m_exclusive) {
                std::string args;
                std::string found;
                for (auto const& arg : ex.m_arguments) {
                    args += " " + arg->m_flags.front();
                    if (!result.at(arg).empty()) {
                        if (!found.empty()) {
                            _throw_error("argument " + arg->m_flags.front()
                                         + ": not allowed with argument " + found);
                        }
                        found = arg->m_flags.front();
                    }
                }
                if (ex.m_required && found.empty()) {
                    if (ex.m_arguments.empty()) {
                        throw IndexError("list index out of range");
                    }
                    _throw_error("one of the arguments" + args + " is required");
                }
            }
        }
        std::vector<std::string> required_args;
        for (auto const& arg : optional) {
            if (arg->m_required && result.at(arg).empty()) {
                auto args = detail::_vector_to_string(arg->m_flags, "/");
                required_args.emplace_back(std::move(args));
            }
        }
        bool subparser_required = subparser.first && !parser && subparser.first->required();
        if (!required_args.empty() || pos < positional.size() || subparser_required) {
            std::string args;
            for ( ; pos < positional.size(); ++pos) {
                if (subparser_required && pos == subparser.second) {
                    if (!args.empty()) {
                        args += ", ";
                    }
                    args += subparser.first->flags_to_string();
                }
                auto const& arg = positional.at(pos);
                if (args.empty()) {
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg->m_nargs == Argument::OPTIONAL || arg->m_nargs == Argument::ZERO_OR_MORE) {
                        _store_default_value(arg);
                        continue;
                    }
                }
                if (!args.empty()) {
                    args += ", ";
                }
                args += arg->m_flags.front();
            }
            if (subparser_required && pos == subparser.second) {
                if (!args.empty()) {
                    args += ", ";
                }
                args += subparser.first->flags_to_string();
            }
            if (!required_args.empty()) {
                args += ", ";
            }
            args += detail::_vector_to_string(required_args, ", ");
            if (!args.empty()) {
                throw_error("the following arguments are required: " + args);
            }
        }
        if (!only_known && !unrecognized_args.empty()) {
            throw_error("unrecognized arguments: " + detail::_vector_to_string(unrecognized_args));
        }
        for (auto& arg : result) {
            if (arg.second.empty() && arg.first->m_action != Action::count
                    && arg.first->m_type == Argument::Optional) {
                auto const& value = default_argument_value(*arg.first);
                if (value.status() || arg.first->action() & (Action::store_true | Action::store_false)) {
                    arg.second.push_back(value());
                }
            }
        }
        for (auto const& pair : m_default_values) {
            if (!result.exists(pair.first)) {
                auto arg = std::make_shared<Argument>(std::vector<std::string>{ pair.first },
                                                      pair.first, Argument::Positional);
                arg->default_value(pair.second);
                result.create(arg, { pair.second });
            }
        }
        return Namespace(std::move(result), std::move(unrecognized_args));
    }

    void throw_error(std::string const& message, std::ostream& os = std::cerr) const
    {
        print_usage(os);
        throw std::logic_error(m_prog + ": error: " + message);
    }

    detail::Value<std::string> const& default_argument_value(Argument const& arg) const
    {
        return (arg.m_default.status() || !m_argument_default.status()) ? arg.m_default : m_argument_default;
    }

    std::vector<std::string> convert_arg_line_to_args(std::string const& file) const
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

    std::vector<pArgument> positional_arguments(bool add_suppress = true, bool add_groups = true) const
    {
        std::vector<pArgument> result;
        result.reserve(m_positional.size());
        for (auto const& parent : m_parents) {
            auto const args = parent.positional_arguments(add_suppress, add_groups);
            result.insert(std::end(result), std::begin(args), std::end(args));
        }
        for (auto const& arg : m_positional) {
            if ((add_suppress || !arg.first->m_help_type.status()) && (add_groups || !arg.second)) {
                result.push_back(arg.first);
            }
        }
        return result;
    }

    std::vector<pArgument> optional_arguments(bool add_suppress = true, bool add_groups = true) const
    {
        std::vector<pArgument> result;
        result.reserve(m_optional.size() + 1);
        if (m_add_help) {
            auto help = std::make_shared<Argument>(detail::_help_flags(m_prefix_chars),
                                                   "help", Argument::Optional);
            help->help("show this help message and exit").action(Action::help);
            result.emplace_back(std::move(help));
        }
        for (auto const& parent : m_parents) {
            auto const args = parent.optional_arguments(add_suppress, add_groups);
            result.insert(std::end(result), std::begin(args), std::end(args));
        }
        for (auto const& arg : m_optional) {
            if ((add_suppress || !arg.first->m_help_type.status()) && (add_groups || !arg.second)) {
                result.push_back(arg.first);
            }
        }
        return result;
    }

    std::pair<Subparser*, std::size_t> subpurser_info(bool add_suppress = true) const
    {
        auto _func = [] (ArgumentParser const& parser,
                std::pair<Subparser*, std::size_t>& res, bool add_suppress)
        {
            for (std::size_t p = 0, a = 0, pos = parser.m_subparsers->m_position,
                 size = parser.m_positional.size(); p < pos && a < size; ++a, ++p) {
                res.second += (add_suppress || !parser.m_positional.at(a).first->m_help_type.status());
            }
        };
        std::pair<Subparser*, std::size_t> res = { nullptr, 0 };
        if (m_subparsers) {
            res.first = m_subparsers.get();
            for (auto const& parent : m_parents) {
                res.second += parent.positional_arguments(add_suppress, true).size();
            }
            _func(*this, res, add_suppress);
        } else {
            for (std::size_t i = 0; i < m_parents.size(); ++i) {
                auto const& parent = m_parents.at(i);
                if (parent.m_subparsers) {
                    res.first = parent.m_subparsers.get();
                    for (std::size_t j = 0; j < i; ++j) {
                        res.second += m_parents.at(j).positional_arguments(add_suppress, true).size();
                    }
                    _func(parent, res, add_suppress);
                    break;
                }
            }
        }
        return res;
    }

    static std::string custom_usage(std::vector<pArgument> const& positional,
                                    std::vector<pArgument> const& optional,
                                    std::deque<pGroup> const& groups,
                                    std::deque<ExclusiveGroup> const& exclusive,
                                    std::pair<Subparser*, std::size_t> const& subparser,
                                    std::string const& program)
    {
        auto res = program;
        std::size_t min_size = 0;
        for (auto const& arg : positional) {
            auto const str = arg->usage();
            if (min_size < str.size()) {
                min_size = str.size();
            }
        }
        auto ex_opt = optional;
        for (auto const& ex : exclusive) {
            for (auto arg : ex.m_arguments) {
                ex_opt.erase(std::remove(std::begin(ex_opt), std::end(ex_opt), arg), std::end(ex_opt));
            }
            auto const str = ex.usage();
            if (min_size < str.size()) {
                min_size = str.size();
            }
        }
        for (auto const& arg : ex_opt) {
            auto const str = arg->usage();
            if (min_size < str.size()) {
                min_size = str.size();
            }
        }
        for (auto const& group : groups) {
            group->limit_usage(min_size);
        }
        std::size_t const usage_length = std::string("usage: ").size();
        std::size_t pos = usage_length + program.size();
        std::size_t offset = usage_length;
        if (pos + (min_size > 0 ? (1 + min_size) : 0) <= detail::_usage_limit) {
            offset += program.size() + (min_size > 0);
        } else if (!(ex_opt.empty() && positional.empty() && !subparser.first)) {
            res += "\n" + std::string(offset - 1, ' ');
            pos = offset - 1;
        }
        auto _arg_usage = [&pos, offset, &res] (std::string const& str, bool bkt)
        {
            if ((pos + 1 == offset) || (pos + 1 + str.size() <= detail::_usage_limit)) {
                res += " ";
            } else {
                res += "\n" + std::string(offset, ' ');
                pos = offset;
            }
            res += (bkt ? "[" + str + "]" : str);
            pos += 1 + str.size();
        };
        for (auto const& arg : ex_opt) {
            _arg_usage(arg->usage(), true);
        }
        for (auto const& ex : exclusive) {
            _arg_usage(ex.usage(), false);
        }
        for (std::size_t i = 0; i < positional.size(); ++i) {
            if (subparser.first && subparser.second == i) {
                _arg_usage(subparser.first->usage(), false);
            }
            auto const str = positional.at(i)->usage();
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

    void print_custom_usage(std::vector<pArgument> const& positional,
                            std::vector<pArgument> const& optional,
                            std::deque<pGroup> const& groups,
                            std::deque<ExclusiveGroup> const& exclusive,
                            std::pair<Subparser*, std::size_t> const& subparser,
                            std::string const& program,
                            std::ostream& os = std::cout) const
    {
        os << "usage: " << custom_usage(positional, optional, groups, exclusive,
                                        subparser, program) << std::endl;
    }

    void print_custom_help(std::vector<pArgument> const& positional_all,
                           std::vector<pArgument> const& optional_all,
                           std::vector<pArgument> const& positional,
                           std::vector<pArgument> const& optional,
                           std::deque<pGroup> const& groups,
                           std::deque<ExclusiveGroup> const& exclusive,
                           std::pair<Subparser*, std::size_t> const& subparser,
                           std::string const& program,
                           std::string const& usage,
                           std::string const& description,
                           std::string const& epilog,
                           std::ostream& os = std::cout) const
    {
        if (!usage.empty()) {
            os << "usage: " << usage << std::endl;
        } else {
            print_custom_usage(positional_all, optional_all, groups, exclusive, subparser, program, os);
        }
        if (!description.empty()) {
            os << std::endl << description << std::endl;
        }
        std::size_t min_size = 0;
        bool show_default = m_formatter_class.status()
                && m_formatter_class() == ArgumentDefaultsHelpFormatter;
        bool sub_positional = subparser.first && subparser.first->title().empty()
                && subparser.first->description().empty();
        for (auto const& arg : positional) {
            auto size = arg->flags_to_string().size();
            if (min_size < size) {
                min_size = size;
            }
        }
        for (auto const& arg : optional) {
            auto size = arg->flags_to_string().size();
            if (min_size < size) {
                min_size = size;
            }
        }
        for (auto const& group : groups) {
            group->limit_help_flags(min_size);
        }
        min_size = std::min(min_size + 4, detail::_argument_help_limit);
        if (!positional.empty() || sub_positional) {
            os << std::endl << "positional arguments:" << std::endl;
            for (std::size_t i = 0; i < positional.size(); ++i) {
                if (sub_positional && subparser.second == i) {
                    os << subparser.first->print(min_size) << std::endl;
                }
                os << positional.at(i)->print(false, m_argument_default, min_size) << std::endl;
            }
            if (sub_positional && subparser.second == positional.size()) {
                os << subparser.first->print(min_size) << std::endl;
                for (auto const& arg : subparser.first->m_parsers) {
                    os << arg.print(min_size) << std::endl;
                }
            }
        }
        if (!optional.empty()) {
            os << std::endl << "optional arguments:" << std::endl;
            for (auto it = std::begin(optional); it != std::end(optional); ++it) {
                os << (*it)->print(show_default && !(m_add_help && it == std::begin(optional)),
                                   m_argument_default, min_size) << std::endl;
            }
        }
        for (auto const& group : groups) {
            if (subparser.first) {
                if (group.get() != subparser.first || !sub_positional) {
                    group->print_help(os, show_default, m_argument_default, min_size);
                }
            } else if (group.get() != subparser.first) {
                group->print_help(os, show_default, m_argument_default, min_size);
            }
        }
        if (!epilog.empty()) {
            os << std::endl << epilog << std::endl;
        }
    }

    std::string m_prog;
    std::vector<ArgumentParser> m_parents;
    detail::Value<HelpFormatter> m_formatter_class;
    std::string m_fromfile_prefix_chars;
    detail::Value<std::string> m_argument_default;
    bool m_add_help;
    bool m_allow_abbrev;
    bool m_exit_on_error;

    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
    std::shared_ptr<Subparser> m_subparsers;
};
} // argparse

#pragma GCC diagnostic pop

#endif // _ARGPARSE_ARGUMENT_PARSER_HPP_
