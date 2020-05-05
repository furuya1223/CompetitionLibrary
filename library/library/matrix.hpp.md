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


# :warning: library/matrix.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/matrix.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 17:08:05+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

template <typename T>
class Mat {
  private:
    vector<vector<T>> _dat;
    T mod;

    void SwapRow(int a, int b) {
        if (a < 0 || a >= height() || b < 0 || b >= height()) return;
        rep(i, width()) {
            T tmp = _dat[a][i];
            _dat[a][i] = _dat[b][i];
            _dat[b][i] = tmp;
        }
    }
    void SwapCol(int a, int b) {
        if (a < 0 || a >= width() || b < 0 || b >= width()) return;
        rep(i, width()) {
            T tmp = _dat[i][a];
            _dat[i][a] = _dat[i][b];
            _dat[i][b] = tmp;
        }
    }

  public:
    Mat<T>(int mod) {
        this->mod = mod;
    }
    Mat<T>(int h, int w, int mod) {
        _dat = vector<vector<T>>(h);
        this->mod = mod;
        rep(i, height()) {
            _dat[i] = vector<T>(w);
            fill(begin(_dat[i]), end(_dat[i]), 0);
        }
    }
    Mat<T>(vector<vector<T>> dat, int mod) {
        this->mod = mod;
        _dat = dat;
    }

    int height() {
        return _dat.size();
    }
    int width() {
        return _dat[0].size();
    }

    vector<T> &operator[](int i) {
        return _dat[i];
    }
    Mat<T> &operator=(Mat b) {
        this->_dat = b._dat;
        return (*this);
    }
    Mat<T> operator+(Mat &b) {
        if (height() != b.height() || width() != b.width()) {
            cout << "Plus Error" << endl;
            exit(1);
        }
        Mat<T> ret(height(), width());
        rep(i, height()) rep(j, width()) ret[i][j] = (_dat[i][j] + b[i][j]) % mod;
        return ret;
    }
    Mat<T> operator-(Mat &b) {
        if (height() != b.height() || width() != b.width()) {
            cout << "Minus Error" << endl;
            exit(1);
        }
        Mat<T> ret(height(), width());
        rep(i, height()) rep(j, width()) ret[i][j] = (_dat[i][j] - b[i][j] + mod) % mod;
        return ret;
    }
    Mat<T> operator*(Mat b) {
        if (width() != b.height()) {
            cout << "Production Error" << endl;
            exit(1);
        }
        int h = height(), w = b.width();
        Mat<T> ret(h, w, mod);
        rep(i, h) rep(j, w) rep(k, width()) {
            ret[i][j] += (_dat[i][k] * b[k][j]) % mod;
            ret[i][j] %= mod;
        }
        return ret;
    }

    Mat<T> transpose() {
        Mat<T> ret(width(), height());
        rep(i, width()) rep(j, height()) {
            ret[i][j] = _dat[j][i];
        }
        return ret;
    }

    Mat<T> identity(int n) {
        Mat<T> idt(n, n, mod);
        rep(i, n) idt[i][i] = 1;
        return idt;
    }

    double determinant() {
        if (height() != width()) {
            cout << "Determinant Error" << endl;
            exit(1);
        }
        Mat<T> tmp = *this;
        int N = height();
        rep(i, N) {
            repr(j, i + 1, N) {
                double buf = tmp[j][i] / (double)tmp[i][i];
                rep(k, N) {
                    tmp[j][k] -= buf * tmp[i][k];
                }
            }
        }
        double det = 1;
        rep(i, N) {
            det *= tmp[i][i];
        }
        return det;
    }

    Mat<T> operator~() {
        if (height() != width()) {
            cout << "Inverse Error" << endl;
            exit(1);
        }
        int N = height();
        Mat<T> ret = identity(N);
        Mat<T> copy = *this;
        double buf;
        rep(i, N) {
            buf = 1 / (double)copy[i][i];
            rep(j, N) {
                copy[i][j] *= buf;
                ret[i][j] *= buf;
            }
            rep(j, N) {
                if (i != j) {
                    buf = copy[j][i];
                    rep(k, N) {
                        copy[j][k] -= copy[i][k] * buf;
                        ret[j][k] -= ret[i][k] * buf;
                    }
                }
            }
        }
        return ret;
    }

