#include "header.hpp"

namespace FFT {
// FFT 用の簡易複素数
struct Comp {
    double real, imag;

    Comp(double r = 0, double i = 0) : real(r), imag(i) {}
    template <class T>
    Comp(T r) : real(r){};
    // 極形式で生成
    static Comp polar(double r, double theta) {
        return Comp(r * cos(theta), r * sin(theta));
    }
    Comp &operator+=(const Comp &rhs) {
        real += rhs.real;
        imag += rhs.imag;
        return *this;
    }
    Comp &operator-=(const Comp &rhs) {
        real -= rhs.real;
        imag -= rhs.imag;
        return *this;
    }
    Comp &operator*=(const Comp &rhs) {
        double new_real = real * rhs.real - imag * rhs.imag;
        imag = real * rhs.imag + imag * rhs.real;
        real = new_real;
        return *this;
    }
    Comp &operator/=(const Comp &rhs) {
        *this *= ~rhs;
        real /= rhs.abs();
        imag /= rhs.abs();
        return *this;
    }
    // 複素共役
    Comp operator~() const {
        return Comp(real, -imag);
    }
    // 絶対値
    double abs() const {
        return sqrt(real * real + imag * imag);
    }
};
Comp operator+(const Comp &lhs, const Comp &rhs) {
    return Comp(lhs) += rhs;
}
Comp operator-(const Comp &lhs, const Comp &rhs) {
    return Comp(lhs) -= rhs;
}
Comp operator*(const Comp &lhs, const Comp &rhs) {
    return Comp(lhs) *= rhs;
}
Comp operator/(const Comp &lhs, const Comp &rhs) {
    return Comp(lhs) /= rhs;
}
ostream &operator<<(ostream &os, const Comp &comp_var) {
    os << comp_var.real << "+" << comp_var.imag << "i";
    return os;
}

// 重み（回転因子）: Weight[i][j] = 1 の原始 2^i 乗根の j 乗
vector<vector<Comp>> Weight(1, vector<Comp>(1, 1));

// 重みの初期化（step = log_2(N)）
void W_init(int step) {
    for (int i = 1; i <= step; i++) {
        Weight.emplace_back(1 << (i - 1));
        const double theta = 2 * M_PI / (1 << i);
        for (int j = 0; j < (1 << (i - 1)); j++) {
            Weight[i][j] = ((j & 1) != 0 ? Comp::polar(1.0, theta * j) : Weight[i - 1][j >> 1]);
        }
    }
}

// ビット反転並べ替え
void bit_reverse(vector<Comp> &x) {
    int n = x.size();
    for (int i = 0, j = 1; j < n - 1; j++) {
        for (int k = n >> 1; k > (i ^= k); k >>= 1)
            ;
        if (i < j) std::swap(x[i], x[j]); // x[i]とx[j]を交換する
    }
}

// Cooley–Tukey FFT algorithm
template <bool inverse = false>
void fft(vector<Comp> &a) {
    int n = a.size();
    int h = 0; // h = log_2(n)
    for (int i = 0; 1 << i < n; i++) h++;
    bit_reverse(a); // バタフライ演算用の並べ替え

    // バタフライ演算
    for (int s = 1, b = 1; b < n; s++, b <<= 1) {
        // 第 s = log_2(b)+1 段
        // ブロックサイズ = 2b = 2^s
        for (int j = 0; j < b; j++) {
            // グループ内 j 個目
            // 重み w = (1 の原始 2b=2^s 乗根の j 乗)
            Comp w = (inverse ? ~Weight[s][j] : Weight[s][j]);
            for (int k = 0; k < n; k += b * 2) {
                // k を先頭とするグループ
                Comp s = a[j + k];         // 前
                Comp t = a[j + k + b] * w; // 後
                a[j + k] = s + t;          // 前の更新
                a[j + k + b] = s - t;      // 後の更新
            }
        }
    }
    // 逆変換時にサイズで割る調整
    if (inverse)
        for (int i = 0; i < n; i++) a[i] /= n;
}

// FFT による畳み込み（答えが a に入る）
void convolve(vector<double> &a, const vector<double> &b) {
    int s = a.size() + b.size() - 1; // 畳み込み結果のサイズ
    int t = 1;                       // FFT に使う配列のサイズ（2 の累乗）
    int step = 0;
    while (t < s) {
        t *= 2;
        step++;
    }
    // 実部を a, 虚部を b とした複素数列を作成
    vector<Comp> C(t, Comp(0, 0));
    for (int i = 0; i < a.size(); i++) {
        C[i].real = a[i];
    }
    for (int i = 0; i < b.size(); i++) {
        C[i].imag = b[i];
    }
    W_init(step); // 重み初期化
    fft(C);       // a, b をまとめて FFT
    // まとめて FFT した結果から A[i]*B[i] を計算
    C[0] = (C[0] + ~C[0]) * (C[0] - ~C[0]) * Comp(0, -0.25);
    for (int i = 1; i <= (t >> 1); i++) {
        Comp x = C[i] + ~C[t - i];
        Comp y = C[i] - ~C[t - i];
        C[i] = x * y * Comp(0, -0.25);
        C[t - i] = ~C[i];
    }
    fft<true>(C); // IFFT
    a.resize(s);
    for (int i = 0; i < s; i++) a[i] = C[i].real; // 実部が答え
}
} // namespace FFT
