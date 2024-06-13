/* SPDX-License-Identifier: MIT
 *
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

#ifndef _SUDOKU_SOLVER_SUDOKU_HPP_
#define _SUDOKU_SOLVER_SUDOKU_HPP_

#if __cplusplus >= 201103L
#include <cstdint>
#else
#include <stdint.h>
#endif  // C++11+

#include <cstddef>
#include <iostream>
#include <vector>

namespace sudoku {
template <class T>
class sudoku_t
{
public:
    typedef T                               value_type;
    typedef std::vector<std::vector<T> >    board_type;

    explicit
    sudoku_t(
            std::size_t x = 3,
            std::size_t y = 3)
        : m_x(x),
          m_y(y),
          m_board()
    {
        m_board.resize(m_x * m_y, std::vector<T>(m_y * m_x, T()));
    }

    sudoku_t(
            sudoku_t const& orig)
        : m_x(orig.m_x),
          m_y(orig.m_y),
          m_board(orig.m_board)
    { }

    inline sudoku_t&
    operator =(
            sudoku_t const& rhs)
    {
        if (this != &rhs) {
            m_x     = rhs.m_x;
            m_y     = rhs.m_y;
            m_board = rhs.m_board;
        }
        return *this;
    }

#if __cplusplus >= 201103L
    explicit
    sudoku_t(
            sudoku_t&& orig) noexcept
        : m_x(std::move(orig.m_x)),
          m_y(std::move(orig.m_y)),
          m_board(std::move(orig.m_board))
    { }

    inline sudoku_t&
    operator =(
            sudoku_t&& rhs) noexcept
    {
        if (this != &rhs) {
            m_x     = std::move(rhs.m_x);
            m_y     = std::move(rhs.m_y);
            m_board = std::move(rhs.m_board);
        }
        return *this;
    }
#endif  // C++11+

    inline board_type const&
    operator ()() const
    {
        return m_board;
    }

    inline board_type const&
    board() const
    {
        return m_board;
    }

    inline std::size_t
    block_width() const
    {
        return m_x;
    }

    inline std::size_t
    block_height() const
    {
        return m_y;
    }

    inline std::size_t
    blocks_at_x() const
    {
        return m_y;
    }

    inline std::size_t
    blocks_at_y() const
    {
        return m_x;
    }

    inline std::size_t
    max_value() const
    {
        return block_width() * block_height();
    }

    inline std::size_t
    board_width() const
    {
        return block_width() * blocks_at_x();
    }

    inline std::size_t
    board_height() const
    {
        return block_height() * blocks_at_y();
    }

    inline std::size_t
    elements() const
    {
        return board_width() * board_height();
    }

    inline void
    reset()
    {
        for (std::size_t i = 0; i < m_board.size(); ++i) {
            for (std::size_t j = 0; j < m_board.at(i).size(); ++j) {
                m_board.at(i).at(j) = T();
            }
        }
    }

    inline void
    load(std::vector<T> const& values)
    {
        for (std::size_t i = 0; i < elements(); ++i) {
            std::size_t x = i % board_width();
            std::size_t y = i / board_width();
            m_board[y][x] = i < values.size() ? values.at(i) : T();
        }
    }

    inline T
    get(std::size_t x,
            std::size_t y) const
    {
        return m_board.at(y).at(x);
    }

    inline bool
    can_set(std::size_t x,
            std::size_t y,
            T value) const
    {
        return x < board_width() && y < board_height() && value <= max_value();
    }

    inline bool
    set(std::size_t x,
            std::size_t y,
            T value)
    {
        if (!can_set(x, y, value)) {
            return false;
        }
        m_board[y][x] = value;
        return true;
    }

    inline void
    print(std::ostream& os = std::cout) const
    {
        for (std::size_t y = 0; y < board_height(); ++y) {
            if (y > 0 && y % block_height() == 0) {
                for (std::size_t x = 0; x < board_width(); ++x) {
                    if (x > 0 && x % block_width() == 0) {
                        os << "+";
                    }
                    os << "--";
                }
                os << std::endl;
            }
            for (std::size_t x = 0; x < board_width(); ++x) {
                if (x > 0 && x % block_width() == 0) {
                    os << "|";
                }
                if (m_board[y][x] != T()) {
                    if (m_board[y][x] < 10) {
                        os << " ";
                    }
                    os << int32_t(m_board[y][x]);
                } else {
                    os << "  ";
                }
            }
            os << std::endl;
        }
    }

    inline bool
    is_valid() const
    {
        return is_valid(m_board);
    }

    inline bool
    is_solved() const
    {
        return is_solved(m_board);
    }

    inline bool
    solve()
    {
        if (!is_valid()) {
            return false;
        }
        if (is_solved()) {
            return true;
        }
        return on_solve(m_board, 0);
    }

    static bool
    is_valid(
            sudoku_t<T> const& s)
    {
        return s.is_valid();
    }

    static bool
    is_solved(
            sudoku_t<T> const& s)
    {
        return s.is_solved();
    }

    static bool
    solve(sudoku_t<T>& s)
    {
        return s.solve();
    }

private:
    inline bool
    on_solve(
            board_type& board,
            std::size_t index) const
    {
        for (std::size_t i = index; i < elements(); ++i) {
            std::size_t x = i % board_width();
            std::size_t y = i / board_width();
            if (board[y][x] == T()) {
                for (std::size_t v = 1; v <= max_value(); ++v) {
                    board[y][x] = T(v);
                    if (!is_incremental_valid(board, x, y)) {
                        continue;
                    }
                    if (on_solve(board, i + 1)) {
                        return true;
                    }
                }
                board[y][x] = T();
                return false;
            }
        }
        return is_solved(board);
    }

    inline bool
    is_incremental_valid(
            board_type const& board,
            std::size_t px,
            std::size_t py) const
    {
        // check rows
        {
            std::size_t y = py;
            std::vector<bool> map(max_value() + 1);
            for (std::size_t x = 0; x < board_width(); ++x) {
                if (board[y][x] != T()) {
                    if (map[board[y][x]]) {
                        return false;
                    }
                    map[board[y][x]] = true;
                }
            }
        }
        // check cols
        {
            std::size_t x = px;
            std::vector<bool> map(max_value() + 1);
            for (std::size_t y = 0; y < board_height(); ++y) {
                if (board[y][x] != T()) {
                    if (map[board[y][x]]) {
                        return false;
                    }
                    map[board[y][x]] = true;
                }
            }
        }
        // check blocks
        {
            std::size_t ix = px / blocks_at_x();
            std::size_t iy = py / blocks_at_y();
            std::vector<bool> map(max_value() + 1);
            for (std::size_t iix = 0; iix < block_width(); ++iix) {
                for (std::size_t iiy = 0; iiy < block_height(); ++iiy) {
                    std::size_t x = ix * block_width() + iix;
                    std::size_t y = iy * block_height() + iiy;
                    if (board[y][x] != T()) {
                        if (map[board[y][x]]) {
                            return false;
                        }
                        map[board[y][x]] = true;
                    }
                }
            }
        }
        return true;
    }

    inline bool
    is_valid(
            board_type const& board) const
    {
        // check values
        for (std::size_t y = 0; y < board_height(); ++y) {
            for (std::size_t x = 0; x < board_width(); ++x) {
                if (static_cast<std::size_t>(board[y][x]) > max_value()) {
                    return false;
                }
            }
        }
        // check rows
        for (std::size_t y = 0; y < board_height(); ++y) {
            std::vector<bool> map(max_value() + 1);
            for (std::size_t x = 0; x < board_width(); ++x) {
                if (board[y][x] != T()) {
                    if (map[board[y][x]]) {
                        return false;
                    }
                    map[board[y][x]] = true;
                }
            }
        }
        // check cols
        for (std::size_t x = 0; x < board_width(); ++x) {
            std::vector<bool> map(max_value() + 1);
            for (std::size_t y = 0; y < board_height(); ++y) {
                if (board[y][x] != T()) {
                    if (map[board[y][x]]) {
                        return false;
                    }
                    map[board[y][x]] = true;
                }
            }
        }
        // check blocks
        for (std::size_t ix = 0; ix < blocks_at_x(); ++ix) {
            for (std::size_t iy = 0; iy < blocks_at_y(); ++iy) {
                std::vector<bool> map(max_value() + 1);
                for (std::size_t iix = 0; iix < block_width(); ++iix) {
                    for (std::size_t iiy = 0; iiy < block_height(); ++iiy) {
                        std::size_t x = ix * block_width() + iix;
                        std::size_t y = iy * block_height() + iiy;
                        if (board[y][x] != T()) {
                            if (map[board[y][x]]) {
                                return false;
                            }
                            map[board[y][x]] = true;
                        }
                    }
                }
            }
        }
        return true;
    }

    inline bool
    is_solved(
            board_type const& board) const
    {
        for (std::size_t y = 0; y < board_height(); ++y) {
            for (std::size_t x = 0; x < board_width(); ++x) {
                if (board[y][x] == T()) {
                    return false;
                }
            }
        }
        return true;
    }

    std::size_t m_x;
    std::size_t m_y;
    board_type m_board;
};
}  // namespace sudoku

#endif // _SUDOKU_SOLVER_SUDOKU_HPP_
