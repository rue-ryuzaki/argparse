/* SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Tool for checking the width of lines of code (code-width-checker)
 *
 * Copyright (c) 2022-2024 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <argparse/argparse_decl.hpp>

char const version[] = "%(prog)s v0.1.0";
std::size_t const default_limit = 80;

int
main(int argc,
        char const* const argv[])
{
    argparse::ArgumentParser parser = argparse::ArgumentParser(argc, argv)
            .description("Tool for checking the width of lines of code")
            .fromfile_prefix_chars("@")
            .formatter_class(argparse::ArgumentDefaultsHelpFormatter)
            .epilog("by rue-ryuzaki (c) 2022-2024");
    parser.add_argument("--version")
            .action("version")
            .version(version);
    parser.add_argument("--show_lines")
            .action("store_true")
            .help("show line numbers");
    parser.add_argument("-l", "--limit")
            .action("store")
            .default_value<std::size_t>(default_limit)
            .help("set line width limit");
    parser.add_argument("file")
            .action("extend")
            .one_or_more()
            .help("file names");

    if (argc == 1) {
        parser.print_help();
        return 0;
    }

    argparse::Namespace const args = parser.parse_args();
    std::size_t limit = args.get<std::size_t>("limit");
    std::vector<std::string> files
            = args.get<std::vector<std::string> >("file");
    bool show_lines = args.get<bool>("show_lines");

    for (std::size_t i = 0; i < files.size(); ++i) {
        std::string const& file = files.at(i);
        std::ifstream is(file.c_str());
        std::cout << "[ .. ] try to open file: '" << file << "'" << std::endl;
        if (!is.is_open()) {
            std::cout << "[FAIL] no such file: '" << file << "'" << std::endl;
            continue;
        }
        std::size_t overlines = 0;
        std::size_t oversymbols = 0;
        std::size_t maxwidth = 0;
        std::size_t maxline = 0;
        std::size_t index = 0;
        std::string line;
        std::vector<std::size_t> overlinesindexes;
        while (std::getline(is, line)) {
            ++index;
            std::size_t size = line.size();
            if (maxwidth < size) {
                maxwidth = size;
                maxline = index;
            }
            if (size > limit) {
                ++overlines;
                overlinesindexes.push_back(index);
                oversymbols += size - limit;
            }
        }
        std::cout << "[ .. ] report:" << std::endl;
        std::cout << "[ .. ] total lines:        " << index << std::endl;
        std::cout << (maxwidth > limit ? "[FAIL]" : "[ OK ]")
                  << " maximal line width: " << maxwidth << " (limit: " << limit
                  << ") at line " << maxline << std::endl;
        std::cout << (oversymbols > 0 ? "[FAIL]" : "[ OK ]")
                  << " overwidth symbols:  " << oversymbols << std::endl;
        std::cout << (overlines > 0 ? "[FAIL]" : "[ OK ]")
                  << " overwidth lines:    " << overlines << std::endl;
        if (show_lines && !overlinesindexes.empty()) {
            std::cout << "[FAIL] overwidth line numbers:" << std::endl;
            std::cout << "       " << overlinesindexes.front();
            for (std::size_t i = 1; i < overlinesindexes.size(); ++i) {
                std::cout << ", " << overlinesindexes.at(i);
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
