#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

using namespace std;

constexpr int TRIAL = 1;         // 試行回数
constexpr bool PARK3ALL1 = true; // 最初の 3 park を全探索するか乱択か
constexpr bool PARK3ALL2 = true; // 最初の 3 park を全探索するか乱択か
bool park3all; // 最初の 3 park を全探索するか乱択か
constexpr int FIRST_ROUTE_SEARCH = 10000; // PARK3ALL が false なら有効
int unrestricted_park_num;
constexpr int UNRESTRICTED_PARK_NUM_1 = 5; // 無制限配達先の対象 park 数
constexpr int UNRESTRICTED_PARK_NUM_2 = 5; // 無制限配達先の対象 park 数
constexpr int RETRICTED_PARK_NUM = 5;
constexpr bool USE_MODIFIED_SCORE = false; // 修正スコアを使用するか
constexpr int MAX_CHANGE_STEP = 5;
constexpr bool RANDOM_CHANGE_STEP = false; //  swap 回数をランダムにするか
constexpr bool FIXED_STEP = false;

constexpr double START_TEMP_1 = 1000;
constexpr double START_TEMP_2 = 1000;
constexpr double END_TEMP = 5;
constexpr double TIME_LIMIT = 59.99;

int benchmark_count = 0;

#define repr(i, a, b) for (int i = (int)(a); i < (int)(b); ++i)
#define rep(i, n) repr(i, 0, n)
#define reprrev(i, a, b) for (int i = (int)(b)-1; i >= (int)(a); --i)
#define reprev(i, n) reprrev(i, 0, n)
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define mp make_pair
#define mt make_tuple
#define INF 1050000000
#define EPS 0.0000000001

// vvvvvvvvvvvvvvvvvvvvvvv debug output vvvvvvvvvvvvvvvvvvvvvvv
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

random_device seed_gen;
mt19937 engine(seed_gen());

// 多次元vector生成
template <class T>
vector<T> make_v(size_t size, const T &init) {
    return vector<T>(size, init);
}
template <class... Ts>
auto make_v(size_t size, Ts... rest) {
    return vector<decltype(make_v(rest...))>(size, make_v(rest...));
}

//======================================================

chrono::system_clock::time_point start;
int kappa, lambda;
vector<tuple<string, double, double>> parks;
vector<tuple<string, double, int>> dests;
vector<vector<double>> dist_car, dist_cart;
vector<vector<int>> nearest;
vector<int> dest_0, dest_120, dest_240;

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

constexpr double nano_unit = 1.0 / 1000000000;
inline double get_elapsed_sec() {
    return static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(
                                   std::chrono::system_clock::now() - start)
                                   .count()) *
           nano_unit;
}

inline void output_route(const Route &route, int finish_i, int finish_j) {
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
                count = 1;
            } else {
                cout << get<0>(dests[dest]) << " ";
                ++count;
            }
        }
        cout << endl;
    }
}

inline bool check_route(const Route &route, const int &finish_i,
                        const int &finish_j) {
    // 配達制限チェック
    repr(j, finish_j + 1, route.dest_order[route.park_order[finish_i]].size()) {
        if (get<2>(dests[route.dest_order[route.park_order[finish_i]][j]]) !=
            0) {
            return false;
        }
    }
    repr(i, finish_i + 1, kappa) {
        rep(j, route.dest_order[route.park_order[i]].size()) {
            if (get<2>(dests[route.dest_order[route.park_order[i]][j]]) != 0) {
                return false;
            }
        }
    }
    return true;
}

inline tuple<double, int, int> score(const Route &route) {
    double current_time = 0;
    int prev_park = -1;
    int finish_i = kappa;
    int finish_j = 0;
    int complete = 0;

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
                current_time > get<2>(dests[dest - kappa]) + EPS) {
                // 時間切れ
                return mt(-1, -1, -1);
            }
            if (current_time > 240 + EPS) {
                // 現在の配達先は失敗
                finish_i = i, finish_j = j - 1;
                goto finished;
            }
            // 配達完了
            ++complete;
            ++count;
            if (complete == lambda) {
                // すべて配達
                finish_i = i, finish_j = j;
                goto finished;
            }
            prev_dest = dest;
        }

        // 車両に戻る
        if (i != kappa - 1) {
            current_time +=
                dist_cart[route.dest_order[park].back()][park]; // 車両へ移動
            current_time += get<1>(parks[park]);                // 乗車
        }
        prev_park = park;
    }
