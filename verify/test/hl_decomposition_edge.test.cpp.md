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


# :heavy_check_mark: test/hl_decomposition_edge.test.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#098f6bcd4621d373cade4e832627b4f6">test</a>
* <a href="{{ site.github.repository_url }}/blob/master/test/hl_decomposition_edge.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-05-06 14:00:54+09:00


* see: <a href="https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_D">https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_D</a>


## Depends on

* :heavy_check_mark: <a href="../../library/library/header.hpp.html">library/header.hpp</a>
* :heavy_check_mark: <a href="../../library/library/hl_decomposition.hpp.html">library/hl_decomposition.hpp</a>
* :heavy_check_mark: <a href="../../library/library/segment_tree.hpp.html">library/segment_tree.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_D"

#include "../library/hl_decomposition.hpp"
#include "../library/segment_tree.hpp"

int main() {
    int n;
    cin >> n;
    Graph<int> G(n, false);
    rep(i, n) {
        int k;
        cin >> k;
        rep(j, k) {
            int c;
            cin >> c;
            G.add_edge(i, c, 0);
        }
    }
    HLDecomposition<int> hld(G, 0);
    SegTree<long long> st(
        n, 0, [](long long a, long long b) { return a + b; },
        [](long long a, long long b) { return a + b; });
    int q;
    cin >> q;
    rep(i, q) {
        int kind;
        cin >> kind;
        if (kind == 0) {
            // add
            int v, w;
            cin >> v >> w;
            st.change(hld.encode[v], w);
        } else {
            // getSum
            int u;
            cin >> u;
            vector<pair<int, int>> queries = hld.queries(0, u);
            long long ans = 0;
            rep(i, queries.size()) {
                int l, r;
                tie(l, r) = queries[i];
                r++;
                if (i == queries.size() - 1) l++;
                ans += st.query(l, r);
            }
            cout << ans << '\n';
        }
    }

    return 0;
}
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "test/hl_decomposition_edge.test.cpp"
#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_D"

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
#line 1 "library/segment_tree.hpp"



#line 5 "library/segment_tree.hpp"

template <class T>
class SegTree {
    int n;                       // 葉の数
    vector<T> data;              // データを格納するvector
    T def;                       // 初期値かつ単位元
    function<T(T, T)> operation; // 区間クエリで使う処理
    function<T(T, T)> update;    // 点更新で使う処理

    // 区間[a,b)の総和。ノードk=[l,r)に着目している。
    T _query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return def; // 交差しない
        if (a <= l && r <= b)
            return data[k]; // a,l,r,bの順で完全に含まれる
        else {
            T c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2); // 左の子
            T c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r); // 右の子
            return operation(c1, c2);
        }
    }

  public:
    // _n:必要サイズ, _def:初期値かつ単位元, _operation:クエリ関数,
    // _update:更新関数
    SegTree(size_t _n, T _def, function<T(T, T)> _operation, function<T(T, T)> _update)
        : def(_def), operation(_operation), update(_update) {
        n = 1;
        while (n < _n) {
            n *= 2;
        }
        data = vector<T>(2 * n - 1, def);
    }
    SegTree() {}

    // 場所i(0-indexed)の値をxで更新
    void change(int i, T x) {
        i += n - 1;
        data[i] = update(data[i], x);
        while (i > 0) {
            i = (i - 1) / 2;
            data[i] = operation(data[i * 2 + 1], data[i * 2 + 2]);
        }
    }

    // [a, b)の区間クエリを実行
    T query(int a, int b) {
        return _query(a, b, 0, 0, n);
    }

    // 添字でアクセス
    T operator[](int i) {
        return data[i + n - 1];
    }
};

// 遅延伝播セグ木
template <class T, class U>
class LazySegTree {
    /*
    区間更新・区間和
    auto operation = [](ll a, ll b) { return a + b; };
    auto adapt_lazy = [](ll a, ll b) { return b; };
    auto merge_lazy = [](ll a, ll b) { return b; };
    auto multiply_lazy = [](ll a, int n) { return a * n; };

    区間加算・区間和
    auto operation = [](ll a, ll b) { return a + b; };
    auto adapt_lazy = [](ll a, ll b) { return a + b; };
    auto merge_lazy = [](ll a, ll b) { return a + b; };
    auto multiply_lazy = [](ll a, int n) { return a * n; };

    区間更新・区間最小
    auto operation = [](ll a, ll b) { return min(a, b); };
    auto adapt_lazy = [](ll a, ll b) { return b; };
    auto merge_lazy = [](ll a, ll b) { return b; };
    auto multiply_lazy = [](ll a, int n) { return a; };

    区間加算・区間最小
    auto operation = [](ll a, ll b) { return min(a, b); };
    auto adapt_lazy = [](ll a, ll b) { return a + b; };
    auto merge_lazy = [](ll a, ll b) { return a + b; };
    auto multiply_lazy = [](ll a, int n) { return a; };
    */
    int n;
    vector<T> dat;
    vector<U> lazy;
    vector<int> width;

