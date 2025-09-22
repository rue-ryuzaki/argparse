/*
 * Copyright (c) 2025 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

TEST_CASE("1. usage", "[argument_parser]")
{
    argparse::ArgumentParser parser = argparse::ArgumentParser()
            .output_width(60).exit_on_error(false);

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
