#include <bits/stdc++.h>
using namespace std;

int main() {
    map<int, int> arr;

    arr[0] = 1;
    arr[5] = 8;
    arr[2] = 3;

    for (auto i: arr) cout << i.second << ' ';
    cout << endl;
}