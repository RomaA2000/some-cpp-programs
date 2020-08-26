//
// Created by Роман Агеев on 04.04.2020.
//
#include <map>
#include <unordered_map>
#include <iostream>
#include <array>
#include <vector>

class suff_tree {
  struct node {
    int64_t left = 0;
    int64_t right = 0;
    int64_t parent = 0;
    int64_t link = 0;
    std::unordered_map<char, int64_t> child_list;

    node() = default;

    node(int64_t left, int64_t right, int64_t parent = -1)
        : left(left), right(right), parent(parent), link(-1) {}

    int64_t size() {
      return right - left;
    }

    int64_t &getter(char c) {
      if (!child_list.count(c)) {
        return child_list[c] = -1;
      } else {
        return child_list[c];
      }
    }
  };

  std::string s;

  std::array<node, 1000000> t;
  int64_t sz;
  std::vector<int64_t> ans;

  struct state {
    int64_t v, pos;
    state(int64_t v, int64_t pos) : v(v), pos(pos) {}
  };

  int64_t leafs_num = 0;
  state now{0, 0};

  state next(state st, int64_t l, int64_t r) {
    while (l < r)
      if (st.pos == t[static_cast<size_t>(st.v)].size()) {
        st = state(
            t[static_cast<size_t>(st.v)]
                .getter(s[static_cast<size_t>(l)]), 0);
        if (st.v == -1) {
          return st;
        }
      } else {
        if (s[
            static_cast<size_t>(
                t[static_cast<size_t>(st.v)].left
                    + st.pos)
        ] != s[static_cast<size_t>(l)]) {
          return state(-1, -1);
        }
        if (r + st.pos < t[static_cast<size_t>(st.v)].size() + l) {
          return state(st.v, st.pos + r - l);
        }
        l += t[static_cast<size_t>(st.v)].size();
        l -= st.pos;
        st.pos = t[static_cast<size_t>(st.v)].size();
      }
    return st;
  }

  int64_t split(state st) {
    if (st.pos
        ==
            t[static_cast<size_t>(st.v)].size()) {
      return st.v;
    }
    if (st.pos == 0) {
      return t[static_cast<size_t>(st.v)].parent;
    }
    node v = t[static_cast<size_t>(st.v)];
    int64_t id = sz++;
    int64_t tmp = v.left + st.pos;
    t[static_cast<size_t>(id)] =
        node(v.left, tmp, v.parent);
    t[static_cast<size_t>(v.parent)].getter(s[static_cast<size_t>(v.left)]) = id;
    t[static_cast<size_t>(id)].getter(
        s[static_cast<size_t>(v.left + st.pos)])
        = st.v;
    t[static_cast<size_t>(st.v)].parent = id;
    t[static_cast<size_t>(st.v)].left += st.pos;
    return id;
  }

  int64_t get_link(int64_t v) {
    if (t[static_cast<size_t>(v)].link != -1) {
      return t[static_cast<size_t>(v)].link;
    }
    if (t[static_cast<size_t>(v)].parent == -1) {
      return 0;
    }
    int64_t to = get_link(t[static_cast<size_t>(v)].parent);
    t[static_cast<size_t>(v)].link
        = split(next(state(to,
                           t[static_cast<size_t>(to)].size()),
                     t[static_cast<size_t>(v)].left
                         + (t[static_cast<size_t>(v)].parent == 0),
                     t[static_cast<size_t>(v)].right));
    return t[static_cast<size_t>(v)].link;
  }

 public:
  void print() {
    int64_t ans_i = 0;
    for (auto i : ans) {
      ans_i += i;
      std::cout << ans_i << std::endl;
    }
  }

  explicit suff_tree(std::string st) : s(std::move(st)), sz(1) {
    for (size_t i = 0; i < s.size(); ++i) {
      while (true) {
        state new_ptr = next(now, i, i + 1);
        if (new_ptr.v != -1) {
          now = new_ptr;
          break;
        }
        int64_t mid = split(now);
        int64_t leaf = sz++;
        t[static_cast<size_t>(leaf)] = node(i, s.size(), mid);
        ++leafs_num;
        t[static_cast<size_t>(mid)].getter(s[i]) = leaf;
        now.v = get_link(mid);
        now.pos = t[static_cast<size_t>(now.v)].size();
        if (!mid) {
          break;
        }
      }
      ans.push_back(leafs_num);
    }
  }
};

int main() {
  std::string s;
  std::cin >> s;
  suff_tree tree(s);
  tree.print();
}
