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


# :heavy_check_mark: library/ntt.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/ntt.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-06 10:54:59+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Verified with

* :heavy_check_mark: <a href="../../verify/test/ntt.test.cpp.html">test/ntt.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
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


#line 2 "library/ntt.hpp"

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

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

