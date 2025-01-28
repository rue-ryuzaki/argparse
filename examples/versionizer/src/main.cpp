/* SPDX-License-Identifier: MIT
 *
 * MIT License
 *
 * Version organizer for C/C++ (Versionizer v0.1-rc2)
 *
 * Copyright (c) 2021-2024 Golubchikov Mihail
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

#include <argparse/argparse_decl.hpp>

#include "versionizer.hpp"

int
command(std::string const& str)
{
    return system(str.c_str());
}

int
main(int argc,
        char const* const argv[])
{
    argparse::ArgumentParser parser = argparse::ArgumentParser(argc, argv)
            .comment_prefix_chars("#")
            .fromfile_prefix_chars("@")
            .suggest_on_error(true)
            .description("Version manager for C/C++ projects")
            .epilog("by rue-ryuzaki (c) 2022-2024");
    parser.add_argument("--type")
            .choices("M", "MM", "MR", "MMP", "MMR", "MMPR")
            .required(true).help("version type");
    parser.add_argument("--source")
            .required(true).help("source file with version");
    parser.add_argument("--program")
            .required(true).help("program name in source file");
    parser.add_argument("--file")
            .action("append").nargs("+").help("file names with version M.M.P");
    parser.add_argument("--apply")
            .choices("major", "minor", "patch", "rc", "rc-major", "rc-minor")
            .default_value("")
            .help("version patch type");
    parser.add_argument("--patch")
            .action("store_true").help("patch version in source file");
    parser.add_argument("--commit")
            .action("store_true").help("commit changes in git repository");

    argparse::Namespace const args = parser.parse_args();

    versionizer::Versionizer ver;
    ver.set_type(args.get<std::string>("type"));

    std::string versionFile = args.get<std::string>("source");
    std::string programName = args.get<std::string>("program");
    std::cout << "program " << programName << std::endl;
    std::cout << "analize " << versionFile << std::endl;

    auto version = versionizer::Version::from_file(versionFile, programName);
    std::cout << "old version v" << version.to_string(ver.type()) << std::endl;

    std::string versionPatch = args.get<std::string>("apply");
    if (!versionPatch.empty()) {
        auto oldVersion = version;
        if (versionPatch == "major") {
            version.apply_major(ver.type());
        } else if (versionPatch == "minor") {
            version.apply_minor(ver.type());
        } else if (versionPatch == "patch") {
            version.apply_patch(ver.type());
        } else if (versionPatch == "rc") {
            version.apply_rc(ver.type());
        } else if (versionPatch == "rc-major") {
            version.apply_rc_major(ver.type());
        } else if (versionPatch == "rc-minor") {
            version.apply_rc_minor(ver.type());
        } else {
            throw std::logic_error(
                     "invalid apply version patch type '" + versionPatch + "'");
        }
        std::cout << "new version v" << version.to_string(ver.type())
                  << std::endl;
        if (args.get<bool>("patch")) {
            version.patch_file(versionFile, programName);
            auto const files = args.get<std::vector<std::string> >("file");
            for (auto const& file : files) {
                ver.patch_file(file, oldVersion, version);
            }
            if (args.get<bool>("commit")) {
                std::string message = "\"Set: version v"
                        + version.to_string(ver.type()) + "\"";
                if (command("git add " + versionFile) == 0
                        && command("git commit -m " + message) == 0) {
                    std::cout << "commit " << message << " done" << std::endl;
                }
            }
        }
    }
    return 0;
}
