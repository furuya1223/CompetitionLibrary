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


# :warning: library/aug_dijkstra.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/aug_dijkstra.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

template <class T, class C = int>
struct augEdge {
    T from, to;
    C cost;
    bool operator<(augEdge e) {
        return cost < e.cost;
    }
    bool operator>(augEdge e) {
        return cost > e.cost;
    }
};
template <class T, class C = int>
class augGraph {
    int n;
    bool directed;
    map<T, vector<augEdge<T, C>>> edges;

  public:
    augGraph(bool directed)
        : directed(directed), edges(map<T, vector<augEdge<T, C>>>()) {}

    void add_edge(T s, T t, C cost) {
        edges[s].push_back(augEdge<T, C>{s, t, cost});
        if (!directed) {
            edges[t].push_back(augEdge<T, C>{t, s, cost});
        }
    }
    augGraph() {}

    vector<augEdge<T, C>> operator[](T key) {
        if (edges.find(key) == edges.end()) {
            return vector<augEdge<T, C>>();
        }
        return edges[key];
    }
};

template <class T, class C>
void augDijkstra(augGraph<T, C> graph, T start, map<T, C> &cost) {
    // vi prev(V);
    pql<pair<C, T>> Q;

    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        T pos = Q.top().second;
        C d = Q.top().first;
        Q.pop();
        rep(i, graph[pos].size()) {
            T to = graph[pos][i].to;
            C c = graph[pos][i].cost;
            if (cost.find(to) == cost.end() || cost[to] > cost[pos] + c) {
                cost[to] = cost[pos] + c;
                Q.push(mp(cost[to], to));
                // prev[graph[pos][i].to] = pos;
            }
        }
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    int N, M;
    cin >> N >> M;

    map<Pi, vector<augEdge<Pi>>> G;

    rep(i, M) {
        int p, q, c;
        cin >> p >> q >> c;
        G[mp(p, 0)].push_back(augEdge<Pi>{mp(p, 0), mp(p, c), 1});
        G[mp(p, c)].push_back(augEdge<Pi>{mp(p, c), mp(p, 0), 1});
        G[mp(q, 0)].push_back(augEdge<Pi>{mp(q, 0), mp(q, c), 1});
        G[mp(q, c)].push_back(augEdge<Pi>{mp(q, c), mp(q, 0), 1});
        G[mp(p, c)].push_back(augEdge<Pi>{mp(p, c), mp(q, c), 0});
        G[mp(q, c)].push_back(augEdge<Pi>{mp(q, c), mp(p, c), 0});
    }

    map<Pi, int> cost;

    augDijkstra<Pi>(G, mp(1, 0), cost);

    if (cost.find(mp(N, 0)) == cost.end())
        cout << -1 << endl;
    else
        cout << (cost[mp(N, 0)] - 1) / 2 + 1 << endl;

    return 0;
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


#line 2 "library/aug_dijkstra.hpp"

template <class T, class C = int>
struct augEdge {
    T from, to;
    C cost;
    bool operator<(augEdge e) {
        return cost < e.cost;
    }
    bool operator>(augEdge e) {
        return cost > e.cost;
    }
};
template <class T, class C = int>
class augGraph {
    int n;
    bool directed;
    map<T, vector<augEdge<T, C>>> edges;

  public:
    augGraph(bool directed)
        : directed(directed), edges(map<T, vector<augEdge<T, C>>>()) {}

    void add_edge(T s, T t, C cost) {
        edges[s].push_back(augEdge<T, C>{s, t, cost});
        if (!directed) {
            edges[t].push_back(augEdge<T, C>{t, s, cost});
        }
    }
    augGraph() {}

    vector<augEdge<T, C>> operator[](T key) {
        if (edges.find(key) == edges.end()) {
            return vector<augEdge<T, C>>();
        }
        return edges[key];
    }
};

template <class T, class C>
void augDijkstra(augGraph<T, C> graph, T start, map<T, C> &cost) {
    // vi prev(V);
    pql<pair<C, T>> Q;

    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        T pos = Q.top().second;
        C d = Q.top().first;
        Q.pop();
        rep(i, graph[pos].size()) {
            T to = graph[pos][i].to;
            C c = graph[pos][i].cost;
            if (cost.find(to) == cost.end() || cost[to] > cost[pos] + c) {
                cost[to] = cost[pos] + c;
                Q.push(mp(cost[to], to));
                // prev[graph[pos][i].to] = pos;
            }
        }
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    int N, M;
    cin >> N >> M;

    map<Pi, vector<augEdge<Pi>>> G;

    rep(i, M) {
        int p, q, c;
        cin >> p >> q >> c;
        G[mp(p, 0)].push_back(augEdge<Pi>{mp(p, 0), mp(p, c), 1});
        G[mp(p, c)].push_back(augEdge<Pi>{mp(p, c), mp(p, 0), 1});
        G[mp(q, 0)].push_back(augEdge<Pi>{mp(q, 0), mp(q, c), 1});
        G[mp(q, c)].push_back(augEdge<Pi>{mp(q, c), mp(q, 0), 1});
        G[mp(p, c)].push_back(augEdge<Pi>{mp(p, c), mp(q, c), 0});
        G[mp(q, c)].push_back(augEdge<Pi>{mp(q, c), mp(p, c), 0});
    }

    map<Pi, int> cost;

    augDijkstra<Pi>(G, mp(1, 0), cost);

    if (cost.find(mp(N, 0)) == cost.end())
        cout << -1 << endl;
    else
        cout << (cost[mp(N, 0)] - 1) / 2 + 1 << endl;

    return 0;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

