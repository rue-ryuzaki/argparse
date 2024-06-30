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

#include "versionizer.hpp"

#include <fstream>
#include <regex>
#include <stdexcept>

namespace versionizer {
namespace detail {
// ----------------------------------------------------------------------------
inline std::regex
_create_regex(
        std::string const& name,
        std::string const& type)
{
    return std::regex("#define[\\s-[\\r\\n]]*" + name
                      + "_VERSION_" + type + "[\\s-[\\r\\n]]*([0-9]+)");
}

// ----------------------------------------------------------------------------
inline std::regex
_create_regex_major(
        std::string const& name)
{
    return _create_regex(name, "MAJOR");
}

// ----------------------------------------------------------------------------
inline std::regex
_create_regex_minor(
        std::string const& name)
{
    return _create_regex(name, "MINOR");
}

// ----------------------------------------------------------------------------
inline std::regex
_create_regex_patch(
        std::string const& name)
{
    return _create_regex(name, "PATCH");
}

// ----------------------------------------------------------------------------
inline std::regex
_create_regex_rc(
        std::string const& name)
{
    return _create_regex(name, "RC");
}
}  // namespace detail

// ----------------------------------------------------------------------------
// -- Version -----------------------------------------------------------------
// ----------------------------------------------------------------------------
Version&
Version::apply_major(
        Type type)
{
    validate(type);
    switch (type) {
        case M :
        case MM :
        case MMP :
            ++major;
            minor = 0;
            patch = 0;
            rc = 0;
            break;
        case MR :
            if (rc == 0) {
                ++major;
            } else {
                rc = 0;
            }
            minor = 0;
            patch = 0;
            break;
        case MMR :
            if (rc == 0) {
                ++major;
                minor = 0;
            } else {
                if (minor > 0) {
                    throw std::invalid_argument("error: invalid minor version");
                }
                rc = 0;
            }
            patch = 0;
            break;
        case MMPR :
            if (rc == 0) {
                ++major;
                minor = 0;
                patch = 0;
            } else {
                if (minor > 0) {
                    throw std::invalid_argument("error: invalid minor version");
                }
                if (patch > 0) {
                    throw std::invalid_argument("error: invalid patch version");
                }
                rc = 0;
            }
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
    return *this;
}

// ----------------------------------------------------------------------------
Version&
Version::apply_minor(
        Type type)
{
    validate(type);
    switch (type) {
        case MM :
        case MMP :
            ++minor;
            patch = 0;
            rc = 0;
            break;
        case MMR :
            if (rc == 0) {
                ++minor;
            } else {
                rc = 0;
            }
            patch = 0;
            break;
        case MMPR :
            if (rc == 0) {
                ++minor;
                patch = 0;
            } else {
                if (patch > 0) {
                    throw std::invalid_argument("error: invalid patch version");
                }
                rc = 0;
            }
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
    return *this;
}

// ----------------------------------------------------------------------------
Version&
Version::apply_patch(
        Type type)
{
    validate(type);
    switch (type) {
        case MMP :
            ++patch;
            rc = 0;
            break;
        case MMPR :
            if (rc == 0) {
                ++patch;
            } else {
                throw std::invalid_argument("error: invalid rc version");
            }
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
    return *this;
}

// ----------------------------------------------------------------------------
Version&
Version::apply_rc(
        Type type)
{
    validate(type);
    switch (type) {
        case MR :
            if (rc == 0) {
                ++major;
            } else if (major <= 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            ++rc;
            minor = 0;
            patch = 0;
            break;
        case MMR :
            if (rc == 0) {
                ++major;
                minor = 0;
            } else if (major <= 0) {
                throw std::invalid_argument("error: invalid minor version");
            } else if (minor > 0) {
                throw std::invalid_argument("error: invalid minor version");
            }
            ++rc;
            patch = 0;
            break;
        case MMPR :
            if (rc == 0) {
                ++minor;
            } else if (minor <= 0) {
                throw std::invalid_argument("error: invalid minor version");
            } else if (patch > 0) {
                throw std::invalid_argument("error: invalid patch version");
            }
            ++rc;
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
    return *this;
}

// ----------------------------------------------------------------------------
Version&
Version::apply_rc_major(
        Type type)
{
    validate(type);
    switch (type) {
        case MR :
            if (rc == 0) {
                ++major;
            } else if (major <= 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            ++rc;
            minor = 0;
            patch = 0;
            break;
        case MMR :
            if (rc == 0) {
                ++major;
                minor = 0;
            } else if (major <= 0) {
                throw std::invalid_argument("error: invalid major version");
            } else if (minor > 0) {
                throw std::invalid_argument("error: invalid minor version");
            }
            ++rc;
            patch = 0;
            break;
        case MMPR :
            if (rc == 0) {
                ++major;
                minor = 0;
                patch = 0;
            } else if (major <= 0) {
                throw std::invalid_argument("error: invalid major version");
            } else if (minor > 0) {
                throw std::invalid_argument("error: invalid minor version");
            } else if (patch > 0) {
                throw std::invalid_argument("error: invalid patch version");
            }
            ++rc;
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
    return *this;
}

// ----------------------------------------------------------------------------
Version&
Version::apply_rc_minor(
        Type type)
{
    validate(type);
    switch (type) {
        case MMPR :
            if (rc == 0) {
                ++minor;
                patch = 0;
            } else if (minor <= 0) {
                throw std::invalid_argument("error: invalid minor version");
            } else if (patch > 0) {
                throw std::invalid_argument("error: invalid patch version");
            }
            ++rc;
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
    return *this;
}

// ----------------------------------------------------------------------------
std::string
Version::to_string(
        Type type) const
{
    validate(type);
    switch (type) {
        case M :
            return std::to_string(major);
        case MM :
            return std::to_string(major) + "." + std::to_string(minor);
        case MR :
            if (rc == 0) {
                return std::to_string(major);
            } else {
                return std::to_string(major) + "-rc" + std::to_string(rc);
            }
        case MMP :
            return std::to_string(major)
                    + "." + std::to_string(minor)
                    + "." + std::to_string(patch);
        case MMR :
            if (rc == 0) {
                return std::to_string(major) + "." + std::to_string(minor);
            } else {
                return std::to_string(major) + "-rc" + std::to_string(rc);
            }
        case MMPR :
            if (rc == 0) {
                return std::to_string(major)
                        + "." + std::to_string(minor)
                        + "." + std::to_string(patch);
            } else {
                return std::to_string(major)
                        + "." + std::to_string(minor)
                        + "-rc" + std::to_string(rc);
            }
        default :
            throw std::logic_error("error: invalid version type");
    }
}

// ----------------------------------------------------------------------------
Version
Version::from_string(
        std::string const& str)
{
    Version res{ -1, -1, -1, -1 };
    std::smatch match;
    if (std::regex_search(str, match, std::regex("^([0-9]+)$"))) {
        res.major = std::stoi(std::string(match[1]));
        res.minor = 0;
        res.patch = 0;
        res.rc = 0;
    } else if (std::regex_search(
                   str, match, std::regex("^([0-9]+).([0-9]+)$"))) {
        res.major = std::stoi(std::string(match[1]));
        res.minor = std::stoi(std::string(match[2]));
        res.patch = 0;
        res.rc = 0;
    } else if (std::regex_search(
                   str, match, std::regex("^([0-9]+).([0-9]+).([0-9]+)$"))) {
        res.major = std::stoi(std::string(match[1]));
        res.minor = std::stoi(std::string(match[2]));
        res.patch = std::stoi(std::string(match[3]));
        res.rc = 0;
    } else if (std::regex_search(
                   str, match, std::regex("^([0-9]+)-rc([0-9]+)$"))) {
        res.major = std::stoi(std::string(match[1]));
        res.minor = 0;
        res.patch = 0;
        res.rc = std::stoi(std::string(match[2]));
    } else if (std::regex_search(
                   str, match, std::regex("^([0-9]+).([0-9]+)-rc([0-9]+)$"))) {
        res.major = std::stoi(std::string(match[1]));
        res.minor = std::stoi(std::string(match[2]));
        res.patch = 0;
        res.rc = std::stoi(std::string(match[3]));
    }
    return res;
}

// ----------------------------------------------------------------------------
void
Version::patch_file(
        std::string const& file,
        std::string const& name) const
{
    std::vector<std::string> data;
    std::ifstream is(file.c_str());
    if (is.is_open()) {
        std::regex const regex_major = detail::_create_regex_major(name);
        std::regex const regex_minor = detail::_create_regex_minor(name);
        std::regex const regex_patch = detail::_create_regex_patch(name);
        std::regex const regex_rc = detail::_create_regex_rc(name);
        std::smatch match;
        std::string line;
        while (std::getline(is, line)) {
            if (std::regex_search(line, match, regex_major)) {
                line.resize(line.size() - std::string(match[1]).size());
                line += std::to_string(major);
            } else if (std::regex_search(line, match, regex_minor)) {
                line.resize(line.size() - std::string(match[1]).size());
                line += std::to_string(minor);
            } else if (std::regex_search(line, match, regex_patch)) {
                line.resize(line.size() - std::string(match[1]).size());
                line += std::to_string(patch);
            } else if (std::regex_search(line, match, regex_rc)) {
                line.resize(line.size() - std::string(match[1]).size());
                line += std::to_string(rc);
            }
            data.push_back(line);
        }
        is.close();
    }
    std::ofstream os(file.c_str(), std::ios::out | std::ios::trunc);
    for (auto const& line : data) {
        os << line << std::endl;
    }
}

// ----------------------------------------------------------------------------
Version
Version::from_file(
        std::string const& file,
        std::string const& name)
{
    Version res{ -1, -1, -1, -1 };
    std::ifstream is(file.c_str());
    if (is.is_open()) {
        std::regex const regex_major = detail::_create_regex_major(name);
        std::regex const regex_minor = detail::_create_regex_minor(name);
        std::regex const regex_patch = detail::_create_regex_patch(name);
        std::regex const regex_rc = detail::_create_regex_rc(name);
        std::smatch match;
        std::string line;
        while (std::getline(is, line)) {
            if (std::regex_search(line, match, regex_major)) {
                res.major = std::stoi(std::string(match[1]));
            } else if (std::regex_search(line, match, regex_minor)) {
                res.minor = std::stoi(std::string(match[1]));
            } else if (std::regex_search(line, match, regex_patch)) {
                res.patch = std::stoi(std::string(match[1]));
            } else if (std::regex_search(line, match, regex_rc)) {
                res.rc = std::stoi(std::string(match[1]));
            }
        }
        is.close();
    }
    return res;
}

// ----------------------------------------------------------------------------
void
Version::validate(
        Type type) const
{
    switch (type) {
        case M :
            if (major < 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            break;
        case MM :
            if (major < 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            if (minor < 0) {
                throw std::invalid_argument("error: invalid minor version");
            }
            break;
        case MR :
            if (major < 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            if (rc < 0) {
                throw std::invalid_argument("error: invalid rc version");
            }
            break;
        case MMP :
            if (major < 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            if (minor < 0) {
                throw std::invalid_argument("error: invalid minor version");
            }
            if (patch < 0) {
                throw std::invalid_argument("error: invalid patch version");
            }
            break;
        case MMR :
            if (major < 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            if (minor < 0) {
                throw std::invalid_argument("error: invalid minor version");
            }
            if (rc < 0) {
                throw std::invalid_argument("error: invalid rc version");
            }
            break;
        case MMPR :
            if (major < 0) {
                throw std::invalid_argument("error: invalid major version");
            }
            if (minor < 0) {
                throw std::invalid_argument("error: invalid minor version");
            }
            if (patch < 0) {
                throw std::invalid_argument("error: invalid patch version");
            }
            if (rc < 0) {
                throw std::invalid_argument("error: invalid rc version");
            }
            break;
        default :
            throw std::logic_error("error: invalid version type");
    }
}

// ----------------------------------------------------------------------------
// -- Versionizer -------------------------------------------------------------
// ----------------------------------------------------------------------------
Versionizer::Versionizer()
    : m_type(None)
{ }

// ----------------------------------------------------------------------------
void
Versionizer::set_type(
        std::string const& value)
{
    if (value == "M") {
        m_type = M;
    } else if (value == "MM") {
        m_type = MM;
    } else if (value == "MR") {
        m_type = MR;
    } else if (value == "MMP") {
        m_type = MMP;
    } else if (value == "MMR") {
        m_type = MMR;
    } else if (value == "MMPR") {
        m_type = MMPR;
    } else {
        throw std::invalid_argument(
                    "error: unknown version type '" + value + "'");
    }
}

// ----------------------------------------------------------------------------
Type
Versionizer::type() const
{
    return m_type;
}

// ----------------------------------------------------------------------------
std::string
Versionizer::to_string(
        Version const& ver) const
{
    return ver.to_string(type());
}

// ----------------------------------------------------------------------------
void
Versionizer::patch_file(
        std::string const& file,
        Version const& oldVersion,
        Version const& newVersion,
        bool replaceOnce) const
{
    std::string const oldVer = oldVersion.to_string(type());
    std::string const newVer = newVersion.to_string(type());
    std::vector<std::string> data;
    std::ifstream is(file.c_str());
    if (is.is_open()) {
        bool replaced = false;
        std::string line;
        while (std::getline(is, line)) {
            std::size_t pos = line.find(oldVer);
            if (pos != std::string::npos && (!replaceOnce || !replaced)) {
                line.replace(pos, oldVer.length(), newVer);
                replaced = true;
            }
            data.push_back(line);
        }
        is.close();
    }
    std::ofstream os(file.c_str(), std::ios::out | std::ios::trunc);
    for (auto const& line : data) {
        os << line << std::endl;
    }
}
}  // namespace versionizer
