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


# :warning: library/library.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/library.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

int digit_sum(long long n, long long b) {
    int sum = 0;
    while (n > 0) {
        sum += n % b;
        n /= b;
    }
    return sum;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double combi(long long n, long long r) {
    int i;
    double p = 1;
    for (i = 1; i <= r; i++) p = (double)p * (n - i + 1) / i;
    return p;
}

long long powMod(long long x, long long n, long long mod) {
    long long res = 1;
    while (n > 0) {
        if (n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

// 最大公約数
int gcd(int a, int b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// 階乗・組み合わせのモジュラ逆数
class FactorialMod {
    // モジュラ逆数を求める(modが素数と仮定)
    void ModInv() {
        inv[0] = 0;
        inv[1] = 1;
        for (int i = 2; i <= n; i++) {
            inv[i] = mod - ((mod / i) * inv[mod % i] % mod);
        }
    }

    // n!をmodで割った余り
    void FacInv() {
        fac[0] = facinv[0] = 1;
        for (int i = 1; i <= n; i++) {
            fac[i] = (fac[i - 1] * i) % mod;
            facinv[i] = (facinv[i - 1] * (int)inv[i]) % mod;
        }
    }

  public:
    int n;
    int mod;
    vector<long long> inv;
    vector<long long> fac;
    vector<long long> facinv;

    FactorialMod(int _n, int _mod)
        : n(_n), mod(_mod), inv(vector<long long>(_n + 1)),
          fac(vector<long long>(_n + 1)), facinv(vector<long long>(_n + 1)) {
        ModInv();
        FacInv();
    }

    ll CombiMod(int r, int k) {
        if (r == 0 && k == 0) return 1;
        if (r <= 0 || k < 0 || k > r) return 0;
        if (k == 0) return 1;

        return (((fac[r] * facinv[k]) % mod) * facinv[r - k]) % mod;
    }
};

// 座標圧縮
int compress(vector<int> x, map<int, int> &zip, vector<int> &unzip) {
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());
    for (int i = 0; i < x.size(); i++) {
        zip[x[i]] = i;
        unzip[i] = x[i];
    }
    return x.size();
}

vector<int> eratosthenes(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    repr(i, 2, n + 1) {
        if (!is_prime[i]) continue;
        repr(j, 2, n / i + 1) {
            is_prime[i * j] = false;
        }
    }
    vector<int> prime;
    rep(i, n + 1) {
        if (is_prime[i]) prime.push_back(i);
    }
    return prime;
}

int divisor_num(long long n, vector<int> &prime) {
    if (n == 0) return 0;
    if (prime.size() == 0) {
        prime = eratosthenes((int)sqrt(n));
    }
    int ans = 1;
    for (auto p : prime) {
        int s = 0;
        while (n % p == 0) {
            s++;
            n /= p;
        }
        ans *= s + 1;
    }
    return ans;
}

int popcount(int bits) {
    bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
    return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}

template <typename T>
struct cumsum {
    vector<long long> data;

    cumsum(vector<T> a) {
        data = vector<long long>(a.size() + 1, 0);
        for (size_t i = 0; i < a.size(); i++) {
            data[i + 1] = data[i] + a[i];
        }
    }

    long long calculate(int l, int r) {
        if (l > r) return 0;
        l = min<int>(data.size() - 1, max<int>(0, l));
        r = min<int>(data.size() - 1, max<int>(0, r));
        return data[r] - data[l];
    }
};

void Z_algorithm(string str, vector<int> &Z) {
    const int L = str.length();
    for (int i = 1, left = 0, right = 0; i < L; i++) {
        if (i > right) {
            left = right = i;
            for (; right < L && str[right - left] == str[right]; right++)
                ;
            Z[i] = right - left;
            right--;
        } else {
            int k = i - left;
            if (Z[k] < right - i + 1) {
                Z[i] = Z[k];
            } else {
                left = i;
                for (; right < L && str[right - left] == str[right]; right++)
                    ;
                Z[i] = right - left;
                right--;
            }
        }
    }
}

// 転倒数を求める
long long merge_count(vector<long long> &a) {
    int n = a.size();
    if (n <= 1) return 0;
    long long cnt = 0;
    vector<long long> b(a.begin(), a.begin() + n / 2);
    vector<long long> c(a.begin() + n / 2, a.end());

    cnt += merge_count(b);
    cnt += merge_count(c);

    int ai = 0, bi = 0, ci = 0;

    while (ai < n) {
        if (bi < b.size() && (ci == c.size() || b[bi] < c[ci])) {
            a[ai] = b[bi];
            bi++;
        } else {
            cnt += n / 2 - bi;
            a[ai] = c[ci];
            ci++;
        }
        ai++;
    }
    return cnt;
}

// LIS O(NlogN)
#define index_of(as, x)                                                        \
    distance(as.begin(), lower_bound(as.begin(), as.end(), x))
vector<int> lis(const vector<int> &a) {
    const int n = a.size();
    vector<int> A(n, INF);
    vector<int> id(n);
    for (int i = 0; i < n; ++i) {
        id[i] = index_of(A, a[i]);
        A[id[i]] = a[i];
    }
    int m = *max_element(id.begin(), id.end());
    vector<int> b(m + 1);
    for (int i = n - 1; i >= 0; --i)
        if (id[i] == m) b[m--] = a[i];
    return b;
}

// 素数テーブル O(N log log N)
void makePrimeTable(vector<bool> &prime) {
    fill(all(prime), true);
    prime[0] = false;
    prime[1] = false;
    rep(i, prime.size()) {
        if (!prime[i]) continue;
        repr(j, 2, (prime.size() - 1) / i + 1) {
            prime[i * j] = false;
        }
    }
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


#line 2 "library/library.hpp"

int digit_sum(long long n, long long b) {
    int sum = 0;
    while (n > 0) {
        sum += n % b;
        n /= b;
    }
    return sum;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double combi(long long n, long long r) {
    int i;
    double p = 1;
    for (i = 1; i <= r; i++) p = (double)p * (n - i + 1) / i;
    return p;
}

long long powMod(long long x, long long n, long long mod) {
    long long res = 1;
    while (n > 0) {
        if (n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

// 最大公約数
int gcd(int a, int b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// 階乗・組み合わせのモジュラ逆数
class FactorialMod {
    // モジュラ逆数を求める(modが素数と仮定)
    void ModInv() {
        inv[0] = 0;
        inv[1] = 1;
        for (int i = 2; i <= n; i++) {
            inv[i] = mod - ((mod / i) * inv[mod % i] % mod);
        }
    }

    // n!をmodで割った余り
    void FacInv() {
        fac[0] = facinv[0] = 1;
        for (int i = 1; i <= n; i++) {
            fac[i] = (fac[i - 1] * i) % mod;
            facinv[i] = (facinv[i - 1] * (int)inv[i]) % mod;
        }
    }

  public:
    int n;
    int mod;
    vector<long long> inv;
    vector<long long> fac;
    vector<long long> facinv;

    FactorialMod(int _n, int _mod)
        : n(_n), mod(_mod), inv(vector<long long>(_n + 1)),
          fac(vector<long long>(_n + 1)), facinv(vector<long long>(_n + 1)) {
        ModInv();
        FacInv();
    }

    ll CombiMod(int r, int k) {
        if (r == 0 && k == 0) return 1;
        if (r <= 0 || k < 0 || k > r) return 0;
        if (k == 0) return 1;

        return (((fac[r] * facinv[k]) % mod) * facinv[r - k]) % mod;
    }
};

// 座標圧縮
int compress(vector<int> x, map<int, int> &zip, vector<int> &unzip) {
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());
    for (int i = 0; i < x.size(); i++) {
        zip[x[i]] = i;
        unzip[i] = x[i];
    }
    return x.size();
}

vector<int> eratosthenes(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    repr(i, 2, n + 1) {
        if (!is_prime[i]) continue;
        repr(j, 2, n / i + 1) {
            is_prime[i * j] = false;
        }
    }
    vector<int> prime;
    rep(i, n + 1) {
        if (is_prime[i]) prime.push_back(i);
    }
    return prime;
}

int divisor_num(long long n, vector<int> &prime) {
    if (n == 0) return 0;
    if (prime.size() == 0) {
        prime = eratosthenes((int)sqrt(n));
    }
    int ans = 1;
    for (auto p : prime) {
        int s = 0;
        while (n % p == 0) {
            s++;
            n /= p;
        }
        ans *= s + 1;
    }
    return ans;
}

int popcount(int bits) {
    bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
    return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}

template <typename T>
struct cumsum {
    vector<long long> data;

    cumsum(vector<T> a) {
        data = vector<long long>(a.size() + 1, 0);
        for (size_t i = 0; i < a.size(); i++) {
            data[i + 1] = data[i] + a[i];
        }
    }

    long long calculate(int l, int r) {
        if (l > r) return 0;
        l = min<int>(data.size() - 1, max<int>(0, l));
        r = min<int>(data.size() - 1, max<int>(0, r));
        return data[r] - data[l];
    }
};

void Z_algorithm(string str, vector<int> &Z) {
    const int L = str.length();
    for (int i = 1, left = 0, right = 0; i < L; i++) {
        if (i > right) {
            left = right = i;
            for (; right < L && str[right - left] == str[right]; right++)
                ;
            Z[i] = right - left;
            right--;
        } else {
            int k = i - left;
            if (Z[k] < right - i + 1) {
                Z[i] = Z[k];
            } else {
                left = i;
                for (; right < L && str[right - left] == str[right]; right++)
                    ;
                Z[i] = right - left;
                right--;
            }
        }
    }
}

// 転倒数を求める
long long merge_count(vector<long long> &a) {
    int n = a.size();
    if (n <= 1) return 0;
    long long cnt = 0;
    vector<long long> b(a.begin(), a.begin() + n / 2);
    vector<long long> c(a.begin() + n / 2, a.end());

    cnt += merge_count(b);
    cnt += merge_count(c);

    int ai = 0, bi = 0, ci = 0;

    while (ai < n) {
        if (bi < b.size() && (ci == c.size() || b[bi] < c[ci])) {
            a[ai] = b[bi];
            bi++;
        } else {
            cnt += n / 2 - bi;
            a[ai] = c[ci];
            ci++;
        }
        ai++;
    }
    return cnt;
}

// LIS O(NlogN)
#define index_of(as, x)                                                        \
    distance(as.begin(), lower_bound(as.begin(), as.end(), x))
vector<int> lis(const vector<int> &a) {
    const int n = a.size();
    vector<int> A(n, INF);
    vector<int> id(n);
    for (int i = 0; i < n; ++i) {
        id[i] = index_of(A, a[i]);
        A[id[i]] = a[i];
    }
    int m = *max_element(id.begin(), id.end());
    vector<int> b(m + 1);
    for (int i = n - 1; i >= 0; --i)
        if (id[i] == m) b[m--] = a[i];
    return b;
}

// 素数テーブル O(N log log N)
void makePrimeTable(vector<bool> &prime) {
    fill(all(prime), true);
    prime[0] = false;
    prime[1] = false;
    rep(i, prime.size()) {
        if (!prime[i]) continue;
        repr(j, 2, (prime.size() - 1) / i + 1) {
            prime[i * j] = false;
        }
    }
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

