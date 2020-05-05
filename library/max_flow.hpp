#include "header.hpp"

class MaxFlow {
    struct FlowEdge {
        int to;
        ll cap;
        int rev;
    };
    vector<vector<FlowEdge>> flow_graph;
    vector<long long> level; // Distance from source
    vector<int> iter;        // How far have you finished looking

    // Calculate distance from source to other vertices
    void bfs(int source) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[source] = 0;
        que.push(source);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (int i = 0; i < flow_graph[v].size(); i++) {
                FlowEdge &e = flow_graph[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // Search increasing path with DFS
    long long dfs(int v, int target, long long f) {
        if (v == target) return f;
        for (int &i = iter[v]; i < flow_graph[v].size(); i++) {
            FlowEdge &e = flow_graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, target, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    flow_graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

  public:
    MaxFlow(int n) : flow_graph(n), level(n), iter(n) {}

    void add_edge(int from, int to, ll cap) {
        flow_graph[from].push_back(FlowEdge{to, cap, (int)flow_graph[to].size()});
        flow_graph[to].push_back(FlowEdge{from, 0, (int)flow_graph[from].size() - 1});
    }

    // calculate max flow from s to target
    ll max_flow(int source, int target) {
        ll flow = 0;
        while (true) {
            bfs(source);
            if (level[target] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            long long f;
            while ((f = dfs(source, target, INFL)) > 0) {
                flow += f;
            }
        }
    }
};

class ProjectSelection {
    int num;
    MaxFlow max_flow; // |V| = num+2, source = num, target = num+1
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

    // if choose i and not choose j, get penalty
    void add_penalty(int i, int j, long long penalty) {
        max_flow.add_edge(i, j, penalty);
    }

    long long max_reward() {
        return sum_reward - max_flow.max_flow(num, num + 1);
    }
};