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


# :warning: library/rational.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/rational.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Code

<a id="unbundled"></a>
{% raw %}
```cpp
// http://www.prefield.com/algorithm/math/rational.html

template <class T>
struct rational {
    T p, q;
    void normalize() { // keep q positive
        if (q < 0) p *= -1, q *= -1;
        T d = gcd(p < 0 ? -p : p, q);
        if (d == 0)
            p = 0, q = 1;
        else
            p /= d, q /= d;
    }
    rational<T>(T p, T q = 1) : p(p), q(q) {
        normalize();
    }
    rational<T> &operator+=(const rational<T> &a) {
        p = a.q * p + a.p * q;
        q = a.q * q;
        normalize();
        return *this;
    }
    rational<T> &operator-=(const rational<T> &a) {
        p = a.q * p - a.p * q;
        q = a.q * q;
        normalize();
        return *this;
    }
    rational<T> &operator*=(const rational<T> &a) {
        p *= a.p;
        q *= a.q;
        normalize();
        return *this;
    }
    rational<T> &operator/=(const rational<T> &a) {
        p *= a.q;
        q *= a.p;
        normalize();
        return *this;
    }
    rational<T> &operator-() {
        p *= -1;
        return *this;
    }
};

template <class T>
rational<T> operator+(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) += b;
}
template <class T>
rational<T> operator*(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) *= b;
}
template <class T>
rational<T> operator-(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) -= b;
}
template <class T>
rational<T> operator/(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) /= b;
}
template <class T>
bool operator<(const rational<T> &a, const rational<T> &b) { // avoid overflow
    return (long double)a.p * b.q < (long double)a.q * b.p;
}
template <class T>
bool operator<=(const rational<T> &a, const rational<T> &b) {
    return !(b < a);
}
template <class T>
bool operator>(const rational<T> &a, const rational<T> &b) {
    return b < a;
}
template <class T>
bool operator>=(const rational<T> &a, const rational<T> &b) {
    return !(a < b);
}
template <class T>
bool operator==(const rational<T> &a, const rational<T> &b) {
    return !(a < b) && !(b < a);
}
template <class T>
bool operator!=(const rational<T> &a, const rational<T> &b) {
    return (a < b) || (b < a);
}
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "library/rational.hpp"
// http://www.prefield.com/algorithm/math/rational.html

template <class T>
struct rational {
    T p, q;
    void normalize() { // keep q positive
        if (q < 0) p *= -1, q *= -1;
        T d = gcd(p < 0 ? -p : p, q);
        if (d == 0)
            p = 0, q = 1;
        else
            p /= d, q /= d;
    }
    rational<T>(T p, T q = 1) : p(p), q(q) {
        normalize();
    }
    rational<T> &operator+=(const rational<T> &a) {
        p = a.q * p + a.p * q;
        q = a.q * q;
        normalize();
        return *this;
    }
    rational<T> &operator-=(const rational<T> &a) {
        p = a.q * p - a.p * q;
        q = a.q * q;
        normalize();
        return *this;
    }
    rational<T> &operator*=(const rational<T> &a) {
        p *= a.p;
        q *= a.q;
        normalize();
        return *this;
    }
    rational<T> &operator/=(const rational<T> &a) {
        p *= a.q;
        q *= a.p;
        normalize();
        return *this;
    }
    rational<T> &operator-() {
        p *= -1;
        return *this;
    }
};

template <class T>
rational<T> operator+(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) += b;
}
template <class T>
rational<T> operator*(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) *= b;
}
template <class T>
rational<T> operator-(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) -= b;
}
template <class T>
rational<T> operator/(const rational<T> &a, const rational<T> &b) {
    return rational<T>(a) /= b;
}
template <class T>
bool operator<(const rational<T> &a, const rational<T> &b) { // avoid overflow
    return (long double)a.p * b.q < (long double)a.q * b.p;
}
template <class T>
bool operator<=(const rational<T> &a, const rational<T> &b) {
    return !(b < a);
}
template <class T>
bool operator>(const rational<T> &a, const rational<T> &b) {
    return b < a;
}
template <class T>
bool operator>=(const rational<T> &a, const rational<T> &b) {
    return !(a < b);
}
template <class T>
bool operator==(const rational<T> &a, const rational<T> &b) {
    return !(a < b) && !(b < a);
}
template <class T>
bool operator!=(const rational<T> &a, const rational<T> &b) {
    return (a < b) || (b < a);
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