finished:
    // 配達制限チェック
    if (!check_route(route, finish_i, finish_j)) {
        return mt(-1, -1, -1);
    }

    return mt(complete * 10000.0 + 14400 - current_time * 60, finish_i,
              finish_j);
}

// 配達数を増やすようスコアを改変（探索用なのでスコアのみ返す）
inline double score_modified(const Route &route) {
    double current_time = 0;
    int prev_park = -1;
    int finish_i = kappa;
    int finish_j = 0;
    int complete = 0;
    double true_score = -1;
    double penalty = 0;
    double reward = 0;

    // 配達
    rep(i, kappa) {
        int park = route.park_order[i]; // 次の駐車可能箇所

        if (route.dest_order[park].size() == 0) continue;
        if (prev_park != -1) {
            if (dist_car[prev_park][park] < -EPS) {
                // 移動不可能
                return -1;
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
                current_time > get<2>(dests[dest - kappa]) + EPS) {
                // 時間切れ
                return -1;
            }
            if (current_time > 240 + EPS) {
                // 現在の配達先は失敗
                if (true_score < 0) {
                    true_score = complete * 10000.0 + 14400 - current_time * 60;
                    finish_i = i, finish_j = j - 1;
                } else {
                    penalty = current_time * 0.01;
                    goto finished;
                }
            }
            // 配達完了
            ++complete;
            ++count;
            if (complete == lambda) {
                // すべて配達
                finish_i = i, finish_j = j;
                goto finished;
            }
            prev_dest = dest;
        }

        // 車両に戻る
        if (i != kappa - 1) {
            current_time +=
                dist_cart[route.dest_order[park].back()][park]; // 車両へ移動
            current_time += get<1>(parks[park]);                // 乗車
        }
        prev_park = park;
    }
finished:
    // 配達制限チェック
    if (!check_route(route, finish_i, finish_j)) {
        return -1;
    }

    return true_score + reward - penalty;
}

inline vector<int> inplace_change(Route &route) {
    constexpr int mode1 = 1;
    constexpr int mode2 = mode1 + 3;
    constexpr int mode3 = mode2 + 3;
    constexpr int mode4 = mode3 + 3;

    int mode = engine() % mode4;
    if (mode < mode1) {
        // park を swap
        int i = engine() % kappa;
        int j;
        do {
            j = engine() % kappa;
        } while (i == j);
        std::swap(route.park_order[i], route.park_order[j]);
        return {0, i, j};
    } else if (mode < mode2) {
        // park 内で i, j を swap
        int p = engine() % kappa;
        while (route.dest_order[p].size() < 2) {
            p = (p + 1) % kappa;
        }
        if (route.dest_order[p].size() < 2) return {-1};
        int i = engine() % route.dest_order[p].size();
        int j;
        do {
            j = engine() % route.dest_order[p].size();
        } while (i == j);
        std::swap(route.dest_order[p][i], route.dest_order[p][j]);
        return {1, p, i, j};
    } else if (mode < mode3) {
        int p1;
        do {
            p1 = engine() % kappa;
        } while (route.dest_order[p1].size() == 0);
        int p2;
        do {
            p2 = engine() % kappa;
        } while (p1 == p2);
        // p1 の要素を選択
        int i = engine() % route.dest_order[p1].size();
        // 選択要素を p1 の末尾へ移動
        if (i != route.dest_order[p1].size() - 1) {
            std::swap(route.dest_order[p1][i], route.dest_order[p1].back());
        }
        // 選択要素を p2 に追加
        route.dest_order[p2].push_back(route.dest_order[p1].back());
        // 選択要素を p1 から削除
        route.dest_order[p1].pop_back();
        // 選択要素を p2 末尾からランダムな位置に変える
        int j = engine() % route.dest_order[p2].size();
        if (j != route.dest_order[p2].size() - 1) {
            std::swap(route.dest_order[p2][j], route.dest_order[p2].back());
        }
        return {2, p1, p2, i, j};
    } else {
        // int p1 = irand(0, kappa);
        int p1 = engine() % kappa;
        int p2;
        do {
            // p2 = irand(0, kappa);
            p2 = engine() % kappa;
        } while (p1 == p2);
        std::swap(route.dest_order[p1], route.dest_order[p2]);
        return {3, p1, p2};
    }
}

