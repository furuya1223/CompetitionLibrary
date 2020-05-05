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


# :warning: library/lowest_common_ancestor.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/lowest_common_ancestor.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :question: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

class LCA {
public:
	int N;
    int root;
    vector<int> depth;
	int maxdepth = 0;
	vvi dblpar;

	LCA(const Graph<int> &G, int root = 0): root(root) {
		N = G.size();
		depth = vi(N);
		dfs1(G, root, -1);
		int D = bindigit(maxdepth) + 1;
		dblpar = vvi(D, vi(N));
		rep(i, D) {
			dfs2(G, 0, 0, i);
		}
	}
 
	void dfs1(const Graph<int> &G, int pos, int par, int d = 0) {
		depth[pos] = d;
		maxdepth = max(maxdepth, d);
		for (auto e : G[pos]) {
			if (e.to == par)continue;
			dfs1(G, e.to, pos, d + 1);
		}
	}
 
	void dfs2(const Graph<int> &G, int pos, int par, int dbl) {
		if (dbl == 0) {
			dblpar[0][pos] = par;
		}
		else {
			dblpar[dbl][pos] = dblpar[dbl - 1][dblpar[dbl - 1][pos]];
		}
		for (auto e : G[pos]) {
			if (e.to == par)continue;
			dfs2(G, e.to, pos, dbl);
		}
	}
 
	int parent(int pos, int step) {
		int ret = pos;
		int dbl = 0;
		while (step > 0) {
			if ((step & 1) != 0) {
				ret = dblpar[dbl][ret];
			}
			step >>= 1;
			dbl++;
		}
		return ret;
	}
 
	int bindigit(int n) {
		int ret = 0;
		while (n > 0) {
			ret++;
			n >>= 1;
		}
		return ret;
	}
 
	int lca(int u, int v) {
		if (depth[u] < depth[v]) {
			swap(u, v);
		}
		u = parent(u, depth[u] - depth[v]);
		int ng = -1, ok = depth[u];
		while (ok - ng > 1) {
			int mid = (ok + ng) / 2;
			if (parent(u, mid) == parent(v, mid)) {
				ok = mid;
			}
			else {
				ng = mid;
			}
		}
		return parent(u, ok);
	}
 
	int dist(int u, int v) {
		return depth[u] + depth[v] - 2 * depth[lca(u, v)];
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


#line 2 "library/lowest_common_ancestor.hpp"

class LCA {
public:
	int N;
    int root;
    vector<int> depth;
	int maxdepth = 0;
	vvi dblpar;

	LCA(const Graph<int> &G, int root = 0): root(root) {
		N = G.size();
		depth = vi(N);
		dfs1(G, root, -1);
		int D = bindigit(maxdepth) + 1;
		dblpar = vvi(D, vi(N));
		rep(i, D) {
			dfs2(G, 0, 0, i);
		}
	}
 
	void dfs1(const Graph<int> &G, int pos, int par, int d = 0) {
		depth[pos] = d;
		maxdepth = max(maxdepth, d);
		for (auto e : G[pos]) {
			if (e.to == par)continue;
			dfs1(G, e.to, pos, d + 1);
		}
	}
 
	void dfs2(const Graph<int> &G, int pos, int par, int dbl) {
		if (dbl == 0) {
			dblpar[0][pos] = par;
		}
		else {
			dblpar[dbl][pos] = dblpar[dbl - 1][dblpar[dbl - 1][pos]];
		}
		for (auto e : G[pos]) {
			if (e.to == par)continue;
			dfs2(G, e.to, pos, dbl);
		}
	}
 
	int parent(int pos, int step) {
		int ret = pos;
		int dbl = 0;
		while (step > 0) {
			if ((step & 1) != 0) {
				ret = dblpar[dbl][ret];
			}
			step >>= 1;
			dbl++;
		}
		return ret;
	}
 
	int bindigit(int n) {
		int ret = 0;
		while (n > 0) {
			ret++;
			n >>= 1;
		}
		return ret;
	}
 
	int lca(int u, int v) {
		if (depth[u] < depth[v]) {
			swap(u, v);
		}
		u = parent(u, depth[u] - depth[v]);
		int ng = -1, ok = depth[u];
		while (ok - ng > 1) {
			int mid = (ok + ng) / 2;
			if (parent(u, mid) == parent(v, mid)) {
				ok = mid;
			}
			else {
				ng = mid;
			}
		}
		return parent(u, ok);
	}
 
	int dist(int u, int v) {
		return depth[u] + depth[v] - 2 * depth[lca(u, v)];
	}
};

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

