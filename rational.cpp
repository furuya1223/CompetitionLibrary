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