    function<T(T, T)> operation;
    function<T(T, U)> adapt_lazy;
    function<U(U, U)> merge_lazy;
    function<U(U, int)> multiply_lazy;

    T init, def, lazy_def;

    // ツリー上のノード k に lazy 値 v をセット
    void set_lazy(int k, U v) {
        if (lazy[k] == lazy_def) {
            lazy[k] = v;
            dat[k] = adapt_lazy(dat[k], multiply_lazy(v, width[k]));
        } else {
            lazy[k] = merge_lazy(lazy[k], v);
            dat[k] = adapt_lazy(dat[k], multiply_lazy(v, width[k]));
        }
    }
    // lazy を 1 段下ろす
    void push(int k) {
        if (lazy[k] == lazy_def) return;
        set_lazy(k * 2 + 1, lazy[k]);
        set_lazy(k * 2 + 2, lazy[k]);
        lazy[k] = lazy_def;
    }

    // 区間[a,b)の区間クエリ。ノードk=[l,r)に着目している。
    long long _query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return def;    // 交差しない
        if (a <= l && r <= b) return dat[k]; // a,l,r,bの順で完全に含まれる
        push(k);
        long long c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2); // 左の子
        long long c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r); // 右の子
        return operation(c1, c2);
    }

  public:
    // _n:必要サイズ, _def:初期値, _operation:クエリ関数,
    // _adapt_lazy:区間作用素適用関数, _merge_lazy:区間作用素マージ関数,
    // _multiply_lazy:作用素を要素数で変形
    LazySegTree(
        size_t _n, T _init, T _def, T _lazy_def, function<T(T, T)> _operation,
        function<T(T, U)> _adapt_lazy, function<U(U, U)> _merge_lazy,
        function<U(U, int)> _multiply_lazy = [](U u, int n) { return u; })
        : init(_init), def(_def), lazy_def(_lazy_def), operation(_operation),
          adapt_lazy(_adapt_lazy), merge_lazy(_merge_lazy), multiply_lazy(_multiply_lazy) {
        n = 1;
        while (n < _n) n *= 2;
        dat = vector<long long>(2 * n - 1, init);
        lazy = vector<long long>(2 * n - 1, lazy_def);
        width = vector<int>(2 * n - 1);
        width[0] = n;
        repr(i, 1, 2 * n - 1) width[i] = width[(i - 1) / 2] / 2;
    }

    // 区間[a,b)にvを作用させる
    void range_operation(int a, int b, long long v, int k = 0, int l = 0, int r = -1) {
        if (r == -1) r = n;
        if (r <= a || b <= l) return; // 交差しない
        if (a <= l && r <= b) {       // a,l,r,bの順で完全に含まれる
            set_lazy(k, v);
            return;
        }
        push(k);
        range_operation(a, b, v, 2 * k + 1, l, (l + r) / 2); // 左の子
        range_operation(a, b, v, 2 * k + 2, (l + r) / 2, r); // 右の子
        dat[k] = operation(dat[k * 2 + 1], dat[k * 2 + 2]);
    }

    // [a, b)の区間クエリを実行
    long long query(int a, int b) {
        return _query(a, b, 0, 0, n);
    }

    //  添字でアクセス
    long long operator[](int i) {
        return query(i, i + 1);
    }
};

// Binary Indexed Tree（区間和）
struct BIT {
    int n, height;
    vector<int> dat;

    BIT(int _n) {
        n = 1;
        height = 1;
        while (n < _n) {
            n *= 2;
            height++;
        }
        dat = vector<int>(n + 1);
    }

    // [0,i)までの和
    int sum(int i) {
        int s = 0;
        while (i > 0) {
            s += dat[i];
            i -= i & -i; // i & -i は i の最後の1ビット
        }
        return s;
    }

    void add(int i, int x) {
        i++; // 1-indexedに変更
        while (i <= n) {
            dat[i] += x;
            i += i & -i;
        }
    }
};


#line 5 "test/hl_decomposition_edge.test.cpp"

int main() {
    int n;
    cin >> n;
    Graph<int> G(n, false);
    rep(i, n) {
        int k;
        cin >> k;
        rep(j, k) {
            int c;
            cin >> c;
            G.add_edge(i, c, 0);
        }
    }
    HLDecomposition<int> hld(G, 0);
    SegTree<long long> st(
        n, 0, [](long long a, long long b) { return a + b; },
        [](long long a, long long b) { return a + b; });
    int q;
    cin >> q;
    rep(i, q) {
        int kind;
        cin >> kind;
        if (kind == 0) {
            // add
            int v, w;
            cin >> v >> w;
            st.change(hld.encode[v], w);
        } else {
            // getSum
            int u;
            cin >> u;
            vector<pair<int, int>> queries = hld.queries(0, u);
            long long ans = 0;
            rep(i, queries.size()) {
                int l, r;
                tie(l, r) = queries[i];
                r++;
                if (i == queries.size() - 1) l++;
                ans += st.query(l, r);
            }
            cout << ans << '\n';
        }
    }

    return 0;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

