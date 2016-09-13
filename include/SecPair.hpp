#ifndef __SECPAIR_HPP__
#define __SECPAIR_HPP__

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

const unsigned MAX_SIZE = 48;

class SecPair
{
public:
  SecPair() : f_(-1), b_(-1), mult_(1)
  { check(); }
  SecPair(int f, int b, int mult) : f_(f), b_(b), mult_(mult)
  { check(); }
  SecPair(const SecPair& rhs) : f_(rhs.f_), b_(rhs.b_), mult_(rhs.mult_) { check(); }
  SecPair(SecPair&& rhs) = default;
  SecPair& operator=(SecPair&&) = default;

  SecPair abs();
  bool isPos();

  SecPair& operator*=(int m);
  SecPair operator*(int m);
  bool operator==(const SecPair& rhs) const;
  bool operator!=(const SecPair& rhs) const;
  SecPair& operator=(const SecPair& rhs);
  SecPair& operator+=(const SecPair& rhs);
  SecPair operator+(const SecPair& rhs) const;
  SecPair& operator-=(const SecPair& rhs);
  SecPair operator-(const SecPair& rhs) const;
  bool operator<(const SecPair& rhs) const;
  
public:
  int f_;
  int b_;
  int mult_;
  std::vector<int> all_legs_ = std::vector<int>(48,0);
private:
  void check();
};

std::ostream& operator<<(std::ostream& out, const SecPair& p);

#endif
