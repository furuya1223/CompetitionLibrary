#include "header.hpp"

// Dinic 法による最大フロー O(V^2 * E) だが高速
class MaxFlow {
    struct flowEdge {
        int to;             // 行き先
        long long capacity; // 容量
        int reverse_edge;   // 逆辺の index
        flowEdge(int t, long long c, int r)
            : to(t), capacity(c), reverse_edge(r) {}
    };
    class flowGraph {
        vector<vector<flowEdge>> edges; // グラフの隣接リスト表現
      public:
        flowGraph(int n_) : edges(n_) {}
        flowGraph() {}
        void add_edge(int s, int t, long long cap) {
            edges[s].emplace_back(t, cap, (int)edges[t].size());
            edges[t].emplace_back(s, 0, (int)edges[s].size() - 1);
        }
        vector<flowEdge> &operator[](size_t i) {
            return edges[i];
        }
        int size() const {
            return edges.size();
        }
    };

    flowGraph graph;
    vector<int> level;
    vector<int> iter;

    // sからの最短距離をBFSで計算
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const auto &e : graph[v]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // 増加パスをDFSで探す（v に着目、終点は t, 現在の最小流量は f）
    long long dfs(int v, int t, long long f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < graph[v].size(); i++) {
            flowEdge &e = graph[v][i];
            if (e.capacity > 0 && level[v] < level[e.to]) {
                long long d = dfs(e.to, t, min(f, e.capacity));
                if (d > 0) {
                    e.capacity -= d;
                    graph[e.to][e.reverse_edge].capacity += d;
                    return d;
                }
            }
        }
        return 0;
    }

  public:
    MaxFlow(int n) : graph(n), level(n), iter(n) {}

    // s から t へ容量 cap の辺を追加
    void add_edge(int s, int t, long long cap) {
        graph.add_edge(s, t, cap);
    }

    // sからtへの最大流を求める
    long long max_flow(int s, int t) {
        long long flow = 0;
        while (true) {
            bfs(s);
            // t へ到達不可能になったら出力
            if (level[t] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            long long increase;
            while ((increase = dfs(s, t, INFL)) > 0) {
                flow += increase;
            }
        }
    }
};

class ProjectSelection {
    int num;
    MaxFlow max_flow; // 頂点数は num+2, 始点 num, 終点 num+1
    long long sum_reward = 0;

  public:
    ProjectSelection(vector<long long> rewards)
        : num(rewards.size()), max_flow(rewards.size() + 2) {
        for (int i = 0; i < num; i++) {
            if (rewards[i] > 0) {
                max_flow.add_edge(num, i, rewards[i]);
                sum_reward += rewards[i];
            } else {
                max_flow.add_edge(i, num + 1, -rewards[i]);
            }
        }
    }

    // i を選択し、j を選択しないと penalty だけ損する
    void add_penalty(int i, int j, long long penalty) {
        max_flow.add_edge(i, j, penalty);
    }

    // 最大の利得を計算
    long long max_reward() {
        return sum_reward - max_flow.max_flow(num, num + 1);
    }
};