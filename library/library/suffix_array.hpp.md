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


# :warning: library/suffix_array.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/suffix_array.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Depends on

* :heavy_check_mark: <a href="header.hpp.html">library/header.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "header.hpp"

class SuffixArray {
    static const int L_type = 0, S_type = 1, LMS_type = 2;

    static void induce_sort(const vector<int> &v, vector<int> &sa,
                            const vector<int> &type, vector<int> start,
                            vector<int> end) {
        int len = sa.size();
        // L-type の要素を各バケットに前から詰める
        for (int i = 0; i < len; i++) {
            if (sa[i] == -1) continue;
            if (sa[i] - 1 >= 0 && type[sa[i] - 1] == L_type) {
                sa[start[v[sa[i] - 1]]++] = sa[i] - 1;
            }
            if (i != 0 && type[sa[i]] == LMS_type) {
                sa[i] = -1;
            }
        }
        // S-type の要素を各バケットに後ろから詰める
        for (int i = len - 1; i >= 0; i--) {
            if (sa[i] == -1) continue;
            if (sa[i] - 1 >= 0 && type[sa[i] - 1] != L_type) {
                sa[--end[v[sa[i] - 1]]] = sa[i] - 1;
            }
        }
    }

  public:
    vector<int> str; // 入力配列
    vector<int> suffix_array;
    vector<int> order; // SA における添字: order[suffix_array[i]] = i
    vector<int> lpc;   // SA[i], SA[i+1] の LPC 長

    SuffixArray(vector<int> &v, int char_num) {
        str = v;
        suffix_array = SA_IS(str, char_num);
    }

    SuffixArray(const string &s) {
        str.resize(s.size());
        for (int i = 0; i < s.size(); i++) {
            str[i] = s[i] - 'a' + 1;
        }
        suffix_array = SA_IS(str, 26);
    }

    static vector<int> SA_IS(vector<int> &v, int char_num) {
        if (v.size() == 1) {
            // 再帰の終わり
            return vector<int>(1, 0);
        }
        v.push_back(0);
        char_num++;

        int len = v.size();
        vector<int> tmp_sa(len, -1);
        vector<int> type(len);
        vector<int> start(char_num), end(char_num); // バケットiの注目範囲
        vector<int> LMS;                            // LMS の開始位置
        vector<int> LMS_order(len, -1); // LMS のソート順序（非 LMS では -1）

        // type の判定
        type.back() = S_type;
        end[0] = 1;
        for (int i = len - 2; i >= 0; i--) {
            if (v[i] > v[i + 1]) {
                type[i] = L_type;
                if (type[i + 1] == S_type) {
                    LMS.push_back(i + 1);
                    type[i + 1] = LMS_type;
                    LMS_order[i + 1] = 0;
                }
            } else if (v[i] < v[i + 1]) {
                type[i] = S_type;
            } else {
                type[i] = type[i + 1];
            }
            end[v[i]]++;
        }
        // start, end の計算
        start[0] = 0;
        for (int i = 1; i < char_num; i++) {
            end[i] += end[i - 1];
            start[i] = end[i - 1];
        }
        // Induce Sort の準備
        for (int i = 0; i < len; i++) {
            if (LMS_order[i] != -1) {
                tmp_sa[--end[v[i]]] = i;
            }
        }
        // end の復元
        for (int i = 0; i < char_num - 1; i++) {
            end[i] = start[i + 1];
        }
        end[char_num - 1] = len;

        // Induce Sort で LMS-substring をソートする
        induce_sort(v, tmp_sa, type, start, end);

        // LMS のみからなる数列を作成する

        // LMS のソート順序を調べる
        int counter = 0;
        int prev_lms = -1; // 1 つ前の LMS の先頭
        for (int i = 0; i < len; i++) {
            if (LMS_order[tmp_sa[i]] != -1) {
                LMS_order[tmp_sa[i]] = ++counter;
                if (prev_lms == -1) {
                    prev_lms = tmp_sa[i];
                    continue;
                }
                // 1 つ前の LMS と同じか判定
                bool different = false;
                for (int j = 0; j < len; j++) {
                    // j 文字目（prev_lms+j と tmp_sa[i]+j）をチェック
                    if (prev_lms + j >= len || tmp_sa[i] + j >= len ||
                        (j != 0 && (LMS_order[prev_lms + j] != -1 ||
                                    LMS_order[tmp_sa[i] + j] != -1))) {
                        if (!(LMS_order[prev_lms + j] != -1 &&
                              LMS_order[tmp_sa[i] + j] != -1)) {
                            different = true;
                        }
                        break;
                    }
                    if (v[prev_lms + j] != v[tmp_sa[i] + j]) {
                        different = true;
                        break;
                    }
                }
                if (!different) {
                    LMS_order[tmp_sa[i]] = --counter;
                }
                prev_lms = tmp_sa[i];
            }
        }
        vector<int> new_v; // v での出現順に LMS-substring の順位が並ぶ
        vector<int> position(len, 0); // 各 LMS の開始位置
        counter = 0;
        for (int i = 0; i < len; i++) {
            if (LMS_order[i] != -1) {
                new_v.push_back(LMS_order[i]);
                position[counter++] = i;
            }
        }
        // SA-IS を再帰適用して LMS をソートする
        vector<int> lms_sa = SA_IS(new_v, new_v.size());

        // Induce Sort の準備
        fill(tmp_sa.begin(), tmp_sa.end(), -1);
        for (int i = lms_sa.size() - 1; i >= 0; i--) {
            if (LMS_order[position[lms_sa[i]]] != -1) {
                tmp_sa[--end[v[position[lms_sa[i]]]]] = position[lms_sa[i]];
            }
        }
        // end の復元
        for (int i = 0; i < char_num - 1; i++) {
            end[i] = start[i + 1];
        }
        end[char_num - 1] = len;

        // Induce Sort で saffix array を完成させる
        induce_sort(v, tmp_sa, type, start, end);

        tmp_sa.erase(tmp_sa.begin()); // 先頭の空文字列を削除
        v.pop_back();                 // 末尾に足した 0 を削除

        return tmp_sa;
    }

