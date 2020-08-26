//
// Created by Роман Агеев on 21.02.2020.
//
#include <iostream>
#include <vector>

class substring_founder {
 public:
  substring_founder() = default;
  substring_founder(substring_founder const &) = default;
  substring_founder &operator=(substring_founder const &) = default;
  substring_founder(substring_founder &&) = default;
  substring_founder &operator=(substring_founder &&) = default;

  static std::vector<size_t> p_func(std::string const &s) {
    std::vector<size_t> ans(s.size(), 0);
    size_t k = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        k = ans[i - 1];
        while ((k > 0) && s[i] != s[k]) {
          k = ans[k - 1];
        }
        if (s[i] == s[k]) {
          ++k;
        }
        ans[i] = k;
    }
    return ans;
  }
};

int main() {
  std::string p;
  std::cin >> p;
  auto ans = substring_founder::p_func(p);
  for (size_t i = 0; i < ans.size(); ++i) {
    std::cout << ans[i] << " ";
  }
  return 0;
}
