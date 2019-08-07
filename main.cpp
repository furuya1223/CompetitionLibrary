#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
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
#define chmin(mi, val) mi = min(mi, val)
#define chmax(ma, val) ma = max(ma, val)
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
using vvd = vector<vector<double>>;
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

// vvvvvvvvvvvvvvvvvvvvvvv debug output vvvvvvvvvvvvvvvvvvvvvvv
// vector
template <typename T>
istream &operator>>(istream &is, vector<T> &vec) {
    for (T &x : vec) is >> x;
    return is;
}
// pair
template <typename T, typename U>
ostream &operator<<(ostream &os, pair<T, U> &pair_var) {
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
// map
template <typename T, typename U>
ostream &operator<<(ostream &os, map<T, U> &map_var) {
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
ostream &operator<<(ostream &os, set<T> &set_var) {
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
#define dump(...)                                                              \
    DUMPOUT << "  " << string(#__VA_ARGS__) << ": "                            \
            << "[" << to_string(__LINE__) << ":" << __FUNCTION__ << "]"        \
            << endl                                                            \
            << "    ",                                                         \
        dump_func(__VA_ARGS__)
#else
#define DEB if (false)
#define dump(...)
#endif

// ^^^^^^^^^^^^^^^^^^^^^^^ debug output ^^^^^^^^^^^^^^^^^^^^^^^

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

int dir4[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
int dir8[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                  {1, 0},   {-1, 1}, {0, 1},  {1, 1}};
char dirchar[4] = {'<', '^', '>', 'v'};

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
    using result_type =
        decltype(std::declval<F>()(std::declval<typename A::value_type>()));
    vector<result_type> y(v.size());
    std::transform(std::cbegin(v), std::cend(v), std::begin(y), f);
    return y;
}

// 多次元vector生成
template <class T>
vector<T> make_v(size_t size, const T &init) {
    return vector<T>(size, init);
}
template <class... Ts>
auto make_v(size_t size, Ts... rest) {
    return vector<decltype(make_v(rest...))>(size, make_v(rest...));
}

template <typename T>
T Max(vector<T> a) {
    return *max_element(all(a));
}
template <typename T>
T Min(vector<T> a) {
    return *min_element(all(a));
}
template <typename T>
T Sum(vector<T> a) {
    return accumulate(all(a), (T)0);
}

// mapでカウントとかする
template <typename T>
void Add(map<T, int> &m, T item) {
    if (m.find(item) == m.end()) {
        m[item] = 1;
    } else {
        m[item]++;
    }
}

// デフォルト値つきのmapのget
template <typename T, typename U>
U Get(map<T, U> m, T key, U def) {
    if (m.find(key) == m.end()) {
        return def;
    } else {
        return m[key];
    }
}

template <typename T>
bool Contains(set<T> t, const T &key) {
    return t.find(key) != t.end();
}

template <typename T, typename U>
bool Contains(map<T, U> t, const T &key) {
    return t.find(key) != t.end();
}

template <class T>
struct Edge {
    int from, to;
    T cost;
    bool operator<(Edge e) {
        return cost < e.cost;
    }
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
    Graph(int n, bool directed = false)
        : n(n), directed(directed), edges(vector<vector<Edge<T>>>(n)) {}

    void add_edge(int s, int t, T cost) {
        edges[s].push_back(Edge<T>{s, t, cost});
        if (!directed) {
            edges[t].push_back(Edge<T>{t, s, cost});
        }
    }
    Graph() {}

    vector<Edge<T>> operator[](size_t i) const {
        return edges[i];
    }

    int size() const {
        return n;
    }
};

//======================================================

template <class T>
struct augEdge {
    T from, to;
    ll cost;
    bool operator<(augEdge<T> e) {
        return cost < e.cost;
    }
    bool operator>(augEdge<T> e) {
        return cost > e.cost;
    }
};

template <class T>
void augDijkstra(map<T, vector<augEdge<T>>> graph, T start, map<T, ll> &cost) {
    pql<pair<ll, T>> Q;

    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        T pos = Q.top().second;
        ll d = Q.top().first;
        Q.pop();
        if (d > cost[pos]) continue;
        rep(i, graph[pos].size()) {
            T to = graph[pos][i].to;
            ll c = graph[pos][i].cost;
            if (cost.find(to) == cost.end() || cost[to] > cost[pos] + c) {
                cost[to] = cost[pos] + c;
                Q.push(mp(cost[to], to));
            }
        }
    }
}

map<string, vector<augEdge<string>>> G_car, G_cart;
map<string, pair<ll, bool>> roads;
map<string, vector<pair<string, string>>> crosses;
map<string, pair<string, ll>> parks;
map<string, pair<string, ll>> dests;
vector<string> nodes;

// 道路情報
// 道路ID→始点交差点、終点交差点、駐車可能箇所リスト、配達先リスト
struct RoadInfo {
    string start_cross, end_cross;
    vector<pair<ll, string>> park_dest; // (Sからの距離, ID)
};
map<string, RoadInfo> road_info_se, road_info_es, road_info_cart;

int main() {
    // std::random_device seed_gen;
    // std::mt19937 engine(seed_gen());
    int N_road, N_cross, N_park, N_dest, N_query;
    cin >> N_road >> N_cross >> N_park >> N_dest >> N_query;

    // 道路
    rep(i, N_road) {
        string r;
        ll l, f;
        cin >> r >> l >> f;
        roads[r] = mp(l, f == 1);
        road_info_se[r] = RoadInfo();
        road_info_cart[r] = RoadInfo();
        if (f == 0) {
            road_info_es[r] = RoadInfo();
        }
    }

    // 交差点
    rep(i, N_cross) {
        string c, r_in, r_out;
        cin >> c >> r_in >> r_out;
        if (!Contains(crosses, c)) {
            crosses[c] = vector<pair<string, string>>();
        }
        crosses[c].push_back(mp(r_in, r_out));
        if (r_in.substr(4, 2) == "SE") {
            road_info_cart[r_in.substr(0, 4)].end_cross = c;
        } else {
            road_info_cart[r_in.substr(0, 4)].start_cross = c;
        }
        if (r_out.substr(4, 2) == "SE") {
            road_info_cart[r_out.substr(0, 4)].start_cross = c;
        } else {
            road_info_cart[r_out.substr(0, 4)].end_cross = c;
        }
    }

    // 駐車可能箇所
    rep(i, N_park) {
        string p, r;
        ll s;
        cin >> p >> r >> s;
        parks[p] = mp(r, s);
        if (p.substr(4, 2) == "SE") {
            road_info_se[r].park_dest.push_back(mp(s, p));
        } else {
            road_info_es[r].park_dest.push_back(mp(s, p));
        }
        road_info_cart[r].park_dest.push_back(mp(s, p));
        nodes.push_back(p);
    }

    // 配達先
    rep(i, N_dest) {
        string d, r;
        ll s;
        cin >> d >> r >> s;
        dests[d] = mp(r, s);
        road_info_cart[r].park_dest.push_back(mp(s, d));
        nodes.push_back(d);
    }

    // グラフ構築

    // 車両用グラフ（駐車可能箇所、道路の端点がノード）
    // 順方向
    for (auto road_info_pair : road_info_se) {
        string road = road_info_pair.first;
        RoadInfo road_info = road_info_pair.second;

        // 始点・終点を追加
        road_info.park_dest.push_back(mp(-1, road + "SES"));
        road_info.park_dest.push_back(mp(roads[road].first + 1, road + "SEE"));

        // S からの距離でソート
        sort(all(road_info.park_dest));
        road_info.park_dest[0] = mp(0, road + "SES");
        road_info.park_dest.back() = mp(roads[road].first, road + "SEE");

        // グラフに辺を追加
        rep(i, road_info.park_dest.size() - 1) {
            auto from_dist_pos = road_info.park_dest[i];
            ll from_dist = from_dist_pos.first;
            string from_pos = from_dist_pos.second;
            auto to_dist_pos = road_info.park_dest[i + 1];
            ll to_dist = to_dist_pos.first;
            string to_pos = to_dist_pos.second;
            ll dist = to_dist - from_dist;
            assert(dist >= 0);

            if (!Contains(G_car, from_pos)) {
                G_car[from_pos] = vector<augEdge<string>>();
            }
            G_car[from_pos].push_back(augEdge<string>{from_pos, to_pos, dist});
        }
    }

    // 逆方向（一方通行でない道路のみ）
    for (auto road_info_pair : road_info_es) {
        string road = road_info_pair.first;
        RoadInfo road_info = road_info_pair.second;

        // 始点・終点を追加
        road_info.park_dest.push_back(mp(-1, road + "ESS"));
        road_info.park_dest.push_back(mp(roads[road].first + 1, road + "ESE"));

        // S からの距離でソート
        sort(all(road_info.park_dest));
        road_info.park_dest[0] = mp(0, road + "ESS");
        road_info.park_dest.back() = mp(roads[road].first, road + "ESE");

        // グラフに辺を追加
        rep(i, road_info.park_dest.size() - 1) {
            auto from_dist_pos = road_info.park_dest[i + 1];
            ll from_dist = from_dist_pos.first;
            string from_pos = from_dist_pos.second;
            auto to_dist_pos = road_info.park_dest[i];
            ll to_dist = to_dist_pos.first;
            string to_pos = to_dist_pos.second;
            ll dist = from_dist - to_dist;
            assert(dist >= 0);

            // 車両用グラフ
            if (!Contains(G_car, from_pos)) {
                G_car[from_pos] = vector<augEdge<string>>();
            }
            G_car[from_pos].push_back(augEdge<string>{from_pos, to_pos, dist});
        }
    }

    // 交差点の接続情報
    for (auto cross_info : crosses) {
        for (auto info : cross_info.second) {
            // R003SE R004SE なら、R003SEE->R004SES となる
            string from = info.first + info.first[5];
            string to = info.second + info.second[4];
            if (!Contains(G_car, from)) {
                G_car[from] = vector<augEdge<string>>();
            }
            G_car[from].push_back(augEdge<string>{from, to, 0});
        }
    }

    // 台車用グラフ
    for (auto road_info_pair : road_info_cart) {
        string road = road_info_pair.first;
        RoadInfo road_info = road_info_pair.second;

        // 始点・終点を追加
        road_info.park_dest.push_back(mp(0, road_info.start_cross));
        road_info.park_dest.push_back(
            mp(roads[road].first, road_info.end_cross));

        sort(all(road_info.park_dest));

        // グラフに辺を追加
        rep(i, road_info.park_dest.size() - 1) {
            auto from_dist_pos = road_info.park_dest[i];
            ll from_dist = from_dist_pos.first;
            string from_pos = from_dist_pos.second;
            auto to_dist_pos = road_info.park_dest[i + 1];
            ll to_dist = to_dist_pos.first;
            string to_pos = to_dist_pos.second;
            ll dist = to_dist - from_dist;
            assert(dist >= 0);

            if (!Contains(G_cart, from_pos)) {
                G_cart[from_pos] = vector<augEdge<string>>();
            }
            G_cart[from_pos].push_back(augEdge<string>{from_pos, to_pos, dist});

            if (!Contains(G_cart, to_pos)) {
                G_cart[to_pos] = vector<augEdge<string>>();
            }
            G_cart[to_pos].push_back(augEdge<string>{to_pos, from_pos, dist});
        }
    }
    // デバッグ
    DEB {
        // for (auto pos_edge : G_car) {
        //     string pos = pos_edge.first;
        //     for (auto edge : G_car[pos]) {
        //         cout << pos << " " << edge.to << " " << edge.cost << endl;
        //     }
        // }
        // for (auto pos_edge : G_cart) {
        //     string pos = pos_edge.first;
        //     for (auto edge : G_cart[pos]) {
        //         cout << pos << " " << edge.to << " " << edge.cost << endl;
        //     }
        // }
    }

    // park, dest 間の最短距離を事前に計算
    map<pair<string, string>, ll> dist_car, dist_cart;
    for (auto from : nodes) {
        if (from.size() == 4) continue;
        map<string, ll> cost_car;
        augDijkstra(G_car, from, cost_car);
        for (auto to : nodes) {
            if (to.size() == 4) continue;
            if (Contains(cost_car, to)) {
                dist_car[mp(from, to)] = cost_car[to];
            } else {
                dist_car[mp(from, to)] = -1;
            }
        }
    }

    for (auto from : nodes) {
        map<string, ll> cost_cart;
        augDijkstra(G_cart, from, cost_cart);
        for (auto to : nodes) {
            if (Contains(cost_cart, to)) {
                dist_cart[mp(from, to)] = cost_cart[to];
            } else {
                dist_cart[mp(from, to)] = -1;
            }
        }
    }

    // DEB {
    //     for (auto pair_dist : dist_car) {
    //         cout << pair_dist.first.first << " " << pair_dist.first.second
    //              << " " << pair_dist.second << endl;
    //     }
    //     cout << endl;
    //     for (auto pair_dist : dist_cart) {
    //         cout << pair_dist.first.first << " " << pair_dist.first.second
    //              << " " << pair_dist.second << endl;
    //     }
    // }

    // クエリ
    rep(i, N_query) {
        int q;
        string from, to;
        cin >> q >> from >> to;
        if (q == 0) {
            // 車両
            cout << dist_car[mp(from, to)] << endl;
        } else {
            // 台車
            cout << dist_cart[mp(from, to)] << endl;
        }
    }

    return 0;
}
