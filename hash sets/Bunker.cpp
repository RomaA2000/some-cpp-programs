//
// Created by Роман Агеев on 2019-03-24.
//
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

vector<vector<int32_t >> graph;
int32_t max_d, root, max_vert;
bool is_good;
struct node {
  int32_t parent;
  int32_t depth;
  node(int32_t a, int32_t b) : parent(a), depth(b) {
  }
  node() : node(-1, -1) {
  }
};

void create(int32_t in, int32_t n) {
  vector<bool> was(n);
  queue<int32_t> queue;
  vector<node> tree(n);
  was[in] = 1;
  queue.push(in);
  tree[in].depth = 0;
  while (!queue.empty()) {
    int32_t now = queue.front();
    queue.pop();
    for (size_t i = 0; i < (size_t) graph[now].size(); i++) {
      int32_t next_node = graph[now][i];
      if (!was[next_node]) {
        queue.push(next_node);
        was[next_node] = 1;
        tree[next_node].parent = now;
        tree[next_node].depth = tree[now].depth + 1;
      }
    }
  }
  max_d = tree[0].depth;
  max_vert = 0;
  for (int32_t i = 0; i < n; i++) {
    if (tree[i].depth > max_d) {
      max_vert = i;
      max_d = tree[i].depth;
    }
  }
  root = max_vert;
  for (int32_t i = 0; i < max_d / 2; i++) {
    root = tree[root].parent;
  }
}

void gen_hash(pair<int64_t, int64_t> &h, int64_t in) {
  h = {(int64_t) (h.first * in), (int64_t) (h.second + in)};
}

int64_t check(bool is_root, int32_t in, vector<bool> &was) {
  was[in] = 1;
  vector<int64_t> ans;
  pair<int64_t, int64_t> h = {1, 0};
  for (size_t i = 0; i < (size_t) graph[in].size(); i++) {
    int32_t now = graph[in][i];
    if (!was[now]) {
      ans.push_back(check(false, now, was));
      gen_hash(h, ans.back());
    }
  }
  if (is_root) {
    if ((ans.size() == 2) && (ans[0] == ans[1])) {
      is_good = 1;
    }
  }
  return h.first + h.second;
}

bool check_ans(int32_t in, int32_t n) {
  vector<bool> was(n);
  check(true, in, was);
  return is_good;
}

void start(int32_t n) {
  graph.resize(n);
  int32_t a, b;
  for (size_t i = 0; i < (size_t) n - 1; i++) {
    cin >> a >> b;
    --a;
    --b;
    graph[a].push_back(b);
    graph[b].push_back(a);
  }
  create(0, n);
  create(max_vert, n);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int32_t n;
  cin >> n;
  start(n);
  if (max_d % 2 != 0 || graph[root].size() != 2) {
    cout << "NO" << '\n';
  } else {
    if (check_ans(root, n)) {
      cout << "YES" << '\n';
    } else {
      cout << "NO" << '\n';
    }
  }
  return 0;
}
