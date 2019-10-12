#include "header.hpp"

// str と str[i:] の最長共通接頭辞の長さを O(|str|) で計算
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

class RollingHash {
    int num = 10; // 基数と法の数
    int len;      // 文字列の長さ
    vector<int> base = {1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000,
                        1000 + rand() % 1000000000, 1000 + rand() % 1000000000};
    const vector<int> mod = {999999797,  999999883,  999999893,  999999929,
                             999999937,  1000000007, 1000000009, 1000000021,
                             1000000033, 1000000087};
    vector<vector<long long>> hash, power;

  public:
    RollingHash(const string &str, const vector<int> &base_ = vector<int>()) {
        if (base_.size() != 0) base = base_;
        len = str.size();
        rep(i, num) {
            hash.emplace_back(len + 1);
            power.emplace_back(len + 1);
            power[i][0] = 1;
            hash[i][0] = 0;
        }
        repr(i, 1, len + 1) {
            rep(j, num) {
                hash[j][i] = (hash[j][i - 1] * base[j] + str[i - 1]) % mod[j];
                power[j][i] = power[j][i - 1] * base[j] % mod[j];
            }
        }
    }

    // [l, r) のハッシュ値を得る
    inline vector<long long> get_hash(int l, int r) {
        vector<long long> res(num);
        rep(i, num) {
            res[i] = (hash[i][r] - hash[i][l] * power[i][r - l]) % mod[i];
            if (res[i] < 0) res[i] += mod[i];
        }
        return res;
    }

    // str[a:], str[b:] の最長共通接頭辞の長さを O(log|str|) で計算
    inline int lcp(int a, int b) {
        int ok = 0, ng = min(len - a, len - b);
        while (ng - ok > 1) {
            int mid = (ok + ng) / 2;
            if (get_hash(a, a + mid) == get_hash(b, b + mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        return ok;
    }

    auto get_base() {
        return base;
    }
};