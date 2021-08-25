# ArgumentParser v0.2.5
Python-like argument parser for C++11+ projects
## Code example
```
#include <iostream>

#include <argparse/argparse.hpp>

int main(int argc, char* argv[])
{
    auto parser = argparse::ArgumentParser(argc, argv)
            .prog("FooBar")
            .description("foo")
            .epilog("bar");
    parser.add_argument({ "-f", "--foo" }).help("FOO help").default_value("foo");
    parser.add_argument({ "-b", "--bar" }).help("BAR help").default_value("42");
    parser.print_help();
    auto const args = parser.parse_args();
    std::cout << "foo: " << args.get<std::string>("-f") << std::endl;
    std::cout << "bar: " << args.get<int>("--bar") << std::endl;
    return 0;
}
```
## Usage example
```
./a.out -f=foobar -b 101
```
## Output
```
usage: FooBar [-h] [-f FOO] [-b BAR]

foo

optional arguments:
  -h, --help         show this help message and exit
  -f FOO, --foo FOO  FOO help
  -b BAR, --bar BAR  BAR help

bar
foo: foobar
bar: 101
```
## ArgumentParser objects support
- [x] prog - The name of the program (default: argv[0] or "untitled")
- [x] usage - The string describing the program usage (default: generated from arguments added to parser)
- [x] description - Text to display before the argument help (default: "")
- [x] epilog - Text to display after the argument help (default: "")
- [x] parents - A list of ArgumentParser objects whose arguments should also be included
- [ ] formatter_class - A class for customizing the help output
- [x] prefix_chars - The set of characters that prefix optional arguments (default: "-")
- [x] fromfile_prefix_chars - The set of characters that prefix files from which additional arguments should be read (default: "")
- [x] argument_default - The global default value for arguments (default: "")
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