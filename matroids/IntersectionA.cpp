//
// Created by Роман Агеев on 03.01.2020.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <queue>
#include <random>

class DSU {
  mutable std::vector<size_t> p;
  mutable std::vector<uint64_t> rang;

 public:
  explicit DSU(size_t n) : p(n), rang(n, 0) {
    for (size_t i = 0; i < n; ++i) {
      p[i] = i;
    }
  }

  size_t get(size_t v) const {
    if (p[v] == v) {
      return v;
    } else {
      p[v] = get(p[v]);
      return p[v];
    }
  }

  void union_vertex(size_t v, size_t u) {
    size_t pv = get(v);
    size_t pu = get(u);
    if (rang[pv] < rang[pu]) {
      p[pv] = pu;
    } else {
      p[pu] = pv;
      if (rang[pv] == rang[pu]) {
        ++rang[pv];
      }
    }
  }

  bool test(size_t v, size_t u) const {
    return get(v) == get(u);
  }
};

struct Edge {
  size_t begin;
  size_t end;
  uint64_t color;

  Edge(size_t begin, size_t end, uint64_t color) :
      begin(begin), end(end), color(color) {}

  Edge(Edge const &) = default;
  Edge(Edge &&) = default;
  ~Edge() = default;
  Edge& operator=(Edge const &) = default;
  Edge& operator=(Edge &&) = default;
};

class Rainbow {
  std::vector<Edge> edge_list;
  size_t n;
  uint64_t max_color;

  template<typename T>
  using matrix = std::vector<std::vector<T>>;

  template<typename T>
  using parts = std::pair<std::vector<T>, std::vector<T>>;

  void set(size_t i, std::vector<bool> & color_mask, DSU & dsu) const {
    color_mask[edge_list[i].color] = true;
    dsu.union_vertex(edge_list[i].begin, edge_list[i].end);
  }

  void set_X(std::vector<size_t> & f, size_t f_n, std::vector<size_t> & s, size_t s_n, std::vector<bool> & color_mask, DSU & dsu) const {
    if (!color_mask[edge_list[f_n].color]) {
      f.push_back(s_n);
    }
    if (!dsu.test(edge_list[f_n].begin,edge_list[f_n].end)) {
      s.push_back(f_n);
    }
  }

  matrix<size_t> get_graph(std::vector<bool> const & mask) const {
    matrix<size_t> replacement_graph(edge_list.size());
    std::vector<bool> color_mask;
    for (size_t i = 0; i < edge_list.size(); ++i) {
      if (mask[i]) {
        color_mask.assign(max_color, false);
        DSU dsu(n);
        for (size_t j = 0; j < edge_list.size(); ++j) {
          if (mask[j] && i != j) {
            set(j, color_mask, dsu);
          }
        }
        for (size_t j = 0; j < edge_list.size(); ++j) {
          if (!mask[j]) {
            set_X(replacement_graph[j], j, replacement_graph[i], i, color_mask, dsu);
          }
        }
      }
    }
    return replacement_graph;
  }

  parts<size_t> get_cut(std::vector<bool> const & mask) const {
    parts<size_t> X;
    std::vector<bool> color_mask(max_color, false);
    DSU dsu(n);
    for (size_t i = 0; i < edge_list.size(); ++i) {
      if (mask[i]) {
        set(i, color_mask, dsu);
      }
    }
    for (size_t i = 0; i < edge_list.size(); ++i) {
      if (!mask[i]) {
        set_X(X.first, i, X.second, i, color_mask, dsu);
      }
    }
    return X;
  }

  std::pair<size_t , std::vector<size_t>> get_path(matrix<size_t> const & replacement_graph, parts<size_t> const & cut) const {
    std::vector<uint64_t> length(edge_list.size(), std::numeric_limits<uint64_t>::max());
    std::queue<size_t> queue;
    std::vector<size_t> last(edge_list.size(), std::numeric_limits<size_t>::max());
    for (auto i : cut.second) {
      queue.push(i);
      length[i] = 0;
    }
    while(!queue.empty()) {
      size_t now = queue.front();
      queue.pop();
      for (auto i : replacement_graph[now]) {
        if ((length[now] != std::numeric_limits<uint64_t>::max()) && (length[now] + 1 < length[i])) {
          queue.push(i);
          length[i] = length[now] + 1;
          last[i] = now;
        }
      }
    }
    std::pair<uint64_t, size_t> min(std::numeric_limits<uint64_t>::max(), std::numeric_limits<size_t>::max());
    for (auto i : cut.first) {
      if (min.first > length[i]) {
        min.first = length[i];
        min.second = i;
      }
    }
    return {min.second, last};
  }

  public:
  Rainbow(std::vector<Edge> edge_list, size_t n, uint64_t max_color) :
  edge_list(std::move(edge_list)), n(n), max_color(max_color){}

  std::vector<size_t> get_forest() {
    std::vector<bool> mask(edge_list.size(), false);
    bool is_ans = false;
    std::vector<bool> color_mask;
    while(!is_ans) {
      auto const & const_mask = mask;
      auto graph_getter = std::async([this, const_mask] () -> auto { return get_graph(const_mask);});
      auto cut_getter = std::async([this, const_mask] () -> auto { return get_cut(const_mask);});
      auto [v, path] = get_path(graph_getter.get(), cut_getter.get());
//    auto [v, path] = get_path(get_graph(mask), get_cut(mask));
      bool flag = (v == std::numeric_limits<size_t>::max());
      while (v != std::numeric_limits<size_t>::max()) {
        mask[v] = !mask[v];
        v = path[v];
      }
      is_ans |= flag;
    }
    std::vector<size_t> ans;
    for(size_t i = 0; i < mask.size(); ++i) {
      if (mask[i]) {
        ans.push_back(i);
      }
    }
    return ans;
  }
};

int main() {
  freopen("rainbow.in", "r", stdin);
  freopen("rainbow.out", "w", stdout);
  size_t n = 0, m = 0;
  std::cin >> n >> m;
  size_t a = 0, b = 0;
  uint64_t c = 0, max_color = 0;
  std::vector<Edge> edge_list;
  edge_list.reserve(m);
  for (size_t i = 0; i < m; ++i) {
    std::cin >> a >> b >> c;
    max_color = std::max(max_color, c);
    --a;
    --b;
    --c;
    edge_list.emplace_back(a, b, c);
  }
  Rainbow rainbow(std::move(edge_list), n, max_color);
  auto ans = rainbow.get_forest();
  std::cout << ans.size() << std::endl;
  for (auto i : ans) {
    std::cout << i + 1 << ' ';
  }
  return 0;
}
//int main() {
//  std::random_device rd;
//  std::mt19937 mersenne(rd());
//  uint64_t const num = 100;
//  for (size_t j = 0; j < num; ++j) {
//    uint64_t const n = mersenne() % 100 + 1;
//    uint64_t const m = mersenne() % 5000 + 1;
//    uint64_t const s = mersenne() % 100 + 1;
//    std::vector<Edge> edge_list;
//    uint64_t max_color = 0;
//    edge_list.reserve(m);
//    for (size_t i = 0; i < m; ++i) {
//      uint64_t c = mersenne() % s + 1;
//      max_color = std::max(max_color, c);
//      edge_list.emplace_back(mersenne() % n, mersenne() % n, --c);
//    }
//    Rainbow rainbow(std::move(edge_list), n, max_color);
//    auto ans = rainbow.get_forest();
//  }
//}