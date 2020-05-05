#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/1/GRL_1_A"

#include "../library/graph.hpp"

int main(void) {
    int V, E, r;
    cin >> V >> E >> r;
    Graph<int> G(V, true);
    rep(i, E) {
        int s, t, d;
        cin >> s >> t >> d;
        G.add_edge(s, t, d);
    }
    vector<long long> cost;
    Dijkstra(G, r, cost);
    rep(i, V) {
        if (cost[i] == INFL)
            cout << "INF" << '\n';
        else
            cout << cost[i] << '\n';
    }
}
