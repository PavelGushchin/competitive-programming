#ifndef __NUMERIC_HPP__
#define __NUMERIC_HPP__
namespace algos {
namespace numeric {
    
    template<typename T, typename N> 
    inline T binpow(T a, N n) {
        T r(1);
        while (n > 0) {
            if (n & 1) { r *= a; }
            a *= a; n >>= 1;
        }
        return r;
    }

    template<int mod>
    struct IntMod {
        int value;
        IntMod(int value_ = 0) : value(value_) {
            if (value >= mod || value < 0) value %= mod;
            if (value < 0) value += mod;
        }
        IntMod(ll x) {
            if (x >= mod || x < 0) x %= mod;
            if (x < 0) x += mod;
            value = int(x);
        }
        IntMod& operator+=(IntMod num) {
            value += num.value;
            if (value >= mod) value -= mod;
            return *this;
        }
        IntMod& operator-=(IntMod num) {
            value -= num.value;
            if (value < 0) value += mod;
            return *this;
        }
        IntMod operator-() const { return IntMod(0) - *this; }
        IntMod operator+(IntMod num) const { return IntMod(*this) += num; }
        IntMod operator-(IntMod num) const { return IntMod(*this) -= num; }
        IntMod operator*(IntMod num) const { return IntMod(int(value * 1LL * num.value % mod)); }
        IntMod& operator*=(IntMod num) { return *this = *this * num; }
        IntMod operator/(IntMod num) const { return *this * binpow(num, mod-2); }
        IntMod& operator/=(IntMod num) { return *this = *this / num; }
        
        friend bool operator<(const IntMod<mod> &a, const IntMod<mod> &b){
            return a.value < b.value;
        }
        friend std::ostream& operator<<(std::ostream& os, const IntMod<mod>& num) {
            return os << num.value;
        }
        GEN_COMPARATORS_MEMBERS(IntMod<mod>)
    };

    template<int mod>
    struct Combinatorics {
        
        int N;
        std::vector<IntMod<mod>> fact, ifact;
        
        Combinatorics(int n_)
            : N(n_), fact(N+1,1), ifact(N+1,1)
        {
            for (int i = 2; i <= N; i++)
                fact[i] = fact[i-1] * i;
            ifact[N] = fact[0] / fact[N];
            for (int i = N-1; i >= 0; i--)
                ifact[i] = ifact[i+1] * (i+1);
        }
        
        IntMod<mod> operator()(int n, int k) const {
            return C(n, k);
        }
        
        IntMod<mod> C(int n, int k) const {
            if (k < 0 || k > n || n < 0) return 0;
            return fact[n] * ifact[k] * ifact[n-k];
        }
        
    };
    
    template<typename T>
    void gauss(std::vector<std::vector<T>> &a) {
        const int n = (int)a.size();
        assert((int)a[0].size()+1 == n);
        // решаем СЛАУ
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                T coeff = (a[j][i] / a[i][i]);
                for (int k = i; k <= n; k++)
                    a[j][k] -= a[i][k] * coeff;
            }
        }
        for (int i = 0; i < n; i++)
            a[i].back() /= a[i][i];
    }
    
    template<typename T>
    void gauss(std::vector<std::vector<T>> &a, std::vector<T> &b) {
        const int n = (int)a.size();
        assert((int)a[0].size() == n);
        assert((int)b.size() == n);
        // решаем СЛАУ
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) 
                if (i != j) {
                    T coeff = (a[j][i] / a[i][i]);
                    for (int k = i; k < n; k++)
                        a[j][k] -= a[i][k] * coeff;
                    b[j] -= coeff * b[i];
                }
        // находим ответ:
        for (int i = 0; i < n; i++)
            b[i] /= a[i][i];
    }
    
    template<typename T>
    struct Poly : public std::vector<T>
    {    
        Poly() : std::vector<T>(){}

        Poly(const std::vector<std::pair<T,T>> & points)
            : std::vector<T>(points.size())    
        {
            interpolate(points);
        }
        
        void interpolate(const auto & points)
        {
            const int n = (int)points.size();
            // строим систему линейных уравнений
            std::vector a(n, std::vector(n+1, T(0)));
            for (int i = 0; i < n; i++) {
                const auto [x, f] = points[i];
                T px = 1;
                for (int j = 0; j < n; j++) {
                    a[i][j] = px;
                    px *= x;
                }
                a[i][n] = f;
            }
            // решаем СЛАУ
            gauss(a);
            // находим ответ
            this->resize(n);
            for (int i = n-1; i >= 0; i--)
                (*this)[i] = a[i].back();
        }
        
        T operator()(T x) const {
            T px = 1, res = 0;
            for (const auto &c : *this) {
                res += c * px;
                px *= x;
            }
            return res;
        }
        
    };

    inline int addmod(int a, int b, int mod) {
        return (a += b) >= mod ? a -= mod : a;
    }
    
    inline int submod(int a, int b, int mod) {
        return (a -= b) < 0 ? a += mod : a;
    }
    
    inline int mulmod(int a, int b, int mod) {
        return int(1LL * a * b % mod);
    }
    
    inline int powmod(int x, int64_t n, int mod) {
        if (x < 2) { return x; }
        int res = 1;
        while (n > 0) {
            if (n & 1) { res = mulmod(res,x,mod); }
            n >>= 1; x = mulmod(x,x,mod);
        }
        return res;
    }

    inline int totient(int x) {
        int res = 1;
        for (int i = 2; i * i <= x; ++i) {
            if (x % i == 0) {
                int p = 1;
                x /= i;
                while (x % i == 0) {
                    p *= i; x /= i;
                }
                res *= (i - 1) * p;
            }
        }
        if (x > 1) { res *= (x-1); }
        return res;
    }

    inline int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    inline vi primeDivisors(int x) {
        vi answ;
        for (int i = 2; i * i <= x; ++i) {
            if (x % i == 0) {
                answ.push_back(i);
                x /= i;
                while (x % i == 0) { x /= i; }
            }
        }
        if (x > 1) { answ.push_back(x); }
        return answ;
    }

    inline int primitiveRoot(const int mod) {
        int tot = totient(mod);
        vi divs = primeDivisors(tot);
        for (auto &it : divs) {
            it = tot / it;
        }
        for (int i = 2; i < mod; ++i) {
            bool ok = true;
            for (int p : divs) {
                if (powmod(i,p, mod) == 1) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                int root = i;
                return root;
            }
        }
        return -1;
    }

    inline int logmod(int root, int y, const int mod)
    {
        // root ^ x == y
        int k = (int)std::sqrt(mod);
        std::vector<pii> big, small;
        int inv = powmod(root, mod-2, mod);
        small.resize(k);
        small[0] = pii(y, 0);
        for (int i = 1; i < k; ++i) {
            auto prev = small[i-1];
            prev.first = mulmod(prev.first, inv, mod);
            prev.second++;
            small[i] = prev;
        }
        big.resize((mod+k-1)/k);
        int rootK = powmod(root,k, mod);
        big[0] = pii(1, 0);
        for (int i = 1; i < isz(big); ++i) {
            big[i].first = mulmod(big[i-1].first, rootK, mod);
            big[i].second = i * k;
        }
        std::sort(all(big));
        std::sort(all(small));
        for (auto &pair : big) {
            auto it = std::lower_bound(all(small), pii(pair.first, 0));
            if (it == small.end() || it->first != pair.first) {
                continue;
            }
            int p = pair.second + it->second;
            assert(powmod(root, p, mod) == y);
            return p;
        }
        return -1;
    }
} // namespace numeric
} // namespace algos
#endif // __NUMERIC_HPP__