#ifndef LIBRARY_EXTGCD_GARNER_HPP_
#define LIBRARY_EXTGCD_GARNER_HPP_

#include "header.hpp"

// solve ax+by=gcd(a,b)
// return gcd(a,b)
long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    long long x2, y2;
    long long d = ext_gcd(b, a % b, x2, y2);
    x = y2;
    y = x2 - (a / b) * y2;
    return d;
}

long long mod_inv(long long a, long long mod) {
    long long x, y;
    long long d = ext_gcd(a, mod, x, y);
    if (d != 1) {
        // there is no inverse
        return -1;
    }
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// x % m[i] = r[i] % m[i]
// gcd(m[i], m[j])=1 (i!=j) is needed
long long garner(vector<long long> r, vector<long long> m) {
    int n = r.size();
    long long m_prod = 1;      // m_prod means product of m[i]
    long long x = r[0] % m[0]; // initial value meets first condition
    for (int i = 1; i < n; i++) {
        m_prod *= m[i - 1]; // update cumurative product of m
        long long t = ((r[i] - x) * mod_inv(m_prod, m[i])) % m[i];
        if (t < 0) t += m[i]; // for modular of minus
        x += t * m_prod;      // update x
    }
    return x;
}

#endif // LIBRARY_EXTGCD_GARNER_HPP_