/*
// ax+by=gcd(a,b) �̉� (x,y) ���v�Z
// ���ł� gcd(a,b) ��Ԃ�
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

// m ��@�Ƃ����Ƃ��� a �̃��W�����t�����v�Z
long long mod_inv(long long a, long long m) {
	long long x, y;
	long long d = ext_gcd(a, m, x, y);
	if (d != 1) {
		// ���W�����t�������݂��Ȃ�
		return -1;
	}
	x %= m;
	if (x < 0)x += m;
	return x;
}

// x % m[i] = r[i] % m[i] �𖞂������ōŏ��� x ��Ԃ�
// i!=j �ɑ΂��� gcd(m[i], m[j])=1 �ł���Ɖ���
// �Ƃ肠�������̑��ݔ���͕ۗ�
long long garner(vector<long long> r, vector<long long> m) {
	int n = r.size();
	long long m_prod = 1;       // m_prod �ɂ� m[i] �̐ς����Ă���
	long long x = r[0] % m[0];  // �ŏ��̏����𖞂����悤�� x �̏����l
	for (int i = 1; i < n; i++) {
		m_prod *= m[i - 1];     // m �̗ݐϐς��X�V
		long long t = ((r[i] - x) * mod_inv(m_prod, m[i])) % m[i];
		if (t < 0)t += m[i];    // ���̐��̏�]�̑΍�
		x += t * m_prod;        // x ���X�V
	}
	return x;
}
*/