inline void undo(Route &route, const vector<int> &param) {
    if (param[0] == -1)
        return;
    else if (param[0] == 0) {
        // i, j を swap
        std::swap(route.park_order[param[1]], route.park_order[param[2]]);
    } else if (param[0] == 1) {
        // p 内で i, j を swap
        std::swap(route.dest_order[param[1]][param[2]],
                  route.dest_order[param[1]][param[3]]);
    } else if (param[0] == 2) {
        // p2 の j 要素を末尾に移動し、p1 の末尾に追加し、i 番目に移動する
        int p1 = param[1];
        int p2 = param[2];
        int i = param[3];
        int j = param[4];
        // 選択要素を p2 の末尾へ移動
        if (j != route.dest_order[p2].size() - 1) {
            std::swap(route.dest_order[p2][j], route.dest_order[p2].back());
        }
        // 選択要素を p1 に追加
        route.dest_order[p1].push_back(route.dest_order[p2].back());
        // 選択要素を p2 から削除
        route.dest_order[p2].pop_back();
        // 選択要素を p1 の i 番目へ移動
        if (i != route.dest_order[p1].size() - 1) {
            std::swap(route.dest_order[p1][i], route.dest_order[p1].back());
        }
    } else {
        std::swap(route.dest_order[param[1]], route.dest_order[param[2]]);
    }
}

// 初期ルート作成（ランダム要素あり）
Route generate_first_route() {
    Route route;
    rep(i, kappa) {
        route.park_order[i] = i;
    }
    shuffle(all(route.park_order), engine);

    shuffle(all(dest_0), engine);
    shuffle(all(dest_120), engine);
    shuffle(all(dest_240), engine);
    rep(i, lambda) {
        int dest = (i < dest_120.size()
                        ? dest_120[i]
                        : i < dest_120.size() + dest_240.size()
                              ? dest_240[i - dest_120.size()]
                              : dest_0[i - dest_120.size() - dest_240.size()]);
        if (i < dest_120.size() + dest_240.size()) {
            double min_dist = INF;
            int nearest_park = 0;
            rep(j, RETRICTED_PARK_NUM) {
                int park = route.park_order[j];
                // dump(dist_cart[i][j], min_dist);
                if (dist_cart[dest][park] < min_dist) {
                    min_dist = dist_cart[dest][park];
                    nearest_park = park;
                    // dump(i, park, nearest_park);
                }
            }
            route.dest_order[nearest_park].push_back(dest);
        } else {
            double min_dist = INF;
            int nearest_park = 0;
            rep(j, min(unrestricted_park_num, kappa)) {
                int park = route.park_order[j];
                // dump(dist_cart[i][park], min_dist);
                if (dist_cart[dest][park] < min_dist) {
                    min_dist = dist_cart[dest][park];
                    nearest_park = park;
                    // dump(i, park, nearest_park);
                }
            }
            route.dest_order[nearest_park].push_back(dest);
        }
    }
    return route;
}

// 初期ルート作成（最初の 3 park を全通り試す）
Route generate_first_route_3_parks(int top_parks[3]) {
    Route route;
    route.park_order[0] = top_parks[0];
    route.park_order[1] = top_parks[1];
    route.park_order[2] = top_parks[2];
    int index = 3;
    rep(i, kappa) {
        if (i == top_parks[0] || i == top_parks[1] || i == top_parks[2])
            continue;
        route.park_order[index] = i;
        ++index;
    }

    shuffle(route.park_order.begin() + 3, route.park_order.end(), engine);

    shuffle(all(dest_0), engine);
    shuffle(all(dest_120), engine);
    shuffle(all(dest_240), engine);
    rep(i, lambda) {
        int dest = (i < dest_120.size()
                        ? dest_120[i]
                        : i < dest_120.size() + dest_240.size()
                              ? dest_240[i - dest_120.size()]
                              : dest_0[i - dest_120.size() - dest_240.size()]);
        if (i < dest_120.size() + dest_240.size()) {
            // 時間制限あり
            double min_dist = INF;
            int nearest_park = 0;
            rep(j, 3) {
                int park = route.park_order[j];
                // dump(dist_cart[i][j], min_dist);
                if (dist_cart[dest][park] < min_dist) {
                    min_dist = dist_cart[dest][park];
                    nearest_park = park;
                    // dump(i, park, nearest_park);
                }
            }
            route.dest_order[nearest_park].push_back(dest);
        } else {
            // 時間制限なし
            double min_dist = INF;
            int nearest_park = 0;
            rep(j, min(unrestricted_park_num, kappa)) {
                int park = route.park_order[j];
                // dump(dist_cart[i][park], min_dist);
                if (dist_cart[dest][park] < min_dist) {
                    min_dist = dist_cart[dest][park];
                    nearest_park = park;
                    // dump(i, park, nearest_park);
                }
            }
            route.dest_order[nearest_park].push_back(dest);
        }
    }
    return route;
}

