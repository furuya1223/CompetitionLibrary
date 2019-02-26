/*
class Lowlink {
public:
	vi ord, low;
	vb vis;
	vi articulation;
	vPi bridge;
	Graph BCCtree;
	vi n2g;
	vvi component;
	set<Pi>bridgeSet;
 
	Lowlink(const Graph &G) {
		ord = vi(G.size());
		low = vi(G.size());
		vis = vb(G.size(), false);
		int k = 0;
		for (int i = 0; i < G.size(); i++) {
			if (!vis[i]) dfs(G, i, -1, k);
		}
	}
 
	void dfs(const Graph &G, int v, int p, int &k)
	{
		vis[v] = true;
 
		ord[v] = k++;
		low[v] = ord[v];
 
		bool isArticulation = false;
		int ct = 0;
 
		for (int i = 0; i < G[v].size(); i++) {
			if (!vis[G[v][i].to]) {
				ct++;
				dfs(G, G[v][i].to, v, k);
				low[v] = min(low[v], low[G[v][i].to]);
				if (p != -1 && ord[v] <= low[G[v][i].to]) isArticulation = true;
				if (ord[v] < low[G[v][i].to]) {
					bridge.push_back(make_pair(min(v, G[v][i].to), max(v, G[v][i].to)));
					bridgeSet.insert(make_pair(min(v, G[v][i].to), max(v, G[v][i].to)));
				}
			}
			else if (G[v][i].to != p) {
				low[v] = min(low[v], ord[G[v][i].to]);
			}
		}
 
		if (p == -1 && ct > 1) isArticulation = true;
		if (isArticulation) articulation.push_back(v);
	}
 
	void bcc(const Graph &G) {
		if (bridge.size() == 0) {
			component = vvi(1);
			n2g = vi(G.size());
			rep(i, G.size()) {
				component[0].push_back(i);
				n2g[i] = 0;
			}
			BCCtree = Graph(1);
			return;
		}
		int k = 0;
		n2g = vi(G.size(), -1);
		vis = vb(G.size(), false);
		component = vvi(G.size());

 
		for (Pi e : bridge) {
			if (!vis[e.first]) {
				bccdfs(G, e.first, -1, k);
				k++;
			}
			if (!vis[e.second]) {
				bccdfs(G, e.second, -1, k);
				k++;
			}
		}
		BCCtree = Graph(k);
		component.resize(k);
		for (Pi e : bridge) {
			int a = n2g[e.first];
			int b = n2g[e.second];
			BCCtree[a].push_back(Edge{ a,b,1 });
			BCCtree[b].push_back(Edge{ b,a,1 });
		}
	}
 
	void bccdfs(const Graph &G, int pos, int par, int k) {
		vis[pos] = true;
		n2g[pos] = k;
		component[k].push_back(pos);
		for (Edge e : G[pos]) {
			if (vis[e.to]) continue;
			if (bridgeSet.find(mp(min(e.from, e.to), max(e.from, e.to))) != bridgeSet.end())continue;
			bccdfs(G, e.to, pos, k);
		}
	}
};
*/