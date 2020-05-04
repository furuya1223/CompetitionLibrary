/*
// 最大フロー(Ford-Fulkerson)=================================================
//to:行き先,cap:容量,rev:逆(G[e.to][e.rev]で逆辺の構造体にアクセスできる。)
struct edge {
	int to, cap, rev;
};

vector<edge> G[220];
bool used[220];

//辺を追加。revは,相互参照できるようにしている。sizeで実現しているが、別のルールを用いても良い(i.e ハッシュ)。逆辺の逆辺は通常の辺である。
void add_edge(int from, int to, int cap)
{
	G[from].push_back(edge{ to, cap, (int)G[to].size() });
	G[to].push_back(edge{ from, 0, (int)G[from].size() - 1 });
}

//sからtへ移動可能ならそのルートの最大流量を返し、不可能なら0を返す。
int dfs(int v, int t, int f)
{
	if (v == t) return f;
	used[v] = true;
	rep(i, G[v].size()) {
		edge &e = G[v][i];
		if (!used[e.to] && e.cap>0) {
			int d = dfs(e.to, t, min(f, e.cap));
			if (d > 0) {
				e.cap -= d;
				G[e.to][e.rev].cap += d;
				return d;
			}
		}
	}
	return 0;
}

//sからtへの最大流量を返す。
int
max_flow(int s, int t)
{
	int flow = 0;
	while (true) {
		memset(used, 0, sizeof(used));
		int f = dfs(s, t, INF);
		if (f == 0) {
			return flow;
		}
		flow += f;
	}
}

// 最大フロー(Dinic)=====================================================

struct edge { int to; ll cap; int rev; };
#define MAX_V 102
vector<edge> G[MAX_V];	// グラフの隣接リスト表現
ll level[MAX_V];	// sからの距離
int iter[MAX_V];	// どこまで調べ終わったか

					// fromからtoへ向かう容量capの辺をグラフに追加する
void add_edge(int from, int to, ll cap)
{
	G[from].push_back(edge{ to, cap, (int)G[to].size() });
	G[to].push_back(edge{ from, 0, (int)G[from].size() - 1 });
}

// sからの最短距離をBFSで計算
void bfs(int s) {
	memset(level, -1, sizeof(level));
	queue<int> que;
	level[s] = 0;
	que.push(s);
	while (!que.empty()) {
		int v = que.front();
		que.pop();
		for (int i = 0; i < G[v].size(); i++) {
			edge &e = G[v][i];
			if (e.cap > 0 && level[e.to] < 0) {
				level[e.to] = level[v] + 1;
				que.push(e.to);
			}
		}
	}
}

// 増加パスをDFSで探す
ll dfs(int v, int t, ll f) {
	if (v == t)return f;
	for (int &i = iter[v]; i < G[v].size(); i++) {
		edge &e = G[v][i];
		if (e.cap > 0 && level[v] < level[e.to]) {
			ll d = dfs(e.to, t, min(f, e.cap));
			if (d > 0) {
				e.cap -= d;
				G[e.to][e.rev].cap += d;
				return d;
			}
		}
	}
	return 0;
}

// sからtへの最大流を求める
ll max_flow(int s, int t) {
	ll flow = 0;
	while (true) {
		bfs(s);
		if (level[t] < 0)return flow;
		memset(iter, 0, sizeof(iter));
		ll f;
		while ((f = dfs(s, t, INF)) > 0) {
			flow += f;
		}
	}
}
// 最小費用流============================================================

// ダイクストラver. O(FElogV)

struct edge { int to, cap, cost, rev; };

vector<edge> G[MAX_V];	// グラフの隣接リスト表現
int h[MAX_V];		// ポテンシャル
int dist[MAX_V];	// 最短距離
int prevv[MAX_V], preve[MAX_V];	// 直前の頂点と辺

// fromからtoへ向かう容量cap, コストcostの辺をグラフに追加する
void add_edge(int from, int to, int cap, int cost) {
	G[from].push_back(edge{ to, cap, cost, (int)G[to].size() });
	G[to].push_back(edge{ from, 0, -cost, (int)G[from].size() - 1 });
}

// sからtへの流量fの最小費用流を求める。不可なら-1。Vは頂点数
int min_cost_flow(int s, int t, int f, int V) {
	int res = 0;
	fill(h, h + V, 0);	// hを初期化
	while (f > 0) {
		// ダイクストラ法を用いてhを更新
		priority_queue<P, vector<P>, greater<P>> que;
		fill(dist, dist + V, INF);
		dist[s] = 0;
		que.push(mp(0, s));
		while (!que.empty()) {
			P p = que.top();
			que.pop();
			int v = p.second;
			if (dist[v] < p.first)continue;
			for (int i = 0; i < G[v].size(); i++) {
				edge &e = G[v][i];
				if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
					dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
					prevv[e.to] = v;
					preve[e.to] = i;
					que.push(mp(dist[e.to], e.to));
				}
			}
		}
		if (dist[t] == INF) {
			// これ以上流せない
			return -1;
		}
		for (int v = 0; v < V; v++) h[v] += dist[v];

		// s-t間最短路に沿って目一杯流す
		int d = f;
		for (int v = t; v != s; v = prevv[v]) {
			d = min(d, G[prevv[v]][preve[v]].cap);
		}
		f -= d;
		res += d * h[t];
		for (int v = t; v != s; v = prevv[v]) {
			edge &e = G[prevv[v]][preve[v]];
			e.cap -= d;
			G[v][e.rev].cap += d;
		}
	}
	return res;
}

*/