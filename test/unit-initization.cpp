/*
* Copyright (c) 2021-2023 Golubchikov Mihail <https://github.com/rue-ryuzaki>
*/

#include "./argparse_decl.hpp"
#include "./catch-define.h"

TEST_CASE("1. initization check", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser();

    SECTION("1.1. default values") {
        REQUIRE(parser.prog() == "untitled");
        REQUIRE(parser.usage() == "");
        REQUIRE(parser.description() == "");
        REQUIRE(parser.epilog() == "");
        REQUIRE(parser.prefix_chars() == "-");
        REQUIRE(parser.fromfile_prefix_chars() == "");
        REQUIRE(parser.argument_default() == "");
        REQUIRE(parser.add_help() == true);
        REQUIRE(parser.allow_abbrev() == true);
        REQUIRE(parser.exit_on_error() == true);
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

        REQUIRE(parser.prog() == prog);
        REQUIRE(parser.usage() == usage);
        REQUIRE(parser.description() == description);
        REQUIRE(parser.epilog() == epilog);
        REQUIRE(parser.prefix_chars() == prefix_chars);
        REQUIRE(parser.fromfile_prefix_chars() == fromfile_prefix_chars);
        REQUIRE(parser.argument_default() == argument_default);
        REQUIRE(parser.add_help() == add_help);
        REQUIRE(parser.allow_abbrev() == allow_abbrev);
        REQUIRE(parser.exit_on_error() == exit_on_error);

        // check properties, which can be cleared
        parser.usage("")
                .description("")
                .epilog("")
                .fromfile_prefix_chars("")
                .argument_default("");

        REQUIRE_FALSE(parser.usage() == usage);
        REQUIRE_FALSE(parser.description() == description);
        REQUIRE_FALSE(parser.epilog() == epilog);
        REQUIRE_FALSE(parser.fromfile_prefix_chars() == fromfile_prefix_chars);
        REQUIRE_FALSE(parser.argument_default() == argument_default);

        // check properties, which can't be cleared
        parser.prog("").prefix_chars("");

        REQUIRE(parser.prog() == prog);
        REQUIRE(parser.prefix_chars() == prefix_chars);
    }
}
