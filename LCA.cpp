/*
class LCA {
public:
	int N;
	vi depth;
	int maxdepth = 0;
	vvi dblpar;
 
	LCA(const Graph &G) {
		N = G.size();
		depth = vi(N);
		dfs1(G, 0, -1);
		int D = bindigit(maxdepth) + 1;
		dblpar = vvi(D, vi(N));
		rep(i, D) {
			dfs2(G, 0, 0, i);
		}
	}
 
	void dfs1(const Graph &G, int pos, int par, int d = 0) {
		depth[pos] = d;
		maxdepth = max(maxdepth, d);
		for (auto e : G[pos]) {
			if (e.to == par)continue;
			dfs1(G, e.to, pos, d + 1);
		}
	}
 
	void dfs2(const Graph &G, int pos, int par, int dbl) {
		if (dbl == 0) {
			dblpar[0][pos] = par;
		}
		else {
			dblpar[dbl][pos] = dblpar[dbl - 1][dblpar[dbl - 1][pos]];
		}
		for (auto e : G[pos]) {
			if (e.to == par)continue;
			dfs2(G, e.to, pos, dbl);
		}
	}
 
	int parent(int pos, int step) {
		int ret = pos;
		int dbl = 0;
		while (step > 0) {
			if ((step & 1) != 0) {
				ret = dblpar[dbl][ret];
			}
			step >>= 1;
			dbl++;
		}
		return ret;
	}
 
	int bindigit(int n) {
		int ret = 0;
		while (n > 0) {
			ret++;
			n >>= 1;
		}
		return ret;
	}
 
	int lca(int u, int v) {
		if (depth[u] < depth[v]) {
			swap(u, v);
		}
		u = parent(u, depth[u] - depth[v]);
		int ng = -1, ok = depth[u];
		while (ok - ng > 1) {
			int mid = (ok + ng) / 2;
			if (parent(u, mid) == parent(v, mid)) {
				ok = mid;
			}
			else {
				ng = mid;
			}
		}
		return parent(u, ok);
	}
 
	int dist(int u, int v) {
		return depth[u] + depth[v] - 2 * depth[lca(u, v)];
	}
};
*/