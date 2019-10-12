#include "header.hpp"

template <class T>
class ConvesHullTrick {
    static const T IS_QUERY = std::numeric_limits<T>::lowest();

    // 直線 ax+b
    struct Line {
        T a, b;
        // Lines において次の直線のポインタを得る
        mutable std::function<const Line *()> nextLine;

        bool operator<(Line rhs) const {
            if (b == IS_QUERY) {
                // 自分がクエリ
                const Line *next_line = rhs.nextLine();
                if (next_line == nullptr) return false;
                T x = a;
                return (rhs.a - next_line->a) * x + (rhs.b - next_line->b) > 0;
            } else if (rhs.b == IS_QUERY) {
                // rhs がクエリ
                const Line *next_line = nextLine();
                if (next_line == nullptr) return false;
                T x = rhs.a;
                return (a - next_line->a) * x + (b - next_line->b) > 0;
            }
            return (a < rhs.a || (a == rhs.a && b < rhs.b));
        }
    };
    set<Line> lines; // 直線集合

    // l が不要か判定
    bool isVain(Line l) {
        auto it = lines.find(l);
        auto next_it = next(it);
        if (it == lines.begin()) {
            if (next_it == lines.end()) return false;
            return it->a == next_it->a && it->b <= next_it->b;
        } else {
            auto prev_it = prev(it);
            if (next_it == lines.end())
                return it->a == prev_it->a && it->b <= prev_it->b;
            return (prev_it->b - it->b) * (it->a - next_it->a) >=
                   (it->b - next_it->b) * (prev_it->a - it->a);
        }
    }

  public:
    // 直線 ax+b を追加
    void add(T a, T b) {
        auto it = lines.insert(Line{a, b}).first;
        // 次の直線を得る関数をセットしておく
        it->nextLine = [=] {
            return (std::next(it) == lines.end() ? nullptr : &*std::next(it));
        };

        if (isVain(*it)) {
            // 今追加した直線が不要
            lines.erase(it);
            return;
        }
        // 追加した直線の前後が不要なら削除していく
        while (next(it) != lines.end() && isVain(*next(it))) {
            lines.erase(next(it));
        }
        while (it != lines.begin() && isVain(*prev(it))) {
            lines.erase(prev(it));
        }
    }

    // x での最小値を出力
    T get(T x) {
        auto l = lines.lower_bound(Line{x, IS_QUERY});
        return l->a * x + l->b;
    }
};

// https://atcoder.jp/contests/dp/tasks/dp_z
int main() {
    int N;
    ll C;
    cin >> N >> C;
    ConvesHullTrick<ll> CHT;
    vll dp(N);
    dp[0] = 0;
    rep(i, N) {
        ll h;
        cin >> h;
        if (i != 0) {
            dp[i] = CHT.get(h) + C + h * h;
        }
        CHT.add(-2 * h, h * h + dp[i]);
    }
    cout << dp[N - 1] << endl;

    return 0;
}
