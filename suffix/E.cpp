//
// Created by Роман Агеев on 03.04.2020.
//
#include <vector>
#include <iostream>
#include <functional>

size_t alphabet_size;

std::vector<int64_t> sort_cyclic_shifts(std::vector<size_t> &s) {
  std::vector<int64_t> p(s.size()), c(s.size()), cnt(std::max(alphabet_size, s.size()));
  std::function<void(std::function<size_t(size_t)> const &)>
      _cnt_1 = [&cnt, &s](std::function<size_t(size_t)> const &f) {
    for (size_t i = 0; i < s.size(); ++i) {
      ++cnt[f(i)];
    }
  };
  std::function<void(size_t)> _cnt_2 = [&cnt](size_t n) {
    for (size_t i = 1u; i < n; ++i) {
      cnt[i] += cnt[i - 1];
    }
  };
  std::function<void(std::function<size_t(size_t)> const &,
                     std::function<int(size_t)> const &)>
      _cnt_3 = [&cnt, &s, &p](std::function<size_t(size_t)> const &g_f,
                              std::function<int(size_t)> const &s_f) {
    for (size_t i = s.size(); i--;) {
      --cnt[g_f(i)];
      p[static_cast<size_t>(cnt[g_f(i)])] = s_f(i);
    }
  };
  auto string_it = [&s](size_t i) { return static_cast<size_t>(s[i]); };
  _cnt_1(string_it);
  _cnt_2(alphabet_size);
  auto identity = [](auto i) { return i; };
  _cnt_3(string_it, identity);
  size_t size = 1u;
  c[static_cast<size_t>(p[0])] = 0;
  for (size_t i = 1; i < s.size(); ++i) {
    if (s[static_cast<size_t>(p[i])] != s[static_cast<size_t>(p[i - 1])]) {
      ++size;
    }
    c[static_cast<size_t>(p[i])] = size - 1;
  }
  std::vector<int64_t> pn(s.size()), cn(s.size());
  for (size_t h = 0; (1u << h) < s.size(); ++h) {
    for (size_t i = 0; i < s.size(); ++i) {
      pn[i] = p[i] - (1u << h);
      if (pn[i] < 0) {
        pn[i] += s.size();
      }
    }
    fill(cnt.begin(), cnt.begin() + size, 0);
    auto c_p_getter = [&c, &pn](size_t i) {
      return static_cast<size_t>
      (c[static_cast<size_t>(pn[i])]);
    };
    _cnt_1(c_p_getter);
    _cnt_2(size);
    _cnt_3(c_p_getter,
           [&pn](size_t i) { return pn[i]; });
    cn[static_cast<size_t>(p[0])] = 0;
    size = 1;
    for (size_t i = 1; i < s.size(); ++i) {
      auto s_getter = [&c, &p, &h, &s](size_t i) {
        return static_cast<size_t>(
            c[static_cast<size_t>((p[i] + (1u << h)) % s.size())]);
      };
      std::pair<size_t, size_t> cur =
          {c[static_cast<size_t>(p[i])], s_getter(i)};
      std::pair<size_t, size_t> prev =
          {c[static_cast<size_t>(p[i - 1])], s_getter(i - 1)};
      if (cur != prev) {
        ++size;
      }
      cn[static_cast<size_t>(p[i])] = size - 1;
    }
    c.swap(cn);
  }
  return p;
}

std::vector<size_t> lcp(std::vector<size_t> const &s, std::vector<size_t> const &suff_arr) {
  std::vector<size_t> pos(s.size(), 0);
  for (size_t i = 0; i < s.size(); ++i) {
    pos[suff_arr[i]] = i;
  }
  size_t l = 0;
  std::vector<size_t> lcp(s.size(), 0);
  for (size_t i = 0; i < s.size(); ++i) {
    if (pos[i] + 1 == s.size()) {
      l = 0;
    } else {
      size_t j = suff_arr[pos[i] + 1];
      while (j + l < s.size() && i + l < s.size() && s[i + l] == s[j + l]) {
        ++l;
      }
      lcp[pos[i]] = l;
      if (l != 0) {
        --l;
      }
    }
  }
  return lcp;
}

struct data {
  uint64_t num;
  uint64_t l;
  uint64_t idx;
  data() = default;
  data(uint64_t num, uint64_t l, uint64_t idx) : num(num), l(l), idx(idx) {}
};

void make_r(std::vector<size_t> const &s,
            std::vector<size_t> const &suff_array,
            std::vector<size_t> const &lcp) {
  std::vector<data> stack;
  uint64_t idx = 0;
  uint64_t len = s.size() - 1;
  uint64_t num = 1;
  for (size_t i = 1; i < s.size(); ++i) {
    uint64_t now_num = 1;
    while (!stack.empty() && lcp[i] <= stack.back().l) {
      data temp = stack.back();
      stack.pop_back();
      now_num += temp.num;
      if (now_num * temp.l > len * num) {
        num = now_num;
        len = temp.l;
        idx = temp.idx;
      }
    }
    if (stack.empty() || lcp[i] > stack.back().l) {
      stack.emplace_back(now_num, lcp[i], i);
    }
  }
  std::cout << len * num << "\n";
  std::cout << len << "\n";
  if (idx != 0) {
    for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
      std::cout << s[suff_array[static_cast<size_t>(idx)] + i] << " ";
    }
  } else {
    for (size_t i = 0; i + 1 < s.size(); ++i) {
      std::cout << s[i] << " ";
    }
  }
  std::cout << std::endl;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  alphabet_size = m + 1;
  std::vector<size_t> s;
  s.reserve(n);
  size_t a;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> a;
    s.push_back(a);
  }
  ++n;
  s.push_back(0);
  auto v_tmp = sort_cyclic_shifts(s);
  std::vector<size_t> new_ans;
  new_ans.reserve(s.size());
  for (auto i : v_tmp) {
    new_ans.push_back(static_cast<size_t>(i));
  }
  auto v = lcp(s, new_ans);
  make_r(s, new_ans, v);
}

