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


# :warning: library/convex_hull_trick.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/convex_hull_trick.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

template <class T>
class ConvesHullTrick {
    static const T IS_QUERY = std::numeric_limits<T>::lowest();

    // 直線 ax+b
    struct Line {
        T a, b;
        // Lines において次の直線のポインタを得る
        mutable std::function<const Line *()> nextLine;

        bool operator<(Line rhs) const {
            if (b == IS_QUERY) {
                // 自分がクエリ
                const Line *next_line = rhs.nextLine();
                if (next_line == nullptr) return false;
                T x = a;
                return (rhs.a - next_line->a) * x + (rhs.b - next_line->b) > 0;
            } else if (rhs.b == IS_QUERY) {
                // rhs がクエリ
                const Line *next_line = nextLine();
                if (next_line == nullptr) return false;
                T x = rhs.a;
                return (a - next_line->a) * x + (b - next_line->b) > 0;
            }
            return (a < rhs.a || (a == rhs.a && b < rhs.b));
        }
    };
    set<Line> lines; // 直線集合

    // l が不要か判定
    bool isVain(Line l) {
        auto it = lines.find(l);
        auto next_it = next(it);
        if (it == lines.begin()) {
            if (next_it == lines.end()) return false;
            return it->a == next_it->a && it->b <= next_it->b;
        } else {
            auto prev_it = prev(it);
            if (next_it == lines.end())
                return it->a == prev_it->a && it->b <= prev_it->b;
            return (prev_it->b - it->b) * (it->a - next_it->a) >=
                   (it->b - next_it->b) * (prev_it->a - it->a);
        }
    }

  public:
    // 直線 ax+b を追加
    void add(T a, T b) {
        auto it = lines.insert(Line{a, b}).first;
        // 次の直線を得る関数をセットしておく
        it->nextLine = [=] {
            return (std::next(it) == lines.end() ? nullptr : &*std::next(it));
        };

        if (isVain(*it)) {
            // 今追加した直線が不要
            lines.erase(it);
            return;
        }
        // 追加した直線の前後が不要なら削除していく
        while (next(it) != lines.end() && isVain(*next(it))) {
            lines.erase(next(it));
        }
        while (it != lines.begin() && isVain(*prev(it))) {
            lines.erase(prev(it));
        }
    }

    // x での最小値を出力
    T get(T x) {
        auto l = lines.lower_bound(Line{x, IS_QUERY});
        return l->a * x + l->b;
    }
};

// https://atcoder.jp/contests/dp/tasks/dp_z
int main() {
    int N;
    ll C;
    cin >> N >> C;
    ConvesHullTrick<ll> CHT;
    vll dp(N);
    dp[0] = 0;
    rep(i, N) {
        ll h;
        cin >> h;
        if (i != 0) {
            dp[i] = CHT.get(h) + C + h * h;
        }
        CHT.add(-2 * h, h * h + dp[i]);
    }
    cout << dp[N - 1] << endl;

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


#line 2 "library/convex_hull_trick.hpp"

template <class T>
class ConvesHullTrick {
    static const T IS_QUERY = std::numeric_limits<T>::lowest();

    // 直線 ax+b
    struct Line {
        T a, b;
        // Lines において次の直線のポインタを得る
        mutable std::function<const Line *()> nextLine;

        bool operator<(Line rhs) const {
            if (b == IS_QUERY) {
                // 自分がクエリ
                const Line *next_line = rhs.nextLine();
                if (next_line == nullptr) return false;
                T x = a;
                return (rhs.a - next_line->a) * x + (rhs.b - next_line->b) > 0;
            } else if (rhs.b == IS_QUERY) {
                // rhs がクエリ
                const Line *next_line = nextLine();
                if (next_line == nullptr) return false;
                T x = rhs.a;
                return (a - next_line->a) * x + (b - next_line->b) > 0;
            }
            return (a < rhs.a || (a == rhs.a && b < rhs.b));
        }
    };
    set<Line> lines; // 直線集合

    // l が不要か判定
    bool isVain(Line l) {
        auto it = lines.find(l);
        auto next_it = next(it);
        if (it == lines.begin()) {
            if (next_it == lines.end()) return false;
            return it->a == next_it->a && it->b <= next_it->b;
        } else {
            auto prev_it = prev(it);
            if (next_it == lines.end())
                return it->a == prev_it->a && it->b <= prev_it->b;
            return (prev_it->b - it->b) * (it->a - next_it->a) >=
                   (it->b - next_it->b) * (prev_it->a - it->a);
        }
    }

  public:
    // 直線 ax+b を追加
    void add(T a, T b) {
        auto it = lines.insert(Line{a, b}).first;
        // 次の直線を得る関数をセットしておく
        it->nextLine = [=] {
            return (std::next(it) == lines.end() ? nullptr : &*std::next(it));
        };

        if (isVain(*it)) {
            // 今追加した直線が不要
            lines.erase(it);
            return;
        }
        // 追加した直線の前後が不要なら削除していく
        while (next(it) != lines.end() && isVain(*next(it))) {
            lines.erase(next(it));
        }
        while (it != lines.begin() && isVain(*prev(it))) {
            lines.erase(prev(it));
        }
    }

    // x での最小値を出力
    T get(T x) {
        auto l = lines.lower_bound(Line{x, IS_QUERY});
        return l->a * x + l->b;
    }
};

// https://atcoder.jp/contests/dp/tasks/dp_z
int main() {
    int N;
    ll C;
    cin >> N >> C;
    ConvesHullTrick<ll> CHT;
    vll dp(N);
    dp[0] = 0;
    rep(i, N) {
        ll h;
        cin >> h;
        if (i != 0) {
            dp[i] = CHT.get(h) + C + h * h;
        }
        CHT.add(-2 * h, h * h + dp[i]);
    }
    cout << dp[N - 1] << endl;

    return 0;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

