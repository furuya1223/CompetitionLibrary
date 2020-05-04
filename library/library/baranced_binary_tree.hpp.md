---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :warning: library/baranced_binary_tree.hpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#d521f765a49c72507257a2620612ee96">library</a>
* <a href="{{ site.github.repository_url }}/blob/master/library/baranced_binary_tree.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-04 16:27:14+09:00




## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include <iostream>

// insert/delete赤黒木による平衡二分探索木(set仕様)
// Kはcomparableであることが必要
template <class K>
class BST {
    // 色クラス（赤か黒）
    enum class Color { Red, Black, Error };
    // 頂点
    class Node {
      public:
        K key;
        Color color = Color::Red;
        int size = 1;
        Node *left = nullptr,
             *right = nullptr; // 子。leftは自身よりも小さい値のNode
        Node(K key) : key(key) {}
    };

    Node *root;
    bool change = false; // 変更が必要か否か

    bool isRed(Node *node) {
        return node != nullptr && node->color == Color::Red;
    }
    bool isBlack(Node *node) {
        return node != nullptr && node->color == Color::Black;
    }

    // nodeを根とする部分木のサイズを取得
    int getSize(Node *node) {
        if (node == nullptr)
            return 0;
        else
            return node->size;
    }

    // サイズ再計算する
    void resetSize(Node *node) {
        if (node != nullptr) {
            node->size = getSize(node->left) + getSize(node->right) + 1;
        }
    }

    // 左回転 回転した部分木の根を返す
    Node *rotateL(Node *v) {
        Node *u = v->right, *t2 = u->left;
        int size1 = getSize(v->left);
        int size2 = getSize(t2);
        int size3 = getSize(u->right);
        u->left = v;
        u->size = size1 + size2 + size3 + 2;
        v->right = t2;
        v->size = size1 + size2 + 1;
        return u;
    }

    // 右回転 回転した部分木の根を返す
    Node *rotateR(Node *u) {
        Node *v = u->left, *t2 = v->right;
        int size1 = getSize(v->left);
        int size2 = getSize(t2);
        int size3 = getSize(u->right);
        v->right = u;
        v->size = size1 + size2 + size3 + 2;
        u->left = t2;
        u->size = size2 + size3 + 1;
        return v;
    }

    // 左回転->右回転
    Node *rotateLR(Node *u) {
        u->left = rotateL(u->left);
        return rotateR(u);
    }

    // 右回転->左回転
    Node *rotateRL(Node *u) {
        u->right = rotateR(u->right);
        return rotateL(u);
    }

    // 部分木K以下にkeyを挿入。挿入後の根ノードのポインタを返す
    Node *insert(Node *t, K key) {
        if (t == nullptr) {
            // ここに挿入
            change = true;
            return new Node(key);
        } else if (key < t->key) {
            t->left = insert(t->left, key);
            return balance(t);
        } else if (key > t->key) {
            t->right = insert(t->right, key);
            return balance(t);
        } else {
            // キーがすでに存在
            change = false;
            resetSize(t);
            return t;
        }
    }

    // 挿入に伴うバランス バランス後の部分木の根を返す
    Node *balance(Node *t) {
        // tの子と孫で赤が連続している場合に修正
        if (!change || !isBlack(t)) {
            // 変更不要状態か、tが赤orNULLなら何もしない
        } else if (isRed(t->left) && isRed(t->left->left)) {
            t = rotateR(t);
            t->left->color = Color::Black;
        } else if (isRed(t->left) && isRed(t->left->right)) {
            t = rotateLR(t);
            t->left->color = Color::Black;
        } else if (isRed(t->right) && isRed(t->right->left)) {
            t = rotateRL(t);
            t->right->color = Color::Black;
        } else if (isRed(t->right) && isRed(t->right->right)) {
            t = rotateL(t);
            t->right->color = Color::Black;
        } else {
            change = false;
        }
        resetSize(t);
        return t;
    }

