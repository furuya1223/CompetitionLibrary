#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_B"

#include "../library/segment_tree.hpp"

int main() {
    int n, q;
    cin >> n >> q;
    int def = 0;
    SegTree<int> segment_tree(
        n, def, [](int a, int b) { return a + b; }, [](int a, int b) { return a + b; });
    rep(i, q) {
        int com, x, y;
        cin >> com >> x >> y;
        if (com == 0) {
            // add(x, y)
            x--;
            segment_tree.change(x, y);
        } else {
            // getSum(x, y)
            x--, y--;
            cout << segment_tree.query(x, y + 1) << '\n';
        }
    }
}