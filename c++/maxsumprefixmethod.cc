#include <iostream>
#include <vector>
using namespace std;

// arr = {4, -8, 9, -4, 1, -8, -1, 6}
// output = 9

// arr = {-2, -3, 4, -1, -2, 1, 5, -3}
// output = 7

int findMaxSumSubarrayPrefix(vector<int> &arr) {
    int fsub, bsub, fIndex = 0, bIndex = arr.size()-1;
    vector<int> fPrefix(arr.size(), 0), bPrefix(arr.size(), 0);
    fPrefix[0] = arr[0];
    bPrefix[arr.size()-1] = arr[arr.size()-1];

    fsub = min(0, fPrefix[0]);
    bsub = min(0, bPrefix[arr.size()-1]);

    for (int i=1; i<arr.size(); i++) {
        fPrefix[i] = fPrefix[i-1] + arr[i];
        if (fPrefix[i] < fsub && i < bIndex) {
            fsub = fPrefix[i];
            fIndex = i;
        }
        bPrefix[arr.size()-1-i] = bPrefix[arr.size()-i] + arr[arr.size()-1-i];
        if (bPrefix[arr.size()-1-i] < bsub && (arr.size()-1-i) > fIndex) {
            bsub = bPrefix[arr.size()-1-i];
            bIndex = arr.size()-1-i;
        }
    }
    cout << "fsub=" << fsub << " bsub=" << bsub << endl;
    if (fsub < 0 && bsub < 0)
        return bPrefix[0]-bsub-fsub;
    else if (fsub < 0)
        return bPrefix[0]-fsub;
    else if (bsub < 0)
        return bPrefix[0]-bsub;
    else
        return bPrefix[0];
}

int main(void) {
    int TC, N, num;
    vector<int> arr;
    cin >> TC;
    for (int i=0; i<TC; i++) {
        cin >> N;
        arr.clear();
        for (int j=0; j<N; j++) {
            cin >> num;
            arr.push_back(num);
        }
        cout << findMaxSumSubarrayPrefix(arr) << endl;
    }
}
