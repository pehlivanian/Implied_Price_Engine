#ifndef __IMPLIEDENGINE_IMPL_HPP__
#define __IMPLIEDENGINE_IMPL_HPP__

// XXX
// Temporary; used for display purposes only in .dot output
const int large_int =   200000;
const int small_int =  100000;

template<int N>
class ImpliedEngine;

template<int N>
struct impl<ImpliedEngine<N>>
{
  using pVec =      std::vector<std::vector<int>>;
  using sVec =      std::vector<std::vector<size_t>>;
  using pubVec =    std::vector<std::vector<QuotePublisher>>;
  using graphVec =  std::vector<MarketGraph*>;

  impl() :      n_(N),
	   m_( n_*(n_ - 1)/2),
	   uQuote_(2),
	   iQuote_(2),
	   G_(n_),
	   quote_publishers_(2),
	   Decomposer_(std::make_unique<Decomposer>())

  {
    for(size_t i=0; i<2; ++i)
      uQuote_[i] = std::vector<std::pair<int, size_t>>(n_, (i == 0) ? std::make_pair(std::numeric_limits<int>::min(), 0) :
                                        std::make_pair(std::numeric_limits<int>::max(), 0));
    for(size_t i=0; i<2; ++i)
      iQuote_[i] = std::vector<std::pair<int, size_t>>(n_, (i == 0) ? std::make_pair(std::numeric_limits<int>::min(), 0) :
                                        std::make_pair(std::numeric_limits<int>::max(), 0));
    for(size_t i=0; i<2; ++i)
      quote_publishers_[i] = std::vector<QuotePublisher>(m_ + n_);
  }

  ~impl()
  {
    for(size_t i=0; i<G_.size(); ++i)
      delete G_[i];
  }

  int n_;
  int m_;

    using atomic_price = std::atomic<int>;
    using atomic_size = std::atomic<size_t>;

  std::vector<std::vector<std::pair<int, size_t>>>     uQuote_;
  std::vector<std::vector<std::pair<int, size_t>>>     iQuote_;

  std::vector<MarketGraph*> G_;

  std::vector<std::vector<QuotePublisher>>      quote_publishers_;
  std::unique_ptr<Decomposer>                   Decomposer_;
  std::list<SecPair>                            all_markets_;

};

template<int N>
Price_Size_Pair
ImpliedEngine<N>::merge_quote_bid_(int leg)
{
    // SERIALIZE_READS;
    Price_Size_Pair uq = (p_->uQuote_)[0][leg];
    Price_Size_Pair iq = (p_->iQuote_)[0][leg];
    if (uq.first == iq.first)
        return Price_Size_Pair(uq.first, uq.second + iq.second);
    else if (uq.first > iq.first )
        return uq;
    else
        return iq;
}

template<int N>
Price_Size_Pair
ImpliedEngine<N>::merge_quote_ask_(int leg)
{
    // SERIALIZE_READS;
    Price_Size_Pair uq = (p_->uQuote_)[1][leg];
    Price_Size_Pair iq = (p_->iQuote_)[1][leg];
    if (uq.first == iq.first)
        return Price_Size_Pair(uq.first, uq.second + iq.second);
    else if (uq.first < iq.first )
        return uq;
    else
        return iq;
}

template<int N>
void
ImpliedEngine<N>::init_decomposition_()
{
  const size_t nl = p_->n_;

  size_t node_count = -1;

  // Insert legs
  for(size_t i=0; i<nl; ++i)
    {
      SecPair p(i, -1, 1);
      (p_->Decomposer_)->insert(++node_count, p);
      (p_->Decomposer_)->insert(p, node_count);
      (p_->all_markets_).push_back(p);
    }

  // Insert spreads
  for(size_t i=0; i<nl; ++i)
    {
      for(size_t j=i+1; j<nl; ++j)
	{
	  SecPair p(i, j, 1);
	  (p_->Decomposer_)->insert(++node_count, p);
	  (p_->Decomposer_)->insert(p, node_count);
	  (p_->all_markets_).push_back(p);
	}
    }
}

