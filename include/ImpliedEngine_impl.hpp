#ifndef __IMPLIEDENGINE_IMPL_HPP__
#define __IMPLIEDENGINE_IMPL_HPP__

// XXX
// Temporary; used for display purposes only in .dot output
const int large_int =   200000;
const int small_int =  100000;

template<typename T>
struct impl
{
  T* backpointer;
};

template<typename S, template <typename> class T>
struct impl_ST
{
  T<S>* backpointer;
};

template<typename S, typename U, template <typename, typename> class T>
struct impl_STU
{
  T<S,U>* backpointer;
};

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
	   uPrice_(2),
	   uSize_(2),
	   iPrice_(2),
	   iSize_(2),
	   G_(n_),
	   quote_publishers_(2),
	   Decomposer_(std::make_unique<Decomposer>())

  {
    for(size_t i=0; i<2; ++i)
      uPrice_[i] = std::vector<int>(n_, (i == 0) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max());
    for(size_t i=0; i<2; ++i)
      uSize_[i] = std::vector<size_t>(n_,0);
    
    for(size_t i=0; i<2; ++i)
      iPrice_[i] = std::vector<int>(n_, (i == 0) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max());
    for(size_t i=0; i<2; ++i)
      iSize_[i] = std::vector<size_t>(n_,0); 

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
  
  std::vector<std::vector<int>>    uPrice_;
  std::vector<std::vector<size_t>> uSize_;
  std::vector<std::vector<int>>    iPrice_;
  std::vector<std::vector<size_t>> iSize_;

  std::vector<MarketGraph*> G_;

  std::vector<std::vector<QuotePublisher>>      quote_publishers_;
  std::unique_ptr<Decomposer>      Decomposer_;
  std::list<SecPair>               all_markets_;

};

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
	  (p_->G_)[i]->addEdge(0,j,0);
	  (p_->G_)[i]->addEdge(j,0,0);
	  (p_->G_)[i]->addEdge(j,1,0);
	  (p_->G_)[i]->addEdge(1,j,0);
	}

      for(size_t j=2; j<=nl; ++j)
	{
	  for(size_t k=j+1; k<=nl; ++k)
	    {
	      (p_->G_)[i]->addEdge(j, k, 0);
	      (p_->G_)[i]->addEdge(k, j, 0);
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
      UBSub UBSbid = std::make_shared<UserBookSubscriber>(&(p_->uPrice_[0][i]));
      UQSbid->attach_bid(UBSbid);
      (p_->quote_publishers_)[0][(p_->Decomposer_)->node_from_market_decomp(mkt)].attach_bid(UQSbid);

      UQSub UQSask = std::make_shared<UserQuoteSubscriber>();
      UBSub UBSask = std::make_shared<UserBookSubscriber>(&(p_->uPrice_[1][i]));
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

	      IQSub IQSubscriber = std::make_shared<ImpliedQuoteSubscriber>(v1, v2, (p_->G_)[i], i);
	      IBSub IBbidSubscriber  = std::make_shared<ImpliedBookSubscriber>(&(p_->iPrice_[0][i]));
	      IBSub IBaskSubscriber = std::make_shared<ImpliedBookSubscriber>(&(p_->iPrice_[1][i]));
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
		  (p_->G_)[i]->updateEdgeWeight(v1, v2, large_int);
		}
	      else
		{
		  (p_->G_)[i]->updateEdgeWeight(v1, v2, small_int);
		}
	      ++eb;
	    }
	}
    }
}

template<int N>
inline void 
ImpliedEngine<N>::publish_(size_t a, const SecPair& mkt, const QuotePublishEvent& pe)
{
  (p_->quote_publishers_)[a][(p_->Decomposer_)->node_from_market_decomp(mkt)].nudge(pe);
}

template<int N>
inline void 
ImpliedEngine<N>::publish_bid_(const SecPair& mkt, const QuotePublishEvent& pe)
{
  (p_->quote_publishers_)[0][(p_->Decomposer_)->node_from_market_decomp(mkt)].nudge_bid(pe);
}

template<int N>
inline void 
ImpliedEngine<N>::publish_ask_(const SecPair& mkt, const QuotePublishEvent& pe)
{
  (p_->quote_publishers_)[1][(p_->Decomposer_)->node_from_market_decomp(mkt)].nudge_ask(pe);
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

  // Write out one at random
  graph_utils::toDot((p_->G_)[0], std::string("./test_before.dot"));

  // Test publishing mechanism
  publish_bid_(SecPair(0, -1, 1), QuotePublishEvent(4504));
  publish_bid_(SecPair(1, -1, 1), QuotePublishEvent(4604));
  publish_bid_(SecPair(2, -1, 1), QuotePublishEvent(4704));
  publish_bid_(SecPair(3, -1, 1), QuotePublishEvent(4804));
  publish_bid_(SecPair(4, -1, 1), QuotePublishEvent(4904));
  publish_bid_(SecPair(5, -1, 1), QuotePublishEvent(5004));

  publish_ask_(SecPair(0, -1, 1), QuotePublishEvent(4514));
  publish_ask_(SecPair(1, -1, 1), QuotePublishEvent(4614));
  publish_ask_(SecPair(2, -1, 1), QuotePublishEvent(4714));
  publish_ask_(SecPair(3, -1, 1), QuotePublishEvent(4814));
  publish_ask_(SecPair(4, -1, 1), QuotePublishEvent(4914));
  publish_ask_(SecPair(5, -1, 1), QuotePublishEvent(5014));

#if 1
  publish_ask_(SecPair(1, -1, 1), QuotePublishEvent(4610));
  publish_ask_(SecPair(0, 3,  1), QuotePublishEvent(-300));
  publish_bid_(SecPair(1, 3, 1), QuotePublishEvent(-200));

  publish_bid_(SecPair(0,2,1), QuotePublishEvent(-200));
  publish_bid_(SecPair(2,3,1), QuotePublishEvent(-100));
  publish_bid_(SecPair(3,-1,1), QuotePublishEvent(4805));
  
#endif

#if 1
  std::cout << "USER PRICES:\n";
  std::for_each((p_->uPrice_[1]).begin(), (p_->uPrice_[1]).end(),
		[](int a){std::cout << a << " : "; });
  std::cout << "\n";
  std::for_each((p_->uPrice_[0]).begin(), (p_->uPrice_[0]).end(),
		[](int a){std::cout << a << " : "; });
  std::cout << "\n\n";

  std::cout << "IMPLIED PRICES:\n";
  std::for_each((p_->iPrice_[1]).begin(), (p_->iPrice_[1]).end(),
		[](int a){std::cout << a << " : "; });
  std::cout << "\n";
  std::for_each((p_->iPrice_[0]).begin(), (p_->iPrice_[0]).end(),
		[](int a){std::cout << a << " : "; });
  std::cout<< "\n";
#endif
  // Write out again
  graph_utils::toDot((p_->G_)[0], std::string("./test_after.dot"), (p_->G_)[0]->get_distance(0), 
		     (p_->G_)[0]->get_predecessor(0));
}

#endif
