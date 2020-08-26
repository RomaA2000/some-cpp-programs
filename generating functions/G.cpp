
#include <iostream>
#include <array>

template<size_t size>
class Solution {
  std::string const s;
  size_t idx;
  std::array<std::array<uint64_t, size>, size> C;

  static uint64_t facor(uint64_t from, uint64_t to) {
    uint64_t ans = 1;
    for (uint64_t i = from + 1; i < to + 1; ++i) {
      ans *= i;
    }
    return ans;
  }

  void B(std::array<uint64_t, size> &result) {
    result[1] = 1;
  }

  void L(std::array<uint64_t, size> &result) {
    std::array<uint64_t, size> arg1{0};
    idx += 2;
    arg1 = get_result();
    result[0] = 1;
    for (size_t i = 1; i < size; ++i) {
      for (size_t j = 1; j <= i; ++j) {
        result[i] += arg1[j] * result[i - j];
      }
    }
  }

  void P(std::array<uint64_t, size> &result) {
    std::array<uint64_t, size> arg1{0}, arg2{0};
    idx += 2;
    arg1 = get_result();
    ++idx;
    arg2 = get_result();
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j <= i; ++j) {
        result[i] += arg1[j] * arg2[i - j];
      }
    }
  }

  void S(std::array<uint64_t, size> & result) {
    std::array<uint64_t, size> arg1{0};
    std::array<std::array<uint64_t, size>, size> m_set{std::array<uint64_t, size>{0}};
    idx += 2;
    arg1 = get_result();
    m_set[0].fill(1);
    result[0] = 1;
    for (size_t i = 1; i < size; ++i) {
      for (size_t j = 1; j < size; ++j) {
        for (size_t k = 0; k < (i / j) + 1; ++k) {
          uint64_t m = arg1[j] + k >= 1 ? arg1[j] + k - 1 : 0;
          m_set[i][j] += (facor(m - k, m) / facor(0, k)) * m_set[i - k * j][j - 1];
        }
      }
      result[i] = m_set[i][i];
    }
  }

  std::array<uint64_t, size> get_result() {
    std::array<uint64_t, size> result{0};
    switch (s[idx]) {
      case 'B':
        B(result);
        break;
      case 'L':
        L(result);
        break;
      case 'P':
        P(result);
        break;
      case 'S':
        S(result);
        break;
      default:
        break;
    }
    ++idx;
    return result;
  }

 public:
  Solution(std::string s) : s(std::move(s)), idx(0) {}

  std::array<uint64_t, size> start() {
    return get_result();
  }
};
int main() {
  std::string s;
  std::cin >> s;
  auto ans = Solution<7>(s).start();
  for (auto &i : ans) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}