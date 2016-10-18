#include "SecPair.hpp"

void
SecPair::check()
{
  this->all_legs_ = std::vector<int>(48,0);

  if ( (this->f_ >= 0) && (this->b_ >= 0))
      {
	if (this->f_ < this->b_ )
	  {
	    this->all_legs_[this->f_] = this->mult_ * 1;
	    this->all_legs_[this->b_] = this->mult_ * -1;
	  }
	else
	  {
	    this->all_legs_[this->b_] = this->mult_ * 1;
	    this->all_legs_[this->f_] = this->mult_ * -1;
	    int f_tmp = this->f_;
	    this->f_ = this->b_;
	    this->b_ = f_tmp;
	    this->mult_ *= -1;
	  }
      }
  else if (this->f_ >= 0)
    {
      this->all_legs_[this->f_] = this->mult_ * 1; 
      this->b_ = -1;
    }
  else if (this->b_ >= 0)
    {
      this->all_legs_[this->b_] = this->mult_ * 1;
      this->f_ = this->b_;
      this->b_ = -1;
      this->mult_ *= -1;
    }
  else
    {
      this->f_ = -1;
      this->b_ = -1;
      this->mult_ = 1;
    }
}

bool
SecPair::operator==(const SecPair& rhs) const
{

  if ( (this->f_ * this->mult_== rhs.mult_ * rhs.f_) &&
       (this->b_ * this->mult_== rhs.mult_ * rhs.b_) )
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
SecPair::operator!=(const SecPair& rhs) const
{
  return !(rhs == *this);
}

SecPair&
SecPair::operator=(const SecPair& rhs)
{

  this->f_ = rhs.f_;
  this->b_ = rhs.b_;
  this->mult_ = rhs.mult_;
  check();

  return *this;    
}

SecPair&
SecPair::operator+=(const SecPair& rhs)
{

  if (rhs.f_ >= 0)
    this->all_legs_[rhs.f_] += rhs.mult_ * 1;
  if (rhs.b_ >= 0)
    this->all_legs_[rhs.b_] += rhs.mult_ * -1;

  auto ie = this->all_legs_.end();
  
  auto itf = std::find(this->all_legs_.begin(), ie, 1);
  auto itb = std::find(this->all_legs_.begin(), ie, -1);
  if (itf != ie)
      this->f_ = std::distance(this->all_legs_.begin(), itf);
  else
      this->f_ = -1;
  if (itb != ie)
      this->b_ = std::distance(this->all_legs_.begin(), itb);
  else
      this->b_ = -1;

  this->mult_ = 1;

  check();
  
  return *this;
}

SecPair
SecPair::operator+(const SecPair& rhs) const
{
  SecPair SecPair_tmp = *this;
  SecPair_tmp += rhs;
  return SecPair_tmp;
}

SecPair&
SecPair::operator*=(int m)
{
  this->mult_ *= m;
  check();
  return *this;
}

SecPair
SecPair::operator*(int m)
{
  SecPair SecPair_tmp(*this);
  SecPair_tmp *= m;
  return SecPair_tmp;
}

void operator*(int m, SecPair& rhs) { rhs*=m; }

SecPair&
SecPair::operator-=(const SecPair& rhs)
{
  SecPair rhs_tmp = SecPair(rhs);
  rhs_tmp *= -1;
  *this = *this + rhs_tmp;

  check();

  return *this;
  
}

SecPair
SecPair::operator-(const SecPair& rhs) const
{
  SecPair SecPair_tmp(*this);
  SecPair_tmp -= rhs; 
  return SecPair_tmp;
}

bool
SecPair::operator<(const SecPair& rhs) const
{
  // Arbitrary
  return ((this->mult_ * this->f_) < (rhs.mult_ * rhs.f_));
}

SecPair
SecPair::abs() const
{
  return SecPair(this->f_, this->b_, 1);
}

bool
SecPair::isPos() const
{
  return (this->abs() == *this);
}

bool
SecPair::isLeg() const
{
    return ((this->f_ >= 0) && (this->b_ < 0));
}

int
SecPair::leg0() const
{
    return this->f_;
}

int
SecPair::leg1() const
{
    return this->b_;
}

std::ostream& operator<<(std::ostream& out, const SecPair& p)
{
  if ((p.f_ < 0) && (p.b_ < 0))
    {
      out << "0";
      return out;
    }
  else if ((p.f_ >= 0) && (p.b_ < 0))
    {
      std::string parity = "l";
      if (p.mult_ == -1)
	parity = "neg_l";
      out << parity << p.f_;
      return out;
    }
  else
    {
      std::string parity = "s";
      if (p.mult_ == -1)
	parity = "neg_s";
      out << parity << p.f_ << "_" << p.b_;
      return out;
    }
}

