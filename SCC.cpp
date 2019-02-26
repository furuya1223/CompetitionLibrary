/*class SCCD {
public:
	stack<int> post;
	vb used;

	void dfs(int pos, int par, const Graph &G) {
		used[pos] = true;
		rep(i, G[pos].size()) {
			int to = G[pos][i].to;
			if (used[to])continue;
			dfs(to, pos, G);
		}
		post.push(pos);
	}

	void dfsrev(int pos, int par, const Graph &rev, vi &group) {
		used[pos] = true;
		group.push_back(pos);
		rep(i, rev[pos].size()) {
			int to = rev[pos][i].to;
			if (used[to])continue;
			dfsrev(to, pos, rev, group);
		}
	}

	Graph SCC(Graph G, vvi &scc, vi &n2g) {
		int N = G.size();
		Graph rev(N);
		rep(i, N) {
			rep(j, G[i].size()) {
				Edge e = G[i][j];
				rev[e.to].push_back(Edge{ e.to,e.from,1 });
			}
		}

		used = vb(N, false);
		rep(i, N) {
			if (!used[i])dfs(i, -1, G);
		}

		fill(all(used), false);
		while (!post.empty()) {
			int pos = post.top();
			post.pop();
			if (used[pos])continue;
			vi group;
			dfsrev(pos, -1, rev, group);
			scc.push_back(group);
		}

		rep(i, scc.size()) {
			rep(j, scc[i].size()) {
				n2g[scc[i][j]] = i;
			}
		}

		Graph dag(scc.size());
		set<pair<int, int>> usedEdges;

		rep(i, N) {
			rep(j, G[i].size()) {
				Edge e = G[i][j];
				int s = n2g[e.from];
				int t = n2g[e.to];
				if (usedEdges.find(mp(s, t)) != usedEdges.end() && s != t) {
					dag[s].push_back(Edge{ s,t,1 });
					usedEdges.insert(mp(s, t));
				}
			}
		}

		return dag;
	}
};
*/