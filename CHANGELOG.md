# Changelog

## [1.9.0] - 2023-12-11

  * Fix: link shared library with argparse_static
  * Add: SubParsers::add_parser_group (parser groups)
  * Add: argparse:split_to_args(str) function
  * Add: ArgumentParser::usage(argparse::SUPPRESS) support
  * Add: specifiers support for parser groups and parsers
  * Mark: ArgumentParser::Subparser as deprecated (use argparse::SubParsers)
  * Remove: deprecated code

## [1.8.4] - 2023-11-13

  * Fix: help generation with metavar array
  * Add: combined Argument::nargs support
  * Add: Argument::type(void (*)(std::string const&, void*)) factory function
  * Mark: Argument::type(void (*)(std::istream&, void*)) as deprecated

## [1.8.3] - 2023-11-06

  * Fix: Namespace::get for array and queue
  * Fix: Namespace::get for bool type
  * Add: Argument & ArgumentParser handle support for C++98
  * Add: ArgumentParser::has_subparsers function
  * Add: ArgumentParser::has_env function
  * Add: Argument::type(std::function<void(std::istream&, void*)>) conversion function
  * Mark: Argument::handle(std::function<void()>) as deprecated
  * Mark: ArgumentParser::handle(std::function<void()>) as deprecated
  * Mark: ArgumentParser::have_env as deprecated
  * Update: logo
  * Update: bash_completion output
  * Update: Namespace::get implementation

## [1.8.2] - 2023-08-20

  * Add: parser's aliases to parser_names
  * Add: ArgumentParser::list_env function
  * Update: bash completion generation
  * Fix: nargs SUPPRESS arguments processing
  * Fix: pass language to subparsers in parse_args()

## [1.8.1] - 2023-07-19

  * Allow to use and store default language in help, error, Namespace
  * Add: argparse::argparse_static target
  * Add: ArgumentParser::comment_prefix_chars
  * Fix: process_separate_arg_abbrev impl
  * Fix: indent for utf8 usage
  * Fix: utf8 string check

## [1.8.0] - 2023-06-17

  * Add: ArgumentParser ctor with environment variables
  * Add: ArgumentParser have_env, get_env functions
  * Add: build as static lib and use as decl header
  * Add: implementation for operand type arguments A=...
  * Add: Argument::nargs(argparse::SUPPRESS) support
  * Fix: Argument::help message clear in add_argument(Argument)
  * Fix: Argument::required value for positional arguments
  * Fix: Argument check for mutex groups
  * Fix: Subparser help (C++98)
  * Remove: deprecated code

## [1.7.6] - 2023-04-25

  * Fix: separator split for Namespace::get<tuple>()
  * Fix: tab replace in raw help formatters
  * Fix: _split_lines impl in RawTextHelpFormatter

## [1.7.5] - 2023-04-17

  * Add: mark ARGPARSE_TAB_SIZE define as deprecated (use HelpFormatter::_tab_size)
  * Add: Namespace::contains function
  * Fix: Use unicode 15 to upper case conversion
  * Update: parser not found error

## [1.7.4] - 2023-03-17

  * Set: additional formatter_class functions as deprecated
  * Update: HelpFormatter implementation
  * Update: shared_ptr impl for C++98
  * Fix: pkg-config prefix in launchpad build

## [1.7.3] - 2023-01-27

  * Fix: build with mxe
  * Update: diagnostic checks
  * Add: argparse::SUPPRESS support for Subparser::help
  * Add: detail::_to_u8upper for utf-8 non-latin symbols
  * Add: pkg-config support

## [1.7.2] - 2022-12-19

  * Add: mark ARGPARSE_NO_AUTODETECT define as deprecated
  * Fix: code scanning alerts
  * Fix: attribute names generation #19
  * Add: Argument construction with 3 and 4 flags for C++98
  * Add: ArgumentParser::self_test() function
  * Add: bash completion functions for ArgumentParser
  * Add: initializer_list overloaded functions for C++11+

## [1.7.1] - 2022-11-18

  * Update: terminal size detection defines
  * Add: Action::language support
  * Fix: compile with g++-4.1, g++-4.3 on ubuntu 10.04
  * Fix: compile with g++-3.4 on ubuntu 8.04
  * Add: Argument::choices(1st, 2nd, ...) function
  * Add: Argument::choice function
  * Add: ArgumentParser::usage_title function
  * Fix: detail::_to_string for custom types
  * Fix: Namespace::get containers of custom types
  * Add: ArgumentParser::try_parse_args functions

