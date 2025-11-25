#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

struct Complex {
  long double x, y;
  Complex operator+(const Complex &a) const {
    return (Complex){x + a.x, y + a.y};
  }
  Complex operator-(const Complex &a) const {
    return (Complex){x - a.x, y - a.y};
  }
  Complex operator*(const Complex &a) const {
    return (Complex){x * a.x - y * a.y, x * a.y + y * a.x};
  }
};
const long double PI = acos(-1);
constexpr int MOD1 = 998244353;
constexpr int G1 = 3;
constexpr int Gi1 = 332748118;
constexpr int MOD2 = 1004535809;
constexpr int G2 = 3;
constexpr int Gi2 = 334845270;
constexpr int MOD3 = 469762049;
constexpr int G3 = 3;
constexpr int Gi3 = 156587350;

void FFT(Complex *f, int flag, int n);
void NTT(long long *const f, int flag, int n, const int MOD, const int G,
         const int Gi);
void DoubleNTT(long long *const f, long long *const g, int flag, int n,
               const int MOD, const int G, const int Gi);
class int2048 {
  // len : 最高位所在下标
  // sign 1正数和零 -1负数
  int len, sign;
  std::vector<int> a;

public:
  static constexpr int BASE = 1e9;
  static constexpr int BASE_LEN = 9;
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);
  operator double() const;
  operator std::string() const;
  long long Qpow(long long, long long, const int);
  int StringToInt(const std::string &, int, int);
  std::string IntToString(int, int) const;
  void Init();
  void Init(const std::string &);
  bool CheckZero() const;
  friend int2048 DivForce(const int2048 &, const int2048 &);
  friend int2048 MulForce(const int2048 &, const int2048 &);
  friend int2048 Div(const int2048 &, const int2048 &);
  friend inline int2048 Shift(const int2048 &, const int);
  friend inline int2048 NewtonInv(const int2048 &);
  friend inline int2048 CalcInvb(const int2048 &);

  int getsign() { return sign; }
  int2048 &add1();
  int2048 &minus1();

  void read(const std::string &);
  void print();

  int2048 &add(const int2048 &);
  friend int2048 add(int2048, const int2048 &);

  int2048 &minus(const int2048 &);
  friend int2048 minus(int2048, const int2048 &);

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};

#endif
