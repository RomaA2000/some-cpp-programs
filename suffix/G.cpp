//
// Created by Роман Агеев on 03.04.2020.
//
#include <vector>
#include <string>
#include <iostream>
#include <functional>

const size_t alphabet_size = 1024u;

std::vector<int64_t> sort_cyclic_shifts(std::string const &s) {
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

std::vector<size_t> suffix_array_construction(std::string s) {
  s += "$";
  std::vector<int64_t> sorted_shifts = sort_cyclic_shifts(s);
  sorted_shifts.erase(sorted_shifts.begin());
  std::vector<size_t> real_ans;
  real_ans.reserve(sorted_shifts.size());
  for (int64_t i : sorted_shifts) {
    real_ans.push_back(static_cast<size_t>(i));
  }
  return real_ans;
}

std::vector<size_t> lcp(std::string const &s, std::vector<size_t> const &suff_arr) {
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
  lcp.pop_back();
  return lcp;
}

int main() {
  std::string f;
  std::string s;
  std::getline(std::cin, f);
  std::getline(std::cin, s);
  std::string in = f + "|" + s;
  auto suff_arr = suffix_array_construction(in);
  auto lcp_arr = lcp(in, suff_arr);
  std::string ans;
  for (size_t i = 0; i < lcp_arr.size(); ++i) {
    if (ans.size() < lcp_arr[i] &&
        !((suff_arr[i] < f.size()) ^ (suff_arr[i + 1] > f.size()))) {
      ans = in.substr(suff_arr[i], lcp_arr[i]);
    }
  }
}