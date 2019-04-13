//
// Created by Роман Агеев on 2019-03-18.
//
#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <ctime>

using namespace std;
const uint32_t AlfSize = 26;
template<class T2>
class hash_f {
};

bool is_not_prime(size_t in) {
  for (size_t i = 2; i * i <= in; ++i) {
    if (in % i == 0) {
      return true;
    }
  }
  return false;
}

size_t last_prime(size_t in) {
  while (is_not_prime(in)) {
    --in;
  }
  return in;
}
size_t next_prime(size_t in) {
  while (is_not_prime(in)) {
    ++in;
  }
  return in;
}

template<>
class hash_f<string> {
  static size_t p;
  static vector<size_t> p_all;
  size_t m;

 public:
  explicit hash_f(size_t size) : m(size) {
  }

  uint32_t gen_hash(string in) {
    if (p_all.size() < in.length()) {
      size_t last = p_all.size();
      p_all.resize(in.length());
      if (p_all.size() > 0) {
        p_all[0] = 1;
      }
      for (size_t i = 1 < last ? last : 1; i < in.length(); i++) {
        p_all[i] = p_all[i - 1] * p;
      }
    }
    uint32_t ans = 0;
    for (size_t i = 0; i < in.size(); i++) {
      ans +=  (uint32_t)((in[i] - 'a' + 1) * p_all[i]);
    }
    return (uint32_t) (ans % m);
  }
};

uint32_t gen_rand(uint32_t min, uint32_t max) {
  static mt19937 gen((uint32_t) time(nullptr));
  return uniform_int_distribution<uint32_t>(min, max)(gen);
}

uint32_t gen_rand(uint32_t min) {
  return gen_rand(min, INT32_MAX);
}

template<typename T1, typename T2>
struct mymap {
  size_t min_size;
  size_t size;
  size_t counter;
  hash_f<T1> in;
  vector<list<pair<T1, T2>>> inf;

  auto find(list<pair<T1, T2>> &now, T1 elem) {
    for (auto i = now.begin(); i != now.end(); i++) {
      if ((*i).first == elem) {
        return i;
      }
    }
    return now.end();
  }

  void resize() {
    if (4 * size <= 3 * counter) {
      mymap new_in(size * 2);
      for (size_t i = 0; i < (size_t) inf.size(); i++) {
        for (auto j = inf[i].begin(); j != inf[i].end(); j++) {
          new_in.new_insert(*j);
        }
      }
      swap(*this, new_in);
    }
  }

  mymap(size_t size1) : min_size(100), size(size1), counter(), in(size1) {
    inf.resize(size1);
  }

  void new_insert(pair<T1, T2> input) {
    inf[in.gen_hash(input.first)].push_back(input);
    counter++;
  }

 public:
  mymap() : min_size(100), size(min_size), counter(), in(min_size) {
    inf.resize(min_size);
  }

  void insert(pair<T1, T2> input) {
    if (find(inf[in.gen_hash(input.first)], input.first) == inf[in.gen_hash(input.first)].end()) {
      inf[in.gen_hash(input.first)].push_back(input);
      counter++;
      resize();
    } else {
      auto i = find(inf[in.gen_hash(input.first)], input.first);
      (*i).second = input.second;
    }
  }

  void erase(T1 input) {
    if (find(inf[in.gen_hash(input)], input) != inf[in.gen_hash(input)].end()) {
      inf[in.gen_hash(input)].erase(find(inf[in.gen_hash(input)], input));
      counter--;
    }
  }

  const T2 *find(T1 input) {
    if (find(inf[in.gen_hash(input)], input) != inf[in.gen_hash(input)].end()) {
      return &((*find(inf[in.gen_hash(input)], input)).second);
    } else {
      return nullptr;
    }
  }
};

size_t hash_f<string>::p = next_prime(AlfSize);
vector<size_t> hash_f<string>::p_all;
int main() {
  freopen("map.in", "r", stdin);
  freopen("map.out", "w", stdout);
  mymap<string, string> in;
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string input;
  string a, b;
  while (cin >> input) {
    cin >> a;
    if (input[0] == 'p') {
      cin >> b;
      in.insert({a, b});
    } else if (input[0] == 'd') {
      in.erase(a);
    } else if (input[0] == 'g') {
      auto ans = in.find(a);
      cout << (ans == nullptr ? "none" : *ans) << '\n';
    }
  }
  return 0;
}
