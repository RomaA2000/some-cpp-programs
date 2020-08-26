#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

int main() {
  freopen("schedule.in", "r", stdin);
  freopen("schedule.out", "w", stdout);
  size_t n = 0;
  std::cin >> n;
  uint64_t ans = 0;
  std::vector<std::pair<size_t, size_t>> input;
  std::set<size_t> time;
  std::pair<size_t, size_t> a;
  input.resize(n);
  for(size_t i = 0; i < n; ++i) {
    std::cin >> a.second >> a.first;
    input.push_back(a);
    time.insert(i + 1);
  }
  std::sort(input.rbegin(), input.rend());
  for (size_t i = 0; i < n; ++i) {
    auto j = time.upper_bound(input[i].second);
    if (j == time.begin()) {
      ans += input[i].first;
    } else {
      time.erase(--j);
    }
  }
  std::cout << ans << std::endl;
  return 0;
}
