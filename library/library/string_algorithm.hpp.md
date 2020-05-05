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


# :warning: library/string_algorithm.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/string_algorithm.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

// str と str[i:] の最長共通接頭辞の長さを O(|str|) で計算
void Z_algorithm(string str, vector<int> &Z) {
    const int L = str.length();
    for (int i = 1, left = 0, right = 0; i < L; i++) {
        if (i > right) {
            left = right = i;
            for (; right < L && str[right - left] == str[right]; right++)
                ;
            Z[i] = right - left;
            right--;
        } else {
            int k = i - left;
            if (Z[k] < right - i + 1) {
                Z[i] = Z[k];
            } else {
                left = i;
                for (; right < L && str[right - left] == str[right]; right++)
                    ;
                Z[i] = right - left;
                right--;
            }
        }
    }
}

class RollingHash {
    int num = 10; // 基数と法の数
    int len;      // 文字列の長さ
    vector<int> base = {1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000};
    const vector<int> mod = {999999797,  999999883,  999999893,  999999929,  999999937,
                             1000000007, 1000000009, 1000000021, 1000000033, 1000000087};
    vector<vector<long long>> hash, power;

  public:
    RollingHash(const string &str, const vector<int> &base_ = vector<int>()) {
        if (base_.size() != 0) base = base_;
        len = str.size();
        rep(i, num) {
            hash.emplace_back(len + 1);
            power.emplace_back(len + 1);
            power[i][0] = 1;
            hash[i][0] = 0;
        }
        repr(i, 1, len + 1) {
            rep(j, num) {
                hash[j][i] = (hash[j][i - 1] * base[j] + str[i - 1]) % mod[j];
                power[j][i] = power[j][i - 1] * base[j] % mod[j];
            }
        }
    }

    // [l, r) のハッシュ値を得る
    inline vector<long long> get_hash(int l, int r) {
        vector<long long> res(num);
        rep(i, num) {
            res[i] = (hash[i][r] - hash[i][l] * power[i][r - l]) % mod[i];
            if (res[i] < 0) res[i] += mod[i];
        }
        return res;
    }

    // str[a:], str[b:] の最長共通接頭辞の長さを O(log|str|) で計算
    inline int lcp(int a, int b) {
        int ok = 0, ng = min(len - a, len - b);
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (get_hash(a, a + mid) == get_hash(b, b + mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        return ok;
    }

    auto get_base() {
        return base;
    }
};
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


#line 2 "library/string_algorithm.hpp"

// str と str[i:] の最長共通接頭辞の長さを O(|str|) で計算
void Z_algorithm(string str, vector<int> &Z) {
    const int L = str.length();
    for (int i = 1, left = 0, right = 0; i < L; i++) {
        if (i > right) {
            left = right = i;
            for (; right < L && str[right - left] == str[right]; right++)
                ;
            Z[i] = right - left;
            right--;
        } else {
            int k = i - left;
            if (Z[k] < right - i + 1) {
                Z[i] = Z[k];
            } else {
                left = i;
                for (; right < L && str[right - left] == str[right]; right++)
                    ;
                Z[i] = right - left;
                right--;
            }
        }
    }
}

class RollingHash {
    int num = 10; // 基数と法の数
    int len;      // 文字列の長さ
    vector<int> base = {1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000};
    const vector<int> mod = {999999797,  999999883,  999999893,  999999929,  999999937,
                             1000000007, 1000000009, 1000000021, 1000000033, 1000000087};
    vector<vector<long long>> hash, power;

  public:
    RollingHash(const string &str, const vector<int> &base_ = vector<int>()) {
        if (base_.size() != 0) base = base_;
        len = str.size();
        rep(i, num) {
            hash.emplace_back(len + 1);
            power.emplace_back(len + 1);
            power[i][0] = 1;
            hash[i][0] = 0;
        }
        repr(i, 1, len + 1) {
            rep(j, num) {
                hash[j][i] = (hash[j][i - 1] * base[j] + str[i - 1]) % mod[j];
                power[j][i] = power[j][i - 1] * base[j] % mod[j];
            }
        }
    }

    // [l, r) のハッシュ値を得る
    inline vector<long long> get_hash(int l, int r) {
        vector<long long> res(num);
        rep(i, num) {
            res[i] = (hash[i][r] - hash[i][l] * power[i][r - l]) % mod[i];
            if (res[i] < 0) res[i] += mod[i];
        }
        return res;
    }

    // str[a:], str[b:] の最長共通接頭辞の長さを O(log|str|) で計算
    inline int lcp(int a, int b) {
        int ok = 0, ng = min(len - a, len - b);
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (get_hash(a, a + mid) == get_hash(b, b + mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        return ok;
    }

    auto get_base() {
        return base;
    }
};

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

