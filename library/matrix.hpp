#include "header.hpp"

template <typename T>
class Mat {
  private:
    vector<vector<T>> _dat;
    T mod;

    void SwapRow(int a, int b) {
        if (a < 0 || a >= height() || b < 0 || b >= height()) return;
        rep(i, width()) {
            T tmp = _dat[a][i];
            _dat[a][i] = _dat[b][i];
            _dat[b][i] = tmp;
        }
    }
    void SwapCol(int a, int b) {
        if (a < 0 || a >= width() || b < 0 || b >= width()) return;
        rep(i, width()) {
            T tmp = _dat[i][a];
            _dat[i][a] = _dat[i][b];
            _dat[i][b] = tmp;
        }
    }

  public:
    Mat<T>(int mod) {
        this->mod = mod;
    }
    Mat<T>(int h, int w, int mod) {
        _dat = vector<vector<T>>(h);
        this->mod = mod;
        rep(i, height()) {
            _dat[i] = vector<T>(w);
            fill(begin(_dat[i]), end(_dat[i]), 0);
        }
    }
    Mat<T>(vector<vector<T>> dat, int mod) {
        this->mod = mod;
        _dat = dat;
    }

    int height() {
        return _dat.size();
    }
    int width() {
        return _dat[0].size();
    }

    vector<T> &operator[](int i) {
        return _dat[i];
    }
    Mat<T> &operator=(Mat b) {
        this->_dat = b._dat;
        return (*this);
    }
    Mat<T> operator+(Mat &b) {
        if (height() != b.height() || width() != b.width()) {
            cout << "Plus Error" << endl;
            exit(1);
        }
        Mat<T> ret(height(), width());
        rep(i, height()) rep(j, width()) ret[i][j] =
            (_dat[i][j] + b[i][j]) % mod;
        return ret;
    }
    Mat<T> operator-(Mat &b) {
        if (height() != b.height() || width() != b.width()) {
            cout << "Minus Error" << endl;
            exit(1);
        }
        Mat<T> ret(height(), width());
        rep(i, height()) rep(j, width()) ret[i][j] =
            (_dat[i][j] - b[i][j] + mod) % mod;
        return ret;
    }
    Mat<T> operator*(Mat b) {
        if (width() != b.height()) {
            cout << "Production Error" << endl;
            exit(1);
        }
        int h = height(), w = b.width();
        Mat<T> ret(h, w, mod);
        rep(i, h) rep(j, w) rep(k, width()) {
            ret[i][j] += (_dat[i][k] * b[k][j]) % mod;
            ret[i][j] %= mod;
        }
        return ret;
    }

    Mat<T> transpose() {
        Mat<T> ret(width(), height());
        rep(i, width()) rep(j, height()) {
            ret[i][j] = _dat[j][i];
        }
        return ret;
    }

    Mat<T> identity(int n) {
        Mat<T> idt(n, n, mod);
        rep(i, n) idt[i][i] = 1;
        return idt;
    }

    double determinant() {
        if (height() != width()) {
            cout << "Determinant Error" << endl;
            exit(1);
        }
        Mat<T> tmp = *this;
        int N = height();
        rep(i, N) {
            repr(j, i + 1, N) {
                double buf = tmp[j][i] / (double)tmp[i][i];
                rep(k, N) {
                    tmp[j][k] -= buf * tmp[i][k];
                }
            }
        }
        double det = 1;
        rep(i, N) {
            det *= tmp[i][i];
        }
        return det;
    }

    Mat<T> operator~() {
        if (height() != width()) {
            cout << "Inverse Error" << endl;
            exit(1);
        }
        int N = height();
        Mat<T> ret = identity(N);
        Mat<T> copy = *this;
        double buf;
        rep(i, N) {
            buf = 1 / (double)copy[i][i];
            // (i,i)ê¨ï™Ç1Ç…Ç∑ÇÈ
            rep(j, N) {
                copy[i][j] *= buf;
                ret[i][j] *= buf;
            }
            rep(j, N) {
                if (i != j) {
                    buf = copy[j][i];
                    rep(k, N) {
                        copy[j][k] -= copy[i][k] * buf;
                        ret[j][k] -= ret[i][k] * buf;
                    }
                }
            }
        }
        return ret;
    }

    void show() {
        rep(i, height()) {
            rep(j, width()) {
                cout << _dat[i][j];
                if (j != width() - 1) cout << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }
    void showSize() {
        cout << '(' << height() << ',' << width() << ')' << endl;
    }
};

template <typename T>
Mat<T> powMat(Mat<T> x, ll n) {
    Mat<T> res = x.identity(x.height());
    while (n > 0) {
        if (n & 1) res = res * x;
        x = x * x;
        n >>= 1;
    }
    return res;
}