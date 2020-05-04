#ifndef LIBRARY_HEADER_HPP_
#define LIBRARY_HEADER_HPP_

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

#endif // LIBRARY_HEADER_HPP_