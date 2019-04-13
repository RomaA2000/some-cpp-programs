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
ll n;
void CoutVector(vector<int>& outarray) {
    for (int i = 0; i < (int)outarray.size(); i++) {
        cout << outarray[i] << " ";
    }
    cout << endl;
}
vector<int> lis(vector<int> array) {
    int size = (int) array.size();
    vector <int> ans, pos, prev;
    ans.resize(size + 1);
    pos.resize(size + 1);
    prev.resize(size + 1);
    pos[0] = -1;
    prev[0] = -1;
    ans[0] = -2e9;
    int maxsize = 0;
    for (int i = 1; i <= size; i++) {
        ans[i] = 2e9;
    }
    for (int i = 0; i < size; i++) {
        int index = (int)((upper_bound(ans.begin(), ans.end(), array[i])) - ans.begin());
        if ((index > 0) && (index <= size)
            && (ans[index- 1] < array[i]) && (array[i] < ans[index])) {
            ans[index] = array[i];
            pos[index] = i;
            prev[i] = pos[index - 1];
            maxsize = max(maxsize, index);
        }
    }
    int first = pos[maxsize];
    vector<int> answer;
    while (first != -1) {
        answer.push_back(array[first]);
        first = prev[first];
    }
    reverse(answer.begin(), answer.end());
    cout << maxsize << endl;
    return answer;
}
int main() {
    freopen("lis.in", "r", stdin);
    freopen("lis.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    vector<int> array, ans;
    array.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }
    ans = lis(array);
    CoutVector(ans);
    return 0;
}
