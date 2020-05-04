#include "header.hpp"

// rank, select が可能なビットベクトル
class BitVector {
    vector<int> cumulative_sum;
    int pos = 1;
    vector<int> pos_select;

  public:
    vector<vector<int>> select; // select[b][n] = n 番目（0-indexed）の b の位置

    BitVector(int n)
        : cumulative_sum(n + 1), pos_select(2, 0), select(2, vector<int>(n)) {
        cumulative_sum[0] = 0;
    }

    // 数 b を追加。n 回呼ばれる
    void push(int b) {
        cumulative_sum[pos] = cumulative_sum[pos - 1] + b;
        select[b][pos_select[b]] = pos - 1;
        pos++;
        pos_select[b]++;
    }

    int operator[](int idx) const {
        return cumulative_sum[idx + 1] - cumulative_sum[idx];
    }

    // [0, n) に x がいくつあるか
    int rank(int n, int b) const {
        if (n < 0) return 0;
        n = min(n, size());
        if (b == 1) {
            return cumulative_sum[n];
        } else {
            return n - cumulative_sum[n];
        }
    }

    int size() const {
        return cumulative_sum.size() - 1;
    }
};
ostream &operator<<(ostream &os, const BitVector &bv) {
    for (int i = 0; i < bv.size(); i++) {
        os << bv[i];
    }
    os << endl;
    return os;
}

class WaveletMatrix {
    int len;                       // 数列の長さ
    int digit = 0;                 // 値の桁数
    vector<BitVector> bit_vectors; // ビットベクトル
    vector<int> num_zero;          // 各段での 0 の個数
    vector<long long> sorted;      // ビット逆転ソート済み列
    map<int, int> start;           // sorted における各値の開始位置
    vector<vector<long long>> cumulative_sum; // 各段での累積和

  public:
    WaveletMatrix(vector<long long> v) {
        len = v.size();
        long long max_value = *max_element(v.begin(), v.end());
        while (max_value > 0) {
            max_value >>= 1;
            digit++;
        }
        if (digit == 0) digit++;

        bit_vectors.reserve(digit);
        cumulative_sum.reserve(digit);
        num_zero = vector<int>(digit, 0);
        for (int d = 0; d < digit; d++) {
            bit_vectors.emplace_back(len);
            cumulative_sum.emplace_back(len + 1, 0);
            // 累積和を記録
            for (int i = 0; i < len; i++) {
                cumulative_sum[d][i + 1] = cumulative_sum[d][i] + v[i];
            }
            // ビットベクトルの構築
            for (int i = 0; i < len; i++) {
                bit_vectors[d].push(v[i] >> (digit - 1 - d) & 1);
            }
            // 安定ソート
            vector<long long> temp;
            temp.reserve(len);
            for (int i = 0; i < len; i++) {
                if ((v[i] >> (digit - 1 - d) & 1) == 0) {
                    temp.push_back(v[i]);
                    num_zero[d]++;
                }
            }
            for (int i = 0; i < len; i++) {
                if ((v[i] >> (digit - 1 - d) & 1) == 1) {
                    temp.push_back(v[i]);
                }
            }
            v = temp;
        }
        for (int i = len - 1; i >= 0; i--) {
            start[v[i]] = i;
        }
        sorted = v;
    }

    // n 番目（0-indexed）の値を得る
    long long operator[](int n) const {
        int ans = 0;
        for (int d = 0; d < digit; d++) {
            ans <<= 1;
            ans += bit_vectors[d][n];
            if (bit_vectors[d][n] == 1) {
                n = num_zero[d] + bit_vectors[d].rank(n, 1);
            } else {
                n = bit_vectors[d].rank(n, 0);
            }
        }
        return ans;
    }

    // [0, n) に x がいくつ含まれるか
    int rank(int n, long long x) {
        if (start.find(x) == start.end()) return 0;
        for (int d = 0; d < digit; d++) {
            if ((x >> (digit - 1 - d) & 1) == 1) {
                n = num_zero[d] + bit_vectors[d].rank(n, 1);
            } else {
                n = bit_vectors[d].rank(n, 0);
            }
        }
        return n - start[x];
    }

