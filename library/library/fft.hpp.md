---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :warning: library/fft.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/fft.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :question: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

namespace FFT {
// FFT 用の簡易複素数
struct Comp {
    double real, imag;

    Comp(double r = 0, double i = 0) : real(r), imag(i) {}
    template <class T>
    Comp(T r) : real(r){};
    // 極形式で生成
    static Comp polar(double r, double theta) {
        return Comp(r * cos(theta), r * sin(theta));
    }
    Comp &operator+=(const Comp &rhs) {
        real += rhs.real;
        imag += rhs.imag;
        return *this;
    }
    Comp &operator-=(const Comp &rhs) {
        real -= rhs.real;
        imag -= rhs.imag;
        return *this;
    }
    Comp &operator*=(const Comp &rhs) {
        double new_real = real * rhs.real - imag * rhs.imag;
        imag = real * rhs.imag + imag * rhs.real;
        real = new_real;
        return *this;
    }
    // 複素共役
    Comp operator~() const { return Comp(real, -imag); }
};
Comp operator+(const Comp &lhs, const Comp &rhs) { return Comp(lhs) += rhs; }
Comp operator-(const Comp &lhs, const Comp &rhs) { return Comp(lhs) -= rhs; }
Comp operator*(const Comp &lhs, const Comp &rhs) { return Comp(lhs) *= rhs; }
Comp operator/(const Comp &lhs, const Comp &rhs) { return Comp(lhs) /= rhs; }
ostream &operator<<(ostream &os, const Comp &comp_var) {
    os << comp_var.real << "+" << comp_var.imag << "i";
    return os;
}

// 重み（回転因子）: Weight[i][j] = 1 の原始 2^i 乗根の j 乗
vector<vector<Comp>> Weight(1, vector<Comp>(1, 1));

// 重みの初期化（step = log_2(N)）
void W_init(int step) {
    for (int i = 1; i <= step; i++) {
        Weight.emplace_back(1 << (i - 1));
        const double theta = 2 * M_PI / (1 << i);
        for (int j = 0; j < (1 << (i - 1)); j++) {
            Weight[i][j] = ((j & 1) != 0 ? Comp::polar(1.0, theta * j)
                                         : Weight[i - 1][j >> 1]);
        }
    }
}

// ビット反転並べ替え
void bit_reverse(vector<Comp> &x) {
    int n = x.size();
    for (int i = 0, j = 1; j < n - 1; j++) {
        for (int k = n >> 1; k > (i ^= k); k >>= 1)
            ;
        if (i < j) std::swap(x[i], x[j]); // x[i]とx[j]を交換する
    }
}

// Cooley–Tukey FFT algorithm
template <bool inverse = false>
void fft(vector<Comp> &a) {
    int n = a.size();
    int h = 0; // h = log_2(n)
    for (int i = 0; 1 << i < n; i++) h++;
    bit_reverse(a); // バタフライ演算用の並べ替え

    // バタフライ演算
    for (int s = 1, b = 1; b < n; s++, b <<= 1) {
        // 第 s = log_2(b)+1 段
        // ブロックサイズ = 2b = 2^s
        for (int j = 0; j < b; j++) {
            // グループ内 j 個目
            // 重み w = (1 の原始 2b=2^s 乗根の j 乗)
            Comp w = (inverse ? ~Weight[s][j] : Weight[s][j]);
            for (int k = 0; k < n; k += b * 2) {
                // k を先頭とするグループ
                Comp s = a[j + k];         // 前
                Comp t = a[j + k + b] * w; // 後
                a[j + k] = s + t;          // 前の更新
                a[j + k + b] = s - t;      // 後の更新
            }
        }
    }
    // 逆変換時にサイズで割る調整
    if (inverse)
        for (int i = 0; i < n; i++) a[i] /= n;
}

// FFT による畳み込み（答えが a に入る）
void convolve(vector<double> &a, const vector<double> &b) {
    int s = a.size() + b.size() - 1; // 畳み込み結果のサイズ
    int t = 1; // FFT に使う配列のサイズ（2 の累乗）
    int step = 0;
    while (t < s) {
        t *= 2;
        step++;
    }
    // 実部を a, 虚部を b とした複素数列を作成
    vector<Comp> C(t, Comp(0, 0));
    for (int i = 0; i < a.size(); i++) {
        C[i].real = a[i];
    }
    for (int i = 0; i < b.size(); i++) {
        C[i].imag = b[i];
    }
    W_init(step); // 重み初期化
    fft(C);       // a, b をまとめて FFT
    // まとめて FFT した結果から A[i]*B[i] を計算
    C[0] = (C[0] + ~C[0]) * (C[0] - ~C[0]) * Comp(0, -0.25);
    for (int i = 1; i <= (t >> 1); i++) {
        Comp x = C[i] + ~C[t - i];
        Comp y = C[i] - ~C[t - i];
        C[i] = x * y * Comp(0, -0.25);
        C[t - i] = ~C[i];
    }
    fft<true>(C); // IFFT
    a.resize(s);
    for (int i = 0; i < s; i++) a[i] = C[i].real; // 実部が答え
}
} // namespace FFT

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
            for (int i = 0; i < n; i++)
                (a[i] *= powMod(n, mod - 2, mod)) %= mod;
        return a;
    }

    static vector<long long> convolve(vector<long long> a,
                                      vector<long long> b) {
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
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "library/header.hpp"



#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <sys/timeb.h>
#include <vector>

using namespace std;

#define repr(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rep(i, n) repr(i, 0, n)
#define reprrev(i, a, b) for (int i = (int)(b)-1; i >= (int)(a); i--)
#define reprev(i, n) reprrev(i, 0, n)
#define repi(itr, ds) for (auto itr = ds.begin(); itr != ds.end(); itr++)
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define mp make_pair
#define mt make_tuple
#define INF 1050000000
#define INFR INT_MAX
#define INFL (long long)(4e18)
#define INFLR LLONG_MAX
#define EPS (1e-10)
#define MOD 1000000007
//#define MOD 998244353
#define PI 3.141592653589793238
#define RMAX 4294967295

typedef long long ll;
typedef pair<int, int> Pi;
typedef pair<ll, ll> Pll;
typedef pair<double, double> Pd;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<bool> vb;
typedef vector<char> vc;
typedef vector<string> vs;
typedef vector<double> vd;
typedef vector<Pi> vPi;
typedef vector<Pll> vPll;
typedef vector<Pd> vPd;
typedef vector<vector<int>> vvi;
typedef vector<vector<bool>> vvb;
typedef vector<vector<ll>> vvll;
typedef vector<vector<char>> vvc;
typedef vector<vector<string>> vvs;
typedef vector<vector<double>> vvd;
typedef vector<vector<Pi>> vvPi;
typedef vector<vector<vector<int>>> vvvi;
typedef vector<vector<vector<ll>>> vvvll;
typedef vector<vector<vector<Pi>>> vvvPi;
typedef vector<vector<vector<vector<Pi>>>> vvvvPi;
typedef priority_queue<int, vector<int>, greater<int>> pqli;
typedef priority_queue<ll, vector<ll>, greater<ll>> pqlll;
typedef priority_queue<Pi, vector<Pi>, greater<Pi>> pqlP;
template <class T>
using vec = vector<T>;
template <class T>
using pql = priority_queue<T, vector<T>, greater<T>>;

template <class T>
struct Edge {
    int from, to;
    T cost;
    bool operator<(Edge e) {
        return cost < e.cost;
    }
    Edge(int f, int t, T c) : from(f), to(t), cost(c) {}
};

template <class T>
ostream &operator<<(ostream &os, Edge<T> &edge) {
    os << "(" << edge.from << "->" << edge.to << ":" << edge.cost << ")";
    return os;
}
template <class T = int>
class Graph {
    int n;
    bool directed;
    vector<vector<Edge<T>>> edges;

  public:
    Graph(int n, bool directed)
        : n(n), directed(directed), edges(vector<vector<Edge<T>>>(n)) {}

    void add_edge(int s, int t, T cost) {
        edges[s].emplace_back(s, t, cost);
        if (!directed) {
            edges[t].emplace_back(t, s, cost);
        }
    }
    Graph() {}

    vector<Edge<T>> &operator[](size_t i) {
        return edges[i];
    }

    vector<Edge<T>> &operator[](size_t i) const {
        return edges[i];
    }

    int size() const {
        return n;
    }
};


#line 2 "library/fft.hpp"

namespace FFT {
// FFT 用の簡易複素数
struct Comp {
    double real, imag;

    Comp(double r = 0, double i = 0) : real(r), imag(i) {}
    template <class T>
    Comp(T r) : real(r){};
    // 極形式で生成
    static Comp polar(double r, double theta) {
        return Comp(r * cos(theta), r * sin(theta));
    }
    Comp &operator+=(const Comp &rhs) {
        real += rhs.real;
        imag += rhs.imag;
        return *this;
    }
    Comp &operator-=(const Comp &rhs) {
        real -= rhs.real;
        imag -= rhs.imag;
        return *this;
    }
    Comp &operator*=(const Comp &rhs) {
        double new_real = real * rhs.real - imag * rhs.imag;
        imag = real * rhs.imag + imag * rhs.real;
        real = new_real;
        return *this;
    }
    // 複素共役
    Comp operator~() const { return Comp(real, -imag); }
};
Comp operator+(const Comp &lhs, const Comp &rhs) { return Comp(lhs) += rhs; }
Comp operator-(const Comp &lhs, const Comp &rhs) { return Comp(lhs) -= rhs; }
Comp operator*(const Comp &lhs, const Comp &rhs) { return Comp(lhs) *= rhs; }
Comp operator/(const Comp &lhs, const Comp &rhs) { return Comp(lhs) /= rhs; }
ostream &operator<<(ostream &os, const Comp &comp_var) {
    os << comp_var.real << "+" << comp_var.imag << "i";
    return os;
}

// 重み（回転因子）: Weight[i][j] = 1 の原始 2^i 乗根の j 乗
vector<vector<Comp>> Weight(1, vector<Comp>(1, 1));

// 重みの初期化（step = log_2(N)）
void W_init(int step) {
    for (int i = 1; i <= step; i++) {
        Weight.emplace_back(1 << (i - 1));
        const double theta = 2 * M_PI / (1 << i);
        for (int j = 0; j < (1 << (i - 1)); j++) {
            Weight[i][j] = ((j & 1) != 0 ? Comp::polar(1.0, theta * j)
                                         : Weight[i - 1][j >> 1]);
        }
    }
}

// ビット反転並べ替え
void bit_reverse(vector<Comp> &x) {
    int n = x.size();
    for (int i = 0, j = 1; j < n - 1; j++) {
        for (int k = n >> 1; k > (i ^= k); k >>= 1)
            ;
        if (i < j) std::swap(x[i], x[j]); // x[i]とx[j]を交換する
    }
}

// Cooley–Tukey FFT algorithm
template <bool inverse = false>
void fft(vector<Comp> &a) {
    int n = a.size();
    int h = 0; // h = log_2(n)
    for (int i = 0; 1 << i < n; i++) h++;
    bit_reverse(a); // バタフライ演算用の並べ替え

    // バタフライ演算
    for (int s = 1, b = 1; b < n; s++, b <<= 1) {
        // 第 s = log_2(b)+1 段
        // ブロックサイズ = 2b = 2^s
        for (int j = 0; j < b; j++) {
            // グループ内 j 個目
            // 重み w = (1 の原始 2b=2^s 乗根の j 乗)
            Comp w = (inverse ? ~Weight[s][j] : Weight[s][j]);
            for (int k = 0; k < n; k += b * 2) {
                // k を先頭とするグループ
                Comp s = a[j + k];         // 前
                Comp t = a[j + k + b] * w; // 後
                a[j + k] = s + t;          // 前の更新
                a[j + k + b] = s - t;      // 後の更新
            }
        }
    }
    // 逆変換時にサイズで割る調整
    if (inverse)
        for (int i = 0; i < n; i++) a[i] /= n;
}

// FFT による畳み込み（答えが a に入る）
void convolve(vector<double> &a, const vector<double> &b) {
    int s = a.size() + b.size() - 1; // 畳み込み結果のサイズ
    int t = 1; // FFT に使う配列のサイズ（2 の累乗）
    int step = 0;
    while (t < s) {
        t *= 2;
        step++;
    }
    // 実部を a, 虚部を b とした複素数列を作成
    vector<Comp> C(t, Comp(0, 0));
    for (int i = 0; i < a.size(); i++) {
        C[i].real = a[i];
    }
    for (int i = 0; i < b.size(); i++) {
        C[i].imag = b[i];
    }
    W_init(step); // 重み初期化
    fft(C);       // a, b をまとめて FFT
    // まとめて FFT した結果から A[i]*B[i] を計算
    C[0] = (C[0] + ~C[0]) * (C[0] - ~C[0]) * Comp(0, -0.25);
    for (int i = 1; i <= (t >> 1); i++) {
        Comp x = C[i] + ~C[t - i];
        Comp y = C[i] - ~C[t - i];
        C[i] = x * y * Comp(0, -0.25);
        C[t - i] = ~C[i];
    }
    fft<true>(C); // IFFT
    a.resize(s);
    for (int i = 0; i < s; i++) a[i] = C[i].real; // 実部が答え
}
} // namespace FFT

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
            for (int i = 0; i < n; i++)
                (a[i] *= powMod(n, mod - 2, mod)) %= mod;
        return a;
    }

    static vector<long long> convolve(vector<long long> a,
                                      vector<long long> b) {
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

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