    void show() {
        rep(i, height()) {
            rep(j, width()) {
                cout << _dat[i][j];
                if (j != width() - 1) cout << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }
    void showSize() {
        cout << '(' << height() << ',' << width() << ')' << endl;
    }
};

template <typename T>
Mat<T> powMat(Mat<T> x, ll n) {
    Mat<T> res = x.identity(x.height());
    while (n > 0) {
        if (n & 1) res = res * x;
        x = x * x;
        n >>= 1;
    }
    return res;
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


#line 2 "library/matrix.hpp"

template <typename T>
class Mat {
  private:
    vector<vector<T>> _dat;
    T mod;

    void SwapRow(int a, int b) {
        if (a < 0 || a >= height() || b < 0 || b >= height()) return;
        rep(i, width()) {
            T tmp = _dat[a][i];
            _dat[a][i] = _dat[b][i];
            _dat[b][i] = tmp;
        }
    }
    void SwapCol(int a, int b) {
        if (a < 0 || a >= width() || b < 0 || b >= width()) return;
        rep(i, width()) {
            T tmp = _dat[i][a];
            _dat[i][a] = _dat[i][b];
            _dat[i][b] = tmp;
        }
    }

  public:
    Mat<T>(int mod) {
        this->mod = mod;
    }
    Mat<T>(int h, int w, int mod) {
        _dat = vector<vector<T>>(h);
        this->mod = mod;
        rep(i, height()) {
            _dat[i] = vector<T>(w);
            fill(begin(_dat[i]), end(_dat[i]), 0);
        }
    }
    Mat<T>(vector<vector<T>> dat, int mod) {
        this->mod = mod;
        _dat = dat;
    }

    int height() {
        return _dat.size();
    }
    int width() {
        return _dat[0].size();
    }

    vector<T> &operator[](int i) {
        return _dat[i];
    }
    Mat<T> &operator=(Mat b) {
        this->_dat = b._dat;
        return (*this);
    }
    Mat<T> operator+(Mat &b) {
        if (height() != b.height() || width() != b.width()) {
            cout << "Plus Error" << endl;
            exit(1);
        }
        Mat<T> ret(height(), width());
        rep(i, height()) rep(j, width()) ret[i][j] = (_dat[i][j] + b[i][j]) % mod;
        return ret;
    }
    Mat<T> operator-(Mat &b) {
        if (height() != b.height() || width() != b.width()) {
            cout << "Minus Error" << endl;
            exit(1);
        }
        Mat<T> ret(height(), width());
        rep(i, height()) rep(j, width()) ret[i][j] = (_dat[i][j] - b[i][j] + mod) % mod;
        return ret;
    }
    Mat<T> operator*(Mat b) {
        if (width() != b.height()) {
            cout << "Production Error" << endl;
            exit(1);
        }
        int h = height(), w = b.width();
        Mat<T> ret(h, w, mod);
        rep(i, h) rep(j, w) rep(k, width()) {
            ret[i][j] += (_dat[i][k] * b[k][j]) % mod;
            ret[i][j] %= mod;
        }
        return ret;
    }

    Mat<T> transpose() {
        Mat<T> ret(width(), height());
        rep(i, width()) rep(j, height()) {
            ret[i][j] = _dat[j][i];
        }
        return ret;
    }

    Mat<T> identity(int n) {
        Mat<T> idt(n, n, mod);
        rep(i, n) idt[i][i] = 1;
        return idt;
    }

    double determinant() {
        if (height() != width()) {
            cout << "Determinant Error" << endl;
            exit(1);
        }
        Mat<T> tmp = *this;
        int N = height();
        rep(i, N) {
            repr(j, i + 1, N) {
                double buf = tmp[j][i] / (double)tmp[i][i];
                rep(k, N) {
                    tmp[j][k] -= buf * tmp[i][k];
                }
            }
        }
        double det = 1;
        rep(i, N) {
            det *= tmp[i][i];
        }
        return det;
    }

    Mat<T> operator~() {
        if (height() != width()) {
            cout << "Inverse Error" << endl;
            exit(1);
        }
        int N = height();
        Mat<T> ret = identity(N);
        Mat<T> copy = *this;
        double buf;
        rep(i, N) {
            buf = 1 / (double)copy[i][i];
            rep(j, N) {
                copy[i][j] *= buf;
                ret[i][j] *= buf;
            }
            rep(j, N) {
                if (i != j) {
                    buf = copy[j][i];
                    rep(k, N) {
                        copy[j][k] -= copy[i][k] * buf;
                        ret[j][k] -= ret[i][k] * buf;
                    }
                }
            }
        }
        return ret;
    }

    void show() {
        rep(i, height()) {
            rep(j, width()) {
                cout << _dat[i][j];
                if (j != width() - 1) cout << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }
    void showSize() {
        cout << '(' << height() << ',' << width() << ')' << endl;
    }
};

template <typename T>
Mat<T> powMat(Mat<T> x, ll n) {
    Mat<T> res = x.identity(x.height());
    while (n > 0) {
        if (n & 1) res = res * x;
        x = x * x;
        n >>= 1;
    }
    return res;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

