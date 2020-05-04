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


# :warning: library/max_flow.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/max_flow.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

// Dinic 法による最大フロー O(V^2 * E) だが高速
class MaxFlow {
    struct flowEdge {
        int to;             // 行き先
        long long capacity; // 容量
        int reverse_edge;   // 逆辺の index
        flowEdge(int t, long long c, int r)
            : to(t), capacity(c), reverse_edge(r) {}
    };
    class flowGraph {
        vector<vector<flowEdge>> edges; // グラフの隣接リスト表現
      public:
        flowGraph(int n_) : edges(n_) {}
        flowGraph() {}
        void add_edge(int s, int t, long long cap) {
            edges[s].emplace_back(t, cap, (int)edges[t].size());
            edges[t].emplace_back(s, 0, (int)edges[s].size() - 1);
        }
        vector<flowEdge> &operator[](size_t i) {
            return edges[i];
        }
        int size() const {
            return edges.size();
        }
    };

    flowGraph graph;
    vector<int> level;
    vector<int> iter;

    // sからの最短距離をBFSで計算
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const auto &e : graph[v]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // 増加パスをDFSで探す（v に着目、終点は t, 現在の最小流量は f）
    long long dfs(int v, int t, long long f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < graph[v].size(); i++) {
            flowEdge &e = graph[v][i];
            if (e.capacity > 0 && level[v] < level[e.to]) {
                long long d = dfs(e.to, t, min(f, e.capacity));
                if (d > 0) {
                    e.capacity -= d;
                    graph[e.to][e.reverse_edge].capacity += d;
                    return d;
                }
            }
        }
        return 0;
    }

  public:
    MaxFlow(int n) : graph(n), level(n), iter(n) {}

    // s から t へ容量 cap の辺を追加
    void add_edge(int s, int t, long long cap) {
        graph.add_edge(s, t, cap);
    }

    // sからtへの最大流を求める
    long long max_flow(int s, int t) {
        long long flow = 0;
        while (true) {
            bfs(s);
            // t へ到達不可能になったら出力
            if (level[t] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            long long increase;
            while ((increase = dfs(s, t, INFL)) > 0) {
                flow += increase;
            }
        }
    }
};

class ProjectSelection {
    int num;
    MaxFlow max_flow; // 頂点数は num+2, 始点 num, 終点 num+1
    long long sum_reward = 0;

  public:
    ProjectSelection(vector<long long> rewards)
        : num(rewards.size()), max_flow(rewards.size() + 2) {
        for (int i = 0; i < num; i++) {
            if (rewards[i] > 0) {
                max_flow.add_edge(num, i, rewards[i]);
                sum_reward += rewards[i];
            } else {
                max_flow.add_edge(i, num + 1, -rewards[i]);
            }
        }
    }

    // i を選択し、j を選択しないと penalty だけ損する
    void add_penalty(int i, int j, long long penalty) {
        max_flow.add_edge(i, j, penalty);
    }

    // 最大の利得を計算
    long long max_reward() {
        return sum_reward - max_flow.max_flow(num, num + 1);
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


#line 2 "library/max_flow.hpp"

// Dinic 法による最大フロー O(V^2 * E) だが高速
class MaxFlow {
    struct flowEdge {
        int to;             // 行き先
        long long capacity; // 容量
        int reverse_edge;   // 逆辺の index
        flowEdge(int t, long long c, int r)
            : to(t), capacity(c), reverse_edge(r) {}
    };
    class flowGraph {
        vector<vector<flowEdge>> edges; // グラフの隣接リスト表現
      public:
        flowGraph(int n_) : edges(n_) {}
        flowGraph() {}
        void add_edge(int s, int t, long long cap) {
            edges[s].emplace_back(t, cap, (int)edges[t].size());
            edges[t].emplace_back(s, 0, (int)edges[s].size() - 1);
        }
        vector<flowEdge> &operator[](size_t i) {
            return edges[i];
        }
        int size() const {
            return edges.size();
        }
    };

    flowGraph graph;
    vector<int> level;
    vector<int> iter;

    // sからの最短距離をBFSで計算
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const auto &e : graph[v]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // 増加パスをDFSで探す（v に着目、終点は t, 現在の最小流量は f）
    long long dfs(int v, int t, long long f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < graph[v].size(); i++) {
            flowEdge &e = graph[v][i];
            if (e.capacity > 0 && level[v] < level[e.to]) {
                long long d = dfs(e.to, t, min(f, e.capacity));
                if (d > 0) {
                    e.capacity -= d;
                    graph[e.to][e.reverse_edge].capacity += d;
                    return d;
                }
            }
        }
        return 0;
    }

  public:
    MaxFlow(int n) : graph(n), level(n), iter(n) {}

    // s から t へ容量 cap の辺を追加
    void add_edge(int s, int t, long long cap) {
        graph.add_edge(s, t, cap);
    }

    // sからtへの最大流を求める
    long long max_flow(int s, int t) {
        long long flow = 0;
        while (true) {
            bfs(s);
            // t へ到達不可能になったら出力
            if (level[t] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            long long increase;
            while ((increase = dfs(s, t, INFL)) > 0) {
                flow += increase;
            }
        }
    }
};

class ProjectSelection {
    int num;
    MaxFlow max_flow; // 頂点数は num+2, 始点 num, 終点 num+1
    long long sum_reward = 0;

  public:
    ProjectSelection(vector<long long> rewards)
        : num(rewards.size()), max_flow(rewards.size() + 2) {
        for (int i = 0; i < num; i++) {
            if (rewards[i] > 0) {
                max_flow.add_edge(num, i, rewards[i]);
                sum_reward += rewards[i];
            } else {
                max_flow.add_edge(i, num + 1, -rewards[i]);
            }
        }
    }

    // i を選択し、j を選択しないと penalty だけ損する
    void add_penalty(int i, int j, long long penalty) {
        max_flow.add_edge(i, j, penalty);
    }

    // 最大の利得を計算
    long long max_reward() {
        return sum_reward - max_flow.max_flow(num, num + 1);
    }
};

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

