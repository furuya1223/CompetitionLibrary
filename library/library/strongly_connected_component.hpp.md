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


# :warning: library/strongly_connected_component.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/strongly_connected_component.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :question: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

class SCCD {
  public:
    stack<int> post;
    vb used;

    inline void dfs(int pos, int par, Graph<int> &g) {
        used[pos] = true;
        for (const auto &e : g[pos]) {
            if (used[e.to]) continue;
            dfs(e.to, pos, g);
        }
        post.push(pos);
    }

    inline void dfsrev(int pos, int par, Graph<int> &rev, vi &group) {
        used[pos] = true;
        group.push_back(pos);
        for (const auto &e : rev[pos]) {
            if (used[e.to]) continue;
            dfsrev(e.to, pos, rev, group);
        }
    }

    Graph<int> SCC(Graph<int> &G, vvi &scc, vi &n2g, Graph<int> &dag) {
        int n = G.size();
        Graph<int> rev(n, true);
        // 逆辺グラフの作成
        rep(i, n) {
            rep(j, G[i].size()) {
                auto e = G[i][j];
                rev.add_edge(e.to, e.from, 1);
            }
        }

        // 1 回目の DFS
        used = vb(n, false);
        rep(i, n) {
            if (!used[i]) dfs(i, -1, G);
        }

        // 2 回目の DFS
        fill(all(used), false);
        while (!post.empty()) {
            int pos = post.top();
            post.pop();
            if (used[pos]) continue;
            vi group;
            dfsrev(pos, -1, rev, group);
            scc.push_back(group);
        }

        // 頂点番号から強連結成分の番号を得る辞書の作成
        rep(i, scc.size()) {
            rep(j, scc[i].size()) {
                n2g[scc[i][j]] = i;
            }
        }

        dag = Graph<int>(scc.size(), true);
        set<pair<int, int>> usedEdges;

        // DAG を作成
        rep(i, n) {
            for (const auto &e : G[i]) {
                int s = n2g[e.from];
                int t = n2g[e.to];
                if (usedEdges.find(mp(s, t)) == usedEdges.end() && s != t) {
                    dag.add_edge(s, t, 1);
                    usedEdges.insert(mp(s, t));
                }
            }
        }
        return dag;
    }
};

class SAT {
    bool solved = false;

  public:
    int num;
    Graph<int> graph, dag;
    vvi scc;
    vi n2g;

    SAT(int n) : num(n), graph(2 * n, true), n2g(2 * num) {}

    void add_constraint(bool x_is_true, int x, bool y_is_true, int y) {
        assert(0 <= x && x < num && 0 <= y && y < num);
        if (!x_is_true) x += num;
        if (!y_is_true) y += num;
        graph.add_edge((x + num) % (2 * num), y, 1); // not x -> y
        graph.add_edge((y + num) % (2 * num), x, 1); // not y -> x
    }

    void solve() {
        SCCD sccd;
        sccd.SCC(graph, scc, n2g, dag);
        solved = true;
    }

    bool get_satisfiability() {
        if (!solved) solve();
        for (int i = 0; i < num; i++) {
            if (n2g[i] == n2g[i + num]) return false;
        }
        return true;
    }

    bool get_assignment(vector<bool> &assignment) {
        assignment.resize(num);
        if (!solved) solve();
        vector<bool> ret(num);
        for (int i = 0; i < num; i++) {
            if (n2g[i] == n2g[i + num]) return false;
            assignment[i] = n2g[i] > n2g[i + num];
        }
        return true;
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


#line 2 "library/strongly_connected_component.hpp"

class SCCD {
  public:
    stack<int> post;
    vb used;

    inline void dfs(int pos, int par, Graph<int> &g) {
        used[pos] = true;
        for (const auto &e : g[pos]) {
            if (used[e.to]) continue;
            dfs(e.to, pos, g);
        }
        post.push(pos);
    }

    inline void dfsrev(int pos, int par, Graph<int> &rev, vi &group) {
        used[pos] = true;
        group.push_back(pos);
        for (const auto &e : rev[pos]) {
            if (used[e.to]) continue;
            dfsrev(e.to, pos, rev, group);
        }
    }

    Graph<int> SCC(Graph<int> &G, vvi &scc, vi &n2g, Graph<int> &dag) {
        int n = G.size();
        Graph<int> rev(n, true);
        // 逆辺グラフの作成
        rep(i, n) {
            rep(j, G[i].size()) {
                auto e = G[i][j];
                rev.add_edge(e.to, e.from, 1);
            }
        }

        // 1 回目の DFS
        used = vb(n, false);
        rep(i, n) {
            if (!used[i]) dfs(i, -1, G);
        }

        // 2 回目の DFS
        fill(all(used), false);
        while (!post.empty()) {
            int pos = post.top();
            post.pop();
            if (used[pos]) continue;
            vi group;
            dfsrev(pos, -1, rev, group);
            scc.push_back(group);
        }

        // 頂点番号から強連結成分の番号を得る辞書の作成
        rep(i, scc.size()) {
            rep(j, scc[i].size()) {
                n2g[scc[i][j]] = i;
            }
        }

        dag = Graph<int>(scc.size(), true);
        set<pair<int, int>> usedEdges;

        // DAG を作成
        rep(i, n) {
            for (const auto &e : G[i]) {
                int s = n2g[e.from];
                int t = n2g[e.to];
                if (usedEdges.find(mp(s, t)) == usedEdges.end() && s != t) {
                    dag.add_edge(s, t, 1);
                    usedEdges.insert(mp(s, t));
                }
            }
        }
        return dag;
    }
};

class SAT {
    bool solved = false;

  public:
    int num;
    Graph<int> graph, dag;
    vvi scc;
    vi n2g;

    SAT(int n) : num(n), graph(2 * n, true), n2g(2 * num) {}

    void add_constraint(bool x_is_true, int x, bool y_is_true, int y) {
        assert(0 <= x && x < num && 0 <= y && y < num);
        if (!x_is_true) x += num;
        if (!y_is_true) y += num;
        graph.add_edge((x + num) % (2 * num), y, 1); // not x -> y
        graph.add_edge((y + num) % (2 * num), x, 1); // not y -> x
    }

    void solve() {
        SCCD sccd;
        sccd.SCC(graph, scc, n2g, dag);
        solved = true;
    }

    bool get_satisfiability() {
        if (!solved) solve();
        for (int i = 0; i < num; i++) {
            if (n2g[i] == n2g[i + num]) return false;
        }
        return true;
    }

    bool get_assignment(vector<bool> &assignment) {
        assignment.resize(num);
        if (!solved) solve();
        vector<bool> ret(num);
        for (int i = 0; i < num; i++) {
            if (n2g[i] == n2g[i + num]) return false;
            assignment[i] = n2g[i] > n2g[i + num];
        }
        return true;
    }
};

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

