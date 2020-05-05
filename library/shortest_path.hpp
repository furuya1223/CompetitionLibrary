#include "header.hpp"

void Dijkstra(Graph<int> &graph, int start, vector<long long> &cost) {
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