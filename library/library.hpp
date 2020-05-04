#include "header.hpp"

int digit_sum(long long n, long long b) {
    int sum = 0;
    while (n > 0) {
        sum += n % b;
        n /= b;
    }
    return sum;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double combi(long long n, long long r) {
    int i;
    double p = 1;
    for (i = 1; i <= r; i++) p = (double)p * (n - i + 1) / i;
    return p;
}

long long powMod(long long x, long long n, long long mod) {
    long long res = 1;
    while (n > 0) {
        if (n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

// 最大公約数
int gcd(int a, int b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// 階乗・組み合わせのモジュラ逆数
class FactorialMod {
    // モジュラ逆数を求める(modが素数と仮定)
    void ModInv() {
        inv[0] = 0;
        inv[1] = 1;
        for (int i = 2; i <= n; i++) {
            inv[i] = mod - ((mod / i) * inv[mod % i] % mod);
        }
    }

    // n!をmodで割った余り
    void FacInv() {
        fac[0] = facinv[0] = 1;
        for (int i = 1; i <= n; i++) {
            fac[i] = (fac[i - 1] * i) % mod;
            facinv[i] = (facinv[i - 1] * (int)inv[i]) % mod;
        }
    }

  public:
    int n;
    int mod;
    vector<long long> inv;
    vector<long long> fac;
    vector<long long> facinv;

    FactorialMod(int _n, int _mod)
        : n(_n), mod(_mod), inv(vector<long long>(_n + 1)),
          fac(vector<long long>(_n + 1)), facinv(vector<long long>(_n + 1)) {
        ModInv();
        FacInv();
    }

    ll CombiMod(int r, int k) {
        if (r == 0 && k == 0) return 1;
        if (r <= 0 || k < 0 || k > r) return 0;
        if (k == 0) return 1;

        return (((fac[r] * facinv[k]) % mod) * facinv[r - k]) % mod;
    }
};

// 座標圧縮
int compress(vector<int> x, map<int, int> &zip, vector<int> &unzip) {
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());
    for (int i = 0; i < x.size(); i++) {
        zip[x[i]] = i;
        unzip[i] = x[i];
    }
    return x.size();
}

vector<int> eratosthenes(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    repr(i, 2, n + 1) {
        if (!is_prime[i]) continue;
        repr(j, 2, n / i + 1) {
            is_prime[i * j] = false;
        }
    }
    vector<int> prime;
    rep(i, n + 1) {
        if (is_prime[i]) prime.push_back(i);
    }
    return prime;
}

int divisor_num(long long n, vector<int> &prime) {
    if (n == 0) return 0;
    if (prime.size() == 0) {
        prime = eratosthenes((int)sqrt(n));
    }
    int ans = 1;
    for (auto p : prime) {
        int s = 0;
        while (n % p == 0) {
            s++;
            n /= p;
        }
        ans *= s + 1;
    }
    return ans;
}

int popcount(int bits) {
    bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
    return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}

template <typename T>
struct cumsum {
    vector<long long> data;

    cumsum(vector<T> a) {
        data = vector<long long>(a.size() + 1, 0);
        for (size_t i = 0; i < a.size(); i++) {
            data[i + 1] = data[i] + a[i];
        }
    }

    long long calculate(int l, int r) {
        if (l > r) return 0;
        l = min<int>(data.size() - 1, max<int>(0, l));
        r = min<int>(data.size() - 1, max<int>(0, r));
        return data[r] - data[l];
    }
};

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

// 転倒数を求める
long long merge_count(vector<long long> &a) {
    int n = a.size();
    if (n <= 1) return 0;
    long long cnt = 0;
    vector<long long> b(a.begin(), a.begin() + n / 2);
    vector<long long> c(a.begin() + n / 2, a.end());

    cnt += merge_count(b);
    cnt += merge_count(c);

    int ai = 0, bi = 0, ci = 0;

    while (ai < n) {
        if (bi < b.size() && (ci == c.size() || b[bi] < c[ci])) {
            a[ai] = b[bi];
            bi++;
        } else {
            cnt += n / 2 - bi;
            a[ai] = c[ci];
            ci++;
        }
        ai++;
    }
    return cnt;
}

// LIS O(NlogN)
#define index_of(as, x)                                                        \
    distance(as.begin(), lower_bound(as.begin(), as.end(), x))
vector<int> lis(const vector<int> &a) {
    const int n = a.size();
    vector<int> A(n, INF);
    vector<int> id(n);
    for (int i = 0; i < n; ++i) {
        id[i] = index_of(A, a[i]);
        A[id[i]] = a[i];
    }
    int m = *max_element(id.begin(), id.end());
    vector<int> b(m + 1);
    for (int i = n - 1; i >= 0; --i)
        if (id[i] == m) b[m--] = a[i];
    return b;
}

// 素数テーブル O(N log log N)
void makePrimeTable(vector<bool> &prime) {
    fill(all(prime), true);
    prime[0] = false;
    prime[1] = false;
    rep(i, prime.size()) {
        if (!prime[i]) continue;
        repr(j, 2, (prime.size() - 1) / i + 1) {
            prime[i * j] = false;
        }
    }
}