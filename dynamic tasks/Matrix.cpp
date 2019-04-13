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
int INF = INT32_MAX;
vector<vector<int>> input;
vector<vector<string>> ans;
class matrix {
 public:
    int width;
    int height;
    matrix() {
        width = 0;
        height = 0;
    }
};
void GetAns(vector<vector<string>> &ans, vector<vector<int>> &input,
            vector<matrix> &allmatrix, int left, int right) {
    if (input[left][right] == -1) {
        if (left == right - 1) {
            input[left][right] = -1;
            ans[left][right] = "A";
        } else {
            input[left][right] = INF;
            for (int i = left + 1; i <= right - 1; i++) {
                GetAns(ans, input, allmatrix, left, i);
                GetAns(ans, input, allmatrix, i, right);
                int answer = allmatrix[left].width *
                allmatrix[i - 1].height * allmatrix[right - 1].height +
                input[left][i] + input[i][right];
                if (answer < input[left][right]) {
                    input[left][right] = answer;
                    ans[left][right] = "(" + ans[left][i] + ans[i][right] + ")";
                }
            }
        }
    }
}
void GetBraces(vector<matrix> &allmatrix) {
    int size = (int) allmatrix.size();
    input.resize(size + 1);
    ans.resize(size + 1);
    for (int i = 0; i < size + 1; i++) {
        input[i].resize(size + 1);
        ans[i].resize(size + 1);
    }
    for (int i = 0; i < size + 1; i++) {
        for (int j = 0; j < size + 1; j++) {
            input[i][j] = -1;
        }
    }
    GetAns(ans, input , allmatrix, 0, (int) allmatrix.size());
}
int main() {
    ll n;
    vector<matrix> allmatrix;
    freopen("matrix.in", "r", stdin);
    freopen("matrix.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    allmatrix.resize(n);
    int w, h;
    for (int i = 0; i < n; i++) {
        cin >> w >> h;
        allmatrix[i].width = w;
        allmatrix[i].height = h;
    }
    GetBraces(allmatrix);
    cout << ans[0][allmatrix.size()] << endl;
    return 0;
}
