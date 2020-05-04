#include "header.hpp"

template <class T, class C = int>
struct augEdge {
    T from, to;
    C cost;
    bool operator<(augEdge e) {
        return cost < e.cost;
    }
    bool operator>(augEdge e) {
        return cost > e.cost;
    }
};
template <class T, class C = int>
class augGraph {
    int n;
    bool directed;
    map<T, vector<augEdge<T, C>>> edges;

  public:
    augGraph(bool directed)
        : directed(directed), edges(map<T, vector<augEdge<T, C>>>()) {}

    void add_edge(T s, T t, C cost) {
        edges[s].push_back(augEdge<T, C>{s, t, cost});
        if (!directed) {
            edges[t].push_back(augEdge<T, C>{t, s, cost});
        }
    }
    augGraph() {}

    vector<augEdge<T, C>> operator[](T key) {
        if (edges.find(key) == edges.end()) {
            return vector<augEdge<T, C>>();
        }
        return edges[key];
    }
};

template <class T, class C>
void augDijkstra(augGraph<T, C> graph, T start, map<T, C> &cost) {
    // vi prev(V);
    pql<pair<C, T>> Q;

    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        T pos = Q.top().second;
        C d = Q.top().first;
        Q.pop();
        rep(i, graph[pos].size()) {
            T to = graph[pos][i].to;
            C c = graph[pos][i].cost;
            if (cost.find(to) == cost.end() || cost[to] > cost[pos] + c) {
                cost[to] = cost[pos] + c;
                Q.push(mp(cost[to], to));
                // prev[graph[pos][i].to] = pos;
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
        G[mp(p, 0)].push_back(augEdge<Pi>{mp(p, 0), mp(p, c), 1});
        G[mp(p, c)].push_back(augEdge<Pi>{mp(p, c), mp(p, 0), 1});
        G[mp(q, 0)].push_back(augEdge<Pi>{mp(q, 0), mp(q, c), 1});
        G[mp(q, c)].push_back(augEdge<Pi>{mp(q, c), mp(q, 0), 1});
        G[mp(p, c)].push_back(augEdge<Pi>{mp(p, c), mp(q, c), 0});
        G[mp(q, c)].push_back(augEdge<Pi>{mp(q, c), mp(p, c), 0});
    }

    map<Pi, int> cost;

    augDijkstra<Pi>(G, mp(1, 0), cost);

    if (cost.find(mp(N, 0)) == cost.end())
        cout << -1 << endl;
    else
        cout << (cost[mp(N, 0)] - 1) / 2 + 1 << endl;

    return 0;
}