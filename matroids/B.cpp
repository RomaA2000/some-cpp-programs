//
// Created by Роман Агеев on 02.01.2020.
//

#include <iostream>
#include <vector>
#include <algorithm>

class DSU {
  std::vector<size_t> p;
  std::vector<uint64_t> rang;

 public:
  explicit DSU(size_t n) : p(n), rang(n, 0) {
    for (size_t i = 0; i < n; ++i) {
      p[i] = i;
    }
  }

  size_t get(size_t v) {
    if (p[v] == v) {
      return v;
    } else {
      size_t ans = get(p[v]);
      p[v] = ans;
      return ans;
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
};

struct Edge {
  size_t begin;
  size_t end;
  size_t number;
  uint64_t weight;

  Edge(size_t begin, size_t end, size_t number, uint64_t weight) :
      begin(begin), end(end), number(number), weight(weight) {}

  Edge(Edge const &) = default;
  Edge(Edge &&) = default;
  ~Edge() = default;
  Edge& operator=(Edge const &) = default;

  bool operator<(Edge const &in) const {
    return weight < in.weight;
  }
};

class MST {

  std::vector<Edge> edges;
  size_t n = 0;
 public:
  MST() = default;

  void add(Edge e) {
    n = std::max(n, std::max(e.begin, e.end));
    edges.emplace_back(e);
  }

  std::vector<size_t> get_not_mst() {
    std::sort(edges.rbegin(), edges.rend());
    DSU dsu(n + 1);
    std::vector<size_t> ans;
    ans.reserve(edges.size());
    for (auto const & i : edges) {
      if (dsu.get(i.begin) == dsu.get(i.end)) {
        ans.push_back(i.number);
      } else {
        dsu.union_vertex(i.begin, i.end);
      }
    }
    return ans;
  }
};

int main() {

  freopen("destroy.in", "r", stdin);
  freopen("destroy.out", "w", stdout);

  size_t n = 0, m = 0;
  uint64_t s = 0;
  std::cin >> n >> m >> s;
  std::vector<uint64_t> edges;
  edges.reserve(m);
  MST mst;
  for (size_t i = 0; i < m; ++i) {
    size_t b = 0, e = 0;
    uint64_t w = 0;
    std::cin >> b >> e >> w;
    --b;
    --e;
    mst.add(Edge(b, e, i, w));
    edges.push_back(w);
  }
  auto not_mst = mst.get_not_mst();
  std::vector<size_t> ans;
  uint64_t sum = 0;
  for (auto i = not_mst.crbegin(); i != not_mst.crend(); ++i) {
    if (sum + edges[*i] > s) {
      break;
    } else {
      ans.push_back(*i);
      sum += edges[*i];
    }
  }
  std::sort(ans.begin(), ans.end());
  std::cout << ans.size() << std::endl;
  for (auto i : ans) {
    std::cout << i + 1 << ' ';
  }
  return 0;
}