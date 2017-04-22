#include <iostream>
#include <vector>
using namespace std;

#define max(a, b) (a) > (b) ? (a) : (b)
int knapsack(vector <int>& itemW, vector <int>& itemV, int W, int N) {
    int dp[N][W+1];

    for (int i=0; i<N; i++)
        for (int j=0; j<W+1; j++)
            dp[i][j] = 0;

    for (int i=0; i<N; i++) {
        for (int j=1; j<W+1; j++) {

            if (i==0) {
                dp[0][j] = itemV[0];
            } else {
                if (j < itemW[i])
                    dp[i][j] = dp[i-1][j];
                else
                    dp[i][j] = max(itemV[i] + dp[i-1][j-itemW[i]], dp[i-1][j]);
            }
        }
    }

    for (int i=0; i<N; i++) {
        for (int j=0; j<W+1; j++)
            cout << dp[i][j] << " ";
        cout << endl;
    }
    return dp[N-1][W];
}

int main(void) {
    int W, N, ans;
    vector <int> itemW, itemV;
    cout << "Total Weight to carry: ";
    cin >> W;
    cout << "Total available items: ";
    cin >> N;

    for (int i=0; i<N; i++) {
        int w;
        cin >> w;
        itemW.push_back(w);
    }

    for (int i=0; i<N; i++) {
        int v;
        cin >> v;
        itemV.push_back(v);
    }

    ans = knapsack(itemW, itemV, W, N);
    cout << "Maximum value that you can carry: " << ans << endl;
    return 0;
}
