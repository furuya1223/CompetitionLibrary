#include "header.hpp"

void Dijkstra(Graph<int> graph, int start, vll &cost) {
    vi prev(graph.size());
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

// 隣接行列ver
void Dijkstra(vvd graph, int start, vd &cost, int V) {
    vi prev(V);
    priority_queue<pair<double, int>, vector<pair<double, int>>,
                   greater<pair<double, int>>>
        Q;

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

bool BellmanFord(vvP graph, int start, vi &cost, int V) {
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
            // 全辺(i->graph[i][j].first)に対して
            if (visitable[i] &&
                cost[graph[i][j].first] > cost[i] + graph[i][j].second) {
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

// 木の重心
vector<int> Centroid(const Graph &G) {
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

// 最小全域有向木。Edgeは(辺コスト, (始点, 終点))
ll Chi_Liu_Edmonds(vector<Edge> edges, int V, int start) {
    vector<P> mins(V, mp(INF, -1)); // その頂点に入る最小辺の(辺コスト, 始点)
    rep(i, edges.size()) {
        Edge edge = edges[i];
        mins[edge.second.second] =
            min(mins[edge.second.second], mp(edge.first, edge.second.first));
    }
    mins[start] = mp(-1, -1); // 始点には辺が入らない

    vector<int> group(V, 0);
    vector<bool> isCycle(V, false); // 頂点が縮約されたもの（元閉路）かどうか
    int count = 0; // グループ分けで割り振る番号＝縮約後の頂点数

    vector<bool> used(V, false);
    rep(i, V) {
        if (used[i]) continue;
        vector<int> chain;
        int cursor = i;
        while (cursor != -1 && !used[cursor]) {
            // 頂点iから元を辿っていく
            used[cursor] = true;
            chain.push_back(cursor);
            cursor = mins[cursor].second; // 頂点cursorに入ってくる位置へ移動
        }
        if (cursor != -1) {
            // 閉路発見の可能性あり
            // chainの現cursor以降が閉路
            bool inCycle = false; // 閉路に突入したかどうかを表すフラグ
            rep(j, chain.size()) {
                group[chain[j]] = count;
                if (chain[j] == cursor) {
                    // ここから閉路
                    isCycle[count] = true;
                    inCycle = true;
                }
                if (!inCycle) count++; // 閉路内でなければ別グループ扱い
            }
            if (inCycle) count++;
        } else {
            // 根で終了した＝閉路見つからず
            // chain内の頂点すべてを単独のグループに分ける
            rep(j, chain.size()) {
                group[chain[j]] = count;
                count++;
            }
        }
    }
    // この時点でグループ分けが終了している

    if (count == V) {
        // 縮約で頂点数が不変＝閉路が無い
        ll ans = 1; // 根のコスト-1を打ち消すための1
        rep(i, V) {
            ans += mins[i].first;
        }
        return ans;
    }

    // 縮約した閉路のコスト合計を計算
    ll res = 0;
    rep(i, V) {
        if (i != start && isCycle[group[i]]) {
            res += mins[i].first;
        }
    }

    // 閉路が存在するので、新たにグラフを構成
    vector<Edge> newEdges;
    rep(i, edges.size()) {
        Edge edge = edges[i];
        int to = edge.second.second;
        int gfrom = group[edge.second.first];
        int gto = group[edge.second.second];
        if (gfrom == gto) {
            // 始点と終点が同じグループ（同じ閉路）なら、追加せず
            continue;
        } else if (isCycle[gto]) {
            // 終点が縮約後の閉路なら、コストを計算し直して追加
            newEdges.push_back(mp(edge.first - mins[to].first, mp(gfrom, gto)));
        } else {
            // それ以外は、単純に追加
            newEdges.push_back(mp(edge.first, mp(gfrom, gto)));
        }
    }

    return res + Chi_Liu_Edmonds(newEdges, count, group[start]);
}
