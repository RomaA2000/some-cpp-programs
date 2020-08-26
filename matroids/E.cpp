//
// Created by Роман Агеев on 02.01.2020.
//

#include <cstdint>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <future>

class mask {
  uint32_t _inf;
  static uint32_t moved(uint32_t i) {
    return 1u << i;
  }
 public:
  explicit mask(uint32_t i) : _inf(i) {}
  mask() = default;
  mask(mask const &) = default;
  mask(mask &&) = default;
  ~mask() = default;
  mask &operator=(mask const &) = default;
  mask &operator=(mask &&) = default;

  bool operator==(mask const &a) const {
    return _inf == a._inf;
  }

  bool operator!=(mask const &a) const {
    return _inf != a._inf;
  }

  uint32_t get_inf() const {
    return _inf;
  }

  bool get(uint32_t i) const {
    return moved(i) & _inf;
  }

  mask &remove(uint32_t i) {
    auto m = moved(i);
    if (m & _inf) {
      _inf ^= m;
    }
    return *this;
  }

  uint32_t size() const {
    uint32_t size = 0;
    for (uint32_t i = 0; i < 16; ++i) {
      size += get(i);
    }
    return size;
  }

  mask &set(uint32_t i) {
    _inf |= moved(i);
    return *this;
  }

  std::vector<uint32_t> get_all() const {
    std::vector<uint32_t> ans{};
    for (uint32_t i = 0; i < 16; ++i) {
      if (get(i)) {
        ans.push_back(i);
      }
    }
    return ans;
  }
};

namespace std {
template<>
class hash<mask> {
 public:
  size_t operator()(const mask &m) const noexcept {
    return hash<uint32_t>()((uint32_t) m.get_inf());
  }
};
}

template<typename T>
void sort(T &&f, T &&s) {
  std::sort(f, s);
}

int main() {

  freopen("cycles.in", "r", stdin);
  freopen("cycles.out", "w", stdout);

  size_t n = 0, m = 0;
  std::unordered_set<mask> not_good;
  std::cin >> n >> m;
  std::vector<std::pair<uint64_t, size_t>> weights;
  weights.reserve(n);
  uint64_t w = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> w;
    weights.emplace_back(w, i);
  }
  auto get_sorted =
      std::async(std::launch::deferred, sort<decltype(weights.rbegin())>, weights.rbegin(), weights.rend());
  uint32_t num = 0;
  size_t a = 0;
  std::queue<mask> queue;
  for (size_t i = 0; i < m; ++i) {
    mask now{};
    std::cin >> num;
    for (size_t j = 0; j < num; ++j) {
      std::cin >> a;
      now.set(--a);
    }
    not_good.insert(now);
    queue.push(now);
  }
  while (!queue.empty()) {
    auto i = queue.front();
    queue.pop();
    for (uint32_t j = 0; j < n; ++j) {
      auto i_new = i;
      i_new.set(j);
      if (i != i_new && (not_good.count(i_new) == 0)) {
        queue.push(i_new);
        not_good.insert(i_new);
      }
    }
  }
  get_sorted.get();
  uint64_t ans = 0;
  mask now{};
  for (auto const &i: weights) {
    auto tmp = now;
    if (not_good.count(tmp.set(i.second)) == 0) {
      ans += i.first;
      now.set(i.second);
    }
  }
  std::cout << ans << std::endl;
}