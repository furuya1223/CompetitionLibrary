#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_B"

#include "../library/min_cost_flow.hpp"

int main(void) {
    int V, E, F;
    cin >> V >> E >> F;
    MinCostFlow mcf(V);
    rep(i, E) {
        int u, v, c, d;
        cin >> u >> v >> c >> d;
        mcf.add_edge(u, v, c, d);
    }
    cout << mcf.min_cost_flow(0, V - 1, F) << endl;
}