    void calc_lpc() {
        if (lpc.size() != 0) return;

        int len = str.size();
        lpc.resize(len);
        order.resize(len);
        for (int i = 0; i < len; i++) {
            order[suffix_array[i]] = i;
        }
        int h = 0;
        for (int i = 0; i < len; i++) {
            // str[i..] と SA でその前にある接尾辞の LPC を求める
            if (order[i] == 0) {
                h = 0;
                continue;
            }
            int j = suffix_array[order[i] - 1];
            if (h > 0) h--;
            while (i + h < len && j + h < len) {
                if (str[i + h] != str[j + h]) break;
                h++;
            }
            lpc[order[i] - 1] = h;
        }
    }
};

// https://s8pc-2.contest.atcoder.jp/tasks/s8pc_2_e
int main() {
    string S;
    cin >> S;
    int N = S.size();

    SuffixArray sa(S);
    sa.calc_lpc();
    ll ans = (ll)N * (N + 1) * (N + 1) / 2 - (ll)N * (N + 1) * (2 * N + 1) / 6;
    rep(i, S.size() - 1) {
        ans -= (ll)sa.lpc[i] * (sa.lpc[i] + 1) / 2;
    }
    cout << ans << endl;

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


#line 2 "library/suffix_array.hpp"

class SuffixArray {
    static const int L_type = 0, S_type = 1, LMS_type = 2;

    static void induce_sort(const vector<int> &v, vector<int> &sa,
                            const vector<int> &type, vector<int> start,
                            vector<int> end) {
        int len = sa.size();
        // L-type の要素を各バケットに前から詰める
        for (int i = 0; i < len; i++) {
            if (sa[i] == -1) continue;
            if (sa[i] - 1 >= 0 && type[sa[i] - 1] == L_type) {
                sa[start[v[sa[i] - 1]]++] = sa[i] - 1;
            }
            if (i != 0 && type[sa[i]] == LMS_type) {
                sa[i] = -1;
            }
        }
        // S-type の要素を各バケットに後ろから詰める
        for (int i = len - 1; i >= 0; i--) {
            if (sa[i] == -1) continue;
            if (sa[i] - 1 >= 0 && type[sa[i] - 1] != L_type) {
                sa[--end[v[sa[i] - 1]]] = sa[i] - 1;
            }
        }
    }

  public:
    vector<int> str; // 入力配列
    vector<int> suffix_array;
    vector<int> order; // SA における添字: order[suffix_array[i]] = i
    vector<int> lpc;   // SA[i], SA[i+1] の LPC 長

    SuffixArray(vector<int> &v, int char_num) {
        str = v;
        suffix_array = SA_IS(str, char_num);
    }

    SuffixArray(const string &s) {
        str.resize(s.size());
        for (int i = 0; i < s.size(); i++) {
            str[i] = s[i] - 'a' + 1;
        }
        suffix_array = SA_IS(str, 26);
    }

    static vector<int> SA_IS(vector<int> &v, int char_num) {
        if (v.size() == 1) {
            // 再帰の終わり
            return vector<int>(1, 0);
        }
        v.push_back(0);
        char_num++;

        int len = v.size();
        vector<int> tmp_sa(len, -1);
        vector<int> type(len);
        vector<int> start(char_num), end(char_num); // バケットiの注目範囲
        vector<int> LMS;                            // LMS の開始位置
        vector<int> LMS_order(len, -1); // LMS のソート順序（非 LMS では -1）

        // type の判定
        type.back() = S_type;
        end[0] = 1;
        for (int i = len - 2; i >= 0; i--) {
            if (v[i] > v[i + 1]) {
                type[i] = L_type;
                if (type[i + 1] == S_type) {
                    LMS.push_back(i + 1);
                    type[i + 1] = LMS_type;
                    LMS_order[i + 1] = 0;
                }
            } else if (v[i] < v[i + 1]) {
                type[i] = S_type;
            } else {
                type[i] = type[i + 1];
            }
            end[v[i]]++;
        }
        // start, end の計算
        start[0] = 0;
        for (int i = 1; i < char_num; i++) {
            end[i] += end[i - 1];
            start[i] = end[i - 1];
        }
        // Induce Sort の準備
        for (int i = 0; i < len; i++) {
            if (LMS_order[i] != -1) {
                tmp_sa[--end[v[i]]] = i;
            }
        }
        // end の復元
        for (int i = 0; i < char_num - 1; i++) {
            end[i] = start[i + 1];
        }
        end[char_num - 1] = len;

        // Induce Sort で LMS-substring をソートする
        induce_sort(v, tmp_sa, type, start, end);

        // LMS のみからなる数列を作成する

        // LMS のソート順序を調べる
        int counter = 0;
        int prev_lms = -1; // 1 つ前の LMS の先頭
        for (int i = 0; i < len; i++) {
            if (LMS_order[tmp_sa[i]] != -1) {
                LMS_order[tmp_sa[i]] = ++counter;
                if (prev_lms == -1) {
                    prev_lms = tmp_sa[i];
                    continue;
                }
                // 1 つ前の LMS と同じか判定
                bool different = false;
                for (int j = 0; j < len; j++) {
                    // j 文字目（prev_lms+j と tmp_sa[i]+j）をチェック
                    if (prev_lms + j >= len || tmp_sa[i] + j >= len ||
                        (j != 0 && (LMS_order[prev_lms + j] != -1 ||
                                    LMS_order[tmp_sa[i] + j] != -1))) {
                        if (!(LMS_order[prev_lms + j] != -1 &&
                              LMS_order[tmp_sa[i] + j] != -1)) {
                            different = true;
                        }
                        break;
                    }
                    if (v[prev_lms + j] != v[tmp_sa[i] + j]) {
                        different = true;
                        break;
                    }
                }
                if (!different) {
                    LMS_order[tmp_sa[i]] = --counter;
                }
                prev_lms = tmp_sa[i];
            }
        }
        vector<int> new_v; // v での出現順に LMS-substring の順位が並ぶ
        vector<int> position(len, 0); // 各 LMS の開始位置
        counter = 0;
        for (int i = 0; i < len; i++) {
            if (LMS_order[i] != -1) {
                new_v.push_back(LMS_order[i]);
                position[counter++] = i;
            }
        }
        // SA-IS を再帰適用して LMS をソートする
        vector<int> lms_sa = SA_IS(new_v, new_v.size());

        // Induce Sort の準備
        fill(tmp_sa.begin(), tmp_sa.end(), -1);
        for (int i = lms_sa.size() - 1; i >= 0; i--) {
            if (LMS_order[position[lms_sa[i]]] != -1) {
                tmp_sa[--end[v[position[lms_sa[i]]]]] = position[lms_sa[i]];
            }
        }
        // end の復元
        for (int i = 0; i < char_num - 1; i++) {
            end[i] = start[i + 1];
        }
        end[char_num - 1] = len;

        // Induce Sort で saffix array を完成させる
        induce_sort(v, tmp_sa, type, start, end);

        tmp_sa.erase(tmp_sa.begin()); // 先頭の空文字列を削除
        v.pop_back();                 // 末尾に足した 0 を削除

        return tmp_sa;
    }

    void calc_lpc() {
        if (lpc.size() != 0) return;

        int len = str.size();
        lpc.resize(len);
        order.resize(len);
        for (int i = 0; i < len; i++) {
            order[suffix_array[i]] = i;
        }
        int h = 0;
        for (int i = 0; i < len; i++) {
            // str[i..] と SA でその前にある接尾辞の LPC を求める
            if (order[i] == 0) {
                h = 0;
                continue;
            }
            int j = suffix_array[order[i] - 1];
            if (h > 0) h--;
            while (i + h < len && j + h < len) {
                if (str[i + h] != str[j + h]) break;
                h++;
            }
            lpc[order[i] - 1] = h;
        }
    }
};

// https://s8pc-2.contest.atcoder.jp/tasks/s8pc_2_e
int main() {
    string S;
    cin >> S;
    int N = S.size();

    SuffixArray sa(S);
    sa.calc_lpc();
    ll ans = (ll)N * (N + 1) * (N + 1) / 2 - (ll)N * (N + 1) * (2 * N + 1) / 6;
    rep(i, S.size() - 1) {
        ans -= (ll)sa.lpc[i] * (sa.lpc[i] + 1) / 2;
    }
    cout << ans << endl;

    return 0;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

