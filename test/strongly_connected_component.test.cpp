#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include "../library/strongly_connected_component.hpp"

int main() {
    int N, M;
    cin >> N >> M;
    Graph<int> G(N, true);
    rep(i, M) {
        int a, b;
        cin >> a >> b;
        G.add_edge(a, b, 1);
    }

    SCCD sccd;
    vvi scc;
    vi n2g(N);
    Graph<int> dag;
    sccd.SCC(G, scc, n2g, dag);

    cout << scc.size() << endl;
    rep(i, scc.size()) {
        cout << scc[i].size();
        rep(j, scc[i].size()) {
            cout << " " << scc[i][j];
        }
        cout << endl;
    }

    return 0;
}