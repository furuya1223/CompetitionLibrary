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
<script type="text/javascript" src="../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../assets/css/copy-button.css" />


# :warning: blog.cpp

<a href="../index.html">Back to top page</a>

* category: <a href="../index.html#5058f1af8388633f609cadb75a75dc9d">.</a>
* <a href="{{ site.github.repository_url }}/blob/master/blog.cpp">View this file on GitHub</a>
    - Last commit date: 2019-08-03 22:47:20+09:00




## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

#define repr(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rep(i, n) repr(i, 0, n)

// Binary Indexed Tree （区間和）
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
        dat = vector<int>(n + 1, 0);
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
        i++;
        while (i <= n) {
            dat[i] += x;
            i += i & -i;
        }
    }
};

struct segment {
    int x1, y1, x2, y2;
};
vector<segment> horizontal, vertical;

// horizontal[h_index] を下辺とする長方形の数を走査線で計算
long long scan(int h_index) {
    segment h = horizontal[h_index];
    BIT bit(10001);
    vector<vector<int>> del(10001);
    // h と交わる縦線を記録
    for (auto v : vertical) {
        if (h.x1 <= v.x1 && v.x1 <= h.x2 && v.y1 <= h.y1 && h.y1 <= v.y2) {
            // v は h と交わる
            bit.add(v.x1, 1);
            del[v.y2].push_back(v.x1);
        }
    }
    long long ans = 0;
    // 走査線を上に動かす
    repr(i, h_index + 1, horizontal.size()) {
        if (horizontal[i].y1 == h.y1) continue;
        // ここまでに消すべき x 座標を消す
        repr(y, horizontal[i - 1].y1, horizontal[i].y1) {
            for (auto x : del[y]) {
                bit.add(x, -1);
            }
        }
        // 交わる縦線を数える
        int n = bit.sum(horizontal[i].x2 + 1) - bit.sum(horizontal[i].x1);
        ans += n * (n - 1) / 2;
    }
    return ans;
}

int main(void) {
    int N;
    cin >> N;
    rep(i, N) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        x1 += 5000;
        y1 += 5000;
        x2 += 5000;
        y2 += 5000;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2) {
            vertical.push_back(segment{x1, y1, x2, y2});
        } else {
            horizontal.push_back(segment{x1, y1, x2, y2});
        }
    }
    sort(horizontal.begin(), horizontal.end(),
         [](segment a, segment b) { return a.y1 < b.y1; });

    long long ans = 0;
    rep(i, horizontal.size()) {
        ans += scan(i);
    }
    cout << ans << endl;

    return 0;
}
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "blog.cpp"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

#define repr(i, a, b) for (int i = (int)(a); i < (int)(b); i++)
#define rep(i, n) repr(i, 0, n)

// Binary Indexed Tree （区間和）
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
        dat = vector<int>(n + 1, 0);
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
        i++;
        while (i <= n) {
            dat[i] += x;
            i += i & -i;
        }
    }
};

struct segment {
    int x1, y1, x2, y2;
};
vector<segment> horizontal, vertical;

// horizontal[h_index] を下辺とする長方形の数を走査線で計算
long long scan(int h_index) {
    segment h = horizontal[h_index];
    BIT bit(10001);
    vector<vector<int>> del(10001);
    // h と交わる縦線を記録
    for (auto v : vertical) {
        if (h.x1 <= v.x1 && v.x1 <= h.x2 && v.y1 <= h.y1 && h.y1 <= v.y2) {
            // v は h と交わる
            bit.add(v.x1, 1);
            del[v.y2].push_back(v.x1);
        }
    }
    long long ans = 0;
    // 走査線を上に動かす
    repr(i, h_index + 1, horizontal.size()) {
        if (horizontal[i].y1 == h.y1) continue;
        // ここまでに消すべき x 座標を消す
        repr(y, horizontal[i - 1].y1, horizontal[i].y1) {
            for (auto x : del[y]) {
                bit.add(x, -1);
            }
        }
        // 交わる縦線を数える
        int n = bit.sum(horizontal[i].x2 + 1) - bit.sum(horizontal[i].x1);
        ans += n * (n - 1) / 2;
    }
    return ans;
}

int main(void) {
    int N;
    cin >> N;
    rep(i, N) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        x1 += 5000;
        y1 += 5000;
        x2 += 5000;
        y2 += 5000;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2) {
            vertical.push_back(segment{x1, y1, x2, y2});
        } else {
            horizontal.push_back(segment{x1, y1, x2, y2});
        }
    }
    sort(horizontal.begin(), horizontal.end(),
         [](segment a, segment b) { return a.y1 < b.y1; });

    long long ans = 0;
    rep(i, horizontal.size()) {
        ans += scan(i);
    }
    cout << ans << endl;

    return 0;
}

```
{% endraw %}

<a href="../index.html">Back to top page</a>

