#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>
#include <functional>
typedef long long ll;
typedef unsigned long long ull;
using namespace std;
void CoutVector(vector<int> outarray) {
    cout << outarray.size() << endl;
    for (int i = 0; i < (int)outarray.size(); i++) {
        cout << outarray[i] << " ";
    }
    cout << endl;
}
void PackBackpack(vector<int> &w, vector<int> &c, vector<vector<int>> &answer, int s, int k) {
    for (int i = 0; i <= s; i++) {
        answer[i][0] = 0;
    }
    for (int i = 0; i <= k; i++) {
        answer[0][i] = 0;
    }
    for (int i = 1; i <= k; i++) {
        for (int j = 0; j <= s; j++) {
            if (i == 0) {
                answer[j][i] = 0;
            } else {
                if (j >= w[i - 1]) {
                    answer[j][i] = max(answer[j][i - 1], answer[j - w[i - 1]][i - 1] + c[i - 1]);
                } else {
                    answer[j][i] = answer[j][i - 1];
                }
            }
        }
    }
}
vector<int> Backpack(vector<int> &w, vector<int> &c, int M) {
    vector<int> answer;
    vector<vector<int>> data;
    data.resize(M + 1);
    for (int i = 0; i < M + 1; i++) {
        data[i].resize(w.size() + 1);
    }
    PackBackpack(w, c, data, M, (int) w.size());
    int i = (int) w.size();
    int j = M;
    while (i > 0)  {
        if (data[j][i] != data[j][i - 1])  {
            answer.push_back(i);
            i--;
            j -= w[i];
        } else {
            i--;
        }
    }
    reverse(answer.begin(), answer.end());
    return answer;
}
int main() {
    freopen("knapsack.in", "r", stdin);
    freopen("knapsack.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n, M;
    cin >> n >> M;
    vector<int> w, c, answer;
    w.resize(n);
    c.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }
    answer = Backpack(w, c, M);
    CoutVector(answer);
    return 0;
}
