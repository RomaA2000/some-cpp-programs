#include <iostream>
#include <vector>
#include <queue>
#include <deque>

struct Edge {
  size_t begin = 0;
  size_t end = 0;
  int64_t capacity = 0;
  int64_t flow = 0;
  size_t back_num = 0;
  size_t num = 0;
  Edge() = default;
  Edge(size_t begin, size_t end,
       uint64_t capacity, uint64_t flow, size_t back_num, size_t num)
      : begin(begin), end(end), capacity(capacity),
        flow(flow), back_num(back_num), num(num) {}
  Edge(Edge const &) = default;
  Edge(Edge &&) = default;
  Edge &operator=(Edge const &) = default;
};

using path = std::pair<int64_t, std::vector<size_t>>;

class Graph {
 public:
  Graph(std::vector<std::vector<Edge>> graph)
      : graph(std::move(graph)) {
    t = graph.size() - 1;
  }

  Graph() = default;

  std::vector<path> solve_path() {
    std::vector<path> ans_vector;
    path ans;
    do {
      ans.second.clear();
      std::vector<bool> used(graph.size());
      std::vector<Edge *> edge_list;
      size_t now = s;
      while (!used[now] && now != t) {
        used[now] = true;
        for (auto &i : graph[now]) {
          if (i.flow > 0 && i.num != std::numeric_limits<size_t>::max() && !used[i.end]) {
            edge_list.push_back(&i);
            now = i.end;
            break;
          }
        }
      }
      ans.first = std::numeric_limits<int64_t>::max();
      for (auto i : edge_list) {
        ans.second.push_back(i->num);
        ans.first = std::min(ans.first, i->flow);
      }
      for (auto i : edge_list) {
        i->flow -= ans.first;
      }
      ans_vector.push_back(ans);
    } while (!ans.second.empty());
    ans_vector.pop_back();
    return ans_vector;
  }

  int64_t solve_flow() {
    int64_t max_flow = 0;
    std::vector<int64_t> dist(graph.size(), -1);
    std::vector<size_t> now(graph.size(), 0);
    while (bfs(dist)) {
      now.assign(graph.size(), 0);
      int64_t pushed = dfs(s, std::numeric_limits<int64_t>::max() / 10, now, dist);
      while (pushed != 0) {
        max_flow += pushed;
        pushed = dfs(s, std::numeric_limits<int64_t>::max() / 10, now, dist);
      }
    }
    return max_flow;
  }

  void add_edge(size_t begin, size_t end, uint64_t cap, size_t num) {
    if (graph.size() < std::max(begin, end) + 1) {
      graph.resize(std::max(begin, end) + 1);
    }
    graph[begin].emplace_back(begin, end, cap, 0, graph[end].size(), num);
    graph[end].emplace_back(end, begin, 0, 0,
        graph[begin].size() - 1, std::numeric_limits<size_t>::max());
    t = graph.size() - 1;
  }
  size_t t = 0;
  size_t s = 0;

 private:
  std::vector<std::vector<Edge>> graph;

  bool bfs(std::vector<int64_t> &dist) const {
    std::queue<size_t> q;
    q.push(s);
    dist.assign(graph.size(), -1);
    dist[s] = static_cast<int64_t>(0);
    while (!q.empty()) {
      size_t now = q.front();
      q.pop();
      for (size_t i = 0; i < graph[now].size(); ++i) {
        Edge e = graph[now][i];
        if ((dist[e.end] == -1) && (e.flow < e.capacity)) {
          dist[e.end] = dist[now] + 1;
          q.push(e.end);
        }
      }
    }
    return dist[t] != -1;
  }

  int64_t dfs(size_t v, int64_t flow, std::vector<size_t> &now, std::vector<int64_t> &dist) {
    if ((v == t) || (flow == 0)) {
      return flow;
    }
    for (; now[v] < graph[v].size(); ++now[v]) {
      Edge &e = graph[v][now[v]];
      if (dist[e.end] == dist[v] + 1) {
        int64_t pushed_flow = dfs(e.end, std::min(flow, e.capacity - e.flow), now, dist);
        if (pushed_flow > 0) {
          e.flow += pushed_flow;
          graph[e.end][e.back_num].flow -= pushed_flow;
          return pushed_flow;
        }
      }
    }
    return 0;
  }
};

int main() {
  size_t n = 0, m = 0;
  std::cin >> n >> m;
  Graph graph;
  size_t begin = 1, end = 1;
  uint64_t capacity = 0;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> begin >> end >> capacity;
    --begin;
    --end;
    graph.add_edge(begin, end, capacity, i);
  }
  graph.solve_flow();
  auto answer = graph.solve_path();
  std::cout << answer.size() << std::endl;
  for (auto const &i : answer) {
    std::cout << i.first << " " << i.second.size() << " ";
    for (auto j : i.second) {
      std::cout << j + 1 << " ";
    }
    std::cout << "\n";
  }
  std::vector<std::pair<size_t, size_t>> test;
  auto & [f, s] = test.back();
  std::cout << f << std::endl;
  return 0;
}