template<int N>
void
ImpliedEngine<N>::init_graphs_()
{
  const size_t nl = p_->n_;  // Number of legs

  for(size_t i=0; i<nl; ++i)
    {
      (p_->G_)[i] = new MarketGraph(nl + 1);

      for(size_t j=2; j<=nl; ++j)
	{
	  (p_->G_)[i]->addEdge(0, j, 0, 0);
	  (p_->G_)[i]->addEdge(j, 0, 0, 0);
	  (p_->G_)[i]->addEdge(j, 1, 0, 0);
	  (p_->G_)[i]->addEdge(1, j, 0, 0);
	}

      for(size_t j=2; j<=nl; ++j)
	{
	  for(size_t k=j+1; k<=nl; ++k)
	    {
	      (p_->G_)[i]->addEdge(j, k, 0, 0);
	      (p_->G_)[i]->addEdge(k, j, 0, 0);
	    }
	}
    }
}

template<int N>
void
ImpliedEngine<N>::init_markets_()
{
  const int nl = p_->n_;  // Number of legs

  for(int i=0; i<nl; ++i)
    {
      int node_num = 0;

      // Add all vertex markets
      (p_->G_)[i]->addVertexProp(node_num++, SecPair(-1, -1, 1));
      (p_->G_)[i]->addVertexProp(node_num++, SecPair(i, -1, 1));

      auto ib = (p_->all_markets_).begin();
      auto ie = (p_->all_markets_).end();
      while(ib != ie)
	{
	  if ((ib->f_ == i) && (ib->b_ >= 0))
	    {
	      (p_->G_)[i]->addVertexProp(node_num++, *ib);
	    }
	  if ((ib->b_ == i) && (ib->f_ >= 0))
	    {
	      SecPair p(ib->f_, ib->b_, -1 * ib->mult_);
	      (p_->G_)[i]->addVertexProp(node_num++, p);
	    }
	  ++ib;
	}

      // Add all edge markets
      for (size_t j=0; j<(p_->G_)[i]->numVertices(); ++j)
	{
	  auto eb = (p_->G_)[i]->begin(j);
	  auto ee = (p_->G_)[i]->end(j);
	  while( eb != ee)
	    {
	      int v1 = j;
	      int v2 = eb->first;
	      auto vp1 = (p_->G_)[i]->vertexProp(v1);
	      auto vp2 = (p_->G_)[i]->vertexProp(v2);
	      auto vdiff = vp2 - vp1;
	      (p_->G_)[i]->addEdgeProp(v1, v2, vdiff);
	      ++eb;
	    }
	}

    }

}

using UQSub = std::shared_ptr<UserQuoteSubscriber>;
using IQSub = std::shared_ptr<ImpliedQuoteSubscriber>;
using UBSub = std::shared_ptr<UserBookSubscriber>;
using IBSub = std::shared_ptr<ImpliedBookSubscriber>;

template<int N>
void
ImpliedEngine<N>::init_subscribers_()
{
  const size_t nl = p_->n_;  // Number of legs

  // User quotes associated with legs; simple pass-through
  // Note: these should be first in line when a user quote update
  // message arrives; first update user quotes, then compute implieds,
  // then merge the two in the book object.
  for(size_t i=0; i<nl; ++i)
    {
      SecPair mkt = SecPair(i, -1, 1);

      UQSub UQSbid = std::make_shared<UserQuoteSubscriber>();
      UBSub UBSbid = std::make_shared<UserBookSubscriber>(&(p_->uQuote_[0][i]));
      UQSbid->attach_bid(UBSbid);
      (p_->quote_publishers_)[0][(p_->Decomposer_)->node_from_market_decomp(mkt)].attach_bid(UQSbid);

      UQSub UQSask = std::make_shared<UserQuoteSubscriber>();
      UBSub UBSask = std::make_shared<UserBookSubscriber>(&(p_->uQuote_[1][i]));
      UQSask->attach_ask(UBSask);
      (p_->quote_publishers_)[1][(p_->Decomposer_)->node_from_market_decomp(mkt)].attach_ask(UQSask);
    }

  // Graph edges associated with implied quote derivations
  // std::unordered_map<SecPair, int> QShash_map;

  for(size_t i=0; i<nl; ++i)
    {
      for(size_t j=0; j<(p_->G_)[i]->numVertices(); ++j)
	{
	  auto eb = (p_->G_)[i]->edge_prop_begin(j);
	  auto ee = (p_->G_)[i]->edge_prop_end(j);

	  while(eb != ee)
	    {
	      int v1 = j, v2 = eb->first;
	      SecPair mkt = eb->second;
	      int ind = (p_->Decomposer_)->node_from_market_decomp(mkt.abs());

	      IQSub IQSubscriber     = std::make_shared<ImpliedQuoteSubscriber>(v1, v2, (p_->G_)[i], i);
	      IBSub IBbidSubscriber  = std::make_shared<ImpliedBookSubscriber>(&(p_->iQuote_[0][i]));
	      IBSub IBaskSubscriber  = std::make_shared<ImpliedBookSubscriber>(&(p_->iQuote_[1][i]));
	      IQSubscriber->attach_bid(IBbidSubscriber);
	      IQSubscriber->attach_ask(IBaskSubscriber);

	      if (mkt.isPos())
		    (p_->quote_publishers_)[1][ind].attach_ask(IQSubscriber);
	      else
		    (p_->quote_publishers_)[0][ind].attach_bid(IQSubscriber);

	      ++eb;
	    }
	}
    }
}

