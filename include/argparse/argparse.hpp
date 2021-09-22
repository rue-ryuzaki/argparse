/*
 * MIT License
 *
 * Argument parser for C++11 (ArgumentParser v0.5.2)
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

#define ARGPARSE_VERSION_MAJOR 0
#define ARGPARSE_VERSION_MINOR 5
#define ARGPARSE_VERSION_PATCH 2

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace argparse {
namespace detail {
std::size_t const _usage_limit = 80;
std::size_t const _argument_help_limit = 24;
std::string const _default_prefix_chars = "-";
std::string const _pseudo_argument = "--";

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
    return (s.size() > 1 && s.front() == s.back()
            && (s.front() == '\'' || s.front() == '\"'));
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

static inline std::vector<std::string> _split_equal(std::string const& s)
{
    std::size_t pos = s.find('=');
    if (pos != std::string::npos) {
        return { s.substr(0, pos), s.substr(pos + 1) };
    } else {
        return { s };
    }
}

static inline bool _starts_with(std::string const& s, std::string const& pattern)
{
    return s.compare(0, pattern.size(), pattern) == 0;
}

template <class T>
bool _is_value_exists(T const& value, std::vector<T> const& vec)
{
    return std::find(std::begin(vec), std::end(vec), value) != std::end(vec);
}

static inline bool _is_string_contains_char(std::string const& str, char c)
{
    return std::find(std::begin(str), std::end(str), c) != std::end(str);
}

static inline bool _is_optional_argument(std::string const& arg,
                                         std::string const& prefix_chars)
{
    return _is_string_contains_char(prefix_chars, arg.front());
}

static inline std::string _flag_name(std::string str)
{
    char prefix = str.front();
    str.erase(std::begin(str),
              std::find_if(std::begin(str), std::end(str),
                           [prefix] (char c) -> bool { return c != prefix; }));
    return str;
}

static inline std::vector<std::string> _help_flags(std::string const& prefix_chars)
{
    auto const& prefix
            = _is_string_contains_char(prefix_chars, _default_prefix_chars.front())
            ? _default_prefix_chars.front() : prefix_chars.front();
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

static inline std::string _vector_to_string(std::vector<std::string> const& vec,
                                            std::string const& separator = " ",
                                            std::string const& quotes = "",
                                            bool replace_space = false,
                                            std::string const& none = "")
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

    bool operator ==(Value const& rhs) const
    {
        return this->m_status == rhs.m_status && this->m_value == rhs.m_value;
    }

    void clear()
    {
        m_status = false;
        m_value = T();
    }

    bool        status() const { return m_status; }
    T const&    value()  const { return m_value; }
    T const& operator()()const { return m_value; }

    void set(T const& value)
    {
        m_status = true;
        m_value = value;
    }

    void set(T&& value)
    {
        m_status = true;
        m_value = value;
    }

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
    NONE,
    SUPPRESS,
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
    friend class ArgumentParser;

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
          m_help_type(NONE),
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
                m_default = "0";
                m_const = "1";
                m_nargs = NARGS_INT;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::store_false :
                m_default = "1";
                m_const = "0";
                m_nargs = NARGS_INT;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case Action::version :
                help("show program's version number and exit");
            case Action::help :
                if (type() == Positional) {
                    // version and help actions cannot be positional
                    throw TypeError("got an unexpected keyword argument 'required'");
                }
            case Action::store_const :
            case Action::append_const :
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
        m_nargs_str = param;
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
                || (type() == Optional && m_nargs == OPTIONAL
                    && (m_action & (Action::store | Action::append | Action::extend)))) {
            m_const = detail::_trim_copy(value);
        } else if (type() == Optional && m_nargs != OPTIONAL
                   && (m_action & (Action::store | Action::append | Action::extend))) {
            throw ValueError("nargs must be '?' to supply const");
        } else {
            throw TypeError("got an unexpected keyword argument 'const'");
        }
        return *this;
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
        if (!(m_action & (Action::store_true | Action::store_false))) {
            m_default = detail::_trim_copy(value);
        }
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
        m_choices.set(std::move(values));
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
        if (type() == Positional) {
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
        m_help_type = NONE;
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
        if (type() == Positional) {
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
            m_handle_str(str);
        }
    }

    void handle() const
    {
        if (m_handle) {
            m_handle();
        }
    }

    Type type() const
    {
        return m_type;
    }

    Enum help_type() const
    {
        return m_help_type;
    }

    uint32_t num_args() const
    {
        return m_num_args;
    }

    std::string usage() const
    {
        std::string res;
        if (type() == Optional) {
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
        if (type() == Optional) {
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

    std::string get_nargs_suffix() const
    {
        auto const name = get_argument_name();
        std::string res;
        if (type() == Optional && !name.empty()) {
            res += " ";
        }
        switch (m_nargs) {
            case OPTIONAL :
                res += "[" +  name + "]";
                break;
            case ZERO_OR_MORE :
                res += "[" +  name + " ...]";
                break;
            case ONE_OR_MORE :
                res += name + " [" +  name + " ...]";
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
        return type() == Optional ? detail::_to_upper(res) : res;
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
                return "";
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
        if (!dest().empty()) {
            return dest() == rhs;
        }
        for (auto const& flag : flags()) {
            if (flag == rhs) {
                return true;
            }
        }
        return false;
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
    Enum        m_help_type;
    detail::Value<std::string> m_metavar;
    std::string m_dest_str;
    std::vector<std::string> m_dest;
    detail::Value<std::string> m_version;
    std::function<void(std::string)> m_handle_str;
    std::function<void()> m_handle;
};

inline bool operator <(Argument const& lhs, Argument const& rhs)
{
    return lhs.flags() < rhs.flags();
}

/*!
 * \brief BaseParser class
 */
