#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
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
#define chmin(mi, value) mi = min(mi, value)
#define chmax(ma, value) ma = max(ma, value)
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
// #define MOD 998244353
#define PI 3.141592653589793238
#define RMAX 4294967295

using vi = vector<int>;
using vvi = vector<vector<int>>;
using vvvi = vector<vector<vector<int>>>;
using vvvvi = vector<vector<vector<vector<int>>>>;
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vector<ll>>;
using vvvll = vector<vector<vector<ll>>>;
using vd = vector<double>;
using vvd = vector<vd>;
using vvvd = vector<vvd>;
using vvvvd = vector<vvvd>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using vvs = vector<vector<string>>;
using Pi = pair<int, int>;
using vPi = vector<Pi>;
using vvPi = vector<vector<Pi>>;
using vvvPi = vector<vector<vector<Pi>>>;
using vvvvPi = vector<vector<vector<vector<Pi>>>>;
using Pll = pair<ll, ll>;
using vPll = vector<Pll>;
using Pd = pair<double, double>;
using vPd = vector<Pd>;
template <class T>
using vec = vector<T>;
template <class T>
using pql = priority_queue<T, vector<T>, greater<T>>;
using Comp = complex<double>;

// vvvvvvvvvvvvvvvvvvvvvvv debug output vvvvvvvvvvvvvvvvvvvvvvv
// vector input
template <typename T>
istream &operator>>(istream &is, vector<T> &vec) {
    for (T &x : vec) is >> x;
    return is;
}
// pair
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &pair_var) {
    os << "(" << pair_var.first << ", " << pair_var.second << ")";
    return os;
}
// vector
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &vec) {
    os << "{";
    for (int i = 0; i < vec.size(); i++) {
        os << vec[i] << (i + 1 == vec.size() ? "" : ", ");
    }
    os << "}";
    return os;
}
// deque
template <typename T>
ostream &operator<<(ostream &os, const deque<T> &vec) {
    os << "{";
    for (int i = 0; i < vec.size(); i++) {
        os << vec[i] << (i + 1 == vec.size() ? "" : ", ");
    }
    os << "}";
    return os;
}
// map
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &map_var) {
    os << "{";
    repi(itr, map_var) {
        os << *itr;
        itr++;
        if (itr != map_var.end()) os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
// set
template <typename T>
ostream &operator<<(ostream &os, const set<T> &set_var) {
    os << "{";
    repi(itr, set_var) {
        os << *itr;
        itr++;
        if (itr != set_var.end()) os << ", ";
        itr--;
    }
    os << "}";
    return os;
}
// multiset
template <typename T>
ostream &operator<<(ostream &os, const multiset<T> &set_var) {
    os << "{";
    repi(itr, set_var) {
        os << *itr;
        itr++;
        if (itr != set_var.end()) os << ", ";
        itr--;
    }
    os << "}";
    return os;
}

#define DUMPOUT cerr

void dump_func() {
    DUMPOUT << endl;
}
template <class Head, class... Tail>
void dump_func(Head &&head, Tail &&... tail) {
    DUMPOUT << head;
    if (sizeof...(Tail) > 0) {
        DUMPOUT << ", ";
    }
    dump_func(std::move(tail)...);
}
#ifdef DEBUG_
#define DEB
#define dump(...)                                                                                  \
    DUMPOUT << "  " << string(#__VA_ARGS__) << ": "                                                \
            << "[" << to_string(__LINE__) << ":" << __FUNCTION__ << "]" << endl                    \
            << "    ",                                                                             \
        dump_func(__VA_ARGS__)
#else
#define DEB if (false)
#define dump(...)
#endif

// ^^^^^^^^^^^^^^^^^^^^^^^ debug output ^^^^^^^^^^^^^^^^^^^^^^^

random_device seed_gen;
mt19937 engine(seed_gen());

string YN(bool y, int id = 0) {
    if (id) cout << id;
    return (y ? "YES" : "NO");
}
string yn(bool y, int id = 0) {
    if (id) cout << id;
    return (y ? "Yes" : "No");
}
string ON(bool y, int id = 0) {
    if (id) cout << id;
    return (y ? "OK" : "NG");
}

int dir4[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
vector<char> dirchar = {'R', 'L', 'D', 'U'};
int dir8[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

// [a,b)
int irand(int a, int b) {
    static mt19937 Rand(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> dist(a, b - 1);
    return dist(Rand);
}

// [a,b)
double drand(int a, int b) {
    static mt19937 Rand(static_cast<unsigned int>(time(nullptr)));
    uniform_real_distribution<double> dist(a, b);
    return dist(Rand);
}

// https://qiita.com/IgnorantCoder/items/3101d6276e9bdddf872c
template <typename A, typename F>
inline auto transform(const A &v, F &&f) {
    using result_type = decltype(std::declval<F>()(std::declval<typename A::value_type>()));
    vector<result_type> y(v.size());
    std::transform(std::cbegin(v), std::cend(v), std::begin(y), f);
    return y;
}

// generate vector which has multiple dimension
template <class T>
vector<T> make_v(size_t size, const T &init) {
    return vector<T>(size, init);
}
template <class... Ts>
auto make_v(size_t size, Ts... rest) {
    return vector<decltype(make_v(rest...))>(size, make_v(rest...));
}

template <typename T>
T Max(const vector<T> &a) {
    return *max_element(all(a));
}
template <typename T>
T Min(const vector<T> &a) {
    return *min_element(all(a));
}
template <typename T>
T Sum(const vector<T> &a) {
    return accumulate(all(a), (T)0);
}

// for counting using map
template <typename T>
void Add(map<T, int> &m, T item) {
    if (m.find(item) == m.end()) {
        m[item] = 1;
    } else {
        m[item]++;
    }
}

// for counting using map
template <typename T>
void Erase(map<T, int> &m, T item) {
    if (m.find(item) == m.end()) {
    } else {
        m[item]--;
        if (m[item] == 0) {
            m.erase(item);
        }
    }
}

// get method for map with default value
template <typename T, typename U>
U Get(map<T, U> m, T key, U def) {
    if (m.find(key) == m.end()) {
        return def;
    } else {
        return m[key];
    }
}

template <typename T>
inline bool Contains(const set<T> &t, const T &key) {
    return t.find(key) != t.end();
}

template <typename T, typename U>
inline bool Contains(const map<T, U> &t, const T &key) {
    return t.find(key) != t.end();
}

template <class T>
struct Edge {
    int from, to;
    T cost;
    Edge(int f, int t, T c) : from(f), to(t), cost(c) {}
    Edge() {}
};
template <class T>
bool operator<(const Edge<T> e1, const Edge<T> e2) {
    return e1.cost < e2.cost || (e1.cost == e2.cost && e1.from < e2.from) ||
           (e1.cost == e2.cost && e1.from == e2.from && e1.to < e2.to);
}

template <class T>
ostream &operator<<(ostream &os, const Edge<T> &edge) {
    os << "(" << edge.from << "->" << edge.to << ":" << edge.cost << ")";
    return os;
}

template <class T = int>
class Graph {
    int n;
    bool directed;

  public:
    vector<vector<Edge<T>>> edges;
    Graph(int n, bool directed) : n(n), directed(directed), edges(vector<vector<Edge<T>>>(n)) {}
    Graph() {}

    void add_edge(int s, int t, T cost) {
        edges[s].emplace_back(s, t, cost);
        if (!directed) {
            edges[t].emplace_back(t, s, cost);
        }
    }
    void add_edge(Edge<T> e) {
        edges[e.from].push_back(e);
        if (!directed) {
            edges[e.to].emplace_back(e.to, e.from, e.cost);
        }
    }

    vector<Edge<T>> &operator[](size_t i) {
        return edges[i];
    }
    int size() const {
        return n;
    }
};

//======================================================


int main(void) {
    

    return 0;
}
