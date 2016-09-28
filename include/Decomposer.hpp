#ifndef __DECOMPOSER_HPP__
#define __DECOMPOSER_HPP__

#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <ostream>

#include "SecPair.hpp"

namespace std
{
  template<>
  struct hash<SecPair>
  {
    std::size_t operator()(const SecPair& k) const
    {   
      return ( (std::hash<int>()(k.f_)
		^ (std::hash<int>()(k.b_) << 1)) >> 1);
    }
  };
}

template<typename R, typename... Args>
auto memoize(std::function<R(Args...)>&& f)
{
  using F = std::function<R(Args...)>;
  std::map<std::tuple<Args...>,R> cache;
  return ([cache=std::map<std::tuple<Args...>,R>{},
	   f=std::forward<F>(f)](Args&&... args) mutable
    {
      std::tuple<Args...> t(args...);
      if (cache.find(t) == cache.end())
        {
          R r = f(std::forward<Args...>(args)...);
          cache[t] = r;
        }
      return cache[t];
    });   
}

class Decomposer
{
public: 
  using nTmMap = std::unordered_map<int, SecPair>;
  using mTnMap = std::unordered_map<SecPair, int>;
  
  Decomposer() : 
    node_to_market_decomp_(nTmMap()),
    market_decomp_to_node_(mTnMap())
  { 
    node_to_market_decomp_.clear(); 
    market_decomp_to_node_.clear(); 
  }
  void insert(int, SecPair);
  void insert(SecPair, int);
  SecPair market_decomp_from_node(int) const;
  int node_from_market_decomp(const SecPair&) const;
  friend std::ostream& operator<<(std::ostream&, const Decomposer&);
private:
  std::unordered_map<int, SecPair> node_to_market_decomp_;
  std::unordered_map<SecPair, int> market_decomp_to_node_;
};

#endif
