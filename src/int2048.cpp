#include "int2048.h"
#include <cstddef>

int2048::operator double() const {
  double ans = 0;
  for (int i = len; i >= 0; i--) {
    ans = ans * BASE + a[i];
  }
  if (sign == -1) {
    ans *= -1;
  }
  return ans;
}

int2048::operator std::string() const {
  if (CheckZero()) {
    return "0";
  }
  std::string str = "";
  if (sign == -1) {
    str += '-';
  }
  for (int i = len; i >= 0; i--) {
    str += IntToString(a[i], (i != len));
  }
  return str;
}

long long Qpow(long long a, long long b, const int MOD) {
  long long ans = 1;
  while (b) {
    if (b & 1) {
      ans = ans * a % MOD;
    }
    a = a * a % MOD;
    b >>= 1;
  }
  return ans;
}

void FFT(Complex *f, int flag, int n) {
  int *tr = new int[n + 5];
  tr[0] = 0;
  for (int i = 0; i < n; i++) {
    tr[i] = (tr[i >> 1] >> 1) | ((i & 1) ? (n / 2) : 0);
  }
  for (int i = 0; i < n; i++) {
    if (i < tr[i]) {
      std::swap(f[i], f[tr[i]]);
    }
  }
  for (int p = 2; p <= n; p *= 2) {
    Complex w1 = (Complex){cos(2 * PI / p), flag * sin(2 * PI / p)};
    int len = p / 2;
    for (int k = 0; k < n; k += p) {
      Complex tmp = (Complex){1, 0};
      for (int i = k; i < k + len; i++) {
        Complex temp = tmp * f[i + len];
        f[i + len] = f[i] - temp;
        f[i] = f[i] + temp;
        tmp = tmp * w1;
      }
    }
  }
  delete[] tr;
}

