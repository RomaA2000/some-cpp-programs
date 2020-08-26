#include <iostream>
#include <vector>
#include <algorithm>

class substring_founder {
 public:
  substring_founder() = default;
  substring_founder(substring_founder const &) = default;
  substring_founder &operator=(substring_founder const &) = default;
  substring_founder(substring_founder &&) = default;
  substring_founder &operator=(substring_founder &&) = default;

  static std::vector<size_t> substr_z_func(std::string const &s, size_t size) {
    std::vector<size_t> ans(size + 1, 0);
    size_t l = size;
    size_t r = size;
    for (size_t i = size; i--;) {
      if (i >= r) {
        ans[i] = std::min(ans[i + 1 - r], size + i - l);
      }
      while (i >= ans[i] && s[i - ans[i]] == s[size - ans[i]]) {
        ++ans[i];
      }
      if (i + 1 - ans[i] < r) {
        l = i;
        r = i + 1 - ans[i];
      }
    }
    return ans;
  }
};

int main() {
  std::string p;
  std::cin >> p;
  uint64_t ans = 0;
  for (size_t i = 0; i < p.size(); ++i) {
    auto z = substring_founder::substr_z_func(p, i);
    ans += i + 1 - *std::max_element(z.begin(), z.end());
    std::cout << ans << '\n';
  }
  return 0;
}
