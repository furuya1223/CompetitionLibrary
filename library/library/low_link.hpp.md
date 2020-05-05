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


# :warning: library/low_link.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/low_link.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :question: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

class Lowlink {
    void dfs(const Graph<int> &G, int v, int p, int &k) {
        vis[v] = true;

        ord[v] = k++;
        low[v] = ord[v];

        bool isArticulation = false;
        int ct = 0;

        for (int i = 0; i < G[v].size(); i++) {
            if (!vis[G[v][i].to]) {
                ct++;
                dfs(G, G[v][i].to, v, k);
                low[v] = min(low[v], low[G[v][i].to]);
                if (p != -1 && ord[v] <= low[G[v][i].to]) isArticulation = true;
                if (ord[v] < low[G[v][i].to]) {
                    bridge.push_back(
                        make_pair(min(v, G[v][i].to), max(v, G[v][i].to)));
                    bridgeSet.insert(
                        make_pair(min(v, G[v][i].to), max(v, G[v][i].to)));
                }
            } else if (G[v][i].to != p) {
                low[v] = min(low[v], ord[G[v][i].to]);
            }
        }

        if (p == -1 && ct > 1) isArticulation = true;
        if (isArticulation) articulation.push_back(v);
    }

    void bccdfs(const Graph<int> &G, int pos, int par, int k) {
        vis[pos] = true;
        n2g[pos] = k;
        component[k].push_back(pos);
        for (auto e : G[pos]) {
            if (vis[e.to]) continue;
            if (bridgeSet.find(mp(min(e.from, e.to), max(e.from, e.to))) !=
                bridgeSet.end())
                continue;
            bccdfs(G, e.to, pos, k);
        }
    }

  public:
    vi ord, low;
    vb vis;
    vi articulation;
    vPi bridge;
    Graph<int> BCCtree;
    vi n2g;
    vvi component;
    set<Pi> bridgeSet;

    Lowlink(const Graph<int> &G) {
        ord = vi(G.size());
        low = vi(G.size());
        vis = vb(G.size(), false);
        int k = 0;
        for (int i = 0; i < G.size(); i++) {
            if (!vis[i]) dfs(G, i, -1, k);
        }
    }

    void bcc(const Graph<int> &G) {
        if (bridge.size() == 0) {
            component = vvi(1);
            n2g = vi(G.size());
            rep(i, G.size()) {
                component[0].push_back(i);
                n2g[i] = 0;
            }
            BCCtree = Graph<int>(1);
            return;
        }
        int k = 0;
        n2g = vi(G.size(), -1);
        vis = vb(G.size(), false);
        component = vvi(G.size());

        for (Pi e : bridge) {
            if (!vis[e.first]) {
                bccdfs(G, e.first, -1, k);
                k++;
            }
            if (!vis[e.second]) {
                bccdfs(G, e.second, -1, k);
                k++;
            }
        }
        BCCtree = Graph<int>(k);
        component.resize(k);
        for (Pi e : bridge) {
            int a = n2g[e.first];
            int b = n2g[e.second];
            BCCtree[a].push_back(Edge<int>{a, b, 1});
            BCCtree[b].push_back(Edge<int>{b, a, 1});
        }
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


#line 2 "library/low_link.hpp"

class Lowlink {
    void dfs(const Graph<int> &G, int v, int p, int &k) {
        vis[v] = true;

        ord[v] = k++;
        low[v] = ord[v];

        bool isArticulation = false;
        int ct = 0;

        for (int i = 0; i < G[v].size(); i++) {
            if (!vis[G[v][i].to]) {
                ct++;
                dfs(G, G[v][i].to, v, k);
                low[v] = min(low[v], low[G[v][i].to]);
                if (p != -1 && ord[v] <= low[G[v][i].to]) isArticulation = true;
                if (ord[v] < low[G[v][i].to]) {
                    bridge.push_back(
                        make_pair(min(v, G[v][i].to), max(v, G[v][i].to)));
                    bridgeSet.insert(
                        make_pair(min(v, G[v][i].to), max(v, G[v][i].to)));
                }
            } else if (G[v][i].to != p) {
                low[v] = min(low[v], ord[G[v][i].to]);
            }
        }

        if (p == -1 && ct > 1) isArticulation = true;
        if (isArticulation) articulation.push_back(v);
    }

    void bccdfs(const Graph<int> &G, int pos, int par, int k) {
        vis[pos] = true;
        n2g[pos] = k;
        component[k].push_back(pos);
        for (auto e : G[pos]) {
            if (vis[e.to]) continue;
            if (bridgeSet.find(mp(min(e.from, e.to), max(e.from, e.to))) !=
                bridgeSet.end())
                continue;
            bccdfs(G, e.to, pos, k);
        }
    }

  public:
    vi ord, low;
    vb vis;
    vi articulation;
    vPi bridge;
    Graph<int> BCCtree;
    vi n2g;
    vvi component;
    set<Pi> bridgeSet;

    Lowlink(const Graph<int> &G) {
        ord = vi(G.size());
        low = vi(G.size());
        vis = vb(G.size(), false);
        int k = 0;
        for (int i = 0; i < G.size(); i++) {
            if (!vis[i]) dfs(G, i, -1, k);
        }
    }

    void bcc(const Graph<int> &G) {
        if (bridge.size() == 0) {
            component = vvi(1);
            n2g = vi(G.size());
            rep(i, G.size()) {
                component[0].push_back(i);
                n2g[i] = 0;
            }
            BCCtree = Graph<int>(1);
            return;
        }
        int k = 0;
        n2g = vi(G.size(), -1);
        vis = vb(G.size(), false);
        component = vvi(G.size());

        for (Pi e : bridge) {
            if (!vis[e.first]) {
                bccdfs(G, e.first, -1, k);
                k++;
            }
            if (!vis[e.second]) {
                bccdfs(G, e.second, -1, k);
                k++;
            }
        }
        BCCtree = Graph<int>(k);
        component.resize(k);
        for (Pi e : bridge) {
            int a = n2g[e.first];
            int b = n2g[e.second];
            BCCtree[a].push_back(Edge<int>{a, b, 1});
            BCCtree[b].push_back(Edge<int>{b, a, 1});
        }
    }
};

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