void NTT(long long *const f, int flag, int n, const int MOD, int G, int Gi) {
  int *tr = new int[n + 5];
  tr[0] = 0;
  for (int i = 0; i < n; i++) {
    tr[i] = (tr[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
  }
  for (int i = 0; i < n; i++) {
    if (i < tr[i]) {
      std::swap(f[i], f[tr[i]]);
    }
  }
  for (int p = 2, p_len = 1; p <= n; p *= 2, p_len++) {
    int len = p >> 1;
    long long w1 = Qpow(flag == 1 ? G : Gi, (MOD - 1) >> p_len, MOD);
    std::vector<long long> visw(len + 2);
    visw[0] = 1;
    for (int i = 1; i < len; i++) {
      visw[i] = visw[i - 1] * w1 % MOD;
    }
    for (int k = 0; k < n; k += p) {
      long long w = 1;
      for (int i = k; i < k + len; i++) {
        long long temp = w * f[i + len] % MOD;
        f[i + len] = (f[i] - temp);
        if (f[i + len] < 0) {
          f[i + len] += MOD;
        }
        f[i] += temp;
        if (f[i] >= MOD) {
          f[i] -= MOD;
        }
        w = visw[i - k + 1];
      }
    }
  }
  if (flag == -1) {
    long long inv_n = Qpow(n, MOD - 2, MOD);
    for (int i = 0; i < n; i++) {
      f[i] = f[i] * inv_n % MOD;
    }
  }
  delete[] tr;
}

void DoubleNTT(long long *const f, long long *const g, int flag, int n,
               const int MOD, int G, int Gi) {
  int *tr = new int[n + 5];
  tr[0] = 0;
  for (int i = 0; i < n; i++) {
    tr[i] = (tr[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
  }
  for (int i = 0; i < n; i++) {
    if (i < tr[i]) {
      std::swap(f[i], f[tr[i]]);
      std::swap(g[i], g[tr[i]]);
    }
  }
  for (int p = 2, p_len = 1; p <= n; p <<= 1, p_len++) {
    int len = p >> 1;
    long long w1 = Qpow(flag == 1 ? G : Gi, (MOD - 1) >> p_len, MOD);
    std::vector<long long> visw(len + 2);
    visw[0] = 1;
    for (int i = 1; i < len; i++) {
      visw[i] = visw[i - 1] * w1 % MOD;
    }
    for (int k = 0; k < n; k += p) {
      long long w = 1;
      for (int i = k; i < k + len; i++) {
        long long temp = w * f[i + len] % MOD;
        f[i + len] = (f[i] - temp);
        if (f[i + len] < 0) {
          f[i + len] += MOD;
        }
        f[i] += temp;
        if (f[i] >= MOD) {
          f[i] -= MOD;
        }
        temp = w * g[i + len] % MOD;
        g[i + len] = (g[i] - temp);
        if (g[i + len] < 0) {
          g[i + len] += MOD;
        }
        g[i] += temp;
        if (g[i] >= MOD) {
          g[i] -= MOD;
        }
        w = visw[i - k + 1];
      }
    }
  }
  if (flag == -1) {
    long long inv_n = Qpow(n, MOD - 2, MOD);
    for (int i = 0; i < n; i++) {
      f[i] = f[i] * inv_n % MOD;
      g[i] = g[i] * inv_n % MOD;
    }
  }
  delete[] tr;
}

int2048 MulForce(const int2048 &a, const int2048 &b) {
  int tmp_len = a.len + b.len;
  std::vector<__int128> tmp(a.len + b.len + 2);
  for (int i = 0; i <= a.len; i++) {
    for (int j = 0; j <= b.len; j++) {
      tmp[i + j] += 1ll * a.a[i] * b.a[j];
    }
  }
  for (int i = 0; i <= tmp_len; i++) {
    tmp[i + 1] += tmp[i] / int2048::BASE;
    tmp[i] %= int2048::BASE;
  }
  if (tmp[tmp_len + 1]) {
    tmp_len++;
  }
  while (tmp_len && tmp[tmp_len] == 0) {
    tmp_len--;
  }
  int2048 ans;
  ans.len = tmp_len;
  ans.a.resize(ans.len + 1);
  for (int i = 0; i <= tmp_len; i++) {
    ans.a[i] = tmp[i];
  }
  ans.sign = a.sign * b.sign;
  if (ans.CheckZero()) {
    ans.sign = 1;
  }
  return ans;
}

int2048 DivForce(const int2048 &a, const int2048 &b) {
  int2048 x(b), y(1ll);
  std::vector<int2048> s1, s2;
  while (x <= a) {
    s1.emplace_back(x);
    s2.emplace_back(y);
    x = x + x;
    y = y + y;
  }
  x = a;
  int2048 ans;
  for (int i = (int)s1.size() - 1; i >= 0; i--) {
    if (s1[i] <= x) {
      x -= s1[i];
      ans += s2[i];
    }
  }
  return ans;
}

inline int2048 Shift(const int2048 &a, const int dig) {
  int2048 ans;
  if (a.len + dig < 0) {
    return ans;
  }
  ans.len = a.len + dig;
  ans.a.resize(ans.len + 1);
  for (int i = 0; i <= ans.len; i++) {
    if (i - dig >= 0 && i - dig <= a.len) {
      ans.a[i] = a.a[i - dig];
    } else {
      ans.a[i] = 0;
    }
  }
  return ans;
}

inline int2048 NewtonInv(const int2048 &b) {
  int m = b.len + 1, k = (m + 3) / 2;
  int2048 g_2m = Shift(int2048(1), 2 * m);
  if (b.len <= 11) {
    return DivForce(g_2m, b);
  }
  int2048 tmp = Shift(b, -(m - k)); // b'
  int2048 res = NewtonInv(tmp);     // c'
  int2048 ans = Shift(res + res, m - k) - Shift(b * res * res, -2 * k);
  return ans;
}

int2048 Div(const int2048 &a, const int2048 &b) {
  int2048 x(a), y(b), tmp_b(b);
  if (x < y) {
    return int2048();
  }
  x.sign = 1;
  y.sign = 1;
  tmp_b.sign = 1;
  int n = x.len + 1, m = y.len + 1;
  if (n > m * 2) {
    x = Shift(x, n - m * 2);
    y = Shift(y, n - m * 2);
    m = n - m;
    n = m * 2;
  }
  int2048 res = NewtonInv(y);
  res = Shift(res * x, -m * 2);
  if (a - res * tmp_b >= tmp_b) {
    res.add1();
  }
  if (a < res * tmp_b) {
    res.minus1();
  }
  return res;
}

int int2048::StringToInt(const std::string &str, int l, int r) {
  long long res = 0;
  for (int i = l; i <= r; i++) {
    res = res * 10 + str[i] - '0';
  }
  return res;
}

std::string int2048::IntToString(int val, int op) const {
  std::string ans = "";
  long long base = BASE / 10;
  for (int i = 1; i <= BASE_LEN; i++) {
    if (op || val / base) {
      ans += val / base + '0';
      val %= base;
      op = 1;
    }
    base /= 10;
  }
  return ans;
}

bool int2048::CheckZero() const { return !(len || a[0]); }

void int2048::Init(const std::string &str) {
  len = 0;
  sign = 1;
  a.clear();
  int sz = str.size(), down = 0;
  if (str[0] == '-') {
    down = 1;
    sign = -1;
  } else if (str[0] == '+') {
    down = 1;
  }
  for (int i = sz - 1; i >= down; i -= BASE_LEN) {
    a.emplace_back(StringToInt(str, std::max(down, i - BASE_LEN + 1), i));
    len++;
  }
  len--;
  while (len && a[len] == 0) {
    len--;
  }
  if ((*this).CheckZero()) {
    sign = 1;
  }
}

int2048::int2048() {
  len = 0;
  sign = 1;
  a.clear();
  a.emplace_back(0);
}

int2048::int2048(long long num) {
  len = 0;
  sign = 1;
  a.clear();
  if (num < 0) {
    sign = -1;
    num = -num;
  }
  while (num) {
    a.emplace_back(num % BASE);
    len++;
    num /= BASE;
  }
  if (a.size() == 0) {
    a.emplace_back(0);
    len++;
  }
  len--;
}

int2048::int2048(const std::string &str) { Init(str); }

int2048::int2048(const int2048 &tmp) {
  len = tmp.len;
  sign = tmp.sign;
  a = tmp.a;
}

void int2048::read(const std::string &strr) { Init(strr); }

void int2048::print() {
  if ((*this).CheckZero()) {
    sign = 1;
    std::cout << "0";
    return;
  }
  if (sign == -1) {
    std::cout << "-";
  }
  for (int i = len; i >= 0; i--) {
    std::cout << IntToString(a[i], (i != len));
  }
}

int2048 &int2048::add1() {
  a.resize(len + 2);
  a[0]++;
  for (int i = 0; i <= len; i++) {
    if (a[i] > BASE) {
      a[i] -= BASE;
      a[i + 1]++;
    } else {
      break;
    }
  }
  return (*this);
}

int2048 &int2048::minus1() {
  if ((*this).CheckZero()) {
    a[0] = 1;
    sign = -1;
    return ((*this));
  }
  a.resize(len + 1);
  for (int i = 0; i <= len; i++) {
    if (a[i] == 0) {
      a[i] = BASE - 1;
    } else {
      a[i]--;
      break;
    }
  }
  return (*this);
}

int2048 &int2048::add(const int2048 &b) {
  int flag = (sign == b.sign) ? 1 : -1;
  int temp_len = std::max(len, b.len), temp_sign = sign;
  std::vector<int> temp(temp_len + 5);
  for (int i = 0; i <= temp_len; i++) {
    if (i <= len) {
      temp[i] += a[i];
    }
    if (i <= b.len) {
      temp[i] += flag * b.a[i];
    }
    temp[i + 1] += temp[i] / BASE;
    temp[i] %= BASE;
    if (temp[i] < 0) {
      temp[i] += BASE;
      temp[i + 1] -= 1;
    }
  }
  if (temp[temp_len + 1]) {
    temp_len++;
  }
  if (temp[temp_len] < 0) {
    temp_sign *= -1;
    for (int i = 0; i <= temp_len; i++) {
      temp[i] = -temp[i];
    }
    for (int i = 0; i <= temp_len; i++) {
      if (temp[i] < 0) {
        temp[i] += BASE;
        temp[i + 1] -= 1;
      }
    }
  }
  while (temp_len && temp[temp_len] == 0) {
    temp_len--;
  }
  if (temp_len == 0 && temp[0] == 0) {
    sign = 1;
  }
  a.resize(temp_len + 1);
  for (int i = 0; i <= temp_len; i++) {
    a[i] = temp[i];
  }
  sign = temp_sign;
  len = temp_len;
  return *this;
}

int2048 add(int2048 a, const int2048 &b) {
  int2048 tmp(a);
  tmp.add(b);
  return tmp;
}

int2048 &int2048::minus(const int2048 &b) {
  int2048 tmp(b);
  if (tmp.CheckZero() == 0) {
    tmp.sign *= -1;
  }
  return add(tmp);
}

int2048 minus(int2048 a, const int2048 &b) {
  int2048 tmp(b);
  if (tmp.CheckZero() == 0) {
    tmp.sign *= -1;
  }
  return add(a, tmp);
}

int2048 int2048::operator+() const { return (*this); }

int2048 int2048::operator-() const {
  int2048 ans(*this);
  if (ans.CheckZero() == 0) {
    ans.sign *= -1;
  }
  return (ans);
}

int2048 &int2048::operator=(const int2048 &b) {
  len = b.len;
  sign = b.sign;
  a = b.a;
  return (*this);
}

int2048 &int2048::operator+=(const int2048 &a) {
  add(a);
  return *this;
}

int2048 operator+(int2048 a, const int2048 &b) { return add(a, b); }

int2048 &int2048::operator-=(const int2048 &a) {
  minus(a);
  return (*this);
}

int2048 operator-(int2048 a, const int2048 &b) { return minus(a, b); }

int2048 &int2048::operator*=(const int2048 &a) {
  (*this) = (*this) * a;
  return (*this);
}

int2048 operator*(int2048 a, const int2048 &b) {
  if (a.len <= 16 || b.len <= 16) {
    return MulForce(a, b);
  }
  int len = a.len + b.len, n = 1;
  while (n <= len) {
    n *= 2;
  }
  long long *f1 = new long long[n + 5](), *g1 = new long long[n + 5]();
  long long *f2 = new long long[n + 5](), *g2 = new long long[n + 5]();
  long long *f3 = new long long[n + 5](), *g3 = new long long[n + 5]();
  __int128 *res = new __int128[n + 5]();
  for (int i = 0; i <= a.len; i++) {
    f1[i] = f2[i] = f3[i] = a.a[i];
  }
  for (int i = 0; i <= b.len; i++) {
    g1[i] = g2[i] = g3[i] = b.a[i];
  }
  DoubleNTT(f1, g1, 1, n, MOD1, G1, Gi1);
  DoubleNTT(f2, g2, 1, n, MOD2, G2, Gi2);
  DoubleNTT(f3, g3, 1, n, MOD3, G3, Gi3);
  for (int i = 0; i < n; i++) {
    f1[i] = f1[i] * g1[i] % MOD1;
    f2[i] = f2[i] * g2[i] % MOD2;
    f3[i] = f3[i] * g3[i] % MOD3;
  }
  NTT(f1, -1, n, MOD1, G1, Gi1);
  NTT(f2, -1, n, MOD2, G2, Gi2);
  NTT(f3, -1, n, MOD3, G3, Gi3);
  __int128 M = (__int128)MOD1 * MOD2 * MOD3;
  __int128 inv1 = Qpow(MOD1, MOD2 - 2, MOD2);
  __int128 inv12 = Qpow(1ll * MOD1 * MOD2 % MOD3, MOD3 - 2, MOD3);
  for (int i = 0; i <= len; i++) {
    __int128 x = f1[i], y = f2[i], z = f3[i];
    __int128 k = (y - x + MOD2) % MOD2 * inv1 % MOD2;
    __int128 tmp = (x + k * MOD1) % M;
    __int128 t = (z - tmp % MOD3 + MOD3) % MOD3 * inv12 % MOD3;
    res[i] = (tmp + t * MOD1 % M * MOD2 % M) % M;
  }
  for (int i = 0; i <= len; i++) {
    res[i + 1] += res[i] / int2048::BASE;
    res[i] %= int2048::BASE;
  }
  while (res[len + 1]) {
    len++;
    res[len + 1] += res[len] / int2048::BASE;
    res[len] %= int2048::BASE;
  }
  while (len && res[len] == 0) {
    len--;
  }
  int2048 ans;
  ans.sign = a.sign * b.sign;
  ans.len = len;
  ans.a.clear();
  for (int i = 0; i <= len; i++) {
    ans.a.push_back(res[i]);
  }
  if (ans.CheckZero() == 1) {
    ans.sign = 1;
  }
  delete[] f1;
  delete[] g1;
  delete[] f2;
  delete[] g2;
  delete[] f3;
  delete[] g3;
  delete[] res;
  return ans;
}

int2048 &int2048::operator/=(const int2048 &a) {
  (*this) = (*this) / a;
  return (*this);
}

int2048 operator/(int2048 a, const int2048 &b) {
  int2048 tmp_a(a), tmp_b(b);
  if (a.CheckZero() == 1) {
    return int2048();
  }
  int sign = a.sign * b.sign;
  tmp_a.sign = 1, tmp_b.sign = 1;
  int2048 ans = Div(tmp_a, tmp_b);
  if (sign == -1 && ans * tmp_b != tmp_a) {
    ans += int2048(1);
  }
  ans.sign = sign;
  return ans;
}

int2048 &int2048::operator%=(const int2048 &a) {
  (*this) = (*this) % a;
  return (*this);
}

int2048 operator%(int2048 a, const int2048 &b) { return a - (a / b) * b; }

std::istream &operator>>(std::istream &a, int2048 &b) {
  std::string str;
  a >> str;
  b.read(str);
  return a;
}

std::ostream &operator<<(std::ostream &a, const int2048 &b) {
  int2048 tmp(b);
  if (tmp.CheckZero()) {
    a << "0";
    return a;
  }
  if (tmp.sign == -1) {
    a << "-";
  }
  for (int i = tmp.len; i >= 0; i--) {
    a << tmp.IntToString(tmp.a[i], (i != tmp.len));
  }
  return a;
}

bool operator==(const int2048 &a, const int2048 &b) {
  int2048 tmp_a(a), tmp_b(b);
  if (tmp_a.CheckZero()) {
    tmp_a.sign = 1;
  }
  if (tmp_b.CheckZero()) {
    tmp_b.sign = 1;
  }
  if (tmp_a.sign != tmp_b.sign || tmp_a.len != tmp_b.len) {
    return 0;
  }
  for (int i = tmp_a.len; i >= 0; i--) {
    if (tmp_a.a[i] != tmp_b.a[i]) {
      return 0;
    }
  }
  return 1;
}

bool operator!=(const int2048 &a, const int2048 &b) { return !(a == b); }

bool operator<(const int2048 &a, const int2048 &b) {
  int2048 tmp_a(a), tmp_b(b);
  if (tmp_a.CheckZero()) {
    tmp_a.sign = 1;
  }
  if (tmp_b.CheckZero()) {
    tmp_b.sign = 1;
  }
  if (tmp_a.sign != tmp_b.sign) {
    return (tmp_a.sign < tmp_b.sign);
  }
  int sign = tmp_a.sign;
  if (tmp_a.len != tmp_b.len) {
    return (sign == 1) ? (tmp_a.len < tmp_b.len) : (tmp_a.len > tmp_b.len);
  }
  for (int i = tmp_a.len; i >= 0; i--) {
    if (tmp_a.a[i] != tmp_b.a[i]) {
      return (sign == 1) ? (tmp_a.a[i] < tmp_b.a[i])
                         : (tmp_a.a[i] > tmp_b.a[i]);
    }
  }
  return 0;
}

bool operator>(const int2048 &a, const int2048 &b) {
  return (a == b || a < b) ? 0 : 1;
}

bool operator<=(const int2048 &a, const int2048 &b) {
  return (a < b || a == b);
}

bool operator>=(const int2048 &a, const int2048 &b) {
  return (a > b || a == b);
}

