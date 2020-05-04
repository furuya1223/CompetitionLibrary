#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_A"

#include "../library/segment_tree.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    int def = 2147483647; // 2**31-1
    SegTree<int> segment_tree(
        n, def, [](int a, int b) { return min(a, b); }, [](int a, int b) { return b; });
    rep(i, q) {
        int com, x, y;
        cin >> com >> x >> y;
        if (com == 0) {
            // update(x, y)
            segment_tree.change(x, y);
        } else {
            // find(x, y)
            cout << segment_tree.query(x, y + 1) << '\n';
        }
    }
}