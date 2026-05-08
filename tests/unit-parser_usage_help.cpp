/*
 * Copyright (c) 2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. main usage", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser()
            .output_width(60).exit_on_error(false).color(false);

    parser.add_argument("store").action(argparse::store);
    parser.add_argument("--true").action(argparse::store_true);
    parser.add_argument("--false").action(argparse::store_false);
    parser.add_argument("append").action(argparse::append);
    parser.add_argument("-c").action(argparse::count);
    parser.add_argument("extend").action(argparse::extend);

    SECTION("1.1. default output width") {
        parser.output_width(80);

        CHECK(parser.format_usage()
              == "usage: untitled [-h] [--true] [--false] [-c] store append extend");
    }

    SECTION("1.2. default usage") {
        CHECK(parser.format_usage()
              == "usage: untitled [-h] [--true] [--false] [-c] store append\n"
                 "                extend");
    }

    SECTION("1.3. override usage") {
        parser.usage("untitled [-h] [--true] [--false] [-c] store append extend");

        CHECK(parser.format_usage()
              == "usage: untitled [-h] [--true] [--false] [-c] store append extend");
    }

    SECTION("1.4. override usage title") {
        parser.usage_title("my usage");

        CHECK(parser.format_usage()
              == "my usage: untitled [-h] [--true] [--false] [-c] store append\n"
                 "                   extend");
    }

    SECTION("1.5. small output width") {
        parser.output_width(30);

        CHECK(parser.format_usage()
              == "usage: untitled\n"
                 "       [-h] [--true] [--false]\n"
                 "       [-c] store append extend");
    }
}

TEST_CASE("2. subparser usage", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser()
            .output_width(80).exit_on_error(false).color(false);

    parser.add_argument("--true").action(argparse::store_true);
    parser.add_argument("--false").action(argparse::store_false);

    argparse::SubParsers& subparser = parser.add_subparsers();
    argparse::ArgumentParser& parser_a = subparser.add_parser("a")
            .add_argument(argparse::Argument("-b"));

    SECTION("2.1. default behaviour") {
        CHECK(parser.format_usage()
              == "usage: untitled [-h] [--true] [--false] {a} ...");
        CHECK(parser_a.format_usage()
              == "usage: untitled a [-h] [-b B]");
    }

    SECTION("2.2. override parent's usage") {
        parser.usage("custom");

        CHECK(parser.format_usage()
              == "usage: custom");
        CHECK(parser_a.format_usage()
              == "usage: untitled a [-h] [-b B]");
    }

    SECTION("2.3. override child's usage") {
        parser_a.usage("custom");

        CHECK(parser.format_usage()
              == "usage: untitled [-h] [--true] [--false] {a} ...");
        CHECK(parser_a.format_usage()
              == "usage: custom");
    }
}

TEST_CASE("3. help", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser()
            .description("description").epilog("epilog").output_width(80).exit_on_error(false).color(false);

    parser.add_argument("--action").action(argparse::BooleanOptionalAction).help("action help");
    parser.add_argument("-c", "--count").help("count help");

    SECTION("3.1. groups with title") {
        argparse::MutuallyExclusiveGroup& mutex = parser.add_mutually_exclusive_group();
        mutex.add_argument("--option1").help("option1 help");
        mutex.add_argument("--option2").help("option2 help");

        argparse::ArgumentGroup& group = parser.add_argument_group("title", "description");
        group.add_argument("--option3").help("option3 help");
        group.add_argument("option4").help("option4 help");
        CHECK(parser.format_help()
              == "usage: untitled [-h] [--action | --no-action] [-c COUNT] [--option3 OPTION3]\n"
                 "                [--option1 OPTION1 | --option2 OPTION2] option4\n"
                 "\n"
                 "description\n"
                 "\n"
                 "options:\n"
                 "  -h, --help            show this help message and exit\n"
                 "  --action, --no-action\n"
                 "                        action help\n"
                 "  -c, --count COUNT     count help\n"
                 "  --option1 OPTION1     option1 help\n"
                 "  --option2 OPTION2     option2 help\n"
                 "\n"
                 "title:\n"
                 "  description\n"
                 "\n"
                 "  --option3 OPTION3     option3 help\n"
                 "  option4               option4 help\n"
                 "\n"
                 "epilog");
    }

    SECTION("3.2. groups without title or description") {
        argparse::ArgumentGroup& group1 = parser.add_argument_group();
        group1.add_argument("--option1").help("option1 help");

        argparse::ArgumentGroup& group2 = parser.add_argument_group().title("title");
        group2.add_argument("--option2").help("option2 help");

        argparse::ArgumentGroup& group3 = parser.add_argument_group().description("description");
        group3.add_argument("--option3").help("option3 help");

        CHECK(parser.format_help()
              == "usage: untitled [-h] [--action | --no-action] [-c COUNT] [--option1 OPTION1]\n"
                 "                [--option2 OPTION2] [--option3 OPTION3]\n"
                 "\n"
                 "description\n"
                 "\n"
                 "options:\n"
                 "  -h, --help            show this help message and exit\n"
                 "  --action, --no-action\n"
                 "                        action help\n"
                 "  -c, --count COUNT     count help\n"
                 "\n"
                 "  --option1 OPTION1     option1 help\n"
                 "\n"
                 "title:\n"
                 "  --option2 OPTION2     option2 help\n"
                 "\n"
                 "  description\n"
                 "\n"
                 "  --option3 OPTION3     option3 help\n"
                 "\n"
                 "epilog");
    }
}