    // n 番目（0-indexed）の x の位置（無ければ-1）
    int select(int n, long long x) {
        if (start.find(x) == start.end() || sorted[start[x] + n] != x)
            return -1;
        n = start[x] + n;
        for (int d = digit - 1; d >= 0; d--) {
            if ((x >> (digit - 1 - d) & 1) == 1) {
                n = bit_vectors[d].select[1][n - num_zero[d]];
            } else {
                n = bit_vectors[d].select[0][n];
            }
        }
        return n;
    }

    // [l, r) で n 番目（0-indexed）に小さい値 O(log(max))
    long long quantile(int l, int r, int n) {
        for (int d = 0; d < digit; d++) {
            if (bit_vectors[d].rank(r, 0) - bit_vectors[d].rank(l, 0) > n) {
                // 0
                l = bit_vectors[d].rank(l, 0);
                r = bit_vectors[d].rank(r, 0);
            } else {
                // 1
                n -= bit_vectors[d].rank(r, 0) - bit_vectors[d].rank(l, 0);
                l = num_zero[d] + bit_vectors[d].rank(l, 1);
                r = num_zero[d] + bit_vectors[d].rank(r, 1);
            }
        }
        return sorted[l];
    }

    // [l, r) で出現回数が多い順に k 個の値と頻度を返す O((r-l)log(max))
    vector<pair<long long, int>> topk(int l, int r, int k) {
        vector<pair<long long, int>> res;
        res.reserve(k);
        priority_queue<tuple<int, int, int, int>> pq;
        pq.push(make_tuple(r - l, 0, l, r));
        while (!pq.empty() && res.size() < k) {
            int dummy, d;
            tie(dummy, d, l, r) = pq.top();
            pq.pop();
            if (d == digit) {
                // 終わり
                res.push_back(make_pair(sorted[l], r - l));
                continue;
            }
            int l0 = bit_vectors[d].rank(l, 0);
            int r0 = bit_vectors[d].rank(r, 0);
            int l1 = num_zero[d] + bit_vectors[d].rank(l, 1);
            int r1 = num_zero[d] + bit_vectors[d].rank(r, 1);
            pq.push(make_tuple(r0 - l0, d + 1, l0, r0));
            pq.push(make_tuple(r1 - l1, d + 1, l1, r1));
        }
        return res;
    }

    // [l, r) に含まれる x 未満の値の個数 O(log(max))
    int less_freq(int l, int r, long long x) {
        if (x <= 0) return 0;
        if (l >= r) return 0;
        if (x >= 1LL << (digit)) return r - l;
        int ans = 0;
        for (int d = 0; d < digit; d++) {
            if ((x >> (digit - 1 - d) & 1) == 1) {
                ans += bit_vectors[d].rank(r, 0) - bit_vectors[d].rank(l, 0);
                l = num_zero[d] + bit_vectors[d].rank(l, 1);
                r = num_zero[d] + bit_vectors[d].rank(r, 1);
            } else {
                l = bit_vectors[d].rank(l, 0);
                r = bit_vectors[d].rank(r, 0);
            }
        }
        return ans;
    }

    // [l, r) に含まれる [a, b) の範囲の値の個数 O(log(max))
    int range_freq(int l, int r, long long a, long long b) {
        return less_freq(l, r, b) - less_freq(l, r, a);
    }

