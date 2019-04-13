#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

typedef int64_t ll;
typedef uint64_t ull;
typedef vector<vector<ll>> matrix;

void coutVector(vector<ll> &ans) {
    for (ll i = 0; i < (ll) ans.size(); i++) {
        cout << ans[i] << endl;
    }
}

void getVector(matrix &in, vector<pair<ll, ll>> &sequence, ll n) {
    in.clear();
    sequence.clear();
    in.resize(n);
    for (ll i = 0, a, b; i < n - 1; i++) {
        cin >> a >> b;
        sequence.push_back({a - 1, b - 1});
        in[a - 1].push_back(b - 1);
        in[b - 1].push_back(a - 1);
    }
}

void dfs(vector<ll> &under, vector<ll> &length, matrix &in, vector<bool> &was, ll v) {
    was[v] = 1;
    for (int i = 0; i < (int) in[v].size(); i++) {
        ll to = in[v][i];
        if (was[to] == 0) {
            dfs(under, length, in, was, to);
            under[v] += under[to] + 1;
            length[v] += (under[to] + 1 + length[to]);
        }
    }
}

void dfsUp(ll &n, vector<ll> &upLength, vector<ll> &under,
           vector<ll> &length, matrix &in, vector<bool> &was, ll v, ll pref) {
    was[v] = 1;
    ll to;
    if (v != pref) {
        upLength[v] = (n - under[v] - 1) + upLength[pref] +
        length[pref] - length[v] - under[v] - 1;
    }
    for (int i = 0; i < (int) in[v].size(); i++) {
        to = in[v][i];
        if (was[to] == 0) {
            dfsUp(n, upLength, under, length, in, was, to, v);
        }
    }
}

void getInf(vector<ll> &under, vector<ll> &length, vector<ll> &upLength, matrix &in, ll n) {
    vector<bool> was;
    was.assign(n, 0);
    under.assign(n, 0);
    length.assign(n, 0);
    upLength.assign(n, 0);
    dfs(under, length, in, was, 0);
    was.clear();
    was.assign(n, 0);
    dfsUp(n, upLength, under, length, in, was, 0, 0);
}

vector<ll> getAns(matrix &in, vector<pair<ll, ll>> &sequence, ll n) {
    vector<ll> out, under, length, upLength;
    out.clear();
    getInf(under, length, upLength, in, n);
    for (ll i = 0, a, b; i < (int) sequence.size(); i++) {
        a = sequence[i].first;
        b = sequence[i].second;
        if (under[a] < under[b]) {
            swap(a, b);
        }
        ll u1, u2, term1, term2;
        u1 = length[b] + under[b] + 1;
        u2 = upLength[a] + length[a] - length[b] - under[b] - 1;
        term1 = n - under[b] - 1;
        term2 = under[b] + 1;
        out.push_back(u1*term1 + term2*u2);
    }
    return out;
}

int main() {
    freopen("treedp.in", "r", stdin);
    freopen("treedp.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    ll n;
    cin >> n;
    matrix in;
    vector<pair<ll, ll>> sequence;
    getVector(in, sequence, n);
    vector<ll> out;
    out = getAns(in, sequence, n);
    coutVector(out);
    return 0;
}
