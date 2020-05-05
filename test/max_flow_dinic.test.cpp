#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A"

#include "../library/max_flow.hpp"

int main(void) {
    int V, E;
    cin >> V >> E;
    MaxFlow mf(V);
    rep(i, E) {
        int u, v, c;
        cin >> u >> v >> c;
        mf.add_edge(u, v, c);
    }
    cout << mf.max_flow(0, V - 1) << endl;
}
