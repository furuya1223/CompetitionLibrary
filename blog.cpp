#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define repr(i,a,b) for(int i=(int)(a);i<(int)(b);i++)
#define rep(i,n) repr(i,0,n)

typedef long long ll;
typedef vector<bool> vb;
typedef vector<ll> vll;
typedef vector<vector<ll> > vvll;

int N, M;
vll x[3];

ll num(ll a, bool f) {
	return (f ? a : -a);
}

ll solve(vb f) {
	vvll dp(N + 1, vll(M + 1, 0));
	rep(i, N) {
		rep(j, M) {
			if (j > i)continue;
			ll next = dp[i][j];
			rep(k, 3) {
				next += num(x[k][i], f[k]);
			}

			if (j == i) {
				dp[i + 1][j + 1] = next;
			}
			else {
				dp[i + 1][j + 1] = max(dp[i][j + 1], next);
			}
		}
	}
	return dp[N][M];
}

int main() {
	cin >> N >> M;
	rep(i, 3) {
		x[i] = vll(N);
	}
	rep(i, N) {
		rep(j, 3) {
			cin >> x[j][i];
		}
	}

	vb f = { true,false };
	ll ma = 0;
	rep(i, 2)rep(j, 2)rep(k, 2) {
		ll s = solve({ f[i], f[j], f[k] });
		ma = max(ma, s);
	}
	cout << ma << endl;

	return 0;
}
