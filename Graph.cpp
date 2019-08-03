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

// �אڍs��ver
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
            // �S��(i->graph[i][j].first)�ɑ΂���
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

// �؂̏d�S
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

// �ŏ��S��L���؁BEdge��(�ӃR�X�g, (�n�_, �I�_))
ll Chi_Liu_Edmonds(vector<Edge> edges, int V, int start) {
    vector<P> mins(V, mp(INF, -1)); // ���̒��_�ɓ���ŏ��ӂ�(�ӃR�X�g, �n�_)
    rep(i, edges.size()) {
        Edge edge = edges[i];
        mins[edge.second.second] =
            min(mins[edge.second.second], mp(edge.first, edge.second.first));
    }
    mins[start] = mp(-1, -1); // �n�_�ɂ͕ӂ�����Ȃ�

    vector<int> group(V, 0);
    vector<bool> isCycle(V, false); // ���_���k�񂳂ꂽ���́i���H�j���ǂ���
    int count = 0; // �O���[�v�����Ŋ���U��ԍ����k���̒��_��

    vector<bool> used(V, false);
    rep(i, V) {
        if (used[i]) continue;
        vector<int> chain;
        int cursor = i;
        while (cursor != -1 && !used[cursor]) {
            // ���_i���猳��H���Ă���
            used[cursor] = true;
            chain.push_back(cursor);
            cursor = mins[cursor].second; // ���_cursor�ɓ����Ă���ʒu�ֈړ�
        }
        if (cursor != -1) {
            // �H�����̉\������
            // chain�̌�cursor�ȍ~���H
            bool inCycle = false; // �H�ɓ˓��������ǂ�����\���t���O
            rep(j, chain.size()) {
                group[chain[j]] = count;
                if (chain[j] == cursor) {
                    // ��������H
                    isCycle[count] = true;
                    inCycle = true;
                }
                if (!inCycle) count++; // �H���łȂ���ΕʃO���[�v����
            }
            if (inCycle) count++;
        } else {
            // ���ŏI���������H�����炸
            // chain���̒��_���ׂĂ�P�Ƃ̃O���[�v�ɕ�����
            rep(j, chain.size()) {
                group[chain[j]] = count;
                count++;
            }
        }
    }
    // ���̎��_�ŃO���[�v�������I�����Ă���

    if (count == V) {
        // �k��Œ��_�����s�ρ��H������
        ll ans = 1; // ���̃R�X�g-1��ł��������߂�1
        rep(i, V) {
            ans += mins[i].first;
        }
        return ans;
    }

    // �k�񂵂��H�̃R�X�g���v���v�Z
    ll res = 0;
    rep(i, V) {
        if (i != start && isCycle[group[i]]) {
            res += mins[i].first;
        }
    }

    // �H�����݂���̂ŁA�V���ɃO���t���\��
    vector<Edge> newEdges;
    rep(i, edges.size()) {
        Edge edge = edges[i];
        int to = edge.second.second;
        int gfrom = group[edge.second.first];
        int gto = group[edge.second.second];
        if (gfrom == gto) {
            // �n�_�ƏI�_�������O���[�v�i�����H�j�Ȃ�A�ǉ�����
            continue;
        } else if (isCycle[gto]) {
            // �I�_���k���̕H�Ȃ�A�R�X�g���v�Z�������Ēǉ�
            newEdges.push_back(mp(edge.first - mins[to].first, mp(gfrom, gto)));
        } else {
            // ����ȊO�́A�P���ɒǉ�
            newEdges.push_back(mp(edge.first, mp(gfrom, gto)));
        }
    }

    return res + Chi_Liu_Edmonds(newEdges, count, group[start]);
}
