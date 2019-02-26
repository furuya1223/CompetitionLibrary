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

// �ő����
int gcd(int a, int b)
{
	if (b == 0) return a;
	else return gcd(b, a % b);
}


// ���W�����t�������߂�(mod���f���Ɖ���)
void ModInv(int n, vector<ll> &inv, int mod)
{
	inv[0] = 0;
	inv[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		inv[i] = mod - ((mod / i) * inv[mod % i] % mod);
	}
}

// n!��mod�Ŋ������]��
void FacInv(int n, vector<ll> &inv, vector<ll> &fac, vector<ll> &facInv, int mod)
{
	fac[0] = facInv[0] = 1;
	for (int i = 1; i <= n; i++)
	{
		fac[i] = (fac[i - 1] * i) % mod;
		facInv[i] = (facInv[i - 1] * (int)inv[i]) % mod;
	}
}

// nCk��mod�Ŋ������]������߂�Bmod�f������
// ModInv()��FacInv()���K�v
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

// ���W���k
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

	// �������i_n�͕K�v�ȗv�f���j
	SegTree(int _n) {
		n = 1;
		height = 1;
		while (n < _n) {
			n *= 2;
			height++;
		}
		dat = vector<ll>(2 * n - 1, def);
	}

	// �ꏊi(0-indexed)��x�𑫂�
	void add(int i, ll x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

	// �ꏊi(0-indexed)�̒l��x�ɍX�V
	void change(int i, ll x) {
		i += n - 1;
		dat[i] = x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] = operation(dat[i * 2 + 1], dat[i * 2 + 2]);
		}
	}

	// ���[a,b)�̑��a�B�m�[�hk=[l,r)�ɒ��ڂ��Ă���B
	ll _query(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return def;  // �������Ȃ�
		if (a <= l && r <= b)return dat[k];   // a,l,r,b�̏��Ŋ��S�Ɋ܂܂��
		else {
			ll c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2);    // ���̎q
			ll c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r);    // �E�̎q
			return operation(c1, c2);
		}
	}

	// �����N�G��_query()���Ăяo��
	ll query(int a, int b) {
		return _query(a, b, 0, 0, n);
	}
};

// ��ԓh��Ԃ�RMQ�^
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

	// �������i_n�͕K�v�ȗv�f���j
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

	// �c���[��̃m�[�hk��Lazy�lv���Z�b�g
	void setLazy(int k, int v) {
		// �h��Ԃ�
		lazy[k] = v;

		// min,max�̏ꍇ
		dat[k] = v;

		// sum�̏ꍇ
		//dat[k] = lazy[k] * width[k];
	}

	void push(int k) {
		if (lazy[k] == NIL)return;
		setLazy(k * 2 + 1, lazy[k]);
		setLazy(k * 2 + 2, lazy[k]);
		lazy[k] = NIL;
	}

	// ���[a,b)��v�œh��Ԃ�
	void fill(int a, int b, int v, int k = 0, int l = 0, int r = -1) {
		if (r == -1)r = n;
		if (r <= a || b <= l)return;  // �������Ȃ�
		if (a <= l && r <= b) {   // a,l,r,b�̏��Ŋ��S�Ɋ܂܂��
			setLazy(k, v);
			return;
		}

		push(k);

		fill(a, b, v, 2 * k + 1, l, (l + r) / 2);    // ���̎q
		fill(a, b, v, 2 * k + 2, (l + r) / 2, r);    // �E�̎q

		dat[k] = operation(dat[k * 2 + 1], dat[k * 2 + 2]);
	}

	// �ꏊi(0-indexed)��x�𑫂�
	void add(int i, ll x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

	// �ꏊi(0-indexed)�̒l��x�ɍX�V
	void change(int i, ll x) {
		i += n - 1;
		dat[i] = x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] = operation(dat[i * 2 + 1], dat[i * 2 + 2]);
		}
	}

	// ���[a,b)�̑��a�B�m�[�hk=[l,r)�ɒ��ڂ��Ă���B
	ll _query(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return def;  // �������Ȃ�
		if (a <= l && r <= b)return dat[k];   // a,l,r,b�̏��Ŋ��S�Ɋ܂܂��

		push(k);

		ll c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2);    // ���̎q
		ll c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r);    // �E�̎q
		return operation(c1, c2);
	}

	// �����N�G��_query()���Ăяo��
	ll query(int a, int b) {
		return _query(a, b, 0, 0, n);
	}

	ll operator[](int i) {
		//return dat[i + n - 1];
		return query(i, i + 1);
	}
};

