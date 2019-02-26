/*
// ax+by=gcd(a,b) の解 (x,y) を計算
// ついでに gcd(a,b) を返す
long long ext_gcd(long long a, long long b, long long &x, long long &y) {
	if (b == 0) {
		x = 1, y = 0;
		return a;
	}
	long long x2, y2;
	long long d = ext_gcd(b, a%b, x2, y2);
	x = y2;
	y = x2 - (a / b)*y2;
	return d;
}

// m を法としたときの a のモジュラ逆数を計算
long long mod_inv(long long a, long long m) {
	long long x, y;
	long long d = ext_gcd(a, m, x, y);
	if (d != 1) {
		// モジュラ逆数が存在しない
		return -1;
	}
	x %= m;
	if (x < 0)x += m;
	return x;
}

// x % m[i] = r[i] % m[i] を満たす正で最小の x を返す
// i!=j に対して gcd(m[i], m[j])=1 であると仮定
// とりあえず解の存在判定は保留
long long garner(vector<long long> r, vector<long long> m) {
	int n = r.size();
	long long m_prod = 1;       // m_prod には m[i] の積を入れていく
	long long x = r[0] % m[0];  // 最初の条件を満たすような x の初期値
	for (int i = 1; i < n; i++) {
		m_prod *= m[i - 1];     // m の累積積を更新
		long long t = ((r[i] - x) * mod_inv(m_prod, m[i])) % m[i];
		if (t < 0)t += m[i];    // 負の数の剰余の対策
		x += t * m_prod;        // x を更新
	}
	return x;
}
*/