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