/*
std::ifstream in("input.txt");
std::cin.rdbuf(in.rdbuf());
ofstream ofs("output.txt");
cout.rdbuf(ofs.rdbuf());

int digitSum(int n) {
	int sum = 0;
	string str = to_string(n);
	for (auto c : str) {
		sum += c - '0';
	}
	return sum;
}

double dist(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double combi(ll n, ll r) {
	int i;
	double p = 1;
	for (i = 1; i <= r; i++)
		p = (double)p*(n - i + 1) / i;
	return p;
}

ll powMod(ll x, ll n, ll mod) {
	ll res = 1;
	while (n > 0) {
		if (n & 1) res = res * x % mod;
		x = x * x % mod;
		n >>= 1;
	}
	return res;
}

// 最大公約数
int gcd(int a, int b)
{
	if (b == 0) return a;
	else return gcd(b, a % b);
}


// モジュラ逆数を求める(modが素数と仮定)
void ModInv(int n, vector<ll> &inv, int mod)
{
	inv[0] = 0;
	inv[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		inv[i] = mod - ((mod / i) * inv[mod % i] % mod);
	}
}

// n!をmodで割った余り
void FacInv(int n, vector<ll> &inv, vector<ll> &fac, vector<ll> &facInv, int mod)
{
	fac[0] = facInv[0] = 1;
	for (int i = 1; i <= n; i++)
	{
		fac[i] = (fac[i - 1] * i) % mod;
		facInv[i] = (facInv[i - 1] * (int)inv[i]) % mod;
	}
}

// nCkをmodで割った余りを求める。mod素数限定
// ModInv()とFacInv()が必要
ll CombiMod(int n, int k, int mod)
{
	if (n == 0 && k == 0) return 1;
	if (n <= 0 || k < 0 || k > n) return 0;
	if (k == 0) return 1;

	vector<ll> inv(n + 1);
	vector<ll> fac(n + 1);
	vector<ll> facInv(n + 1);

	ModInv(n, inv, mod);
	FacInv(n, inv, fac, facInv, mod);
	return (((fac[n] * facInv[k]) % mod) * facInv[n - k]) % mod;
}

// 座標圧縮
int compress(vector<int> x, map<int,int> &zip, vi &unzip) {
	sort(x.begin(), x.end());
	x.erase(unique(x.begin(), x.end()), x.end());
	for (int i = 0; i < x.size(); i++) {
		zip[x[i]] = i;
		unzip[i] = x[i];
	}
	return x.size();
}

int popcount(int bits) {
	bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
	bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
	bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
	bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
}


template<typename T>
struct cumsum {
	vector<long long> data;
	
	cumsum(vector<T> a) {
		data = vector<long long>(a.size() + 1, 0);
		for (size_t i = 0; i < a.size(); i++) {
			data[i + 1] = data[i] + a[i];
		}
	}

	long long calculate(int l, int r) {
		if (l > r)return 0;
		l = min<int>(data.size() - 1, max<int>(0, l));
		r = min<int>(data.size() - 1, max<int>(0, r));
		return data[r] - data[l];
	}
};

class SegTree {
public:
	int n, height;
	vector<ll> dat;

	ll def = 0;
	ll operation(ll a, ll b) {
		return a + b;
	}

	// 初期化（_nは必要な要素数）
	SegTree(int _n) {
		n = 1;
		height = 1;
		while (n < _n) {
			n *= 2;
			height++;
		}
		dat = vector<ll>(2 * n - 1, def);
	}

	// 場所i(0-indexed)にxを足す
	void add(int i, ll x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

	// 場所i(0-indexed)の値をxに更新
	void change(int i, ll x) {
		i += n - 1;
		dat[i] = x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] = operation(dat[i * 2 + 1], dat[i * 2 + 2]);
		}
	}

	// 区間[a,b)の総和。ノードk=[l,r)に着目している。
	ll _query(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return def;  // 交差しない
		if (a <= l && r <= b)return dat[k];   // a,l,r,bの順で完全に含まれる
		else {
			ll c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2);    // 左の子
			ll c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r);    // 右の子
			return operation(c1, c2);
		}
	}

	// 内部クエリ_query()を呼び出す
	ll query(int a, int b) {
		return _query(a, b, 0, 0, n);
	}
};

// 区間塗りつぶしRMQ型
class LazySegTree {
public:
	int n;
	vector<ll> dat;
	vector<ll> lazy;
	vector<int> depth;
	vector<int> width;
	const int NIL = -INF;

	ll def = INT32_MAX;
	ll operation(ll a, ll b) {
		return min(a, b);
	}

	// 初期化（_nは必要な要素数）
	LazySegTree(int _n) {
		n = 1;
		while (n < _n) {
			n *= 2;
		}
		dat = vector<ll>(2 * n - 1, def);
		lazy = vector<ll>(2 * n - 1, NIL);
		depth = vector<int>(2 * n - 1);
		width = vector<int>(2 * n - 1);
		depth[0] = 0;
		width[0] = n;
		repl(i, 1, 2 * n - 1) {
			depth[i] = depth[(i - 1) / 2] + 1;
			width[i] = width[(i - 1) / 2] / 2;
		}
	}

	// ツリー上のノードkにLazy値vをセット
	void setLazy(int k, int v) {
		// 塗りつぶし
		lazy[k] = v;

		// min,maxの場合
		dat[k] = v;

		// sumの場合
		//dat[k] = lazy[k] * width[k];
	}

	void push(int k) {
		if (lazy[k] == NIL)return;
		setLazy(k * 2 + 1, lazy[k]);
		setLazy(k * 2 + 2, lazy[k]);
		lazy[k] = NIL;
	}

	// 区間[a,b)をvで塗りつぶす
	void fill(int a, int b, int v, int k = 0, int l = 0, int r = -1) {
		if (r == -1)r = n;
		if (r <= a || b <= l)return;  // 交差しない
		if (a <= l && r <= b) {   // a,l,r,bの順で完全に含まれる
			setLazy(k, v);
			return;
		}

		push(k);

		fill(a, b, v, 2 * k + 1, l, (l + r) / 2);    // 左の子
		fill(a, b, v, 2 * k + 2, (l + r) / 2, r);    // 右の子

		dat[k] = operation(dat[k * 2 + 1], dat[k * 2 + 2]);
	}

	// 場所i(0-indexed)にxを足す
	void add(int i, ll x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

	// 場所i(0-indexed)の値をxに更新
	void change(int i, ll x) {
		i += n - 1;
		dat[i] = x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] = operation(dat[i * 2 + 1], dat[i * 2 + 2]);
		}
	}

	// 区間[a,b)の総和。ノードk=[l,r)に着目している。
	ll _query(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return def;  // 交差しない
		if (a <= l && r <= b)return dat[k];   // a,l,r,bの順で完全に含まれる

		push(k);

		ll c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2);    // 左の子
		ll c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r);    // 右の子
		return operation(c1, c2);
	}

	// 内部クエリ_query()を呼び出す
	ll query(int a, int b) {
		return _query(a, b, 0, 0, n);
	}

	ll operator[](int i) {
		//return dat[i + n - 1];
		return query(i, i + 1);
	}
};

// 区間加算区間和型
class LazySegTree {
public:
	int n;
	vector<ll> dat;
	vector<ll> lazy;
	vector<int> depth;
	vector<int> width;
	const int NIL = -INF;

	ll def = 0;
	ll operation(ll a, ll b) {
		return a + b;
	}

	// 初期化（_nは必要な要素数）
	LazySegTree(int _n) {
		n = 1;
		while (n < _n) {
			n *= 2;
		}
		dat = vector<ll>(2 * n - 1, def);
		lazy = vector<ll>(2 * n - 1, NIL);
		depth = vector<int>(2 * n - 1);
		width = vector<int>(2 * n - 1);
		depth[0] = 0;
		width[0] = n;
		repl(i, 1, 2 * n - 1) {
			depth[i] = depth[(i - 1) / 2] + 1;
			width[i] = width[(i - 1) / 2] / 2;
		}
	}

	// ツリー上のノードkにLazy値vをセット
	void setLazy(int k, ll v) {
		if (lazy[k] == NIL) {
			lazy[k] = v;
		}
		else {
			lazy[k] += v;
		}

		// min,maxの場合
		//dat[k] += v;

		// sumの場合
		dat[k] += v * width[k];
	}

	void push(int k) {
		//cout << "push: " << k << endl;
		if (lazy[k] == NIL)return;
		setLazy(k * 2 + 1, lazy[k]);
		setLazy(k * 2 + 2, lazy[k]);
		lazy[k] = NIL;
	}

	// 区間[a,b)にvを足す
	void rangeAdd(int a, int b, ll v, int k = 0, int l = 0, int r = -1) {
		if (r == -1)r = n;
		if (r <= a || b <= l)return;  // 交差しない
		if (a <= l && r <= b) {   // a,l,r,bの順で完全に含まれる
			setLazy(k, v);
			return;
		}

		push(k);

		rangeAdd(a, b, v, 2 * k + 1, l, (l + r) / 2);    // 左の子
		rangeAdd(a, b, v, 2 * k + 2, (l + r) / 2, r);    // 右の子

		dat[k] = operation(dat[k * 2 + 1], dat[k * 2 + 2]);
	}

	// 区間[a,b)の総和。ノードk=[l,r)に着目している。
	ll _query(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return def;  // 交差しない
		if (a <= l && r <= b)return dat[k];   // a,l,r,bの順で完全に含まれる

		push(k);

		ll c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2);    // 左の子
		ll c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r);    // 右の子
		return operation(c1, c2);
	}

	// 内部クエリ_query()を呼び出す
	ll query(int a, int b) {
		return _query(a, b, 0, 0, n);
	}

	ll operator[](int i) {
		//return dat[i + n - 1];
		return query(i, i + 1);
	}
};

struct SumSegTree {
	int n, height;
	vector<int> dat;

	// 初期化（_nは最大要素数）
	SumSegTree(int _n) {
		n = 1;
		height = 1;
		while (n < _n) {
			n *= 2;
			height++;
		}
		dat = vector<int>(2 * n - 1);
	}

	// 場所i(0-indexed)にxを足す
	void add(int i, int x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

	// 区間[a,b)の総和。ノードk=[l,r)に着目している。
	int _sum(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return 0;	// 交差しない
		if (a <= l && r <= b)return dat[k];	// a,l,r,bの順で完全に含まれる
		else {
			int s1 = _sum(a, b, 2 * k + 1, l, (l + r) / 2);	// 左の子
			int s2 = _sum(a, b, 2 * k + 2, (l + r) / 2, r);	// 右の子
			return s1 + s2;
		}
	}

	// 内部クエリ_sum()を呼び出す
	int sum(int a, int b) {
		return _sum(a, b, 0, 0, n);
	}
};

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
			i -= i & -i;	// i & -i は i の最後の1ビット
		}
		return s;
	}

	void add(int i, int x) {
		i++;	// 0-indexedに変更
		while (i <= n) {
			dat[i] += x;
			i += i & -i;
		}
	}
};

void Z_algorithm(string str, vector<int> &Z) {
	const int L = str.length();
	for (int i = 1, left = 0, right = 0; i < L; i++) {
		if (i > right) {
			left = right = i;
			for (; right < L && str[right - left] == str[right]; right++);
			Z[i] = right - left;
			right--;
		}
		else {
			int k = i - left;
			if (Z[k] < right - i + 1) {
				Z[i] = Z[k];
			}
			else {
				left = i;
				for (; right < L && str[right - left] == str[right]; right++);
				Z[i] = right - left;
				right--;
			}
		}
	}
}

// 転倒数を求める
ll merge_count(vll &a) {
	int n = a.size();
	if (n <= 1)return 0;
	ll cnt = 0;
	vll b(a.begin(), a.begin() + n / 2);
	vll c(a.begin() + n / 2, a.end());

	cnt += merge_count(b);
	cnt += merge_count(c);

	int ai = 0, bi = 0, ci = 0;

	while (ai < n) {
		if (bi < b.size() && (ci == c.size() || b[bi] < c[ci])) {
			a[ai] = b[bi];
			bi++;
		}
		else {
			cnt += n / 2 - bi;
			a[ai] = c[ci];
			ci++;
		}
		ai++;
	}
	return cnt;
}

// LIS O(NlogN)
#define index_of(as, x) \
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
void makePrimeTable(vb &prime) {
	fill(all(prime), true);
	prime[0] = false;
	prime[1] = false;
	rep(i, prime.size()) {
		if (!prime[i])continue;
		repr(j, 2, (prime.size() - 1) / i + 1) {
			prime[i*j] = false;
		}
	}
}

*/