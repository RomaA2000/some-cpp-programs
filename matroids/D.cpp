//
// Created by Роман Агеев on 02.01.2020.
//
#include <cstdint>
#include <unordered_set>
#include <iostream>
#include <vector>

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

  mask & remove(uint32_t i) {
    auto m = moved(i);
    if (m & _inf) {
      _inf ^= m;
    }
    return *this;
  }

  uint32_t size() const {
    uint32_t size = 0;
    for (size_t i = 0; i < 16; ++i) {
      size += get(i);
    }
    return size;
  }

  mask & set(uint32_t i) {
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

int main() {

  freopen("check.in", "r", stdin);
  freopen("check.out", "w", stdout);

  size_t n = 0, m = 0;
  std::unordered_set<mask> good;
  std::cin >> n >> m;
  uint16_t num = 0;
  size_t a = 0;
  uint16_t max_size = 0;
  for (size_t i = 0; i < m; ++i) {
    mask now{};
    std::cin >> num;
    max_size = std::max(max_size, num);
    for (size_t j = 0; j < num; ++j) {
      std::cin >> a;
      now.set(--a);
    }
    good.insert(now);
  }
  std::unordered_set<mask> now;
  for (auto i: good) {
    if (i.size() == max_size) {
      now.insert(i);
    }
  }
  bool ans = true;
  while (ans) {
    std::unordered_set<mask> new_set;
    for (auto i: now) {
      ans &= (good.count(i) > 0);
    }
    for (auto i: now) {
      for(uint16_t j = 0; j < 16; ++j) {
        auto i_new = i;
        if (i != i_new.remove(j)) {
          new_set.insert(i_new);
        }
      }
    }
    std::swap(new_set, now);
    if (now.size() <= 1) {
      break;
    }
  }
  ans &= (good.count(mask()) > 0);
  for (auto const & i : good) {
    for (auto const & j : good) {
      if (i.size() > j.size()) {
        bool flag = false;
        for (auto k : i.get_all()) {
          if (!j.get(k)) {
            auto j_t = j;
            if (good.count(j_t.set(k)) > 0) {
              flag = true;
              break;
            }
          }
        }
        ans &= flag;
      }
    }
  }
  if (ans) {
    std:: cout << "YES" << std::endl;
  } else {
    std:: cout << "NO" << std::endl;
  }
}