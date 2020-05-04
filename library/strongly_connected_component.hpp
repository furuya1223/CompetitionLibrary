#include "header.hpp"

class SCCD {
  public:
    stack<int> post;
    vb used;

    inline void dfs(int pos, int par, Graph<int> &g) {
        used[pos] = true;
        for (const auto &e : g[pos]) {
            if (used[e.to]) continue;
            dfs(e.to, pos, g);
        }
        post.push(pos);
    }

    inline void dfsrev(int pos, int par, Graph<int> &rev, vi &group) {
        used[pos] = true;
        group.push_back(pos);
        for (const auto &e : rev[pos]) {
            if (used[e.to]) continue;
            dfsrev(e.to, pos, rev, group);
        }
    }

    Graph<int> SCC(Graph<int> &G, vvi &scc, vi &n2g, Graph<int> &dag) {
        int n = G.size();
        Graph<int> rev(n, true);
        // 逆辺グラフの作成
        rep(i, n) {
            rep(j, G[i].size()) {
                auto e = G[i][j];
                rev.add_edge(e.to, e.from, 1);
            }
        }

        // 1 回目の DFS
        used = vb(n, false);
        rep(i, n) {
            if (!used[i]) dfs(i, -1, G);
        }

        // 2 回目の DFS
        fill(all(used), false);
        while (!post.empty()) {
            int pos = post.top();
            post.pop();
            if (used[pos]) continue;
            vi group;
            dfsrev(pos, -1, rev, group);
            scc.push_back(group);
        }

        // 頂点番号から強連結成分の番号を得る辞書の作成
        rep(i, scc.size()) {
            rep(j, scc[i].size()) {
                n2g[scc[i][j]] = i;
            }
        }

        dag = Graph<int>(scc.size(), true);
        set<pair<int, int>> usedEdges;

        // DAG を作成
        rep(i, n) {
            for (const auto &e : G[i]) {
                int s = n2g[e.from];
                int t = n2g[e.to];
                if (usedEdges.find(mp(s, t)) == usedEdges.end() && s != t) {
                    dag.add_edge(s, t, 1);
                    usedEdges.insert(mp(s, t));
                }
            }
        }
        return dag;
    }
};

class SAT {
    bool solved = false;

  public:
    int num;
    Graph<int> graph, dag;
    vvi scc;
    vi n2g;

    SAT(int n) : num(n), graph(2 * n, true), n2g(2 * num) {}

    void add_constraint(bool x_is_true, int x, bool y_is_true, int y) {
        assert(0 <= x && x < num && 0 <= y && y < num);
        if (!x_is_true) x += num;
        if (!y_is_true) y += num;
        graph.add_edge((x + num) % (2 * num), y, 1); // not x -> y
        graph.add_edge((y + num) % (2 * num), x, 1); // not y -> x
    }

    void solve() {
        SCCD sccd;
        sccd.SCC(graph, scc, n2g, dag);
        solved = true;
    }

    bool get_satisfiability() {
        if (!solved) solve();
        for (int i = 0; i < num; i++) {
            if (n2g[i] == n2g[i + num]) return false;
        }
        return true;
    }

    bool get_assignment(vector<bool> &assignment) {
        assignment.resize(num);
        if (!solved) solve();
        vector<bool> ret(num);
        for (int i = 0; i < num; i++) {
            if (n2g[i] == n2g[i + num]) return false;
            assignment[i] = n2g[i] > n2g[i + num];
        }
        return true;
    }
};