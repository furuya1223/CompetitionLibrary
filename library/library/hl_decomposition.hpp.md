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


# :heavy_check_mark: library/hl_decomposition.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/hl_decomposition.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 17:13:00+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Verified with

* :heavy_check_mark: <a href="../../verify/test/hl_decomposition_edge.test.cpp.html">test/hl_decomposition_edge.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
// cf. https://tubo28.me/compprog/algorithm/hld/
#include "header.hpp"

template <class T>
struct HLDecomposition {
    Graph<T> &g;
    int n;
    vector<vector<int>> chains; // chain の列。head の浅い順
    vector<int> parent;         // 親の頂点番号。根なら -1
    vector<int> subsize;        // 自身を根とする部分木のサイズ
    vector<int> depth;          // 根からの距離
    vector<int> head;           // 自身を含む chain の先頭の頂点
    vector<int> last;           // 自身を含む chain の末尾の頂点
    vector<int> next;           // 自身が属する chain での次の頂点。範囲外なら -1
    vector<int> prev;           // 自身が属する chain での前の頂点。範囲外なら -1
    vector<int> chain;          // 自身が属する chain 番号
    vector<int> idx;            // 自身が属する chain での自身の番号
    vector<int> encode; // 元の頂点番号をパスクエリ用に並べ替えた番号に変える
    vector<int> decode; // パスクエリ用に並べ替えた頂点番号を元の番号に変える

    HLDecomposition(Graph<T> &g_, int r = -1)
        : g(g_), n(g.size()), chains(0), parent(n, 0), subsize(n, 0), depth(n, -1), head(n, 0),
          last(n, 0), next(n, -1), prev(n, -1), chain(n, -1), idx(n, 0), encode(n), decode(n) {
        if (r != -1) decompose(r);
    }

    // 頂点 v が属する chain の親頂点を返す
    // u から根まで登りたいときは
    // for(; u != -1; u = climb(u))
    int climb(int v) const {
        return parent[head[v]];
    }

    // chains[first][second] = v
    std::pair<int, int> get_index(int v) const {
        return std::make_pair(chain[v], idx[v]);
    }

    int size() const {
        return n;
    }

    void decompose(const int root) {
        stack<int> stk; // DFS 用スタック
        stk.push(root);
        parent[root] = -1;
        depth[root] = 0;

        // DFS
        while (!stk.empty()) {
            int v = stk.top();
            stk.pop();
            if (v >= 0) {
                // 行きがけに parent, depth を計算
                stk.push(~v); // 帰りがけ用に反転したものを積んでおく
                for (auto &e : g[v]) {
                    int child = e.to;
                    if (depth[child] == -1) {
                        depth[child] = depth[v] + 1;
                        parent[child] = v;
                        stk.push(child);
                    }
                }
            } else {
                // 帰りがけに subsize を計算し、Heavy Edge を決める
                // next は Heavy Edge の子とする
                int m = 0;
                v = ~v;
                subsize[v] = 1;
                for (auto &e : g[v]) {
                    int child = e.to;
                    if (parent[v] == child) continue;
                    subsize[v] += subsize[child];
                    if (m < subsize[child]) {
                        m = subsize[child];
                        next[v] = child;
                    }
                }
            }
        }

        stk.push(root);
        while (!stk.empty()) {
            int h = stk.top();
            stk.pop();
            for (auto &e : g[h]) {
                if (parent[h] != e.to) stk.push(e.to);
            }

            if (chain[h] != -1) continue;
            chains.push_back(vector<int>());
            vector<int> &path = chains.back();

            // Heavy Edge を辿っていき、chains に追加する
            for (int cur = h; cur != -1;) {
                path.push_back(cur);
                cur = next[cur];
            }

            // 今追加した chain の情報を調べて記録する
            for (int i = 0; i < (int)path.size(); i++) {
                int v = path[i];
                head[v] = path.front();
                last[v] = path.back();
                next[v] = i + 1 != (int)path.size() ? path[i + 1] : -1;
                prev[v] = i != 0 ? path[i - 1] : -1;
                chain[v] = chains.size() - 1;
                idx[v] = i;
            }
        }

        int new_index = 0;
        for (auto path : chains) {
            for (auto v : path) {
                encode[v] = new_index;
                decode[new_index] = v;
                new_index++;
            }
        }
    }

    int lca(int u, int v) {
        // 同じ chain に属するまで、head が深い側の頂点を登らせる
        while (chain[u] != chain[v]) {
            if (depth[head[u]] > depth[head[v]])
                u = climb(u);
            else
                v = climb(v);
        }
        // 同じ chain に来たら、浅い方が LCA
        return depth[u] < depth[v] ? u : v;
    }

    // 頂点が値を持つパスクエリ用の区間列（閉区間）を encode して返す
    // LCA は result.back()[0]（辺が値を持つ場合は LCA を除く）
    vector<pair<int, int>> queries(int u, int v) {
        vector<pair<int, int>> result;
        while (chain[u] != chain[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                result.push_back(make_pair(encode[head[u]], encode[u]));
                u = climb(u);
            } else {
                result.push_back(make_pair(encode[head[v]], encode[v]));
                v = climb(v);
            }
        }
        result.push_back(minmax(encode[u], encode[v]));
        return result;
    }
};

// 辺が値を持つ場合に、子側の頂点に値を下ろす
template <class T>
vector<T> down(Graph<T> &G, int root, T def) {
    int n = G.size();
    vector<T> result(n);
    stack<tuple<int, int, T>> stk;
    stk.push(make_tuple(-1, root, def));
    while (!stk.empty()) {
        int parent, idx;
        T val;
        tie(parent, idx, val) = stk.top();
        stk.pop();
        result[idx] = val;
        for (const auto &e : G[idx]) {
            if (e.to == parent) continue;
            stk.push(make_tuple(idx, e.to, e.cost));
        }
    }
    return result;
}
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "library/hl_decomposition.hpp"
// cf. https://tubo28.me/compprog/algorithm/hld/
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


