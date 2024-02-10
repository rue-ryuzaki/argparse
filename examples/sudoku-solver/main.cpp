/*
* MIT License
*
* Simple CLI C++ sudoku puzzles solver (sudoku-solver)
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
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <argparse/argparse_decl.hpp>

#include "sudoku.hpp"

char const version[] = "%(prog)s v0.1.0";

int
main(int argc,
        char const* const argv[])
{
    argparse::ArgumentParser parser = argparse::ArgumentParser(argc, argv)
            .description("Simple CLI C++ sudoku puzzles solver")
            .fromfile_prefix_chars("@")
            .epilog("by rue-ryuzaki (c) 2022-2024");
    parser.add_argument("--version")
            .action("version")
            .version(version);
    parser.add_argument("--type")
            .choices("2x2", "2x3", "3x2", "3x3")
            .default_value("3x3")
            .help("sudoku size, in blocks (default: %(default)s)");
    parser.add_argument("--values")
            .nargs("*")
            .help("sudoku puzzle values");

    if (argc == 1) {
        parser.print_help();
        return 0;
    }

    argparse::Namespace const args = parser.parse_args();

    std::pair<std::size_t, std::size_t> type
            = args.get<std::pair<std::size_t, std::size_t> >("type", 'x');
    std::vector<int8_t> values = args.get<std::vector<int8_t> >("values");

    sudoku::sudoku_t<int8_t> sudoku(type.first, type.second);
    sudoku.load(values);

    if (!sudoku.is_valid()) {
        std::cerr << "[FAIL] Input sudoku puzzle is not valid:" << std::endl;
        sudoku.print(std::cerr);
        return 1;
    }

    std::cout << "[ .. ] Sudoku puzzle loaded: " << std::endl;
    sudoku.print();

    if (sudoku.solve()) {
        std::cout << "[ OK ] Sudoku puzzle solved:" << std::endl;
        sudoku.print();
    } else {
        std::cerr << "[FAIL] Can't solve sudoku puzzle" << std::endl;
        return 1;
    }

    return 0;
}