class BaseParser
{
public:
    /*!
     *  \brief Construct base parser
     *
     *  \return BaseParser object
     */
    BaseParser()
        : m_usage(),
          m_description(),
          m_epilog(),
          m_prefix_chars(detail::_default_prefix_chars),
          m_arguments(),
          m_optional(),
          m_positional()
    { }

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
    Argument& add_argument(char const* flag)
    {
        return add_argument({ std::string(flag) });
    }

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    Argument& add_argument(std::vector<std::string> flags)
    {
        if (flags.empty()) {
            throw ValueError("empty options");
        }
        flags.front() = detail::_trim_copy(flags.front());
        auto flag_name = flags.front();
        if (flag_name.empty()) {
            throw IndexError("string index out of range");
        }

        auto prefixes = 0ul;
        auto _update_flag_name = [&flag_name, &prefixes] (std::string const& arg)
        {
            auto name = detail::_flag_name(arg);
            auto count_prefixes = arg.size() - name.size();
            if (prefixes < count_prefixes) {
                prefixes = count_prefixes;
                flag_name = name;
            }
        };
        bool is_optional = detail::_is_optional_argument(flag_name, m_prefix_chars);
        if (is_optional) {
            _update_flag_name(flag_name);
        } else if (flags.size() > 1) {
            // no positional multiflag
            throw ValueError("invalid option string " + flags.front()
                             + ": must starts with a character '" + m_prefix_chars + "'");
        }
        for (std::size_t i = 1; i < flags.size(); ++i) {
            // check arguments
            flags.at(i) = detail::_trim_copy(flags.at(i));
            auto const& flag = flags.at(i);
            if (flag.empty()) {
                throw IndexError("string index out of range");
            }
            if (!detail::_is_optional_argument(flag, m_prefix_chars)) {
                // no positional and optional args
                throw ValueError("invalid option string " + flag
                                 + ": must starts with a character '" + m_prefix_chars + "'");
            }
            _update_flag_name(flag);
        }
        Argument arg(flags, flag_name, is_optional ? Argument::Optional : Argument::Positional);
        if (is_optional) {
            for (auto const& arg_flag : flags) {
                for (auto const& opt : m_optional) {
                    for (auto const& flag : opt.flags()) {
                        if (flag == arg_flag) {
                            throw ArgumentError("argument " + detail::_vector_to_string(flags, "/")
                                                + ": conflicting option string: " + flag);
                        }
                    }
                }
            }
        }
        m_arguments.emplace_back(std::move(arg));
        (is_optional ? m_optional : m_positional).push_back(m_arguments.back());
        return m_arguments.back();
    }

protected:
    std::string           m_usage;
    std::string           m_description;
    std::string           m_epilog;
    std::string           m_prefix_chars;
    std::vector<Argument> m_arguments;
    std::vector<Argument> m_optional;
    std::vector<Argument> m_positional;
};

/*!
 * \brief ArgumentParser objects
 */
class ArgumentParser : public BaseParser
{
    typedef std::vector<std::string> ArgumentValue;
    typedef std::pair<const Argument, ArgumentValue> ArgumentData;

    class Storage
    {
    public:
        Storage()
            : m_data()
        { }

        void create(Argument const& key,
                    std::vector<std::string> const& value = std::vector<std::string>())
        {
            auto const& flag = conflict_arg(key);
            if (flag.empty()) {
                m_data.insert({ key, value });
            } else {
                throw ArgumentError("argument " + detail::_vector_to_string(key.flags(), "/")
                                    + ": conflicting dest string: " + flag);
            }
        }

        void create(std::vector<Argument> const& arguments)
        {
            for (auto const& arg : arguments) {
                create(arg);
            }
        }

        void store_value(Argument const& arg, std::string const& value)
        {
            at(arg).push_back(value);
            arg.handle(value);
        }

        void store_default_value(Argument const& arg, std::string const& value)
        {
            if (arg.action() == Action::store) {
                auto& storage = at(arg);
                if (storage.empty()) {
                    storage.push_back(value);
                }
            }
        }

        bool self_value_stored(Argument const& arg)
        {
            if (arg.action() == Action::store_const) {
                auto& storage = at(arg);
                if (storage.empty()) {
                    storage.push_back(arg.const_value());
                }
                arg.handle(arg.const_value());
                return true;
            } else if (arg.action() & (Action::store_true | Action::store_false)) {
                auto& storage = at(arg);
                if (storage.empty()) {
                    storage.push_back(arg.const_value());
                }
                arg.handle();
                return true;
            } else if (arg.action() == Action::append_const) {
                at(arg).push_back(arg.const_value());
                arg.handle(arg.const_value());
                return true;
            } else if (arg.action() == Action::count) {
                at(arg).emplace_back(std::string());
                arg.handle();
                return true;
            }
            return false;
        }

        bool exists(std::string const& key) const
        {
            return std::find_if(std::begin(m_data), std::end(m_data),
                                [key] (ArgumentData const& pair) -> bool
            { return pair.first == key; }) != std::end(m_data);
        }

        bool exists(Argument const& key) const
        {
            return m_data.count(key) != 0;
        }

        ArgumentData const& at(std::string const& key) const
        {
            auto it = std::find_if(std::begin(m_data), std::end(m_data),
                                   [key] (ArgumentData const& pair) -> bool
            { return pair.first == key; });
            if (it == std::end(m_data)) {
                throw std::logic_error("key '" + key + "' not found");
            }
            return *it;
        }

        ArgumentValue& at(Argument const& key)
        {
            return m_data.at(key);
        }

        ArgumentValue const& at(Argument const& key) const
        {
            return m_data.at(key);
        }

        inline std::map<Argument, ArgumentValue>::iterator       begin()        { return std::begin(m_data); }
        inline std::map<Argument, ArgumentValue>::iterator       end()          { return std::end(m_data); }
        inline std::map<Argument, ArgumentValue>::const_iterator begin()  const { return std::begin(m_data); }
        inline std::map<Argument, ArgumentValue>::const_iterator end()    const { return std::end(m_data); }

