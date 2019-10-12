/*
class UnionFind {
public:
	vector<int> par, rank, _size;
	int _num;

	UnionFind(int n) {
		par = vector<int>(n);
		rank = vector<int>(n);
		_size = vector<int>(n);
		num = n;
		for (int i = 0; i < n; i++) {
			par[i] = i;
			rank[i] = 0;
			_size[i] = 1;
		}
	}

	// 木の根を求める
	int find(int x) {
		if (par[x] == x) {
			return x;
		}
		else {
			return par[x] = find(par[x]);
		}
	}

	// xとyの属する集合を併合
	void unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y)return;

		num--;
		if (rank[x] < rank[y]) {
			par[x] = y;
			_size[y] += _size[x];
		}
		else {
			par[y] = x;
			_size[x] += _size[y];
			if (rank[x] == rank[y])rank[x]++;
		}
	}

	// xとyが同じ集合に属するか否か
	bool same(int x, int y) {
		return find(x) == find(y);
	}

	// xの属する集合のサイズ
	int size(int x) {
		return _size[find(x)];
	}

	// 集合の個数
	int num(){
		return _num;
	}
};
*/