    Node *drop(Node *t, K key) {
        if (t == nullptr) {
            change = false;
            return nullptr;
        } else if (key < t->key) {
            t->left = drop(t->left, key);
            return balanceL(t);
        } else if (key > t->key) {
            t->right = drop(t->right, key);
            return balanceR(t);
        } else {
            // ノードtを削除する
            if (t->left == nullptr) {
                switch (t->color) {
                case Color::Red:
                    change = false;
                    break;
                case Color::Black:
                    change = true;
                    break;
                }
                auto ret = t->right;
                delete t;
                return ret; // 右部分木を昇格させる
            } else {
                auto result = deleteMax(t->left);
                t->left = result.first; // 左部分木の最大値を削除する
                t->key = result.second; // 左部分木の削除した最大値で置き換える
                return balanceL(t);
            }
        }
    }

    // 部分木 t の最大値のノードを削除する
    // 戻り値は削除により修正された部分木
    // 削除した最大値を lmax に保存する
    std::pair<Node *, K> deleteMax(Node *t) {
        if (t->right != nullptr) {
            auto result = deleteMax(t->right);
            t->right = result.first;
            resetSize(t);
            return std::make_pair(balanceR(t), result.second);
        } else {
            // ここが最大キー
            switch (t->color) {
            case Color::Red:
                change = false;
                break;
            case Color::Black:
                change = true;
                break;
            }
            auto ret = std::make_pair(t->left, t->key);
            delete t;
            return ret; // 左部分木を昇格させる
        }
    }

    // 左部分木のノード削除に伴う赤黒木の修正(パターンマッチ)
    // 戻り値は修正された木
    Node *balanceL(Node *t) {
        if (!change) {
            // 修正なしと赤ノード削除の場合は何もしない
        } else if (isBlack(t->right) && isRed(t->right->left)) {
            Color rb = t->color;
            t = rotateRL(t);
            t->color = rb;
            t->left->color = Color::Black;
            change = false;
        } else if (isBlack(t->right) && isRed(t->right->right)) {
            Color rb = t->color;
            t = rotateL(t);
            t->color = rb;
            t->left->color = Color::Black;
            t->right->color = Color::Black;
            change = false;
        } else if (isBlack(t->right)) {
            Color rb = t->color;
            t->color = Color::Black;
            t->right->color = Color::Red;
            change = (rb == Color::Black);
        } else if (isRed(t->right)) {
            t = rotateL(t);
            t->color = Color::Black;
            t->left->color = Color::Red;
            t->left = balanceL(t->left);
            change = false;
        } else { // 黒ノード削除の場合、ここはありえない
            std::cout << __FUNCTION__ << "(" << __LINE__ << "): Error";
        }
        resetSize(t);
        return t;
    }

    // 右部分木のノード削除に伴う赤黒木の修正(パターンマッチ)
    // 戻り値は修正された木
    Node *balanceR(Node *t) {
        if (!change) {
            // 修正なしと赤ノード削除の場合は何もしない
        } else if (isBlack(t->left) && isRed(t->left->right)) {
            Color rb = t->color;
            t = rotateLR(t);
            t->color = rb;
            t->right->color = Color::Black;
            change = false;
        } else if (isBlack(t->left) && isRed(t->left->left)) {
            Color rb = t->color;
            t = rotateR(t);
            t->color = rb;
            t->left->color = Color::Black;
            t->right->color = Color::Black;
            change = false;
        } else if (isBlack(t->left)) {
            Color rb = t->color;
            t->color = Color::Black;
            t->left->color = Color::Red;
            change = (rb == Color::Black);
        } else if (isRed(t->left)) {
            t = rotateR(t);
            t->color = Color::Black;
            t->right->color = Color::Red;
            t->right = balanceR(t->right);
            change = false;
        } else { // 黒ノード削除の場合、ここはありえない
            std::cout << __FUNCTION__ << "(" << __LINE__ << "): Error";
        }
        resetSize(t);
        return t;
    }

    // デバッグ用・赤連続禁止のチェック
    Color colorChain(Node *t) {
        if (t == nullptr) return Color::Black;
        Color p = t->color;
        Color color_l = colorChain(t->left);
        Color color_r = colorChain(t->right);
        if (color_l == Color::Error || color_r == Color::Error) {
            return Color::Error;
        }
        if (p == Color::Black) {
            return p;
        }
        if (p == Color::Red && color_l == Color::Black &&
            color_r == Color::Black) {
            return p;
        }
        return Color::Error;
    }

