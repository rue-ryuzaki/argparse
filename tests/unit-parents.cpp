/*
 * Copyright (c) 2021-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. parents", "[argument_parser]")
{
    SECTION("1.1. ArgumentParser help conflict") {
        argparse::ArgumentParser parent1 = argparse::ArgumentParser();
        argparse::ArgumentParser parent2 = argparse::ArgumentParser().add_help(false);
        argparse::ArgumentParser parent3 = argparse::ArgumentParser().prefix_chars("+");

        CHECK_THROWS(argparse::ArgumentParser().parents(parent1));
        CHECK_NOTHROW(argparse::ArgumentParser().add_help(false).parents(parent1));
        CHECK_NOTHROW(argparse::ArgumentParser().parents(parent2));
        CHECK_NOTHROW(argparse::ArgumentParser().parents(parent3));
    }

    SECTION("1.2. Parser help conflict") {
        argparse::ArgumentParser parent1 = argparse::ArgumentParser();
        argparse::ArgumentParser parent2 = argparse::ArgumentParser().add_help(false);
        argparse::ArgumentParser parent3 = argparse::ArgumentParser().prefix_chars("+");

        argparse::ArgumentParser parser = argparse::ArgumentParser();
        argparse::SubParsers& subparsers = parser.add_subparsers();

        CHECK_THROWS(subparsers.add_parser("1").parents(parent1));
        CHECK_NOTHROW(subparsers.add_parser("2").add_help(false).parents(parent1));
        CHECK_NOTHROW(subparsers.add_parser("3").parents(parent2));
        CHECK_NOTHROW(subparsers.add_parser("4").parents(parent3));
    }
}