template<int N>
void
ImpliedEngine<N>::init_weights_()
{
  const size_t nl = p_->n_;  // Number of legs

  for(size_t i=0; i<nl; ++i)
    {
      for(size_t j=0; j<(p_->G_)[i]->numVertices(); ++j)
	{
	  auto eb = (p_->G_)[i]->edge_prop_begin(j);
	  auto ee = (p_->G_)[i]->edge_prop_end(j);

	  while(eb != ee)
	    {
	      int v1 = j;
	      int v2 = eb->first;
	      SecPair mkt = eb->second;
	      if (mkt.isPos())
		{
		  (p_->G_)[i]->updateEdgeWeight(v1, v2, large_int, 0);
		}
	      else
		{
		  (p_->G_)[i]->updateEdgeWeight(v1, v2, small_int, 0);
		}
	      ++eb;
	    }
	}
    }
}

template<int N>
inline void
ImpliedEngine<N>::publish_bid(const SecPair& mkt, const QuotePublishEvent& pe)
{
    (p_->quote_publishers_)[0][(p_->Decomposer_)->node_from_market_decomp(mkt)].nudge_bid(pe);
}

template<int N>
inline void
ImpliedEngine<N>::publish_ask(const SecPair& mkt, const QuotePublishEvent& pe)
{
    (p_->quote_publishers_)[1][(p_->Decomposer_)->node_from_market_decomp(mkt)].nudge_ask(pe);
}

template<int N>
void
ImpliedEngine<N>::write_dot(int leg_num, char* filename) const
{
  graph_utils::toDot((p_->G_)[leg_num], std::string(filename));
}

template<int N>
void
ImpliedEngine<N>::write_dot(int leg_num, char* filename, const std::vector<int>& d, const std::vector<int>& p) const
{
    graph_utils::toDot((p_->G_)[leg_num], std::string(filename), d, p);
}

template<int N>
void
ImpliedEngine<N>::write_user_curve() const
{
    std::cout << " : ===========\n";
    std::cout << " : USER PRICES\n";
    std::cout << " : ===========\n";
    write_curve_(p_->uQuote_);
}

template<int N>
void
ImpliedEngine<N>::write_implied_curve() const
{
    std::cout << " : ==============\n";
    std::cout << " : IMPLIED PRICES\n";
    std::cout << " : ==============\n";
    write_curve_(p_->iQuote_);
}

