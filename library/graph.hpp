#include "header.hpp"

void Dijkstra(const Graph<int> &graph, int start, vector<long long> &cost) {
    cost.resize(graph.size());
    fill(cost.begin(), cost.end(), INFL);
    vector<int> prev(graph.size());
    pql<Pll> Q;

    fill(cost.begin(), cost.end(), INFL);
    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        int pos = Q.top().second;
        ll d = Q.top().first;
        Q.pop();
        rep(i, graph[pos].size()) {
            if (cost[graph[pos][i].to] > cost[pos] + graph[pos][i].cost) {
                cost[graph[pos][i].to] = cost[pos] + graph[pos][i].cost;
                Q.push(mp(cost[graph[pos][i].to], graph[pos][i].to));
                prev[graph[pos][i].to] = pos;
            }
        }
    }
}

void Dijkstra(vvd graph, int start, vd &cost, int V) {
    vi prev(V);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> Q;

    fill(cost.begin(), cost.end(), INF);
    cost[start] = 0;

    Q.push(mp(0, start)); // (cost, index)

    while (!Q.empty()) {
        pair<double, int> _pos = Q.top();
        double d = _pos.first;
        int pos = _pos.second;
        Q.pop();
        rep(i, V) {
            if (i == pos) continue;
            if (cost[i] > cost[pos] + graph[pos][i]) {
                cost[i] = cost[pos] + graph[pos][i];
                Q.push(mp(cost[i], i));
            }
        }
    }
}

bool BellmanFord(vvPi graph, int start, vi &cost, int V) {
    vi prev(V);
    vb visitable(V, false);
    int k = 1;
    int visitable_num = 0;

    stack<int> st;
    st.push(start);
    while (!st.empty()) {
        int pos = st.top();
        st.pop();
        visitable[pos] = true;
        rep(i, graph[pos].size()) {
            if (!visitable[graph[pos][i].first]) {
                st.push(graph[pos][i].first);
            }
        }
    }

    rep(i, V) {
        if (visitable[i]) visitable_num++;
    }

    fill(cost.begin(), cost.end(), INF);
    cost[start] = 0;

    while (true) {
        bool flag = true;
        rep(i, V) rep(j, graph[i].size()) {
            if (visitable[i] && cost[graph[i][j].first] > cost[i] + graph[i][j].second) {
                cost[graph[i][j].first] = cost[i] + graph[i][j].second;
                prev[graph[i][j].first] = i;
                flag = false;
                break;
            }
        }
        if (flag) break;
        if (k == visitable_num) return false;
        k++;
    }
    return true;
}

bool WarshallFloyd(vvll &graph, int V) {
    rep(k, V) rep(i, V) rep(j, V) {
        // i->(k)->j
        if (graph[i][k] != INFL && graph[k][j] != INFL) {
            graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
        }
    }
    rep(i, V) {
        if (graph[i][i] < 0) return false;
    }
    return true;
}

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