    private:
        std::string const& conflict_arg(Argument const& arg) const
        {
            auto _get_argument_flags = [] (Argument const& arg) -> std::vector<std::string> const&
            {
                return arg.dest().empty() ? arg.flags() : arg.m_dest;
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

        std::map<Argument, ArgumentValue> m_data;
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
              m_help()
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
                m_prefix_chars = value;
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
         *  \brief Get parser 'help' message
         *
         *  \return Parser 'help' message
         */
        std::string const& help() const
        {
            return m_help;
        }

    private:
        std::string print(std::size_t limit = detail::_argument_help_limit) const
        {
            std::string res = "  " + m_name;
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
    };

    /*!
     * \brief Subparser class
     */
    class Subparser
    {
        friend class ArgumentParser;

    public:
        /*!
         *  \brief Construct subparser
         *
         *  \return Subparser object
         */
        Subparser()
            : m_title(),
              m_description(),
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
         *  \brief Get subparser 'title' value
         *
         *  \return Subparser 'title' value
         */
        std::string const& title() const
        {
            return m_title;
        }

        /*!
         *  \brief Get subparser 'description' value
         *
         *  \return Subparser 'description' value
         */
        std::string const& description() const
        {
            return m_description;
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

        std::string m_title;
        std::string m_description;
        std::string m_prog;
        std::string m_dest;
        bool        m_required;
        std::string m_help;
        detail::Value<std::string> m_metavar;
        std::vector<Parser> m_parsers;
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
        template <class T, std::size_t N> struct is_stl_array<std::array        <T, N> >   :std::true_type{};

        template <class T>       struct is_stl_queue:std::false_type{};
        template <class... Args> struct is_stl_queue<std::stack                 <Args...> >:std::true_type{};
        template <class... Args> struct is_stl_queue<std::queue                 <Args...> >:std::true_type{};

    public:
        /*!
         *  \brief Construct object with parsed arguments
         *
         *  \param arguments Parsed arguments
         *
         *  \return Object with parsed arguments
         */
        Namespace(Storage const& arguments)
            : m_arguments(arguments)
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
                if (pair.first.type() == Argument::Optional && pair.first.dest().empty()) {
                    for (auto const& flag : pair.first.flags()) {
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
            if (args.first.action() == Action::count) {
                return T(args.second.size());
            }
            if (args.second.empty()) {
                return T();
            }
            if (args.second.size() != 1) {
                throw TypeError("trying to get data from array argument '" + key + "'");
            }
            if (args.second.front().empty()) {
                return T();
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
            if (args.first.action() == Action::count) {
                throw TypeError("invalid get type for argument '" + key + "'");
            }
            if (args.second.empty()) {
                return T();
            }
            if (args.second.size() != 1) {
                throw TypeError("trying to get data from array argument '" + key + "'");
            }
            if (args.second.front().empty()) {
                return T();
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
                                          and not is_stl_queue<typename std::decay<T>::type>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto const& args = data(key);
            if (args.second.empty()) {
                return T();
            }
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
            switch (args.first.action()) {
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
                        return args.first.default_value() == "0" ? "false" : "true";
                    }
                    if (args.second.size() != 1) {
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return args.second.front() == "0" ? "false" : "true";
                case Action::count :
                    return std::to_string(args.second.size());
                case Action::store :
                case Action::append :
                case Action::append_const :
                case Action::extend :
                    return detail::_vector_to_string(args.second, " ", "", true);
                default :
                    throw ValueError("action not supported");
            }
        }

        /*!
         *  \brief Get parsed argument value as string
         *
         *  \param key Argument name
         *
         *  \return Parsed argument value as string
         */
        std::string to_string(std::string const& key) const
        {
            auto const& args = data(key);
            switch (args.first.action()) {
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
                        return args.first.default_value() == "0" ? "false" : "true";
                    }
                    if (args.second.size() != 1) {
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return args.second.front() == "0" ? "false" : "true";
                case Action::count :
                    return std::to_string(args.second.size());
                case Action::store :
                case Action::append :
                case Action::append_const :
                case Action::extend :
                    return "[" + detail::_vector_to_string(args.second, ", ", "'", false, "None") + "]";
                default :
                    throw ValueError("action not supported");
            }
        }

    private:
        ArgumentData const& data(std::string const& key) const
        {
            if (m_arguments.exists(key)) {
                return m_arguments.at(key);
            }
            for (auto const& pair : m_arguments) {
                if (pair.first.type() == Argument::Optional && pair.first.dest().empty()) {
                    for (auto const& flag : pair.first.flags()) {
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

        template <class T,
                  typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
        T to_type(std::string const& data) const
        {
            return detail::_remove_quotes(data);
        }

        template <class T,
                  typename std::enable_if<not std::is_constructible<std::string, T>::value>::type* = nullptr>
        T to_type(std::string const& data) const
        {
            T result = T();
            std::stringstream ss(data);
            ss >> result;
            if (ss.fail() || !ss.eof()) {
                throw TypeError("can't convert value '" + data + "'");
            }
            return result;
        }

        Namespace& operator =(Namespace const&) = delete;

        Storage m_arguments;
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
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_add_help(true),
          m_allow_abbrev(true),
          m_exit_on_error(true),
          m_default_values(),
          m_parsed_arguments(),
          m_subparsers(nullptr),
          m_subparser_pos()
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
        : ArgumentParser(argc, (char const**)(argv))
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
    ~ArgumentParser()
    {
        if (m_subparsers) {
            delete m_subparsers;
            m_subparsers = nullptr;
        }
    }

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
            m_prefix_chars = value;
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
            handle_error("cannot have multiple subparser arguments");
        }
        for (auto const& parent : m_parents) {
            if (parent.m_subparsers) {
                handle_error("cannot have multiple subparser arguments");
            }
        }
        m_subparser_pos = m_positional.size();
        m_subparsers = new Subparser();
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
        auto const positional = positional_arguments();
        auto const optional = optional_arguments();
        for (auto const& arg : positional) {
            if (detail::_is_value_exists(dest, arg.flags())) {
                return default_argument_value(arg)();
            }
        }
        for (auto const& arg : optional) {
            if (!arg.dest().empty()) {
                if (arg.dest() == dest) {
                    return default_argument_value(arg)();
                }
            } else {
                for (auto const& flag : arg.flags()) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        return default_argument_value(arg)();
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
            if (arg.type() == Argument::Positional) {
                if (detail::_is_value_exists(dest, arg.flags())) {
                    arg.default_value(value);
                    return true;
                }
            } else if (!arg.dest().empty()) {
                if (arg.dest() == dest) {
                    arg.default_value(value);
                    return true;
                }
            } else {
                for (auto const& flag : arg.flags()) {
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
                    if (_set_value(arg, dest, value)) {
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
                if (_set_value(arg, dest, value)) {
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
     *  \param parsed_arguments Arguments to parse
     *
     *  \return Object with parsed arguments
     */
    Namespace parse_args(std::vector<std::string> parsed_arguments) const
    {
        if (m_exit_on_error) {
            try {
                return parse_known_args(parsed_arguments);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            } catch (...) {
                std::cerr << "error: unexpected error" << std::endl;
            }
            exit(1);
        } else {
            return parse_known_args(parsed_arguments);
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
            auto const positional = positional_arguments(false);
            auto const optional = optional_arguments(false);
            auto const subparser = subpurser_info(false);
            print_custom_usage(positional, optional, subparser, prog(), os);
        }
    }

    /*!
     *  \brief Print program help message
     *
     *  \param os Output stream
     */
    void print_help(std::ostream& os = std::cout) const
    {
        auto const positional = positional_arguments(false);
        auto const optional = optional_arguments(false);
        auto const subparser = subpurser_info(false);
        print_custom_help(positional, optional, subparser, prog(), usage(), description(), epilog(), os);
    }

private:
    Namespace parse_known_args(std::vector<std::string> parsed_arguments) const
    {
        if (!m_fromfile_prefix_chars.empty()) {
            std::vector<std::string> temp;
            temp.reserve(parsed_arguments.size());
            for (auto const& arg : parsed_arguments) {
                if (detail::_is_string_contains_char(m_fromfile_prefix_chars, arg.front())) {
                    auto const load_args = convert_arg_line_to_args(arg.substr(1));
                    temp.insert(std::end(temp), std::begin(load_args), std::end(load_args));
                } else {
                    temp.push_back(arg);
                }
            }
            parsed_arguments = std::move(temp);
        }
        auto _validate_arguments = [] (std::vector<Argument> const& arguments)
        {
            for (auto const& arg : arguments) {
                if ((arg.action() & (Action::store_const | Action::append_const))
                        && !arg.m_const.status()) {
                    throw TypeError("missing 1 required positional argument: 'const'");
                }
                for (auto const& flag : arg.flags()) {
                    if (flag == detail::_pseudo_argument && arg.dest().empty()) {
                        throw ValueError("dest= is required for options like '--'");
                    }
                }
            }
        };
        auto _validate_argument_value = [this] (Argument const& arg, std::string const& value)
        {
            auto const& choices = arg.m_choices;
            if (choices.status()) {
                auto str = detail::_remove_quotes(value);
                if (!(str.empty() && choices().empty())
                        && !detail::_is_value_exists(str, choices())) {
                    auto values = detail::_vector_to_string(choices(), ", ", "'");
                    handle_error("argument " + arg.flags().front()
                                 + ": invalid choice: '" + str + "' (choose from " + values + ")");
                }
            }
        };
        auto _is_negative_numbers_presented = [] (std::vector<Argument> const& optionals,
                std::string const& prefix_chars)
        {
            if (!detail::_is_string_contains_char(prefix_chars, '-')) {
                return false;
            }
            for (auto const& arg : optionals) {
                for (auto const& flag : arg.flags()) {
                    if (detail::_is_negative_number(flag)) {
                        return true;
                    }
                }
            }
            return false;
        };

        auto positional = positional_arguments();
        auto const optional = optional_arguments();
        auto const subparser = subpurser_info();
        Parser const* capture_parser = nullptr;
        std::vector<Argument> subparser_optional;

        _validate_arguments(positional);
        _validate_arguments(optional);
        if (subparser.first) {
            for (auto const& parser : subparser.first->m_parsers) {
                _validate_arguments(parser.m_arguments);
            }
        }

        bool have_negative_options = _is_negative_numbers_presented(optional, m_prefix_chars);
        bool was_pseudo_argument = false;

        std::vector<std::string> subparser_flags;
        if (subparser.first && !subparser.first->dest().empty()) {
            subparser_flags.push_back(subparser.first->dest());
        }
        Argument subparser_arg(subparser_flags, subparser.first ? subparser.first->dest()
                                                                : "", Argument::Positional);

        Storage result;
        result.create(positional);
        result.create(optional);
        if (subparser.first) {
            auto const& dest = subparser.first->dest();
            if (!dest.empty()) {
                result.create(subparser_arg);
            }
            for (auto const& parser : subparser.first->m_parsers) {
                result.create(parser.m_arguments);
            }
        }

        auto _get_subparser_optional_arg_by_flag = [&subparser_optional] (std::string const& key) -> Argument const*
        {
            for (auto const& arg : subparser_optional) {
                if (detail::_is_value_exists(key, arg.flags())) {
                    return &arg;
                }
            }
            return nullptr;
        };
        auto _get_optional_arg_by_flag = [&optional] (std::string const& key) -> Argument const*
        {
            for (auto const& arg : optional) {
                if (detail::_is_value_exists(key, arg.flags())) {
                    return &arg;
                }
            }
            return nullptr;
        };
        auto _prefix_chars = [this, capture_parser] () -> std::string const&
        {
            return capture_parser ? capture_parser->m_prefix_chars : m_prefix_chars;
        };

        std::vector<std::string> unrecognized_args;

        std::size_t pos = 0;
        auto _store_value = [&] (Argument const& arg, std::string const& value)
        {
            _validate_argument_value(arg, value);
            result.store_value(arg, value);
        };
        auto _store_default_value = [&] (Argument const& arg)
        {
            auto const& value = default_argument_value(arg);
            if (value.status()) {
                result.store_default_value(arg, value());
            }
        };
        auto _is_positional_arg_stored = [&] (Argument const& arg) -> bool
        {
            if (arg.action() == Action::append_const && arg.m_default.status()) {
                handle_error(detail::_ignore_default(arg.flags().front(), arg.default_value()));
            }
            return result.self_value_stored(arg);
        };
        auto _try_capture_parser = [&] (Parser const*& capture_parser,
                std::deque<std::string>& arguments)
        {
            std::size_t finish = pos;
            std::size_t min_args = 0;
            std::size_t one_args = 0;
            bool more_args = false;
            bool capture_need = false;
            for ( ; finish < positional.size(); ++finish) {
                if (subparser.first && !capture_parser && finish == subparser.second) {
                    if (min_args + 1 > arguments.size()) {
                        break;
                    }
                    capture_need = true;
                    break;
                }
                auto const& arg = positional.at(finish);
                if (!(arg.action() & (Action::store | Action::append | Action::extend))) {
                    continue;
                }
                std::size_t min_amount = 0;
                switch (arg.m_nargs) {
                    case Argument::OPTIONAL :
                        ++one_args;
                        break;
                    case Argument::ONE_OR_MORE :
                        ++min_amount;
                    case Argument::ZERO_OR_MORE :
                        more_args = true;
                        break;
                    default :
                        min_amount += arg.num_args();
                        break;
                }
                if (min_args + min_amount > arguments.size()) {
                    break;
                }
                min_args += min_amount;
            }
            if (subparser.first && !capture_parser && finish == positional.size()
                    && min_args < arguments.size()) {
                capture_need = true;
            }
            if (!capture_need) {
                return;
            }
            ++min_args;
            if (min_args == arguments.size()) {
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    switch (arg.m_nargs) {
                        case Argument::NARGS_DEF :
                        case Argument::ONE_OR_MORE :
                            _store_value(arg, arguments.front());
                            arguments.pop_front();
                            break;
                        case Argument::OPTIONAL :
                        case Argument::ZERO_OR_MORE :
                            _store_default_value(arg);
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg.num_args(); ++n) {
                                _store_value(arg, arguments.front());
                                arguments.pop_front();
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
                    switch (arg.m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_value(arg, arguments.front());
                            arguments.pop_front();
                            break;
                        case Argument::ONE_OR_MORE :
                            _store_value(arg, arguments.front());
                            arguments.pop_front();
                            while (over_args > 0) {
                                _store_value(arg, arguments.front());
                                arguments.pop_front();
                                --over_args;
                            }
                            break;
                        case Argument::OPTIONAL :
                            _store_default_value(arg);
                            break;
                        case Argument::ZERO_OR_MORE :
                            if (over_args > 0) {
                                while (over_args > 0) {
                                    _store_value(arg, arguments.front());
                                    arguments.pop_front();
                                    --over_args;
                                }
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg.num_args(); ++n) {
                                _store_value(arg, arguments.front());
                                arguments.pop_front();
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
                    switch (arg.m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_value(arg, arguments.front());
                            arguments.pop_front();
                            break;
                        case Argument::OPTIONAL :
                            if (over_args < one_args) {
                                _store_value(arg, arguments.front());
                                arguments.pop_front();
                                ++over_args;
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg.num_args(); ++n) {
                                _store_value(arg, arguments.front());
                                arguments.pop_front();
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
                    if (arg.m_nargs == Argument::NARGS_DEF) {
                        _store_value(arg, arguments.front());
                        arguments.pop_front();
                    } else {
                        std::size_t num_args = (arg.m_nargs == Argument::OPTIONAL ? 1 : arg.num_args());
                        for (std::size_t n = 0; n < num_args; ++n) {
                            _store_value(arg, arguments.front());
                            arguments.pop_front();
                        }
                    }
                }
            }
            auto const& name = arguments.front();
            std::string choices;
            for (auto const& parser : subparser.first->m_parsers) {
                if (!choices.empty()) {
                    choices += ", ";
                }
                choices += "'" + parser.m_name + "'";
                if (parser.m_name == name) {
                    capture_parser = &parser;
                    if (!subparser.first->dest().empty()) {
                        result.at(subparser_arg).push_back(name);
                    }
                    break;
                }
            }
            if (capture_parser) {
                if (m_add_help) {
                    subparser_optional.emplace_back(Argument(detail::_help_flags(capture_parser->m_prefix_chars), "help", Argument::Optional)
                                                    .help("show this help message and exit").action(Action::help));
                }
                std::vector<Argument> pos;
                for (auto const& arg : capture_parser->m_arguments) {
                    (arg.type() == Argument::Positional ? pos : subparser_optional).push_back(arg);
                }
                positional.insert(std::begin(positional) + subparser.second,
                                  std::begin(pos), std::end(pos));
                arguments.pop_front();
                have_negative_options
                        = _is_negative_numbers_presented(capture_parser->m_optional,
                                                         capture_parser->m_prefix_chars);
            } else {
                handle_error("invalid choice: '" + name + "' (choose from " + choices + ")");
            }
        };
        auto _match_args_partial = [&] (std::deque<std::string> const& arguments)
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
                if (!(arg.action() & (Action::store | Action::append | Action::extend))) {
                    continue;
                }
                std::size_t min_amount = 0;
                switch (arg.m_nargs) {
                    case Argument::OPTIONAL :
                        ++one_args;
                        break;
                    case Argument::ONE_OR_MORE :
                        ++min_amount;
                    case Argument::ZERO_OR_MORE :
                        more_args = true;
                        break;
                    default :
                        min_amount += arg.num_args();
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
            } else if (min_args == arguments.size()) {
                std::size_t i = 0;
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    switch (arg.m_nargs) {
                        case Argument::NARGS_DEF :
                        case Argument::ONE_OR_MORE :
                            _store_value(arg, arguments.at(i++));
                            break;
                        case Argument::OPTIONAL :
                        case Argument::ZERO_OR_MORE :
                            _store_default_value(arg);
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg.num_args(); ++n) {
                                _store_value(arg, arguments.at(i++));
                            }
                            break;
                        default :
                            break;
                    }
                }
            } else if (more_args) {
                std::size_t over_args = arguments.size() - min_args;
                std::size_t i = 0;
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    switch (arg.m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_value(arg, arguments.at(i++));
                            break;
                        case Argument::ONE_OR_MORE :
                            _store_value(arg, arguments.at(i++));
                            while (over_args > 0) {
                                _store_value(arg, arguments.at(i++));
                                --over_args;
                            }
                            break;
                        case Argument::OPTIONAL :
                            _store_default_value(arg);
                            break;
                        case Argument::ZERO_OR_MORE :
                            if (over_args > 0) {
                                while (over_args > 0) {
                                    _store_value(arg, arguments.at(i++));
                                    --over_args;
                                }
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        default :
                            for (std::size_t n = 0; n < arg.num_args(); ++n) {
                                _store_value(arg, arguments.at(i++));
                            }
                            break;
                    }
                }
            } else if (min_args + one_args >= arguments.size()) {
                std::size_t over_args = min_args + one_args - arguments.size();
                std::size_t i = 0;
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    switch (arg.m_nargs) {
                        case Argument::NARGS_DEF :
                            _store_value(arg, arguments.at(i++));
                            break;
                        case Argument::OPTIONAL :
                            if (over_args < one_args) {
                                _store_value(arg, arguments.at(i++));
                                ++over_args;
                            } else {
                                _store_default_value(arg);
                            }
                            break;
                        case Argument::NARGS_INT :
                            for (std::size_t n = 0; n < arg.num_args(); ++n) {
                                _store_value(arg, arguments.at(i++));
                            }
                            break;
                        default :
                            break;
                    }
                }
            } else {
                std::size_t i = 0;
                for ( ; pos < finish; ++pos) {
                    auto const& arg = positional.at(pos);
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg.m_nargs == Argument::NARGS_DEF) {
                        _store_value(arg, arguments.at(i++));
                    } else {
                        std::size_t num_args = (arg.m_nargs == Argument::OPTIONAL ? 1 : arg.num_args());
                        for (std::size_t n = 0; n < num_args; ++n) {
                            _store_value(arg, arguments.at(i++));
                        }
                    }
                }
                for ( ; i < arguments.size(); ++i) {
                    unrecognized_args.push_back(arguments.at(i));
                }
            }
        };
        auto _separate_arg_abbrev = [_get_optional_arg_by_flag, &optional]
                (std::vector<std::string>& temp, std::string const& arg, std::string const& name)
        {
            if (name.size() + 1 == arg.size()) {
                auto const splitted = detail::_split_equal(arg);
                if (splitted.size() == 2
                        && _get_optional_arg_by_flag(splitted.front())) {
                    temp.push_back(arg);
                    return;
                }
                std::vector<std::string> flags;
                for (std::size_t i = 0; i < name.size(); ++i) {
                    if (name.at(i) == '=') {
                        if (flags.empty()) {
                            flags.push_back(name.substr(i));
                        } else {
                            flags.back() += name.substr(i);
                        }
                        break;
                    }
                    Argument const* argument = nullptr;
                    for (auto const& opt : optional) {
                        for (auto const& flag : opt.flags()) {
                            if (flag.size() == 2 && flag.back() == name.at(i)) {
                                flags.push_back(flag);
                                argument = &opt;
                                break;
                            }
                        }
                    }
                    if (flags.size() == i) {
                        // not found
                        if (flags.empty()) {
                            flags.push_back(arg);
                        } else {
                            auto str = name.substr(i);
                            if (!str.empty()) {
                                if (!detail::_starts_with(str, "=")) {
                                    flags.back() += "=";
                                }
                                flags.back() += str;
                            }
                        }
                        break;
                    } else if (argument->action() & (Action::store | Action::append | Action::extend)) {
                        auto str = name.substr(i + 1);
                        if (!str.empty()) {
                            if (!detail::_starts_with(str, "=")) {
                                flags.back() += "=";
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
        auto _check_abbreviations = [this, _separate_arg_abbrev, &result,
                _prefix_chars, &have_negative_options, &was_pseudo_argument]
                (std::vector<std::string>& arguments, size_t i, std::vector<Argument> const& optionals)
        {
            auto& arg = arguments.at(i);
            if (!arg.empty() && detail::_is_optional_argument(arg, _prefix_chars())
                    && !result.exists(arg) && !was_pseudo_argument
                    && (have_negative_options || !detail::_is_negative_number(arg))) {
                std::vector<std::string> temp;
                if (m_allow_abbrev) {
                    bool is_flag_added = false;
                    std::string args;
                    std::vector<std::string> keys;
                    for (auto const& opt : optionals) {
                        for (auto const& flag : opt.flags()) {
                            if (detail::_starts_with(flag, arg)) {
                                is_flag_added = true;
                                keys.push_back(flag);
                                if (!args.empty()) {
                                    args += ",";
                                }
                                args += " " + flag;
                                break;
                            }
                            if (flag.size() == 2
                                    && detail::_starts_with(arg, flag)) {
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
                        handle_error("ambiguous option: '" + arg + "' could match" + args);
                    }
                    if (is_flag_added) {
                        temp.push_back(keys.empty() ? arg : keys.front());
                    } else {
                        auto name = detail::_flag_name(keys.empty() ? arg : keys.front());
                        _separate_arg_abbrev(temp, arg, name);
                    }
                } else {
                    auto name = detail::_flag_name(arg);
                    _separate_arg_abbrev(temp, arg, name);
                }
                arguments.erase(std::begin(arguments) + i);
                arguments.insert(std::begin(arguments) + i, std::begin(temp), std::end(temp));
            }
        };

        auto _print_help_and_exit = [this, subparser, &capture_parser] ()
        {
            if (capture_parser) {
                std::vector<Argument> pos;
                std::vector<Argument> opt;
                if (m_add_help) {
                    opt.emplace_back(Argument(detail::_help_flags(capture_parser->m_prefix_chars), "help", Argument::Optional)
                                     .help("show this help message and exit").action(Action::help));
                }
                for (auto const& arg : capture_parser->m_arguments) {
                    (arg.type() == Argument::Optional ? opt : pos).push_back(arg);
                }
                auto program = subparser.first->prog();
                if (program.empty()) {
                    program = prog();
                }
                auto const positional_args = positional_arguments(false);
                for (std::size_t i = 0; i < positional_args.size(); ++i) {
                    if (subparser.first && subparser.second == i) {
                        break;
                    }
                    auto const str = positional_args.at(i).usage();
                    if (str.empty()) {
                        continue;
                    }
                    program += " " + str;
                }
                program += " " + capture_parser->m_name;
                print_custom_help(pos, opt, { nullptr, 0 }, program, capture_parser->usage(),
                                  capture_parser->description(), capture_parser->epilog());
            } else {
                print_help();
            }
            exit(0);
        };
        for (std::size_t i = 0; i < parsed_arguments.size(); ++i) {
            if (parsed_arguments.at(i) == detail::_pseudo_argument && !was_pseudo_argument) {
                was_pseudo_argument = true;
                continue;
            }
            _check_abbreviations(parsed_arguments, i, capture_parser ? subparser_optional : optional);
            auto arg = parsed_arguments.at(i);
            auto const splitted = detail::_split_equal(arg);
            if (splitted.size() == 2) {
                arg = splitted.front();
            }
            auto const* temp = capture_parser ? _get_subparser_optional_arg_by_flag(arg)
                                              : _get_optional_arg_by_flag(arg);
            if (was_pseudo_argument) {
                temp = nullptr;
            }
            if (temp) {
                switch (temp->action()) {
                    case Action::store :
                        result.at(*temp).clear();
                    case Action::append :
                    case Action::extend :
                        if (splitted.size() == 2) {
                            if (temp->m_nargs != Argument::NARGS_DEF && temp->num_args() > 1) {
                                handle_error(temp->error_nargs(arg));
                            }
                            _store_value(*temp, splitted.back());
                        } else {
                            uint32_t n = 0;
                            uint32_t num_args = temp->num_args();
                            while (true) {
                                ++i;
                                if (i == parsed_arguments.size()) {
                                    if (n == 0) {
                                        switch (temp->m_nargs) {
                                            case Argument::NARGS_DEF :
                                            case Argument::NARGS_INT :
                                            case Argument::ONE_OR_MORE :
                                                handle_error(temp->error_nargs(arg));
                                                break;
                                            case Argument::OPTIONAL :
                                                _store_value(*temp, temp->const_value());
                                                break;
                                            default :
                                                break;
                                        }
                                    } else if (temp->m_nargs == Argument::NARGS_INT && n < num_args) {
                                        handle_error(temp->error_nargs(arg));
                                    }
                                    break;
                                } else {
                                    auto const& next = parsed_arguments.at(i);
                                    if (!detail::_is_optional_argument(next, _prefix_chars())
                                            || (!have_negative_options
                                                && detail::_is_negative_number(next))) {
                                        _store_value(*temp, next);
                                        ++n;
                                    } else if (n == 0) {
                                        --i;
                                        switch (temp->m_nargs) {
                                            case Argument::NARGS_DEF :
                                            case Argument::NARGS_INT :
                                            case Argument::ONE_OR_MORE :
                                                handle_error(temp->error_nargs(arg));
                                                break;
                                            case Argument::OPTIONAL :
                                                _store_value(*temp, temp->const_value());
                                                break;
                                            default :
                                                break;
                                        }
                                        break;
                                    } else {
                                        if (temp->m_nargs == Argument::NARGS_INT && n < num_args) {
                                            handle_error(temp->error_nargs(arg));
                                        }
                                        --i;
                                        break;
                                    }
                                }
                                if (temp->m_nargs == Argument::NARGS_DEF
                                        || temp->m_nargs == Argument::OPTIONAL
                                        || (temp->m_nargs == Argument::NARGS_INT && n == num_args)) {
                                    break;
                                }
                            }
                        }
                        break;
                    case Action::store_const :
                    case Action::store_true :
                    case Action::store_false :
                    case Action::append_const :
                    case Action::count :
                        if (splitted.size() == 1) {
                            if (temp->action() == Action::append_const && !temp->default_value().empty()) {
                                handle_error(detail::_ignore_default(temp->flags().front(), temp->default_value()));
                            }
                            result.self_value_stored(*temp);
                        } else {
                            handle_error(detail::_ignore_explicit(arg, splitted.back()));
                        }
                        break;
                    case Action::help :
                        if (splitted.size() == 1) {
                            _print_help_and_exit();
                        } else {
                            handle_error(detail::_ignore_explicit(arg, splitted.back()));
                        }
                        break;
                    case Action::version :
                        if (splitted.size() == 1) {
                            if (!temp->m_version.status()) {
                                throw AttributeError("'ArgumentParser' object has no attribute 'version'");
                            }
                            std::cout << temp->version() << std::endl;
                            exit(0);
                        } else {
                            handle_error(detail::_ignore_explicit(arg, splitted.back()));
                        }
                        break;
                    default :
                        handle_error("action not supported");
                        break;
                }
            } else if (!was_pseudo_argument &&
                       ((have_negative_options && detail::_is_negative_number(arg))
                        || (detail::_is_optional_argument(arg, _prefix_chars())
                            && !detail::_is_negative_number(arg)))) {
                unrecognized_args.push_back(arg);
            } else {
                std::deque<std::string> values;
                values.push_back(parsed_arguments.at(i));
                while (true) {
                    ++i;
                    if (i == parsed_arguments.size()) {
                        break;
                    } else {
                        auto const& next = parsed_arguments.at(i);
                        if (!detail::_is_optional_argument(next, _prefix_chars())
                                || was_pseudo_argument
                                || (!have_negative_options
                                    && detail::_is_negative_number(next))) {
                            values.push_back(next);
                        } else {
                            --i;
                            break;
                        }
                    }
                }
                if (subparser.first && !capture_parser) {
                    _try_capture_parser(capture_parser, values);
                    if (capture_parser) {
                        if (i == parsed_arguments.size()) {
                            --i;
                        }
                        i -= values.size();
                    } else {
                        _match_args_partial(values);
                    }
                } else {
                    _match_args_partial(values);
                }
            }
        }
        std::vector<std::string> required_args;
        for (auto const& arg : optional) {
            if (arg.required() && result.at(arg).empty()) {
                auto args = detail::_vector_to_string(arg.flags(), "/");
                required_args.emplace_back(args);
            }
        }
        bool subparser_required = subparser.first && !capture_parser && subparser.first->required();
        if (!required_args.empty() || pos < positional.size() || subparser_required) {
            std::string args;
            for ( ; pos < positional.size(); ++pos) {
                if (subparser_required && pos == subparser.second) {
                    auto const& dest = subparser.first->dest();
                    if (dest.empty()) {
                        throw TypeError("sequence item 0: expected str instance, NoneType found");
                    }
                    if (!args.empty()) {
                        args += ", ";
                    }
                    args += dest;
                }
                auto const& arg = positional.at(pos);
                if (args.empty()) {
                    if (_is_positional_arg_stored(arg)) {
                        continue;
                    }
                    if (arg.m_nargs == Argument::OPTIONAL || arg.m_nargs == Argument::ZERO_OR_MORE) {
                        _store_default_value(arg);
                        continue;
                    }
                }
                if (!args.empty()) {
                    args += ", ";
                }
                args += arg.flags().front();
            }
            if (subparser_required && pos == subparser.second) {
                auto const& dest = subparser.first->dest();
                if (dest.empty()) {
                    throw TypeError("sequence item 0: expected str instance, NoneType found");
                }
                if (!args.empty()) {
                    args += ", ";
                }
                args += dest;
            }
            if (!required_args.empty()) {
                args += ", ";
            }
            args += detail::_vector_to_string(required_args, ", ");
            if (!args.empty()) {
                handle_error("the following arguments are required: " + args);
            }
        }
        if (!unrecognized_args.empty()) {
            auto args = detail::_vector_to_string(unrecognized_args);
            handle_error("unrecognized arguments: " + args);
        }
        for (auto& arg : result) {
            if (arg.second.empty() && arg.first.action() != Action::count
                    && arg.first.type() == Argument::Optional) {
                auto const& value = default_argument_value(arg.first);
                if (value.status()) {
                    arg.second.push_back(value());
                }
            }
        }
        for (auto const& pair : m_default_values) {
            if (!result.exists(pair.first)) {
                auto arg = Argument({ pair.first }, pair.first, Argument::Positional);
                arg.default_value(pair.second);
                result.create(arg, { pair.second });
            }
        }
        return Namespace(result);
    }

    void handle_error(std::string const& error = "unknown") const
    {
        print_usage(std::cerr);
        throw std::logic_error(m_prog + ": error: " + error);
    }

    detail::Value<std::string> const& default_argument_value(Argument const& arg) const
    {
        return arg.m_default.status() ? arg.m_default : m_argument_default;
    }

    std::vector<std::string> convert_arg_line_to_args(std::string const& file) const
    {
        std::ifstream is(file);
        if (!is.is_open()) {
            handle_error("[Errno 2] No such file or directory: '" + file + "'");
        }
        std::vector<std::string> res;
        std::string line;
        while (std::getline(is, line)) {
            res.push_back(line);
        }
        is.close();
        return res;
    }

    std::vector<Argument> positional_arguments(bool add_suppress = true) const
    {
        std::vector<Argument> result;
        for (auto const& parent : m_parents) {
            auto const args = parent.positional_arguments(add_suppress);
            result.insert(std::end(result), std::begin(args), std::end(args));
        }
        for (auto const& arg : m_arguments) {
            if (arg.type() == Argument::Positional
                    && (add_suppress || arg.help_type() != SUPPRESS)) {
                result.push_back(arg);
            }
        }
        return result;
    }

    std::vector<Argument> optional_arguments(bool add_suppress = true) const
    {
        std::vector<Argument> result;
        if (m_add_help) {
            result.emplace_back(Argument(detail::_help_flags(m_prefix_chars), "help", Argument::Optional)
                                .help("show this help message and exit").action(Action::help));
        }
        for (auto const& parent : m_parents) {
            auto const args = parent.optional_arguments(add_suppress);
            result.insert(std::end(result), std::begin(args), std::end(args));
        }
        for (auto const& arg : m_arguments) {
            if (arg.type() == Argument::Optional
                    && (add_suppress || arg.help_type() != SUPPRESS)) {
                result.push_back(arg);
            }
        }
        return result;
    }

    std::pair<Subparser*, std::size_t> subpurser_info(bool add_suppress = true) const
    {
        std::pair<Subparser*, std::size_t> res = { nullptr, 0 };
        if (m_subparsers) {
            res.first = m_subparsers;
            for (auto const& parent : m_parents) {
                res.second += parent.positional_arguments(add_suppress).size();
            }
            for (std::size_t p = 0, a = 0; p < m_subparser_pos && a < m_arguments.size(); ++a) {
                auto const& arg = m_arguments.at(a);
                if (arg.type() == Argument::Positional) {
                    ++p;
                    res.second += (add_suppress || arg.help_type() != SUPPRESS);
                }
            }
        } else {
            for (std::size_t i = 0; i < m_parents.size(); ++i) {
                auto const& parent = m_parents.at(i);
                if (parent.m_subparsers) {
                    res.first = m_subparsers;
                    for (std::size_t j = 0; j < i; ++j) {
                        res.second += m_parents.at(j).positional_arguments(add_suppress).size();
                    }
                    for (std::size_t p = 0, a = 0; p < parent.m_subparser_pos && a < parent.m_arguments.size(); ++a) {
                        auto const& arg = parent.m_arguments.at(a);
                        if (arg.type() == Argument::Positional) {
                            ++p;
                            res.second += (add_suppress || arg.help_type() != SUPPRESS);
                        }
                    }
                    break;
                }
            }
        }
        return res;
    }

    static std::string custom_usage(std::vector<Argument> const& positional,
                                    std::vector<Argument> const& optional,
                                    std::pair<Subparser*, std::size_t> const& subparser,
                                    std::string const& program)
    {
        auto res = program;
        std::size_t min_size = 0;
        if (subparser.first) {
            auto const str = subparser.first->usage();
            if (min_size < str.size()) {
                min_size = str.size();
            }
        }
        for (auto const& arg : positional) {
            auto const str = arg.usage();
            if (min_size < str.size()) {
                min_size = str.size();
            }
        }
        for (auto const& arg : optional) {
            auto const str = arg.usage();
            if (min_size < str.size()) {
                min_size = str.size();
            }
        }
        std::size_t const usage_length = std::string("usage: ").size();
        std::size_t pos = usage_length + program.size();
        std::size_t offset = usage_length;
        if (pos + (min_size > 0 ? (1 + min_size) : 0) <= detail::_usage_limit) {
            offset += program.size() + (min_size > 0);
        } else if (!(optional.empty() && positional.empty() && !subparser.first)) {
            res += "\n" + std::string(offset - 1, ' ');
            pos = offset - 1;
        }
        auto _write_arg_usage = [&pos, &offset, &res] (std::string const& str, bool bkt)
        {
            if ((pos + 1 == offset) || (pos + 1 + str.size() <= detail::_usage_limit)) {
                res += " " + (bkt ? "[" + str + "]" : str);
            } else {
                res += "\n" + std::string(offset, ' ') + (bkt ? "[" + str + "]" : str);
                pos = offset;
            }
            pos += 1 + str.size();
        };
        for (auto const& arg : optional) {
            _write_arg_usage(arg.usage(), true);
        }
        for (std::size_t i = 0; i < positional.size(); ++i) {
            if (subparser.first && subparser.second == i) {
                auto const str = subparser.first->usage();
                _write_arg_usage(str, false);
            }
            auto const str = positional.at(i).usage();
            if (str.empty()) {
                continue;
            }
            _write_arg_usage(str, false);
        }
        if (subparser.first && subparser.second == positional.size()) {
            auto const str = subparser.first->usage();
            _write_arg_usage(str, false);
        }
        return res;
    }

    static void print_custom_usage(std::vector<Argument> const& positional,
                                   std::vector<Argument> const& optional,
                                   std::pair<Subparser*, std::size_t> const& subparser,
                                   std::string const& program,
                                   std::ostream& os = std::cout)
    {
        os << "usage: " << custom_usage(positional, optional, subparser, program) << std::endl;
    }

    static void print_custom_help(std::vector<Argument> const& positional,
                                  std::vector<Argument> const& optional,
                                  std::pair<Subparser*, std::size_t> const& subparser,
                                  std::string const& program,
                                  std::string const& usage,
                                  std::string const& description,
                                  std::string const& epilog,
                                  std::ostream& os = std::cout)
    {
        if (!usage.empty()) {
            os << "usage: " << usage << std::endl;
        } else {
            print_custom_usage(positional, optional, subparser, program, os);
        }
        if (!description.empty()) {
            os << std::endl << description << std::endl;
        }
        std::size_t min_size = 0;
        bool subparser_positional = (subparser.first
                                     && subparser.first->title().empty()
                                     && subparser.first->description().empty());
        if (subparser.first) {
            auto size = subparser.first->flags_to_string().size();
            if (min_size < size) {
                min_size = size;
            }
            for (auto const& arg : subparser.first->m_parsers) {
                auto size = arg.m_name.size();
                if (min_size < size) {
                    min_size = size;
                }
            }
        }
        for (auto const& arg : positional) {
            auto size = arg.flags_to_string().size();
            if (min_size < size) {
                min_size = size;
            }
        }
        for (auto const& arg : optional) {
            auto size = arg.flags_to_string().size();
            if (min_size < size) {
                min_size = size;
            }
        }
        min_size += 4;
        if (min_size > detail::_argument_help_limit) {
            min_size = detail::_argument_help_limit;
        }
        if (!positional.empty() || subparser_positional) {
            os << std::endl << "positional arguments:" << std::endl;
            for (std::size_t i = 0; i < positional.size(); ++i) {
                if (subparser_positional && subparser.second == i) {
                    os << subparser.first->print(min_size) << std::endl;
                }
                os << positional.at(i).print(min_size) << std::endl;
            }
            if (subparser_positional && subparser.second == positional.size()) {
                os << subparser.first->print(min_size) << std::endl;
                for (auto const& arg : subparser.first->m_parsers) {
                    os << arg.print(min_size) << std::endl;
                }
            }
        }
        if (!optional.empty()) {
            os << std::endl << "optional arguments:" << std::endl;
            for (auto const& arg : optional) {
                os << arg.print(min_size) << std::endl;
            }
        }
        if (subparser.first && !subparser_positional) {
            os << std::endl
               << (subparser.first->title().empty() ? "subcommands"
                                                    : subparser.first->title())
               << ":" << std::endl;
            if (!subparser.first->description().empty()) {
                os << "  " << subparser.first->description() << std::endl << std::endl;
            }
            os << subparser.first->print(min_size) << std::endl;
            for (auto const& arg : subparser.first->m_parsers) {
                os << arg.print(min_size) << std::endl;
            }
        }
        if (!epilog.empty()) {
            os << std::endl << epilog << std::endl;
        }
    }

    std::string m_prog;
    std::vector<ArgumentParser> m_parents;
    std::string m_fromfile_prefix_chars;
    detail::Value<std::string> m_argument_default;
    bool m_add_help;
    bool m_allow_abbrev;
    bool m_exit_on_error;

    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
    Subparser*  m_subparsers;
    std::size_t m_subparser_pos;
};
} // argparse

#endif // _ARGPARSE_ARGUMENT_PARSER_HPP_
