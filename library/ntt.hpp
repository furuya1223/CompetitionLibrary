#include "header.hpp"

long long powMod(long long x, long long n, long long mod) {
    long long res = 1;
    while (n > 0) {
        if (n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

// NTT
template <long long mod, long long primitive_root>
class NTT {
  public:
    static long long get_mod() {
        return mod;
    }

    static vector<long long> fft(vector<long long> a, bool inverse = false) {
        int n = a.size();
        assert((n ^ (n & -n)) == 0); // n = 2^k
        int h = 0;
        for (int i = 0; 1 << i < n; i++) h++;
        // バタフライ演算用の配置入れ替え（{0,1,2,3,4,5,6,7}->{0,4,2,6,1,5,3,7}）
        for (int i = 0; i < n; i++) {
            int j = 0;
            for (int k = 0; k < h; k++) j |= (i >> k & 1) << (h - 1 - k);
            if (i < j) swap(a[i], a[j]);
        }
        // 1 の原始 n 乗根
        long long base_root = powMod(primitive_root, (mod - 1) / n, mod);
        if (inverse) {
            // 逆変換時には逆数を使用
            base_root = powMod(base_root, mod - 2, mod);
        }

        // バタフライ演算
        for (int i = 1, stage = 0; i < n; i *= 2, stage++) {
            // 第 log_2(i) + 1 段
            // ブロックサイズ = i * 2
            long long w = 1;
            // 1 の原始 2*i 乗根
            long long base = powMod(base_root, n / (2 * i), mod);
            for (int j = 0; j < i; j++) {
                // グループ内 j 個目
                // 重み w = 1 の原始 2*i 乗根の j 乗
                for (int k = 0; k < n; k += i * 2) {
                    // k を先頭とするグループ
                    long long s = a[j + k + 0];             // 前
                    long long t = (a[j + k + i] * w) % mod; // 後
                    a[j + k + 0] = (s + t) % mod;           // 前の更新
                    a[j + k + i] = (s - t + mod) % mod;     // 後の更新
                }
                (w *= base) %= mod;
            }
        }
        // 逆変換時にサイズで割る調整
        if (inverse)
            for (int i = 0; i < n; i++) (a[i] *= powMod(n, mod - 2, mod)) %= mod;
        return a;
    }

    static vector<long long> convolve(vector<long long> a, vector<long long> b) {
        int s = a.size() + b.size() - 1;
        int t = 1;
        while (t < s) t *= 2;
        a.resize(t);
        b.resize(t);
        a = fft(a);
        b = fft(b);
        for (int i = 0; i < t; i++) (a[i] *= b[i]) %= mod;
        a = fft(a, true);
        a.resize(s);
        return a;
    }
};
NTT<998244353, 3> NTT1;
NTT<469762049, 3> NTT2;
NTT<167772161, 3> NTT3;