#include "header.hpp"

template <class T>
class SegTree {
    int n;                       // 葉の数
    vector<T> data;              // データを格納するvector
    T def;                       // 初期値かつ単位元
    function<T(T, T)> operation; // 区間クエリで使う処理
    function<T(T, T)> update;    // 点更新で使う処理

    // 区間[a,b)の総和。ノードk=[l,r)に着目している。
    T _query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return def; // 交差しない
        if (a <= l && r <= b)
            return data[k]; // a,l,r,bの順で完全に含まれる
        else {
            T c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2); // 左の子
            T c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r); // 右の子
            return operation(c1, c2);
        }
    }

  public:
    // _n:必要サイズ, _def:初期値かつ単位元, _operation:クエリ関数,
    // _update:更新関数
    SegTree(size_t _n, T _def, function<T(T, T)> _operation,
            function<T(T, T)> _update)
        : def(_def), operation(_operation), update(_update) {
        n = 1;
        while (n < _n) {
            n *= 2;
        }
        data = vector<T>(2 * n - 1, def);
    }

    // 場所i(0-indexed)の値をxで更新
    void change(int i, T x) {
        i += n - 1;
        data[i] = update(data[i], x);
        while (i > 0) {
            i = (i - 1) / 2;
            data[i] = operation(data[i * 2 + 1], data[i * 2 + 2]);
        }
    }

    // [a, b)の区間クエリを実行
    T query(int a, int b) {
        return _query(a, b, 0, 0, n);
    }

    // 添字でアクセス
    T operator[](int i) {
        return data[i + n - 1];
    }
};

// 区間加算区間和型
template <class T, class U>
class LazySegTree {
    /*
    区間更新・区間和
    auto operation = [](ll a, ll b) { return a + b; };
    auto adapt_lazy = [](ll a, ll b) { return b; };
    auto merge_lazy = [](ll a, ll b) { return b; };
    auto multiply_lazy = [](ll a, int n) { return a * n; };

    区間加算・区間和
    auto operation = [](ll a, ll b) { return a + b; };
    auto adapt_lazy = [](ll a, ll b) { return a + b; };
    auto merge_lazy = [](ll a, ll b) { return a + b; };
    auto multiply_lazy = [](ll a, int n) { return a * n; };

    区間更新・区間最小
    auto operation = [](ll a, ll b) { return min(a, b); };
    auto adapt_lazy = [](ll a, ll b) { return b; };
    auto merge_lazy = [](ll a, ll b) { return b; };
    auto multiply_lazy = [](ll a, int n) { return a; };

    区間加算・区間最小
    auto operation = [](ll a, ll b) { return min(a, b); };
    auto adapt_lazy = [](ll a, ll b) { return a + b; };
    auto merge_lazy = [](ll a, ll b) { return a + b; };
    auto multiply_lazy = [](ll a, int n) { return a; };
    */
    int n;
    vector<T> dat;
    vector<U> lazy;
    vector<int> width;

    function<T(T, T)> operation;
    function<T(T, U)> adapt_lazy;
    function<U(U, U)> merge_lazy;
    function<U(U, int)> multiply_lazy;

    T init, def, lazy_def;

    // ツリー上のノード k に lazy 値 v をセット
    void set_lazy(int k, U v) {
        if (lazy[k] == lazy_def) {
            lazy[k] = v;
            dat[k] = adapt_lazy(dat[k], multiply_lazy(v, width[k]));
        } else {
            lazy[k] = merge_lazy(lazy[k], v);
            dat[k] = adapt_lazy(dat[k], multiply_lazy(v, width[k]));
        }
    }
    // lazy を 1 段下ろす
    void push(int k) {
        if (lazy[k] == lazy_def) return;
        set_lazy(k * 2 + 1, lazy[k]);
        set_lazy(k * 2 + 2, lazy[k]);
        lazy[k] = lazy_def;
    }

    // 区間[a,b)の区間クエリ。ノードk=[l,r)に着目している。
    long long _query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return def; // 交差しない
        if (a <= l && r <= b) return dat[k]; // a,l,r,bの順で完全に含まれる
        push(k);
        long long c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2); // 左の子
        long long c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r); // 右の子
        return operation(c1, c2);
    }

  public:
    // _n:必要サイズ, _def:初期値, _operation:クエリ関数,
    // _adapt_lazy:区間作用素適用関数, _merge_lazy:区間作用素マージ関数,
    // _multiply_lazy:作用素を要素数で変形
    LazySegTree(size_t _n, T _init, T _def, T _lazy_def,
                function<T(T, T)> _operation, function<T(T, U)> _adapt_lazy,
                function<U(U, U)> _merge_lazy,
                function<U(U, int)> _multiply_lazy = [](U u,
                                                        int n) { return u; })
        : init(_init), def(_def), lazy_def(_lazy_def), operation(_operation),
          adapt_lazy(_adapt_lazy), merge_lazy(_merge_lazy),
          multiply_lazy(_multiply_lazy) {
        n = 1;
        while (n < _n) n *= 2;
        dat = vector<long long>(2 * n - 1, init);
        lazy = vector<long long>(2 * n - 1, lazy_def);
        width = vector<int>(2 * n - 1);
        width[0] = n;
        repr(i, 1, 2 * n - 1) width[i] = width[(i - 1) / 2] / 2;
    }

    // 区間[a,b)にvを作用させる
    void range_operation(int a, int b, long long v, int k = 0, int l = 0,
                         int r = -1) {
        if (r == -1) r = n;
        if (r <= a || b <= l) return; // 交差しない
        if (a <= l && r <= b) {       // a,l,r,bの順で完全に含まれる
            set_lazy(k, v);
            return;
        }
        push(k);
        range_operation(a, b, v, 2 * k + 1, l, (l + r) / 2); // 左の子
        range_operation(a, b, v, 2 * k + 2, (l + r) / 2, r); // 右の子
        dat[k] = operation(dat[k * 2 + 1], dat[k * 2 + 2]);
    }

    // [a, b)の区間クエリを実行
    long long query(int a, int b) {
        return _query(a, b, 0, 0, n);
    }

    //  添字でアクセス
    long long operator[](int i) {
        return query(i, i + 1);
    }
};

// Binary Indexed Tree（区間和）
struct BIT {
    int n, height;
    vector<int> dat;

    BIT(int _n) {
        n = 1;
        height = 1;
        while (n < _n) {
            n *= 2;
            height++;
        }
        dat = vector<int>(n + 1);
    }

    // [0,i)までの和
    int sum(int i) {
        int s = 0;
        while (i > 0) {
            s += dat[i];
            i -= i & -i; // i & -i は i の最後の1ビット
        }
        return s;
    }

    void add(int i, int x) {
        i++; // 1-indexedに変更
        while (i <= n) {
            dat[i] += x;
            i += i & -i;
        }
    }
};