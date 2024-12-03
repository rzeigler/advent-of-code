#include <array>
#include <cstddef>
#include <functional>
#include <future>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>

class Matrix {
public:
  Matrix(std::string &&data, int row_len) : data(data), row_len(row_len) {
    if (data.length() % row_len != 0) {
      throw std::logic_error("invalid matrix construction");
    }
  }

  explicit Matrix(std::istream &is) {
    std::string line;
    std::getline(is, line);
    if (line.empty()) {
      data = "";
      row_len = 0;
    } else {
      data.append(line);
      row_len = line.length();
      while (std::getline(is, line) && !line.empty()) {
        data.append(line);
      }
    }
  }

  inline char at(size_t row, size_t col) const {
    return data[row * row_len + col];
  }

  inline bool in_bounds(ptrdiff_t row, ptrdiff_t col) const {
    bool row_bounds = 0 <= row && row < static_cast<ptrdiff_t>(rows());
    bool col_bounds = 0 <= col && col < static_cast<ptrdiff_t>(cols());
    return row_bounds && col_bounds;
  }

  inline size_t rows() const { return data.length() / row_len; }

  inline size_t cols() const { return row_len; }

  void print(std::ostream &os) const {
    os << data.length() / row_len << " x " << row_len;
    for (size_t i = 0; i < data.length() / row_len; i++) {
      std::string_view row{data.cbegin() + i * row_len,
                           data.cbegin() + (i + 1) * row_len};
      os << row << std::endl;
    }
  }

private:
  std::string data;
  unsigned int row_len;
};

uint32_t
search(Matrix const &matrix,
       std::function<int(Matrix const &, size_t row, size_t col)> search_impl);

uint32_t part1(Matrix const &matrix, size_t row, size_t col);

uint32_t part2(Matrix const &matrix, size_t row, size_t col);

using Offset = std::tuple<ptrdiff_t, ptrdiff_t>;
using Pattern = std::tuple<Offset, char>;
using Layout = std::array<Pattern, 5>;

constexpr Offset pairwise_mult(Offset l, Offset r) {
  auto [l1, l2] = l;
  auto [r1, r2] = r;
  return Offset{l1 * r1, l2 * r2};
}

int main() {
  Matrix matrix(std::cin);
  std::println("{}", search(matrix, part1));
  std::println("{}", search(matrix, part2));
}

uint32_t
search(Matrix const &matrix,
       std::function<int(Matrix const &, size_t row, size_t col)> search_impl) {
  int found = 0;
  for (size_t row{0}; row < matrix.rows(); row++) {
    for (size_t col{0}; col < matrix.cols(); col++) {
      found += search_impl(matrix, row, col);
    }
  }
  return found;
}

uint32_t part1(Matrix const &matrix, size_t start_row, size_t start_col) {
  // clang-format off
  constexpr std::array PT1_MOVE = std::array{
    std::tuple{-1, -1}, std::tuple{-1, 0}, std::tuple{-1, 1},
    std::tuple{ 0, -1},                    std::tuple{ 0, 1},
    std::tuple{ 1, -1}, std::tuple{ 1, 0}, std::tuple{ 1, 1}
  };
  // clang-format on

  constexpr std::string_view PT1_NEEDLE{"XMAS"};

  uint32_t count = 0;
  for (auto const &[row_move, col_move] : PT1_MOVE) {
    ptrdiff_t current_row{static_cast<ptrdiff_t>(start_row)};
    ptrdiff_t current_col{static_cast<ptrdiff_t>(start_col)};
    auto search_iter{PT1_NEEDLE.cbegin()};
    while (search_iter != PT1_NEEDLE.cend() &&
           matrix.in_bounds(current_row, current_col) &&
           matrix.at(current_row, current_col) == *search_iter) {
      search_iter++;
      current_row += row_move;
      current_col += col_move;
    }
    // We managed to cross the entire search string
    if (search_iter == PT1_NEEDLE.cend()) {
      count++;
    }
  }
  return count;
}

// There are 4 possible patterns of the for X-MAS that we can compute ahead of
// time, Basically, a rotation of
// m  m
//  a
// s  s
// around the a
// Build this now
consteval std::array<Layout, 4> make_search_pattern() {
  std::array<Layout, 4> search_patterns;
  // clang-format off
  search_patterns[0] = Layout {
    Pattern{Offset{-1, -1}, 'M'},                           Pattern{Offset{-1, 1}, 'M'},
                                 Pattern{Offset{0, 0}, 'A'},
    Pattern{Offset{ 1, -1}, 'S'},                           Pattern{Offset{ 1, 1}, 'S'}
  };

  // Can multiply the pattern by this to achieve clockwise rotation
  std::array clockwise_multipliers  ={
    Offset{1, -1},             Offset{-1, 1},
                  Offset{1, 1},
    Offset{-1, 1},             Offset{1, -1}
  };
  // clang-format on

  // Rotate the above pattern clockwise
  for (uint i{1}; i < search_patterns.size(); i++) {
    auto const &prev = search_patterns[i - 1];
    auto &next = search_patterns[i];

    for (uint p{0}; p < prev.size(); p++) {
      next[p] = std::make_tuple(
          pairwise_mult(std::get<0>(prev[p]), clockwise_multipliers[p]),
          std::get<1>(prev[p]));
    }
  }
  return search_patterns;
}

uint32_t part2(Matrix const &matrix, size_t start_row, size_t start_col) {
  constexpr std::array<Layout, 4> search_pattern = make_search_pattern();

  for (auto &layout : search_pattern) {
    bool valid = true;
    for (auto &[offset, ch] : layout) {
      auto [row_diff, col_diff] = offset;
      ptrdiff_t row = start_row + row_diff;
      ptrdiff_t col = start_col + col_diff;
      if (!matrix.in_bounds(row, col) || matrix.at(row, col) != ch) {
        valid = false;
        break;
      }
    }
    if (valid) {
      return 1;
    }
  }
  return 0;
}
