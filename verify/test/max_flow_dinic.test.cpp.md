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


# :heavy_check_mark: test/max_flow_dinic.test.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#098f6bcd4621d373cade4e832627b4f6">test</a>
* <a href="{{ site.github.repository_url }}/blob/master/test/max_flow_dinic.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-05-05 12:01:30+09:00


* see: <a href="https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A">https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A</a>


## Depends on

* :heavy_check_mark: <a href="../../library/library/header.hpp.html">library/header.hpp</a>
* :heavy_check_mark: <a href="../../library/library/max_flow.hpp.html">library/max_flow.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A"

#include "../library/max_flow.hpp"

int main(void) {
    int V, E;
    cin >> V >> E;
    MaxFlow mf(V);
    rep(i, E) {
        int u, v, c;
        cin >> u >> v >> c;
        mf.add_edge(u, v, c);
    }
    cout << mf.max_flow(0, V - 1) << endl;
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "test/max_flow_dinic.test.cpp"
#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A"

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

class MaxFlow {
    struct FlowEdge {
        int to;
        ll cap;
        int rev;
    };
    vector<vector<FlowEdge>> flow_graph;
    vector<long long> level; // Distance from source
    vector<int> iter;        // How far have you finished looking

    // Calculate distance from source to other vertices
    void bfs(int source) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[source] = 0;
        que.push(source);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (int i = 0; i < flow_graph[v].size(); i++) {
                FlowEdge &e = flow_graph[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // Search increasing path with DFS
    long long dfs(int v, int target, long long f) {
        if (v == target) return f;
        for (int &i = iter[v]; i < flow_graph[v].size(); i++) {
            FlowEdge &e = flow_graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, target, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    flow_graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

  public:
    MaxFlow(int n) : flow_graph(n), level(n), iter(n) {}

    void add_edge(int from, int to, ll cap) {
        flow_graph[from].push_back(FlowEdge{to, cap, (int)flow_graph[to].size()});
        flow_graph[to].push_back(FlowEdge{from, 0, (int)flow_graph[from].size() - 1});
    }

    // calculate max flow from s to target
    ll max_flow(int source, int target) {
        ll flow = 0;
        while (true) {
            bfs(source);
            if (level[target] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            long long f;
            while ((f = dfs(source, target, INFL)) > 0) {
                flow += f;
            }
        }
    }
};

class ProjectSelection {
    int num;
    MaxFlow max_flow; // |V| = num+2, source = num, target = num+1
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

    // if choose i and not choose j, get penalty
    void add_penalty(int i, int j, long long penalty) {
        max_flow.add_edge(i, j, penalty);
    }

    long long max_reward() {
        return sum_reward - max_flow.max_flow(num, num + 1);
    }
};
#line 4 "test/max_flow_dinic.test.cpp"

int main(void) {
    int V, E;
    cin >> V >> E;
    MaxFlow mf(V);
    rep(i, E) {
        int u, v, c;
        cin >> u >> v >> c;
        mf.add_edge(u, v, c);
    }
    cout << mf.max_flow(0, V - 1) << endl;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

