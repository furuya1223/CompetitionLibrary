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


# :warning: library/mod_int.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/mod_int.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :question: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

template <uint_fast64_t Mod>
class mint {
    using u64 = uint_fast64_t;
    u64 value;

  public:
    constexpr mint(long long value = 0) noexcept : value(value % Mod) {}

    u64 &get() noexcept {
        return value;
    }
    constexpr const u64 &get() const noexcept {
        return value;
    }
    constexpr mint operator+(const mint &rhs) const noexcept {
        return mint(*this) += rhs;
    }
    constexpr mint operator-(const mint &rhs) const noexcept {
        return mint(*this) -= rhs;
    }
    constexpr mint operator*(const mint &rhs) const noexcept {
        return mint(*this) *= rhs;
    }
    constexpr mint operator/(const mint &rhs) const noexcept {
        return mint(*this) /= rhs;
    }
    mint &operator+=(const mint &rhs) noexcept {
        value += rhs.value;
        if (value >= Mod) {
            value -= Mod;
        }
        return *this;
    }
    mint &operator-=(const mint &rhs) noexcept {
        if (value < rhs.value) {
            value += Mod;
        }
        value -= rhs.value;
        return *this;
    }
    mint &operator*=(const mint &rhs) noexcept {
        value = value * rhs.value % Mod;
        return *this;
    }
    mint &operator/=(mint rhs) noexcept {
        u64 exp = Mod - 2;
        while (exp) {
            if (exp % 2 == 1) {
                *this *= rhs;
            }
            rhs *= rhs;
            exp /= 2;
        }
        return *this;
    }
    mint pow(u64 n) const noexcept {
        mint ret(1), p(*this);
        while (n > 0) {
            if (n & 1) ret *= p;
            p *= p;
            n >>= 1;
        }
        return ret;
    }
};
template <class Char, class Traits, uint_fast64_t Mod>
basic_ostream<Char, Traits> &operator<<(basic_ostream<Char, Traits> &os,
                                        const mint<Mod> &modint) {
    os << modint.get();
    return os;
}
template <class Char, class Traits, uint_fast64_t Mod>
std::basic_istream<Char, Traits> &operator>>(basic_istream<Char, Traits> &is,
                                             mint<Mod> &modint) {
    is >> modint.get();
    return is;
}
template <uint_fast64_t Mod>
constexpr bool operator==(const mint<Mod> &lhs, const mint<Mod> &rhs) {
    return lhs.get() == rhs.get();
}
template <uint_fast64_t Mod>
constexpr bool operator!=(const mint<Mod> &lhs, const mint<Mod> &rhs) {
    return lhs != rhs;
}
template <uint_fast64_t Mod>
constexpr bool operator==(const long long lhs, const mint<Mod> &rhs) {
    return lhs == rhs.get();
}
template <uint_fast64_t Mod>
constexpr bool operator!=(const long long lhs, const mint<Mod> &rhs) {
    return lhs != rhs.get();
}
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


#line 2 "library/mod_int.hpp"

template <uint_fast64_t Mod>
class mint {
    using u64 = uint_fast64_t;
    u64 value;

  public:
    constexpr mint(long long value = 0) noexcept : value(value % Mod) {}

    u64 &get() noexcept {
        return value;
    }
    constexpr const u64 &get() const noexcept {
        return value;
    }
    constexpr mint operator+(const mint &rhs) const noexcept {
        return mint(*this) += rhs;
    }
    constexpr mint operator-(const mint &rhs) const noexcept {
        return mint(*this) -= rhs;
    }
    constexpr mint operator*(const mint &rhs) const noexcept {
        return mint(*this) *= rhs;
    }
    constexpr mint operator/(const mint &rhs) const noexcept {
        return mint(*this) /= rhs;
    }
    mint &operator+=(const mint &rhs) noexcept {
        value += rhs.value;
        if (value >= Mod) {
            value -= Mod;
        }
        return *this;
    }
    mint &operator-=(const mint &rhs) noexcept {
        if (value < rhs.value) {
            value += Mod;
        }
        value -= rhs.value;
        return *this;
    }
    mint &operator*=(const mint &rhs) noexcept {
        value = value * rhs.value % Mod;
        return *this;
    }
    mint &operator/=(mint rhs) noexcept {
        u64 exp = Mod - 2;
        while (exp) {
            if (exp % 2 == 1) {
                *this *= rhs;
            }
            rhs *= rhs;
            exp /= 2;
        }
        return *this;
    }
    mint pow(u64 n) const noexcept {
        mint ret(1), p(*this);
        while (n > 0) {
            if (n & 1) ret *= p;
            p *= p;
            n >>= 1;
        }
        return ret;
    }
};
template <class Char, class Traits, uint_fast64_t Mod>
basic_ostream<Char, Traits> &operator<<(basic_ostream<Char, Traits> &os,
                                        const mint<Mod> &modint) {
    os << modint.get();
    return os;
}
template <class Char, class Traits, uint_fast64_t Mod>
std::basic_istream<Char, Traits> &operator>>(basic_istream<Char, Traits> &is,
                                             mint<Mod> &modint) {
    is >> modint.get();
    return is;
}
template <uint_fast64_t Mod>
constexpr bool operator==(const mint<Mod> &lhs, const mint<Mod> &rhs) {
    return lhs.get() == rhs.get();
}
template <uint_fast64_t Mod>
constexpr bool operator!=(const mint<Mod> &lhs, const mint<Mod> &rhs) {
    return lhs != rhs;
}
template <uint_fast64_t Mod>
constexpr bool operator==(const long long lhs, const mint<Mod> &rhs) {
    return lhs == rhs.get();
}
template <uint_fast64_t Mod>
constexpr bool operator!=(const long long lhs, const mint<Mod> &rhs) {
    return lhs != rhs.get();
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

