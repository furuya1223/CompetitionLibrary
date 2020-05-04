#include "header.hpp"

class UnionFind {
  public:
    vector<int> par, rank, size_;
    int num_;

    UnionFind(int n) {
        par = vector<int>(n);
        rank = vector<int>(n);
        size_ = vector<int>(n);
        num_ = n;
        for (int i = 0; i < n; i++) {
            par[i] = i;
            rank[i] = 0;
            size_[i] = 1;
        }
    }

    int find(int x) {
        if (par[x] == x) {
            return x;
        } else {
            return par[x] = find(par[x]);
        }
    }

    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;

        num_--;
        if (rank[x] < rank[y]) {
            par[x] = y;
            size_[y] += size_[x];
        } else {
            par[y] = x;
            size_[x] += size_[y];
            if (rank[x] == rank[y]) rank[x]++;
        }
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int size(int x) {
        return size_[find(x)];
    }

    int num() {
        return num_;
    }
};
