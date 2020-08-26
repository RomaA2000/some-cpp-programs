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

  static std::vector<size_t> get(std::string const &s, std::string const &m, char c = '#') {
    std::string inf = s + c + m;
    auto z = z_func(inf);
    std::vector<size_t> ans;
    ans.reserve(m.size());
    std::back_insert_iterator<std::vector<size_t>> in(ans);
    for (size_t i = s.size() + 1; i < inf.size(); ++i) {
      in = z[i];
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
  std::string s, p;
  std::cin >> s;
  std::cin >> p;
  auto ans = substring_founder::get(s, p);
  std::reverse(s.begin(), s.end());
  std::reverse(p.begin(), p.end());
  auto ans_r = substring_founder::get(s, p);
  std::vector<size_t> out;
  for (size_t i = 0; i < ans.size(); ++i) {
    if (ans[i] == s.size() ||
    (ans.size() >= i + s.size() &&
    ans[i] + ans_r[ans.size() - i - s.size()] + 1 == s.size())) {
      out.push_back(i);
    }
  }
  std::cout << out.size() << std::endl;
  for (auto i : out) {
    std::cout << i + 1 << " ";
  }
  return 0;
}
