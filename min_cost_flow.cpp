#include "header.hpp"

// ダイクストラver. O(FElogV)

class MinCostFlow {
    struct flowEdge {
        int to;
        long long capacity, cost;
        int reverse_edge;
        flowEdge(int t, long long cap, long long cos, int rev)
            : to(t), capacity(cap), cost(cos), reverse_edge(rev) {}
    };
    class flowGraph {
        vector<vector<flowEdge>> edges; // グラフの隣接リスト表現
      public:
        flowGraph(int n_) : edges(n_) {}
        flowGraph() {}
        void add_edge(int s, int t, long long cap, long long cos) {
            edges[s].emplace_back(t, cap, cos, (int)edges[t].size());
            edges[t].emplace_back(s, 0, -cos, (int)edges[s].size() - 1);
        }
        vector<flowEdge> &operator[](size_t i) {
            return edges[i];
        }
        int size() const {
            return edges.size();
        }
    };

    flowGraph graph;
    vector<long long> potential, distance;
    vector<int> prev_v, prev_e;

  public:
    MinCostFlow(int n)
        : graph(n), potential(n), distance(n), prev_v(n), prev_e(n) {}

    // s から t へ容量 cap の辺を追加
    void add_edge(int s, int t, long long cap, long long cos) {
        graph.add_edge(s, t, cap, cos);
    }

    // s から t への流量 f の最小費用流を求める。不可なら-1
    int min_cost_flow(int s, int t, int f) {
        int ans = 0;
        fill(potential.begin(), potential.end(), 0);
        while (f > 0) {
            // ダイクストラ法を用いて最短経路を計算
            using P_LL_I = pair<long long, int>;
            priority_queue<P_LL_I, vector<P_LL_I>, greater<P_LL_I>> que;
            fill(distance.begin(), distance.end(), INFL);
            distance[s] = 0;
            que.push(mp(0, s));
            while (!que.empty()) {
                long long dist;
                int v;
                tie(dist, v) = que.top();
                que.pop();
                if (distance[v] < dist) continue;
                for (int i = 0; i < graph[v].size(); i++) {
                    flowEdge &e = graph[v][i];
                    if (e.capacity > 0 &&
                        distance[e.to] > distance[v] + e.cost + potential[v] -
                                             potential[e.to]) {
                        distance[e.to] = distance[v] + e.cost + potential[v] -
                                         potential[e.to];
                        prev_v[e.to] = v;
                        prev_e[e.to] = i;
                        que.push(mp(distance[e.to], e.to));
                    }
                }
            }
            if (distance[t] == INFL) {
                // これ以上流せない
                return -1;
            }
            // potential を更新
            for (int v = 0; v < graph.size(); v++) potential[v] += distance[v];

            // s-t 最短経路のボトルネック流量の計算
            long long bottleneck = f;
            for (int v = t; v != s; v = prev_v[v]) {
                bottleneck =
                    min(bottleneck, graph[prev_v[v]][prev_e[v]].capacity);
            }
            // s-t 最短経路に沿ってボトルネック流量を流す
            f -= bottleneck;
            ans += bottleneck * potential[t];
            for (int v = t; v != s; v = prev_v[v]) {
                flowEdge &e = graph[prev_v[v]][prev_e[v]];
                e.capacity -= bottleneck;
                graph[v][e.reverse_edge].capacity += bottleneck;
            }
        }
        return ans;
    }
};