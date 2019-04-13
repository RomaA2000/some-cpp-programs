//
// Created by Роман Агеев on 2019-03-19.
//
#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <ctime>

using namespace std;
uint32_t gen_rand(uint32_t);

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

size_t next_prime(size_t in) {
  while (is_not_prime(in)) {
    ++in;
  }
  return in;
}

template<>
class hash_f<string> {
  static size_t p;
  size_t m;

 public:
  explicit hash_f(size_t size) : m(size) {
  }

  uint32_t gen_hash(string in) {
    uint32_t ans = 0;
    for (size_t i = 0; i < in.size(); i++) {
      ans = ans * (uint32_t) p + (in[i] - 'a' + 1);
    }
    return (uint32_t) (ans % m);
  }
};

template<typename T1, typename T2>
struct mymap {
  size_t min_size;
  size_t size;
  size_t counter;
  hash_f<T1> in;
  struct inside_set {
    size_t min_size;
    size_t counter;
    size_t size;
    hash_f<T2> inh;
    vector<list<T2>> inf;

    inside_set(size_t sz) : min_size(8), counter(), size(sz), inh(sz) {
      inf.resize(sz);
    }

    inside_set() : min_size(8), counter(), size(min_size), inh(min_size) {
      inf.resize(min_size);
    }

    void resize() {
      if (10 * size <=  counter) {
        size *= 2;
        vector<list<T2>> new_data = vector<list<T2>>(size);
        inh = hash_f<T2>(size);
        new_data.swap(inf);
        for (auto& ls : new_data) {
          for (const auto &x : ls) {
            size_t hash = inh.gen_hash(x);
            inf[hash].push_back(x);
          }
        }
      }
    }

    void deleter(T2 &inp) {
      size_t h = inh.gen_hash(inp);
      for (auto i = inf[h].begin(); i != inf[h].end(); i++) {
        if (*i == inp) {
          inf[h].erase(i);
          counter--;
          return;
        }
      }
    }

    void clear() {
      inside_set new_in;
      swap(*this, new_in);
    }

    void coutall(vector<T2> &ans) {
      for (size_t i = 0; i < size; i++) {
        for (auto j = inf[i].begin(); j != inf[i].end(); j++) {
          ans.push_back(*j);
        }
      }
    }

    void put(T2 &inp) {
      size_t h = inh.gen_hash(inp);
      inf[h].push_back(inp);
      counter++;
      resize();
    }
  };
  vector<list<pair<T1, inside_set>>> inf;

  auto finder(T1 &elem) {
    size_t h = in.gen_hash(elem);
    for (auto i = inf[h].begin(); i != inf[h].end(); i++) {
      if ((*i).first == elem) {
        return i;
      }
    }
    return inf[h].end();
  }

  bool find_in_set(T1 &inff, T2 &infs) {
    auto iter = finder(inff);
    if (iter == inf[in.gen_hash(inff)].end()) {
      return false;
    }
    pair<T1, inside_set> &i = *iter;
    list<T2> &now = i.second.inf[i.second.inh.gen_hash(infs)];
    for (auto j = now.begin(); j != now.end(); j++) {
      if (*j == infs) {
        return true;
      }
    }
    return false;
  }

  void resize_map() {
    if (10 *  size <= counter) {
      size *= 2;
      vector<list<pair<T2, inside_set>>> new_data = vector<list<pair<T2, inside_set>>>(size);
      in = hash_f<T2>(size);
      new_data.swap(inf);
      for (auto& ls : new_data) {
        for (const auto &x : ls) {
          size_t hash = in.gen_hash(x.first);
          inf[hash].push_back(x);
        }
      }
    }
  }

  mymap(size_t size1) : min_size(8), size(size1), counter(), in(size1) {
    inf.resize(size1);
  }

  mymap() : min_size(8), size(min_size), counter(), in(min_size) {
    inf.resize(min_size);
  }

  void insert(T1 &inputf, T2 &inputs) {
    size_t h = in.gen_hash(inputf);
    pair<T2, inside_set> e;
    e.first = inputf;
    if (!find_in_set(inputf, inputs)) {
      auto iter = finder(inputf);
      if (iter == inf[h].end()) {
        inf[h].push_back(e);
        iter = finder(inputf);
      }
      (*iter).second.put(inputs);
      counter++;
    }
    resize_map();
  }

  void deleter(T1 &inputf, T2 &inputs) {
    if (find_in_set(inputf, inputs)) {
      auto iter = finder(inputf);
      (*iter).second.deleter(inputs);
    }
  }

  void coutall(T1 &input, vector<T2> &ans) {
    auto i = finder(input);
    ans.clear();
    if (i == inf[in.gen_hash(input)].end()) {
      return;
    } else {
      (*i).second.coutall(ans);
    }
  }

  void erase(T1 &input) {
    auto i = finder(input);
    if (i != inf[in.gen_hash(input)].end()) {
      inf[in.gen_hash(input)].erase(i);
      counter--;
    }
  }
};

uint32_t gen_rand(uint32_t min, uint32_t max) {
  static mt19937 gen((uint32_t) time(nullptr));
  return uniform_int_distribution<uint32_t>(min, max)(gen);
}

uint32_t gen_rand(uint32_t min) {
  return gen_rand(min, INT32_MAX);
}

size_t hash_f<string>::p = next_prime(29);

int main() {
  freopen("multimap.in", "r", stdin);
  freopen("multimap.out", "w", stdout);
  mymap<string, string> in;
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string input;
  string a, b;
  while (cin >> input) {
    cin >> a;
    if ((input[0] == 'p') && (input[1] == 'u')) {
      cin >> b;
      in.insert(a, b);
    } else if (input.size() == 9) {
      in.erase(a);
    } else if (input[0] == 'd') {
      cin >> b;
      in.deleter(a, b);
    } else if (input[0] == 'g') {
      vector<string> all;
      in.coutall(a, all);
      cout << all.size() << ' ';
      if (!all.empty()) {
        for (uint32_t i = 0; i < (uint32_t) all.size(); i++) {
          cout << all[i] << ' ';
        }
      }
      cout << '\n';
    }
  }
  return 0;
}

