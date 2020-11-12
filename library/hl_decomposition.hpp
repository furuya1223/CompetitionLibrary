// cf. https://tubo28.me/compprog/algorithm/hld/
#include "header.hpp"

template <class T>
class HLDecomposition {
    void decompose(const int root) {
        stack<int> stk; // DFS 用スタック
        stk.push(root);
        parent[root] = -1;
        depth[root] = 0;

        // DFS
        while (!stk.empty()) {
            int v = stk.top();
            stk.pop();
            if (v >= 0) {
                // 行きがけに parent, depth を計算
                stk.push(~v); // 帰りがけ用に反転したものを積んでおく
                for (auto &e : g[v]) {
                    int child = e.to;
                    if (depth[child] == -1) {
                        depth[child] = depth[v] + 1;
                        parent[child] = v;
                        stk.push(child);
                    }
                }
            } else {
                // 帰りがけに subsize を計算し、Heavy Edge を決める
                // next は Heavy Edge の子とする
                int m = 0;
                v = ~v;
                subsize[v] = 1;
                for (auto &e : g[v]) {
                    int child = e.to;
                    if (parent[v] == child) continue;
                    subsize[v] += subsize[child];
                    if (m < subsize[child]) {
                        m = subsize[child];
                        next[v] = child;
                    }
                }
            }
        }

        stk.push(root);
        while (!stk.empty()) {
            int h = stk.top();
            stk.pop();
            for (auto &e : g[h]) {
                if (parent[h] != e.to) stk.push(e.to);
            }

            if (chain[h] != -1) continue;
            chains.push_back(vector<int>());
            vector<int> &path = chains.back();

            // Heavy Edge を辿っていき、chains に追加する
            for (int cur = h; cur != -1;) {
                path.push_back(cur);
                cur = next[cur];
            }

            // 今追加した chain の情報を調べて記録する
            for (int i = 0; i < (int)path.size(); i++) {
                int v = path[i];
                head[v] = path.front();
                last[v] = path.back();
                next[v] = i + 1 != (int)path.size() ? path[i + 1] : -1;
                prev[v] = i != 0 ? path[i - 1] : -1;
                chain[v] = chains.size() - 1;
                idx[v] = i;
            }
        }

        int new_index = 0;
        for (auto path : chains) {
            for (auto v : path) {
                encode[v] = new_index;
                decode[new_index] = v;
                new_index++;
            }
        }
    }

  public:
    Graph<T> &g;
    int n;
    vector<vector<int>> chains; // chain の列。head の浅い順
    vector<int> parent;         // 親の頂点番号。根なら -1
    vector<int> subsize;        // 自身を根とする部分木のサイズ
    vector<int> depth;          // 根からの距離
    vector<int> head;           // 自身を含む chain の先頭の頂点
    vector<int> last;           // 自身を含む chain の末尾の頂点
    vector<int> next;           // 自身が属する chain での次の頂点。範囲外なら -1
    vector<int> prev;           // 自身が属する chain での前の頂点。範囲外なら -1
    vector<int> chain;          // 自身が属する chain 番号
    vector<int> idx;            // 自身が属する chain での自身の番号
    vector<int> encode; // 元の頂点番号をパスクエリ用に並べ替えた番号に変える
    vector<int> decode; // パスクエリ用に並べ替えた頂点番号を元の番号に変える

    HLDecomposition(Graph<T> &g_, int r = -1)
        : g(g_), n(g.size()), chains(0), parent(n, 0), subsize(n, 0), depth(n, -1), head(n, 0),
          last(n, 0), next(n, -1), prev(n, -1), chain(n, -1), idx(n, 0), encode(n), decode(n) {
        if (r != -1) decompose(r);
    }

    // 頂点 v が属する chain の親頂点を返す
    // u から根まで登りたいときは
    // for(; u != -1; u = climb(u))
    int climb(int v) const {
        return parent[head[v]];
    }

    // chains[first][second] = v
    std::pair<int, int> get_index(int v) const {
        return std::make_pair(chain[v], idx[v]);
    }

    int size() const {
        return n;
    }

    // encode の順序に並べ替えたものを作成
    template<class U>
    vector<U> permute(vector<U> arr){
        vector<U> result(arr.size());
        for(int i = 0; i < arr.size(); i++){
            result[encode[i]] = arr[i];
        }
        return result;
    }

    int lca(int u, int v) {
        // 同じ chain に属するまで、head が深い側の頂点を登らせる
        while (chain[u] != chain[v]) {
            if (depth[head[u]] > depth[head[v]])
                u = climb(u);
            else
                v = climb(v);
        }
        // 同じ chain に来たら、浅い方が LCA
        return depth[u] < depth[v] ? u : v;
    }

    // 頂点が値を持つパスクエリ用の区間列（閉区間）を encode して返す
    // LCA は result.back()[0]（辺が値を持つ場合は LCA を除く）
    vector<pair<int, int>> queries(int u, int v, bool remove_lca = false) {
        vector<pair<int, int>> result;
        while (chain[u] != chain[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                result.push_back(make_pair(encode[head[u]], encode[u]));
                u = climb(u);
            } else {
                result.push_back(make_pair(encode[head[v]], encode[v]));
                v = climb(v);
            }
        }
        if(remove_lca){
            result.push_back(minmax(encode[u] + 1, encode[v]));
        }else{
            result.push_back(minmax(encode[u], encode[v]));
        }
        return result;
    }
};

// 辺が値を持つ場合に、子側の頂点に値を下ろす
template <class T>
vector<T> down(Graph<T> &G, int root, T def) {
    int n = G.size();
    vector<T> result(n);
    stack<tuple<int, int, T>> stk;
    stk.push(make_tuple(-1, root, def));
    while (!stk.empty()) {
        int parent, idx;
        T val;
        tie(parent, idx, val) = stk.top();
        stk.pop();
        result[idx] = val;
        for (const auto &e : G[idx]) {
            if (e.to == parent) continue;
            stk.push(make_tuple(idx, e.to, e.cost));
        }
    }
    return result;
}