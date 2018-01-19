#include <vector>
#include <iostream>
using namespace std;

// arr = {4, -8, 9, -4, 1, -8, -1, 6}, k = 4
// output = {9, 6, 6, 5}

// arr = {-2, -3, 4, -1, -2, 1, 5, -3}, k = 3
// output = {7, 6, 5}

vector<int> findKMaxSumSubarrayN2(vector<int> &arr, int K) {
    vector<int> maxhere(arr.size(), 0);
    vector<int> heap, res;
    heap.push_back(arr[0]);

    maxhere[0] = arr[0];
    for (int i=1; i<arr.size(); i++) {
        maxhere[i] = maxhere[i-1] + arr[i];
    }

    for (int i=1; i<arr.size(); i++) {
        heap.push_back(maxhere[i]);
        for (int j=i-1; j>=0; j--) {
            heap.push_back(maxhere[i] - maxhere[j]);
        }
    }
    sort(heap.begin(), heap.end(), greater<int>());
    for (int i=0; i<K; i++) {
        res.push_back(heap[i]);
    }
    return res;
}

vector<int> findKMaxSumSubarray(vector<int> &arr, int K) {
    vector<int> res, arrMin;
    vector<int> maxhere(arr.size(), 0);
    int resMin = INT_MAX, resPos = 0;

    maxhere[0] = arr[0];
    for (int i=1; i<arr.size(); i++) {
        maxhere[i] = maxhere[i-1] + arr[i];
    }

    for (int i=0; i<arr.size(); i++) {
        if (res.size() >= K) {
            for (int j=0; j<arrMin.size(); j++) {
                resMin = INT_MAX;
                for (int j=0; j<K; j++) {
                    if (res[j] < resMin) {
                        resMin = res[j];
                        resPos = j;
                    }
                }
                if ((maxhere[i] - arrMin[j]) > resMin) {
                    res[resPos] = (maxhere[i] - arrMin[j]);
                }
            }
        } else {
            for (int j=0; j<arrMin.size() && res.size() < K; j++) {
                res.push_back(maxhere[i] - arrMin[j]);
            }
        }
        if (arrMin.size() < K)
            arrMin.push_back(maxhere[i]);
        else {
            int temp = INT_MIN, tempPos;
            for (int j=0; j<K; j++) {
                if (arrMin[j] > temp) {
                    temp = arrMin[j];
                    tempPos = j;
                }
            }
            if (maxhere[i] < temp) {
                arrMin[tempPos] = maxhere[i];
            }
        }
    }
    return res;
}

int main(void) {
    int TC, N, K, num;
    vector<int> arr, res;
    cin >> TC;
    for (int i=0; i<TC; i++) {
        cin >> N;
        arr.clear();
        for (int j=0; j<N; j++) {
            cin >> num;
            arr.push_back(num);
        }
        cin >> K;
        res = findKMaxSumSubarray(arr, K);
        for (int k=0; k<res.size(); k++) cout << res[k] << " ";
        cout << endl;
    }
}