    // デバッグ用・黒の数チェック
    int blackHeight(Node *t) {
        if (t == nullptr) return 0;
        int height_l = blackHeight(t->left);
        int height_r = blackHeight(t->right);
        if (height_l < 0 || height_r < 0 || height_l != height_r) return -1;
        return height_l + (t->color == Color::Black ? 1 : 0);
    }

  public:
    BST() {
        root = nullptr;
    }

    // 空判定
    bool isEmpty() {
        return root == nullptr;
    }

    // 検索してNodeのポインタを返す。見つからなければnullptr
    Node *find(K key) {
        Node *pos = root;
        while (pos != nullptr) {
            if (key == pos->key) {
                return pos;
            } else if (key < pos->key) {
                pos = pos->left;
            } else {
                pos = pos->right;
            }
        }
        return pos;
    }

    bool exist(K key) {
        return find(key) != nullptr;
    }

    // 挿入のエントリポイント
    void insert(K key) {
        root = insert(root, key);
        root->color = Color::Black;
    }

    // 削除のエントリポイント
    void drop(K key) {
        root = drop(root, key);
        if (root != nullptr) root->color = Color::Black;
    }

    // 赤連続禁止チェック
    bool redValidation() {
        return colorChain(root) == Color::Black;
    }

    // 黒の数チェック
    bool blackValidation() {
        return blackHeight(root) >= 0;
    }

    // 両方のバリデーション
    void debug_validation() {
        if (!redValidation()) {
            std::cout << "Continuous Red Error!" << std::endl;
        } else if (!blackValidation()) {
            std::cout << "Black Heights Differ Error!" << std::endl;
        } else {
            std::cout << "Valid!" << std::endl;
        }
    }

    // デバッグ用出力
    void debug_show(Node *pos = nullptr) {
        if (pos == nullptr) {
            // 根
            std::cout << "show: ";
            if (root == nullptr) {
                // 空
                std::cout << "Empty" << std::endl;
            } else {
                debug_show(root);
                std::cout << std::endl;
            }
        } else {
            if (pos->left != nullptr) {
                std::cout << "(";
                debug_show(pos->left);
                std::cout << ")";
            }
            std::cout << pos->key << (pos->color == Color::Red ? "R" : "B")
                      << getSize(pos);
            if (pos->right != nullptr) {
                std::cout << "(";
                debug_show(pos->right);
                std::cout << ")";
            }
        }
    }

    // デバッグ用存在確認
    void debug_exist(K key) {
        if (exist(key)) {
            std::cout << key << " is exist" << std::endl;
        } else {
            std::cout << key << " is not exist" << std::endl;
        }
    }

    // 木のサイズ（要素数）を取得
    int size() {
        return getSize(root);
    }

    // k番目のキーを返す errはエラー時の返り値
    K kthKey(int k, K err) {
        return kthKey(root, k, err);
    }
    K kthKey(Node *t, int k, K err) {
        if (t == nullptr) {
            return err;
        }
        if (getSize(t->left) >= k) {
            return kthKey(t->left, k, err);
        } else if (getSize(t->left) == k - 1) {
            return t->key;
        } else {
            return kthKey(t->right, k - getSize(t->left) - 1, err);
        }
    }
};

// AtCoder Regular Contest 033 C 「データ構造」 を解く関数
// 問題URL: https://arc033.contest.atcoder.jp/tasks/arc033_3
// 提出URL: https://arc033.contest.atcoder.jp/submissions/2622699
void solveARC033C() {
    BST<int> tree;

    int Q;
    std::cin >> Q;
    for (size_t i = 0; i < Q; i++) {
        int t, x;
        std::cin >> t >> x;
        if (t == 1) {
            tree.insert(x);
            // tree.debug_show();
        } else {
            int n = tree.kthKey(x, -1);
            std::cout << n << std::endl;
            tree.drop(n);
            // tree.debug_show();
        }
    }
}