// ��ԉ��Z��Ԙa�^
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

	// �������i_n�͕K�v�ȗv�f���j
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

	// �c���[��̃m�[�hk��Lazy�lv���Z�b�g
	void setLazy(int k, ll v) {
		if (lazy[k] == NIL) {
			lazy[k] = v;
		}
		else {
			lazy[k] += v;
		}

		// min,max�̏ꍇ
		//dat[k] += v;

		// sum�̏ꍇ
		dat[k] += v * width[k];
	}

	void push(int k) {
		//cout << "push: " << k << endl;
		if (lazy[k] == NIL)return;
		setLazy(k * 2 + 1, lazy[k]);
		setLazy(k * 2 + 2, lazy[k]);
		lazy[k] = NIL;
	}

	// ���[a,b)��v�𑫂�
	void rangeAdd(int a, int b, ll v, int k = 0, int l = 0, int r = -1) {
		if (r == -1)r = n;
		if (r <= a || b <= l)return;  // �������Ȃ�
		if (a <= l && r <= b) {   // a,l,r,b�̏��Ŋ��S�Ɋ܂܂��
			setLazy(k, v);
			return;
		}

		push(k);

		rangeAdd(a, b, v, 2 * k + 1, l, (l + r) / 2);    // ���̎q
		rangeAdd(a, b, v, 2 * k + 2, (l + r) / 2, r);    // �E�̎q

		dat[k] = operation(dat[k * 2 + 1], dat[k * 2 + 2]);
	}

	// ���[a,b)�̑��a�B�m�[�hk=[l,r)�ɒ��ڂ��Ă���B
	ll _query(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return def;  // �������Ȃ�
		if (a <= l && r <= b)return dat[k];   // a,l,r,b�̏��Ŋ��S�Ɋ܂܂��

		push(k);

		ll c1 = _query(a, b, 2 * k + 1, l, (l + r) / 2);    // ���̎q
		ll c2 = _query(a, b, 2 * k + 2, (l + r) / 2, r);    // �E�̎q
		return operation(c1, c2);
	}

	// �����N�G��_query()���Ăяo��
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

	// �������i_n�͍ő�v�f���j
	SumSegTree(int _n) {
		n = 1;
		height = 1;
		while (n < _n) {
			n *= 2;
			height++;
		}
		dat = vector<int>(2 * n - 1);
	}

	// �ꏊi(0-indexed)��x�𑫂�
	void add(int i, int x) {
		i += n - 1;
		dat[i] += x;
		while (i > 0) {
			i = (i - 1) / 2;
			dat[i] += x;
		}
	}

	// ���[a,b)�̑��a�B�m�[�hk=[l,r)�ɒ��ڂ��Ă���B
	int _sum(int a, int b, int k, int l, int r) {
		if (r <= a || b <= l)return 0;	// �������Ȃ�
		if (a <= l && r <= b)return dat[k];	// a,l,r,b�̏��Ŋ��S�Ɋ܂܂��
		else {
			int s1 = _sum(a, b, 2 * k + 1, l, (l + r) / 2);	// ���̎q
			int s2 = _sum(a, b, 2 * k + 2, (l + r) / 2, r);	// �E�̎q
			return s1 + s2;
		}
	}

	// �����N�G��_sum()���Ăяo��
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

	// [0,i)�܂ł̘a
	int sum(int i) {
		int s = 0;
		while (i > 0) {
			s += dat[i];
			i -= i & -i;	// i & -i �� i �̍Ō��1�r�b�g
		}
		return s;
	}

	void add(int i, int x) {
		i++;	// 0-indexed�ɕύX
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

// �]�|�������߂�
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

// �f���e�[�u�� O(N log log N)
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