#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_D"

#include "../library/hl_decomposition.hpp"
#include "../library/segment_tree.hpp"

int main() {
    int n;
    cin >> n;
    Graph<int> G(n, false);
    rep(i, n) {
        int k;
        cin >> k;
        rep(j, k) {
            int c;
            cin >> c;
            G.add_edge(i, c, 0);
        }
    }
    HLDecomposition<int> hld(G, 0);
    SegTree<long long> st(
        n, 0, [](long long a, long long b) { return a + b; },
        [](long long a, long long b) { return a + b; });
    int q;
    cin >> q;
    rep(i, q) {
        int kind;
        cin >> kind;
        if (kind == 0) {
            // add
            int v, w;
            cin >> v >> w;
            st.change(hld.encode[v], w);
        } else {
            // getSum
            int u;
            cin >> u;
            vector<pair<int, int>> queries = hld.queries(0, u);
            long long ans = 0;
            rep(i, queries.size()) {
                int l, r;
                tie(l, r) = queries[i];
                r++;
                if (i == queries.size() - 1) l++;
                ans += st.query(l, r);
            }
            cout << ans << '\n';
        }
    }

    return 0;
}