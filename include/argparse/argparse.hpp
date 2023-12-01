/*
 * MIT License
 *
 * Argument parser for C++11 (ArgumentParser v0.2.5)
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
#define ARGPARSE_VERSION_MINOR 2
#define ARGPARSE_VERSION_PATCH 5
#define ARGPARSE_VERSION_RC 0

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
size_t const _usage_limit = 80;
size_t const _argument_help_limit = 24;

static inline void _ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not1(std::ptr_fun<int, int>(isspace))));
}

static inline void _rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
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
    std::transform(s.begin(), s.end(), s.begin(), tolower);
    return s;
}

static inline std::string _to_upper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), toupper);
    return s;
}

static inline std::string _file_name(std::string const& s)
{
    return s.substr(s.find_last_of("/\\") + 1);
}

static inline std::string _remove_quotes(std::string const& s)
{
    if (s.size() > 1 && s.front() == s.back()
            && (s.front() == '\'' || s.front() == '\"')) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

static inline std::vector<std::string> _split_equal(std::string const& s)
{
    size_t pos = s.find('=');
    if (pos != std::string::npos) {
        return { s.substr(0, pos), s.substr(pos + 1) };
    } else {
        return { s };
    }
}

static inline bool _starts_with(std::string const& str, std::string const& pattern)
{
    return str.compare(0, pattern.size(), pattern) == 0;
}

static inline bool _is_optional_argument(std::string const& arg, std::string const& prefix_chars)
{
    return std::find(std::begin(prefix_chars), std::end(prefix_chars), arg.front()) != std::end(prefix_chars);
}

static inline std::string _flag_name(std::string str)
{
    char prefix = str.front();
    str.erase(std::begin(str),
              std::find_if(std::begin(str), std::end(str),
                           [prefix] (char c) -> bool { return c != prefix; }));
    return str;
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

static inline std::string _vector_string_to_string(std::vector<std::string> const& vec,
                                                   std::string const& separator = " ",
                                                   std::string const& quotes = "")
{
    std::string res;
    for (auto const& el : vec) {
        if (!res.empty()) {
            res += separator;
        }
        res += quotes + el + quotes;
    }
    return res;
}
} // details

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

enum Enum
{
    NONE,
    SUPPRESS,
};

class ArgumentError : public std::invalid_argument
{
public:
    explicit ArgumentError(std::string const& error)
        : std::invalid_argument("argparse.ArgumentError: " + error)
    { }
};

class AttributeError : public std::invalid_argument
{
public:
    explicit AttributeError(std::string const& error)
        : std::invalid_argument("AttributeError: " + error)
    { }
};

class ValueError : public std::invalid_argument
{
public:
    explicit ValueError(std::string const& error)
        : std::invalid_argument("ValueError: " + error)
    { }
};

class IndexError : public std::logic_error
{
public:
    explicit IndexError(std::string const& error)
        : std::logic_error("IndexError: " + error)
    { }
};

class TypeError : public std::logic_error
{
public:
    explicit TypeError(std::string const& error)
        : std::logic_error("TypeError: " + error)
    { }
};

// ArgumentParser objects
class ArgumentParser
{
    typedef std::pair<Action, std::vector<std::string> > ArgumentValue;

public:
    class Argument
    {
    public:
        enum Type
        {
            Positional,
            Optional
        };

        Argument(std::vector<std::string> const& flags,
                 std::string const& name,
                 Type type)
            : m_flags(flags),
              m_name(name),
              m_type(type),
              m_action(Action::store),
              m_nargs(),
              m_const(),
              m_default(),
              m_choices(),
              m_required(false),
              m_help(),
              m_help_type(NONE),
              m_metavar(),
              m_dest(),
              m_version(),
              m_callback(nullptr)
        { }

        Argument(Argument const& orig)
            : m_flags(orig.m_flags),
              m_name(orig.m_name),
              m_type(orig.m_type),
              m_action(orig.m_action),
              m_nargs(orig.m_nargs),
              m_const(orig.m_const),
              m_default(orig.m_default),
              m_choices(orig.m_choices),
              m_required(orig.m_required),
              m_help(orig.m_help),
              m_help_type(orig.m_help_type),
              m_metavar(orig.m_metavar),
              m_dest(orig.m_dest),
              m_version(orig.m_version),
              m_callback(orig.m_callback)
        { }

        Argument& operator =(Argument const& rhs)
        {
            if (this != &rhs) {
                this->m_flags   = rhs.m_flags;
                this->m_name    = rhs.m_name;
                this->m_type    = rhs.m_type;
                this->m_action  = rhs.m_action;
                this->m_nargs   = rhs.m_nargs;
                this->m_const   = rhs.m_const;
                this->m_default = rhs.m_default;
                this->m_choices = rhs.m_choices;
                this->m_required= rhs.m_required;
                this->m_help    = rhs.m_help;
                this->m_help_type= rhs.m_help_type;
                this->m_metavar = rhs.m_metavar;
                this->m_dest    = rhs.m_dest;
                this->m_version = rhs.m_version;
                this->m_callback= rhs.m_callback;
            }
            return *this;
        }

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

        Argument& action(Action value)
        {
            if (m_action == Action::version) {
                m_help.clear();
            }
            m_action = value;
            switch (m_action) {
                case Action::store_true :
                    m_default = "0";
                    m_const = "1";
                    m_nargs = "0";
                    m_choices.clear();
                    break;
                case Action::store_false :
                    m_default = "1";
                    m_const = "0";
                    m_nargs = "0";
                    m_choices.clear();
                    break;
                case Action::version :
                    m_help = "show program's version number and exit";
                case Action::help :
                    if (type() == Positional) {
                        // TODO: version and help actions cannot be positional
                        throw TypeError("got an unexpected keyword argument 'required'");
                    }
                case Action::store_const :
                case Action::append_const :
                case Action::count :
                    m_nargs = "0";
                    m_choices.clear();
                    break;
                case Action::store :
                case Action::append :
                case Action::extend :
                    if (m_nargs == "0") {
                        m_nargs.clear();
                    }
                    break;
                default :
                    throw ValueError("unknown action");
            }
            return *this;
        }

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
                        throw ValueError("nargs for store actions must be != 0; if you have nothing to store, actions such as store true or store const may be more appropriate");
                    }
                    break;
                case Action::append :
                case Action::extend :
                    if (value == 0) {
                        throw ValueError("nargs for append actions must be != 0; if arg strings are not supplying the value to append, the append const action may be more appropriate");
                    }
                    break;
                default:
                    throw ValueError("unknown action");
            }
            m_nargs = std::to_string(value);
            return *this;
        }

//        Argument& nargs(char value)
//        {
//            return nargs(std::string(1, value));
//        }

        Argument& nargs(std::string const& value)
        {
            if (!(m_action & (Action::store | Action::append | Action::extend))) {
                throw TypeError("got an unexpected keyword argument 'nargs'");
            }
            auto param = detail::_trim_copy(value);
            auto const available_values = { "?", "*", "+" };
            if (std::find(std::begin(available_values), std::end(available_values), param) == std::end(available_values)) {
                throw ValueError("invalid nargs value '" + param + "'");
            }
            m_nargs = param;
            return *this;
        }

        Argument& const_value(std::string const& value)
        {
            if (m_action & (Action::store_const | Action::append_const)
                    || (type() == Optional && m_nargs == "?"
                        && (m_action & (Action::store | Action::append | Action::extend)))) {
                m_const = detail::_trim_copy(value);
            } else if (type() == Optional && (m_action & (Action::store | Action::append | Action::extend)) && m_nargs != "?") {
                throw ValueError("nargs must be '?' to supply const");
            } else {
                throw TypeError("got an unexpected keyword argument 'const'");
            }
            return *this;
        }

        Argument& default_value(std::string const& value)
        {
            if (!(m_action & (Action::store_true | Action::store_false))) {
                m_default = detail::_trim_copy(value);
            }
            return *this;
        }

        Argument& choices(std::vector<std::string> const& value)
        {
            if (!(m_action & (Action::store | Action::append | Action::extend))) {
                throw TypeError("got an unexpected keyword argument 'choices'");
            }
            m_choices.clear();
            for (auto const& str : value) {
                auto param = detail::_trim_copy(str);
                if (!param.empty()) {
                    m_choices.emplace_back(param);
                }
            }
            return *this;
        }

        Argument& required(bool value)
        {
            if (type() == Positional) {
                throw TypeError("'required' is an invalid argument for positionals");
            }
            m_required = value;
            return *this;
        }

        Argument& help(std::string const& value)
        {
            m_help = detail::_trim_copy(value);
            m_help_type = NONE;
            return *this;
        }

        Argument& help(Enum value)
        {
            if (value != SUPPRESS) {
                throw TypeError("got an unexpected keyword argument 'help'");
            }
            m_help_type = value;
            return *this;
        }

        Argument& metavar(std::string const& value)
        {
            m_metavar = detail::_trim_copy(value);
            return *this;
        }

        Argument& dest(std::string const& value)
        {
            if (type() == Positional) {
                throw ValueError("dest supplied twice for positional argument");
            }
            m_dest = detail::_trim_copy(value);
            return *this;
        }

        Argument& version(std::string const& value)
        {
            if (m_action == Action::version) {
                m_version = detail::_trim_copy(value);
            } else {
                throw TypeError("got an unexpected keyword argument 'version'");
            }
            return *this;
        }

        Argument& callback(std::function<void()> func)
        {
            if (m_action == Action::store_true) {
                m_callback = func;
            } else {
                throw TypeError("got an unexpected keyword argument 'callback'");
            }
            return *this;
        }

        std::vector<std::string> const& flags() const
        {
            return m_flags;
        }

        Action action() const
        {
            return m_action;
        }

        std::string const& nargs() const
        {
            return m_nargs;
        }

        std::string const& const_value() const
        {
            return m_const;
        }

        std::string const& default_value() const
        {
            return m_default;
        }

        std::vector<std::string> const& choices() const
        {
            return m_choices;
        }

        bool required() const
        {
            return m_required;
        }

        std::string const& help() const
        {
            return m_help;
        }

        Enum help_type() const
        {
            return m_help_type;
        }

        std::string const& metavar() const
        {
            return m_metavar;
        }

        std::string const& dest() const
        {
            return m_dest;
        }

        std::string const& version() const
        {
            return m_version;
        }

        void callback() const
        {
            if (m_callback) {
                m_callback();
            }
        }

        Type type() const
        {
            return m_type;
        }

        std::string operator()() const
        {
            std::string res;
            if (type() == Optional) {
                res += m_flags.front();
            }
            if (m_action & (Action::store | Action::append | Action::extend | Action::append_const)) {
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
                    if (m_action & (Action::store | Action::append | Action::extend | Action::append_const)) {
                        res += get_nargs_suffix();
                    }
                }
            } else {
                res += get_argument_name();
            }
            return res;
        }

        std::string print(size_t limit = detail::_argument_help_limit) const
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

    private:
        std::string get_nargs_suffix() const
        {
            auto const name = get_argument_name();
            std::string res;
            if (type() == Optional) {
                res += " ";
            }
            if (m_nargs == "?") {
                res += "[" +  name + "]";
            } else if (m_nargs == "*") {
                res += "[" +  name + " ...]";
            } else if (m_nargs == "+") {
                res += name + " [" +  name + " ...]";
            } else if (!m_nargs.empty()) {
                uint32_t num_args = uint32_t(std::stoull(m_nargs));
                for (uint32_t i = 0; i < num_args; ++i) {
                    if (i != 0) {
                        res += " ";
                    }
                    res += name;
                }
            } else {
                res += name;
            }
            return res;
        }

        std::string get_argument_name() const
        {
            if (!m_metavar.empty()) {
                return m_metavar;
            }
            if (!m_choices.empty()) {
                return "{" + detail::_vector_string_to_string(m_choices, ",") + "}";
            }
            auto res = m_dest.empty() ? m_name : m_dest;
            return type() == Optional ? detail::_to_upper(res) : res;
        }

        std::vector<std::string> m_flags;
        std::string m_name;
        Type        m_type;
        Action      m_action;
        std::string m_nargs;
        std::string m_const;
        std::string m_default;
        std::vector<std::string> m_choices;
        bool        m_required;
        std::string m_help;
        Enum        m_help_type;
        std::string m_metavar;
        std::string m_dest;
        std::string m_version;
        std::function<void()> m_callback;
    };

    class Namespace
    {
        template <typename T>       struct is_stl_container:std::false_type{};
        template <typename... Args> struct is_stl_container<std::deque             <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::forward_list      <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::list              <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::multiset          <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::priority_queue    <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::set               <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::vector            <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_container<std::unordered_set     <Args...> >:std::true_type{};

        template <typename T>       struct is_stl_array:std::false_type{};
        template <typename T, std::size_t N> struct is_stl_array<std::array        <T, N> >   :std::true_type{};

        template <typename T>       struct is_stl_queue:std::false_type{};
        template <typename... Args> struct is_stl_queue<std::stack                 <Args...> >:std::true_type{};
        template <typename... Args> struct is_stl_queue<std::queue                 <Args...> >:std::true_type{};

    public:
        Namespace(std::map<std::string, ArgumentValue> const& arguments,
                  std::string const& prefix_chars)
            : m_arguments(arguments),
              m_prefix_chars(prefix_chars)
        { }

        Namespace& operator =(Namespace const&) = delete;

        template <class T, typename std::enable_if<std::is_integral<T>::value
                                                   and not std::is_same<bool, T>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto const& args = data(key);
            if (args.first == Action::count) {
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

        template <class T, typename std::enable_if<std::is_same<bool, T>::value
                                                   or std::is_floating_point<T>::value
                                                   or std::is_same<std::string, T>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto const& args = data(key);
            if (args.first == Action::count) {
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

        template <class T, typename std::enable_if<is_stl_container<typename std::decay<T>::type>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto vector = to_vector<typename T::value_type>(data(key).second);
            return T(std::begin(vector), std::end(vector));
        }

        template <class T, typename std::enable_if<is_stl_array<typename std::decay<T>::type>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto vector = to_vector<typename T::value_type>(data(key).second);
            T res{};
            if (res.size() != vector.size()) {
                std::cerr << "error: array size mismatch: " << res.size() << ", expected " << vector.size() << std::endl;
            }
            std::copy_n(vector.begin(), std::min(res.size(), vector.size()), res.begin());
            return res;
        }

        template <class T, typename std::enable_if<is_stl_queue<typename std::decay<T>::type>::value>::type* = nullptr>
        T get(std::string const& key) const
        {
            auto vector = to_vector<typename T::value_type>(data(key).second);
            return T(std::deque<typename T::value_type>(std::begin(vector), std::end(vector)));
        }

        std::string to_string(std::string const& key) const
        {
            auto const& args = data(key);
            switch (args.first) {
                case Action::store_const :
                    if (args.second.size() != 1) {
                        throw TypeError("trying to get data from array argument '" + key + "'");
                    }
                    return args.second.front();
                case Action::store_true :
                case Action::store_false :
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
                    {
                        std::string res;
                        for (auto const& str : args.second) {
                            if (!res.empty()) {
                                res += ", ";
                            }
                            res += !str.empty() ? str : "None";
                        }
                        return "[" + res + "]";
                    }
                default :
                    throw ValueError("action not supported");
            }
        }

    private:
        ArgumentValue const& data(std::string const& key) const
        {
            if (m_arguments.count(key) != 0) {
                return m_arguments.at(key);
            }
            for (auto const& pair : m_arguments) {
                if (detail::_is_optional_argument(pair.first, m_prefix_chars)
                        && detail::_flag_name(pair.first) == key) {
                    return pair.second;
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

        template <class T, typename std::enable_if<std::is_constructible<std::string, T>::value>::type* = nullptr>
        T to_type(std::string const& data) const
        {
            return detail::_remove_quotes(data);
        }

        template <class T, typename std::enable_if<not std::is_constructible<std::string, T>::value>::type* = nullptr>
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

        std::map<std::string, ArgumentValue> m_arguments;
        std::string m_prefix_chars;
    };

public:
    ArgumentParser()
        : m_prog("untitled"),
          m_usage(),
          m_description(),
          m_epilog(),
          m_parents(),
          m_prefix_chars("-"),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_add_help(true),
          m_allow_abbrev(true),
          m_exit_on_error(true),
          m_parsed_arguments(),
          m_arguments(),
          m_help_argument(Argument({ "-h", "--help" }, "help", Argument::Type::Optional).help("show this help message and exit").action(Action::store_true))
    { }

    ArgumentParser(int argc, char* argv[])
        : ArgumentParser(argc, (char const**)(argv))
    { }

    ArgumentParser(int argc, char const* argv[])
        : m_prog(detail::_file_name(argv[0])),
          m_usage(),
          m_description(),
          m_epilog(),
          m_parents(),
          m_prefix_chars("-"),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_add_help(true),
          m_allow_abbrev(true),
          m_exit_on_error(true),
          m_parsed_arguments(),
          m_arguments(),
          m_help_argument(Argument({ "-h", "--help" }, "help", Argument::Type::Optional).help("show this help message and exit").action(Action::store_true))
    {
        m_parsed_arguments.reserve(argc - 1);
        for (int i = 1; i < argc; ++i) {
            m_parsed_arguments.emplace_back(std::string(argv[i]));
        }
    }

    ArgumentParser& prog(std::string const& param)
    {
        auto value = detail::_trim_copy(param);
        if (!value.empty()) {
            m_prog = value;
        }
        return *this;
    }

    ArgumentParser& usage(std::string const& param)
    {
        m_usage = detail::_trim_copy(param);
        return *this;
    }

    ArgumentParser& description(std::string const& param)
    {
        m_description = detail::_trim_copy(param);
        return *this;
    }

    ArgumentParser& epilog(std::string const& param)
    {
        m_epilog = detail::_trim_copy(param);
        return *this;
    }

    ArgumentParser& parents(std::vector<ArgumentParser> const& param)
    {
        m_parents = param;
        return *this;
    }

    ArgumentParser& prefix_chars(std::string const& param)
    {
        auto value = detail::_trim_copy(param);
        if (!value.empty()) {
            m_prefix_chars = value;
        }
        return *this;
    }

    ArgumentParser& fromfile_prefix_chars(std::string const& param)
    {
        m_fromfile_prefix_chars = detail::_trim_copy(param);
        return *this;
    }

    ArgumentParser& argument_default(std::string const& param)
    {
        m_argument_default = detail::_trim_copy(param);
        return *this;
    }

    ArgumentParser& add_help(bool value)
    {
        m_add_help = value;
        return *this;
    }

    ArgumentParser& allow_abbrev(bool value)
    {
        m_allow_abbrev = value;
        return *this;
    }

    ArgumentParser& exit_on_error(bool value)
    {
        m_exit_on_error = value;
        return *this;
    }

    std::string const& prog() const
    {
        return m_prog;
    }

    std::string const& usage() const
    {
        return m_usage;
    }

    std::string const& description() const
    {
        return m_description;
    }

    std::string const& epilog() const
    {
        return m_epilog;
    }

    std::string const& prefix_chars() const
    {
        return m_prefix_chars;
    }

    std::string const& fromfile_prefix_chars() const
    {
        return m_fromfile_prefix_chars;
    }

    std::string const& argument_default() const
    {
        return m_argument_default;
    }

    bool add_help() const
    {
        return m_add_help;
    }

    bool allow_abbrev() const
    {
        return m_allow_abbrev;
    }

    bool exit_on_error() const
    {
        return m_exit_on_error;
    }

    Argument& add_argument(char const* flag)
    {
        return add_argument({ std::string(flag) });
    }

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
        auto _update_flag_name = [&flag_name, &prefixes] (std::string const& arg) -> void
        {
            auto name = detail::_flag_name(arg);
            auto count_prefixes = arg.size() - name.size();
            if (prefixes < count_prefixes) {
                prefixes = count_prefixes;
                flag_name = name;
            }
        };
        bool is_optional = detail::_is_optional_argument(flag_name, prefix_chars());
        if (is_optional) {
            _update_flag_name(flag_name);
        } else if (flags.size() > 1) {
            // no positional multiflag
            throw ValueError("invalid option string " + flags.front() + ": must starts with a character '" + m_prefix_chars + "'");
        }
        for (size_t i = 1; i < flags.size(); ++i) {
            // check arguments
            auto const& flag = flags.at(i);
            if (flag.empty()) {
                throw IndexError("string index out of range");
            }
            if (!detail::_is_optional_argument(flag, prefix_chars())) {
                // no positional and optional args
                throw ValueError("invalid option string " + flag + ": must starts with a character '" + m_prefix_chars + "'");
            }
            _update_flag_name(flag);
        }
        m_arguments.emplace_back(Argument(flags, flag_name, is_optional ? Argument::Type::Optional : Argument::Type::Positional));
        return m_arguments.back();
    }

    std::string get_default(std::string const& dest) const
    {
        auto const positional = positional_arguments();
        auto const optional = optional_arguments();
        for (auto const& arg : positional) {
            for (auto const& flag : arg.flags()) {
                if (flag == dest) {
                    return !arg.default_value().empty() ? arg.default_value() : argument_default();
                }
            }
        }
        for (auto const& arg : optional) {
            if (!arg.dest().empty()) {
                if (arg.dest() == dest) {
                    return !arg.default_value().empty() ? arg.default_value() : argument_default();
                }
            } else {
                for (auto const& flag : arg.flags()) {
                    auto name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        return !arg.default_value().empty() ? arg.default_value() : argument_default();
                    }
                }
            }
        }
        return std::string();
    }

    Namespace parse_args() const
    {
        return parse_args(m_parsed_arguments);
    }

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

    void print_usage(std::ostream& os = std::cout) const
    {
        os << "usage: " << get_usage() << std::endl;
    }

    void print_help(std::ostream& os = std::cout) const
    {
        print_usage(os);
        if (!m_description.empty()) {
            os << std::endl << m_description << std::endl;
        }
        size_t argument_help_size = 0;
        auto const positional = positional_arguments(false);
        auto const optional = optional_arguments(false);
        for (auto const& arg : positional) {
            auto size = arg.flags_to_string().size();
            if (argument_help_size < size) {
                argument_help_size = size;
            }
        }
        for (auto const& arg : optional) {
            auto size = arg.flags_to_string().size();
            if (argument_help_size < size) {
                argument_help_size = size;
            }
        }
        argument_help_size += 4;
        if (argument_help_size > detail::_argument_help_limit) {
            argument_help_size = detail::_argument_help_limit;
        }
        if (!positional.empty()) {
            os << std::endl << "positional arguments:" << std::endl;
            for (auto const& arg : positional) {
                os << arg.print(argument_help_size) << std::endl;
            }
        }
        if (!optional.empty()) {
            os << std::endl << "optional arguments:" << std::endl;
            for (auto const& arg : optional) {
                os << arg.print(argument_help_size) << std::endl;
            }
        }
        if (!m_epilog.empty()) {
            os << std::endl << m_epilog << std::endl;
        }
    }

private:
    Namespace parse_known_args(std::vector<std::string> parsed_arguments) const
    {
        if (!m_fromfile_prefix_chars.empty()) {
            std::vector<std::string> temp;
            temp.reserve(parsed_arguments.size());
            for (auto const& arg : parsed_arguments) {
                if (std::find(std::begin(m_fromfile_prefix_chars), std::end(m_fromfile_prefix_chars), arg.front())
                        != std::end(m_fromfile_prefix_chars)) {
                    auto const load_args = convert_arg_line_to_args(arg.substr(1));
                    temp.insert(std::end(temp), std::begin(load_args), std::end(load_args));
                } else {
                    temp.push_back(arg);
                }
            }
            parsed_arguments = std::move(temp);
        }
        auto _get_argument_flags = [] (Argument const& arg) -> std::vector<std::string>
        {
            return arg.dest().empty() ? arg.flags() : std::vector<std::string>{ arg.dest() };
        };
        auto _validate_arguments = [=] (std::vector<Argument> const& arguments)
        {
            for (auto const& arg : arguments) {
                if ((arg.action() & (Action::store_const | Action::append_const)) && arg.const_value().empty()) {
                    throw TypeError("missing 1 required positional argument: 'const'");
                }
            }
        };
        auto _validate_argument_value = [=] (Argument const& arg, std::string const& value)
        {
            auto const& choices = arg.choices();
            if (!choices.empty()) {
                auto str = detail::_remove_quotes(value);
                if (std::find(std::begin(choices), std::end(choices), str) == std::end(choices)) {
                    std::string values = detail::_vector_string_to_string(choices, ", ", "'");
                    handle_error("error: argument " + arg.flags().front() + ": invalid choice: '" + str + "' (choose from " + values + ")");
                }
            }
        };
        auto _create_result = [=] (std::vector<Argument> const& arguments,
                std::map<std::string, ArgumentValue>& result)
        {
            for (auto const& arg : arguments) {
                for (auto const& flag : _get_argument_flags(arg)) {
                    if (result.count(flag) == 0) {
                        result[flag] = std::make_pair(arg.action(), std::vector<std::string>());
                    } else {
                        throw ArgumentError("argument " + flag + ": conflicting option string: " + flag);
                    }
                }
            }
        };
        auto _is_negative_numbers_presented = [=] (std::vector<Argument> const& optionals) -> bool
        {
            if (std::find(std::begin(m_prefix_chars), std::end(m_prefix_chars), '-')
                    == std::end(m_prefix_chars)) {
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

        auto const positional = positional_arguments();
        auto const optional = optional_arguments();

        _validate_arguments(positional);
        _validate_arguments(optional);

        bool have_negative_number_options = _is_negative_numbers_presented(optional);

        std::map<std::string, ArgumentValue> result;
        _create_result(positional, result);
        _create_result(optional, result);

        auto _get_optional_argument_by_flag = [=] (std::string const& key) -> Argument const*
        {
            for (auto const& arg : optional) {
                for (auto const& flag : arg.flags()) {
                    if (flag == key) {
                        return &arg;
                    }
                }
            }
            return nullptr;
        };
        auto _get_optional_argument_by_dest = [=] (std::string const& key) -> Argument const*
        {
            for (auto const& arg : optional) {
                if (!arg.dest().empty() && arg.dest() == key) {
                    return &arg;
                }
                for (auto const& flag : arg.flags()) {
                    if (flag == key) {
                        return &arg;
                    }
                }
            }
            return nullptr;
        };

        std::vector<std::vector<std::string> > positional_values;
        std::vector<std::string> unrecognized_arguments;

        size_t position = 0;
        auto _store_positional_arguments = [&] (size_t& i) -> void
        {
            std::vector<std::string> values;
            values.push_back(parsed_arguments.at(i));
            while (true) {
                ++i;
                if (i == parsed_arguments.size()) {
                    break;
                } else {
                    auto const& next = parsed_arguments.at(i);
                    if (!detail::_is_optional_argument(next, prefix_chars())
                            || (!have_negative_number_options && detail::_is_negative_number(next))) {
                        values.push_back(next);
                    } else {
                        --i;
                        break;
                    }
                }
            }
            positional_values.emplace_back(std::move(values));
        };
        auto _is_positional_argument_stored = [&] (Argument const& argument) -> bool
        {
            if (argument.action() & (Action::store_const | Action::store_true | Action::store_false)) {
                for (auto const& flag : _get_argument_flags(argument)) {
                    if (result.at(flag).second.empty()) {
                        result.at(flag).second.push_back(argument.const_value());
                    }
                }
                return true;
            } else if (argument.action() == Action::append_const) {
                for (auto const& flag : _get_argument_flags(argument)) {
                    if (!argument.default_value().empty()) {
                        handle_error("error: argument " + argument.flags().front() + ": ignored default value '" + argument.default_value() + "'");
                    }
                    result.at(flag).second.push_back(argument.const_value());
                }
                return true;
            } else if (argument.action() == Action::count) {
                for (auto const& flag : _get_argument_flags(argument)) {
                    result.at(flag).second.push_back("");
                }
                return true;
            }
            return false;
        };
        auto _store_argument_value = [&] (Argument const& argument, std::string const& arg) -> void
        {
            _validate_argument_value(argument, arg);
            for (auto const& flag : _get_argument_flags(argument)) {
                result.at(flag).second.push_back(arg);
            }
        };
        auto _default_argument_value = [=] (Argument const& argument) -> std::string
        {
            if (!argument.default_value().empty()) {
                return argument.default_value();
            }
            return m_argument_default;
        };
        auto _match_arguments_partial = [&] (std::vector<std::string> const& arguments) -> void
        {
            if (position >= positional.size()) {
                for (auto const& arg : arguments) {
                    unrecognized_arguments.push_back(arg);
                }
                return;
            }
            size_t finish = position;
            size_t min_args = 0;
            size_t one_args = 0;
            bool more_args = false;
            for ( ; finish < positional.size(); ++finish) {
                auto const& argument = positional.at(finish);
                if (!(argument.action() & (Action::store | Action::append | Action::extend))) {
                    continue;
                }
                auto const& nargs = argument.nargs();
                size_t min_amount = 0;
                if (nargs.empty()) {
                    ++min_amount;
                } else if (nargs == "+") {
                    ++min_amount;
                    more_args = true;
                } else if (nargs == "?") {
                    ++one_args;
                } else if (nargs == "*") {
                    more_args = true;
                } else {
                    min_amount += uint32_t(std::stoull(nargs));
                }
                if (min_args + min_amount > arguments.size()) {
                    break;
                }
                min_args += min_amount;
            }
            if (finish == position) {
                for (auto const& arg : arguments) {
                    unrecognized_arguments.push_back(arg);
                }
                return;
            } else if (min_args == arguments.size()) {
                size_t i = 0;
                for ( ; position < finish; ++position) {
                    auto const& argument = positional.at(position);
                    if (_is_positional_argument_stored(argument)) {
                        continue;
                    }
                    auto const& nargs = argument.nargs();
                    if (nargs.empty() || nargs == "+") {
                        _store_argument_value(argument, arguments.at(i));
                        ++i;
                    } else if (nargs == "?" || nargs == "*") {
                        if (argument.action() == Action::store) {
                            for (auto const& flag : _get_argument_flags(argument)) {
                                if (result.at(flag).second.empty()) {
                                    result.at(flag).second.push_back(_default_argument_value(argument));
                                }
                            }
                        }
                    } else {
                        size_t num_args = size_t(std::stoull(nargs));
                        for (size_t n = 0; n < num_args; ++i, ++n) {
                            _store_argument_value(argument, arguments.at(i));
                        }
                    }
                }
            } else if (more_args) {
                size_t over_args = arguments.size() - min_args;
                size_t i = 0;
                for ( ; position < finish; ++position) {
                    auto const& argument = positional.at(position);
                    if (_is_positional_argument_stored(argument)) {
                        continue;
                    }
                    auto const& nargs = argument.nargs();
                    if (nargs.empty()) {
                        _store_argument_value(argument, arguments.at(i));
                        ++i;
                    } else if (nargs == "+") {
                        _store_argument_value(argument, arguments.at(i));
                        ++i;
                        while (over_args > 0) {
                            _store_argument_value(argument, arguments.at(i));
                            ++i;
                            --over_args;
                        }
                    } else if (nargs == "?") {
                        if (argument.action() == Action::store) {
                            for (auto const& flag : _get_argument_flags(argument)) {
                                if (result.at(flag).second.empty()) {
                                    result.at(flag).second.push_back(_default_argument_value(argument));
                                }
                            }
                        }
                    } else if (nargs == "*") {
                        if (over_args > 0) {
                            while (over_args > 0) {
                                _store_argument_value(argument, arguments.at(i));
                                ++i;
                                --over_args;
                            }
                        } else if (argument.action() == Action::store) {
                            for (auto const& flag : _get_argument_flags(argument)) {
                                if (result.at(flag).second.empty()) {
                                    result.at(flag).second.push_back(_default_argument_value(argument));
                                }
                            }
                        }
                    } else {
                        size_t num_args = size_t(std::stoull(nargs));
                        for (size_t n = 0; n < num_args; ++i, ++n) {
                            _store_argument_value(argument, arguments.at(i));
                        }
                    }
                }
            } else if (min_args + one_args >= arguments.size()) {
                size_t over_args = min_args + one_args - arguments.size();
                size_t i = 0;
                for ( ; position < finish; ++position) {
                    auto const& argument = positional.at(position);
                    if (_is_positional_argument_stored(argument)) {
                        continue;
                    }
                    auto const& nargs = argument.nargs();
                    if (nargs.empty()) {
                        _store_argument_value(argument, arguments.at(i));
                        ++i;
                    } else if (nargs == "?") {
                        if (over_args < one_args) {
                            _store_argument_value(argument, arguments.at(i));
                            ++i;
                            ++over_args;
                        } else if (argument.action() == Action::store) {
                            for (auto const& flag : _get_argument_flags(argument)) {
                                if (result.at(flag).second.empty()) {
                                    result.at(flag).second.push_back(_default_argument_value(argument));
                                }
                            }
                        }
                    } else {
                        size_t num_args = size_t(std::stoull(nargs));
                        for (size_t n = 0; n < num_args; ++i, ++n) {
                            _store_argument_value(argument, arguments.at(i));
                        }
                    }
                }
            } else {
                size_t i = 0;
                for ( ; position < finish; ++position) {
                    auto const& argument = positional.at(position);
                    if (_is_positional_argument_stored(argument)) {
                        continue;
                    }
                    auto const& nargs = argument.nargs();
                    if (nargs.empty()) {
                        _store_argument_value(argument, arguments.at(i));
                        ++i;
                    } else {
                        size_t num_args = (nargs == "?" ? 1 : size_t(std::stoull(nargs)));
                        for (size_t n = 0; n < num_args; ++i, ++n) {
                            _store_argument_value(argument, arguments.at(i));
                        }
                    }
                }
                for ( ; i < arguments.size(); ++i) {
                    unrecognized_arguments.push_back(arguments.at(i));
                }
            }
        };
        auto _separate_argument_abbreviations = [=] (std::vector<std::string>& temp_arguments,
                std::string const& arg, std::string const& name) -> void
        {
            if (name.size() + 1 == arg.size()) {
                auto const splitted = detail::_split_equal(arg);
                if (splitted.size() == 2 && _get_optional_argument_by_flag(splitted.front())) {
                    temp_arguments.push_back(arg);
                    return;
                }
                std::vector<std::string> flags;
                for (size_t i = 0; i < name.size(); ++i) {
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
                            if (!detail::_starts_with(str, "=")) {
                                flags.back() += "=";
                            }
                            flags.back() += str;
                        }
                        break;
                    } else if (argument->action() & (Action::store | Action::append | Action::extend)) {
                        auto str = name.substr(i + 1);
                        if (!detail::_starts_with(str, "=")) {
                            flags.back() += "=";
                        }
                        flags.back() += str;
                        break;
                    }
                }
                for (auto const& f : flags) {
                    temp_arguments.push_back(f);
                }
            } else {
                temp_arguments.push_back(arg);
            }
        };
        auto _check_abbreviations = [=] (std::vector<std::string>& arguments) -> void
        {
            std::vector<std::string> temp_arguments;
            for (auto const& arg : arguments) {
                if (!arg.empty() && result.count(arg) == 0
                        && detail::_is_optional_argument(arg, prefix_chars())
                        && (have_negative_number_options || !detail::_is_negative_number(arg))) {
                    if (m_allow_abbrev) {
                        bool is_flag_added = false;
                        std::string args;
                        std::vector<std::string> keys;
                        for (auto const& opt : optional) {
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
                            handle_error("error: ambiguous option: '" + arg + "' could match" + args);
                        }
                        if (is_flag_added) {
                            temp_arguments.push_back(keys.empty() ? arg : keys.front());
                        } else {
                            auto name = detail::_flag_name(keys.empty() ? arg : keys.front());
                            _separate_argument_abbreviations(temp_arguments, arg, name);
                        }
                    } else {
                        auto name = detail::_flag_name(arg);
                        _separate_argument_abbreviations(temp_arguments, arg, name);
                    }
                } else {
                    temp_arguments.push_back(arg);
                }
            }
            arguments = std::move(temp_arguments);
        };
        _check_abbreviations(parsed_arguments);

        for (size_t i = 0; i < parsed_arguments.size(); ++i) {
            auto arg = parsed_arguments.at(i);
            if (m_add_help
                    && std::find(std::begin(m_help_argument.flags()),
                                 std::end(m_help_argument.flags()), arg)
                    != std::end(m_help_argument.flags())) {
                print_help();
                exit(0);
            }
            auto const splitted = detail::_split_equal(arg);
            if (splitted.size() == 2) {
                arg = splitted.front();
            }
            auto const* temp = _get_optional_argument_by_flag(arg);
            if (temp) {
                switch (temp->action()) {
                    case Action::store :
                        for (auto const& flag : _get_argument_flags(*temp)) {
                            result.at(flag).second.clear();
                        }
                    case Action::append :
                    case Action::extend :
                        if (splitted.size() == 2) {
                            auto const& nargs = temp->nargs();
                            if (!nargs.empty() && nargs != "?" && nargs != "*" && nargs != "+" && nargs != "1") {
                                handle_error("error: argument " + arg + ": expected " + nargs + " arguments");
                            }
                            if (splitted.back().empty()) {
                                handle_error("error: argument " + arg + ": expected one argument");
                            }
                            _store_argument_value(*temp, splitted.back());
                        } else {
                            auto const& nargs = temp->nargs();
                            uint32_t n = 0;
                            uint32_t num_args = (!nargs.empty() && nargs != "?" && nargs != "*" && nargs != "+") ? uint32_t(std::stoull(nargs)) : 0;
                            while (true) {
                                ++i;
                                if (i == parsed_arguments.size()) {
                                    if (n == 0) {
                                        if (nargs.empty()) {
                                            handle_error("error: argument " + arg + ": expected one argument");
                                        } else if (nargs == "?") {
                                            _store_argument_value(*temp, temp->const_value());
                                        } else if (nargs == "*") {
                                            // OK
                                        } else if (nargs == "+") {
                                            handle_error("error: argument " + arg + ": expected at least one argument");
                                        } else {
                                            handle_error("error: argument " + arg + ": expected " + nargs + " arguments");
                                        }
                                    } else if (num_args != 0 && n < num_args) {
                                        handle_error("error: argument " + arg + ": expected " + nargs + " arguments");
                                    }
                                    break;
                                } else {
                                    auto const& next = parsed_arguments.at(i);
                                    if (!detail::_is_optional_argument(next, prefix_chars()) || (!have_negative_number_options && detail::_is_negative_number(next))) {
                                        _store_argument_value(*temp, next);
                                        ++n;
                                    } else if (n == 0) {
                                        --i;
                                        if (nargs.empty()) {
                                            handle_error("error: argument " + arg + ": expected one argument");
                                        } else if (nargs == "?") {
                                            _store_argument_value(*temp, temp->const_value());
                                            break;
                                        } else if (nargs == "*") {
                                            break;
                                        } else if (nargs == "+") {
                                            handle_error("error: argument " + arg + ": expected at least one argument");
                                        } else {
                                            handle_error("error: argument " + arg + ": expected " + nargs + " arguments");
                                        }
                                    } else {
                                        if (num_args != 0 && n < num_args) {
                                            handle_error("error: argument " + arg + ": expected " + nargs + " arguments");
                                        }
                                        --i;
                                        break;
                                    }
                                }
                                if (nargs.empty() || (num_args != 0 && n == num_args)) {
                                    break;
                                }
                            }
                        }
                        break;
                    case Action::store_const :
                    case Action::store_true :
                    case Action::store_false :
                        if (splitted.size() == 1) {
                            for (auto const& flag : _get_argument_flags(*temp)) {
                                if (result.at(flag).second.empty()) {
                                    result.at(flag).second.push_back(temp->const_value());
                                }
                            }
                            if (temp->action() == Action::store_true) {
                                temp->callback();
                            }
                        } else {
                            handle_error("error: argument " + arg + ": ignored explicit argument '" + splitted.back() + "'");
                        }
                        break;
                    case Action::append_const :
                        if (splitted.size() == 1) {
                            for (auto const& flag : _get_argument_flags(*temp)) {
                                if (!temp->default_value().empty()) {
                                    handle_error("error: argument " + arg + ": ignored default value '" + temp->default_value() + "'");
                                }
                                result.at(flag).second.push_back(temp->const_value());
                            }
                        } else {
                            handle_error("error: argument " + arg + ": ignored explicit argument '" + splitted.back() + "'");
                        }
                        break;
                    case Action::help :
                        if (splitted.size() == 1) {
                            print_help();
                            exit(0);
                        } else {
                            handle_error("error: argument " + arg + ": ignored explicit argument '" + splitted.back() + "'");
                        }
                        break;
                    case Action::version :
                        if (splitted.size() == 1) {
                            if (temp->version().empty()) {
                                throw AttributeError("'ArgumentParser' object has no attribute 'version'");
                            }
                            std::cout << temp->version() << std::endl;
                            exit(0);
                        } else {
                            handle_error("error: argument " + arg + ": ignored explicit argument '" + splitted.back() + "'");
                        }
                        break;
                    case Action::count :
                        if (splitted.size() == 1) {
                            for (auto const& flag : _get_argument_flags(*temp)) {
                                result.at(flag).second.push_back("");
                            }
                        } else {
                            handle_error("error: argument " + arg + ": ignored explicit argument '" + splitted.back() + "'");
                        }
                        break;
                    default :
                        handle_error("error: action not supported");
                        break;
                }
            } else if (have_negative_number_options && detail::_is_negative_number(arg)) {
                unrecognized_arguments.push_back(arg);
            } else {
                _store_positional_arguments(i);
            }
        }
        for (auto const& values : positional_values) {
            _match_arguments_partial(values);
        }
        std::vector<std::string> required_arguments;
        for (auto const& arg : optional) {
            if (arg.required()) {
                for (auto const& flag : _get_argument_flags(arg)) {
                    if (result.at(flag).second.empty()) {
                        required_arguments.emplace_back(detail::_vector_string_to_string(arg.flags(), "/"));
                        break;
                    }
                }
            }
        }
        if (!required_arguments.empty() || position < positional.size()) {
            std::string args;
            for ( ; position < positional.size(); ++position) {
                auto const& argument = positional.at(position);
                if (args.empty()) {
                    if (_is_positional_argument_stored(argument)) {
                        continue;
                    }
                    auto const& nargs = argument.nargs();
                    if (nargs == "?" || nargs == "*") {
                        if (argument.action() == Action::store) {
                            for (auto const& flag : _get_argument_flags(argument)) {
                                if (result.at(flag).second.empty()) {
                                    result.at(flag).second.push_back(_default_argument_value(argument));
                                }
                            }
                        }
                        continue;
                    }
                }
                if (!args.empty()) {
                    args += ", ";
                }
                args += argument.flags().front();
            }
            args += detail::_vector_string_to_string(required_arguments, ", ");
            if (!args.empty()) {
                handle_error("error: the following arguments are required: " + args);
            }
        }
        if (!unrecognized_arguments.empty()) {
            std::string args = detail::_vector_string_to_string(unrecognized_arguments, " ");
            handle_error("error: unrecognized arguments: " + args);
        }
        for (auto& arg : result) {
            if (arg.second.second.empty() && arg.second.first != Action::count) {
                auto const* argument = _get_optional_argument_by_dest(arg.first);
                if (!argument) {
                    continue;
                }
                auto value = _default_argument_value(*argument);
                if (!value.empty()) {
                    arg.second.second.push_back(value);
                }
            }
        }
        return Namespace(result, prefix_chars());
    }

    void handle_error(std::string const& error = "error: unknown") const
    {
        print_usage(std::cerr);
        throw std::logic_error(m_prog + ": " + error);
    }

    std::vector<std::string> convert_arg_line_to_args(std::string const& file) const
    {
        std::ifstream is(file);
        if (!is.is_open()) {
            handle_error("error: [Errno 2] No such file or directory: '" + file + "'");
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
            if (arg.type() == Argument::Type::Positional
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
            result.push_back(m_help_argument);
        }
        for (auto const& parent : m_parents) {
            auto const args = parent.optional_arguments(add_suppress);
            result.insert(std::end(result), std::begin(args), std::end(args));
        }
        for (auto const& arg : m_arguments) {
            if (arg.type() == Argument::Type::Optional
                    && (add_suppress || arg.help_type() != SUPPRESS)) {
                result.push_back(arg);
            }
        }
        return result;
    }

    std::string get_usage() const
    {
        if (m_usage.empty()) {
            auto res = m_prog;
            size_t argument_usage_size = 0;
            auto const positional = positional_arguments(false);
            auto const optional = optional_arguments(false);
            for (auto const& arg : positional) {
                if (argument_usage_size < arg().size()) {
                    argument_usage_size = arg().size();
                }
            }
            for (auto const& arg : optional) {
                if (argument_usage_size < arg().size()) {
                    argument_usage_size = arg().size();
                }
            }
            size_t usage_length = std::string("usage: ").size();
            size_t pos = usage_length + m_prog.size();
            size_t offset = usage_length;
            if (pos + (argument_usage_size > 0 ? (1 + argument_usage_size) : 0) <= detail::_usage_limit) {
                offset += m_prog.size() + (argument_usage_size > 0 ? 1 : 0);
            } else if (!(optional.empty() && positional.empty())) {
                res += "\n" + std::string(offset - 1, ' ');
                pos = offset - 1;
            }
            for (auto const& arg : optional) {
                auto str = arg();
                if ((pos + 1 == offset) || (pos + 1 + str.size() <= detail::_usage_limit)) {
                    res += " [" + str + "]";
                } else {
                    res += "\n" + std::string(offset, ' ') + "[" + str + "]";
                    pos = offset;
                }
                pos += 1 + str.size();
            }
            for (auto const& arg : positional) {
                auto str = arg();
                if ((pos + 1 == offset) || (pos + 1 + str.size() <= detail::_usage_limit)) {
                    res += " " + str + "";
                } else {
                    res += "\n" + std::string(offset, ' ') + str;
                    pos = offset;
                }
                pos += 1 + str.size();
            }
            return res;
        }
        return m_usage;
    }

    std::string m_prog;
    std::string m_usage;
    std::string m_description;
    std::string m_epilog;
    std::vector<ArgumentParser> m_parents;
    std::string m_prefix_chars;
    std::string m_fromfile_prefix_chars;
    std::string m_argument_default;
    bool m_add_help;
    bool m_allow_abbrev;
    bool m_exit_on_error;

    std::vector<std::string> m_parsed_arguments;
    std::vector<Argument> m_arguments;
    Argument m_help_argument;
};
} // argparse

#endif // _ARGPARSE_ARGUMENT_PARSER_HPP_
