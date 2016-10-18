#include "Decomposer.hpp"

void
Decomposer::insert(int k, SecPair p)
{
  std::pair<int, SecPair> pr = std::make_pair(k, p);
  node_to_market_decomp_.insert(pr);
}

void Decomposer::insert(SecPair k, int n)
{
  std::pair<SecPair, int> pr = std::make_pair(k, n);
  market_decomp_to_node_.insert(std::make_pair(k, n));
}

SecPair
Decomposer::market_decomp_from_node(int n) const
{
  auto f = node_to_market_decomp_.find(n);
  if (f != node_to_market_decomp_.end())
    return f->second;
  else
    return SecPair(-1, -1, 1);
}

int
Decomposer::node_from_market_decomp(const SecPair& p) const
{
  auto f = market_decomp_to_node_.find(p);
  if (f != market_decomp_to_node_.end())
    {
      return f->second;
    }
  else
    return -1;
}

std::ostream& operator<<(std::ostream& out, const Decomposer& d)
{
  auto ib = d.node_to_market_decomp_.begin();
  auto ie = d.node_to_market_decomp_.end();
  while( ib != ie)
    {
      out << ib-> first << " : " << ib->second << "\n";
      ++ib;
    }
  return out;
}

