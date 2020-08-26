#include <iostream>
#include <vector>

class substring_founder {
 public:
  substring_founder() = default;
  substring_founder(substring_founder const &) = default;
  substring_founder &operator=(substring_founder const &) = default;
  substring_founder(substring_founder &&) = default;
  substring_founder &operator=(substring_founder &&) = default;

  static std::vector<size_t> get(std::string const &s, std::string const &m, char c = '#') {
    std::string inf = s + c + m;
    auto z = z_func(inf);
    std::vector<size_t> ans;
    auto start = s.size() + 1;
    for (size_t i = start; i < z.size(); ++i) {
      if (z[i] == s.size()) {
        ans.push_back(i - start);
      }
    }
    return ans;
  }

  static std::vector<size_t> z_func(std::string const &s) {
    std::vector<size_t> ans(s.size(), 0);
    size_t l = 0;
    size_t r = 0;
    for (size_t i = 1; i < s.size(); ++i) {
      if (i <= r) {
        ans[i] = std::min(ans[i - l], r - i + 1);
      }
      while (i + ans[i] < s.size() && s[ans[i]] == s[i + ans[i]]) {
        ++ans[i];
      }
      if (i + ans[i] - 1 > r) {
        l = i;
        r = i + ans[i] - 1;
      }
    }
    return ans;
  }
};

int main() {
  std::string p;
  std::cin >> p;
  auto ans = substring_founder::z_func(p);
  for (size_t i = 1; i < ans.size(); ++i) {
    std::cout << ans[i] << " ";
  }
  return 0;
}
