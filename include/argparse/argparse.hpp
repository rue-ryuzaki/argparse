/*
* MIT License
*
* Argument parser for C++ (ArgumentParser v1.7.2)
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

#undef ARGPARSE_VERSION_MAJOR
#undef ARGPARSE_VERSION_MINOR
#undef ARGPARSE_VERSION_PATCH
#undef ARGPARSE_VERSION_TWEAK
#undef ARGPARSE_VERSION_NUM
#undef ARGPARSE_VERSION_COMPILED
#undef ARGPARSE_VERSION_AT_LEAST

#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_ENUM_TYPE
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_FINAL
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_NULLPTR
#undef _ARGPARSE_OVERRIDE
#undef _ARGPARSE_USE_CONSTEXPR

#undef _ARGPARSE_CXX_98
#undef _ARGPARSE_CXX_11
#undef _ARGPARSE_CXX_14
#undef _ARGPARSE_CXX_17
#undef _ARGPARSE_CXX_20
#undef _ARGPARSE_CXX_23

#define ARGPARSE_VERSION_MAJOR 1
#define ARGPARSE_VERSION_MINOR 7
#define ARGPARSE_VERSION_PATCH 2

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

#ifdef ARGPARSE_NO_AUTODETECT
#warning "ARGPARSE_NO_AUTODETECT define is deprecated and will be removed \
in the next minor release (v1.8.0), \
use ARGPARSE_DISABLE_TERMINAL_SIZE_DETECTION define"
#endif  // ARGPARSE_NO_AUTODETECT

#undef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#if !defined ARGPARSE_DISABLE_TERMINAL_SIZE_DETECTION \
 && !defined ARGPARSE_NO_AUTODETECT
#define ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION

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
#ifdef _ARGPARSE_CXX_17
#include <optional>
#endif  // C++17+
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#ifdef _ARGPARSE_CXX_17
#include <string_view>
#endif  // C++17+
#include <utility>
#include <vector>

#define _ARGPARSE_EXPORT

#ifdef _ARGPARSE_CXX_11
#define _ARGPARSE_FINAL final
#define _ARGPARSE_NOEXCEPT noexcept
#define _ARGPARSE_OVERRIDE override
#define _ARGPARSE_CONSTEXPR constexpr
#define _ARGPARSE_USE_CONSTEXPR constexpr
#define _ARGPARSE_ENUM_TYPE(X) : X
#else
#define _ARGPARSE_FINAL
#define _ARGPARSE_NOEXCEPT
#define _ARGPARSE_OVERRIDE
#define _ARGPARSE_CONSTEXPR
#define _ARGPARSE_USE_CONSTEXPR const
#define _ARGPARSE_ENUM_TYPE(X)
#endif  // C++11+

#ifdef _ARGPARSE_CXX_17
#define _ARGPARSE_INLINE_VARIABLE inline
#else
#define _ARGPARSE_INLINE_VARIABLE static
#endif  // C++17+

#ifndef ARGPARSE_TAB_SIZE
#define ARGPARSE_TAB_SIZE 4
#endif  // ARGPARSE_TAB_SIZE

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
 *  \brief _HelpFormatter class
 */
struct _HelpFormatter
{
    std::string (*_fill_text)(std::string const& text, std::size_t width,
                              std::size_t indent);
    std::string (*_get_default_metavar_for_optional)(Argument const* action);
    std::string (*_get_default_metavar_for_positional)(Argument const* action);
    std::string (*_get_help_string)(Argument const* action,
                                    std::string const& lang);
    std::vector<std::string> (*_split_lines)(std::string const& text,
                                             std::size_t width);
};

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

#ifdef _ARGPARSE_CXX_11
template <class K, class V, class C, class A>
V const& _map_at(std::map<K, V, C, A> const& m, K const& k)
{
    return m.at(k);
}
#else
template <class K, class V, class C, class A>
V const& _map_at(std::map<K, V, C, A> const& m, K const& k)
{
    typename std::map<K, V, C, A>::const_iterator it(m.find(k));
    if (it == m.end()) {
        throw std::out_of_range("key not found in map");
    }
    return it->second;
}
#endif  // C++11+

#ifdef _ARGPARSE_CXX_11
using std::shared_ptr;
using std::make_shared;
#else
//  Slightly modified version of the shared_ptr implementation for C++98
//  from Sébastien Rombauts which is licensed under the MIT License.
//  See https://github.com/SRombauts/shared_ptr
class _shared_ptr_count
{
public:
    _shared_ptr_count()
        : pn(NULL)
    { }

    _shared_ptr_count(_shared_ptr_count const& orig)
        : pn(orig.pn)
    { }

    inline void swap(_shared_ptr_count& lhs) throw()
    {
        std::swap(pn, lhs.pn);
    }

    inline std::size_t use_count() const throw()
    {
        std::size_t count = 0;
        if (pn) {
            count = *pn;
        }
        return count;
    }

    template <class U>
    void acquire(U*& p)
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
    void release(U*& p) throw()
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

    _shared_ptr_base(_shared_ptr_base const& orig)
        : pn(orig.pn)
    { }

    _shared_ptr_count pn;
};

template <class T>
class shared_ptr : public _shared_ptr_base
{
public:
    typedef T element_type;

    shared_ptr() throw()
        : _shared_ptr_base(),
          px(NULL)
    { }

    explicit shared_ptr(T* p)
        : _shared_ptr_base(),
          px(NULL)
    {
        acquire(p);
    }

    explicit shared_ptr(nullptr_t)
        : _shared_ptr_base(),
          px(NULL)
    { }

    template <class U>
    shared_ptr(shared_ptr<U> const& ptr, T* p)
        : _shared_ptr_base(ptr),
          px(NULL)
    {
        acquire(p);
    }

    template <class U>
    explicit shared_ptr(shared_ptr<U> const& orig) throw()
        : _shared_ptr_base(orig),
          px(NULL)
    {
        assert(!orig.get() || (orig.use_count() != 0));
        acquire(static_cast<typename shared_ptr<T>::element_type*>(orig.get()));
    }

    shared_ptr(shared_ptr const& orig) throw()
        : _shared_ptr_base(orig),
          px(NULL)
    {
        assert(!orig.px || (orig.use_count() != 0));
        acquire(orig.px);
    }

    inline shared_ptr& operator =(shared_ptr ptr) throw()
    {
        swap(ptr);
        return *this;
    }

    ~shared_ptr() throw()
    {
        release();
    }

    inline void reset() throw()
    {
        release();
    }

    inline void reset(T* p)
    {
        assert(!p || (px != p));
        release();
        acquire(p);
    }

    inline void swap(shared_ptr& lhs) throw()
    {
        std::swap(px, lhs.px);
        pn.swap(lhs.pn);
    }

    inline operator bool() const throw()
    {
        return use_count() > 0;
    }

    inline bool unique() const throw()
    {
        return use_count() == 1;
    }

    inline std::size_t use_count() const throw()
    {
        return pn.use_count();
    }

    inline T& operator *() const throw()
    {
        assert(NULL != px);
        return *px;
    }

    inline T* operator ->() const throw()
    {
        assert(px);
        return px;
    }

    inline T* get() const throw()
    {
        return px;
    }

private:
    inline void acquire(T* p)
    {
        pn.acquire(p);
        px = p;
    }

    inline void release() throw()
    {
        pn.release(px);
    }

private:
    T* px;
};

template <class T, class U>
bool operator ==(shared_ptr<T> const& l, shared_ptr<U> const& r) throw()
{
    return (l.get() == r.get());
}

template <class T, class U>
bool operator !=(shared_ptr<T> const& l, shared_ptr<U> const& r) throw()
{
    return (l.get() != r.get());
}

template <class T, class U>
bool operator <=(shared_ptr<T> const& l, shared_ptr<U> const& r) throw()
{
    return (l.get() <= r.get());
}

template <class T, class U>
bool operator <(shared_ptr<T> const& l, shared_ptr<U> const& r) throw()
{
    return (l.get() < r.get());
}

template <class T, class U>
bool operator >=(shared_ptr<T> const& l, shared_ptr<U> const& r) throw()
{
    return (l.get() >= r.get());
}

template <class T, class U>
bool operator >(shared_ptr<T> const& l, shared_ptr<U> const& r) throw()
{
    return (l.get() > r.get());
}

template <class T>
shared_ptr<T>
make_shared(T const& t)
{
    return shared_ptr<T>(new T(t));
}

template <class T, class U>
shared_ptr<T> _pointer_cast(shared_ptr<U> const& r) throw()
{
    return shared_ptr<T>(r);
}
#endif  // C++11+

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
private:
    typedef typename remove_reference<T>::type U;

public:
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
class _is_constructible_impl
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

public:
    static const bool value
              = (sizeof(test<T, AU_1, AU_2, AU_3, AU_4>(NULL)) == sizeof(bool));
};

template <class T, class AT_1, class AT_2, class AT_3>
class _is_constructible_impl<T, AT_1, AT_2, AT_3, void>
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

public:
    static const bool value
                    = (sizeof(test<T, AU_1, AU_2, AU_3>(NULL)) == sizeof(bool));
};

template <class T, class AT_1, class AT_2>
class _is_constructible_impl<T, AT_1, AT_2, void, void>
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

public:
    static const bool value
                          = (sizeof(test<T, AU_1, AU_2>(NULL)) == sizeof(bool));
};

template <class T, class AT_1>
class _is_constructible_impl<T, AT_1, void, void, void>
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

public:
    static const bool value = (sizeof(test<T, AU_1>(NULL)) == sizeof(bool));
};

template <class T>
class _is_constructible_impl<T, void, void, void, void>
{
    template <class C_T>
    static C_T testFun(C_T);

    template <class C_T>
    static bool test(
            typename enable_if<sizeof(C_T) == sizeof(testFun(C_T()))>::type*);

    template <class>
    static int test(...);

public:
    static const bool value = (sizeof(test<T>(NULL)) == sizeof(bool));
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
class _is_constructible_impl_ptr
{
public:
    static const bool value = false;
};

template <class T, class AT_1>
class _is_constructible_impl_ptr<T, AT_1,
        typename enable_if<is_pointer<
            typename remove_reference<T>::type>::value, void>::type, void, void>
{
    template <class C_T>
    static bool test(C_T);

    template <class>
    static int test(...);

public:
    static const bool value
                   = (sizeof(test<T>(static_cast<AT_1>(NULL))) == sizeof(bool));
};

template <class T>
class _is_constructible_impl_ptr<T, void, void, void, void>
{
public:
    static const bool value = true;
};

template <class T,
          class AT_1 = void,
          class AT_2 = void,
          class AT_3 = void,
          class AT_4 = void>
class is_constructible
{
    typedef typename _replace_array_with_pointer<T>::type U;

public:
    static const bool value = (
        is_pointer<typename remove_reference<U>::type>::value
                ? _is_constructible_impl_ptr<U, AT_1, AT_2, AT_3, AT_4>::value
                : _is_constructible_impl<U, AT_1, AT_2, AT_3, AT_4>::value
    );
};

template <>
class is_constructible<std::string, _SUPPRESS >
{
public:
    static const bool value = false;
};

template <>
class is_constructible<std::string, std::vector<std::string> >
{
public:
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
template <class T, class... Args>
std::vector<T>
_make_vector(Args... args)
{
    return std::vector<T>{ args... };
}
#else
template <class T>
std::vector<T>
_make_vector(T const& arg1)
{
    std::vector<T> res;
    res.push_back(arg1);
    return res;
}

template <class T>
std::vector<T>
_make_vector(T const& arg1, T const& arg2)
{
    std::vector<T> res;
    res.push_back(arg1);
    res.push_back(arg2);
    return res;
}

template <class T>
std::vector<T>
_make_vector(T const& arg1, T const& arg2, T const& arg3)
{
    std::vector<T> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    return res;
}

template <class T>
std::vector<T>
_make_vector(T const& arg1, T const& arg2, T const& arg3, T const& arg4)
{
    std::vector<T> res;
    res.push_back(arg1);
    res.push_back(arg2);
    res.push_back(arg3);
    res.push_back(arg4);
    return res;
}
#endif  // C++11+

inline std::string
_to_upper(std::string str)
{
#ifdef _ARGPARSE_CXX_11
    std::transform(str.begin(), str.end(), str.begin(),
                   [] (unsigned char c)
    { return static_cast<char>(std::toupper(c)); });
#else
    for (std::size_t i = 0; i < str.size(); ++i) {
        str.at(i) = static_cast<char>(
                        std::toupper(static_cast<unsigned char>(str.at(i))));
    }
#endif  // C++11+
    return str;
}

// -- utf8 support ------------------------------------------------------------
// since v1.7.0
inline uint8_t
_char_to_u8(char c)
{
    return static_cast<uint8_t>(c);
}

inline char
_u8_to_char(uint8_t c)
{
    return static_cast<char>(c);
}

typedef uint32_t u32char;

inline u32char
_char_to_u32(char c)
{
    return static_cast<u32char>(_char_to_u8(c));
}

inline char
_u32_to_char(u32char c)
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

inline std::size_t
_utf8_codepoint_size(uint8_t byte)
{
    if ((byte & _utf8_1b_mask) == _utf8_1b_bits) {
        return 1;
    }
    if ((byte & _utf8_2b_mask) == _utf8_2b_bits) {
        return 2;
    }
    if ((byte & _utf8_3b_mask) == _utf8_3b_bits) {
        return 3;
    }
    if ((byte & _utf8_4b_mask) == _utf8_4b_bits) {
        return 4;
    }
    return 0;
}

inline std::pair<bool, std::size_t>
_utf8_length(std::string const& value, std::ostream& err = std::cerr)
{
    std::size_t res = 0;
    std::size_t i = 0;
    while (i < value.size()) {
        std::size_t cp_size = _utf8_codepoint_size(_char_to_u8(value[i]));
        if (cp_size == 0) {
            err << "argparse error [skip]: invalid code point for string "
                << "'" << value << "'" << std::endl;
            return std::make_pair(false, value.size());
        }
        if (i + cp_size > value.size()) {
            err << "argparse error [skip]: code point for string '"
                << value << "' would be out of bounds" << std::endl;
            return std::make_pair(false, value.size());
        }
        for (std::size_t n = 1; n < cp_size; ++n) {
            if (value[i + n] == '\0') {
                err << "argparse error [skip]: string '" << value << "' "
                    << "is NUL-terminated in the middle of the code point"
                    << std::endl;
                return std::make_pair(false, value.size());
            } else if ((_char_to_u8(value[i + n]) & _utf8_ct_mask)
                                                 != _utf8_ct_bits) {
                err << "argparse error [skip]: invalid byte in code point"
                    << " for string '" << value << "'" << std::endl;
                return std::make_pair(false, value.size());
            }
        }
        i += cp_size;
        ++res;
    }
    return std::make_pair(true, res);
}

// since NEXT_RELEASE
inline bool
_is_utf8(std::string const& value)
{
    std::stringstream ss;
    return _utf8_length(value, ss).second;
}

// since NEXT_RELEASE
inline u32char
_to_upper_codepoint(u32char cp)
{
    // IBM's Unicode lowercase to uppercase conversion mapping table
    if ((0x0061 <= cp && cp <= 0x007a)
     || (0x00e0 <= cp && cp <= 0x00f6)
     || (0x00f8 <= cp && cp <= 0x00fe)
     || (0x03b1 <= cp && cp <= 0x03c1)
     || (0x03c3 <= cp && cp <= 0x03cb)
     || (0x0430 <= cp && cp <= 0x044f)
     || (0xff41 <= cp && cp <= 0xff5a)) {
        // 0x0061 - 0x007a: LATIN LETTERS
        // 0x00e0 - 0x00f6: LATIN LETTERS
        // 0x00f8 - 0x00fe: LATIN LETTERS
        // 0x03b1 - 0x03c1: GREEK LETTERS
        // 0x03c3 - 0x03cb: GREEK LETTERS
        // 0x0430 - 0x044f: CYRILLIC LETTERS
        // 0xff41 - 0xff5a: FULLWIDTH LATIN LETTERS
        return cp - 0x20;
    }
    if ((0x24d0 <= cp && cp <= 0x24e9)) {
        // 0x24d0 - 0x24e9: CIRCLED LATIN LETTERS
        return cp - 0x1a;
    }
    if ((0x0561 <= cp && cp <= 0x0586)
     || (0x10D0 <= cp && cp <= 0x10f5)) {
        // 0x0561 - 0x0586: ARMENIAN LETTERS
        // 0x10D0 - 0x10f5: GEORGIAN LETTERS
        return cp - 0x30;
    }
    if (0x0451 <= cp && cp <= 0x045f) {
        // 0x0451 - 0x045f: CYRILLIC LETTERS
        return cp - 0x50;
    }
    if ((0x0100 <= cp && cp <= 0x012f)
     || (0x0132 <= cp && cp <= 0x0137)
     || (0x014a <= cp && cp <= 0x0177)
     || (0x0182 <= cp && cp <= 0x0185)
     || (0x01a0 <= cp && cp <= 0x01a5)
     || (0x01de <= cp && cp <= 0x01ef)
     || (0x01fa <= cp && cp <= 0x0217)
     || (0x03e2 <= cp && cp <= 0x03ef)
     || (0x0460 <= cp && cp <= 0x0481)
     || (0x0490 <= cp && cp <= 0x04bf)
     || (0x04d0 <= cp && cp <= 0x04eb)
     || (0x04ee <= cp && cp <= 0x04f9)
     || (0x1e00 <= cp && cp <= 0x1e95)
     || (0x1ea0 <= cp && cp <= 0x1ef9)) {
        return cp & 0xfffe;
    }
    if ((0x0139 <= cp && cp <= 0x0148)
     || (0x0179 <= cp && cp <= 0x017e)
     || (0x01b3 <= cp && cp <= 0x01b6)
     || (0x01cd <= cp && cp <= 0x01dc)) {
        return (cp - 1) | 0x0001;
    }
    if ((0x1f00 <= cp && cp <= 0x1f15)
     || (0x1f20 <= cp && cp <= 0x1f45)
     || (0x1f51 == cp || cp == 0x1f53)
     || (0x1f55 == cp || cp == 0x1f57)
     || (0x1f60 <= cp && cp <= 0x1f67)
     || (0x1f80 <= cp && cp <= 0x1fa7)
     || (0x1fb0 <= cp && cp <= 0x1fb1)
     || (0x1fd0 <= cp && cp <= 0x1fd1)
     || (0x1fe0 <= cp && cp <= 0x1fe1)) {
        // GREEK LETTERS
        return cp | 0x0008;
    }
    switch (cp) {
        case 0x00ff :
            return 0x0178;
        case 0x0131 :
            return 0x0049;
        case 0x0188 :
        case 0x018c :
        case 0x0192 :
        case 0x0199 :
        case 0x01a8 :
        case 0x01ad :
        case 0x01b0 :
        case 0x01b9 :
        case 0x01bd :
        case 0x01f5 :
        case 0x04c2 :
        case 0x04c4 :
        case 0x04c8 :
        case 0x04cc :
            return cp - 1;
        case 0x01c6 :
        case 0x01c9 :
        case 0x01cc :
        case 0x01f3 :
            return cp - 2;
        case 0x0253 :
            return 0x0181;
        case 0x0254 :
            return 0x0186;
        case 0x0257 :
            return 0x018a;
        case 0x0258 :
            return 0x018e;
        case 0x0259 :
            return 0x018f;
        case 0x025b :
            return 0x0190;
        case 0x0260 :
            return 0x0193;
        case 0x0263 :
            return 0x0194;
        case 0x0268 :
            return 0x0197;
        case 0x0269 :
            return 0x0196;
        case 0x026f :
            return 0x019c;
        case 0x0272 :
            return 0x019d;
        case 0x0275 :
            return 0x019f;
        case 0x0283 :
            return 0x01a9;
        case 0x0288 :
            return 0x01ae;
        case 0x028a :
            return 0x01b1;
        case 0x028b :
            return 0x01b2;
        case 0x0292 :
            return 0x01b7;
        case 0x03ac :
            return 0x0386;
        case 0x03ad :
            return 0x0388;
        case 0x03ae :
            return 0x0389;
        case 0x03af :
            return 0x038a;
        case 0x03cc :
            return 0x038c;
        case 0x03cd :
            return 0x038e;
        case 0x03ce :
            return 0x038f;
        default :
            return cp;
    }
}

// since NEXT_RELEASE
inline std::string
_to_u8upper(std::string const& value)
{
    std::pair<bool, std::size_t> num_chars = _utf8_length(value);
    if (!num_chars.first) {
        return _to_upper(value);
    }
    std::string res;
    std::size_t i = 0;
    for (std::size_t n = 0; n < num_chars.second; ++n) {
        std::size_t cp_size = _utf8_codepoint_size(_char_to_u8(value[i]));
        u32char cp = 0;
        switch (cp_size) {
            case 1:
                cp = (_char_to_u32(value[i]) & ~_utf8_1b_mask);
                break;
            case 2:
                cp = ((_char_to_u32(value[i    ]) & ~_utf8_2b_mask) <<  6)
                   |  (_char_to_u32(value[i + 1]) & ~_utf8_ct_mask);
                break;
            case 3:
                cp = ((_char_to_u32(value[i    ]) & ~_utf8_3b_mask) << 12)
                   | ((_char_to_u32(value[i + 1]) & ~_utf8_ct_mask) <<  6)
                   |  (_char_to_u32(value[i + 2]) & ~_utf8_ct_mask);
                break;
            case 4:
                cp = ((_char_to_u32(value[i    ]) & ~_utf8_4b_mask) << 18)
                   | ((_char_to_u32(value[i + 1]) & ~_utf8_ct_mask) << 12)
                   | ((_char_to_u32(value[i + 2]) & ~_utf8_ct_mask) <<  6)
                   |  (_char_to_u32(value[i + 3]) & ~_utf8_ct_mask);
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
            res += _u32_to_char((cp >> 6)            | 0xc0);
            res += _u32_to_char((cp & 0x3f)          | 0x80);
        } else if (cp < 0x10000) {
            // three octets
            res += _u32_to_char((cp >> 12)           | 0xe0);
            res += _u32_to_char(((cp >> 6) & 0x3f)   | 0x80);
            res += _u32_to_char((cp & 0x3f)          | 0x80);
        } else {
            // four octets
            res += _u32_to_char((cp >> 18)           | 0xf0);
            res += _u32_to_char(((cp >> 12) & 0x3f)  | 0x80);
            res += _u32_to_char(((cp >>  6) & 0x3f)  | 0x80);
            res += _u32_to_char((cp & 0x3f)          | 0x80);
        }
    }
    return res;
}
// ----------------------------------------------------------------------------

// -- translations support ----------------------------------------------------
// since v1.7.1
typedef std::map<std::string, std::string> LanguagePack;

inline std::string
_tr(LanguagePack const& pack, std::string const& lang)
{
    LanguagePack::const_iterator it;
    if (!lang.empty()) {
        it = pack.find(lang);
        if (it != pack.end()) {
            return it->second;
        }
    }
    it = pack.find(std::string());
    if (it != pack.end()) {
        return it->second;
    }
    return std::string();
}
// ----------------------------------------------------------------------------

inline std::pair<std::size_t, std::size_t>
_get_terminal_size(bool default_values = false)
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

inline void
_limit_to_min(std::size_t& value, std::size_t to) _ARGPARSE_NOEXCEPT
{
    if (value < to) {
        value = to;
    }
}

inline void
_limit_to_max(std::size_t& value, std::size_t to) _ARGPARSE_NOEXCEPT
{
    if (value > to) {
        value = to;
    }
}

inline void
_ltrim(std::string& str)
{
#ifdef _ARGPARSE_CXX_11
    str.erase(str.begin(),
              std::find_if(str.begin(), str.end(),
                           [] (unsigned char c) { return !std::isspace(c); }));
#else
    std::string::iterator it = str.begin();
    for ( ; it != str.end(); ++it) {
        if (!std::isspace(static_cast<unsigned char>(*it))) {
            break;
        }
    }
    str.erase(str.begin(), it);
#endif  // C++11+
}

inline void
_rtrim(std::string& str)
{
#ifdef _ARGPARSE_CXX_11
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [] (unsigned char c)
    { return !std::isspace(c); }).base(), str.end());
#else
    std::string::reverse_iterator it = str.rbegin();
    for ( ; it != str.rend(); ++it) {
        if (!std::isspace(static_cast<unsigned char>(*it))) {
            break;
        }
    }
    str.erase(it.base(), str.end());
#endif  // C++11+
}