template<int N>
void
ImpliedEngine<N>::write_merged_curve() const
{
    std::cout << " : =============\n";
    std::cout << " : MERGED PRICES\n";
    std::cout << " : =============\n";

    std::vector<std::vector<std::pair<int, size_t>>> mQuote(2);
    mQuote[0].resize((p_->iQuote_[0]).size());
    mQuote[1].resize((p_->iQuote_[0]).size());
    for(size_t i=0; i<mQuote[0].size(); ++i)
    {
        if ( ((p_->uQuote_)[0][i].first) == ((p_->iQuote_)[0][i].first))
        {
            auto node = std::make_pair((p_->uQuote_)[0][i].first, (p_->uQuote_)[0][i].second + (p_->iQuote_)[0][i].second);
            mQuote[0][i] = node;
        }
        else if ( ((p_->uQuote_)[0][i].first) > ((p_->iQuote_)[0][i].first))
            mQuote[0][i] = (p_->uQuote_)[0][i];
        else
            mQuote[0][i] = (p_->iQuote_)[0][i];

        if ( ((p_->uQuote_)[1][i].first) == ((p_->iQuote_)[1][i].first))
        {
            auto node = std::make_pair((p_->uQuote_)[1][i].first, (p_->uQuote_)[1][i].second + (p_->iQuote_)[1][i].second);
            mQuote[1][i] = node;
        }
        else if ( ((p_->uQuote_)[1][i].first) < ((p_->iQuote_)[1][i].first))
            mQuote[1][i] = (p_->uQuote_)[1][i];
        else
            mQuote[1][i] = (p_->iQuote_)[1][i];
    }

    write_curve_(mQuote);
}

template<int N>
void
ImpliedEngine<N>::write_curve_(const std::vector<std::vector<std::pair<int, size_t>>>& quote) const
  {
    int l = 0;
    std::for_each((quote[1]).begin(), (quote[1]).end(),
		  [&l](auto a){
		    std::string s("leg_");
		    s+=std::to_string(l++);
		    std::cout << std::setw(16) << s << " "; });
    std::cout << std::endl;

    l = 0;
    std::for_each((quote[1]).begin(), (quote[1]).end(),
		  [&l](auto a){std::cout << std::setw(15) << "================ "; });
    std::cout << std::endl;

      std::for_each(quote[1].begin(), quote[1].end(),
      [](auto a) {
          std::string s = std::to_string(a.first) + " x (" + std::to_string(a.second) + ")";
          std::cout << std::setw(16) << s << " ";
      });
      std::cout << std::endl;

      std::for_each(quote[0].begin(), quote[0].end(),
      [](auto a) {
          std::string s = std::to_string(a.first) + " x (" + std::to_string(a.second) + ")";
          std::cout << std::setw(16) << s << " ";
      });
      std::cout << std::endl;

      std::for_each((quote[1]).begin(), (quote[1]).end(),
                    [&l](auto a){std::cout << std::setw(15) << "================ "; });
    std::cout << std::endl;
  }

template<int N>
void
ImpliedEngine<N>::write_user_quote(int c, std::ostream& fsu) const
{
    write_quote_(get_user_quote(), c, fsu);
}

template<int N>
void
ImpliedEngine<N>::write_implied_quote(int c, std::ostream& fsi) const
{
    write_quote_(get_implied_quote(), c, fsi);
}

template<int N>
void
ImpliedEngine<N>::write_quote_(const std::vector<std::vector<std::pair<int, size_t>>>& q, int c, std::ostream& fs) const
{
    fs << "ask_price_it" + std::to_string(c);
    for(auto k : std::vector<std::pair<int, size_t>>(q[1].begin(), q[1].end()))
        fs << "," << k.first;
    fs << "\n";
    fs << "ask_size_it" + std::to_string(c);;
    for(auto k : std::vector<std::pair<int, size_t>>(q[1].begin(), q[1].end()))
        fs << "," << k.second;
    fs << "\n";
    fs << "bid_price_it" + std::to_string(c);;
    for(auto k : std::vector<std::pair<int, size_t>>(q[0].begin(), q[0].end()))
        fs << "," << k.first;
    fs << "\n";
    fs << "bid_size_it" + std::to_string(c);;
    for(auto k : std::vector<std::pair<int, size_t>>(q[0].begin(), q[0].end()))
        fs << "," << k.second;
    fs << "\n";
}

template<int N>
void
ImpliedEngine<N>::init_()
{
  init_decomposition_();
  init_graphs_();
  init_markets_();
  init_subscribers_();
  init_weights_();

}

#endif