// 初期ルートを n 個生成して最も良いものを選ぶ
Route select_first_route(int n) {
    Route selected;
    double best_score = 0;
    rep(i, n) {
        Route route = generate_first_route();
        double s = get<0>(score(route));
        if (s > best_score) {
            selected = route;
            best_score = s;
        }
    }
    return selected;
}

Route select_first_route_3_parks() {
    Route selected;
    double best_score = 0;
    int top_parks[3];
    rep(i, kappa) {
        rep(j, kappa) {
            if (i == j) continue;
            rep(k, kappa) {
                if (k == i || k == j) continue;
                top_parks[0] = i;
                top_parks[1] = j;
                top_parks[2] = k;
                Route route = generate_first_route_3_parks(top_parks);
                double s = get<0>(score(route));
                // double s = score_modified(route).first;
                if (s > best_score) {
                    selected = route;
                    best_score = s;
                }
            }
        }
    }
    return selected;
}

// 焼きなましで (最良ルート, その得点) を返す
// プログラム開始時から scond 秒経つまで実行
inline pair<Route, double> annealing(Route current_route, double second,
                                     double start_temp, double end_temp) {
    Route best_route;
    double best_score;
    double current_score = get<0>(score(current_route));
    double start_time = get_elapsed_sec();
    double elapsed_time = 0;
    while ((elapsed_time = get_elapsed_sec()) < second) {
        DEB {
            ++benchmark_count;
        }
        elapsed_time -= start_time; // この関数での経過時間に変換
        double t = elapsed_time / second; // (0, 1)
        double temp =
            start_temp + (end_temp - start_temp) * (3 * t * t - 2 * t * t * t);
        // double temp = start_temp + (end_temp - start_temp) *
        //                                (1 - (1 - t) * (1 - t) * (1 - t));
        // double temp = start_temp + (end_temp - start_temp) * t;

        // current_route を一部変更（ undo 用情報を取得）
        vector<int> param;
        vector<vector<int>> params;
        if (MAX_CHANGE_STEP == 1) {
            param = inplace_change(current_route);
        } else {
            int step;
            if (RANDOM_CHANGE_STEP) {
                int max_step = MAX_CHANGE_STEP * (t - 1) * (t - 1) + 1;
                step = engine() % max_step + 1;
            } else if (FIXED_STEP) {
                step = MAX_CHANGE_STEP;
            }

            else {
                step = MAX_CHANGE_STEP * (t - 1) * (t - 1) + 1;
            }
            params.resize(step);
            rep(i, step) {
                params[i] = inplace_change(current_route);
            }
        }

        // 新状態での得点を計算
        double next_score = get<0>(score(current_route));

        // 悪化したときに遷移する確率
        double probability = exp((next_score - current_score) / temp);

        // 新状態に強制的に遷移するかどうかのフラグ
        // bool force_next = probability > drand();
        bool force_next = probability * engine.max() > engine();

        if (current_score < 0 || next_score > current_score ||
            (force_next && next_score > 0)) {
            // 次の状態に遷移する
            current_score = next_score;
            // 最高得点を更新したら諸々の更新
            if (next_score > best_score) {
                dump(next_score, elapsed_time, temp);
                best_score = next_score;
                best_route = current_route;
            }
        } else {
            // 遷移しないので操作を戻す
            if (MAX_CHANGE_STEP == 1) {
                undo(current_route, param);
            } else {
                for_each(rall(params),
                         [&](const auto &p) { undo(current_route, p); });
            }
        }
    }
    return mp(best_route, best_score);
}