int main() {
    BST<int> tree;
    tree.insert(10);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(9);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(8);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(6);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(7);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(15);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(16);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(17);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(18);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(19);
    tree.debug_show();
    tree.debug_validation();

    tree.drop(10);
    tree.debug_show();
    tree.debug_validation();

    tree.drop(7);
    tree.debug_show();
    tree.debug_validation();

    return 0;
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "library/baranced_binary_tree.hpp"
#include <iostream>

// insert/delete赤黒木による平衡二分探索木(set仕様)
// Kはcomparableであることが必要
template <class K>
class BST {
    // 色クラス（赤か黒）
    enum class Color { Red, Black, Error };
    // 頂点
    class Node {
      public:
        K key;
        Color color = Color::Red;
        int size = 1;
        Node *left = nullptr,
             *right = nullptr; // 子。leftは自身よりも小さい値のNode
        Node(K key) : key(key) {}
    };

    Node *root;
    bool change = false; // 変更が必要か否か

    bool isRed(Node *node) {
        return node != nullptr && node->color == Color::Red;
    }
    bool isBlack(Node *node) {
        return node != nullptr && node->color == Color::Black;
    }

    // nodeを根とする部分木のサイズを取得
    int getSize(Node *node) {
        if (node == nullptr)
            return 0;
        else
            return node->size;
    }

    // サイズ再計算する
    void resetSize(Node *node) {
        if (node != nullptr) {
            node->size = getSize(node->left) + getSize(node->right) + 1;
        }
    }

    // 左回転 回転した部分木の根を返す
    Node *rotateL(Node *v) {
        Node *u = v->right, *t2 = u->left;
        int size1 = getSize(v->left);
        int size2 = getSize(t2);
        int size3 = getSize(u->right);
        u->left = v;
        u->size = size1 + size2 + size3 + 2;
        v->right = t2;
        v->size = size1 + size2 + 1;
        return u;
    }

    // 右回転 回転した部分木の根を返す
    Node *rotateR(Node *u) {
        Node *v = u->left, *t2 = v->right;
        int size1 = getSize(v->left);
        int size2 = getSize(t2);
        int size3 = getSize(u->right);
        v->right = u;
        v->size = size1 + size2 + size3 + 2;
        u->left = t2;
        u->size = size2 + size3 + 1;
        return v;
    }

    // 左回転->右回転
    Node *rotateLR(Node *u) {
        u->left = rotateL(u->left);
        return rotateR(u);
    }

    // 右回転->左回転
    Node *rotateRL(Node *u) {
        u->right = rotateR(u->right);
        return rotateL(u);
    }

    // 部分木K以下にkeyを挿入。挿入後の根ノードのポインタを返す
    Node *insert(Node *t, K key) {
        if (t == nullptr) {
            // ここに挿入
            change = true;
            return new Node(key);
        } else if (key < t->key) {
            t->left = insert(t->left, key);
            return balance(t);
        } else if (key > t->key) {
            t->right = insert(t->right, key);
            return balance(t);
        } else {
            // キーがすでに存在
            change = false;
            resetSize(t);
            return t;
        }
    }

    // 挿入に伴うバランス バランス後の部分木の根を返す
    Node *balance(Node *t) {
        // tの子と孫で赤が連続している場合に修正
        if (!change || !isBlack(t)) {
            // 変更不要状態か、tが赤orNULLなら何もしない
        } else if (isRed(t->left) && isRed(t->left->left)) {
            t = rotateR(t);
            t->left->color = Color::Black;
        } else if (isRed(t->left) && isRed(t->left->right)) {
            t = rotateLR(t);
            t->left->color = Color::Black;
        } else if (isRed(t->right) && isRed(t->right->left)) {
            t = rotateRL(t);
            t->right->color = Color::Black;
        } else if (isRed(t->right) && isRed(t->right->right)) {
            t = rotateL(t);
            t->right->color = Color::Black;
        } else {
            change = false;
        }
        resetSize(t);
        return t;
    }

    Node *drop(Node *t, K key) {
        if (t == nullptr) {
            change = false;
            return nullptr;
        } else if (key < t->key) {
            t->left = drop(t->left, key);
            return balanceL(t);
        } else if (key > t->key) {
            t->right = drop(t->right, key);
            return balanceR(t);
        } else {
            // ノードtを削除する
            if (t->left == nullptr) {
                switch (t->color) {
                case Color::Red:
                    change = false;
                    break;
                case Color::Black:
                    change = true;
                    break;
                }
                auto ret = t->right;
                delete t;
                return ret; // 右部分木を昇格させる
            } else {
                auto result = deleteMax(t->left);
                t->left = result.first; // 左部分木の最大値を削除する
                t->key = result.second; // 左部分木の削除した最大値で置き換える
                return balanceL(t);
            }
        }
    }

    // 部分木 t の最大値のノードを削除する
    // 戻り値は削除により修正された部分木
    // 削除した最大値を lmax に保存する
    std::pair<Node *, K> deleteMax(Node *t) {
        if (t->right != nullptr) {
            auto result = deleteMax(t->right);
            t->right = result.first;
            resetSize(t);
            return std::make_pair(balanceR(t), result.second);
        } else {
            // ここが最大キー
            switch (t->color) {
            case Color::Red:
                change = false;
                break;
            case Color::Black:
                change = true;
                break;
            }
            auto ret = std::make_pair(t->left, t->key);
            delete t;
            return ret; // 左部分木を昇格させる
        }
    }

    // 左部分木のノード削除に伴う赤黒木の修正(パターンマッチ)
    // 戻り値は修正された木
    Node *balanceL(Node *t) {
        if (!change) {
            // 修正なしと赤ノード削除の場合は何もしない
        } else if (isBlack(t->right) && isRed(t->right->left)) {
            Color rb = t->color;
            t = rotateRL(t);
            t->color = rb;
            t->left->color = Color::Black;
            change = false;
        } else if (isBlack(t->right) && isRed(t->right->right)) {
            Color rb = t->color;
            t = rotateL(t);
            t->color = rb;
            t->left->color = Color::Black;
            t->right->color = Color::Black;
            change = false;
        } else if (isBlack(t->right)) {
            Color rb = t->color;
            t->color = Color::Black;
            t->right->color = Color::Red;
            change = (rb == Color::Black);
        } else if (isRed(t->right)) {
            t = rotateL(t);
            t->color = Color::Black;
            t->left->color = Color::Red;
            t->left = balanceL(t->left);
            change = false;
        } else { // 黒ノード削除の場合、ここはありえない
            std::cout << __FUNCTION__ << "(" << __LINE__ << "): Error";
        }
        resetSize(t);
        return t;
    }

    // 右部分木のノード削除に伴う赤黒木の修正(パターンマッチ)
    // 戻り値は修正された木
    Node *balanceR(Node *t) {
        if (!change) {
            // 修正なしと赤ノード削除の場合は何もしない
        } else if (isBlack(t->left) && isRed(t->left->right)) {
            Color rb = t->color;
            t = rotateLR(t);
            t->color = rb;
            t->right->color = Color::Black;
            change = false;
        } else if (isBlack(t->left) && isRed(t->left->left)) {
            Color rb = t->color;
            t = rotateR(t);
            t->color = rb;
            t->left->color = Color::Black;
            t->right->color = Color::Black;
            change = false;
        } else if (isBlack(t->left)) {
            Color rb = t->color;
            t->color = Color::Black;
            t->left->color = Color::Red;
            change = (rb == Color::Black);
        } else if (isRed(t->left)) {
            t = rotateR(t);
            t->color = Color::Black;
            t->right->color = Color::Red;
            t->right = balanceR(t->right);
            change = false;
        } else { // 黒ノード削除の場合、ここはありえない
            std::cout << __FUNCTION__ << "(" << __LINE__ << "): Error";
        }
        resetSize(t);
        return t;
    }

    // デバッグ用・赤連続禁止のチェック
    Color colorChain(Node *t) {
        if (t == nullptr) return Color::Black;
        Color p = t->color;
        Color color_l = colorChain(t->left);
        Color color_r = colorChain(t->right);
        if (color_l == Color::Error || color_r == Color::Error) {
            return Color::Error;
        }
        if (p == Color::Black) {
            return p;
        }
        if (p == Color::Red && color_l == Color::Black &&
            color_r == Color::Black) {
            return p;
        }
        return Color::Error;
    }

    // デバッグ用・黒の数チェック
    int blackHeight(Node *t) {
        if (t == nullptr) return 0;
        int height_l = blackHeight(t->left);
        int height_r = blackHeight(t->right);
        if (height_l < 0 || height_r < 0 || height_l != height_r) return -1;
        return height_l + (t->color == Color::Black ? 1 : 0);
    }

  public:
    BST() {
        root = nullptr;
    }

    // 空判定
    bool isEmpty() {
        return root == nullptr;
    }

    // 検索してNodeのポインタを返す。見つからなければnullptr
    Node *find(K key) {
        Node *pos = root;
        while (pos != nullptr) {
            if (key == pos->key) {
                return pos;
            } else if (key < pos->key) {
                pos = pos->left;
            } else {
                pos = pos->right;
            }
        }
        return pos;
    }

    bool exist(K key) {
        return find(key) != nullptr;
    }

    // 挿入のエントリポイント
    void insert(K key) {
        root = insert(root, key);
        root->color = Color::Black;
    }

    // 削除のエントリポイント
    void drop(K key) {
        root = drop(root, key);
        if (root != nullptr) root->color = Color::Black;
    }

    // 赤連続禁止チェック
    bool redValidation() {
        return colorChain(root) == Color::Black;
    }

    // 黒の数チェック
    bool blackValidation() {
        return blackHeight(root) >= 0;
    }

    // 両方のバリデーション
    void debug_validation() {
        if (!redValidation()) {
            std::cout << "Continuous Red Error!" << std::endl;
        } else if (!blackValidation()) {
            std::cout << "Black Heights Differ Error!" << std::endl;
        } else {
            std::cout << "Valid!" << std::endl;
        }
    }

    // デバッグ用出力
    void debug_show(Node *pos = nullptr) {
        if (pos == nullptr) {
            // 根
            std::cout << "show: ";
            if (root == nullptr) {
                // 空
                std::cout << "Empty" << std::endl;
            } else {
                debug_show(root);
                std::cout << std::endl;
            }
        } else {
            if (pos->left != nullptr) {
                std::cout << "(";
                debug_show(pos->left);
                std::cout << ")";
            }
            std::cout << pos->key << (pos->color == Color::Red ? "R" : "B")
                      << getSize(pos);
            if (pos->right != nullptr) {
                std::cout << "(";
                debug_show(pos->right);
                std::cout << ")";
            }
        }
    }

    // デバッグ用存在確認
    void debug_exist(K key) {
        if (exist(key)) {
            std::cout << key << " is exist" << std::endl;
        } else {
            std::cout << key << " is not exist" << std::endl;
        }
    }

    // 木のサイズ（要素数）を取得
    int size() {
        return getSize(root);
    }

    // k番目のキーを返す errはエラー時の返り値
    K kthKey(int k, K err) {
        return kthKey(root, k, err);
    }
    K kthKey(Node *t, int k, K err) {
        if (t == nullptr) {
            return err;
        }
        if (getSize(t->left) >= k) {
            return kthKey(t->left, k, err);
        } else if (getSize(t->left) == k - 1) {
            return t->key;
        } else {
            return kthKey(t->right, k - getSize(t->left) - 1, err);
        }
    }
};

// AtCoder Regular Contest 033 C 「データ構造」 を解く関数
// 問題URL: https://arc033.contest.atcoder.jp/tasks/arc033_3
// 提出URL: https://arc033.contest.atcoder.jp/submissions/2622699
void solveARC033C() {
    BST<int> tree;

    int Q;
    std::cin >> Q;
    for (size_t i = 0; i < Q; i++) {
        int t, x;
        std::cin >> t >> x;
        if (t == 1) {
            tree.insert(x);
            // tree.debug_show();
        } else {
            int n = tree.kthKey(x, -1);
            std::cout << n << std::endl;
            tree.drop(n);
            // tree.debug_show();
        }
    }
}

int main() {
    BST<int> tree;
    tree.insert(10);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(9);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(8);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(6);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(7);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(15);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(16);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(17);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(18);
    tree.debug_show();
    tree.debug_validation();

    tree.insert(19);
    tree.debug_show();
    tree.debug_validation();

    tree.drop(10);
    tree.debug_show();
    tree.debug_validation();

    tree.drop(7);
    tree.debug_show();
    tree.debug_validation();

    return 0;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

