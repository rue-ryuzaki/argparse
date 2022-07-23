[![Argument Parser for C++](https://raw.githubusercontent.com/rue-ryuzaki/argparse/master/doc/argparse-logo.png)](https://github.com/rue-ryuzaki/argparse/releases)

[![Language](https://img.shields.io/badge/C%2B%2B-98/11/14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![Ubuntu](https://github.com/rue-ryuzaki/argparse/workflows/Ubuntu/badge.svg)](https://github.com/rue-ryuzaki/argparse/actions?query=workflow%3AUbuntu)
[![macOS](https://github.com/rue-ryuzaki/argparse/workflows/macOS/badge.svg)](https://github.com/rue-ryuzaki/argparse/actions?query=workflow%3AmacOS)
[![Windows](https://github.com/rue-ryuzaki/argparse/workflows/Windows/badge.svg)](https://github.com/rue-ryuzaki/argparse/actions?query=workflow%3AWindows)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/24785/badge.svg)](https://scan.coverity.com/projects/argparse)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/rue-ryuzaki/argparse.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/rue-ryuzaki/argparse/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/rue-ryuzaki/argparse.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/rue-ryuzaki/argparse/context:cpp)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/rue-ryuzaki/argparse/main/LICENSE)
[![GitHub releases](https://img.shields.io/github/release/rue-ryuzaki/argparse.svg)](https://github.com/rue-ryuzaki/argparse/releases)
[![GitHub downloads](https://img.shields.io/github/downloads/rue-ryuzaki/argparse/total)](https://github.com/rue-ryuzaki/argparse/releases)
[![GitHub issues](https://img.shields.io/github/issues/rue-ryuzaki/argparse.svg)](https://github.com/rue-ryuzaki/argparse/issues)
[![Try it on godbolt online](https://img.shields.io/badge/on-godbolt-blue.svg)](https://godbolt.org/z/arcbv6ecv)
# ArgumentParser
Python-like argument parser for C++ projects (with similar syntax).

This project is a C++ adaptation of Python argparse and supports most of its features (actions, nargs, subparsers, argument groups, help formatting, auto-generated usage and help, and more).
## Supported compilers
C++ support compiler
 - Ubuntu: gcc (tested 4.8, 5 up to 10), clang (tested 3.9 up to 12)
 - macOS: clang (tested 13 on macos-11)
 - Windows: mingw (tested 8.1), clang (tested 11 up to 14), msvc (tested 2019, 2022), github legacy: msvc2017

## Official repositories:
```cpp-argparse-dev``` [PPA](https://launchpad.net/~golubchikov-mihail/+archive/ubuntu/cpp-argparse)

```cpp-argparse-dev``` [AUR](https://aur.archlinux.org/packages/cpp-argparse-dev)

```cpp-argparse-dev``` [MacPorts](https://ports.macports.org/port/cpp-argparse-dev)
## Overview
- Examples:
  - [code](#code-example) - [usage](#usage-example) - [output](#output)
  - [actions](#actions-example)
  - [nargs](#nargs-example)
  - [subparsers](#subparsers-example)
  - [argument groups](#argument-groups-example)
- [Namespace::get<> types support](#namespaceget-types-support--try_get-with-stdoptional-since-c17)
  - [custom types](#custom-type-example)
- [Features](#features)
  - [handle (for C++11+)](#handle)
  - [terminal size auto-detection](#terminal-size-auto-detection)
  - [Argument::implicit_value](#argumentimplicit_value)
- Python API support:
  - [ArgumentParser objects](#argumentparser-objects-support)
  - [add_argument(name or flags) method](#the-add_argumentname-or-flags-method-support)
  - [add_argument() actions](#the-add_argumentname-or-flags-method-support)
- [Execute unit tests](#execute-unit-tests)
- [License](#license)

Additional documentation can be found on the [argparse Wiki](https://github.com/rue-ryuzaki/argparse/wiki)
## Code example
- create parser: auto parser = argparse::ArgumentParser(...);
- add arguments: parser.add_argument(...);
- parse arguments: auto const args = parser.parse_args();
- get parsed argument values: args.get<...>(...);
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv)
            .prog("FooBar")
            .description("foo description")
            .epilog("bar epilog");
    parser.add_argument("-f", "--foo").help("FOO help").default_value("foo");
    parser.add_argument("-b", "--bar").help("BAR help").default_value("42");
    parser.add_argument("baz").help("BAZ help");
    parser.print_help();

    auto const args = parser.parse_args();

    std::cout << std::endl << "Parse data:" << std::endl;
    std::cout << "foo: " << args.get<std::string>("-f") << std::endl;
    std::cout << "bar: " << args.get<int>("--bar") << std::endl;
    std::cout << "baz: " << args.get<std::string>("baz") << std::endl;
    return 0;
}
```
## Usage example
```
./a.out -f=foobar -b 101 baaz
```
## Output
```
usage: FooBar [-h] [-f FOO] [-b BAR] baz

foo description

positional arguments:
  baz                BAZ help

options:
  -h, --help         show this help message and exit
  -f FOO, --foo FOO  FOO help
  -b BAR, --bar BAR  BAR help

bar epilog

Parse data:
foo: foobar
bar: 101
baz: baaz
```
## Actions example
See [supported actions](https://github.com/rue-ryuzaki/argparse#the-add_argument-actions-support)
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv).add_help(false);

    parser.add_argument("--store").action("store");
    parser.add_argument("--store_const").action("store_const").const_value("const_value");
    parser.add_argument("--store_true").action("store_true");
    parser.add_argument("--store_false").action("store_false");
    parser.add_argument("--append").action("append");
    parser.add_argument("--append_const").action("append_const").const_value("const_value");
    parser.add_argument("--count").action("count");
    parser.add_argument("--extend").action("extend");
    parser.add_argument("--version").action("version").version("1.0");
    parser.add_argument("--help").action("help").help("show help and exit");

    auto args = parser.parse_args();

    std::cout << args.get<std::string>("store") << std::endl;
    std::cout << args.get<std::string>("store_const") << std::endl;
    std::cout << args.get<std::string>("store_true") << std::endl;
    std::cout << args.get<std::string>("store_false") << std::endl;
    std::cout << args.get<std::string>("append") << std::endl;
    std::cout << args.get<std::string>("append_const") << std::endl;
    std::cout << args.get<std::string>("count") << std::endl;
    std::cout << args.get<std::string>("extend") << std::endl;

    return 0;
}
```
## Nargs example
Supported nargs:
- "?" or zero_ore_one() or optional(), argument must have zero or one value
- "*" or zero_or_more(), argument must have zero or more values
- "+" or one_or_more(), argument must have one or more values
- N (positive number), argument must have N values
- argparse::REMAINDER or remainder(), all the remaining command-line arguments are gathered into a list
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);

    parser.add_argument("--nargs?").action("store").nargs("?");
    parser.add_argument("--nargs*").action("store").nargs("*");
    parser.add_argument("--nargs+").action("store").nargs("+");
    parser.add_argument("--nargs2").action("store").nargs(2);
    parser.add_argument("--nargs3").action("store").nargs(3);

    auto args = parser.parse_args();

    std::cout << args.get<std::string>("nargs?") << std::endl;
    std::cout << args.get<std::string>("nargs*") << std::endl;
    std::cout << args.get<std::string>("nargs+") << std::endl;
    std::cout << args.get<std::string>("nargs2") << std::endl;
    std::cout << args.get<std::string>("nargs3") << std::endl;

    return 0;
}
```
## Subparsers example
If you want to use subcommands with arguments (like git commands: git clone URL -b BRANCH, git merge --ff, git status), use subparsers.

If you need to get the subparser's parser name, set dest value to the subparser, or use [handle](https://github.com/rue-ryuzaki/argparse#parserhandlestdfunctionvoidstdstring-func) in parsers.

Note:

If subparser not selected in parsed arguments, it is not added to the Namespace, like the arguments of its parsers.

Check if value exists first (Namespace::exists()), or use Namespace::try_get<>() instead of Namespace::get<>() (since C++17).
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--foo").action("store_true").help("foo help");

    auto& subparsers = parser.add_subparsers().dest("sub").help("sub-command help");

    auto& parser_a = subparsers.add_parser("a").help("a help");
    parser_a.add_argument("bar").help("bar help");

    auto& parser_b = subparsers.add_parser("b").help("b help");
    parser_b.add_argument("--baz").choices("XYZ").help("baz help");

    auto const args = parser.parse_args();

    std::cout << "foo: " << args.get<bool>("foo") << std::endl;
    std::cout << "foo: " << args.to_string("foo") << std::endl;
    if (args.exists("sub")) {
        std::cout << "sub: " << args.get<std::string>("sub") << std::endl;
    }
    if (args.exists("bar")) {
        std::cout << "bar: " << args.get<uint32_t>("bar") << std::endl;
    }
    if (args.exists("baz")) {
        std::cout << "baz: " << args.get<std::string>("baz") << std::endl;
    }

    return 0;
}
```
## Argument groups example
You can group arguments into groups, each with its own title and description.
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv).add_help(false);

    auto& group1 = parser.add_argument_group("group 1", "group1 description");
    group1.add_argument("--foo").action("store").help("foo help");

    auto& group2 = parser.add_argument_group("group 2", "group2 description");
    group2.add_argument("bar").action("store").help("bar help");
    group2.add_argument("--baz").action("store").help("baz help");

    parser.print_help();

    return 0;
}
```
## Namespace::get<> types support (+ try_get with std::optional, since C++17)
- base types (bool, integral types, floating point types, std::string, std::string_view (since C++17))
- byte types (char, signed/unsigned char, int8_t, uint8_t, std::byte (since C++17), char8_t (since C++20))
- containers (std::array (since C++11), std::deque, std::forward_list (since C++11), std::list, std::multiset, std::priority_queue, std::queue, std::set, std::stack, std::vector, std::unordered_multiset (since C++11), std::unordered_set (since C++11))
- containers with std::pair/std::tuple (since C++11)
- 2 dimensional containers (std::deque, std::list, std::vector with another containers or queues)
- mapped types (std::map, std::multimap, std::unordered_map (since C++11), std::unordered_multimap (since C++11))
- std::pair
- std::tuple (since C++11)
- custom types
### Don't work:
- pointer and reference types
- plain C arrays
- C++17+ types and containers (std::span)
### Note:
For types with std::pair and std::tuple (also for std::map) needs to specify delimiter (by default it '=') between key and value (for std::pair/std::map) / values (for std::tuple). For space delimiter (' ') all values are parsed from separated command line arguments, otherwise from individual command line argument.

For example:
- ':' : auto args = parser.parse_args("--foo key1:value1 'key2':'value2'"); args.get<std::map<std::string, std::string> >("foo", ':');
- ' ' : auto args = parser.parse_args("--foo key1 value1 'key2' 'value2'"); args.get<std::vector<std::pair<std::string, std::string> > >("foo", ' ');
## Custom type example
### Namespace::get<>
Required std::istream& operator >>(std::istream& is, Type& t).
### Argument::default_value<> and Argument::const_value<>
Required std::ostream& operator <<(std::ostream& os, Type const& t).
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

struct Coord
{
    int x;
    int y;
    int z;

    void print()
    {
        std::cout << "x=" << x << ";y=" << y << ";z=" << z << std::endl;
    }
};
std::istream& operator >>(std::istream& is, Coord& c)
{
    is >> c.x >> c.y >> c.z;
    return is;
}
std::ostream& operator <<(std::ostream& os, Coord const& c)
{
    os << c.x << " " << c.y << " " << c.z;
    return os;
}

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--coord").nargs(3).help("coord help");
    parser.add_argument("--const_coord").action("store_const").default_value(Coord{0, 0, 0})
            .const_value(Coord{1, 1, 1}).help("const coord help");

    auto const args = parser.parse_args({ "--coord", "1", "2", "3" });

    auto c = args.get<Coord>("coord");
    c.print();
    auto c2 = args.get<Coord>("const_coord");
    c2.print();

    return 0;
}
```
## Features
### Handle
#### ArgumentParser::handle(std::function<void(argparse::Namespace)> func)
Called when the parser is executed and passed the namespace of the parser.
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--foo").action("store_true").help("foo help");

    auto& subparsers = parser.add_subparsers().help("sub-command help");

    auto& parser_a = subparsers.add_parser("a").help("a help");
    parser_a.add_argument("bar").help("bar help");
    parser_a.handle([] (argparse::Namespace const& args)
    {
        std::cout << "bar: " << args.get<uint32_t>("bar") << std::endl;
    });

    auto& parser_b = subparsers.add_parser("b").help("b help");
    parser_b.add_argument("--baz").choices("XYZ").help("baz help");
    parser_b.handle([] (argparse::Namespace const& args)
    {
        std::cout << "baz: " << args.get<std::string>("baz") << std::endl;
    });

    auto const args = parser.parse_args();

    std::cout << "foo: " << args.get<bool>("foo") << std::endl;

    return 0;
}
```
#### ArgumentParser::handle(std::function<void()> func)
Called when the parser is executed.
#### ArgumentParser::handle(std::function<void(std::string)> func)
Called when the parser is executed and passed the value of the parser.
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--foo").action("store_true").help("foo help");

    auto& subparsers = parser.add_subparsers().help("sub-command help");

    auto& parser_a = subparsers.add_parser("a").help("a help")
            .handle([] () { std::cout << "Parser A handle" << std::endl; });
    parser_a.add_argument("bar").help("bar help");

    auto& parser_b = subparsers.add_parser("b").help("b help")
            .handle([] (std::string const& value) { std::cout << "Parser B handle " << value << std::endl; });
    parser_b.add_argument("--baz").choices("XYZ").help("baz help");

    auto const args = parser.parse_args();

    std::cout << "foo: " << args.get<bool>("foo") << std::endl;
    if (args.exists("bar")) {
        std::cout << "bar: " << args.get<uint32_t>("bar") << std::endl;
    }
    if (args.exists("baz")) {
        std::cout << "baz: " << args.get<std::string>("baz") << std::endl;
    }

    return 0;
}
```
#### Argument::handle(std::function<void()> func)
Called when the argument is present.
Preferably for value-independent arguments (Action: "store_true", "store_false" or "count")
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--foo").action("store_true").help("foo help")
            .handle([] () { std::cout << "Foo handle" << std::endl; });

    parser.parse_args();

    return 0;
}
```
#### Argument::handle(std::function<void(std::string)> func)
Called when the argument is present and passed the value of the argument.
Preferably for value-dependent arguments (Action: "store", "store_const", "append", "append_const" or "extend")

For value-independent arguments gets const value (Action: "store_true", "store_false") or empty string (Action: "count")
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--foo").action("store").help("foo help")
            .handle([] (std::string const& value)
    { std::cout << "Handle value '" << value << "'" << std::endl; });

    parser.parse_args();

    return 0;
}
```
### Terminal size auto-detection
By default, help output is positioned based on the terminal's width. But you can manually specify the width of the available area using the ArgumentParser::output_width(...) method.
### Argument::implicit_value
The implicit_value argument of add_argument() is used to hold implicit values that are not read from the command line (use with nargs = "?" and "*").

This is an alternative for the const_value (for optional arguments it works only for nargs = "?").

example:
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--foo").nargs("?").implicit_value("bar").default_value("foo");

    auto const args = parser.parse_args();

    std::cout << "  foo = '" << args.get<std::string>("foo") << "'" << std::endl;

    return 0;
}
```
with usage:
```
// here will be default value (if presented)
./a.out
  foo = 'foo'

// here will be implicit value (if presented)
./a.out --foo
  foo = 'bar'

// here will be parsed command line value
./a.out --foo=baz
  foo = 'baz'
```
## ArgumentParser objects support
- [x] prog - The name of the program (default: argv[0] or "untitled")
- [x] usage - The string describing the program usage (default: generated from arguments added to parser)
- [x] description - Text to display before the argument help (default: "")
- [x] epilog - Text to display after the argument help (default: "")
- [x] parents - A list of ArgumentParser objects whose arguments should also be included
- [x] formatter_class - A class for customizing the help output
- [x] prefix_chars - The set of characters that prefix optional arguments (default: "-")
- [x] fromfile_prefix_chars - The set of characters that prefix files from which additional arguments should be read (default: "")
- [x] argument_default - The global default value for arguments (default: none)
- [x] conflict_handler - The strategy for resolving conflicting optionals (usually unnecessary, otherwise set "resolve")
- [x] add_help - Add a -h/--help option to the parser (default: true)
- [x] allow_abbrev - Allows long options to be abbreviated if the abbreviation is unambiguous. (default: true)
- [x] exit_on_error - Determines whether or not ArgumentParser exits with error info when an error occurs. (default: true)
## The add_argument(name or flags) method support
- [x] name or flags - Either a name or a list of option strings, e.g. foo or -f, --foo.
- [x] action - The basic type of action to be taken when this argument is encountered at the command line.
- [x] nargs - The number of command-line arguments that should be consumed.
- [x] const - A constant value required by some action and nargs selections.
- [x] default - The value produced if the argument is absent from the command line and if it is absent from the namespace object.
- [x] type - The type to which the command-line argument should be converted (for MetavarTypeHelpFormatter and Namespace::get<T> type check).
- [x] choices - A container of the allowable values for the argument.
- [x] required - Whether or not the command-line option may be omitted (optionals only).
- [x] help - A brief description of what the argument does.
- [x] metavar - A name for the argument in usage messages.
- [x] dest - The name of the attribute to be added to the object returned by parse_args().
## The add_argument() actions support
- [x] "store" - This just stores the argument’s value. This is the default action.
- [x] "store_const" - This stores the value specified by the const keyword argument.
- [x] "store_true" and "store_false" - These are special cases of "store_const" used for storing the values true and false respectively. In addition, they create default values of false and true respectively.
- [x] "append" - This stores a list, and appends each argument value to the list. This is useful to allow an option to be specified multiple times.
- [x] "append_const" - This stores a list, and appends the value specified by the const keyword argument to the list.
- [x] "count" - This counts the number of times a keyword argument occurs.
- [x] "help" - This prints a complete help message for all the options in the current parser and then exits.
- [x] "version" - This expects a version= keyword argument in the add_argument() call, and prints version information and exits when invoked.
- [x] "extend" - This stores a list, and extends each argument value to the list.
- [x] argparse::BooleanOptionalAction - Adds support for boolean actions such as --foo and --no-foo
## Execute unit tests
To compile and run the tests, you need to execute

```sh
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ctest --output-on-failure
```
## License
<img align="right" src="https://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](https://opensource.org/licenses/MIT):

Copyright &copy; 2021-2022 [Golubchikov Mihail](https://github.com/rue-ryuzaki)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* * *

The class contains a slightly modified version of the shared_ptr implementation for C++98 from Sébastien Rombauts which is licensed under the [MIT License](https://opensource.org/licenses/MIT) (see above). Copyright &copy; 2012-2019 [Sébastien Rombauts](https://github.com/SRombauts)
