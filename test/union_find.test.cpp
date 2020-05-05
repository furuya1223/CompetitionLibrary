#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/1/DSL_1_A"

#include "../library/union_find.hpp"

int main(void) {
    int n, q;
    cin >> n >> q;
    UnionFind uf(n);
    rep(i, q) {
        int com, x, y;
        cin >> com >> x >> y;
        if (com == 0) {
            // unite
            uf.unite(x, y);
        } else {
            // find
            cout << (uf.same(x, y) ? 1 : 0) << '\n';
        }
    }
}