inline void
_trim(std::string& str)
{
    _ltrim(str);
    _rtrim(str);
}

inline std::string
_trim_copy(std::string const& str)
{
    std::string res = str;
    _trim(res);
    return res;
}

template <class T>
std::string
_to_string(T const& value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

inline std::string
_file_name(std::string const& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

inline bool
_have_quotes(std::string const& str)
{
    return str.size() > 1 && str.at(0) == str.at(str.size() - 1)
            && (str.at(0) == '\'' || str.at(0) == '\"');
}

inline void
_resolve_conflict(std::string const& str, std::vector<std::string>& values)
{
    std::vector<std::string>::iterator it
            = std::find(values.begin(), values.end(), str);
    if (it != values.end()) {
        values.erase(it);
    }
}

inline void
_resolve_conflict(std::vector<std::string> const& vec,
                  std::vector<std::string>& values)
{
    for (std::size_t i = 0; i < vec.size(); ++i) {
        _resolve_conflict(vec[i], values);
    }
}

#ifdef _ARGPARSE_CXX_11
template <class T = std::string>
typename std::enable_if<std::is_constructible<std::string, T>::value, T>::type
_remove_quotes(std::string const& str)
{
    return _have_quotes(str) ? T(str).substr(1, str.size() - 2) : T(str);
}
#else
inline std::string
_remove_quotes(std::string const& str)
{
    return _have_quotes(str) ? str.substr(1, str.size() - 2) : str;
}
#endif  // C++11+

inline bool
_contains_substr(std::string const& str, std::string const& substr)
{
#ifdef _ARGPARSE_CXX_23
    return str.contains(substr);
#else
    return str.find(substr) != std::string::npos;
#endif  // C++23+
}

inline std::string
_get_punct(std::string const& str)
{
    std::string res;
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (std::ispunct(static_cast<unsigned char>(str.at(i)))) {
            res += str.at(i);
        }
    }
    return res;
}

inline std::string
_replace(std::string str, char old, std::string const& value)
{
    std::string::size_type pos = str.find(old);
    while (pos != std::string::npos) {
        str.replace(pos, 1, value);
        pos = str.find(old, pos + value.size());
    }
    return str;
}

inline std::string
_replace(std::string str, std::string const& old, std::string const& value)
{
    std::string::size_type pos = str.find(old);
    while (pos != std::string::npos) {
        str.replace(pos, old.length(), value);
        pos = str.find(old, pos + value.size());
    }
    return str;
}

#ifdef _ARGPARSE_CXX_11
inline std::string
_replace(std::string const& str,
         std::function<bool(unsigned char)> func, std::string const& value)
{
    std::string res;
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str.at(i);
        if (func(static_cast<unsigned char>(c))) {
            res += value;
        } else {
            res += c;
        }
    }
    return res;
}
#endif  // C++11+

inline bool
_starts_with(std::string const& str, std::string const& value)
{
#ifdef _ARGPARSE_CXX_20
    return str.starts_with(value);
#else
    return str.compare(0, value.size(), value) == 0;
#endif  // C++20+
}

template <class T>
bool
_is_value_exists(T const& value, std::vector<T> const& vec)
{
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (vec.at(i) == value) {
            return true;
        }
    }
    return false;
}

inline bool
_is_value_exists(char value, std::string const& str)
{
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str.at(i) == value) {
            return true;
        }
    }
    return false;
}

template <class T>
void
_insert_vector_to_end(std::deque<T> const& from, std::deque<T>& to)
{
    if (!from.empty()) {
        to.insert(to.end(), from.begin(), from.end());
    }
}

template <class T>
void
_insert_vector_to_end(std::deque<T> const& from, std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(to.end(), from.begin(), from.end());
    }
}

template <class T>
void
_insert_vector_to_end(std::vector<T> const& from, std::vector<T>& to)
{
    if (!from.empty()) {
        to.insert(to.end(), from.begin(), from.end());
    }
}

template <class T>
void
_move_vector_insert_to_end(std::deque<T>& from, std::vector<T>& to)
{
#ifdef _ARGPARSE_CXX_11
    if (!from.empty()) {
        to.insert(to.end(),
                  std::make_move_iterator(from.begin()),
                  std::make_move_iterator(from.end()));
    }
#else
    _insert_vector_to_end(from, to);
#endif  // C++11+
}

template <class T>
void
_move_vector_insert_to_end(std::vector<T>& from, std::vector<T>& to)
{
#ifdef _ARGPARSE_CXX_11
    if (!from.empty()) {
        to.insert(to.end(),
                  std::make_move_iterator(from.begin()),
                  std::make_move_iterator(from.end()));
    }
#else
    _insert_vector_to_end(from, to);
#endif  // C++11+
}

template <class T>
void
_move_vector_insert_to(std::vector<T>& from, std::vector<T>& to, std::size_t i)
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
_move_vector_replace_at(std::vector<T>& from, std::vector<T>& to, std::size_t i)
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

inline std::string
_flag_name(std::string const& str)
{
    std::string res = str;
    char prefix = res.at(0);
    std::string::iterator it = res.begin();
    for ( ; it != res.end(); ++it) {
        if (*it != prefix) {
            break;
        }
    }
    res.erase(res.begin(), it);
    return res;
}

inline bool
_is_flag_correct(std::string const& str, bool is_optional)
{
    std::string trimmed = _trim_copy(str);
    if (trimmed.size() != str.size()) {
        return false;
    }
    if (is_optional) {
        trimmed = _flag_name(trimmed);
    }
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str.at(i) == '-' || str.at(i) == '_') {
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(str.at(i)))
                || std::ispunct(static_cast<unsigned char>(str.at(i)))) {
            return false;
        }
    }
    return true;
}

inline std::vector<std::string>
_help_flags(std::string const& prefix_chars)
{
    char prefix = _is_value_exists(_prefix_char, prefix_chars)
            ? _prefix_char : prefix_chars.at(0);
#ifdef _ARGPARSE_CXX_11
    return { std::string(1, prefix) + "h", std::string(2, prefix) + "help" };
#else
    return _make_vector(std::string(1, prefix) + "h",
                        std::string(2, prefix) + "help");
#endif  // C++11+
}

inline bool
_is_negative_number(std::string const& str)
{
    double value;
    std::stringstream ss(str);
    ss >> value;
    return !ss.fail() && ss.eof() && value < 0;
}

inline bool
_is_optional(std::string const& arg,
             std::string const& prefix_chars,
             bool have_negative_args,
             bool was_pseudo_arg)
{
    return _is_value_exists(arg.at(0), prefix_chars) && !was_pseudo_arg
            && (have_negative_args || !_is_negative_number(arg));
}

inline bool
_not_optional(std::string const& arg,
              std::string const& prefix_chars,
              bool have_negative_args,
              bool was_pseudo_arg)
{
    return !_is_value_exists(arg.at(0), prefix_chars) || was_pseudo_arg
            || (!have_negative_args && _is_negative_number(arg));
}

inline std::pair<bool, std::string>
_make_no_flag(std::string const& str)
{
    char prefix = str.at(0);
    std::string::const_iterator it = str.begin();
    for ( ; it != str.end(); ++it) {
        if (*it != prefix) {
            break;
        }
    }
    std::pair<bool, std::string> res
            = std::make_pair(std::distance(str.begin(), it) > 1, str);
    if (res.first) {
        res.second.insert(static_cast<std::string::size_type>(2), "no-");
    }
    return res;
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
_store_value_to(std::string& value, std::vector<std::string>& res,
                bool force = false)
{
    if (!value.empty() || force) {
        res.push_back(value);
        value.clear();
    }
}

inline std::vector<std::string>
_split(std::string const& str, char delim,
       bool force = false, bool add_delim = false)
{
    std::vector<std::string> res;
    std::string value;
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str.at(i);
        if (c == delim) {
            _store_value_to(value, res, force);
            if (add_delim) {
                value = std::string(1, delim);
                _store_value_to(value, res, true);
            }
        } else {
            value += c;
        }
    }
    _store_value_to(value, res);
    return res;
}

inline std::vector<std::string>
_split_whitespace(std::string const& str, bool force = false)
{
    std::vector<std::string> res;
    std::string value;
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str.at(i);
        if (std::isspace(static_cast<unsigned char>(c))) {
            _store_value_to(value, res, force);
        } else {
            value += c;
        }
    }
    _store_value_to(value, res);
    return res;
}

inline std::pair<std::string, std::string>
_split_delimiter(std::string const& str, char delim)
{
    std::string::size_type pos = str.find(delim);
    if (pos != std::string::npos) {
        return std::make_pair(str.substr(0, pos), str.substr(pos + 1));
    } else {
        return std::make_pair(str, std::string());
    }
}

inline std::vector<std::string>
_split_equal(std::string const& str, std::string const& prefix)
{
#ifdef _ARGPARSE_CXX_11
    std::string::size_type pos = _is_value_exists(_equal, prefix)
            ? str.find(_equal, static_cast<std::string::size_type>(
                         std::distance(str.begin(),
                                       std::find_if(str.begin(),
                                                    str.end(), [] (char c)
    { return c != _equal; }))))
            : str.find(_equal);
    if (pos != std::string::npos) {
        return { str.substr(0, pos), str.substr(pos + 1) };
    } else {
        return { str };
    }
#else
    std::string::size_type pos;
    if (_is_value_exists(_equal, prefix)) {
        std::string::const_iterator it = str.begin();
        for ( ; it != str.end(); ++it) {
            if (*it != _equal) {
                break;
            }
        }
        pos = str.find(_equal, static_cast<std::string::size_type>(
                           std::distance(str.begin(), it)));
    } else {
        pos = str.find(_equal);
    }
    if (pos != std::string::npos) {
        return _make_vector(str.substr(0, pos), str.substr(pos + 1));
    } else {
        return _make_vector(str);
    }
#endif  // C++11+
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
                   || std::ispunct(static_cast<unsigned char>(
                                       value.at(value.size() - 1)))) {
            quotes.push_back(c);
        }
    }
}

inline std::vector<std::string>
_split_to_args(std::string const& str, std::ostream& err = std::cerr)
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

inline bool
_string_to_bool(std::string const& str) _ARGPARSE_NOEXCEPT
{
    return !str.empty();
}

inline std::string
_bool_to_string(bool value)
{
    return value ? "true" : "false";
}

inline std::string
_bool_to_string(std::string const& str)
{
    return _bool_to_string(_string_to_bool(str));
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
    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::string val = vec.at(i);
        if (quotes.empty() && replace_space && !_have_quotes(val)) {
            val = _replace(val, _space, "\\ ");
        }
        _append_value_to(quotes + val + quotes, res, separator);
    }
    return begin + (res.empty() ? none : res) + end;
}

inline std::string
_vector_to_string(std::vector<std::string>::const_iterator begvec,
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

inline std::string
_matrix_to_string(std::vector<std::string> const& values,
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

inline std::string
_ignore_explicit(std::string const& arg, std::string const& value)
{
    return "argument " + arg + ": ignored explicit argument '" + value + "'";
}

inline void
_format_output_func(std::size_t indent, std::size_t width,
                    std::vector<std::string>& res, std::string& value,
                    std::string const& str)
{
    std::size_t value_size = _utf8_length(value).second;
    if (value_size > indent
            && value_size + 1 + _utf8_length(str).second > width) {
        _store_value_to(value, res);
    }
    value_size = _utf8_length(value).second;
    if (value_size < indent) {
        value.resize(value.size() + indent - value_size, _space);
        value += str;
    } else {
        value += _spaces + str;
    }
}

inline std::string
_format_output(std::string const& head, std::string const& body,
               std::size_t interlayer, std::size_t indent, std::size_t width,
               char delimiter = '\n')
{
    std::vector<std::string> res;
    std::string value = head;
    if (_utf8_length(value).second + interlayer > indent) {
        _store_value_to(value, res);
    }
    std::vector<std::string> split_str = _split(body, '\n', true);
    for (std::size_t i = 0; i < split_str.size(); ++i) {
        std::string const& str = split_str.at(i);
        if (delimiter == '\n') {
            _format_output_func(indent, width, res, value, str);
        } else if (str.empty()) {
            value.resize(value.size() + indent - _utf8_length(value).second,
                         _space);
            _store_value_to(value, res, true);
        } else {
            std::vector<std::string> sub_split_str
                    = _split(str, delimiter, true);
            for (std::size_t j = 0; j < sub_split_str.size(); ++j) {
                std::string const& sub = sub_split_str.at(j);
                _format_output_func(indent, width, res, value, sub);
            }
            _store_value_to(value, res);
        }
    }
    _store_value_to(value, res);
    return _vector_to_string(res, "\n");
}

inline std::string
_help_formatter(std::string const& head,
                _HelpFormatter const& formatter,
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
            std::string const& line = lines.at(i);
            std::size_t value_size = _utf8_length(value).second;
            if (value_size < indent) {
                value.resize(value.size() + indent - value_size, _space);
            }
            value += line;
            _store_value_to(value, res, true);
        }
    }
    _store_value_to(value, res);
    return _vector_to_string(res, "\n");
}

inline void
_print_raw_text_formatter(_HelpFormatter const& formatter,
                          std::string const& text,
                          std::size_t width,
                          std::ostream& os,
                          std::string const& begin = "\n",
                          std::size_t indent = 0,
                          std::string const& end = std::string())
{
    if (!text.empty()) {
        os << begin
           << formatter._fill_text(text, width, indent) << end << std::endl;
    }
}

inline std::string
_filled_string(std::string const& str, std::size_t limit, char filler = '-')
{
    if (str.size() + 2 >= limit) {
        return _spaces + str;
    }
    std::string res = std::string((limit - str.size() - 2) / 2, filler)
                      + _spaces + str + _spaces;
    res.resize(limit, filler);
    return res;
}

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
#endif  // __GNUC__ >= 4.7
#else
    return std::string();
#endif  // _MSC_VER
}

class Type
{
public:
#ifdef _ARGPARSE_CXX_11
    template <class T, typename std::enable_if<
                  std::is_same<std::string, T>::value
#ifdef _ARGPARSE_CXX_17
                  || std::is_same<std::string_view, T>::value
#endif  // C++17+
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
                  !std::is_same<std::string, T>::value
#ifdef _ARGPARSE_CXX_17
                  && !std::is_same<std::string_view, T>::value
#endif  // C++17+
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
#ifdef _ARGPARSE_CXX_17
                  std::is_same<std::string_view, T>::value ||
#endif  // C++17+
                  std::is_same<std::string, T>::value>::type* = nullptr>
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
        auto str = _get_type_name<T>();
        str = _replace(str, "__1::", std::string());
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
                  !std::is_same<std::string, T>::value
#ifdef _ARGPARSE_CXX_17
                  && !std::is_same<std::string_view, T>::value
#endif  // C++17+
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
        std::string str = _get_type_name<T>();
        str = _replace(str, "__1::", std::string());
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
    tuple_type(std::string& s)
    {
        return s;
    }

    template <std::size_t N, class T, class... Ts>
    static std::string&
    tuple_type(std::string& s)
    {
        if (!s.empty()) {
            s += ", ";
        }
        s += name<T>();
        return tuple_type<N - 1, Ts...>(s);
    }

    template <class... Ts>
    static std::string
    tuple_as_string(type_tag<std::tuple<Ts...> >)
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
    Value()
        : m_value(),
          m_has_value(false)
    { }

    Value(Value const& orig)
        : m_value(orig.m_value),
          m_has_value(orig.m_has_value)
    { }

    explicit Value(T const& orig)
        : m_value(orig),
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

    inline Value& operator =(T const& rhs)
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

    explicit Value(T&& orig) _ARGPARSE_NOEXCEPT
        : m_value(std::move(orig)),
          m_has_value(true)
    { }

    inline Value& operator =(Value&& rhs) _ARGPARSE_NOEXCEPT
    {
        if (this != &rhs) {
            m_value     = std::move(rhs.m_value);
            m_has_value = std::move(rhs.m_has_value);
        }
        return *this;
    }

    inline Value& operator =(T&& rhs) _ARGPARSE_NOEXCEPT
    {
        m_value     = std::move(rhs);
        m_has_value = true;
        return *this;
    }
#endif  // C++11+

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
        throw TypeError("type_name mismatch: expected '" + expected() + "'"
                        + ", received '" + received + "'");
    }
}
}  // namespace detail

/*!
 *  \brief _ConflictResolver class
 */
struct _ConflictResolver
{
    virtual ~_ConflictResolver() _ARGPARSE_NOEXCEPT { }

    virtual void check_conflict_arg(Argument const* arg) = 0;
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
    };

    enum Type _ARGPARSE_ENUM_TYPE(uint8_t)
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
          m_dest(detail::_make_vector(std::string())),
#endif  // C++11+
          m_post_trigger(_ARGPARSE_NULLPTR),
          m_required()
    {
        m_help[std::string()] = std::string();
    }

#ifdef _ARGPARSE_CXX_11
    explicit
    Argument(std::vector<std::string>&& flags,
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
    }
#endif  // C++11+

    static detail::shared_ptr<Argument>
    make_argument(std::vector<std::string> const& flags,
                  std::string const& name,
                  Type type)
    {
        return detail::make_shared<Argument>(Argument(flags, name, type));
    }

#ifdef _ARGPARSE_CXX_11
    static detail::shared_ptr<Argument>
    make_argument(std::vector<std::string>&& flags,
                  std::string&& name,
                  Type type)
    {
        return detail::make_shared<Argument>(
                    Argument(std::move(flags), std::move(name), type));
    }
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
    Argument(std::initializer_list<std::string> flags)
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
    Argument(std::string const& flag)
        : m_flags(detail::_make_vector(flag)),
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
          m_dest(detail::_make_vector(std::string())),
          m_post_trigger(_ARGPARSE_NULLPTR),
          m_required()
    {
        m_help[std::string()] = std::string();
    }

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flag1 First flag
     *  \param flag2 Second flag
     *
     *  \return Argument object
     */
    explicit
    Argument(std::string const& flag1, std::string const& flag2)
        : m_flags(detail::_make_vector(flag1, flag2)),
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
          m_dest(detail::_make_vector(std::string())),
          m_post_trigger(_ARGPARSE_NULLPTR),
          m_required()
    {
        m_help[std::string()] = std::string();
    }

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
    Argument(std::string const& flag1, std::string const& flag2,
             std::string const& flag3)
        : m_flags(detail::_make_vector(flag1, flag2, flag3)),
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
          m_dest(detail::_make_vector(std::string())),
          m_post_trigger(_ARGPARSE_NULLPTR),
          m_required()
    {
        m_help[std::string()] = std::string();
    }

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
    Argument(std::string const& flag1, std::string const& flag2,
             std::string const& flag3, std::string const& flag4)
        : m_flags(detail::_make_vector(flag1, flag2, flag3, flag4)),
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
          m_dest(detail::_make_vector(std::string())),
          m_post_trigger(_ARGPARSE_NULLPTR),
          m_required()
    {
        m_help[std::string()] = std::string();
    }
#endif  // C++11+

    /*!
     *  \brief Construct argument object with parsed arguments
     *
     *  \param flags Argument flags
     *
     *  \return Argument object
     */
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
    explicit
    Argument(std::vector<std::string> const& flags)
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
          m_dest(detail::_make_vector(std::string())),
#endif  // C++11+
          m_post_trigger(_ARGPARSE_NULLPTR),
          m_required()
    {
        m_help[std::string()] = std::string();
    }

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
        m_help[std::string()] = std::string();
    }

#ifdef _ARGPARSE_CXX_11
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
        m_help[std::string()] = std::string();
    }
