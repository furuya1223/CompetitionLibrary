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


# :warning: library/min_cost_flow.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/min_cost_flow.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

// ダイクストラver. O(FElogV)

class MinCostFlow {
    struct flowEdge {
        int to;
        long long capacity, cost;
        int reverse_edge;
        flowEdge(int t, long long cap, long long cos, int rev)
            : to(t), capacity(cap), cost(cos), reverse_edge(rev) {}
    };
    class flowGraph {
        vector<vector<flowEdge>> edges; // グラフの隣接リスト表現
      public:
        flowGraph(int n_) : edges(n_) {}
        flowGraph() {}
        void add_edge(int s, int t, long long cap, long long cos) {
            edges[s].emplace_back(t, cap, cos, (int)edges[t].size());
            edges[t].emplace_back(s, 0, -cos, (int)edges[s].size() - 1);
        }
        vector<flowEdge> &operator[](size_t i) {
            return edges[i];
        }
        int size() const {
            return edges.size();
        }
    };

    flowGraph graph;
    vector<long long> potential, distance;
    vector<int> prev_v, prev_e;

  public:
    MinCostFlow(int n)
        : graph(n), potential(n), distance(n), prev_v(n), prev_e(n) {}

    // s から t へ容量 cap の辺を追加
    void add_edge(int s, int t, long long cap, long long cos) {
        graph.add_edge(s, t, cap, cos);
    }

    // s から t への流量 f の最小費用流を求める。不可なら-1
    int min_cost_flow(int s, int t, int f) {
        int ans = 0;
        fill(potential.begin(), potential.end(), 0);
        while (f > 0) {
            // ダイクストラ法を用いて最短経路を計算
            using P_LL_I = pair<long long, int>;
            priority_queue<P_LL_I, vector<P_LL_I>, greater<P_LL_I>> que;
            fill(distance.begin(), distance.end(), INFL);
            distance[s] = 0;
            que.push(mp(0, s));
            while (!que.empty()) {
                long long dist;
                int v;
                tie(dist, v) = que.top();
                que.pop();
                if (distance[v] < dist) continue;
                for (int i = 0; i < graph[v].size(); i++) {
                    flowEdge &e = graph[v][i];
                    if (e.capacity > 0 &&
                        distance[e.to] > distance[v] + e.cost + potential[v] -
                                             potential[e.to]) {
                        distance[e.to] = distance[v] + e.cost + potential[v] -
                                         potential[e.to];
                        prev_v[e.to] = v;
                        prev_e[e.to] = i;
                        que.push(mp(distance[e.to], e.to));
                    }
                }
            }
            if (distance[t] == INFL) {
                // これ以上流せない
                return -1;
            }
            // potential を更新
            for (int v = 0; v < graph.size(); v++) potential[v] += distance[v];

            // s-t 最短経路のボトルネック流量の計算
            long long bottleneck = f;
            for (int v = t; v != s; v = prev_v[v]) {
                bottleneck =
                    min(bottleneck, graph[prev_v[v]][prev_e[v]].capacity);
            }
            // s-t 最短経路に沿ってボトルネック流量を流す
            f -= bottleneck;
            ans += bottleneck * potential[t];
            for (int v = t; v != s; v = prev_v[v]) {
                flowEdge &e = graph[prev_v[v]][prev_e[v]];
                e.capacity -= bottleneck;
                graph[v][e.reverse_edge].capacity += bottleneck;
            }
        }
        return ans;
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


#line 2 "library/min_cost_flow.hpp"

// ダイクストラver. O(FElogV)

class MinCostFlow {
    struct flowEdge {
        int to;
        long long capacity, cost;
        int reverse_edge;
        flowEdge(int t, long long cap, long long cos, int rev)
            : to(t), capacity(cap), cost(cos), reverse_edge(rev) {}
    };
    class flowGraph {
        vector<vector<flowEdge>> edges; // グラフの隣接リスト表現
      public:
        flowGraph(int n_) : edges(n_) {}
        flowGraph() {}
        void add_edge(int s, int t, long long cap, long long cos) {
            edges[s].emplace_back(t, cap, cos, (int)edges[t].size());
            edges[t].emplace_back(s, 0, -cos, (int)edges[s].size() - 1);
        }
        vector<flowEdge> &operator[](size_t i) {
            return edges[i];
        }
        int size() const {
            return edges.size();
        }
    };

    flowGraph graph;
    vector<long long> potential, distance;
    vector<int> prev_v, prev_e;

  public:
    MinCostFlow(int n)
        : graph(n), potential(n), distance(n), prev_v(n), prev_e(n) {}

    // s から t へ容量 cap の辺を追加
    void add_edge(int s, int t, long long cap, long long cos) {
        graph.add_edge(s, t, cap, cos);
    }

    // s から t への流量 f の最小費用流を求める。不可なら-1
    int min_cost_flow(int s, int t, int f) {
        int ans = 0;
        fill(potential.begin(), potential.end(), 0);
        while (f > 0) {
            // ダイクストラ法を用いて最短経路を計算
            using P_LL_I = pair<long long, int>;
            priority_queue<P_LL_I, vector<P_LL_I>, greater<P_LL_I>> que;
            fill(distance.begin(), distance.end(), INFL);
            distance[s] = 0;
            que.push(mp(0, s));
            while (!que.empty()) {
                long long dist;
                int v;
                tie(dist, v) = que.top();
                que.pop();
                if (distance[v] < dist) continue;
                for (int i = 0; i < graph[v].size(); i++) {
                    flowEdge &e = graph[v][i];
                    if (e.capacity > 0 &&
                        distance[e.to] > distance[v] + e.cost + potential[v] -
                                             potential[e.to]) {
                        distance[e.to] = distance[v] + e.cost + potential[v] -
                                         potential[e.to];
                        prev_v[e.to] = v;
                        prev_e[e.to] = i;
                        que.push(mp(distance[e.to], e.to));
                    }
                }
            }
            if (distance[t] == INFL) {
                // これ以上流せない
                return -1;
            }
            // potential を更新
            for (int v = 0; v < graph.size(); v++) potential[v] += distance[v];

            // s-t 最短経路のボトルネック流量の計算
            long long bottleneck = f;
            for (int v = t; v != s; v = prev_v[v]) {
                bottleneck =
                    min(bottleneck, graph[prev_v[v]][prev_e[v]].capacity);
            }
            // s-t 最短経路に沿ってボトルネック流量を流す
            f -= bottleneck;
            ans += bottleneck * potential[t];
            for (int v = t; v != s; v = prev_v[v]) {
                flowEdge &e = graph[prev_v[v]][prev_e[v]];
                e.capacity -= bottleneck;
                graph[v][e.reverse_edge].capacity += bottleneck;
            }
        }
        return ans;
    }
};

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

