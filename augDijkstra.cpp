#include "bits/stdc++.h"
#include <sys/timeb.h>
#include <fstream>

using namespace std;

#define repr(i,a,b) for(int i=(int)(a);i<(int)(b);i++)
#define rep(i,n) repr(i,0,n)
#define reprrev(i,a,b) for(int i=(int)(b)-1;i>=(int)(a);i--)
#define reprev(i,n) reprrev(i,0,n)
#define repi(itr,ds) for(auto itr=ds.begin();itr!=ds.end();itr++)
#define all(a) (a).begin(),(a).end()
#define rall(a) (a).rbegin(),(a).rend()
#define mp make_pair
#define mt make_tuple
#define INF 1050000000
#define INFL 1100000000000000000LL
#define EPS (1e-10)
#define MOD 1000000007
#define PI 3.1415926536
#define RMAX 4294967295

typedef long long ll;
typedef pair<int, int> Pi;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<bool> vb;
typedef vector<char> vc;
typedef vector<string> vs;
typedef vector<double> vd;
typedef vector<Pi> vPi;
typedef vector<vector<int> > vvi;
typedef vector<vector<bool> > vvb;
typedef vector<vector<ll> > vvll;
typedef vector<vector<char> > vvc;
typedef vector<vector<string> > vvs;
typedef vector<vector<double> > vvd;
typedef vector<vector<Pi> > vvPi;
typedef priority_queue<int, vector<int>, greater<int> > pqli;
typedef priority_queue<ll, vector<ll>, greater<ll> > pqlll;
typedef priority_queue<Pi, vector<Pi>, greater<Pi> > pqlP;
struct Edge {
	int from, to, cost;
	bool operator<(Edge e) {
		return cost < e.cost;
	}
};
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;
template <class T>
using vec = vector<T>;
template<class T>
using pql = priority_queue<T, vector<T>, greater<T>>;

string debug_show(Pi a) {
	return "(" + to_string(a.first) + "," + to_string(a.second) + ")";
}

template<class T>
struct augEdge {
	T from, to;
	int cost;
	bool operator<(Edge e) {
		return cost < e.cost;
	}
	bool operator>(Edge e) {
		return cost > e.cost;
	}
};

template<class T>
void augDijkstra(map<T,vector<augEdge<T>>> graph, T start, map<T, int> &cost) {
	//vi prev(V);
	pql<pair<int, T>> Q;

	cost[start] = 0;

	Q.push(mp(0, start));	// (cost, index)

	while (!Q.empty()) {
		T pos = Q.top().second;
		int d = Q.top().first;
		Q.pop();
		rep(i, graph[pos].size()) {
			T to = graph[pos][i].to;
			int c = graph[pos][i].cost;
			if (cost.find(to) == cost.end() || cost[to] > cost[pos] + c) {
				cost[to] = cost[pos] + c;
				Q.push(mp(cost[to], to));
				//prev[graph[pos][i].to] = pos;
			}
		}
	}
}

int main() {
	cin.tie(0);
	ios::sync_with_stdio(false);

	int N, M;
	cin >> N >> M;

	map<Pi, vector<augEdge<Pi>>> G;


	rep(i, M) {
		int p, q, c;
		cin >> p >> q >> c;
		G[mp(p, 0)].push_back(augEdge<Pi>{ mp(p, 0), mp(p, c), 1 });
		G[mp(p, c)].push_back(augEdge<Pi>{ mp(p, c), mp(p, 0), 1 });
		G[mp(q, 0)].push_back(augEdge<Pi>{ mp(q, 0), mp(q, c), 1 });
		G[mp(q, c)].push_back(augEdge<Pi>{ mp(q, c), mp(q, 0), 1 });
		G[mp(p, c)].push_back(augEdge<Pi>{ mp(p, c), mp(q, c), 0 });
		G[mp(q, c)].push_back(augEdge<Pi>{ mp(q, c), mp(p, c), 0 });
	}

	map<Pi, int> cost;

	augDijkstra<Pi>(G, mp(1, 0), cost);

	if (cost.find(mp(N, 0)) == cost.end())cout << -1 << endl;
	else cout << (cost[mp(N, 0)] - 1) / 2 + 1 << endl;

	return 0;
}