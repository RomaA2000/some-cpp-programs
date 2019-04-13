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

size_t last_prime(size_t in) {
  while (is_not_prime(in)) {
    --in;
  }
  return in;
}

template<>
class hash_f<uint32_t> {
  static size_t p;
  size_t m;
  uint32_t a;
  uint32_t b;

 public:
  explicit hash_f(size_t size) : m(size), a(gen_rand(0)), b(gen_rand(0)) {
  }

  size_t gen_hash(uint32_t in) {
    return (size_t) (((a * in + b) % p) % m);
  }
};

uint32_t gen_rand(uint32_t min, uint32_t max) {
  static mt19937 gen((uint32_t) time(nullptr));
  return uniform_int_distribution<uint32_t>(min, max)(gen);
}

uint32_t gen_rand(uint32_t min) {
  return gen_rand(min, INT32_MAX);
}

template<typename T1>
struct myset {
 private:
  size_t min_size;
  size_t size;
  size_t counter;
  hash_f<T1> in;
  vector<list<T1>> inf;

  auto find(list<T1> &now, T1 elem) {
    for (auto i = now.begin(); i != now.end(); i++) {
      if (*i == elem) {
        return i;
      }
    }
    return now.end();
  }

  void resize() {
    if (4 * size <= 3 * counter) {
      myset new_in(size * 2);
      for (size_t i = 0; i < (size_t) inf.size(); i++) {
        for (auto j = inf[i].begin(); j != inf[i].end(); j++) {
          new_in.new_insert(*j);
        }
      }
      swap(*this, new_in);
    }
  }

  myset(size_t size1) : min_size(100), size(size1), counter(), in(size1) {
    inf.resize(size1);
  }

  void new_insert(uint32_t input) {
    inf[in.gen_hash(input)].push_back(input);
    counter++;
  }

 public:
  myset() : min_size(100), size(min_size), counter(), in(min_size) {
    inf.resize(min_size);
  }

  void insert(uint32_t input) {
    if (find(inf[in.gen_hash(input)], input) == inf[in.gen_hash(input)].end()) {
      inf[in.gen_hash(input)].push_back(input);
      counter++;
      resize();
    }
  }

  void erase(uint32_t input) {
    if (find(inf[in.gen_hash(input)], input) != inf[in.gen_hash(input)].end()) {
      inf[in.gen_hash(input)].erase(find(inf[in.gen_hash(input)], input));
      counter--;
    }
  }

  bool find(uint32_t input) {
    return find(inf[in.gen_hash(input)], input) != inf[in.gen_hash(input)].end();
  }
};

size_t hash_f<uint32_t>::p = last_prime(UINT32_MAX);

int main() {
  freopen("set.in", "r", stdin);
  freopen("set.out", "w", stdout);
  myset<uint32_t> in;
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string input;
  uint32_t a;
  while (cin >> input) {
    cin >> a;
    if (input[0] == 'i') {
      in.insert(a);
    } else if (input[0] == 'd') {
      in.erase(a);
    } else if (input[0] == 'e') {
      cout << (in.find(a) ? "true" : "false") << '\n';
    }
  }
  return 0;
}