#endif  // C++11+

    /*!
     *  \brief Copy argument object from another argument
     *
     *  \param rhs Argument object to copy
     *
     *  \return Current argument reference
     */
    inline Argument& operator =(Argument const& rhs)
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
    /*!
     *  \brief Move argument object from another argument
     *
     *  \param rhs Argument object to move
     *
     *  \return Current argument reference
     */
    inline Argument& operator =(Argument&& rhs) _ARGPARSE_NOEXCEPT
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
    inline Argument& action(std::string const& value)
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

    /*!
     *  \brief Set argument 'action' value (default: argparse::store)
     *
     *  \param value Action value
     *
     *  \return Current argument reference
     */
    inline Argument& action(Action value)
    {
        prepare_action(value);
        switch (value) {
            case argparse::store_true :
                if (!m_default.has_value()) {
                    m_default.clear();
                }
                // fallthrough
            case argparse::BooleanOptionalAction :
                m_const.clear("1");
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case argparse::store_false :
                if (!m_default.has_value()) {
                    m_default.clear("1");
                }
                // fallthrough
            case argparse::store_const :
            case argparse::append_const :
                m_const.clear();
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case argparse::version :
                this->help("show program's version number and exit");
                // fallthrough
            case argparse::help :
                check_required();
                // fallthrough
            case argparse::count :
                m_const.clear();
                m_nargs = NARGS_NUM;
                m_nargs_str = "0";
                m_num_args = 0;
                m_choices.clear();
                break;
            case argparse::store :
            case argparse::append :
            case argparse::extend :
                m_const.clear();
                if (m_num_args == 0) {
                    m_nargs = NARGS_DEF;
                    m_nargs_str = "1";
                    m_num_args = 1;
                }
                break;
            case argparse::language :
                check_required();
                m_const.clear();
                m_nargs = NARGS_DEF;
                m_nargs_str = "1";
                m_num_args = 1;
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

    /*!
     *  \brief Set argument 'nargs' argparse::REMAINDER value
     *
     *  \param value argparse::REMAINDER
     *
     *  \return Current argument reference
     */
    inline Argument& nargs(_REMAINDER value)
    {
        if (!(action() & detail::_store_action)
                || value != argparse::REMAINDER) {
            throw TypeError("got an unexpected keyword argument 'nargs'");
        }
        m_nargs = REMAINDING;
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
            m_const = value;
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
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                !std::is_constructible<std::string, T>::value>::type* = nullptr>
    Argument& const_value(T const& value)
#else
    template <class T>
    Argument& const_value(
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
    inline Argument& default_value(std::string const& value)
    {
        m_default = value;
        return *this;
    }

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
    Argument& default_value(T const& value)
#else
    template <class T>
    Argument& default_value(
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
    inline Argument& default_value(_SUPPRESS value)
    {
        if (value != argparse::SUPPRESS) {
            throw TypeError("got an unexpected keyword argument 'default'");
        }
        if (action() == argparse::store_false) {
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
        m_implicit = value;
        return *this;
    }

    /*!
     *  \brief Set custom argument 'implicit' value
     *
     *  \param value Implicit value
     *
     *  \return Current argument reference
     */
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                !std::is_constructible<std::string, T>::value>::type* = nullptr>
    Argument& implicit_value(T const& value)
#else
    template <class T>
    Argument& implicit_value(
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
        m_type_name.clear(value);
        return *this;
    }

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choice value
     *
     *  \since v1.7.1
     *
     *  \return Current argument reference
     */
    inline Argument& choice(std::string const& value)
    {
        if (!(action() & (detail::_store_action | argparse::language))) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        std::vector<std::string> values;
        values.push_back(value);
#ifdef _ARGPARSE_CXX_11
        m_choices = std::move(values);
#else
        m_choices = values;
#endif  // C++11+
        return *this;
    }

    /*!
     *  \brief Set argument 'choices' value. Each character is a separate choice
     *
     *  \param value Choices value
     *
     *  \return Current argument reference
     */
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
    Argument& choices(std::string const& value)
    {
        if (!(action() & (detail::_store_action | argparse::language))) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        std::vector<std::string> values;
        values.reserve(value.size());
        for (std::size_t i = 0; i < value.size(); ++i) {
            values.push_back(std::string(1, value.at(i)));
        }
#ifdef _ARGPARSE_CXX_11
        m_choices = std::move(values);
#else
        m_choices = values;
#endif  // C++11+
        return *this;
    }

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
    Argument& choices(std::string const& value1,
                      std::string const& value2, Args... args)
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
    Argument& choices(std::initializer_list<std::string> value)
    {
        return choices(std::vector<std::string>{ value });
    }
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
    inline Argument& choices(std::string const& value1,
                             std::string const& value2)
    {
        return choices(detail::_make_vector(value1, value2));
    }

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
    inline Argument& choices(std::string const& value1,
                             std::string const& value2,
                             std::string const& value3)
    {
        return choices(detail::_make_vector(value1, value2, value3));
    }

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
    inline Argument& choices(std::string const& value1,
                             std::string const& value2,
                             std::string const& value3,
                             std::string const& value4)
    {
        return choices(detail::_make_vector(value1, value2, value3, value4));
    }
#endif  // C++11+

    /*!
     *  \brief Set argument 'choices' value
     *
     *  \param value Choices value
     *
     *  \return Current argument reference
     */
    inline Argument& choices(std::vector<std::string> const& value)
    {
        if (!(action() & (detail::_store_action | argparse::language))) {
            throw TypeError("got an unexpected keyword argument 'choices'");
        }
        m_choices = value;
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
     *  \brief Set argument 'help' message for selected language (default: "")
     *
     *  \param value Help message
     *  \param lang Language value
     *
     *  \return Current argument reference
     */
    inline Argument& help(std::string const& value,
                          std::string const& lang = std::string())
    {
        if (lang.empty()) {
            m_help_type.clear();
        }
        m_help[lang] = value;
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
     *  \brief Set argument 'version' for arguments with 'version' action
     *
     *  \param value Version value
     *
     *  \return Current argument reference
     */
    inline Argument& version(std::string const& value)
    {
        if (action() == argparse::version) {
            m_version = value;
        } else {
            throw TypeError("got an unexpected keyword argument 'version'");
        }
        return *this;
    }

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar values
     *
     *  \return Current argument reference
     */
    inline Argument& metavar(std::vector<std::string> const& value)
    {
        if (!(action() & (detail::_store_const_action
                          | argparse::BooleanOptionalAction))) {
            throw TypeError("got an unexpected keyword argument 'metavar'");
        }
        m_metavar = value;
        return *this;
    }

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
    Argument& metavar(std::string const& value, Args... args)
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
    Argument& metavar(std::initializer_list<std::string> value)
    {
        return metavar(std::vector<std::string>{ value });
    }
#else
    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value Metavar value
     *
     *  \return Current argument reference
     */
    inline Argument& metavar(std::string const& value)
    {
        return metavar(detail::_make_vector(value));
    }

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *
     *  \return Current argument reference
     */
    inline Argument& metavar(std::string const& value1,
                             std::string const& value2)
    {
        return metavar(detail::_make_vector(value1, value2));
    }

    /*!
     *  \brief Set argument 'metavar' value
     *
     *  \param value1 First value
     *  \param value2 Second value
     *  \param value3 Third value
     *
     *  \return Current argument reference
     */
    inline Argument& metavar(std::string const& value1,
                             std::string const& value2,
                             std::string const& value3)
    {
        return metavar(detail::_make_vector(value1, value2, value3));
    }

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
    inline Argument& metavar(std::string const& value1,
                             std::string const& value2,
                             std::string const& value3,
                             std::string const& value4)
    {
        return metavar(detail::_make_vector(value1, value2, value3, value4));
    }
#endif  // C++11+

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
        m_dest.front() = value;
        return *this;
    }

#ifdef _ARGPARSE_CXX_11
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
        if (action() & (argparse::version | argparse::help)) {
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
        if (action() & (argparse::version | argparse::help)) {
            throw TypeError("got an unexpected keyword argument 'handle'");
        }
        m_handle = [func] (std::string const&) { func(); };
        return *this;
    }
#endif  // C++11+

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
    inline std::string const& help() const
    {
        return detail::_map_at(m_help, std::string());
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

    /*!
     *  \brief Get argument 'metavar' value
     *
     *  \return Argument 'metavar' value
     */
    inline std::string metavar() const
    {
        std::string res = detail::_vector_to_string(m_metavar(), ", ");
        return m_metavar().size() > 1 ? ("[" + res + "]") : res;
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

private:
#ifdef _ARGPARSE_CXX_11
    inline void handle(std::string const& str) const
    {
        if (m_handle) {
            m_handle(detail::_remove_quotes(str));
        }
    }
#endif  // C++11+

    inline void validate() const
    {
        if (m_type == Positional && m_flags.empty() && dest().empty()) {
            throw TypeError("missing 1 required positional argument: 'dest'");
        }
#ifdef _ARGPARSE_CXX_11
        if (std::any_of(m_flags.begin(), m_flags.end(),
                        [] (std::string const& flag)
        { return flag == detail::_pseudo_arg; }) && dest().empty()) {
            throw ValueError("dest= is required for options like '--'");
        }
#else
        for (std::size_t i = 0; i < m_flags.size(); ++i) {
            if (m_flags.at(i) == detail::_pseudo_arg && dest().empty()) {
                throw ValueError("dest= is required for options like '--'");
            }
        }
#endif  // C++11+
    }

    inline void check_required() const
    {
        if (m_type == Positional) {
            // version, help and language actions cannot be positional
            throw TypeError("got an unexpected keyword argument 'required'");
        }
    }

    inline void prepare_action(Action value)
    {
#ifdef _ARGPARSE_CXX_11
        if (action() & (argparse::version | argparse::help)) {
            m_handle = nullptr;
        }
#endif  // C++11+
        if (!(value & detail::_store_const_action)) {
            m_metavar.clear();
        }
        if (m_type == Optional && value == argparse::BooleanOptionalAction) {
            make_no_flags();
            if (m_post_trigger) {
                m_post_trigger->check_conflict_arg(this);
            }
        } else {
            m_all_flags = m_flags;
        }
    }

    inline void make_no_flags()
    {
        m_all_flags.clear();
        for (std::size_t i = 0; i < m_flags.size(); ++i) {
            std::string const& flag = m_flags.at(i);
            m_all_flags.push_back(flag);
            std::pair<bool, std::string> no_flag = detail::_make_no_flag(flag);
            if (no_flag.first) {
                m_all_flags.push_back(no_flag.second);
            }
        }
    }

    inline std::string usage(_HelpFormatter const& formatter) const
    {
        std::string res;
        if (m_type == Optional) {
            if (action() == argparse::BooleanOptionalAction) {
                for (std::size_t i = 0; i < flags().size(); ++i) {
                    detail::_append_value_to(flags().at(i), res, " | ");
                }
            } else {
                res += m_flags.front();
            }
        }
        process_nargs_suffix(res, formatter);
        return res;
    }

    inline std::string flags_to_string(_HelpFormatter const& formatter) const
    {
        std::string res;
        if (m_type == Optional) {
            for (std::size_t i = 0; i < flags().size(); ++i) {
                detail::_append_value_to(flags().at(i), res, ", ");
                process_nargs_suffix(res, formatter);
            }
        } else {
            std::vector<std::string> names = get_argument_name(formatter);
            res += names.size() > 1
                    ? ("[" + detail::_vector_to_string(names, ", ") + "]")
                    : detail::_vector_to_string(names);
        }
        return res;
    }

    inline std::string get_choices() const
    {
        return m_choices.has_value()
                ? detail::_vector_to_string(choices(), ", ") : "None";
    }

    inline std::string get_const() const
    {
        return m_const.has_value() ? const_value() : "None";
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

    inline std::string const& get_dest() const _ARGPARSE_NOEXCEPT
    {
        return dest().empty() ? m_name : dest();
    }

    inline std::string get_metavar() const
    {
        return m_metavar.has_value() ? metavar() : "None";
    }

    inline std::string get_nargs() const
    {
        switch (m_nargs) {
            case NARGS_NUM :
            case ONE_OR_MORE :
            case ZERO_OR_ONE :
            case ZERO_OR_MORE :
                return m_nargs_str;
            case REMAINDING :
                return "...";
            default :
                return "None";
        }
    }

    inline std::string option_strings() const
    {
        return "[" + detail::_vector_to_string(flags(), ", ", "'")+ "]";
    }

    inline std::string get_required() const
    {
        return detail::_bool_to_string(required());
    }

    inline std::string get_type() const
    {
        return m_type_name.has_value() ? type_name() : "None";
    }

    inline std::string print(_HelpFormatter const& formatter,
                             std::size_t limit,
                             std::size_t width,
                             std::string const& lang) const
    {
        std::string help = formatter._get_help_string(this, lang);
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
            { "%(help)s",           [this] () { return this->help();        } },
            { "%(metavar)s",        [this] () { return get_metavar();       } },
            { "%(nargs)s",          [this] () { return get_nargs();         } },
            { "%(option_strings)s", [this] () { return option_strings();    } },
            { "%(required)s",       [this] () { return get_required();      } },
            { "%(type)s",           [this] () { return get_type();          } },
        };
        while (std::regex_search(help, match, r)) {
            text += match.prefix();
            auto specifier = std::string(match[0]);
            auto it = specifiers.find(specifier);
            text += (it != specifiers.end() ? it->second() : specifier);
            help = match.suffix();
        }
        text += help;
        std::swap(help, text);
#else
        help = detail::_replace(help, "%(choices)s", get_choices());
        help = detail::_replace(help, "%(const)s", get_const());
        help = detail::_replace(help, "%(default)s", get_default());
        help = detail::_replace(help, "%(dest)s", get_dest());
        help = detail::_replace(help, "%(metavar)s", get_metavar());
        help = detail::_replace(help, "%(nargs)s", get_nargs());
        help = detail::_replace(help, "%(option_strings)s", option_strings());
        help = detail::_replace(help, "%(required)s", get_required());
        help = detail::_replace(help, "%(type)s", get_type());
        help = detail::_replace(help, "%(help)s", this->help());
#endif  // C++11+
        return detail::_help_formatter("  " + flags_to_string(formatter),
                                       formatter, help, width, limit);
    }

    inline void process_nargs_suffix(std::string& res,
                                     _HelpFormatter const& formatter) const
    {
        if (!(action() & (detail::_store_action
                          | argparse::append_const
                          | argparse::language))) {
            return;
        }
        std::vector<std::string> names = get_argument_name(formatter);
        std::size_t names_size = names.size();
        if (names.size() > 1
                && (m_nargs != NARGS_NUM || names.size() != m_num_args)) {
            throw TypeError("length of metavar tuple does not match nargs");
        }
        if (names.size() == 1
                && m_nargs == NARGS_NUM && names.size() != m_num_args) {
            names.resize(m_num_args, names.front());
        }
        std::string name = names_size > 1
                ? ("[" + detail::_vector_to_string(names, ", ") + "]")
                : detail::_vector_to_string(names);
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
            case REMAINDING :
                res += "...";
                break;
            default :
                res += name;
                break;
        }
    }

    inline std::vector<std::string>
    get_argument_name(_HelpFormatter const& formatter) const
    {
        if (m_metavar.has_value()) {
            return m_metavar();
        }
#ifdef _ARGPARSE_CXX_11
        if (m_choices.has_value()) {
            return { "{" + detail::_vector_to_string(choices(), ",") + "}" };
        }
        return { m_type == Optional
                    ? formatter._get_default_metavar_for_optional(this)
                    : formatter._get_default_metavar_for_positional(this) };
#else
        if (m_choices.has_value()) {
            return detail::_make_vector(
                        "{" + detail::_vector_to_string(choices(), ",") + "}");
        }
        return detail::_make_vector(
                    m_type == Optional
                      ? formatter._get_default_metavar_for_optional(this)
                      : formatter._get_default_metavar_for_positional(this));
#endif  // C++11+
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
    detail::Value<_SUPPRESS> m_default_type;
    detail::Value<_SUPPRESS> m_help_type;
    Type        m_type;
    Nargs       m_nargs;
    std::size_t m_num_args;
    std::string m_nargs_str;
    detail::Value<std::string> m_const;
    detail::Value<std::string> m_default;
    detail::Value<std::string> m_implicit;
    detail::Value<std::string> m_type_name;
    detail::Value<std::vector<std::string> > m_choices;
    detail::LanguagePack m_help;
    detail::Value<std::string> m_version;
    detail::Value<std::vector<std::string> > m_metavar;
    std::vector<std::string> m_dest;
#ifdef _ARGPARSE_CXX_11
    std::function<void(std::string const&)> m_handle;
#endif  // C++11+
    detail::shared_ptr<_ConflictResolver> m_post_trigger;
    detail::Value<bool> m_required;
};

/*!
 *  \brief Formatter for generating usage messages and argument help strings
 */
_ARGPARSE_EXPORT class HelpFormatter
{
public:
    virtual ~HelpFormatter() _ARGPARSE_NOEXCEPT { }

    virtual std::string
    (*_fill_text() const) (std::string const&, std::size_t, std::size_t)
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
    (*_get_help_string() const) (Argument const*, std::string const&)
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
    _fill_text_s(std::string const& text, std::size_t width, std::size_t indent)
    {
        std::vector<std::string> res;
        std::string value;
        std::vector<std::string> lines = _split_lines_s(text, width - indent);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            std::size_t value_size = detail::_utf8_length(value).second;
            if (value_size < indent) {
                value.resize(value.size() + indent - value_size,
                             detail::_space);
            }
            value += lines[i];
            detail::_store_value_to(value, res, true);
        }
        detail::_store_value_to(value, res);
        return detail::_vector_to_string(res, "\n");
    }

    static std::string
    _get_default_metavar_for_optional_s(Argument const* action)
    {
        return detail::_to_u8upper(action->get_dest());
    }

    static std::string
    _get_default_metavar_for_positional_s(Argument const* action)
    {
        return action->get_dest();
    }

    static std::string
    _get_help_string_s(Argument const* action, std::string const& lang)
    {
        return detail::_tr(action->m_help, lang);
    }

    static std::vector<std::string>
    _split_lines_s(std::string const& text, std::size_t width)
    {
        std::string value;
        std::vector<std::string> res;
        std::vector<std::string> split_str = detail::_split_whitespace(text);
        for (std::size_t i = 0; i < split_str.size(); ++i) {
            if (detail::_utf8_length(value).second + 1
                    + detail::_utf8_length(split_str.at(i)).second > width) {
                detail::_store_value_to(value, res);
            }
            if (!value.empty()) {
                value += detail::_spaces;
            }
            value += split_str.at(i);
        }
        detail::_store_value_to(value, res);
        return res;
    }
};

/*!
 *  \brief Help message formatter which retains any formatting in descriptions
 */
_ARGPARSE_EXPORT
class _RawDescriptionHelpFormatter : virtual public HelpFormatter
{
public:
    virtual ~_RawDescriptionHelpFormatter() _ARGPARSE_NOEXCEPT { }

    std::string
    (*_fill_text() const) (std::string const&, std::size_t,
                           std::size_t) _ARGPARSE_OVERRIDE
    {
        return _fill_text_s;
    }

protected:
    static std::string
    _fill_text_s(std::string const& text, std::size_t width, std::size_t indent)
    {
        std::vector<std::string> res;
        std::string value;
        std::vector<std::string> lines = _split_lines_s(text, width - indent);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            std::size_t value_size = detail::_utf8_length(value).second;
            if (value_size < indent) {
                value.resize(value.size() + indent - value_size,
                             detail::_space);
            }
            value += lines.at(i);
            detail::_store_value_to(value, res, true);
        }
        detail::_store_value_to(value, res);
        return detail::_vector_to_string(res, "\n");
    }

    static std::vector<std::string>
    _split_lines_s(std::string const& text, std::size_t width)
    {
        std::size_t tab_size = ARGPARSE_TAB_SIZE;
        detail::_limit_to_min(tab_size, 2);
        std::string value;
        std::vector<std::string> res;
        std::vector<std::string> split_str = detail::_split(text, '\n', true);
        for (std::size_t i = 0; i < split_str.size(); ++i) {
            std::string const& str = split_str.at(i);
            if (str.empty()) {
                detail::_store_value_to(value, res, true);
            } else {
                std::vector<std::string> sub_split_str
                        = detail::_split(str, detail::_space, true, true);
                for (std::size_t j = 0; j < sub_split_str.size(); ++j) {
                    std::vector<std::string> tab_split_str
                        = detail::_split(sub_split_str.at(j), '\t', true, true);
                    for (std::size_t k = 0; k < tab_split_str.size(); ++k) {
                        std::string sub = tab_split_str.at(k);
                        if (sub == "\t") {
                            sub = std::string(
                                        tab_size - (detail::_utf8_length(
                                                      value).second % tab_size),
                                        detail::_space);
                        }
                        if (detail::_utf8_length(value).second + 1
                                + detail::_utf8_length(sub).second > width) {
                            detail::_store_value_to(value, res);
                            if (sub == "\t") {
                                sub = std::string(tab_size, detail::_space);
                            }
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
} _ARGPARSE_INLINE_VARIABLE RawDescriptionHelpFormatter;

/*!
 *  \brief Help message formatter which retains formatting of all help text
 */
_ARGPARSE_EXPORT
class _RawTextHelpFormatter : virtual public _RawDescriptionHelpFormatter
{
public:
    virtual ~_RawTextHelpFormatter() _ARGPARSE_NOEXCEPT { }

    std::vector<std::string>
    (*_split_lines() const) (std::string const&, std::size_t) _ARGPARSE_OVERRIDE
    {
        return _RawDescriptionHelpFormatter::_split_lines_s;
    }
} _ARGPARSE_INLINE_VARIABLE RawTextHelpFormatter;

/*!
 *  \brief Help message formatter which adds default values to argument help
 */
_ARGPARSE_EXPORT
class _ArgumentDefaultsHelpFormatter : virtual public HelpFormatter
{
public:
    virtual ~_ArgumentDefaultsHelpFormatter() _ARGPARSE_NOEXCEPT { }

    std::string
    (*_get_help_string() const) (Argument const*,
                                 std::string const&) _ARGPARSE_OVERRIDE
    {
        return _get_help_string_s;
    }

protected:
    static std::string
    _get_help_string_s(Argument const* action, std::string const& lang)
    {
        std::string res = detail::_tr(action->m_help, lang);
        if (!res.empty()
                && !detail::_contains_substr(res, "%(default)s")
                && !action->is_suppressed()) {
            if ((action->m_type == Argument::Optional
                 || (action->m_nargs & (Argument::ZERO_OR_ONE
                                        | Argument::ZERO_OR_MORE)))
                    && !(action->action() & (argparse::help
                                             | argparse::version
                                             | argparse::language))) {
                res += " (default: %(default)s)";
            }
        }
        return res;
    }
} _ARGPARSE_INLINE_VARIABLE ArgumentDefaultsHelpFormatter;

/*!
 *  \brief Help message formatter which uses the argument 'type' as the default
 *  metavar value (instead of the argument 'dest')
 */
_ARGPARSE_EXPORT
class _MetavarTypeHelpFormatter : virtual public HelpFormatter
{
public:
    virtual ~_MetavarTypeHelpFormatter() _ARGPARSE_NOEXCEPT { }

    std::string
    (*_get_default_metavar_for_optional() const)
    (Argument const*) _ARGPARSE_OVERRIDE
    {
        return _get_default_metavar_for_optional_s;
    }

    std::string
    (*_get_default_metavar_for_positional() const)
    (Argument const*) _ARGPARSE_OVERRIDE
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
 *  \brief _Group class
 */
class _Group
{
    friend class ArgumentParser;

protected:
    explicit
    _Group(std::string const& title, std::string const& description)
        : m_title(),
          m_description()
    {
        m_title[std::string()] = title;
        m_description[std::string()] = description;
    }

public:
    /*!
     *  \brief Destroy group
     */
    virtual ~_Group() _ARGPARSE_NOEXCEPT { }

    /*!
     *  \brief Get group 'title' value
     *
     *  \return Group 'title' value
     */
    inline std::string const& title() const
    {
        return detail::_map_at(m_title, std::string());
    }

    /*!
     *  \brief Get group 'description' value
     *
     *  \return Group 'description' value
     */
    inline std::string const& description() const
    {
        return detail::_map_at(m_description, std::string());
    }

protected:
    virtual void limit_help_flags(_HelpFormatter const& formatter,
                                  std::size_t& limit) const                 = 0;
    virtual void print_help(std::ostream& os,
                            _HelpFormatter const& formatter,
                            std::string const& prog,
                            std::size_t limit,
                            std::size_t width,
                            std::string const& lang) const                  = 0;

    detail::LanguagePack m_title;
    detail::LanguagePack m_description;
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

    _ArgumentData()
        : m_conflict_handler(),
          m_arguments(),
          m_optional(),
          m_positional(),
          m_add_help(false),
          m_help_added(false)
    { }

    static detail::shared_ptr<_ArgumentData>
    make_argument_data()
    {
        return detail::make_shared<_ArgumentData>(_ArgumentData());
    }

protected:
    typedef detail::shared_ptr<Argument> pArgument;

public:
    /*!
     *  \brief Destroy argument data
     */
    virtual ~_ArgumentData() _ARGPARSE_NOEXCEPT { }

protected:
    inline void update_help(bool add_help, std::string const& prefix_chars)
    {
        m_add_help = add_help;
        if (m_help_added) {
            m_arguments.pop_front();
            m_optional.pop_front();
            m_help_added = false;
        }
        if (m_add_help) {
            std::vector<std::string> help_flags
                    = detail::_help_flags(prefix_chars);
            if (m_conflict_handler == "resolve") {
                for (std::size_t i = 0; i < m_optional.size(); ++i) {
                    std::pair<pArgument, bool> const& pair = m_optional.at(i);
                    detail::_resolve_conflict(pair.first->flags(), help_flags);
                }
            }
            if (!help_flags.empty()) {
#ifdef _ARGPARSE_CXX_11
                auto help = Argument::make_argument(
                            std::move(help_flags), "help", Argument::Optional);
#else
                pArgument help = Argument::make_argument(
                            help_flags, "help", Argument::Optional);
#endif  // C++11+
                help->help("show this help message and exit")
                        .action(argparse::help);
                m_arguments.push_front(help);
                m_optional
                        .push_front(std::make_pair(m_arguments.front(), false));
                m_help_added = true;
            }
        }
    }

    inline std::vector<pArgument>
    get_arguments(bool add_suppress) const
    {
        std::vector<pArgument> res;
        res.reserve(m_arguments.size());
#ifdef _ARGPARSE_CXX_11
        for (auto const& arg : m_arguments) {
#else
        for (std::size_t i = 0; i < m_arguments.size(); ++i) {
            pArgument const& arg = m_arguments.at(i);
#endif  // C++11+
            if ((add_suppress || !arg->m_help_type.has_value())
                    && (arg->m_type != Argument::Optional
                        || !arg->flags().empty())) {
                res.push_back(arg);
            }
        }
        return res;
    }

    inline std::vector<pArgument>
    get_optional(bool add_suppress, bool add_group) const
    {
        std::vector<pArgument> res;
        res.reserve(m_optional.size());
#ifdef _ARGPARSE_CXX_11
        for (auto const& pair : m_optional) {
#else
        for (std::size_t i = 0; i < m_optional.size(); ++i) {
            std::pair<pArgument, bool> const& pair = m_optional.at(i);
#endif  // C++11+
            if ((add_suppress || !pair.first->m_help_type.has_value())
                    && (add_group || !pair.second)
                    && !pair.first->flags().empty()) {
                res.push_back(pair.first);
            }
        }
        return res;
    }

    inline std::vector<pArgument>
    get_positional(bool add_suppress, bool add_group) const
    {
        std::vector<pArgument> res;
        res.reserve(m_positional.size());
#ifdef _ARGPARSE_CXX_11
        for (auto const& pair : m_positional) {
#else
        for (std::size_t i = 0; i < m_positional.size(); ++i) {
            std::pair<pArgument, bool> const& pair = m_positional.at(i);
#endif  // C++11+
            if ((add_suppress || !pair.first->m_help_type.has_value())
                    && (add_group || !pair.second)) {
                res.push_back(pair.first);
            }
        }
        return res;
    }

    static void update_flag_name_func(
            std::string const& arg, std::string& flag, std::size_t& count)
    {
        std::string name = detail::_flag_name(arg);
        std::size_t count_prefixes = arg.size() - name.size();
        if (count < count_prefixes) {
            count = count_prefixes;
#ifdef _ARGPARSE_CXX_11
            flag = std::move(name);
#else
            flag = name;
#endif  // C++11+
        }
    }

    static void update_flag_name(std::vector<std::string>& flags,
                                 std::string const& prefix_chars,
                                 bool is_optional,
                                 std::string& flag_name, std::size_t& prefixes)
    {
        if (is_optional) {
            update_flag_name_func(flag_name, flag_name, prefixes);
        } else if (flags.size() > 1) {
            // no positional multiflag
            throw ValueError("invalid option string " + flags.front()
                             + ": must starts with a character '"
                             + prefix_chars + "'");
        }
        for (std::size_t i = 1; i < flags.size(); ++i) {
            // check arguments
            std::string& flag = flags.at(i);
            if (flag.empty()) {
                throw IndexError("string index out of range");
            }
            if (!detail::_is_value_exists(flag.at(0), prefix_chars)) {
                // no positional and optional args
                throw ValueError("invalid option string " + flag
                                 + ": must starts with a character '"
                                 + prefix_chars + "'");
            }
            update_flag_name_func(flag, flag_name, prefixes);
        }
    }

    static void check_flag_name(std::string const& flag)
    {
        if (flag.empty()) {
            throw IndexError("string index out of range");
        }
    }

    inline void check_conflicting_option(Argument const* arg,
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
    }

    inline void
    check_conflict_arg(Argument const* arg) _ARGPARSE_OVERRIDE _ARGPARSE_FINAL
    {
#ifdef _ARGPARSE_CXX_11
        for (auto& opt : m_optional) {
#else
        for (std::size_t i = 0; i < m_optional.size(); ++i) {
            std::pair<pArgument, bool>& opt = m_optional.at(i);
#endif  // C++11+
            if (opt.first.get() == arg) {
                return;
            }
            check_conflicting_option(arg, opt.first->m_all_flags);
            check_conflicting_option(arg, opt.first->m_flags);
        }
    }

    inline void merge_arguments(_ArgumentData const& data)
    {
#ifdef _ARGPARSE_CXX_11
        for (auto const& arg : data.m_optional) {
            check_conflict_arg(arg.first.get());
            m_optional.push_back(arg);
        }
        for (auto const& arg : data.m_positional) {
            m_positional.push_back(arg);
        }
        for (auto const& arg : data.m_arguments) {
            m_arguments.push_back(arg);
        }
#else
        for (std::size_t i = 0; i < data.m_optional.size(); ++i) {
            check_conflict_arg(data.m_optional.at(i).first.get());
            m_optional.push_back(data.m_optional.at(i));
        }
        for (std::size_t i = 0; i < data.m_positional.size(); ++i) {
            m_positional.push_back(data.m_positional.at(i));
        }
        for (std::size_t i = 0; i < data.m_arguments.size(); ++i) {
            m_arguments.push_back(data.m_arguments.at(i));
        }
#endif  // C++11+
    }

    static void create_argument(detail::shared_ptr<_ArgumentData>& data,
                                std::vector<std::string> const& in_flags,
                                std::string const& prefix_chars)
    {
        std::vector<std::string> flags = in_flags;
        if (flags.empty()) {
            pArgument arg = Argument::make_argument(flags, std::string(),
                                                    Argument::Positional);
            data->m_arguments.push_back(arg);
            return;
        }
        flags.front() = detail::_trim_copy(flags.front());
        std::string flag = flags.front();
        check_flag_name(flag);
        std::size_t prefixes = 0;
        bool is_optional = detail::_is_value_exists(flag.at(0), prefix_chars);
        update_flag_name(flags, prefix_chars, is_optional, flag, prefixes);
        if (is_optional) {
            flag = detail::_replace(flag, '-', "_");
        }
#ifdef _ARGPARSE_CXX_11
        auto arg = Argument::make_argument(
                    std::move(flags), std::move(flag),
                    is_optional ? Argument::Optional : Argument::Positional);
#else
        pArgument arg = Argument::make_argument(
                    flags, flag,
                    is_optional ? Argument::Optional : Argument::Positional);
#endif  // C++11+
        if (is_optional) {
            data->check_conflict_arg(arg.get());
        }
        data->m_arguments.push_back(arg);
        if (is_optional) {
#ifdef _ARGPARSE_CXX_11
            data->m_arguments.back()->m_post_trigger = data;
#else
            data->m_arguments.back()->m_post_trigger
                = detail::_pointer_cast<_ConflictResolver, _ArgumentData>(data);
#endif  // C++11+
        }
    }

    inline void validate_argument(Argument const& argument,
                                  std::string const& prefix_chars)
    {
        Argument arg = argument;
        std::vector<std::string>& flags = arg.m_flags;
        bool is_optional = false;
        if (flags.empty()) {
            arg.m_name = arg.dest();
        } else {
            std::string flag = flags.front();
            check_flag_name(flag);
            std::size_t prefixes = 0;
            is_optional = detail::_is_value_exists(flag.at(0), prefix_chars);
            update_flag_name(flags, prefix_chars, is_optional, flag, prefixes);
            arg.m_name = is_optional ? detail::_replace(flag, '-', "_") : flag;
        }
        arg.m_type = is_optional ? Argument::Optional : Argument::Positional;
        // check
        if (!is_optional) {
            if (arg.dest().empty() && flags.empty()) {
                throw
                TypeError("missing 1 required positional argument: 'dest'");
            }
            if (arg.action()
                  & (argparse::version | argparse::help | argparse::language)) {
                // version, help and language actions cannot be positional
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
            if (arg.action() == argparse::BooleanOptionalAction) {
                arg.make_no_flags();
            }
            check_conflict_arg(&arg);
        }
        m_arguments.push_back(detail::make_shared<Argument>(arg));
    }

    std::string m_conflict_handler;
    std::deque<pArgument> m_arguments;
    std::deque<std::pair<pArgument, bool> > m_optional;
    std::deque<std::pair<pArgument, bool> > m_positional;
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

    explicit
    _ArgumentGroup(std::string& prefix_chars,
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
    { }

public:
    /*!
     *  \brief Create argument group object from another argument group
     *
     *  \param orig Argument group object to copy
     *
     *  \return Argument group object
     */
    _ArgumentGroup(_ArgumentGroup const& orig)
        : m_data(orig.m_data),
          m_prefix_chars(orig.m_prefix_chars),
          m_parent_data(orig.m_parent_data),
          m_argument_default(orig.m_argument_default),
          m_argument_default_type(orig.m_argument_default_type),
          m_is_mutex_group(orig.m_is_mutex_group)
    { }

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
    inline _ArgumentGroup& operator =(_ArgumentGroup const& rhs)
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
    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flag values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    Argument& add_argument(Args... flags)
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
    Argument& add_argument(std::initializer_list<std::string> flags)
    {
        return add_argument(std::vector<std::string>{ flags });
    }
#else
    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::string const& flag)
    {
        return add_argument(detail::_make_vector(flag));
    }

    /*!
     *  \brief Add argument with 2 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::string const& flag1,
                                  std::string const& flag2)
    {
        return add_argument(detail::_make_vector(flag1, flag2));
    }

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
    inline Argument& add_argument(std::string const& flag1,
                                  std::string const& flag2,
                                  std::string const& flag3)
    {
        return add_argument(detail::_make_vector(flag1, flag2, flag3));
    }

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
    inline Argument& add_argument(std::string const& flag1,
                                  std::string const& flag2,
                                  std::string const& flag3,
                                  std::string const& flag4)
    {
        return add_argument(detail::_make_vector(flag1, flag2, flag3, flag4));
    }
#endif  // C++11+

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::vector<std::string> const& flags)
    {
        m_data->create_argument(m_data, flags, m_prefix_chars);
        process_add_argument();
        return *m_data->m_arguments.back();
    }

protected:
    inline void process_add_argument()
    {
        bool optional
                = m_data->m_arguments.back()->m_type == Argument::Optional;
        if (!optional) {
            if (m_is_mutex_group) {
                m_data->m_arguments.pop_back();
                throw
                ValueError("mutually exclusive arguments must be optional");
            }
        } else if (m_parent_data->m_conflict_handler == "resolve") {
            for (std::size_t i = 0; i < m_parent_data->m_optional.size(); ++i) {
                m_parent_data->m_optional.at(i).first->resolve_conflict_flags(
                            m_data->m_arguments.back()->flags());
            }
        }
        if (m_argument_default.has_value()
                && !m_data->m_arguments.back()->m_default.has_value()
                && !m_data->m_arguments.back()->m_default_type.has_value()) {
            m_data->m_arguments.back()->default_value(m_argument_default());
        }
        if (m_argument_default_type.has_value()) {
            m_data->m_arguments.back()->default_value(
                        m_argument_default_type());
        }
        m_parent_data->m_arguments.push_back(m_data->m_arguments.back());
        (optional ? m_data->m_optional : m_data->m_positional)
                .push_back(std::make_pair(m_data->m_arguments.back(),
                                          !m_is_mutex_group));
        (optional ? m_parent_data->m_optional : m_parent_data->m_positional)
                .push_back(std::make_pair(m_data->m_arguments.back(),
                                          !m_is_mutex_group));
    }

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
    ArgumentGroup(std::string const& title,
                  std::string const& description,
                  std::string& prefix_chars,
                  pArgumentData& parent_data,
                  detail::Value<std::string>& argument_default,
                  detail::Value<_SUPPRESS>& argument_default_type)
        : _Group(title, description),
          _ArgumentGroup(prefix_chars, parent_data,
                         argument_default, argument_default_type, false)
    { }

    static detail::shared_ptr<ArgumentGroup>
    make_argument_group(std::string const& title,
                        std::string const& description,
                        std::string& prefix_chars,
                        pArgumentData& parent_data,
                        detail::Value<std::string>& argument_default,
                        detail::Value<_SUPPRESS>& argument_default_type)
    {
        return detail::make_shared<ArgumentGroup>
                (ArgumentGroup(title, description, prefix_chars, parent_data,
                               argument_default, argument_default_type));
    }

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
    ArgumentGroup(ArgumentGroup const& orig)
        : _Group(orig),
          _ArgumentGroup(orig)
    { }

    /*!
     *  \brief Copy argument group object from another argument group
     *
     *  \param rhs Argument group object to copy
     *
     *  \return Current argument group reference
     */
    inline ArgumentGroup& operator =(ArgumentGroup const& rhs)
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
     *  for selected language (default: "")
     *
     *  \param value Title value
     *  \param lang Language value
     *
     *  \return Current argument group reference
     */
    inline ArgumentGroup& title(std::string const& value,
                                std::string const& lang = std::string())
    {
        m_title[lang] = value;
        return *this;
    }

    /*!
     *  \brief Set argument group 'description' value
     *  for selected language (default: "")
     *
     *  \param value Description value
     *  \param lang Language value
     *
     *  \return Current argument group reference
     */
    inline ArgumentGroup& description(std::string const& value,
                                      std::string const& lang = std::string())
    {
        m_description[lang] = value;
        return *this;
    }

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current argument group reference
     */
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
    inline ArgumentGroup& add_argument(Argument const& argument)
    {
        m_data->validate_argument(Argument(argument), m_prefix_chars);
        process_add_argument();
        return *this;
    }

private:
    inline void
    limit_help_flags(_HelpFormatter const& formatter,
                     std::size_t& limit) const _ARGPARSE_OVERRIDE
    {
        for (std::size_t i = 0; i < m_data->m_arguments.size(); ++i) {
            detail::_limit_to_min(limit,
                                  m_data->m_arguments
                                     .at(i)->flags_to_string(formatter).size());
        }
    }

    inline void print_help(std::ostream& os,
                           _HelpFormatter const& formatter,
                           std::string const& prog,
                           std::size_t limit,
                           std::size_t width,
                           std::string const& lang) const _ARGPARSE_OVERRIDE
    {
        std::string description = detail::_tr(m_description, lang);
        if (!description.empty() || !m_data->m_arguments.empty()) {
            std::string title = detail::_tr(m_title, lang);
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
            for (std::size_t i = 0; i < m_data->m_arguments.size(); ++i) {
                os << m_data->m_arguments.at(i)->print(formatter, limit,
                                                      width, lang) << std::endl;
            }
        }
    }
};

/*!
 *  \brief MutuallyExclusiveGroup class
 */
_ARGPARSE_EXPORT class MutuallyExclusiveGroup : public _ArgumentGroup
{
    friend class ArgumentParser;

    explicit
    MutuallyExclusiveGroup(std::string& prefix_chars,
                           pArgumentData& parent_data,
                           detail::Value<std::string>& argument_default,
                           detail::Value<_SUPPRESS>& argument_default_type)
        : _ArgumentGroup(prefix_chars, parent_data,
                         argument_default, argument_default_type, true),
          m_required(false)
    { }

    static MutuallyExclusiveGroup
    make_mutex_group(std::string& prefix_chars,
                     pArgumentData& parent_data,
                     detail::Value<std::string>& argument_default,
                     detail::Value<_SUPPRESS>& argument_default_type)
    {
        return MutuallyExclusiveGroup(prefix_chars, parent_data,
                                      argument_default, argument_default_type);
    }

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
    MutuallyExclusiveGroup(MutuallyExclusiveGroup const& orig)
        : _ArgumentGroup(orig),
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
    inline MutuallyExclusiveGroup& operator =(MutuallyExclusiveGroup const& rhs)
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
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
    inline MutuallyExclusiveGroup& add_argument(Argument const& argument)
    {
        m_data->validate_argument(Argument(argument), m_prefix_chars);
        process_add_argument();
        return *this;
    }

private:
    inline std::string usage(_HelpFormatter const& formatter) const
    {
        std::string res;
        for (std::size_t i = 0; i < m_data->m_arguments.size(); ++i) {
            detail::_append_value_to(
                       m_data->m_arguments.at(i)->usage(formatter), res, " | ");
        }
        return res.empty() ? res
                           : (m_required ? "(" + res + ")" : "[" + res + "]");
    }

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

public:
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
        mapped_type(std::vector<std::string> const& values)
            : m_exists(true),
              m_is_default(true),
              m_values(values),
              m_indexes()
        {
            m_indexes.push_back(m_values.size());
        }

        inline void clear()
        {
            m_values.clear();
            m_indexes.clear();
            m_exists = false;
            m_is_default = false;
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

#ifdef _ARGPARSE_CXX_11
        inline void emplace_back(std::string&& value)
        {
            m_values.emplace_back(std::move(value));
            m_indexes.push_back(m_values.size());
            m_exists = true;
        }
#endif  // C++11+

        inline void push_back(std::string const& value)
        {
            m_values.push_back(value);
            m_indexes.push_back(m_values.size());
            m_exists = true;
        }

        inline void push_values(std::vector<std::string> const& values)
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
        bool m_exists;
        bool m_is_default;
        std::vector<std::string> m_values;
        std::vector<std::size_t> m_indexes;
    };

    typedef pArgument                               key_type;
    typedef std::pair<key_type, mapped_type>        value_type;
    typedef std::deque<value_type>                  map_type;
    typedef map_type::iterator                      iterator;
    typedef map_type::const_iterator                const_iterator;

    _Storage()
        : m_data()
    { }

    inline void
    create(key_type const& key, mapped_type const& value = mapped_type())
    {
        if (key->action() & (argparse::version | argparse::help)) {
            return;
        }
        std::vector<std::string> const& arg_flags = key->flags();
        bool have_key = false;
#ifdef _ARGPARSE_CXX_11
        for (auto& pair : m_data) {
#else
        for (std::size_t i = 0; i < m_data.size(); ++i) {
            value_type& pair = m_data.at(i);
#endif  // C++11+
            have_key |= (key == pair.first);
            if (key != pair.first) {
                pair.first->resolve_conflict_flags(arg_flags);
            }
        }
        if (!have_key) {
            m_data.push_back(std::make_pair(key, value));
        }
    }

#ifdef _ARGPARSE_CXX_11
    template <class T>
    void create(T const& arguments)
    {
        for (auto const& arg : arguments) {
            create(arg);
        }
    }
#else
    void create(std::vector<key_type> const& arguments)
    {
        for (std::size_t i = 0; i < arguments.size(); ++i) {
            create(arguments.at(i));
        }
    }
#endif  // C++11+

    inline void have_value(key_type const& arg)
    {
        if (arg->implicit_value().empty()) {
            at(arg).push_values(std::vector<std::string>());
            on_process_store(arg, std::vector<std::string>());
        } else {
            at(arg).push_back(arg->implicit_value());
            on_process_store(arg, arg->implicit_value());
#ifdef _ARGPARSE_CXX_11
            arg->handle(arg->implicit_value());
#endif  // C++11+
        }
    }

    inline void store_value(key_type const& arg, std::string const& value)
    {
        at(arg).push_back(value);
        on_process_store(arg, value);
#ifdef _ARGPARSE_CXX_11
        arg->handle(value);
#endif  // C++11+
    }

    inline void store_values(key_type const& arg,
                             std::vector<std::string> const& values)
    {
        at(arg).push_values(values);
        on_process_store(arg, values);
#ifdef _ARGPARSE_CXX_11
        for (auto const& value : values) {
            arg->handle(value);
        }
#endif  // C++11+
    }

    inline void store_default_value(key_type const& arg,
                                    std::string const& value)
    {
        if (arg->action()
                & (argparse::store | argparse::BooleanOptionalAction)) {
            mapped_type& arg_data = at(arg);
            if (arg_data.empty()) {
                arg_data.push_default(value);
                on_process_store(arg, value);
#ifdef _ARGPARSE_CXX_11
                arg->handle(value);
#endif  // C++11+
            }
        }
    }

    inline bool self_value_stored(key_type const& arg)
    {
        if (arg->action() & (argparse::store_const | detail::_bool_action)) {
            mapped_type& arg_data = at(arg);
            arg_data.clear();
            arg_data.push_back(arg->const_value());
            on_process_store(arg, arg->const_value());
#ifdef _ARGPARSE_CXX_11
            arg->handle(arg->const_value());
#endif  // C++11+
            return true;
        } else if (arg->action() == argparse::append_const) {
            at(arg).push_back(arg->const_value());
            on_process_store(arg, arg->const_value());
#ifdef _ARGPARSE_CXX_11
            arg->handle(arg->const_value());
#endif  // C++11+
            return true;
        } else if (arg->action() == argparse::count) {
            at(arg).push_back(std::string());
            on_process_store(arg, std::string());
#ifdef _ARGPARSE_CXX_11
            arg->handle(std::string());
#endif  // C++11+
            return true;
        }
        return false;
    }

    inline bool exists_arg(std::string const& key) const
    {
        const_iterator it = find_arg(key);
        return it != end();
    }

    inline bool exists(std::string const& key) const
    {
        const_iterator it = find(key);
        return it != end();
    }

    inline bool exists(key_type const& key) const
    {
        const_iterator it = find(key);
        return it != end();
    }

    inline value_type const& at(std::string const& key) const
    {
        const_iterator it = find(key);
        if (it == end()) {
            throw std::logic_error("key '" + key + "' not found");
        }
        return *it;
    }

    inline mapped_type& at(key_type const& key)
    {
        iterator it = find(key);
        if (it == end()) {
            throw std::logic_error("key '" + key->m_name + "' not found");
        }
        return it->second;
    }

    inline mapped_type const& at(key_type const& key) const
    {
        const_iterator it = find(key);
        if (it == end()) {
            throw std::logic_error("key '" + key->m_name + "' not found");
        }
        return it->second;
    }

    inline iterator erase(iterator i)                { return m_data.erase(i); }

    inline iterator begin()       _ARGPARSE_NOEXCEPT { return m_data.begin(); }
    inline iterator end()         _ARGPARSE_NOEXCEPT { return m_data.end(); }
    inline const_iterator
                    begin() const _ARGPARSE_NOEXCEPT { return m_data.begin(); }
    inline const_iterator
                    end()   const _ARGPARSE_NOEXCEPT { return m_data.end(); }

private:
    inline const_iterator find_arg(std::string const& key) const
    {
        const_iterator it = find(key);
        if (it != end()) {
            return it;
        }
        for (it = begin(); it != end(); ++it) {
            if (it->first->m_type == Argument::Optional
                    && it->first->dest().empty()) {
                for (std::size_t i = 0; i < it->first->m_flags.size(); ++i) {
                    if (detail::_flag_name(it->first->m_flags.at(i)) == key
                            || it->first->m_name == key) {
                        return it;
                    }
                }
            }
        }
        return end();
    }

    inline const_iterator find(std::string const& key) const
    {
        for (const_iterator it = begin(); it != end(); ++it) {
            if (*(it->first) == key) {
                return it;
            }
        }
        return end();
    }

    inline const_iterator find(key_type const& key) const
    {
        for (const_iterator it = begin(); it != end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return end();
    }

    inline iterator find(key_type const& key)
    {
        for (iterator it = begin(); it != end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return end();
    }

    inline void on_process_store(key_type const& arg, std::string const& value)
    {
        std::string const& dest = arg->dest();
        if (!dest.empty()) {
            for (iterator it = begin(); it != end(); ++it) {
                if (it->first != arg && it->first->dest() == dest) {
                    if (it->first->action()
                            & (argparse::store
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

    inline void on_process_store(key_type const& arg,
                                 std::vector<std::string> const& values)
    {
        std::string const& dest = arg->dest();
        if (!dest.empty()) {
            for (iterator it = begin(); it != end(); ++it) {
                if (it->first != arg && it->first->dest() == dest) {
                    if (it->first->action()
                            & (argparse::store
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

    map_type m_data;
};

/*!
 *  \brief Object with parsed arguments
 */
_ARGPARSE_EXPORT class Namespace
{
    friend class ArgumentParser;

    explicit
    Namespace(_Storage const& storage = _Storage())
        : m_storage(storage),
          m_unrecognized_args()
    { }

    explicit
    Namespace(_Storage const& storage, std::vector<std::string> const& args)
        : m_storage(storage),
          m_unrecognized_args(args)
    { }

#ifdef _ARGPARSE_CXX_11
    explicit
    Namespace(_Storage&& storage) _ARGPARSE_NOEXCEPT
        : m_storage(std::move(storage)),
          m_unrecognized_args()
    { }

    explicit
    Namespace(_Storage&& storage,
              std::vector<std::string>&& args) _ARGPARSE_NOEXCEPT
        : m_storage(std::move(storage)),
          m_unrecognized_args(std::move(args))
    { }
#endif  // C++11+

public:
    /*!
     *  \brief Check if argument name exists and specified in parsed arguments
     *
     *  \param key Argument destination name or flag
     *
     *  \return true if argument name exists and specified, otherwise false
     */
    inline bool exists(std::string const& key) const
    {
        _Storage::const_iterator it = storage().find_arg(key);
        if (it != storage().end()) {
            return !it->second.empty()
                    || it->first->action() == argparse::count;
        }
        return false;
    }

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
    typename detail::enable_if<detail::is_constructible<std::string, T>::value
                               || detail::is_floating_point<T>::value
                               || detail::is_same<bool, T>::value
                               || detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == argparse::count) {
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
     *  \brief Get parsed argument value as integer types.
     *  If argument not parsed, returns default value.
     *
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename detail::enable_if<detail::is_integral<T>::value
                               && !detail::is_same<bool, T>::value
                               && !detail::is_byte_type<T>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == argparse::count) {
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
    typename std::enable_if<
        detail::is_stl_array<typename std::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
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
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename detail::enable_if<
      detail::is_stl_container_paired<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        typedef typename T::value_type::first_type K;
        typedef typename T::value_type::second_type V;
        std::vector<std::pair<K, V> > vector
                = to_paired_vector<K, V>(args.second(), delim);
        return T(vector.begin(), vector.end());
    }

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Get parsed argument value as tupled container types.
     *  If argument not parsed, returns empty container.
     *
     *  \param key Argument destination name or flag
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value, T
    >::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        auto const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        auto vector = to_tupled_vector<
                typename T::value_type>(args.second(), delim);
        return T(vector.begin(), vector.end());
    }
#endif  // C++11+

    /*!
     *  \brief Get parsed argument value as mapped types.
     *  If argument not parsed, returns empty map.
     *
     *  \param key Argument destination name or flag
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename detail::enable_if<
        detail::is_stl_map<typename detail::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        typedef typename T::key_type K;
        typedef typename T::mapped_type V;
#ifdef _ARGPARSE_CXX_11
        T res{};
        auto vector = to_paired_vector<K, V>(args.second(), delim);
        for (auto const& pair : vector) {
            res.emplace(std::make_pair(pair.first, pair.second));
        }
#else
        T res;
        std::vector<std::pair<K, V> > vector
                = to_paired_vector<K, V>(args.second(), delim);
        for (std::size_t i = 0; i < vector.size(); ++i) {
            res.insert(std::make_pair(vector.at(i).first, vector.at(i).second));
        }
#endif  // C++11+
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
    typename detail::enable_if<
        detail::is_stl_matrix<typename detail::decay<T>::type>::value
     && !detail::is_stl_matrix_queue<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() != argparse::append
                || !(args.first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        typedef typename T::value_type V;
        typedef typename T::value_type::value_type VV;
        T res;
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
    typename detail::enable_if<
        detail::is_stl_matrix_queue<typename detail::decay<T>::type>::value, T
    >::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() != argparse::append
                || !(args.first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        typedef typename T::value_type V;
        typedef typename T::value_type::value_type VV;
        T res;
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    typename detail::enable_if<
        detail::is_stl_pair<typename detail::decay<T>::type>::value, T>::type
    get(std::string const& key, char delim = detail::_equal) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        if (args.second.empty()) {
            return T();
        }
        typedef typename T::first_type K;
        typedef typename T::second_type V;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args.second.size() != 2) {
                throw
                TypeError("invalid data for paired argument '" + key + "'");
            }
            return std::make_pair(to_type<K>(args.second.front()),
                                  to_type<V>(args.second.at(1)));
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return to_pair<K, V>(args.second.front(), delim);
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
    typename detail::enable_if<
        detail::is_stl_queue<typename detail::decay<T>::type>::value, T>::type
    get(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::basic<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
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
        if (args.first->action() == argparse::count) {
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
        detail::_check_type_name(args.first->m_type_name,
                                 detail::Type::name<T>());
        if (args.first->action() == argparse::count) {
            throw TypeError("invalid get type for argument '" + key + "'");
        }
        return to_type<T>(detail::_vector_to_string(args.second()));
    }

    /*!
     *  \brief Print namespace to output stream (default: std::cout)
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
     *  \param key Argument destination name or flag
     *
     *  \return Parsed argument value as args string
     */
    inline std::string to_args(std::string const& key) const
    {
        _Storage::value_type const& args = data(key);
        switch (args.first->action()) {
            case argparse::store_const :
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
            case argparse::store_true :
            case argparse::store_false :
                if (args.second.empty()) {
                    return detail::_bool_to_string(args.first->default_value());
                }
                if (args.second.size() != 1) {
                    throw TypeError("trying to get data from array argument '"
                                    + key + "'");
                }
                return detail::_bool_to_string(args.second.front());
            case argparse::count :
                return detail::_to_string(args.second.size());
            case argparse::store :
            case argparse::append :
            case argparse::append_const :
            case argparse::extend :
            case argparse::language :
                return detail::_vector_to_string(args.second(), detail::_spaces,
                                                 std::string(), true);
            case argparse::BooleanOptionalAction :
                return boolean_option_to_args(key, args);
            default :
                throw ValueError("action not supported");
        }
    }

    /*!
     *  \brief Get parsed argument value as string
     *
     *  \param key Argument destination name or flag
     *  \param quotes Value quotes
     *
     *  \return Parsed argument value as string
     */
    inline std::string
    to_string(std::string const& key,
              std::string const& quotes = std::string()) const
    {
        _Storage::value_type const& args = data(key);
        switch (args.first->action()) {
            case argparse::store_const :
                if (args.second.empty()) {
                    return std::string("None");
                }
                if (args.second.size() != 1) {
                    throw TypeError("trying to get data from array argument '"
                                    + key + "'");
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

    /*!
     *  \brief Get namespace as string
     *
     *  \return Namespace as string
     */
    inline std::string to_string() const
    {
        std::string res;
        for (_Storage::const_iterator it
             = storage().begin(); it != storage().end(); ++it) {
            _Storage::value_type const& pair = *it;
            std::vector<std::string> const& flags
                    = pair.first->get_argument_flags();
            if (flags.empty()) {
                continue;
            }
            detail::_append_value_to(pair.first->get_dest() + detail::_equals
                                    + to_string(flags.front(), "'"), res, ", ");
        }
        if (!m_unrecognized_args.has_value()) {
            return "Namespace(" + res + ")";
        }
        std::string unknown_args
                = detail::_vector_to_string(unrecognized_args(), ", ", "'");
        return "(Namespace(" + res + "), [" + unknown_args + "])";
    }

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
    std::optional<typename std::enable_if<
        std::is_constructible<std::string, T>::value
        || std::is_floating_point<T>::value
        || std::is_same<bool, T>::value
        || detail::is_byte_type<T>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || args->second.empty()
                || args->second.size() != 1
                || !detail::_is_type_name_correct(args->first->type_name(),
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
            return std::nullopt;
        }
        if (args->first->action() == argparse::count) {
            return T(args->second.size());
        }
        if (args->second.empty() || args->second.size() != 1) {
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
    std::optional<typename std::enable_if<
        detail::is_stl_array<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return std::nullopt;
        }
        auto vector = try_to_vector<typename T::value_type>(args->second());
        if (!vector.operator bool()) {
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
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return std::nullopt;
        }
        auto vector = try_to_vector<typename T::value_type>(
                    args->second().begin(), args->second().end());
        if (!vector.operator bool()) {
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    std::optional<typename std::enable_if<
        detail::is_stl_container_paired<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::value_type::first_type K;
        typedef typename T::value_type::second_type V;
        auto vector = try_to_paired_vector<K, V>(args->second(), delim);
        if (!vector.operator bool()) {
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    std::optional<typename std::enable_if<
        detail::is_stl_container_tupled<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return std::nullopt;
        }
        auto vector = try_to_tupled_vector<
                typename T::value_type>(args->second(), delim);
        if (!vector.operator bool()) {
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    std::optional<typename std::enable_if<
        detail::is_stl_map<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::key_type K;
        typedef typename T::mapped_type V;
        T res{};
        auto vector = try_to_paired_vector<K, V>(args->second(), delim);
        if (!vector.operator bool()) {
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
    std::optional<typename std::enable_if<
        detail::is_stl_matrix<typename std::decay<T>::type>::value
        && !detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() != argparse::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
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
            if (!vector.operator bool()) {
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
    std::optional<typename std::enable_if<
        detail::is_stl_matrix_queue<typename std::decay<T>::type>::value,
    T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() != argparse::append
                || !(args->first->m_nargs
                     & (Argument::NARGS_NUM | Argument::ONE_OR_MORE
                        | Argument::ZERO_OR_MORE))
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
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
            if (!vector.operator bool()) {
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value
     */
    template <class T>
    std::optional<typename std::enable_if<
        detail::is_stl_pair<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return std::nullopt;
        }
        if (args->second.empty()) {
            return T();
        }
        typedef typename T::first_type K;
        typedef typename T::second_type V;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args->second.size() != 2) {
                return std::nullopt;
            }
            auto el1 = try_to_type<K>(args->second.at(0));
            auto el2 = try_to_type<V>(args->second.at(1));
            if (el1.operator bool() && el2.operator bool()) {
                return std::make_pair(el1.value(), el2.value());
            } else {
                return std::nullopt;
            }
        }
        if (args->second.size() != 1) {
            return std::nullopt;
        }
        return try_to_pair<K, V>(args->second.front(), delim);
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
    std::optional<typename std::enable_if<
        detail::is_stl_queue<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(
                        args->first->type_name(), detail::Type::basic<T>())) {
            return std::nullopt;
        }
        typedef typename T::value_type V;
        auto vector = try_to_vector<V>(args->second());
        if (!vector.operator bool()) {
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
     *  \param delim Delimiter
     *
     *  \return Parsed argument value or std::nullopt
     */
    template <class T>
    std::optional<typename std::enable_if<
        detail::is_stl_tuple<typename std::decay<T>::type>::value, T>::type>
    try_get(std::string const& key, char delim = detail::_equal) const
    {
        auto args = try_get_data(key);
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return std::nullopt;
        }
        if (args->second.empty()) {
            return T();
        }
        if (std::isspace(static_cast<unsigned char>(delim))) {
            return try_to_tuple(detail::type_tag<T>{}, args->second());
        }
        if (args->second.size() != 1) {
            return std::nullopt;
        }
        return try_to_tuple(detail::type_tag<T>{},
                            detail::_split(args->second.front(), delim));
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
        if (!args.operator bool()
                || args->first->action() == argparse::count
                || !detail::_is_type_name_correct(args->first->type_name(),
                                                  detail::Type::name<T>())) {
            return std::nullopt;
        }
        return try_to_type<T>(detail::_vector_to_string(args->second()));
    }
#endif  // C++17+

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
                           _Storage::value_type const& args) const
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
                             _Storage::value_type const& args,
                             std::string const& quotes) const
    {
        if (args.second.empty()) {
            return args.first->action() == argparse::BooleanOptionalAction
               ? "None" :  detail::_bool_to_string(args.first->default_value());
        }
        if (args.second.size() != 1) {
            throw
            TypeError("trying to get data from array argument '" + key + "'");
        }
        return args.second.is_default()
                ? quotes + args.second.front() + quotes
                : detail::_bool_to_string(args.second.front());
    }

    inline std::string
    store_actions_to_string(_Storage::value_type const& args,
                            std::string const& quotes) const
    {
        if ((args.first->action() == argparse::store
             && (args.first->m_nargs
                 & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE)))
                || (!args.second.exists()
                    && args.first->m_type == Argument::Optional)
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
            return detail::_vector_to_string(args.second(), ", ",
                                             quotes, false, none, "[", "]");
        } else {
            std::string none = (args.first->m_nargs
                             & (Argument::ZERO_OR_MORE | Argument::REMAINDING))
                    ? std::string() : "None";
            return detail::_matrix_to_string(args.second(),
                                             args.second.indexes(), ", ",
                                             quotes, false, none, "[", "]");
        }
    }

    inline _Storage::value_type const& data(std::string const& key) const
    {
        _Storage::const_iterator it = storage().find_arg(key);
        if (it != storage().end()) {
            return *it;
        }
        throw
        AttributeError("'Namespace' object has no attribute '" + key + "'");
    }

    inline _Storage const& storage() const _ARGPARSE_NOEXCEPT
    {
        return m_storage;
    }

    template <class T, class U>
    std::pair<T, U> to_pair(std::string const& data, char delim) const
    {
        std::pair<std::string, std::string> const pair
                = detail::_split_delimiter(data, delim);
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
                vec.push_back(std::make_pair(to_type<T>(args.at(i)),
                                             to_type<U>(args.at(i + 1))));
            }
        } else {
            vec.reserve(args.size());
            for (std::size_t i = 0; i < args.size(); ++i) {
                vec.push_back(to_pair<T, U>(args.at(i), delim));
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
    std::vector<T> to_vector(value_const_iterator beg,
                             value_const_iterator end) const
#else
    template <class T>
    std::vector<T> to_vector(
            value_const_iterator beg, value_const_iterator end,
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
    std::vector<T> to_vector(value_const_iterator beg,
                             value_const_iterator end) const
#else
    template <class T>
    std::vector<T> to_vector(
            value_const_iterator beg, value_const_iterator end,
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
            std::string value = detail::_remove_quotes(*it);
            if (!data.empty() && !value.empty()) {
                data += detail::_spaces;
            }
            data += value;
        }
        T res;
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
    parse_tuple(std::vector<std::string> const& values,
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
            std::transform(args.begin(), args.end(), std::back_inserter(vec),
                           [this, delim] (std::string const& a)
            { return to_tuple(detail::type_tag<T>{},
                              detail::_split(a, delim)); });
        }
        return vec;
    }
#endif  // C++11+

    template <class T>
    typename detail::enable_if<
        detail::is_constructible<std::string, T>::value, T>::type
    to_type(std::string const& data) const
    {
#ifdef _ARGPARSE_CXX_11
        return detail::_remove_quotes<T>(data);
#else
        return detail::_remove_quotes(data);
#endif  // C++11+
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
            throw TypeError("trying to get data from array argument value '"
                            + data + "'");
        }
        return T(data.at(0));
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
        T res;
        std::stringstream ss(detail::_remove_quotes(data));
        ss >> res;
        if (ss.fail() || !ss.eof()) {
            throw TypeError("invalid " + detail::Type::name<T>()
                            + " value: '" + data + "'");
        }
        return res;
    }

#ifdef _ARGPARSE_CXX_17
    inline std::optional<_Storage::value_type>
    try_get_data(std::string const& key) const
    {
        auto it = storage().find_arg(key);
        if (it != storage().end()) {
            return *it;
        }
        return std::nullopt;
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
            return std::nullopt;
        }
    }

    template <class T, class U>
    std::optional<std::vector<std::pair<T, U> > >
    try_to_paired_vector(std::vector<std::string> const& args, char delim) const
    {
        std::vector<std::pair<T, U> > vec;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            if (args.size() & 1) {
                return std::nullopt;
            }
            vec.reserve(args.size() / 2);
            for (std::size_t i = 0; i < args.size(); i += 2) {
                auto el1 = try_to_type<T>(args.at(i));
                auto el2 = try_to_type<U>(args.at(i + 1));
                if (el1.operator bool() && el2.operator bool()) {
                    vec.emplace_back(std::make_pair(el1.value(), el2.value()));
                } else {
                    return std::nullopt;
                }
            }
        } else {
            vec.reserve(args.size());
            for (auto const& arg : args) {
                auto pair = try_to_pair<T, U>(arg, delim);
                if (pair.operator bool()) {
                    vec.emplace_back(pair.value());
                } else {
                    return std::nullopt;
                }
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
    try_to_vector(value_const_iterator beg, value_const_iterator end) const
    {
        std::vector<T> vec;
        vec.reserve(static_cast<std::size_t>(end - beg));
        for (value_const_iterator it = beg; it != end; ++it) {
            auto el = try_to_type<T>(*it);
            if (el.operator bool()) {
                vec.emplace_back(el.value());
            } else {
                return std::nullopt;
            }
        }
        return vec;
    }

    template <class T,
              typename std::enable_if<
                  !std::is_constructible<std::string, T>::value
                  && !std::is_floating_point<T>::value
                  && !std::is_integral<T>::value>::type* = nullptr>
    std::optional<std::vector<T> >
    try_to_vector(value_const_iterator beg, value_const_iterator end) const
    {
        std::vector<T> vec;
        if (end == beg) {
            return vec;
        }
        std::string data;
        for (value_const_iterator it = beg; it != end; ++it) {
            auto value = detail::_remove_quotes(*it);
            if (!data.empty() && !value.empty()) {
                data += detail::_spaces;
            }
            data += value;
        }
        T res;
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
    try_to_tuple(detail::type_tag<std::tuple<Ts...> >,
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
    try_to_tupled_vector(std::vector<std::string> const& args, char delim) const
    {
        std::vector<T> vec;
        if (std::isspace(static_cast<unsigned char>(delim))) {
            auto const size = std::tuple_size<T>{};
            if (size == 0 || args.size() % size != 0) {
                return std::nullopt;
            }
            vec.reserve(args.size() / size);
            for (std::size_t i = 0; i < args.size(); i += size) {
                std::vector<std::string> temp
                        = { args.begin() + i, args.begin() + i + size };
                auto tuple = try_to_tuple(detail::type_tag<T>{}, temp);
                if (tuple.operator bool()) {
                    vec.emplace_back(tuple.value());
                } else {
                    return std::nullopt;
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
                    return std::nullopt;
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
            return std::nullopt;
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
        T res;
        std::stringstream ss(detail::_remove_quotes(data));
        ss >> res;
        if (ss.fail() || !ss.eof()) {
            return std::nullopt;
        }
        return res;
    }
#endif  // C++17+

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
    typedef detail::shared_ptr<Argument> pArgument;
    typedef std::vector<pArgument> pArguments;
    typedef detail::shared_ptr<_Group> pGroup;
    typedef detail::shared_ptr<ArgumentParser> pParser;

public:
    /*!
     *  \brief Subparser class
     */
    class Subparser : public _Group
    {
        friend class ArgumentParser;

        explicit
        Subparser(std::string const& title, std::string const& description)
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

        static detail::shared_ptr<Subparser>
        make_subparser(std::string const& title, std::string const& description)
        {
            return detail::make_shared<Subparser>(
                        Subparser(title, description));
        }

    public:
        using _Group::title;
        using _Group::description;

        /*!
         *  \brief Set subparser 'title' value
         *  for selected language (default: "")
         *
         *  \param value Title value
         *  \param lang Language value
         *
         *  \return Current subparser reference
         */
        inline Subparser& title(std::string const& value,
                                std::string const& lang = std::string())
        {
            m_title[lang] = value;
            return *this;
        }

        /*!
         *  \brief Set subparser 'description' value
         *  for selected language (default: "")
         *
         *  \param value Description value
         *  \param lang Language value
         *
         *  \return Current subparser reference
         */
        inline Subparser& description(std::string const& value,
                                      std::string const& lang = std::string())
        {
            m_description[lang] = value;
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
            m_prog = value;
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
            m_dest = value;
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
         *  for selected language (default: "")
         *
         *  \param value Help message
         *  \param lang Language value
         *
         *  \return Current subparser reference
         */
        inline Subparser& help(std::string const& value,
                               std::string const& lang = std::string())
        {
            if (lang.empty()) {
                m_help_type.clear();
            }
            m_help[lang] = value;
            return *this;
        }

        /*!
         *  \brief Suppress subparser 'help' message
         *
         *  \param value argparse::SUPPRESS
         *
         *  \since NEXT_RELEASE
         *
         *  \return Current subparser reference
         */
        inline Subparser& help(_SUPPRESS value)
        {
            if (value != argparse::SUPPRESS) {
                throw TypeError("got an unexpected keyword argument 'help'");
            }
            m_help_type = value;
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
            m_metavar = value;
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
        inline std::string const& help() const
        {
            return detail::_map_at(m_help, std::string());
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
         *  \brief Add argument parser with concrete name
         *
         *  \param name Parser name
         *
         *  \return Current argument parser reference
         */
        inline ArgumentParser& add_parser(std::string const& name)
        {
            if (name.empty()) {
                throw ValueError("parser name can't be empty");
            }
#ifdef _ARGPARSE_CXX_11
            m_parsers.emplace_back(make_parser(name));
#else
            m_parsers.push_back(make_parser(name));
#endif  // C++11+
            m_parsers.back()->update_prog(prog_name());
            return *m_parsers.back();
        }

        /*!
         *  \brief Get parser names
         *
         *  \return Parser names container
         */
        inline std::vector<std::string> parser_names() const
        {
            std::vector<std::string> res;
            res.reserve(m_parsers.size());
            for (std::size_t i = 0; i < m_parsers.size(); ++i) {
                res.push_back(m_parsers.at(i)->m_name);
            }
            return res;
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
            std::string program = prog_name();
            for (std::size_t i = 0; i < m_parsers.size(); ++i) {
                m_parsers.at(i)->update_prog(program);
            }
        }

        inline void
        limit_help_flags(_HelpFormatter const&,
                         std::size_t& limit) const _ARGPARSE_OVERRIDE
        {
            detail::_limit_to_min(limit, flags_to_string().size());
#ifdef _ARGPARSE_CXX_11
            for (auto const& p : m_parsers) {
                std::size_t s = p->m_name.size() + 2;
#else
            for (std::size_t i = 0; i < m_parsers.size(); ++i) {
                std::size_t s = m_parsers.at(i)->m_name.size() + 2;
#endif  // C++11+
                detail::_limit_to_min(limit, s);
            }
        }

        inline void print_help(std::ostream& os,
                               _HelpFormatter const& formatter,
                               std::string const& prog,
                               std::size_t limit,
                               std::size_t width,
                               std::string const& lang) const _ARGPARSE_OVERRIDE
        {
            std::string title = detail::_tr(m_title, lang);
            os << "\n" << (title.empty() ? "subcommands" : title) << ":\n";
            detail::_print_raw_text_formatter(
                        formatter,
                        detail::_replace(
                            detail::_tr(m_description, lang), "%(prog)s", prog),
                        width, os, std::string(), 2, "\n");
            os << print(formatter, limit, width, lang) << std::endl;
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
            for (std::size_t i = 0; i < m_parsers.size(); ++i) {
                pParser const& p = m_parsers.at(i);
                detail::_append_value_to(p->m_name, res, ",");
                for (std::size_t j = 0; j < p->aliases().size(); ++j) {
                    detail::_append_value_to(p->aliases().at(j), res, ",");
                }
            }
            return "{" + res + "}";
        }

        inline std::string print(_HelpFormatter const& formatter,
                                 std::size_t limit, std::size_t width,
                                 std::string const& lang) const
        {
            std::string res
                    = detail::_help_formatter(
                        "  " + flags_to_string(), formatter,
                        detail::_tr(m_help, lang), width, limit);
#ifdef _ARGPARSE_CXX_11
            return std::accumulate(m_parsers.begin(), m_parsers.end(),
                                   res, [formatter, limit, width, &lang]
                                   (std::string const& str, pParser const& p)
            {
                auto help = detail::_tr(p->m_help, lang);
                if (!help.empty()) {
                    std::string name = "    " + p->m_name;
                    auto alias = detail::_vector_to_string(p->aliases(), ", ");
                    if (!alias.empty()) {
                        name += " (" + alias + ")";
                    }
                    return str + "\n" + detail::_help_formatter(
                                name, formatter, help, width, limit);
                }
                return str;
            });
#else
            for (std::size_t i = 0; i < m_parsers.size(); ++i) {
                pParser const& p = m_parsers.at(i);
                std::string help = detail::_tr(p->m_help, lang);
                if (!help.empty()) {
                    std::string name = "    " + p->m_name;
                    std::string alias
                            = detail::_vector_to_string(p->aliases(), ", ");
                    if (!alias.empty()) {
                        name += " (" + alias + ")";
                    }
                    return res += "\n" + detail::_help_formatter(
                                name, formatter, help, width, limit);
                }
            }
            return res;
#endif  // C++11+
        }

        std::string m_parent_prog;
        std::string m_parent_args;
        std::string m_prog;
        std::string m_dest;
        detail::LanguagePack m_help;
        detail::Value<std::string> m_metavar;
        std::deque<pParser> m_parsers;
        detail::Value<_SUPPRESS> m_help_type;
        bool        m_required;
    };

private:
    typedef detail::shared_ptr<_ArgumentData> pArgumentData;
    typedef detail::shared_ptr<Subparser> pSubparser;
    typedef std::pair<pSubparser, std::size_t> SubparserInfo;

    inline void
    apply_formatter_class(HelpFormatter const& value)
    {
        HelpFormatter sample;
        if (!m_formatter_class._fill_text
                || sample._fill_text() != value._fill_text()) {
            m_formatter_class._fill_text = value._fill_text();
        }
        if (!m_formatter_class._get_default_metavar_for_optional
                || sample._get_default_metavar_for_optional()
                    != value._get_default_metavar_for_optional()) {
            m_formatter_class._get_default_metavar_for_optional
                    = value._get_default_metavar_for_optional();
        }
        if (!m_formatter_class._get_default_metavar_for_positional
                || sample._get_default_metavar_for_positional()
                    != value._get_default_metavar_for_positional()) {
            m_formatter_class._get_default_metavar_for_positional
                    = value._get_default_metavar_for_positional();
        }
        if (!m_formatter_class._get_help_string
                || sample._get_help_string() != value._get_help_string()) {
            m_formatter_class._get_help_string = value._get_help_string();
        }
        if (!m_formatter_class._split_lines
                || sample._split_lines() != value._split_lines()) {
            m_formatter_class._split_lines = value._split_lines();
        }
    }

    inline void
    set_formatter_class(HelpFormatter const& value)
    {
        m_formatter_class._fill_text = value._fill_text();
        m_formatter_class._get_default_metavar_for_optional
                = value._get_default_metavar_for_optional();
        m_formatter_class._get_default_metavar_for_positional
                = value._get_default_metavar_for_positional();
        m_formatter_class._get_help_string = value._get_help_string();
        m_formatter_class._split_lines = value._split_lines();
    }

    inline void read_args(int argc, char const* argv[])
    {
        if (argc > 0 && argv && argv[0]) {
            m_prog = detail::_file_name(argv[0]);
            m_parsed_arguments.reserve(std::size_t(argc - 1));
            for (int i = 1; i < argc; ++i) {
                if (!argv[i]) {
                    break;
                }
                m_parsed_arguments.push_back(std::string(argv[i]));
            }
        }
    }

    inline void init_translations()
    {
        m_usage[std::string()] = std::string();
        m_usage_title[std::string()] = "usage";
        m_description[std::string()] = std::string();
        m_positionals_title[std::string()] = "positional arguments";
        m_optionals_title[std::string()] = "options";
        m_epilog[std::string()] = std::string();
        m_help[std::string()] = std::string();
    }

public:
    /*!
     *  \brief Construct argument parser with concrete program name
     *  (default: "untitled")
     *
     *  \param prog Program name
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(std::string const& prog = "untitled")
        : m_data(_ArgumentData::make_argument_data()),
          m_name(),
          m_prog("untitled"),
          m_usage(),
          m_usage_title(),
          m_description(),
          m_positionals_title(),
          m_optionals_title(),
          m_epilog(),
          m_help(),
          m_aliases(),
          m_formatter_class(),
          m_prefix_chars(detail::_prefix_chars),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_output_width(),
          m_groups(),
          m_mutex_groups(),
          m_default_values(),
          m_parsed_arguments(),
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
        init_translations();
        set_formatter_class(HelpFormatter());
        this->prog(prog);
        m_data->update_help(true, m_prefix_chars);
    }

    /*!
     *  \brief Construct argument parser from command line arguments
     *  with concrete program name
     *  (default: "" don't override argv[0] or default program name "untitled")
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *  \param prog Program name
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(int argc, char* argv[],
                   std::string const& prog = std::string())
        : m_data(_ArgumentData::make_argument_data()),
          m_name(),
          m_prog("untitled"),
          m_usage(),
          m_usage_title(),
          m_description(),
          m_positionals_title(),
          m_optionals_title(),
          m_epilog(),
          m_help(),
          m_aliases(),
          m_formatter_class(),
          m_prefix_chars(detail::_prefix_chars),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_output_width(),
          m_groups(),
          m_mutex_groups(),
          m_default_values(),
          m_parsed_arguments(),
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
        init_translations();
        set_formatter_class(HelpFormatter());
        m_data->update_help(true, m_prefix_chars);
        read_args(argc, const_cast<char const**>(argv));
        this->prog(prog);
    }

    /*!
     *  \brief Construct argument parser from command line arguments
     *  with concrete program name
     *  (default: "" don't override argv[0] or default program name "untitled")
     *
     *  \param argc Number of command line arguments
     *  \param argv Command line arguments data
     *  \param prog Program name
     *
     *  \return Argument parser object
     */
    explicit
    ArgumentParser(int argc, char const* argv[],
                   std::string const& prog = std::string())
        : m_data(_ArgumentData::make_argument_data()),
          m_name(),
          m_prog("untitled"),
          m_usage(),
          m_usage_title(),
          m_description(),
          m_positionals_title(),
          m_optionals_title(),
          m_epilog(),
          m_help(),
          m_aliases(),
          m_formatter_class(),
          m_prefix_chars(detail::_prefix_chars),
          m_fromfile_prefix_chars(),
          m_argument_default(),
          m_output_width(),
          m_groups(),
          m_mutex_groups(),
          m_default_values(),
          m_parsed_arguments(),
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
        init_translations();
        set_formatter_class(HelpFormatter());
        m_data->update_help(true, m_prefix_chars);
        read_args(argc, argv);
        this->prog(prog);
    }

    /*!
     *  \brief Destroy argument parser
     */
    virtual ~ArgumentParser() _ARGPARSE_NOEXCEPT { }

private:
    static pParser make_parser(std::string const& name)
    {
        pParser res = detail::make_shared<ArgumentParser>(ArgumentParser());
        res->m_prog.clear();
        res->m_name = name;
        return res;
    }

public:
    /*!
     *  \brief Set argument parser 'prog' value (default: argv[0] or "untitled")
     *
     *  \param value Program name
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& prog(std::string const& value)
    {
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
     *  for selected language (default: "")
     *
     *  \param value Usage value
     *  \param lang Language value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& usage(std::string const& value,
                                 std::string const& lang = std::string())
    {
        m_usage[lang] = value;
        return *this;
    }

    /*!
     *  \brief Set title for argument parser 'usage' (default: "usage")
     *  for selected language (default: "")
     *
     *  \param value Title for usage
     *  \param lang Language value
     *
     *  \since v1.7.1
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& usage_title(std::string const& value,
                                       std::string const& lang = std::string())
    {
        if (!value.empty()) {
            m_usage_title[lang] = value;
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'description' value
     *  for selected language (default: "")
     *
     *  \param value Description value
     *  \param lang Language value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& description(std::string const& value,
                                       std::string const& lang = std::string())
    {
        m_description[lang] = value;
        return *this;
    }

    /*!
     *  \brief Set title for positional arguments
     *  (default: "positional arguments") for selected language (default: "")
     *
     *  \param value Title for positional arguments
     *  \param lang Language value
     *
     *  \since v1.7.0
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    positionals_title(std::string const& value,
                      std::string const& lang = std::string())
    {
        if (!value.empty()) {
            m_positionals_title[lang] = value;
        }
        return *this;
    }

    /*!
     *  \brief Set title for optional arguments
     *  (default: "options") for selected language (default: "")
     *
     *  \param value Title for optional arguments
     *  \param lang Language value
     *
     *  \since v1.7.0
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser&
    optionals_title(std::string const& value,
                    std::string const& lang = std::string())
    {
        if (!value.empty()) {
            m_optionals_title[lang] = value;
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'epilog' value
     *  for selected language (default: "")
     *
     *  \param value Epilog value
     *  \param lang Language value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& epilog(std::string const& value,
                                  std::string const& lang = std::string())
    {
        m_epilog[lang] = value;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'help' message (for subparsers)
     *  for selected language (default: "")
     *
     *  \param value Help message
     *  \param lang Language value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& help(std::string const& value,
                                std::string const& lang = std::string())
    {
        m_help[lang] = value;
        return *this;
    }

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value Aliases values
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& aliases(std::vector<std::string> const& value)
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
    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value First value
     *  \param args Other values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser& aliases(std::string const& value, Args... args)
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
    ArgumentParser& aliases(std::initializer_list<std::string> value)
    {
        return aliases(std::vector<std::string>{ value });
    }
#else
    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value First value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& aliases(std::string const& value)
    {
        return aliases(detail::_make_vector(value));
    }

    /*!
     *  \brief Set argument parser 'aliases' value (for subparsers)
     *
     *  \param value1 First value
     *  \param value2 Second value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& aliases(std::string const& value1,
                                   std::string const& value2)
    {
        return aliases(detail::_make_vector(value1, value2));
    }
#endif  // C++11+

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value Parents values
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& parents(std::vector<ArgumentParser> const& value)
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
            for (std::size_t j = 0; j < parent.m_groups.size(); ++j) {
                m_groups.push_back(parent.m_groups.at(j));
            }
            for (std::size_t j = 0; j < parent.m_mutex_groups.size(); ++j) {
                m_mutex_groups.push_back(parent.m_mutex_groups.at(j));
            }
            for (std::size_t j = 0; j < parent.m_default_values.size(); ++j) {
                m_default_values.push_back(parent.m_default_values.at(j));
            }
        }
        return *this;
    }

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
    ArgumentParser& parents(ArgumentParser const& value, Args... args)
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
    ArgumentParser& parents(std::initializer_list<ArgumentParser> value)
    {
        return parents(std::vector<ArgumentParser>{ value });
    }
#else
    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value Parent value
     *
     *  \return Current argument parser reference
     */
    ArgumentParser& parents(ArgumentParser const& value)
    {
        return parents(detail::_make_vector(value));
    }

    /*!
     *  \brief Set argument parser 'parents' value
     *
     *  \param value1 Parent value1
     *  \param value2 Parent value2
     *
     *  \return Current argument parser reference
     */
    ArgumentParser&
    parents(ArgumentParser const& value1, ArgumentParser const& value2)
    {
        return parents(detail::_make_vector(value1, value2));
    }
#endif  // C++11+

    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param value HelpFormatter value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& formatter_class(HelpFormatter const& value)
    {
        set_formatter_class(value);
        return *this;
    }

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param value HelpFormatter value
     *  \param args HelpFormatter values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser&
    formatter_class(HelpFormatter const& value, Args... args)
    {
        formatter_class(value);
        return add_formatter_class(args...);
    }
#else
    /*!
     *  \brief Set argument parser 'formatter_class' value
     *
     *  \param value1 HelpFormatter value1
     *  \param value2 HelpFormatter value2
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& formatter_class(HelpFormatter const& value1,
                                           HelpFormatter const& value2)
    {
        formatter_class(value1);
        return add_formatter_class(value2);
    }
#endif  // C++11+

    /*!
     *  \brief Add argument parser 'formatter_class' value
     *
     *  \param value HelpFormatter value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& add_formatter_class(HelpFormatter const& value)
    {
        apply_formatter_class(value);
        return *this;
    }

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Add argument parser 'formatter_class' value
     *
     *  \param value HelpFormatter value
     *  \param args HelpFormatter values
     *
     *  \return Current argument parser reference
     */
    template <class... Args>
    ArgumentParser&
    add_formatter_class(HelpFormatter const& value, Args... args)
    {
        add_formatter_class(value);
        return add_formatter_class(args...);
    }
#else
    /*!
     *  \brief Add argument parser 'formatter_class' value
     *
     *  \param value1 HelpFormatter value1
     *  \param value2 HelpFormatter value2
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& add_formatter_class(HelpFormatter const& value1,
                                               HelpFormatter const& value2)
    {
        add_formatter_class(value1);
        return add_formatter_class(value2);
    }
#endif  // C++11+

    /*!
     *  \brief Set argument parser 'prefix_chars' value (default: "-")
     *
     *  \param value Prefix chars values (only punctuation characters)
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& prefix_chars(std::string const& value)
    {
        std::string val = detail::_get_punct(value);
        if (!val.empty()) {
            m_prefix_chars = val;
            m_data->update_help(m_data->m_add_help, m_prefix_chars);
        }
        return *this;
    }

    /*!
     *  \brief Set argument parser 'fromfile_prefix_chars' value
     *
     *  \param value Fromfile prefix chars values (only punctuation characters)
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& fromfile_prefix_chars(std::string const& value)
    {
        m_fromfile_prefix_chars = detail::_get_punct(value);
        return *this;
    }

    /*!
     *  \brief Set argument parser 'argument_default' value
     *
     *  \param value Argument default value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& argument_default(std::string const& value)
    {
        m_argument_default = value;
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
     *  \param value Argument default value
     *
     *  \return Current argument parser reference
     */
    inline ArgumentParser& conflict_handler(std::string const& value)
    {
        if (value != "resolve") {
            throw AttributeError("'ArgumentParser' object has no attribute "
                                 "'_handle_conflict_" + value + "'");
        }
        m_data->m_conflict_handler = value;
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
        m_data->update_help(value, m_prefix_chars);
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
    inline std::string const& usage() const
    {
        return detail::_map_at(m_usage, std::string());
    }

    /*!
     *  \brief Get title for argument parser 'usage' (default: "usage")
     *
     *  \since v1.7.1
     *
     *  \return Title for argument parser 'usage'
     */
    inline std::string const& usage_title() const
    {
        return detail::_map_at(m_usage_title, std::string());
    }

    /*!
     *  \brief Get argument parser 'description' value
     *
     *  \return Argument parser 'description' value
     */
    inline std::string const& description() const
    {
        return detail::_map_at(m_description, std::string());
    }

    /*!
     *  \brief Get title for positional arguments
     *  (default: "positional arguments")
     *
     *  \since v1.7.0
     *
     *  \return Title for positional arguments
     */
    inline std::string const& positionals_title() const
    {
        return detail::_map_at(m_positionals_title, std::string());
    }

    /*!
     *  \brief Get title for optional arguments (default: "options")
     *
     *  \since v1.7.0
     *
     *  \return Title for optional arguments
     */
    inline std::string const& optionals_title() const
    {
        return detail::_map_at(m_optionals_title, std::string());
    }

    /*!
     *  \brief Get argument parser 'epilog' value
     *
     *  \return Argument parser 'epilog' value
     */
    inline std::string const& epilog() const
    {
        return detail::_map_at(m_epilog, std::string());
    }

    /*!
     *  \brief Get argument parser 'help' message (for subparsers)
     *
     *  \return Argument parser 'help' message
     */
    inline std::string const& help() const
    {
        return detail::_map_at(m_help, std::string());
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
        return m_data->m_conflict_handler;
    }

    /*!
     *  \brief Get argument parser 'add_help' value (default: true)
     *
     *  \return Argument parser 'add_help' value
     */
    inline bool add_help() const _ARGPARSE_NOEXCEPT
    {
        return m_data->m_add_help;
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

#ifdef _ARGPARSE_CXX_11
    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flag values
     *
     *  \return Current argument reference
     */
    template <class... Args>
    Argument& add_argument(Args... flags)
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
    Argument& add_argument(std::initializer_list<std::string> flags)
    {
        return add_argument(std::vector<std::string>{ flags });
    }
#else
    /*!
     *  \brief Add argument with flag
     *
     *  \param flag Flag value
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::string const& flag)
    {
        return add_argument(detail::_make_vector(flag));
    }

    /*!
     *  \brief Add argument with 2 flags
     *
     *  \param flag1 First value
     *  \param flag2 Second value
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::string const& flag1,
                                  std::string const& flag2)
    {
        return add_argument(detail::_make_vector(flag1, flag2));
    }

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
    inline Argument& add_argument(std::string const& flag1,
                                  std::string const& flag2,
                                  std::string const& flag3)
    {
        return add_argument(detail::_make_vector(flag1, flag2, flag3));
    }

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
    inline Argument& add_argument(std::string const& flag1,
                                  std::string const& flag2,
                                  std::string const& flag3,
                                  std::string const& flag4)
    {
        return add_argument(detail::_make_vector(flag1, flag2, flag3, flag4));
    }
#endif  // C++11+

    /*!
     *  \brief Add argument with flags
     *
     *  \param flags Flags values
     *
     *  \return Current argument reference
     */
    inline Argument& add_argument(std::vector<std::string> const& flags)
    {
        m_data->create_argument(m_data, flags, prefix_chars());
        process_add_argument();
        return *m_data->m_arguments.back();
    }

    /*!
     *  \brief Add argument
     *
     *  \param argument Argument
     *
     *  \return Current parser reference
     */
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
    inline ArgumentParser& add_argument(Argument const& argument)
    {
        m_data->validate_argument(Argument(argument), prefix_chars());
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
        detail::shared_ptr<ArgumentGroup> group
                = ArgumentGroup::make_argument_group(
                    title, description, m_prefix_chars, m_data,
                    m_argument_default, m_argument_default_type);
        m_groups.push_back(pGroup(group));
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
#ifdef _ARGPARSE_CXX_11
        m_mutex_groups.emplace_back(
              MutuallyExclusiveGroup::make_mutex_group(
                        m_prefix_chars, m_data,
                        m_argument_default, m_argument_default_type));
#else
        m_mutex_groups.push_back(
              MutuallyExclusiveGroup::make_mutex_group(
                        m_prefix_chars, m_data,
                        m_argument_default, m_argument_default_type));
#endif  // C++11+
        return m_mutex_groups.back().required(required);
    }

    /*!
     *  \brief Add subparsers. Only arguments of one subparser are allowed
     *
     *  \param title Subparsers title (default: "")
     *  \param description Subparsers description (default: "")
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
        m_subparsers_position = m_data->m_positional.size();
        m_subparsers = Subparser::make_subparser(title, description);
        m_subparsers->update_prog(prog(), subparser_prog_args());
        m_groups.push_back(pGroup(m_subparsers));
        return *m_subparsers;
    }

    /*!
     *  \brief Get subparsers.
     *  If no subparsers are present, returns nullptr
     *
     *  \return Current subparser pointer or nullptr
     */
    inline Subparser* subparsers() const _ARGPARSE_NOEXCEPT
    {
        return m_subparsers.get();
    }

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
#endif  // C++11+

    /*!
     *  \brief Get the default value for a specific argument.
     *  If argument not found, returns empty string.
     *
     *  \param dest Argument destination name or flag
     *
     *  \return Default value for a specific argument
     */
    inline std::string get_default(std::string const& dest) const
    {
        pArguments const positional = m_data->get_positional(true, true);
        pArguments const optional = m_data->get_optional(true, true);
        for (std::size_t i = 0; i < positional.size(); ++i) {
            pArgument const& arg = positional.at(i);
            if (detail::_is_value_exists(dest, arg->m_flags)) {
                if (arg->is_suppressed()) {
                    return detail::_suppress;
                }
                return arg->m_default();;
            }
        }
        for (std::size_t i = 0; i < optional.size(); ++i) {
            pArgument const& arg = optional.at(i);
            if (!arg->dest().empty()) {
                if (arg->dest() == dest) {
                    if (arg->is_suppressed()) {
                        return detail::_suppress;
                    }
                    return arg->m_default();
                }
            } else {
                for (std::size_t j = 0; j < arg->m_flags.size(); ++j) {
                    std::string const& flag = arg->m_flags.at(j);
                    std::string name = detail::_flag_name(flag);
                    if (flag == dest || name == dest) {
                        if (arg->is_suppressed()) {
                            return detail::_suppress;
                        }
                        return arg->m_default();
                    }
                }
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
    /*!
     *  \brief Set default values for a specific arguments
     *
     *  \param pairs List of pairs: { 'argument flag', 'default value' }
     *
     *  \since v1.7.2
     */
    inline void
    set_defaults(
              std::initializer_list<std::pair<std::string, std::string> > pairs)
    {
        set_defaults(
                    std::vector<std::pair<std::string, std::string> >{ pairs });
    }
#endif  // C++11+

    /*!
     *  \brief Set default values for a specific arguments
     *
     *  \param pairs Vector of pairs: { 'argument flag', 'default value' }
     */
    inline void
    set_defaults(std::vector<std::pair<std::string, std::string> > const& pairs)
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

    /*!
     *  \brief Parse command line arguments
     *
     *  \param space Parsed arguments namespace
     *
     *  \return Object with parsed arguments
     */
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
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
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_args(T const& args,
                         Namespace const& space = Namespace()) const
#else
    template <class T>
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    inline Namespace parse_args(std::initializer_list<std::string> const& args,
                                Namespace const& space = Namespace()) const
    {
        return parse_args(std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

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
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
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
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_known_args(T const& args,
                               Namespace const& space = Namespace()) const
#else
    template <class T>
    Namespace parse_known_args(
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    inline Namespace
    parse_known_args(std::initializer_list<std::string> const& args,
                     Namespace const& space = Namespace()) const
    {
        return parse_known_args(std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

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
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
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
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace parse_intermixed_args(T const& args,
                                    Namespace const& space = Namespace()) const
#else
    template <class T>
    Namespace parse_intermixed_args(
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    inline Namespace
    parse_intermixed_args(std::initializer_list<std::string> const& args,
                          Namespace const& space = Namespace()) const
    {
        return parse_intermixed_args(std::vector<std::string>{ args }, space);
    }
#endif  // C++11+

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
#ifdef _ARGPARSE_CXX_11
    template <typename = void>
#endif  // C++11+
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
#ifdef _ARGPARSE_CXX_11
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    Namespace
    parse_known_intermixed_args(T const& args,
                                Namespace const& space = Namespace()) const
#else
    template <class T>
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments
     */
    inline Namespace
    parse_known_intermixed_args(std::initializer_list<std::string> const& args,
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

#ifdef _ARGPARSE_CXX_17
    /*!
     *  \brief Try parse command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <typename = void>
    std::optional<Namespace>
    try_parse_args(Namespace const& space = Namespace()) const
    {
        return try_parse_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Try parse concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    std::optional<Namespace>
    try_parse_args(T const& args,
                   Namespace const& space = Namespace()) const
    {
        return try_parse_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Try parse concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_args(std::initializer_list<std::string> const& args,
                   Namespace const& space = Namespace()) const
    {
        return try_parse_args(std::vector<std::string>{ args }, space);
    }

    /*!
     *  \brief Try parse concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_args(std::vector<std::string> const& args,
                   Namespace const& space = Namespace()) const
    {
        return on_try_parse_arguments(args, false, false, space);
    }

    /*!
     *  \brief Try parse known command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <typename = void>
    std::optional<Namespace>
    try_parse_known_args(Namespace const& space = Namespace()) const
    {
        return try_parse_known_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Try parse known concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    std::optional<Namespace>
    try_parse_known_args(T const& args,
                         Namespace const& space = Namespace()) const
    {
        return try_parse_known_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Try parse known concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_known_args(std::initializer_list<std::string> const& args,
                         Namespace const& space = Namespace()) const
    {
        return try_parse_known_args(std::vector<std::string>{ args }, space);
    }

    /*!
     *  \brief Try parse known concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_known_args(std::vector<std::string> const& args,
                         Namespace const& space = Namespace()) const
    {
        return on_try_parse_arguments(args, true, false, space);
    }

    /*!
     *  \brief Try parse intermixed command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <typename = void>
    std::optional<Namespace>
    try_parse_intermixed_args(Namespace const& space = Namespace()) const
    {
        return try_parse_intermixed_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Try parse intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    std::optional<Namespace>
    try_parse_intermixed_args(T const& args,
                              Namespace const& space = Namespace()) const
    {
        return try_parse_intermixed_args(detail::_split_to_args(args), space);
    }

    /*!
     *  \brief Try parse intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_intermixed_args(std::initializer_list<std::string> const& args,
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_intermixed_args(std::vector<std::string> const& args,
                              Namespace const& space = Namespace()) const
    {
        return on_try_parse_arguments(args, false, true, space);
    }

    /*!
     *  \brief Try parse known intermixed command line arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <typename = void>
    std::optional<Namespace>
    try_parse_known_intermixed_args(Namespace const& space = Namespace()) const
    {
        return try_parse_known_intermixed_args(m_parsed_arguments, space);
    }

    /*!
     *  \brief Try parse known intermixed concrete arguments.
     *  If arguments can't be parsed, returns std::nullopt.
     *
     *  \param args Arguments to parse
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    template <class T,
              typename std::enable_if<
                 std::is_constructible<std::string, T>::value>::type* = nullptr>
    std::optional<Namespace>
    try_parse_known_intermixed_args(T const& args,
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.2
     *
     *  \return Object with parsed arguments or std::nullopt
     */
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
     *  \param space Parsed arguments namespace
     *
     *  \since v1.7.1
     *
     *  \return Object with parsed arguments or std::nullopt
     */
    inline std::optional<Namespace>
    try_parse_known_intermixed_args(std::vector<std::string> const& args,
                                    Namespace const& space = Namespace()) const
    {
        return on_try_parse_arguments(args, true, true, space);
    }
#endif  // C++17+

    /*!
     *  \brief Run self-test and print report
     *  to output stream (default: std::cout)
     *
     *  \param os Output stream
     *
     *  \since v1.7.2
     *
     *  \return True if no warnings or errors were found, false otherwise
     */
    inline bool self_test(std::ostream& os = std::cout) const
    {
        return self_test(std::string(), os);
    }

    /*!
     *  \brief Run self-test and print report for selected language
     *  to output stream (default: std::cout)
     *
     *  \param lang Language value
     *  \param os Output stream
     *
     *  \since v1.7.2
     *
     *  \return True if no warnings or errors were found, false otherwise
     */
    inline bool self_test(std::string const& lang,
                          std::ostream& os = std::cout) const
    {
        std::size_t const limit = 79;
        char const filler = '-';
        std::stringstream ss;
        ss << "cpp-argparse v"
           << ARGPARSE_VERSION_MAJOR << "."
           << ARGPARSE_VERSION_MINOR << "."
           << ARGPARSE_VERSION_PATCH << " self-test report";
        os << detail::_filled_string(ss.str(), limit, filler) << std::endl;
        os << detail::_filled_string(
                  "This report contains parser information and displays "
                  "diagnostic warnings", limit, detail::_space) << std::endl;
        os << detail::_filled_string("overview", limit, filler) << std::endl;
        test_overview(lang, os);
        os << detail::_filled_string("diagnostics", limit, filler) << std::endl;
        bool res = test_diagnostics(lang, os);
        os << detail::_filled_string("end report", limit, filler) << std::endl;
        return res;
    }

    /*!
     *  \brief Print a bash completion to output stream (default: std::cout).
     *  Copy the contents to ~/.bashrc or create a script file and use it
     *
     *  \param os Output stream
     *
     *  \since v1.7.2
     */
    inline void print_bash_completion(std::ostream& os = std::cout) const
    {
        pArguments const optional = m_data->get_optional(false, true);
        pArguments const positional = m_data->get_positional(false, true);
        std::vector<std::string> options;
        for (std::size_t i = 0; i < optional.size(); ++i) {
            detail::_insert_vector_to_end(optional.at(i)->flags(), options);
        }
        bool more_args = false;
        std::size_t min_args = 0;
        std::size_t one_args = 0;
        for (std::size_t i = 0; i < positional.size(); ++i) {
            pArgument const& arg = positional.at(i);
            if (!(arg->action() & detail::_store_action)) {
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
                case Argument::REMAINDING :
                    more_args = true;
                    break;
                default :
                    min_amount += arg->m_num_args;
                    break;
            }
            min_args += min_amount;
        }
        if (m_subparsers) {
            detail::_insert_vector_to_end(
                        m_subparsers->parser_names(), options);
        }
        bool have_positional = more_args || min_args != 0 || one_args != 0;
        if (!options.empty() || have_positional) {
            os << "complete";
            if (have_positional) {
                os << " -f";
            }
            if (!options.empty()) {
                os << " -W \"" << detail::_vector_to_string(options) << "\"";
            }
            os << " " << prog();
            os << std::endl;
        }
    }

    /*!
     *  \brief Print a program usage to output stream (default: std::cout)
     *
     *  \param os Output stream
     */
    inline void print_usage(std::ostream& os = std::cout) const
    {
        print_usage(std::string(), os);
    }

    /*!
     *  \brief Print a program usage for selected language
     *  to output stream (default: std::cout)
     *
     *  \param lang Language value
     *  \param os Output stream
     *
     *  \since v1.7.1
     */
    inline void print_usage(std::string const& lang,
                            std::ostream& os = std::cout) const
    {
        std::string tr_usage_title = detail::_tr(m_usage_title, lang) + ":";
        std::string tr_usage = detail::_tr(m_usage, lang);
        if (!tr_usage.empty()) {
            os << tr_usage_title << " " << despecify(tr_usage) << std::endl;
        } else {
            pArguments const positional = m_data->get_positional(false, true);
            pArguments const optional = m_data->get_optional(false, true);
            print_custom_usage(positional, optional, m_mutex_groups,
                             subparser_info(false), prog(), tr_usage_title, os);
        }
    }

    /*!
     *  \brief Print a help message to output stream (default: std::cout)
     *
     *  \param os Output stream
     */
    inline void print_help(std::ostream& os = std::cout) const
    {
        print_help(std::string(), os);
    }

    /*!
     *  \brief Print a help message for selected language
     *  to output stream (default: std::cout)
     *
     *  \param lang Language value
     *  \param os Output stream
     *
     *  \since v1.7.1
     */
    inline void print_help(std::string const& lang,
                           std::ostream& os = std::cout) const
    {
        pArguments const positional_all = m_data->get_positional(false, true);
        pArguments const optional_all = m_data->get_optional(false, true);
        pArguments const positional = m_data->get_positional(false, false);
        pArguments const optional = m_data->get_optional(false, false);
        SubparserInfo const sub_info = subparser_info(false);
        std::string tr_usage_title = detail::_tr(m_usage_title, lang) + ":";
        std::string tr_usage = detail::_tr(m_usage, lang);
        if (!tr_usage.empty()) {
            os << tr_usage_title << " " << despecify(tr_usage) << std::endl;
        } else {
            print_custom_usage(positional_all, optional_all, m_mutex_groups,
                               sub_info, prog(), tr_usage_title, os);
        }
        std::size_t width = output_width();
        detail::_print_raw_text_formatter(
                    m_formatter_class,
                    despecify(detail::_tr(m_description, lang)),
                    width, os);
        std::size_t size = 0;
        pSubparser subparser = sub_info.first;
        bool sub_positional = is_subparser_positional(subparser);
        for (std::size_t i = 0; i < positional.size(); ++i) {
            std::string flags
                    = positional.at(i)->flags_to_string(m_formatter_class);
            detail::_limit_to_min(size, detail::_utf8_length(flags).second);
        }
        for (std::size_t i = 0; i < optional.size(); ++i) {
            std::string flags
                    = optional.at(i)->flags_to_string(m_formatter_class);
            detail::_limit_to_min(size, detail::_utf8_length(flags).second);
        }
        for (std::size_t i = 0; i < m_groups.size(); ++i) {
            m_groups.at(i)->limit_help_flags(m_formatter_class, size);
        }
        size += 4;
        detail::_limit_to_max(size, argument_name_limit());
        if (!positional.empty() || sub_positional) {
            os << "\n" << detail::_tr(m_positionals_title, lang) << ":\n";
            for (std::size_t i = 0; i < positional.size(); ++i) {
                print_subparser(sub_positional, sub_info, i,
                                m_formatter_class, size, width, lang, os);
                os << despecify(positional.at(i)->print(m_formatter_class,
                                                        size, width, lang))
                   << std::endl;
            }
            print_subparser(sub_positional, sub_info, positional.size(),
                            m_formatter_class, size, width, lang, os);
        }
        if (!optional.empty()) {
            os << "\n" << detail::_tr(m_optionals_title, lang) << ":\n";
            for (std::size_t i = 0; i < optional.size(); ++i) {
                os << despecify(optional.at(i)->print(
                                    m_formatter_class, size, width, lang))
                   << std::endl;
            }
        }
#ifdef _ARGPARSE_CXX_11
        for (auto const& group : m_groups) {
#else
        for (std::size_t i = 0; i < m_groups.size(); ++i) {
            pGroup const& group = m_groups.at(i);
#endif  // C++11+
            print_group(group, subparser, sub_positional, m_formatter_class,
                        prog(), size, width, lang, os);
        }
        detail::_print_raw_text_formatter(
                    m_formatter_class,
                    despecify(detail::_tr(m_epilog, lang)),
                    width, os);
    }

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
    inline std::string
    format_bash_completion() const
    {
        std::stringstream ss;
        print_bash_completion(ss);
        return detail::_trim_copy(ss.str());
    }

    /*!
     *  \brief Return a string containing a program usage
     *  for selected language (default: "")
     *
     *  \param lang Language value
     *
     *  \return Program usage for selected language
     */
    inline std::string
    format_usage(std::string const& lang = std::string()) const
    {
        std::stringstream ss;
        print_usage(lang, ss);
        return detail::_trim_copy(ss.str());
    }

    /*!
     *  \brief Return a string containing a help message
     *  for selected language (default: "")
     *
     *  \param lang Language value
     *
     *  \return Help message for selected language
     */
    inline std::string
    format_help(std::string const& lang = std::string()) const
    {
        std::stringstream ss;
        print_help(lang, ss);
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
#ifdef _ARGPARSE_CXX_11
        return { arg_line };
#else
        return detail::_make_vector(arg_line);
#endif  // C++11+
    }

private:
    inline void
    throw_error(std::string const& message,
                std::string const& lang = std::string(),
                std::ostream& os = std::cerr) const
    {
        print_usage(lang, os);
        throw std::logic_error(prog() + ": error: " + message);
    }

    inline std::string despecify(std::string const& str) const
    {
        return detail::_replace(str, "%(prog)s", prog());
    }

    inline void process_add_argument()
    {
        bool optional
                = m_data->m_arguments.back()->m_type == Argument::Optional;
        if (m_argument_default.has_value()
                && !m_data->m_arguments.back()->m_default.has_value()
                && !m_data->m_arguments.back()->m_default_type.has_value()) {
            m_data->m_arguments.back()->default_value(m_argument_default());
        }
        if (m_argument_default_type.has_value()) {
            m_data->m_arguments.back()->default_value(
                        m_argument_default_type());
        }
        (optional ? m_data->m_optional : m_data->m_positional)
                .push_back(std::make_pair(m_data->m_arguments.back(), false));
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
            std::cerr << "argparse error: unexpected error" << std::endl;
        }
        ::exit(1);
    }

#ifdef _ARGPARSE_CXX_17
    inline std::optional<Namespace>
    on_try_parse_arguments(std::vector<std::string> const& args,
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

    inline std::vector<std::string>
    read_args_from_file(std::vector<std::string> const& arguments) const
    {
        std::vector<std::string> res = arguments;
        if (!fromfile_prefix_chars().empty()) {
            for (std::size_t i = 0; i < res.size(); ++i) {
                while (!res.at(i).empty()
                       && detail::_is_value_exists(res.at(i).at(0),
                                                   fromfile_prefix_chars())) {
                    std::string file = res.at(i).substr(1);
#ifdef _ARGPARSE_CXX_11
                    std::ifstream is(file);
#else
                    std::ifstream is(file.c_str());
#endif  // C++11+
                    if (!is.is_open()) {
                        throw_error("[Errno 2] No such file or directory: '"
                                    + file + "'");
                    }
                    std::vector<std::string> args;
                    std::string line;
                    while (std::getline(is, line)) {
                        std::vector<std::string> line_args
                                = convert_arg_line_to_args(line);
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
                   pArguments optional,
                   _Storage const& storage,
                   SubparserInfo const& subparser)
            : parser(parser),
              optional(optional),
              storage(storage),
              subparser(subparser),
              lang(),
              have_negative_args()
        {
            have_negative_args
                 = negative_numbers_presented(optional, parser->prefix_chars());
        }

        ParserInfo(ParserInfo const& orig)
            : parser(orig.parser),
              optional(orig.optional),
              storage(orig.storage),
              subparser(orig.subparser),
              lang(orig.lang),
              have_negative_args(orig.have_negative_args)
        { }

        inline ParserInfo& operator =(ParserInfo const& rhs)
        {
            if (this != &rhs) {
                parser              = rhs.parser;
                optional            = rhs.optional;
                storage             = rhs.storage;
                subparser           = rhs.subparser;
                lang                = rhs.lang;
                have_negative_args  = rhs.have_negative_args;
            }
            return *this;
        }

        ArgumentParser const* parser;
        pArguments optional;
        _Storage storage;
        SubparserInfo subparser;
        std::string lang;
        bool have_negative_args;
    };

    typedef std::deque<ParserInfo> Parsers;

    inline Namespace
    parse_arguments(std::vector<std::string> const& in_parsed_arguments,
                    bool only_known,
                    bool intermixed,
                    Namespace const& space) const
    {
#ifdef _ARGPARSE_CXX_11
        handle(prog());
#endif  // C++11+
        check_namespace(space);
        std::vector<std::string> parsed_arguments
                = read_args_from_file(in_parsed_arguments);

        Parsers parsers;
        parsers.push_back(ParserInfo(this, m_data->get_optional(true, true),
                                     space.storage(), subparser_info(true)));

        check_mutex_arguments();
        check_intermixed_subparser(intermixed, parsers.back().subparser.first);

        pArguments positional = m_data->get_positional(true, true);
        check_intermixed_remainder(intermixed, positional);

        bool was_pseudo_arg = false;

        validate_arguments(m_data->get_arguments(true));
        parsers.back().storage.create(m_data->get_arguments(true));

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
            bool remainder = is_remainder_positional(pos, positional, parsers);
            std::string arg = parsed_arguments.at(i);
            std::vector<std::string> equals = process_split_equal(arg, parsers);
            pArgument const tmp
                    = get_optional_arg_by_flag(was_pseudo_arg, parsers, arg);
            if (tmp && !remainder) {
                switch (tmp->action()) {
                    case argparse::store :
                        parsers.front().storage.at(tmp).clear();
                        // fallthrough
                    case argparse::append :
                    case argparse::extend :
                        storage_optional_store(parsers, equals,
                                               parsed_arguments, i,
                                               was_pseudo_arg, arg, tmp);
                        break;
                    case argparse::help :
                        process_optional_help(parsers, equals, arg);
                        break;
                    case argparse::version :
                        process_optional_version(parsers, equals, arg, tmp);
                        break;
                    case argparse::language :
                        parsers.front().storage.at(tmp).clear();
                        storage_optional_store(parsers, equals,
                                               parsed_arguments, i,
                                               was_pseudo_arg, arg, tmp);
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
#ifdef _ARGPARSE_CXX_11
        namespace_post_trigger(parsers, only_known, unrecognized_args);
        return create_namespace(only_known, std::move(parsers.front().storage),
                                std::move(unrecognized_args));
#else
        return create_namespace(only_known, parsers.front().storage,
                                unrecognized_args);
#endif  // C++11+
    }

    inline void
    check_namespace(Namespace const& space) const
    {
        if (space.m_unrecognized_args.has_value()
                && !m_data->m_arguments.empty()) {
            pArgument const& arg = m_data->m_arguments.front();
            throw AttributeError(
                   "'tuple' object has no attribute '" + arg->get_dest() + "'");
        }
    }

    inline void check_mutex_arguments() const
    {
#ifdef _ARGPARSE_CXX_11
        for (auto const& group : m_mutex_groups) {
            for (auto const& arg : group.m_data->m_arguments) {
#else
        for (std::size_t i = 0; i < m_mutex_groups.size(); ++i) {
            MutuallyExclusiveGroup const& group = m_mutex_groups.at(i);
            for (std::size_t j = 0; j < group.m_data->m_arguments.size(); ++j) {
                pArgument const& arg = group.m_data->m_arguments.at(j);
#endif  // C++11+
                if (arg->required()) {
                    throw
                    ValueError("mutually exclusive arguments must be optional");
                }
            }
        }
    }

    static void
    check_intermixed_subparser(bool intermixed, pSubparser const& subparser)
    {
        if (intermixed && subparser) {
            throw
            TypeError("parse_intermixed_args: positional arg with nargs=A...");
        }
    }

    static void
    check_intermixed_remainder(bool intermixed, pArguments const& positional)
    {
#ifdef _ARGPARSE_CXX_11
        if (intermixed
                && std::any_of(positional.begin(), positional.end(),
                               [] (pArgument const& arg)
        { return arg->m_nargs == Argument::REMAINDING; })) {
            throw
            TypeError("parse_intermixed_args: positional arg with nargs=...");
        }
#else
        if (intermixed) {
            for (std::size_t i = 0; i < positional.size(); ++i) {
                if (positional.at(i)->m_nargs == Argument::REMAINDING) {
                    throw
                    TypeError(
                        "parse_intermixed_args: positional arg with nargs=...");
                }
            }
        }
#endif  // C++11+
    }

    inline std::vector<std::string>
    process_split_equal(std::string& arg, Parsers const& parsers) const
    {
        std::vector<std::string> equals
             = detail::_split_equal(arg, parsers.back().parser->prefix_chars());
        if (equals.size() == 2 && !equals.front().empty()) {
            arg = equals.front();
        } else {
            equals.resize(1);
        }
        return equals;
    }

#ifdef _ARGPARSE_CXX_11
    static Namespace
    create_namespace(bool only_known,
                     _Storage&& storage,
                     std::vector<std::string>&& unrecognized_args)
    {
        if (only_known) {
            return Namespace(std::move(storage), std::move(unrecognized_args));
        } else {
            return Namespace(std::move(storage));
        }
    }
#else
    static Namespace
    create_namespace(bool only_known,
                     _Storage const& storage,
                     std::vector<std::string> const& unrecognized_args)
    {
        if (only_known) {
            return Namespace(storage, unrecognized_args);
        } else {
            return Namespace(storage);
        }
    }
#endif  // C++11+

    static bool
    negative_numbers_presented(pArguments const& optionals,
                               std::string const& prefix_chars)
    {
        if (detail::_is_value_exists(detail::_prefix_char, prefix_chars)) {
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

    static void validate_arguments(pArguments const& args)
    {
#ifdef _ARGPARSE_CXX_11
        for (auto const& arg : args) {
            arg->validate();
        }
#else
        for (std::size_t i = 0; i < args.size(); ++i) {
            args.at(i)->validate();
        }
#endif  // C++11+
    }

    inline void validate_argument_value(Parsers const& parsers,
                                        Argument const& arg,
                                        std::string const& value) const
    {
        detail::Value<std::vector<std::string> > const& choices = arg.m_choices;
        if (choices.has_value()) {
            std::string str = detail::_remove_quotes(value);
            if (!str.empty() && !detail::_is_value_exists(str, choices())) {
                std::string values
                        = detail::_vector_to_string(choices(), ", ", "'");
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
#ifdef _ARGPARSE_CXX_11
        for (auto const& val : values) {
            validate_argument_value(parsers, *arg, val);
        }
#else
        for (std::size_t i = 0; i < values.size(); ++i) {
            validate_argument_value(parsers, *arg, values.at(i));
        }
#endif  // C++11+
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
        detail::Value<std::string> const& value = arg->m_default;
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
                            storage_store_value(parsers,
                                                tmp, tmp->const_value());
                        }
                    } else if (tmp->action() == argparse::extend) {
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

    inline void storage_optional_store(Parsers& parsers,
                                       std::vector<std::string> const& equals,
                                       std::vector<std::string> const& args,
                                       std::size_t& i,
                                       bool was_pseudo_arg,
                                       std::string const& arg,
                                       pArgument const& tmp) const
    {
        if (equals.size() == 1) {
            std::size_t n = 0;
            std::vector<std::string> values;
            do {
                if (++i == args.size()) {
                    storage_optional_store_func(parsers, arg, tmp, n);
                    break;
                } else {
                    std::string const& next = args.at(i);
                    if (next.empty()
                            || tmp->m_nargs == Argument::REMAINDING
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
            } while (!(tmp->m_nargs
                       & (Argument::NARGS_DEF | Argument::ZERO_OR_ONE))
                     && (tmp->m_nargs != Argument::NARGS_NUM
                         || n != tmp->m_num_args));
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

    inline void
    storage_optional_store_const(Parsers& parsers,
                                 std::vector<std::string> const& equals,
                                 std::string const& arg,
                                 pArgument const& tmp) const
    {
        if (equals.size() == 1) {
            if (tmp->action() == argparse::BooleanOptionalAction) {
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

    inline void process_optional_help(Parsers const& parsers,
                                      std::vector<std::string> const& equals,
                                      std::string const& arg) const
    {
        if (equals.size() != 1) {
            parsers.back().parser->throw_error(
                        detail::_ignore_explicit(arg, equals.back()),
                        parsers.back().lang);
        }
        // print help and exit
        parsers.back().parser->print_help(parsers.back().lang);
        ::exit(0);
    }

    inline void process_optional_version(Parsers const& parsers,
                                         std::vector<std::string> const& equals,
                                         std::string const& arg,
                                         pArgument const& tmp) const
    {
        if (equals.size() != 1) {
            parsers.back().parser->throw_error(
                        detail::_ignore_explicit(arg, equals.back()),
                        parsers.back().lang);
        }
        if (!tmp->m_version.has_value()) {
            throw AttributeError("'ArgumentParser' object has no "
                                 "attribute 'version'");
        }
        std::cout << despecify(tmp->version()) << std::endl;
        ::exit(0);
    }

    inline void match_positional_minimum(Parsers& parsers,
                                         std::deque<std::string>& arguments,
                                         pArgument const& arg) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == argparse::BooleanOptionalAction) {
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

    inline void match_positional_more_zero(Parsers& parsers,
                                           std::deque<std::string>& arguments,
                                           pArgument const& arg,
                                           std::size_t& over_args) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == argparse::BooleanOptionalAction) {
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
            case Argument::NARGS_NUM :
                storage_store_n_values(parsers, arg, arguments,
                                       arg->m_num_args);
                break;
            default :
                break;
        }
    }

    inline void match_positional_optional(Parsers& parsers,
                                          std::deque<std::string>& arguments,
                                          pArgument const& arg,
                                          std::size_t& over_args,
                                          std::size_t one_args) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == argparse::BooleanOptionalAction) {
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

    inline void match_positional_default(Parsers& parsers,
                                         std::deque<std::string>& arguments,
                                         pArgument const& arg) const
    {
        if (storage_is_positional_arg_stored(parsers, arg)) {
            return;
        }
        if (arg->action() == argparse::BooleanOptionalAction) {
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

    inline void match_positionals(Parsers& parsers,
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
            case Argument::REMAINDING :
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
        if (arg->m_nargs == Argument::REMAINDING && !first && !read_all_args) {
            return true;
        }
        return false;
    }

    inline void match_args_partial(Parsers& parsers,
                                   std::size_t& pos,
                                   pArguments const& positional,
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
            detail::_move_vector_insert_to_end(args, unrecognized_args);
        }
    }

    inline bool try_capture_parser(Parsers& parsers,
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
        if (!capture_need && (finish != positional.size()
                              || min_args >= args.size())) {
            match_positionals(parsers, pos, positional, args,
                              finish, min_args, one_args, more_args);
            detail::_insert_vector_to_end(args, unrecognized_args);
            return false;
        }
        match_positionals(parsers, pos, positional, args, finish,
                          ++min_args, one_args, more_args);
        std::string const& name = args.front();
        std::string choices;
        std::string const& dest = parsers.back().subparser.first->dest();
        for (std::size_t i = 0;
             i < parsers.back().subparser.first->m_parsers.size(); ++i) {
            pParser& p = parsers.back().subparser.first->m_parsers.at(i);
            detail::_append_value_to("'" + p->m_name + "'", choices, ", ");
            for (std::size_t j = 0; j < p->aliases().size(); ++j) {
                std::string const& alias = p->aliases().at(j);
                detail::_append_value_to("'" + alias + "'", choices, ", ");
            }
            if (p->m_name == name
                    || detail::_is_value_exists(name, p->aliases())) {
                parsers.push_back(
                            ParserInfo(
                                p.get(),
                                p.get()->m_data->get_optional(true, true),
                                _Storage(), p->subparser_info(true, pos)));
#ifdef _ARGPARSE_CXX_11
                parsers.back().parser->handle(parsers.back().parser->m_name);
#endif  // C++11+
                validate_arguments(p.get()->m_data->get_arguments(true));

                if (!dest.empty()) {
#ifdef _ARGPARSE_CXX_11
                    auto subparser_arg
                            = Argument::make_argument({ dest }, dest,
                                                      Argument::Positional);
                    for (auto& info : parsers) {
#else
                    pArgument subparser_arg
                            = Argument::make_argument(
                                detail::_make_vector(dest),
                                dest, Argument::Positional);
                    for (std::size_t j = 0; j < parsers.size(); ++j) {
                        ParserInfo& info = parsers.at(j);
#endif  // C++11+
                        info.storage.create(subparser_arg);
                    }
                    parsers.front().storage.at(subparser_arg).push_back(name);
                }
                for (std::size_t j = 0; j < parsers.size(); ++j) {
                    parsers.at(j).storage.create(
                            parsers.back().parser->m_data->get_arguments(true));
                }
                pArguments sub_positional = parsers.back()
                        .parser->m_data->get_positional(true, true);
                detail::_move_vector_insert_to(sub_positional, positional, pos);
                args.pop_front();
                return true;
            }
        }
        throw_error("invalid choice: '" + name
                    + "' (choose from " + choices + ")");
        return false;
    }

    inline void check_abbreviations(Parsers const& parsers,
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
            pArguments const& optionals = parsers.back().optional;
            std::vector<std::string> temp;
            if (m_allow_abbrev) {
                bool is_flag_added = false;
                std::string args;
                std::vector<std::string> keys;
                keys.reserve(4);
                for (std::size_t j = 0; j < optionals.size(); ++j) {
                    pArgument const& opt = optionals.at(j);
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
                                "ambiguous option: '" + arg
                                + "' could match" + args);
                }
                std::string const& flag = keys.empty() ? arg : keys.front();
                if (is_flag_added) {
                    temp.push_back(flag);
                } else {
                    separate_arg_abbrev(parsers, temp, arg,
                                        detail::_flag_name(flag), optionals);
                }
            } else {
                separate_arg_abbrev(parsers, temp, arg,
                                    detail::_flag_name(arg), optionals);
            }
            detail::_move_vector_replace_at(temp, arguments, i);
        }
    }

    static bool is_remainder_positional(std::size_t pos,
                                        pArguments const& positional,
                                        Parsers const& parsers)
    {
        return pos < positional.size()
                && positional.at(pos)->m_nargs == Argument::REMAINDING
                && !parsers.front().storage.at(positional.at(pos)).empty();
    }

    static pArgument const
    optional_arg_by_flag(Parsers const& parsers, std::string const& key)
    {
        pArguments const& args = parsers.back().optional;
#ifdef _ARGPARSE_CXX_11
        auto it = std::find_if(args.begin(), args.end(),
                               [&key] (pArgument const& arg)
        { return detail::_is_value_exists(key, arg->flags()); });
#else
        pArguments::const_iterator it = args.begin();
        for ( ; it != args.end(); ++it) {
            if (detail::_is_value_exists(key, (*it)->flags())) {
                break;
            }
        }
#endif  // C++11+
        return it != args.end() ? *it : pArgument(_ARGPARSE_NULLPTR);
    }

    static pArgument const
    get_optional_arg_by_flag(bool was_pseudo_arg,
                             Parsers const& parsers,
                             std::string const& key)
    {
        return was_pseudo_arg ? pArgument(_ARGPARSE_NULLPTR)
                              : optional_arg_by_flag(parsers, key);
    }

    static bool
    process_separate_arg_abbrev(std::string const& name,
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
                   || (argument
                       && (argument->action() & detail::_store_action))) {
            std::string str
                 = name.substr(i + (static_cast<bool>(argument) ? cp_size : 0));
            if (!str.empty()) {
                if (detail::_starts_with(str, detail::_spaces)) {
                    detail::_trim(str);
                    if (!detail::_have_quotes(str)) {
                        str = "'" + str + "'";
                    }
                }
                if (!detail::_starts_with(str, detail::_equals)) {
                    flags.back() += detail::_equals;
                }
                flags.back() += str;
            }
            return false;
        }
        return true;
    }

    inline void separate_arg_abbrev(Parsers const& parsers,
                                    std::vector<std::string>& temp,
                                    std::string const& arg,
                                    std::string const& name,
                                    pArguments const& args) const
    {
        if (name.size() + 1 == arg.size()) {
            std::vector<std::string> const split
                   = detail::_split_equal(arg,
                                         parsers.back().parser->prefix_chars());
            if (split.size() == 2 && !split.front().empty()
                    && optional_arg_by_flag(parsers, split.front())) {
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
                if (!process_separate_arg_abbrev(name, i, cp_size,
                                                 flags, arg, args)) {
                    break;
                }
                i += cp_size;
            }
            detail::_move_vector_insert_to_end(flags, temp);
        } else {
            temp.push_back(arg);
        }
    }

    inline void
    process_positional_args(std::vector<std::string> const& parsed_arguments,
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
#ifdef _ARGPARSE_CXX_11
        for (auto const& info : parsers) {
            for (auto const& ex : info.parser->m_mutex_groups) {
#else
        for (std::size_t i = 0; i < parsers.size(); ++i) {
            ParserInfo const& info = parsers.at(i);
            for (std::size_t j = 0; j < info.parser->m_mutex_groups.size(); ++j)
            {
                MutuallyExclusiveGroup const& ex
                        = info.parser->m_mutex_groups.at(j);
#endif  // C++11+
                std::string args;
                std::string found;
                for (std::size_t k = 0; k < ex.m_data->m_arguments.size(); ++k)
                {
                    pArgument const& arg = ex.m_data->m_arguments.at(k);
                    std::string flags
                            = detail::_vector_to_string(arg->flags(), "/");
                    args += detail::_spaces + flags;
                    if (!parsers.front().storage.at(arg).empty()) {
                        if (!found.empty()) {
                            info.parser->throw_error(
                                        "argument " + flags
                                        + ": not allowed with argument "
                                        + found);
                        }
                        found = flags;
                    }
                }
                if (ex.required() && found.empty()) {
                    if (ex.m_data->m_arguments.empty()) {
                        throw IndexError("list index out of range");
                    }
                    info.parser->throw_error("one of the arguments"
                                             + args + " is required");
                }
            }
        }
    }

    static void
    process_optionals_required(std::vector<std::string>& required_args,
                               pArguments const& optional,
                               _Storage const& storage)
    {
        for (std::size_t i = 0; i < optional.size(); ++i) {
            pArgument const& arg = optional.at(i);
            if (arg->required() && storage.at(arg).empty()) {
                std::string args = detail::_vector_to_string(arg->flags(), "/");
                required_args.push_back(args);
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
    process_required_check(ParserInfo const& parser, _Storage const& storage)
    {
        std::vector<std::string> required_args;
        process_optionals_required(required_args, parser.optional, storage);
        std::string args;
        for (std::size_t i = 0; i < required_args.size(); ++i) {
            detail::_append_value_to(required_args[i], args, ", ");
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
        if (arg->action() == argparse::extend
                && arg->m_nargs == Argument::ZERO_OR_ONE) {
            throw TypeError("'NoneType' object is not iterable");
        }
        if ((arg->m_nargs
             & (Argument::ZERO_OR_ONE | Argument::ZERO_OR_MORE))
                || arg->action() == argparse::BooleanOptionalAction) {
            storage_store_default_value(parsers, arg);
            return true;
        }
        if (arg->m_nargs == Argument::REMAINDING) {
            return true;
        }
        return false;
    }

    inline void
    check_required_args(Parsers& parsers,
                        std::size_t& pos,
                        pArguments const& positional) const
    {
        std::deque<ParserInfo>::reverse_iterator it = parsers.rbegin();
        std::vector<std::string> required_args;
        process_optionals_required(required_args, it->optional,
                                   parsers.front().storage);
        SubparserInfo const& subparser = it->subparser;
        bool sub_required = subparser.first && subparser.first->required();
        if (!required_args.empty() || pos < positional.size() || sub_required) {
            std::string args;
            for ( ; pos < positional.size(); ++pos) {
                process_subparser_required(sub_required, pos, subparser, args);
                pArgument const& arg = positional.at(pos);
                if (args.empty()
                        && skip_positional_required_check(parsers, arg)) {
                    continue;
                }
                detail::_append_value_to(arg->m_flags.front(), args, ", ");
            }
            process_subparser_required(sub_required, pos, subparser, args);
            for (std::size_t i = 0; i < required_args.size(); ++i) {
                detail::_append_value_to(required_args.at(i), args, ", ");
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
    default_values_post_trigger(_Storage& storage) const
    {
        for (_Storage::iterator it = storage.begin(); it != storage.end(); ) {
            if (!it->second.exists()) {
                if (it->first->is_suppressed()) {
                    it = storage.erase(it);
                    continue;
                }
                if (!(it->first->action()
                      & (argparse::count | argparse::language))
                        && it->first->m_type == Argument::Optional) {
                    detail::Value<std::string> const& dv = it->first->m_default;
                    if (dv.has_value()) {
                        it->second.push_default(dv());
                        storage.on_process_store(it->first, dv());
                    } else if (it->first->action() & detail::_bool_action) {
                        it->second.push_back(dv());
                        storage.on_process_store(it->first, dv());
                    }
                }
            }
            ++it;
        }
        for (std::size_t i = 0; i < m_default_values.size(); ++i) {
            std::pair<std::string, std::string> const& pair
                    = m_default_values.at(i);
            if (!storage.exists_arg(pair.first)) {
#ifdef _ARGPARSE_CXX_11
                auto arg = Argument::make_argument(
                            std::vector<std::string>{ pair.first },
                            pair.first, Argument::Positional);
                arg->default_value(pair.second);
                storage.create(arg, _Storage::mapped_type({ pair.second }));
#else
                pArgument arg = Argument::make_argument(
                            detail::_make_vector(pair.first),
                            pair.first, Argument::Positional);
                arg->default_value(pair.second);
                storage.create(arg, _Storage::mapped_type(
                                   detail::_make_vector(pair.second)));
#endif  // C++11+
            }
        }
    }

#ifdef _ARGPARSE_CXX_11
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
#endif  // C++11+

    static bool
    is_default_value_stored(std::deque<pArgument>& arguments,
                            std::string const& dest, std::string const& val)
    {
        for (std::size_t i = 0; i < arguments.size(); ++i) {
            pArgument& arg = arguments.at(i);
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
                for (std::size_t j = 0; j < arg->m_flags.size(); ++j) {
                    std::string const& flag = arg->m_flags.at(j);
                    std::string name = detail::_flag_name(flag);
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

    inline SubparserInfo
    subparser_info(bool add_suppress, std::size_t offset = 0) const
    {
        SubparserInfo res = std::make_pair(m_subparsers, offset);
        for (std::size_t i = 0; i < m_subparsers_position; ++i) {
            res.second += (add_suppress
                           || !m_data->m_positional.at(i)
                                               .first->m_help_type.has_value());
        }
        return res;
    }

    inline std::string subparser_prog_args() const
    {
        std::string res;
        bool add_suppress = false;
        SubparserInfo info = subparser_info(add_suppress);
        pArguments pos = m_data->get_positional(add_suppress, true);
        for (std::size_t i = 0; i < pos.size(); ++i) {
            if (info.second == i) {
                break;
            }
            std::string str = pos.at(i)->usage(m_formatter_class);
            detail::_append_value_to(str, res);
        }
        return res;
    }

    inline void
    add_arg_usage(std::string& res, std::string const& str, bool required) const
    {
        detail::_append_value_to(required ? str : "[" + str + "]", res, "\n");
    }

    inline void
    print_custom_usage(pArguments const& positional,
                       pArguments const& optional,
                       std::deque<MutuallyExclusiveGroup> const& mutex_groups,
                       SubparserInfo const& subparser,
                       std::string const& prog,
                       std::string const& usage_title,
                       std::ostream& os) const
    {
        std::size_t const w = output_width();
        std::string head_prog = usage_title + " " + prog;
        std::size_t indent
                = 1 + (w > detail::_min_width ? head_prog : usage_title).size();
        std::string res;
        pArguments ex_opt = optional;
        for (std::size_t i = 0; i < mutex_groups.size(); ++i) {
            for (std::size_t j = 0;
                 j < mutex_groups.at(i).m_data->m_arguments.size(); ++j) {
                ex_opt.erase(std::remove(
                                 ex_opt.begin(), ex_opt.end(),
                                 mutex_groups.at(i).m_data->m_arguments.at(j)),
                             ex_opt.end());
            }
        }
        for (std::size_t i = 0; i < ex_opt.size(); ++i) {
            add_arg_usage(res, ex_opt.at(i)->usage(m_formatter_class),
                          ex_opt.at(i)->required());
        }
        for (std::size_t i = 0; i < mutex_groups.size(); ++i) {
            add_arg_usage(res, mutex_groups.at(i).usage(m_formatter_class),
                          true);
        }
        for (std::size_t i = 0; i < positional.size(); ++i) {
            if (subparser.first && subparser.second == i
                    && !subparser.first->m_help_type.has_value()) {
                add_arg_usage(res, subparser.first->usage(), true);
            }
            std::string const str = positional.at(i)->usage(m_formatter_class);
            if (str.empty()) {
                continue;
            }
            add_arg_usage(res, str, true);
        }
        if (subparser.first && subparser.second == positional.size()
                && !subparser.first->m_help_type.has_value()) {
            add_arg_usage(res, subparser.first->usage(), true);
        }
        os << detail::_format_output(head_prog, res, 1, indent, w) << std::endl;
    }

    static bool
    is_subparser_positional(pSubparser const& sub)
    {
        return sub && sub->title().empty() && sub->description().empty()
                && !sub->m_help_type.has_value();
    }

    static void
    print_subparser(bool need_print, SubparserInfo const& subparser,
                    std::size_t index, _HelpFormatter const& formatter,
                    std::size_t size, std::size_t width,
                    std::string const& lang, std::ostream& os)
    {
        if (need_print && subparser.second == index) {
            os << subparser.first->print(formatter, size, width, lang)
               << std::endl;
        }
    }

    static void print_group(pGroup const& group,
                            pSubparser const& subparser,
                            bool is_positional,
                            _HelpFormatter const& formatter,
                            std::string const& prog, std::size_t size,
                            std::size_t width,
                            std::string const& lang, std::ostream& os)
    {
        if ((subparser && (group != subparser
                           || (!is_positional
                               && !subparser->m_help_type.has_value())))
                || (!subparser && group != subparser)) {
            group->print_help(os, formatter, prog, size, width, lang);
        }
    }

    inline void update_prog(std::string const& parent_prog)
    {
        m_prog = parent_prog + detail::_spaces + m_name;
    }

    inline void
    test_overview(std::string const& lang, std::ostream& os) const
    {
        if (lang.empty()) {
            os << "default language" << std::endl;
        } else {
            os << "language: " << lang << std::endl;
        }
        os << "prog: " << prog() << std::endl;
        detail::Value<std::string> version;
        for (std::size_t i = 0; i < m_data->m_arguments.size(); ++i) {
            pArgument const& arg = m_data->m_arguments.at(i);
            if (arg->action() == argparse::version) {
                version = arg->version();
                break;
            }
        }
        if (version.has_value()) {
            os << "version: " << version() << std::endl;
        }
        std::string tr_usage = detail::_tr(m_usage, lang);
        if (!tr_usage.empty()) {
            os << "usage: " << despecify(tr_usage) << std::endl;
        }
        std::string tr_description = detail::_tr(m_description, lang);
        if (!tr_description.empty()) {
            os << "description: " << despecify(tr_description) << std::endl;
        }
        std::string tr_epilog = detail::_tr(m_epilog, lang);
        if (!tr_epilog.empty()) {
            os << "epilog: " << despecify(tr_epilog) << std::endl;
        }
        os << "prefix_chars: '" << prefix_chars() << "'" << std::endl;
        if (!fromfile_prefix_chars().empty()) {
            os << "fromfile_prefix_chars: '" << fromfile_prefix_chars() << "'"
               << std::endl;
        }
        if (m_argument_default.has_value()) {
            os << "argument_default: " << argument_default() << std::endl;
        }
        if (!conflict_handler().empty()) {
            os << "conflict_handler: " << conflict_handler() << std::endl;
        }
        os << "add_help: "
           << detail::_bool_to_string(add_help()) << std::endl;
        os << "allow_abbrev: "
           << detail::_bool_to_string(allow_abbrev()) << std::endl;
        os << "exit_on_error: "
           << detail::_bool_to_string(exit_on_error()) << std::endl;
        if (m_output_width.has_value()) {
            os << "output_width [override]: " << output_width() << std::endl;
        } else {
#ifdef ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
            os << "output_width [detected]: " << output_width() << std::endl;
#else
            os << "output_width [default]: " << output_width() << std::endl;
#endif  // ARGPARSE_ENABLE_TERMINAL_SIZE_DETECTION
        }
        if (m_subparsers) {
            std::deque<pParser> parsers = m_subparsers->m_parsers;
            os << "subparsers list:" << std::endl;
            for (std::size_t i = 0; i < parsers.size(); ++i) {
                pParser const& parser = parsers.at(i);
                os << "  " << (i + 1) << ". '" << parser->m_name << "'";
                std::string aliases;
                for (std::size_t j = 0; j < parser->aliases().size(); ++j) {
                    std::string const& alias = parser->aliases().at(j);
                    detail::_append_value_to("'" + alias + "'", aliases, ", ");
                }
                if (!aliases.empty()) {
                    os << ", aliases: " << aliases;
                }
                os << std::endl;
            }
        }
    }

    inline bool
    test_diagnostics(std::string const& lang, std::ostream& os) const
    {
        std::string const status_ok     = "[ OK ]";
        std::string const status_info   = "[INFO]";
        std::string const status_warn   = "[WARN]";
        std::string const status_error  = "[FAIL]";
        typedef std::pair<std::size_t, std::size_t> WarnErrAmount;
        WarnErrAmount diagnostics;
        // check prog
        if (prog() == "untitled") {
            ++diagnostics.first;
            os << status_warn << " used default `prog` value, "
               <<  "override it or pass command line options" << std::endl;
        }
        std::map<std::string, std::size_t> dest_args;
        // check arguments
        for (std::size_t i = 0; i < m_data->m_arguments.size(); ++i) {
            pArgument const& arg = m_data->m_arguments.at(i);
            std::string argument = "argument with ";
            argument += std::string(arg->dest().empty() ? "options" : "dest");
            argument += " " + detail::_vector_to_string(
                            arg->get_argument_flags(), ", ", "'");
            // check flags
            bool is_optional = arg->m_type == Argument::Optional;
            for (std::size_t j = 0; j < arg->flags().size(); ++j) {
                std::string const& flag = arg->flags().at(j);
                if (!detail::_is_utf8(flag)) {
                    ++diagnostics.first;
                    os << status_warn << " " << argument << ": flag '"
                       << flag << "' is not utf-8" << std::endl;
                }
                if (!detail::_is_flag_correct(flag, is_optional)) {
                    ++diagnostics.first;
                    os << status_warn << " " << argument << ": flag '"
                       << flag << "' can be incorrect" << std::endl;
                }
                if (flag == detail::_pseudo_arg && arg->dest().empty()) {
                    ++diagnostics.second;
                    os << status_error << " " << argument << ": dest= "
                       << "is required for options like '--'" << std::endl;
                }
            }
            // check dest
            if (!is_optional) {
                if (arg->dest().empty() && arg->flags().empty()) {
                    ++diagnostics.second;
                    os << status_error << " " << argument << ": missing 1 "
                       << "required positional argument: 'dest'" << std::endl;
                }
                if (!arg->dest().empty() && !arg->flags().empty()) {
                    ++diagnostics.second;
                    os << status_error << " " << argument << ": dest supplied "
                       << "twice for positional argument" << std::endl;
                }
            }
            if (!arg->dest().empty()) {
                std::string const& flag = arg->dest();
                if (!detail::_is_utf8(flag)) {
                    ++diagnostics.first;
                    os << status_warn << " " << argument << ": dest '"
                       << flag << "' is not utf-8" << std::endl;
                }
                if (!detail::_is_flag_correct(flag, is_optional)) {
                    ++diagnostics.first;
                    os << status_warn << " " << argument << ": dest '"
                       << flag << "' can be incorrect" << std::endl;
                }
            }
            for (std::size_t j = 0; j < arg->get_argument_flags().size(); ++j) {
                std::string const& flag = arg->get_argument_flags().at(j);
                if (!flag.empty()) {
                    if (dest_args.count(flag) != 0) {
                        if (conflict_handler() == "resolve") {
                            ++diagnostics.first;
                            os << status_warn << " " << argument
                               << ": conflicting option string: '"
                               << flag << "'" << std::endl;
                        } else {
                            ++diagnostics.second;
                            os << status_error << " " << argument
                               << ": conflicting option string: '"
                               << flag << "'" << std::endl;
                        }
                    }
                    ++dest_args[flag];
                }
            }
            // check choices
            for (std::size_t j = 0; j < arg->choices().size(); ++j) {
                std::string const& choice = arg->choices().at(j);
                if (choice.size() > 1
                        && choice.size() != detail::_trim_copy(choice).size()) {
                    ++diagnostics.first;
                    os << status_warn << " " << argument << ": choice '"
                       << choice << "' can be incorrect" << std::endl;
                }
            }
            // check help
            if (detail::_tr(arg->m_help, lang).empty()
                    && !arg->m_help_type.has_value()) {
                ++diagnostics.first;
                os << status_warn << " " << argument
                   << ": help is not set" << std::endl;
            }
            // check metavar
            if ((arg->action() & (detail::_store_action
                             | argparse::append_const
                             | argparse::language))) {
                std::size_t names_size
                        = arg->get_argument_name(m_formatter_class).size();
                if (names_size > 1
                        && (arg->m_nargs != Argument::NARGS_NUM
                            || names_size != arg->m_num_args)) {
                    ++diagnostics.second;
                    os << status_error << " " << argument << ": length of "
                       << "metavar tuple does not match nargs" << std::endl;
                }
            }
        }
        // check mutually exclusive arguments
        for (std::size_t i = 0; i < m_mutex_groups.size(); ++i) {
            MutuallyExclusiveGroup const& group = m_mutex_groups.at(i);
            for (std::size_t j = 0; j < group.m_data->m_arguments.size(); ++j) {
                pArgument const& arg = group.m_data->m_arguments.at(j);
                std::string argument = "argument with ";
                argument += std::string(arg->dest().empty() ? "options"
                                                            : "dest");
                argument += " " + detail::_vector_to_string(
                                arg->get_argument_flags(), ", ", "'");
                if (arg->required()) {
                    ++diagnostics.second;
                    os << status_error << " " << argument << ": mutually "
                       << "exclusive arguments must be optional" << std::endl;
                }
            }
        }
        // check subparsers
        if (m_subparsers) {
            std::deque<pParser> parsers = m_subparsers->m_parsers;
            if (parsers.empty()) {
                ++diagnostics.first;
                os << status_warn << " subparsers created "
                   << "but no parsers were added" << std::endl;
            }
            // check dest
            if (m_subparsers->dest().empty()) {
                os << status_info << " you can specify `dest` for subparsers "
                   << "to determine used parser" << std::endl;
            } else {
                if (!detail::_is_utf8(m_subparsers->dest())) {
                    ++diagnostics.first;
                    os << status_warn << " subparsers dest '"
                       << m_subparsers->dest() << "' is not utf-8" << std::endl;
                }
                if (!detail::_is_flag_correct(m_subparsers->dest(), false)) {
                    ++diagnostics.first;
                    os << status_warn << " subparsers dest '"
                       << m_subparsers->dest() << "' can be incorrect"
                       << std::endl;
                }
                std::string const& flag = m_subparsers->dest();
                if (dest_args.count(flag) != 0) {
                    if (conflict_handler() == "resolve") {
                        ++diagnostics.first;
                        os << status_warn << " subparsers dest '" << flag
                           << "': conflicting option string: '"
                           << flag << "'" << std::endl;
                    } else {
                        ++diagnostics.second;
                        os << status_error << " subparsers dest '" << flag
                           << "': conflicting option string: '"
                           << flag << "'" << std::endl;
                    }
                }
                ++dest_args[flag];
            }
            // check help
            if (detail::_tr(m_subparsers->m_help, lang).empty()
                    && !m_subparsers->m_help_type.has_value()) {
                ++diagnostics.first;
                os << status_warn << " help for subparsers is not set"
                   << std::endl;
            }
            for (std::size_t i = 0; i < parsers.size(); ++i) {
                pParser const& parser = parsers.at(i);
                // check name
                if (!detail::_is_utf8(parser->m_name)) {
                    ++diagnostics.first;
                    os << status_warn << " name for parser '"
                       << parser->m_name << "' is not utf-8" << std::endl;
                }
                if (!detail::_is_flag_correct(parser->m_name, false)) {
                    ++diagnostics.first;
                    os << status_warn << " name for parser '"
                       << parser->m_name << "' can be incorrect" << std::endl;
                }
                // check help
                if (detail::_tr(parser->m_help, lang).empty()) {
                    ++diagnostics.first;
                    os << status_warn << " help for parser '"
                       << parser->m_name << "' is not set" << std::endl;
                }
            }
        }
        // end diagnostics
        bool res = true;
        if (diagnostics.first == 0 && diagnostics.second == 0) {
            os << status_ok << " no warning or errors detected" << std::endl;
        } else {
            res = false;
            if (diagnostics.first != 0) {
                os << status_warn << " detected warnings: " << diagnostics.first
                   << std::endl;
            }
            if (diagnostics.second != 0) {
                os << status_error << " detected errors: " << diagnostics.second
                   << std::endl;
            }
        }
        return res;
    }

#ifdef _ARGPARSE_CXX_11
    inline void handle(std::string const& str) const
    {
        if (m_handle) {
            m_handle(detail::_remove_quotes(str));
        }
    }

    inline void
    parse_handle(bool only_known,
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
#endif  // C++11+

    pArgumentData m_data;
    std::string m_name;
    std::string m_prog;
    detail::LanguagePack m_usage;
    detail::LanguagePack m_usage_title;
    detail::LanguagePack m_description;
    detail::LanguagePack m_positionals_title;
    detail::LanguagePack m_optionals_title;
    detail::LanguagePack m_epilog;
    detail::LanguagePack m_help;
    std::vector<std::string> m_aliases;
    _HelpFormatter m_formatter_class;
    std::string m_prefix_chars;
    std::string m_fromfile_prefix_chars;
    detail::Value<std::string> m_argument_default;
    detail::Value<std::size_t> m_output_width;
    std::deque<pGroup> m_groups;
    std::deque<MutuallyExclusiveGroup> m_mutex_groups;
    std::vector<std::pair<std::string, std::string> > m_default_values;
    std::vector<std::string> m_parsed_arguments;
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
}  // namespace argparse

#undef _ARGPARSE_CONSTEXPR
#undef _ARGPARSE_ENUM_TYPE
#undef _ARGPARSE_EXPORT
#undef _ARGPARSE_FINAL
#undef _ARGPARSE_INLINE_VARIABLE
#undef _ARGPARSE_NOEXCEPT
#undef _ARGPARSE_NULLPTR
#undef _ARGPARSE_OVERRIDE
#undef _ARGPARSE_USE_CONSTEXPR

#undef _ARGPARSE_CXX_98
#undef _ARGPARSE_CXX_11
#undef _ARGPARSE_CXX_14
#undef _ARGPARSE_CXX_17
#undef _ARGPARSE_CXX_20
#undef _ARGPARSE_CXX_23

#endif  // _ARGPARSE_ARGUMENT_PARSER_HPP_
