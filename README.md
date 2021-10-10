# ArgumentParser v0.6.0
Python-like argument parser for C++11+ projects
## Code example
```cpp
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv)
            .prog("FooBar")
            .description("foo description")
            .epilog("bar epilog");
    parser.add_argument({ "-f", "--foo" }).help("FOO help").default_value("foo");
    parser.add_argument({ "-b", "--bar" }).help("BAR help").default_value("42");
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

optional arguments:
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

    auto& parser_b = subparsers.add_parser("b").help("b help");
    parser_b.add_argument("--baz").choices("XYZ").help("baz help");

    auto const args = parser.parse_args();

    std::cout << "bar: " << args.get<uint32_t>("bar") << std::endl;
    std::cout << "foo: " << args.get<bool>("foo") << std::endl;
    std::cout << "baz: " << args.get<std::string>("baz") << std::endl;

    return 0;
}
```
## Argument groups example
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
## Custom get type example
Required std::istream& operator >>(std::istream& is, Type& t).
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
    is >> c.x;
    is >> c.y;
    is >> c.z;
    return is;
}

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv);
    parser.add_argument("--coord").nargs(3).help("coord help");

    auto const args = parser.parse_args({ "--coord", "1", "2", "3" });

    auto c = args.get<Coord>("coord");
    c.print();

    return 0;
}
```
## Features
### Handle
Argument::handle(std::function<void()> func)
Only available for value-independent arguments (Action: "store_true", "store_false" or "count")
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
Argument::handle(std::function<void(std::string)> func)
Only available for value-dependent arguments (Action: "store", "store_const", "append", "append_const" or "extend")
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
- [ ] conflict_handler - The strategy for resolving conflicting optionals (usually unnecessary)
- [x] add_help - Add a -h/--help option to the parser (default: true)
- [x] allow_abbrev - Allows long options to be abbreviated if the abbreviation is unambiguous. (default: true)
- [x] exit_on_error - Determines whether or not ArgumentParser exits with error info when an error occurs. (default: true)
## The add_argument(name or flags) method support
- [x] name or flags - Either a name or a list of option strings, e.g. foo or -f, --foo.
- [x] action - The basic type of action to be taken when this argument is encountered at the command line.
- [x] nargs - The number of command-line arguments that should be consumed.
- [x] const - A constant value required by some action and nargs selections.
- [x] default - The value produced if the argument is absent from the command line and if it is absent from the namespace object.
- [ ] type - The type to which the command-line argument should be converted.
- [x] choices - A container of the allowable values for the argument.
- [x] required - Whether or not the command-line option may be omitted (optionals only).
- [x] help - A brief description of what the argument does.
- [x] metavar - A name for the argument in usage messages.
- [x] dest - The name of the attribute to be added to the object returned by parse_args().
## The add_argument() actions support
- [x] 'store' - This just stores the argument’s value. This is the default action.
- [x] 'store_const' - This stores the value specified by the const keyword argument.
- [x] 'store_true' and 'store_false' - These are special cases of 'store_const' used for storing the values true and false respectively. In addition, they create default values of false and true respectively.
- [x] 'append' - This stores a list, and appends each argument value to the list. This is useful to allow an option to be specified multiple times.
- [x] 'append_const' - This stores a list, and appends the value specified by the const keyword argument to the list.
- [x] 'count' - This counts the number of times a keyword argument occurs.
- [x] 'help' - This prints a complete help message for all the options in the current parser and then exits.
- [x] 'version' - This expects a version= keyword argument in the add_argument() call, and prints version information and exits when invoked.
- [x] 'extend' - This stores a list, and extends each argument value to the list.