// 配達漏れのペナルティを考慮したスコアを用いて探索
// (ルート, スコア) を返す
pair<Route, double> annealing_modified_score(Route current_route, double second,
                                             double start_temp,
                                             double end_temp) {
    Route best_route;
    double best_score = 0;
    double current_score = score_modified(current_route);
    double start_time = get_elapsed_sec();
    double elapsed_time = 0;
    while ((elapsed_time = get_elapsed_sec()) < second) {
        DEB {
            ++benchmark_count;
        }
        elapsed_time -= start_time; // この関数での経過時間に変換
        double t = elapsed_time / second; // (0, 1)
        double temp =
            start_temp + (end_temp - start_temp) * (3 * t * t - 2 * t * t * t);
        // double temp = start_temp + (end_temp - start_temp) * t;

        // current_route を一部変更（ undo 用情報を取得）
        vector<int> param;
        vector<vector<int>> params;
        if (MAX_CHANGE_STEP == 1) {
            param = inplace_change(current_route);
        } else {
            int step;
            if (RANDOM_CHANGE_STEP) {
                int max_step = MAX_CHANGE_STEP * (t - 1) * (t - 1) + 1;
                step = engine() % max_step + 1;
            } else {
                step = MAX_CHANGE_STEP * (t - 1) * (t - 1) + 1;
            }
            params.resize(step);
            rep(i, step) {
                params[i] = inplace_change(current_route);
            }
        }

        // 新状態での得点を計算
        double next_score = score_modified(current_route);

        // 悪化したときに遷移する確率
        double probability = exp((next_score - current_score) / temp);

        // 新状態に強制的に遷移するかどうかのフラグ
        bool force_next = probability * engine.max() > engine();

        if (current_score < 0 || next_score > current_score ||
            (force_next && next_score > 0)) {
            // 次の状態に遷移する
            current_score = next_score;
            // 最高得点を更新したら諸々の更新
            if (next_score > best_score) {
                dump(next_score, elapsed_time, temp);
                best_score = next_score;
                best_route = current_route;
            }
        } else {
            // 遷移しないので操作を戻す
            if (MAX_CHANGE_STEP == 1) {
                undo(current_route, param);
            } else {
                for_each(rall(params),
                         [&](const auto &p) { undo(current_route, p); });
            }
        }
    }
    return mp(best_route, best_score);
}

void output_route(Route &route) {
    auto result = score(route);
    double score = get<0>(result);
    int finish_i = get<1>(result);
    int finish_j = get<2>(result);
    dump(score, finish_i, finish_j);
    dump(get_elapsed_sec());
    output_route(route, finish_i, finish_j);
}

inline double car_time(const Route &route) {
    double total_time;
    rep(i, kappa) {
        total_time +=
            dist_car[route.park_order[i]][route.park_order[(i + 1) % kappa]];
    }
    return total_time;
}
inline Route tsp_car() {}

int main() {
    start = std::chrono::system_clock::now();
    cin >> kappa >> lambda;
    double start_temp;
    if (kappa >= 20) {
        start_temp = START_TEMP_1;
        unrestricted_park_num = UNRESTRICTED_PARK_NUM_1;
        park3all = false;
    } else {
        start_temp = START_TEMP_2;
        unrestricted_park_num = UNRESTRICTED_PARK_NUM_2;
        park3all = true;
    }

    parks = vector<tuple<string, double, double>>(kappa);
    dests = vector<tuple<string, double, int>>(lambda);
    dest_0.reserve(lambda);
    dest_120.reserve(lambda);
    dest_240.reserve(lambda);

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
        if (r == 0) {
            dest_0.push_back(i);
        } else if (r == 120) {
            dest_120.push_back(i);
        } else if (r == 240) {
            dest_240.push_back(i);
        }
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
        }
    }

    Route best_route;
    double best_score;
    if (TRIAL == 1) {
        // 1 回のみ試行
        Route initial_route;
        if (park3all) {
            // 初期ルートで 3 park 全探索
            initial_route = select_first_route_3_parks();
        } else {
            initial_route = select_first_route(FIRST_ROUTE_SEARCH);
        }
        if (USE_MODIFIED_SCORE) {
            best_route = annealing_modified_score(initial_route, TIME_LIMIT,
                                                  start_temp, END_TEMP)
                             .first;
        } else {
            best_route =
                annealing(initial_route, TIME_LIMIT, start_temp, END_TEMP)
                    .first;
        }
    } else {
        // 複数回試行
        Route initial_route;
        rep(i, TRIAL) {
            if (park3all) {
                // 初期ルートで 3 park 全探索
                initial_route = select_first_route_3_parks();
            } else {
                initial_route = select_first_route(FIRST_ROUTE_SEARCH);
            }
            Route cand_route;
            double cand_score;
            if (USE_MODIFIED_SCORE) {
                auto result = annealing_modified_score(
                    initial_route, TIME_LIMIT * (i + 1) / TRIAL, start_temp,
                    END_TEMP);
                cand_route = result.first;
                cand_score = result.second;
            } else {
                auto result =
                    annealing(initial_route, TIME_LIMIT * (i + 1) / TRIAL,
                              start_temp, END_TEMP);
                cand_route = result.first;
                cand_score = result.second;
            }
            if (cand_score > best_score) {
                best_route = cand_route;
                best_score = cand_score;
            }
        }
    }
    dump(benchmark_count);
    output_route(best_route);

    return 0;
}
