#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
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
//#define MOD 1000000007
#define MOD 998244353
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
    static mt19937 Rand(static_cast<unsigned int>(std::time(nullptr)));
    uniform_int_distribution<int> dist(a, b - 1);
    return dist(Rand);
}

// [a,b)
double drand(int a, int b) {
    static mt19937 Rand(static_cast<unsigned int>(std::time(nullptr)));
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

chrono::system_clock::time_point start;
int kappa, lambda;
vector<tuple<string, double, double>> parks;
vector<tuple<string, double, int>> dests;
vector<vector<double>> dist_car, dist_cart;
vector<vector<int>> nearest;

struct Route {
    vector<int> park_order;
    vector<vector<int>> dest_order;
    Route()
        : park_order(vector<int>(kappa)),
          dest_order(vector<vector<int>>(kappa)) {
        rep(i, kappa) {
            dest_order[i].reserve(lambda);
        }
    }
};

inline double get_elapsed_sec() {
    return static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(
                                   std::chrono::system_clock::now() - start)
                                   .count()) /
           1000000000;
}

void output_route(Route route, int finish_i, int finish_j) {
    rep(i, finish_i + 1) {
        int park = route.park_order[i];
        if (route.dest_order[park].size() == 0) continue;
        cout << get<0>(parks[park]) << " ";
        int count = 0;
        rep(j, (i == finish_i ? finish_j + 1 : route.dest_order[park].size())) {
            int dest = route.dest_order[park][j];
            if (count == 10) {
                cout << endl
                     << get<0>(parks[park]) << " " << get<0>(dests[dest])
                     << " ";
                count = 0;
            } else {
                cout << get<0>(dests[dest]) << " ";
                count++;
            }
        }
        cout << endl;
    }
}

tuple<double, int, int> score(Route route) {
    double current_time = 0;
    int prev_park = -1;
    int finish_i = kappa - 1;
    int finish_j = route.dest_order[route.park_order.back()].size() - 1;
    int complete = 0;
    bool finish = false;

    // 配達
    rep(i, kappa) {
        int park = route.park_order[i]; // 次の駐車可能箇所

        if (route.dest_order[park].size() == 0) continue;
        if (prev_park != -1) {
            if (dist_car[prev_park][park] < -EPS) {
                // 移動不可能
                return mt(-1, -1, -1);
            }
            current_time += dist_car[prev_park][park]; // 車両での移動
            current_time += get<2>(parks[park]);       // 降車
        }

        // 配達先
        int prev_dest = park;
        int count = 0;
        rep(j, route.dest_order[park].size()) {
            int dest = route.dest_order[park][j] + kappa;
            if (count == 10) {
                // 一度車両に戻る
                current_time += dist_cart[prev_dest - kappa][park]; // 移動時間
                prev_dest = park;
                j--;
                count = 0;
                continue;
            }

            // 移動
            current_time += dist_cart[dest - kappa][prev_dest];
            // 滞在時間
            current_time += get<1>(dests[dest - kappa]);
            if (get<2>(dests[dest - kappa]) != 0 &&
                current_time > get<2>(dests[dest - kappa])) {
                // 時間切れ
                return mt(-1, -1, -1);
            }
            if (current_time > 240 - EPS) {
                finish = true;
                break;
            }
            // 配達完了
            complete++;
            count++;
            finish_i = i, finish_j = j;
            if (complete == lambda) {
                // すべて配達
                finish = true;
                break;
            }
            prev_dest = dest;
        }
        if (finish) break;

        // 車両に戻る
        if (i != kappa - 1) {
            current_time +=
                dist_cart[route.dest_order[park].back()][park]; // 車両へ移動
            current_time += get<1>(parks[park]);                // 乗車
        }
        prev_park = park;
    }
    // 配達制限チェック
    repr(i, finish_i + 1, kappa) {
        int start_j = 0;
        repr(j, (i == finish_i ? finish_j + 1 : 0),
             route.dest_order[route.park_order[i]].size()) {
            if (get<2>(dests[route.dest_order[route.park_order[i]][j]]) != 0) {
                return mt(-1, -1, -1);
            }
        }
    }

    return mt(complete * 10000.0 + 14400 - current_time * 60, finish_i,
              finish_j);
}

Route sample_route() {
    Route route;
    route.park_order = {1, 0, 4, 2, 3};
    route.dest_order = {
        {0, 3, 7, 8, 11, 14, 10, 9, 5, 6, 2}, {4, 1}, {}, {}, {13, 12}};
    return route;
}

