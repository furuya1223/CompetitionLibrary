#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "../library/ntt.hpp"

int main(void) {
    int N, M;
    cin >> N >> M;
    vector<long long> a(N), b(M);
    rep(i, N) cin >> a[i];
    rep(i, M) cin >> b[i];
    vector<long long> c = NTT1.convolve(a, b);
    rep(i, c.size()) {
        cout << c[i];
        if (i == c.size() - 1)
            cout << endl;
        else
            cout << ' ';
    }
}