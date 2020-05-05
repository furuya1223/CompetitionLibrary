#include "header.hpp"

vector<int> Centroid(const Graph<int> &G) {
    int n = G.size();
    vector<int> centroid;
    vector<int> sz(n);
    function<void(int, int)> dfs = [&](int u, int prev) {
        sz[u] = 1;
        bool is_centroid = true;
        for (auto v : G[u])
            if (v.to != prev) {
                dfs(v.to, u);
                sz[u] += sz[v.to];
                if (sz[v.to] > n / 2) is_centroid = false;
            }
        if (n - sz[u] > n / 2) is_centroid = false;
        if (is_centroid) centroid.push_back(u);
    };
    dfs(0, -1);
    return centroid;
}

ll Chi_Liu_Edmonds(vector<Edge<int>> edges, int V, int start) {
    vector<Pi> mins(V, mp(INF, -1));
    rep(i, edges.size()) {
        Edge<int> edge = edges[i];
        mins[edge.to] = min(mins[edge.to], mp(edge.cost, edge.from));
    }
    mins[start] = mp(-1, -1);

    vector<int> group(V, 0);
    vector<bool> isCycle(V, false);
    int count = 0;

    vector<bool> used(V, false);
    rep(i, V) {
        if (used[i]) continue;
        vector<int> chain;
        int cursor = i;
        while (cursor != -1 && !used[cursor]) {
            used[cursor] = true;
            chain.push_back(cursor);
            cursor = mins[cursor].second;
        }
        if (cursor != -1) {
            bool inCycle = false;
            rep(j, chain.size()) {
                group[chain[j]] = count;
                if (chain[j] == cursor) {
                    isCycle[count] = true;
                    inCycle = true;
                }
                if (!inCycle) count++;
            }
            if (inCycle) count++;
        } else {
            rep(j, chain.size()) {
                group[chain[j]] = count;
                count++;
            }
        }
    }

    if (count == V) {
        ll ans = 1;
        rep(i, V) {
            ans += mins[i].first;
        }
        return ans;
    }

    ll res = 0;
    rep(i, V) {
        if (i != start && isCycle[group[i]]) {
            res += mins[i].first;
        }
    }

    vector<Edge<int>> newEdges;
    rep(i, edges.size()) {
        Edge<int> edge = edges[i];
        int to = edge.to;
        int gfrom = group[edge.from];
        int gto = group[edge.to];
        if (gfrom == gto) {
            continue;
        } else if (isCycle[gto]) {
            newEdges.push_back(Edge<int>(gfrom, gto, edge.cost - mins[to].first));
        } else {
            newEdges.push_back(Edge<int>(gfrom, gto, edge.cost));
        }
    }

    return res + Chi_Liu_Edmonds(newEdges, count, group[start]);
}