Route random_change(Route route) {
    int mode = irand(0, 3);
    if (mode == 0) {
        // park を swap
        int i = irand(0, kappa);
        int j;
        do {
            j = irand(0, kappa);
        } while (i == j);
        swap(route.park_order[i], route.park_order[j]);
    } else if (mode == 1) {
        // park 内で dest を swap
        int p;
        rep(_, 100) {
            p = irand(0, kappa);
            if (route.dest_order[p].size() >= 2) break;
        }
        if (route.dest_order[p].size() < 2) return route;
        int i = irand(0, route.dest_order[p].size());
        int j;
        do {
            j = irand(0, route.dest_order[p].size());
        } while (i == j);
        swap(route.dest_order[p][i], route.dest_order[p][j]);
    } else {
        // p1 の dest を p2 に入れる
        int p1;
        do {
            p1 = irand(0, kappa);
        } while (route.dest_order[p1].size() == 0);
        int p2;
        do {
            p2 = irand(0, kappa);
        } while (p1 == p2);
        int i = irand(0, route.dest_order[p1].size());
        if (i != route.dest_order[p1].size() - 1) {
            swap(route.dest_order[p1][i], route.dest_order[p1].back());
        }
        route.dest_order[p2].push_back(route.dest_order[p1].back());
        route.dest_order[p1].resize(route.dest_order[p1].size() - 1);
    }
    return route;
}

Route search(Route current_route, double second) {
    double current_score = get<0>(score(current_route));
    while (get_elapsed_sec() < second) {
        Route next_route = random_change(current_route);
        double next_score = get<0>(score(next_route));

        if (current_score < 0 || next_score > current_score) {
            dump(next_score);
            current_route = next_route;
            current_score = next_score;
        }
    }
    return current_route;
}

int main() {
    start = std::chrono::system_clock::now();
    cin >> kappa >> lambda;
    parks = vector<tuple<string, double, double>>(kappa);
    dests = vector<tuple<string, double, int>>(lambda);
    vector<pair<int, int>> sort_dest(lambda);

    rep(i, kappa) {
        string p;
        double t_on, t_off;
        cin >> p >> t_on >> t_off;
        parks[i] = mt(p, t_on, t_off);
    }
    rep(i, lambda) {
        string d;
        double t;
        int r;
        cin >> d >> t >> r;
        dests[i] = mt(d, t, r);
        sort_dest[i] = mp((r == 120 ? 0 : r == 240 ? 1 : 2), i);
    }
    dist_car = make_v(kappa, kappa, 0.0);
    dist_cart = make_v(lambda, kappa + lambda, 0.0);
    rep(i, kappa) {
        rep(j, kappa) {
            double a;
            cin >> a;
            dist_car[i][j] = a / 250.0;
        }
    }
    rep(i, lambda) {
        rep(j, kappa + lambda) {
            double b;
            cin >> b;
            dist_cart[i][j] = b / 100.0;
            // dump(b, dist_cart[i][j]);
        }
    }

    // 実行可能経路を作りたい
    Route route;
    rep(i, kappa) {
        route.park_order[i] = i;
    }
    sort(all(sort_dest));
    rep(i, lambda) {
        double min_dist = INF;
        int nearest_park = 0;
        int dest = get<1>(sort_dest[i]);
        int priority = get<0>(sort_dest[i]);
        if (priority < 2) {
            route.dest_order[0].push_back(dest);
        } else {
            rep(j, kappa) {
                // dump(dist_cart[i][j], min_dist);
                if (dist_cart[dest][j] < min_dist) {
                    min_dist = dist_cart[dest][j];
                    nearest_park = j;
                    // dump(i, j, nearest_park);
                }
            }
            route.dest_order[nearest_park].push_back(dest);
        }
    }

    DEB {
        auto result = score(route);
        double score = get<0>(result);
        int finish_i = get<1>(result);
        int finish_j = get<2>(result);
        dump(score, finish_i, finish_j);
        dump(route.park_order);
        dump(route.dest_order);
    }

    Route best_route = search(route, 9.9);
    auto result = score(best_route);
    double score = get<0>(result);
    int finish_i = get<1>(result);
    int finish_j = get<2>(result);
    dump(score, finish_i, finish_j);
    dump(get_elapsed_sec());
    output_route(best_route, finish_i, finish_j);

    return 0;
}
