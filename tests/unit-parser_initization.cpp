/*
 * Copyright (c) 2021-2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. initization check", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser();

    SECTION("1.1. default values") {
        CHECK(parser.prog() == "untitled");
        CHECK(parser.usage() == "");
        CHECK(parser.description() == "");
        CHECK(parser.epilog() == "");
        CHECK(parser.prefix_chars() == "-");
        CHECK(parser.fromfile_prefix_chars() == "");
        CHECK(parser.argument_default() == "");
        CHECK(parser.add_help() == true);
        CHECK(parser.allow_abbrev() == true);
        CHECK(parser.exit_on_error() == true);
    }

    SECTION("1.2. change values") {
        std::string prog = "prog";
        std::string usage = "prog usage";
        std::string description = "description";
        std::string epilog = "epilog";
        std::string prefix_chars = "-+/";
        std::string fromfile_prefix_chars = "@";
        std::string argument_default = "42";
        bool add_help = false;
        bool allow_abbrev = false;
        bool exit_on_error = false;

        parser.prog(prog)
                .usage(usage)
                .description(description)
                .epilog(epilog)
                .prefix_chars(prefix_chars)
                .fromfile_prefix_chars(fromfile_prefix_chars)
                .argument_default(argument_default)
                .add_help(add_help)
                .allow_abbrev(allow_abbrev)
                .exit_on_error(exit_on_error);

        CHECK(parser.prog() == prog);
        CHECK(parser.usage() == usage);
        CHECK(parser.description() == description);
        CHECK(parser.epilog() == epilog);
        CHECK(parser.prefix_chars() == prefix_chars);
        CHECK(parser.fromfile_prefix_chars() == fromfile_prefix_chars);
        CHECK(parser.argument_default() == argument_default);
        CHECK(parser.add_help() == add_help);
        CHECK(parser.allow_abbrev() == allow_abbrev);
        CHECK(parser.exit_on_error() == exit_on_error);

        // check properties, which can be cleared
        parser.usage("")
                .description("")
                .epilog("")
                .fromfile_prefix_chars("")
                .argument_default("");

        CHECK_FALSE(parser.usage() == usage);
        CHECK_FALSE(parser.description() == description);
        CHECK_FALSE(parser.epilog() == epilog);
        CHECK_FALSE(parser.fromfile_prefix_chars() == fromfile_prefix_chars);
        CHECK_FALSE(parser.argument_default() == argument_default);

        // check properties, which can't be cleared
        parser.prog("").prefix_chars("");

        CHECK(parser.prog() == prog);
        CHECK(parser.prefix_chars() == prefix_chars);
    }
}
