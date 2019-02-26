/*
// �ő�t���[(Ford-Fulkerson)=================================================
//to:�s����,cap:�e��,rev:�t(G[e.to][e.rev]�ŋt�ӂ̍\���̂ɃA�N�Z�X�ł���B)
struct edge {
	int to, cap, rev;
};

vector<edge> G[220];
bool used[220];

//�ӂ�ǉ��Brev��,���ݎQ�Ƃł���悤�ɂ��Ă���Bsize�Ŏ������Ă��邪�A�ʂ̃��[����p���Ă��ǂ�(i.e �n�b�V��)�B�t�ӂ̋t�ӂ͒ʏ�̕ӂł���B
void add_edge(int from, int to, int cap)
{
	G[from].push_back(edge{ to, cap, (int)G[to].size() });
	G[to].push_back(edge{ from, 0, (int)G[from].size() - 1 });
}

//s����t�ֈړ��\�Ȃ炻�̃��[�g�̍ő嗬�ʂ�Ԃ��A�s�\�Ȃ�0��Ԃ��B
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

//s����t�ւ̍ő嗬�ʂ�Ԃ��B
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

// �ő�t���[(Dinic)=====================================================

struct edge { int to; ll cap; int rev; };
#define MAX_V 102
vector<edge> G[MAX_V];	// �O���t�̗אڃ��X�g�\��
ll level[MAX_V];	// s����̋���
int iter[MAX_V];	// �ǂ��܂Œ��׏I�������

					// from����to�֌������e��cap�̕ӂ��O���t�ɒǉ�����
void add_edge(int from, int to, ll cap)
{
	G[from].push_back(edge{ to, cap, (int)G[to].size() });
	G[to].push_back(edge{ from, 0, (int)G[from].size() - 1 });
}

// s����̍ŒZ������BFS�Ōv�Z
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

// �����p�X��DFS�ŒT��
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

// s����t�ւ̍ő嗬�����߂�
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
// �ŏ���p��============================================================

// �_�C�N�X�g��ver. O(FElogV)

struct edge { int to, cap, cost, rev; };

vector<edge> G[MAX_V];	// �O���t�̗אڃ��X�g�\��
int h[MAX_V];		// �|�e���V����
int dist[MAX_V];	// �ŒZ����
int prevv[MAX_V], preve[MAX_V];	// ���O�̒��_�ƕ�

// from����to�֌������e��cap, �R�X�gcost�̕ӂ��O���t�ɒǉ�����
void add_edge(int from, int to, int cap, int cost) {
	G[from].push_back(edge{ to, cap, cost, (int)G[to].size() });
	G[to].push_back(edge{ from, 0, -cost, (int)G[from].size() - 1 });
}

// s����t�ւ̗���f�̍ŏ���p�������߂�B�s�Ȃ�-1�BV�͒��_��
int min_cost_flow(int s, int t, int f, int V) {
	int res = 0;
	fill(h, h + V, 0);	// h��������
	while (f > 0) {
		// �_�C�N�X�g���@��p����h���X�V
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
			// ����ȏ㗬���Ȃ�
			return -1;
		}
		for (int v = 0; v < V; v++) h[v] += dist[v];

		// s-t�ԍŒZ�H�ɉ����Ėڈ�t����
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