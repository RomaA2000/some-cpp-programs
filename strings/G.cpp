//
// Created by Роман Агеев on 23.02.2020.
//
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

class Axo {
  class node;
  typedef node *node_ptr;

  class node {
   public:
    std::unordered_map<char, node_ptr> children;
    std::vector<size_t> str_nums;
    node_ptr suff{};
    node_ptr composed_suff{};
    node_ptr parent{};
    char now_char{};
    node() = default;
    explicit node(node_ptr parent, char c)
        : children(), str_nums(), suff(), composed_suff(), parent(parent), now_char(c) {}
    node(node const &) = default;
    node &operator=(node const &) = default;
    node(node &&) = default;
    node &operator=(node &&) = default;
    ~node() {
      for (auto i : children) {
        i.second->~node();
      }
    }
  };

  node_ptr root = nullptr;
  size_t size = 0;
  std::unordered_set<node *> good;

  static void add_string(node_ptr now_node, std::string const &string, size_t num, size_t idx = 0) {
    if (string.length() == idx) {
      now_node->str_nums.push_back(num);
    } else {
      if (now_node->children[string[idx]] == nullptr) {
        now_node->children[string[idx]] = new node(now_node, string[idx]);
      }
      add_string(now_node->children[string[idx]], string, num, idx + 1);
    }
  }

  void make_suff_links() {
    std::queue<node_ptr> queue;
    root->suff = root;
    root->composed_suff = root;
    queue.push(root);
    while (!queue.empty()) {
      node_ptr now = queue.front();
      queue.pop();
      for (auto v : now->children) {
        queue.push(v.second);
      }
      if (now == root || now->parent == root) {
        now->suff = root;
        now->composed_suff = root;
      } else {
        node_ptr now_suff = now->parent->suff;
        while (now_suff != root && now_suff->children.count(now->now_char) == 0) {
          now_suff = now_suff->suff;
        }
        if (now_suff->children.count(now->now_char) != 0) {
          now_suff = now_suff->children[now->now_char];
        }
        now->suff = now_suff;
        now->composed_suff = now_suff;
      }
    }
  }

  node_ptr make_composed(node_ptr now_node, std::vector<bool> &ans) {
    if (now_node != root && good.count(now_node) == 0) {
      good.insert(now_node);
      now_node->composed_suff = make_composed(now_node->composed_suff, ans);
      if (now_node->str_nums.empty()) {
        return now_node->composed_suff;
      } else {
        for (auto e : now_node->str_nums) {
          ans[e] = true;
        }
        return now_node;
      }
    } else {
      return root;
    }
  }

 public:
  Axo() : root(new node(nullptr, '#')) {}

  ~Axo() {
    if (root != nullptr) {
      root->~node();
    }
  }

  void add_strings(std::vector<std::string> const &in, std::vector<size_t> const &num) {
    size += in.size();
    for (size_t i = 0; i < in.size(); ++i) {
      add_string(root, in[i], num[i]);
    }
    make_suff_links();
  }

  std::vector<bool> get(std::string const &in) {
    std::vector<bool> ans(size, false);
    node_ptr now = root;
    for (auto i : in) {
      while (now != root && now->children.count(i) == 0) {
        now = now->suff;
      }
      if (now->children.count(i) != 0) {
        now = now->children[i];
      }
      make_composed(now, ans);
    }
    return ans;
  }
};

int main() {
  size_t n = 0;
  std::string txt;
  std::cin >> txt;
  std::vector<std::string> input;
  std::vector<size_t> num;
  std::cin >> n;
  input.reserve(n);
  num.reserve(n);
  std::string in;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> in;
    input.push_back(std::move(in));
    num.push_back(i);
  }
  Axo solver;
  solver.add_strings(input, num);
  std::vector<bool> answer = solver.get(txt);
  for (bool i : answer) {
    if (i) {
      std::cout << "Yes" << '\n';
    } else {
      std::cout << "No" << '\n';
    }
  }
  return 0;
}
