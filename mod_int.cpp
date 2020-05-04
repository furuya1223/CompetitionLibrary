#include "header.hpp"

template <uint_fast64_t Mod>
class mint {
    using u64 = uint_fast64_t;
    u64 value;

  public:
    constexpr mint(long long value = 0) noexcept : value(value % Mod) {}

    u64 &get() noexcept {
        return value;
    }
    constexpr const u64 &get() const noexcept {
        return value;
    }
    constexpr mint operator+(const mint &rhs) const noexcept {
        return mint(*this) += rhs;
    }
    constexpr mint operator-(const mint &rhs) const noexcept {
        return mint(*this) -= rhs;
    }
    constexpr mint operator*(const mint &rhs) const noexcept {
        return mint(*this) *= rhs;
    }
    constexpr mint operator/(const mint &rhs) const noexcept {
        return mint(*this) /= rhs;
    }
    mint &operator+=(const mint &rhs) noexcept {
        value += rhs.value;
        if (value >= Mod) {
            value -= Mod;
        }
        return *this;
    }
    mint &operator-=(const mint &rhs) noexcept {
        if (value < rhs.value) {
            value += Mod;
        }
        value -= rhs.value;
        return *this;
    }
    mint &operator*=(const mint &rhs) noexcept {
        value = value * rhs.value % Mod;
        return *this;
    }
    mint &operator/=(mint rhs) noexcept {
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
    mint pow(u64 n) const noexcept {
        mint ret(1), p(*this);
        while (n > 0) {
            if (n & 1) ret *= p;
            p *= p;
            n >>= 1;
        }
        return ret;
    }
};
template <class Char, class Traits, uint_fast64_t Mod>
basic_ostream<Char, Traits> &operator<<(basic_ostream<Char, Traits> &os,
                                        const mint<Mod> &modint) {
    os << modint.get();
    return os;
}
template <class Char, class Traits, uint_fast64_t Mod>
std::basic_istream<Char, Traits> &operator>>(basic_istream<Char, Traits> &is,
                                             mint<Mod> &modint) {
    is >> modint.get();
    return is;
}
template <uint_fast64_t Mod>
constexpr bool operator==(const mint<Mod> &lhs, const mint<Mod> &rhs) {
    return lhs.get() == rhs.get();
}
template <uint_fast64_t Mod>
constexpr bool operator!=(const mint<Mod> &lhs, const mint<Mod> &rhs) {
    return lhs != rhs;
}
template <uint_fast64_t Mod>
constexpr bool operator==(const long long lhs, const mint<Mod> &rhs) {
    return lhs == rhs.get();
}
template <uint_fast64_t Mod>
constexpr bool operator!=(const long long lhs, const mint<Mod> &rhs) {
    return lhs != rhs.get();
}