#line 3 "library/hl_decomposition.hpp"

template <class T>
struct HLDecomposition {
    Graph<T> &g;
    int n;
    vector<vector<int>> chains; // chain の列。head の浅い順
    vector<int> parent;         // 親の頂点番号。根なら -1
    vector<int> subsize;        // 自身を根とする部分木のサイズ
    vector<int> depth;          // 根からの距離
    vector<int> head;           // 自身を含む chain の先頭の頂点
    vector<int> last;           // 自身を含む chain の末尾の頂点
    vector<int> next;           // 自身が属する chain での次の頂点。範囲外なら -1
    vector<int> prev;           // 自身が属する chain での前の頂点。範囲外なら -1
    vector<int> chain;          // 自身が属する chain 番号
    vector<int> idx;            // 自身が属する chain での自身の番号
    vector<int> encode; // 元の頂点番号をパスクエリ用に並べ替えた番号に変える
    vector<int> decode; // パスクエリ用に並べ替えた頂点番号を元の番号に変える

    HLDecomposition(Graph<T> &g_, int r = -1)
        : g(g_), n(g.size()), chains(0), parent(n, 0), subsize(n, 0), depth(n, -1), head(n, 0),
          last(n, 0), next(n, -1), prev(n, -1), chain(n, -1), idx(n, 0), encode(n), decode(n) {
        if (r != -1) decompose(r);
    }

    // 頂点 v が属する chain の親頂点を返す
    // u から根まで登りたいときは
    // for(; u != -1; u = climb(u))
    int climb(int v) const {
        return parent[head[v]];
    }

    // chains[first][second] = v
    std::pair<int, int> get_index(int v) const {
        return std::make_pair(chain[v], idx[v]);
    }

    int size() const {
        return n;
    }

    void decompose(const int root) {
        stack<int> stk; // DFS 用スタック
        stk.push(root);
        parent[root] = -1;
        depth[root] = 0;

        // DFS
        while (!stk.empty()) {
            int v = stk.top();
            stk.pop();
            if (v >= 0) {
                // 行きがけに parent, depth を計算
                stk.push(~v); // 帰りがけ用に反転したものを積んでおく
                for (auto &e : g[v]) {
                    int child = e.to;
                    if (depth[child] == -1) {
                        depth[child] = depth[v] + 1;
                        parent[child] = v;
                        stk.push(child);
                    }
                }
            } else {
                // 帰りがけに subsize を計算し、Heavy Edge を決める
                // next は Heavy Edge の子とする
                int m = 0;
                v = ~v;
                subsize[v] = 1;
                for (auto &e : g[v]) {
                    int child = e.to;
                    if (parent[v] == child) continue;
                    subsize[v] += subsize[child];
                    if (m < subsize[child]) {
                        m = subsize[child];
                        next[v] = child;
                    }
                }
            }
        }

        stk.push(root);
        while (!stk.empty()) {
            int h = stk.top();
            stk.pop();
            for (auto &e : g[h]) {
                if (parent[h] != e.to) stk.push(e.to);
            }

            if (chain[h] != -1) continue;
            chains.push_back(vector<int>());
            vector<int> &path = chains.back();

            // Heavy Edge を辿っていき、chains に追加する
            for (int cur = h; cur != -1;) {
                path.push_back(cur);
                cur = next[cur];
            }

            // 今追加した chain の情報を調べて記録する
            for (int i = 0; i < (int)path.size(); i++) {
                int v = path[i];
                head[v] = path.front();
                last[v] = path.back();
                next[v] = i + 1 != (int)path.size() ? path[i + 1] : -1;
                prev[v] = i != 0 ? path[i - 1] : -1;
                chain[v] = chains.size() - 1;
                idx[v] = i;
            }
        }

        int new_index = 0;
        for (auto path : chains) {
            for (auto v : path) {
                encode[v] = new_index;
                decode[new_index] = v;
                new_index++;
            }
        }
    }

    int lca(int u, int v) {
        // 同じ chain に属するまで、head が深い側の頂点を登らせる
        while (chain[u] != chain[v]) {
            if (depth[head[u]] > depth[head[v]])
                u = climb(u);
            else
                v = climb(v);
        }
        // 同じ chain に来たら、浅い方が LCA
        return depth[u] < depth[v] ? u : v;
    }

    // 頂点が値を持つパスクエリ用の区間列（閉区間）を encode して返す
    // LCA は result.back()[0]（辺が値を持つ場合は LCA を除く）
    vector<pair<int, int>> queries(int u, int v) {
        vector<pair<int, int>> result;
        while (chain[u] != chain[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                result.push_back(make_pair(encode[head[u]], encode[u]));
                u = climb(u);
            } else {
                result.push_back(make_pair(encode[head[v]], encode[v]));
                v = climb(v);
            }
        }
        result.push_back(minmax(encode[u], encode[v]));
        return result;
    }
};

// 辺が値を持つ場合に、子側の頂点に値を下ろす
template <class T>
vector<T> down(Graph<T> &G, int root, T def) {
    int n = G.size();
    vector<T> result(n);
    stack<tuple<int, int, T>> stk;
    stk.push(make_tuple(-1, root, def));
    while (!stk.empty()) {
        int parent, idx;
        T val;
        tie(parent, idx, val) = stk.top();
        stk.pop();
        result[idx] = val;
        for (const auto &e : G[idx]) {
            if (e.to == parent) continue;
            stk.push(make_tuple(idx, e.to, e.cost));
        }
    }
    return result;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