## [1.7.0] - 2022-10-21

  * Add: optionals_title and positionals_title functions
  * Fix: alignment in help for utf-8
  * Fix: only punctuation characters are allowed for prefix chars
  * Add: utf-8 support for flags
  * Remove: deprecated code

## [1.6.8] - 2022-09-30

  * Add: ARGPARSE_VERSION_NUM, ARGPARSE_VERSION_COMPILED, ARGPARSE_VERSION_AT_LEAST macros
  * Add: Argument choices, const, dest, help, metavar, nargs, option_strings, required, type format specifiers
  * Fix: typos in README.md and argparse.hpp (#12)
  * Fix: Argument 'no-' flag generation
  * Fix: help message if Argument metavar values are used

## [1.6.7] - 2022-09-22

  * Add: ArgumentParser::subparsers() function
  * Add: Subparser::parser_names function
  * Set: virtual inherit for HelpFormatter classes

## [1.6.6] - 2022-07-23

  * Update: C++98 shared_ptr implementation
  * Update: format tab up to ARGPARSE_TAB_SIZE (default 4) spaces
  * Fix: Allow share dest storage #9

## [1.6.5] - 2022-06-25

  * Add: missing ArgumentParser functions for C++98
  * Update: Argument::metavar functions

## [1.6.4] - 2022-06-18

  * Add: C++98 standard support for Namespace::get
  * Add: C++98 standard support for detail::Type

## [1.6.3] - 2022-06-17

  * Add: C++98 standard support
  * Fix: detail::_get_type_name() for GCC 4.6

## [1.6.2] - 2022-06-10

  * Fix: MSVC standard check
  * Use: handle as C++11+ feature
  * Fix: detail::_format_name implementation
  * Code refactoring and optimizing

## [1.6.1] - 2022-05-28

  * Fix: Argument default value using algorithm
  * Fix: MetavarTypeHelpFormatter using
  * Fix: avoid to add default to help if it exists
  * Update: Help formatting

## [1.6.0] - 2022-05-20

  * Set: std::string as basic type for std::string_view
  * Add: std::ostream& operator << overload for Namespace
  * Fix: copy default values from parents
  * Fix: usage generation for required options
  * Add: check for optional mutex arguments
  * Fix: argparse::SUPPRESS for positional arguments
  * Add: ARGPARSE_NO_AUTODETECT to disable terminal size auto-detection
  * Fix: help auto-formatting
  * Fix: show default value
  * Fix: optional argument flag name generation
  * Add: allow providing a tuple to metavar
  * Fix: customizing file parsing
  * Add: required flag to add_mutually_exclusive_group
  * Add: title and description to add_subparsers
  * Fix: don't show help for subparsers without help
  * Add: support for subparser aliases
  * Add: nargs(arparse::REMAINDER) or remainder() support
  * Add: %(prog)s format specifier for usage and help
  * Add: %(default)s format specifier for argument help
  * Add: set argument type if calling Argument::*_value<T>(T)
  * Remove: deprecated code
  * Add: argument flags, dest, parser name formatting

## [1.5.2] - 2022-05-13

  * Fix: Argument::choices({ A, B }) with 2 choices

## [1.5.1] - 2022-05-08

  * Fix: conflicting option string error
  * Fix: Namespace::try_get for std::array type
  * Remove: required const value for *_const actions
  * Remove: default value check for append_const actions
  * Code refactoring and optimizing

## [1.5.0] - 2022-04-30

  * Rewrite parents support implementation
  * Rewrite parser's prog name generation
  * Use ArgumentParser as Subparser's parser
  * Mark ArgumentParser::Parser deprecated
  * Fix: conflicting option error
  * Fix: mutually exclusive group help
  * Fix: arguments order in Namespace
  * Fix: subparsers required error message
  * Fix: required args check
  * Add: title, description ArgumentGroup setters

## [1.4.2] - 2022-04-23

  * Add paired containers support to Namespace::get()
  * Add tupled containers support to Namespace::get()
  * Add 2D containers of queue support to Namespace::get()
  * Update type check
  * Fix parseing if Namespace exists
  * Fix read args from file implementation
  * Fix type name determine on windows
  * Add base support for Parser's parents

## [1.4.1] - 2022-04-16

  * Fix code style issues
  * Update terminal size detection on Unix
  * Add Argument::implicit_value() support #4
  * Fix separating argument into abbreviations
  * Mark Namespace::try_get deprecated for C++14
  * Code refactoring and optimizing

## [1.4.0] - 2022-04-10

  * Add 2D containers support for Namespace::get()
  * Update help formatting
  * Allow to set output width
  * Add terminal size auto-detection
  * Rename ExclusiveGroup to MutuallyExclusiveGroup
  * Add add_argument(Argument) to ArgumentGroup and MutuallyExclusiveGroup

## [1.3.8] - 2022-03-26

  * Allow to add_argument(Argument().dest(...))
  * Allow to add_argument().dest(...)
  * Add Namespace::print() and Namespace::to_string()
  * Fix default value store
  * Don't handle none argument value
  * Fix nargs "?" with const and action "extend"
  * Fix Namespace::to_string(...)
  * Fix positional args store

## [1.3.7] - 2022-03-19

  * Add default value SUPPRESS in get_default
  * Remove SUPPRESS arg from Namespace
  * Refactor add_argument() functions
  * Add new Argument constructors
  * Fix ArgumentParser::exit implementation

## [1.3.6] - 2022-03-13

  * Fix delegating constructor call
  * Add GCC-4.8 support
  * Fix detail::_type_name() implementation for clang
  * Don't show default in help for help and version
  * Add default value SUPPRESS in help and parse_args result

## [1.3.5] - 2022-03-06

  * Adjust code for 80 characters
  * Add std::tuple support to Namespace::get<>()
  * Fix Namespace::try_get<std::pair<T, U> >()
  * Allow to use custom types in pair/tuple/containers
  * Add space delimiter impl to get tuple, paired and mapped types
  * Fix detail::_split_to_args implementation

## [1.3.4] - 2022-02-27

  * Move Namespace to argparse
  * Mark argparse::ArgumentParser::Namespace depracated for C++14+
  * Remove ignoring GCC 7 diagnostic '-Wimplicit-fallthrough'

## [1.3.3] - 2022-02-20

  * Use modern C++ features
  * Fix build warnings #1
  * Avoid to use both Argument::handle and Parser::handle functions
  * Add Parser::handle(std::function<void(Namespace)>) function #2
  * Add overloaded add_argument::(Argument) function #2
  * Remove excess print to stderr
  * Update examples

## [1.3.2] - 2022-02-13

  * Add char8_t support to Namespace::get<>()
  * Fix resolved -h/--help argument's default value
  * Update add_argument(std::string, Args...) function
  * Fix build with msvc
  * Fix subparser error usage generation
  * Update Argument::type() implementation
  * Code refactoring and optimizing

## [1.3.1] - 2022-01-30

  * Add argparse::BooleanOptionalAction action support
  * Fix format_help, format_usage
  * Add check for CLI arguments

## [1.3.0] - 2022-01-22

  * Allow to use containers with byte types in Namespace::get<>
  * Add Argument::add_argument(std::string, Args...) function
  * Add ArgumentParser::conflict_handler("resolve") support
  * Use conflict resolve for positional args and subparser
  * Update subparsers arguments storing (check exists() before get() or use try_get())
  * Fix required arguments error message

## [1.2.0] - 2022-01-15

  * Add RawTextHelpFormatter support
  * Add MetavarTypeHelpFormatter support
  * Add initial Argument::type support
  * Add pair type to Namespace::get<>()
  * Add byte types to Namespace::get<>()
  * Add std::string_view support to Namespace::get<>()

## [1.1.4] - 2022-01-10

  * Add RawDescriptionHelpFormatter support
  * Add ArgumentParser::add_formatter_class() function
  * Fix build with C++14+

## [1.1.3] - 2022-01-09

  * Add mapped types to Namespace::get<>()
  * Code refactoring and optimizing

## [1.1.2] - 2021-12-26

  * Code refactoring and optimizing

## [1.1.1] - 2021-12-12

  * Fix exists() and get() for 'count' actions
  * Add Namespace::try_get() with std::optional
  * Code optimizing

## [1.1.0] - 2021-11-21

  * Update ArgumentParser help printing functions
  * Update handle() realization
  * Allow to use Namespace in parse_args()
  * Customizing file parsing

## [1.0.1] - 2021-10-24

  * Fix subparser required parse error
  * Add string args parse support
  * Remove quotes in Argument::handle() call
  * Add Parser::handle() function

## [1.0.0] - 2021-10-17

  * Add ArgumentParser::parse_intermixed_args() function
  * Add ArgumentParser::parse_known_intermixed_args() function
  * Add exiting methods
  * Add custom type to Argument::default_value<>()
  * Add custom type to Argument::const_value<>()
  * Fix parent subparser detection
  * Fix prog name in generated usage
  * Code refactoring and optimizing

## [0.6.1] - 2021-10-16

  * Add BaseParser::add_mutually_exclusive_group() function
  * Code refactoring and optimizing

## [0.6.0] - 2021-10-10

  * Fix load args from file
  * Add ArgumentDefaultsHelpFormatter support
  * Add ArgumentParser::parse_known_args() function
  * Add argument groups
  * Code refactoring and optimizing

## [0.5.3] - 2021-09-26

  * Fix parse arguments with '=' prefix_chars
  * Fix optional arg finding
  * Fix subparsers line indent in help
  * Code refactoring and optimizing

## [0.5.2] - 2021-09-20

  * Add custom type to Namespace::get<>()
  * Fix default help flags generation
  * Fix subparser usage generation
  * Code refactoring and optimizing

## [0.5.1] - 2021-09-18

  * Allow subparser with negative number options present
  * Add conflicting option string check
  * Fix Argument::dest set
  * Add initial pseudo-argument '--' support

## [0.5.0] - 2021-09-17

  * Rename Argument::callback to Argument::handle
  * Add custom nargs functions: optional, zero_or_more, one_or_more
  * Code refactoring and optimizing

## [0.4.4] - 2021-09-14

  * Fix found data in Namespace if Subparser::dest() set
  * Code refactoring and optimizing

## [0.4.3] - 2021-09-13

  * Fix get argument values with spaces
  * Fix ArgumentParser::get_default() custom value
  * Fix parse optional arg with nargs='?'
  * Fix Namespace::to_args() and Namespace::to_string() functions
  * Code refactoring and optimizing

## [0.4.2] - 2021-09-12

  * Add ArgumentParser::set_defaults() function
  * Valid "" value for Argument::const_value/version/choices/metavar/default_value
  * Fix equal "" processing
  * Code refactoring

## [0.4.1] - 2021-09-06

  * Update subparsers support
  * Add Namespace::to_args() function
  * Add Argument::handle() function
  * Use Argument::callback() function to 'count' actions
  * Refactor parser classes
  * Update cpack generation info

## [0.4.0] - 2021-09-05

  * Add subparsers support
  * Add Namespace::exists() check function
  * Add setting argument choices from string
  * Fix main help and usage generation
  * Fix parse unrecognized optional arguments
  * Use Argument::callback() function to 'store_false' actions and positional arguments

## [0.3.0] - 2021-08-29

  * Add documentation for doxygen
  * Code refactoring and optimizing

## [0.2.5] - 2021-08-25

  * Add ArgumentParser::get_default() function
  * Add Argument::callback() function to 'store_true' arguments
  * Fix help message generation
  * Allow argparse::SUPPRESS value for argument help

## [0.2.4] - 2021-08-13

  * Fixed usage and help generation
  * Fixed store negative numbers
  * Fixed call dest() for positional arguments

## [0.2.3] - 2021-08-11

  * Fixed parse arguments with '?' nargs
  * Fixed a case of parsing with the last 2 optional arguments

## [0.2.2] - 2021-08-10

  * Fixed parse arguments containings '='

## [0.2.1] - 2021-08-09

  * Fixed checked actions in _check_abbreviations()
  * Code refactored

## [0.2.0] - 2021-08-09

## [0.1.3] - 2021-07-05

## [0.1.2] - 2021-07-02

## [0.1.1] - 2021-06-26

## [0.1.0] - 2021-06-17
