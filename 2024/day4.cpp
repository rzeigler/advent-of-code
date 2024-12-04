#include <array>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

// TODO: Learn concepts and how to write iterators, then revisit this for
// fanciness
class CharMatrix {
public:
  explicit CharMatrix(std::istream &is) {
    for (std::string line; std::getline(is, line) && !line.empty();) {
      m_data.push_back(line);
    }
    if (m_data.empty()) {
      throw std::domain_error("empty input");
    }
    // TODO: Lower priority, but how do we detect io error on input stream?
    auto iter = m_data.cbegin();
    size_t len = iter->length();
    iter++;

    if (len == 0) {
      throw std::domain_error("empty leading string");
    }

    for (; iter != m_data.cend(); iter++) {
      if (iter->length() != len) {
        throw std::domain_error("not all inputs are the same length");
      }
    }
  }

  void print(std::ostream &os) const {
    for (auto line : m_data) {
      os << line << '\n';
    }
  }

  inline char at(size_t x, size_t y) const { return m_data[y][x]; }

  inline int rows() const { return m_data.size(); }

  inline int columns() const { return m_data[0].size(); }

private:
  // Dimensions must be at least 1x1 or error
  std::vector<std::string> m_data;
};

template <size_t N> class Search {
public:
  consteval Search(std::string_view needle,
                   std::array<std::pair<long, long>, N> offsets)
      : m_needle(needle), m_offsets(offsets) {}

  int search(CharMatrix const &mat, long const start_x,
             long const start_y) const {
    int found = 0;
    for (auto motion : m_offsets) {
      bool success{true};
      // TODO: Make nice with some kind of iterator

      long end_x =
          start_x + static_cast<long>(m_needle.length()) * std::get<0>(motion);
      long end_y =
          start_y + static_cast<long>(m_needle.length()) * std::get<1>(motion);
      if (0 <= end_x && end_x < mat.columns() && 0 <= end_y &&
          end_y < mat.rows()) {
        long x, y;
        char const *iter;
        for (x = start_x, y = start_y, iter = m_needle.cbegin();
             iter < m_needle.cend(); // We don't need to check in bounds since
                                     // we verified space above
             x += std::get<0>(motion),
            y += std::get<1>(motion), iter++) {
          if (mat.at(x, y) != *iter) {
            success = false;
            break;
          }
        }
      } else {
        success = false;
      }
      if (success) {
        std::cout << "Found at " << start_x << "," << start_y << " moving "
                  << std::get<0>(motion) << "," << std::get<1>(motion)
                  << std::endl;
        found++;
      }
    }

    return found;
  }

private:
  const std::string_view m_needle;
  const std::array<std::pair<long, long>, N> m_offsets;
};

constexpr auto XMAS = "XMAS";

int part1(CharMatrix const &mat) {
  constexpr std::array<std::pair<long, long>, 4> dirs{
      {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
  constexpr Search search(XMAS, dirs);

  int count = 0;
  for (long y = 0; y < mat.rows(); y++) {
    for (long x = 0; x < mat.columns(); x++) {
      count += search.search(mat, x, y);
    }
  }
  return count;
}

int main() {
  CharMatrix mat(std::cin);
  std::println("{}", part1(mat));
}