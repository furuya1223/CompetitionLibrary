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


# :warning: library/graph.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/graph.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-05 14:06:25+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

vector<int> Centroid(const Graph<int> &G) {
    int n = G.size();
    vector<int> centroid;
    vector<int> sz(n);
    function<void(int, int)> dfs = [&](int u, int prev) {
        sz[u] = 1;
        bool is_centroid = true;
        for (auto v : G[u])
            if (v.to != prev) {
                dfs(v.to, u);
                sz[u] += sz[v.to];
                if (sz[v.to] > n / 2) is_centroid = false;
            }
        if (n - sz[u] > n / 2) is_centroid = false;
        if (is_centroid) centroid.push_back(u);
    };
    dfs(0, -1);
    return centroid;
}

ll Chi_Liu_Edmonds(vector<Edge<int>> edges, int V, int start) {
    vector<Pi> mins(V, mp(INF, -1));
    rep(i, edges.size()) {
        Edge<int> edge = edges[i];
        mins[edge.to] = min(mins[edge.to], mp(edge.cost, edge.from));
    }
    mins[start] = mp(-1, -1);

    vector<int> group(V, 0);
    vector<bool> isCycle(V, false);
    int count = 0;

    vector<bool> used(V, false);
    rep(i, V) {
        if (used[i]) continue;
        vector<int> chain;
        int cursor = i;
        while (cursor != -1 && !used[cursor]) {
            used[cursor] = true;
            chain.push_back(cursor);
            cursor = mins[cursor].second;
        }
        if (cursor != -1) {
            bool inCycle = false;
            rep(j, chain.size()) {
                group[chain[j]] = count;
                if (chain[j] == cursor) {
                    isCycle[count] = true;
                    inCycle = true;
                }
                if (!inCycle) count++;
            }
            if (inCycle) count++;
        } else {
            rep(j, chain.size()) {
                group[chain[j]] = count;
                count++;
            }
        }
    }

    if (count == V) {
        ll ans = 1;
        rep(i, V) {
            ans += mins[i].first;
        }
        return ans;
    }

    ll res = 0;
    rep(i, V) {
        if (i != start && isCycle[group[i]]) {
            res += mins[i].first;
        }
    }

    vector<Edge<int>> newEdges;
    rep(i, edges.size()) {
        Edge<int> edge = edges[i];
        int to = edge.to;
        int gfrom = group[edge.from];
        int gto = group[edge.to];
        if (gfrom == gto) {
            continue;
        } else if (isCycle[gto]) {
            newEdges.push_back(Edge<int>(gfrom, gto, edge.cost - mins[to].first));
        } else {
            newEdges.push_back(Edge<int>(gfrom, gto, edge.cost));
        }
    }

    return res + Chi_Liu_Edmonds(newEdges, count, group[start]);
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


#line 2 "library/graph.hpp"

vector<int> Centroid(const Graph<int> &G) {
    int n = G.size();
    vector<int> centroid;
    vector<int> sz(n);
    function<void(int, int)> dfs = [&](int u, int prev) {
        sz[u] = 1;
        bool is_centroid = true;
        for (auto v : G[u])
            if (v.to != prev) {
                dfs(v.to, u);
                sz[u] += sz[v.to];
                if (sz[v.to] > n / 2) is_centroid = false;
            }
        if (n - sz[u] > n / 2) is_centroid = false;
        if (is_centroid) centroid.push_back(u);
    };
    dfs(0, -1);
    return centroid;
}

ll Chi_Liu_Edmonds(vector<Edge<int>> edges, int V, int start) {
    vector<Pi> mins(V, mp(INF, -1));
    rep(i, edges.size()) {
        Edge<int> edge = edges[i];
        mins[edge.to] = min(mins[edge.to], mp(edge.cost, edge.from));
    }
    mins[start] = mp(-1, -1);

    vector<int> group(V, 0);
    vector<bool> isCycle(V, false);
    int count = 0;

    vector<bool> used(V, false);
    rep(i, V) {
        if (used[i]) continue;
        vector<int> chain;
        int cursor = i;
        while (cursor != -1 && !used[cursor]) {
            used[cursor] = true;
            chain.push_back(cursor);
            cursor = mins[cursor].second;
        }
        if (cursor != -1) {
            bool inCycle = false;
            rep(j, chain.size()) {
                group[chain[j]] = count;
                if (chain[j] == cursor) {
                    isCycle[count] = true;
                    inCycle = true;
                }
                if (!inCycle) count++;
            }
            if (inCycle) count++;
        } else {
            rep(j, chain.size()) {
                group[chain[j]] = count;
                count++;
            }
        }
    }

    if (count == V) {
        ll ans = 1;
        rep(i, V) {
            ans += mins[i].first;
        }
        return ans;
    }

    ll res = 0;
    rep(i, V) {
        if (i != start && isCycle[group[i]]) {
            res += mins[i].first;
        }
    }

    vector<Edge<int>> newEdges;
    rep(i, edges.size()) {
        Edge<int> edge = edges[i];
        int to = edge.to;
        int gfrom = group[edge.from];
        int gto = group[edge.to];
        if (gfrom == gto) {
            continue;
        } else if (isCycle[gto]) {
            newEdges.push_back(Edge<int>(gfrom, gto, edge.cost - mins[to].first));
        } else {
            newEdges.push_back(Edge<int>(gfrom, gto, edge.cost));
        }
    }

    return res + Chi_Liu_Edmonds(newEdges, count, group[start]);
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

