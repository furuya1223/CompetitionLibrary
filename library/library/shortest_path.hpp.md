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


# :heavy_check_mark: library/shortest_path.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/shortest_path.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-05 14:06:25+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Verified with

* :heavy_check_mark: <a href="../../verify/test/dijkstra.test.cpp.html">test/dijkstra.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

void Dijkstra(Graph<int> &graph, int start, vector<long long> &cost) {
    cost.resize(graph.size());
    fill(cost.begin(), cost.end(), INFL);
    vector<int> prev(graph.size());
    pql<Pll> Q;

    fill(cost.begin(), cost.end(), INFL);
    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        int pos = Q.top().second;
        ll d = Q.top().first;
        Q.pop();
        rep(i, graph[pos].size()) {
            if (cost[graph[pos][i].to] > cost[pos] + graph[pos][i].cost) {
                cost[graph[pos][i].to] = cost[pos] + graph[pos][i].cost;
                Q.push(mp(cost[graph[pos][i].to], graph[pos][i].to));
                prev[graph[pos][i].to] = pos;
            }
        }
    }
}

void Dijkstra(vvd graph, int start, vd &cost, int V) {
    vi prev(V);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> Q;

    fill(cost.begin(), cost.end(), INF);
    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        pair<double, int> _pos = Q.top();
        double d = _pos.first;
        int pos = _pos.second;
        Q.pop();
        rep(i, V) {
            if (i == pos) continue;
            if (cost[i] > cost[pos] + graph[pos][i]) {
                cost[i] = cost[pos] + graph[pos][i];
                Q.push(mp(cost[i], i));
            }
        }
    }
}

bool BellmanFord(vvPi graph, int start, vi &cost, int V) {
    vi prev(V);
    vb visitable(V, false);
    int k = 1;
    int visitable_num = 0;

    stack<int> st;
    st.push(start);
    while (!st.empty()) {
        int pos = st.top();
        st.pop();
        visitable[pos] = true;
        rep(i, graph[pos].size()) {
            if (!visitable[graph[pos][i].first]) {
                st.push(graph[pos][i].first);
            }
        }
    }

    rep(i, V) {
        if (visitable[i]) visitable_num++;
    }

    fill(cost.begin(), cost.end(), INF);
    cost[start] = 0;

    while (true) {
        bool flag = true;
        rep(i, V) rep(j, graph[i].size()) {
            if (visitable[i] && cost[graph[i][j].first] > cost[i] + graph[i][j].second) {
                cost[graph[i][j].first] = cost[i] + graph[i][j].second;
                prev[graph[i][j].first] = i;
                flag = false;
                break;
            }
        }
        if (flag) break;
        if (k == visitable_num) return false;
        k++;
    }
    return true;
}

bool WarshallFloyd(vvll &graph, int V) {
    rep(k, V) rep(i, V) rep(j, V) {
        // i->(k)->j
        if (graph[i][k] != INFL && graph[k][j] != INFL) {
            graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
        }
    }
    rep(i, V) {
        if (graph[i][i] < 0) return false;
    }
    return true;
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


#line 2 "library/shortest_path.hpp"

void Dijkstra(Graph<int> &graph, int start, vector<long long> &cost) {
    cost.resize(graph.size());
    fill(cost.begin(), cost.end(), INFL);
    vector<int> prev(graph.size());
    pql<Pll> Q;

    fill(cost.begin(), cost.end(), INFL);
    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        int pos = Q.top().second;
        ll d = Q.top().first;
        Q.pop();
        rep(i, graph[pos].size()) {
            if (cost[graph[pos][i].to] > cost[pos] + graph[pos][i].cost) {
                cost[graph[pos][i].to] = cost[pos] + graph[pos][i].cost;
                Q.push(mp(cost[graph[pos][i].to], graph[pos][i].to));
                prev[graph[pos][i].to] = pos;
            }
        }
    }
}

void Dijkstra(vvd graph, int start, vd &cost, int V) {
    vi prev(V);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> Q;

    fill(cost.begin(), cost.end(), INF);
    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        pair<double, int> _pos = Q.top();
        double d = _pos.first;
        int pos = _pos.second;
        Q.pop();
        rep(i, V) {
            if (i == pos) continue;
            if (cost[i] > cost[pos] + graph[pos][i]) {
                cost[i] = cost[pos] + graph[pos][i];
                Q.push(mp(cost[i], i));
            }
        }
    }
}

bool BellmanFord(vvPi graph, int start, vi &cost, int V) {
    vi prev(V);
    vb visitable(V, false);
    int k = 1;
    int visitable_num = 0;

    stack<int> st;
    st.push(start);
    while (!st.empty()) {
        int pos = st.top();
        st.pop();
        visitable[pos] = true;
        rep(i, graph[pos].size()) {
            if (!visitable[graph[pos][i].first]) {
                st.push(graph[pos][i].first);
            }
        }
    }

    rep(i, V) {
        if (visitable[i]) visitable_num++;
    }

    fill(cost.begin(), cost.end(), INF);
    cost[start] = 0;

    while (true) {
        bool flag = true;
        rep(i, V) rep(j, graph[i].size()) {
            if (visitable[i] && cost[graph[i][j].first] > cost[i] + graph[i][j].second) {
                cost[graph[i][j].first] = cost[i] + graph[i][j].second;
                prev[graph[i][j].first] = i;
                flag = false;
                break;
            }
        }
        if (flag) break;
        if (k == visitable_num) return false;
        k++;
    }
    return true;
}

bool WarshallFloyd(vvll &graph, int V) {
    rep(k, V) rep(i, V) rep(j, V) {
        // i->(k)->j
        if (graph[i][k] != INFL && graph[k][j] != INFL) {
            graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
        }
    }
    rep(i, V) {
        if (graph[i][i] < 0) return false;
    }
    return true;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

