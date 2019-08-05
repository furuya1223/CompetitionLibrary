#include "header.hpp"

template <std::uint_fast64_t Mod>
class ModInt {
    using u64 = std::uint_fast64_t;
    u64 value;

  public:
    constexpr ModInt(long long value = 0) noexcept : value(value % Mod) {}

    constexpr u64 &get() noexcept {
        return value;
    }
    constexpr const u64 &get() const noexcept {
        return value;
    }
    constexpr ModInt operator+(const ModInt rhs) const noexcept {
        return ModInt(*this) += rhs;
    }
    constexpr ModInt operator-(const ModInt rhs) const noexcept {
        return ModInt(*this) -= rhs;
    }
    constexpr ModInt operator*(const ModInt rhs) const noexcept {
        return ModInt(*this) *= rhs;
    }
    constexpr ModInt operator/(const ModInt rhs) const noexcept {
        return ModInt(*this) /= rhs;
    }
    constexpr ModInt &operator+=(const ModInt rhs) noexcept {
        value += rhs.value;
        if (value >= Mod) {
            value -= Mod;
        }
        return *this;
    }
    constexpr ModInt &operator-=(const ModInt rhs) noexcept {
        if (value < rhs.value) {
            value += Mod;
        }
        value -= rhs.value;
        return *this;
    }
    constexpr ModInt &operator*=(const ModInt rhs) noexcept {
        value = value * rhs.value % Mod;
        return *this;
    }
    constexpr ModInt &operator/=(ModInt rhs) noexcept {
        u64 exp = Mod - 2;
        while (exp) {
            if (exp % 2 == 1) {
                *this *= rhs;
            }
            rhs *= rhs;
            exp /= 2;
        }
        return *this;
    }
    constexpr ModInt pow(u64 n) const noexcept {
        ModInt ret(1), p(*this);
        while (n > 0) {
            if (n & 1) ret *= p;
            p *= p;
            n >>= 1;
        }
        return ret;
    }
};
template <std::uint_fast64_t Mod>
ostream &operator<<(ostream &os, ModInt<Mod> &modint) {
    os << modint.get();
    return os;
}
template <std::uint_fast64_t Mod>
istream &operator>>(istream &is, ModInt<Mod> &modint) {
    is >> modint.get();
    return is;
}
template <std::uint_fast64_t Mod>
constexpr bool operator==(const ModInt<Mod> &lhs, const ModInt<Mod> &rhs) {
    return lhs.get() == rhs.get();
}
template <std::uint_fast64_t Mod>
constexpr bool operator!=(const ModInt<Mod> &lhs, const ModInt<Mod> &rhs) {
    return lhs != rhs;
}
template <std::uint_fast64_t Mod>
constexpr bool operator==(const long long lhs, const ModInt<Mod> &rhs) {
    return lhs == rhs.get();
}
template <std::uint_fast64_t Mod>
constexpr bool operator!=(const long long lhs, const ModInt<Mod> &rhs) {
    return lhs != rhs.get();
}