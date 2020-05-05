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


# :warning: library/extgcd_garner.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/extgcd_garner.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 17:08:05+09:00




## Depends on

* :question: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#ifndef LIBRARY_EXTGCD_GARNER_HPP_
#define LIBRARY_EXTGCD_GARNER_HPP_

#include "header.hpp"

// solve ax+by=gcd(a,b)
// return gcd(a,b)
long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    long long x2, y2;
    long long d = ext_gcd(b, a % b, x2, y2);
    x = y2;
    y = x2 - (a / b) * y2;
    return d;
}

long long mod_inv(long long a, long long mod) {
    long long x, y;
    long long d = ext_gcd(a, mod, x, y);
    if (d != 1) {
        // there is no inverse
        return -1;
    }
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// x % m[i] = r[i] % m[i]
// gcd(m[i], m[j])=1 (i!=j) is needed
long long garner(vector<long long> r, vector<long long> m) {
    int n = r.size();
    long long m_prod = 1;      // m_prod means product of m[i]
    long long x = r[0] % m[0]; // initial value meets first condition
    for (int i = 1; i < n; i++) {
        m_prod *= m[i - 1]; // update cumurative product of m
        long long t = ((r[i] - x) * mod_inv(m_prod, m[i])) % m[i];
        if (t < 0) t += m[i]; // for modular of minus
        x += t * m_prod;      // update x
    }
    return x;
}

#endif // LIBRARY_EXTGCD_GARNER_HPP_
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "library/extgcd_garner.hpp"



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


#line 5 "library/extgcd_garner.hpp"

// solve ax+by=gcd(a,b)
// return gcd(a,b)
long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    long long x2, y2;
    long long d = ext_gcd(b, a % b, x2, y2);
    x = y2;
    y = x2 - (a / b) * y2;
    return d;
}

long long mod_inv(long long a, long long mod) {
    long long x, y;
    long long d = ext_gcd(a, mod, x, y);
    if (d != 1) {
        // there is no inverse
        return -1;
    }
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// x % m[i] = r[i] % m[i]
// gcd(m[i], m[j])=1 (i!=j) is needed
long long garner(vector<long long> r, vector<long long> m) {
    int n = r.size();
    long long m_prod = 1;      // m_prod means product of m[i]
    long long x = r[0] % m[0]; // initial value meets first condition
    for (int i = 1; i < n; i++) {
        m_prod *= m[i - 1]; // update cumurative product of m
        long long t = ((r[i] - x) * mod_inv(m_prod, m[i])) % m[i];
        if (t < 0) t += m[i]; // for modular of minus
        x += t * m_prod;      // update x
    }
    return x;
}



```
{% endraw %}

<a href="../../index.html">Back to top page</a>