    // [l, r) に含まれる [a, b) の範囲の値の和 O(log(max))
    long long range_sum(int l, int r, long long a, long long b) {
        if (l >= r) return 0;
        long long ans = 0;
        stack<tuple<int, int, int, int>> st;
        st.push(make_tuple(0, 0, l, r)); // d, 範囲内理論最小値, l, r
        while (!st.empty()) {
            int d;
            long long min_value;
            tie(d, min_value, l, r) = st.top();
            st.pop();
            if (d == digit) {
                // 終わり
                if (a <= sorted[l] && sorted[l] < b) {
                    ans += (long long)sorted[l] * (r - l);
                }
                continue;
            }
            // この範囲内に [a, b) の値が無いなら飛ばす
            if (min_value >= b || min_value + (1LL << (digit - d)) <= a)
                continue;
            // この範囲内が全て [a, b) に入るなら累積和
            if (a <= min_value && min_value + (1LL << (digit - d)) <= b) {
                ans += cumulative_sum[d][r] - cumulative_sum[d][l];
                continue;
            }

            int l0 = bit_vectors[d].rank(l, 0);
            int r0 = bit_vectors[d].rank(r, 0);
            int l1 = num_zero[d] + bit_vectors[d].rank(l, 1);
            int r1 = num_zero[d] + bit_vectors[d].rank(r, 1);
            if (l0 < r0) st.push(make_tuple(d + 1, min_value, l0, r0));
            if (l1 < r1)
                st.push(make_tuple(d + 1, min_value + (1LL << (digit - d - 1)),
                                   l1, r1));
        }
        return ans;
    }
};

int main() {
    vll v = {5, 4, 5, 5, 2, 1, 5, 6, 1, 3, 5, 0};
    WaveletMatrix wm(v);

    rep(i, v.size()) {
        assert(wm[i] == v[i]);
    }
    assert(wm.rank(0, 5) == 0);
    assert(wm.rank(1, 5) == 1);
    assert(wm.rank(2, 5) == 1);
    assert(wm.rank(3, 5) == 2);
    assert(wm.rank(4, 5) == 3);
    assert(wm.rank(5, 5) == 3);
    assert(wm.rank(6, 5) == 3);
    assert(wm.rank(7, 5) == 4);
    assert(wm.rank(8, 5) == 4);
    assert(wm.rank(9, 5) == 4);
    assert(wm.rank(10, 5) == 4);
    assert(wm.rank(11, 5) == 5);
    assert(wm.rank(12, 5) == 5);

    assert(wm.select(0, 5) == 0);
    assert(wm.select(1, 5) == 2);
    assert(wm.select(2, 5) == 3);
    assert(wm.select(3, 5) == 6);
    assert(wm.select(4, 5) == 10);

    assert(wm.quantile(1, 11, 0) == 1);
    assert(wm.quantile(1, 11, 1) == 1);
    assert(wm.quantile(1, 11, 2) == 2);
    assert(wm.quantile(1, 11, 3) == 3);
    assert(wm.quantile(1, 11, 4) == 4);
    assert(wm.quantile(1, 11, 5) == 5);
    assert(wm.quantile(1, 11, 6) == 5);
    assert(wm.quantile(1, 11, 7) == 5);
    assert(wm.quantile(1, 11, 8) == 5);
    assert(wm.quantile(1, 11, 9) == 6);

    using P_LL_I = pair<long long, int>;
    assert(wm.topk(2, 9, 2) == vector<P_LL_I>({mp(5LL, 3), mp(1LL, 2)}));

    assert(wm.less_freq(1, 11, 0) == 0);
    assert(wm.less_freq(1, 11, 1) == 0);
    assert(wm.less_freq(1, 11, 2) == 2);
    assert(wm.less_freq(1, 11, 3) == 3);
    assert(wm.less_freq(1, 11, 4) == 4);
    assert(wm.less_freq(1, 11, 5) == 5);
    assert(wm.less_freq(1, 11, 6) == 9);
    assert(wm.less_freq(1, 11, 7) == 10);

    assert(wm.range_sum(1, 11, 1, 5) == 11);
    assert(wm.range_sum(1, 11, 2, 5) == 9);
    assert(wm.range_sum(1, 11, 2, 6) == 29);
    assert(wm.range_sum(1, 11, 1, 7) == 37);

    return 0;
}
