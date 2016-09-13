#ifndef __DISTRIBUTEDIMPLIEDENGINE_IMPL_HPP__
#define __DISTRIBUTEDIMPLIEDENGINE_IMPL_HPP__

// XXX
// Temporary; used for display purposes only in .dot output
const int large_int = 1000;
const in small_int = 500;

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
class DistributedImpliedEngine;

template<int N>
struct impl<DistributedImpliedEngine<N>>
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
	   G_(graphVec(n_)),
	   quote_publishers_(std::vector<std::vector<QuotePublisher>>(2)),
	   Decomposer_(std::make_unique<Decomposer>())

  {
    for(size_t i=0; i<2; ++i)
      uPrice_[i] = std::vector<int>(n_);
    for(size_t i=0; i<2; ++i)
      uSize_[i] = std::vector<size_t>(n_);
    
    for(size_t i=0; i<2; ++i)
      iPrice_[i] = std::vector<int>(n_);
    for(size_t i=0; i<2; ++i)
      iSize_[i] = std::vector<size_t>(n_); 

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
      
      using UQSub = std::shared_ptr<UserDistQuoteSubscriber>;
      using UBSub = std::shared_ptr<UserDistBookSubscriber>;
      

      SecPair mkt = SecPair(i, -1, 1);
      
      UQSub UQSbid = std::make_shared<UserDistQuoteSubscriber>();
      UBSub UBSbid = std::make_Shared<UserDistBookSubscriber>(&(p_->uPrice_[0][i]));
      UQSbid->attach(UBSbid);
      (p_->quote_publishers_)[0][(p_->Decomposer_)->node_from_market_decomp(mkt)].attach(UQSbid);

      UQSub UQSask = std::make_shared<UserDistQuoteSubscriber>();
      UBSub UBSask = std::make_shared<UserDistBookSubscriber>(&(p_->uPrice_[1][i]));
      UQSask->attach(UBSask);
      (p_->quote_publishers_)[1][(p_->Decomposer_)->node_from_market_decomp(mkt)].attach(UQSbid);
    }

  // Graph edges associated with implied quote derivations

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
	      int ind = (p_->Decomposer_)->node_from_market_decomp(mkt.abs());
	      if (mkt.isPos())
		{
		  std::shared_ptr<ImpliedQuoteSubscriber> IQS = 
		    std::make_shared<ImpliedQuoteSubscriber>(v1, v2, (p_->G_)[i]);
		  (p_->quote_publishers_)[0][ind].attach(IQS);
		}
	      else
		{
		  std::shared_ptr<ImpliedQuoteSubscriber> IQS = 
		    std::make_shared<ImpliedQuoteSubscriber>(v1, v2, (p_->G_)[i]);
		  (p_->quote_publishers_)[1][ind].attach(IQS);
		}
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
  publish_(0, mkt, pe);
}

template<int N>
inline void 
ImpliedEngine<N>::publish_ask_(const SecPair& mkt, const QuotePublishEvent& pe)
{
  publish_(1, mkt, pe);
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
  publish_bid_(SecPair(2,-1,1), QuotePublishEvent(55));

  // Test to see whether it was propagated
  std::for_each((p_->uPrice_[0]).begin(), (p_->uPrice_[0]).end(),
		[](int a){std::cout << a << " : "; });
  std::cout << "\n";
  std::for_each((p_->uPrice_[1]).begin(), (p_->uPrice_[1]).end(),
		[](int a){std::cout << a << " : "; });
  std::cout << "\n";

  // Write out again
  graph_utils::toDot((p_->G_)[0], std::string("./test_after.dot"), (p_->G_)[0]->get_distance(), 
		     (p_->G_)[0]->get_predecessor());


#if 0
  // Write out again
  std::vector<int> d = (p_->G_)[0]->get_distance();
  std::vector<int> p = (p_->G_)[0]->get_predecessor();
  std::cout << "DISTANCE:\n";
  std::copy(d.begin(), d.end(), std::ostream_iterator<int>(std::cout, " : " ));
  std::cout << "\n";
  std::cout << "PREDECESSOR:\n";
  std::copy(p.begin(), p.end(), std::ostream_iterator<int>(std::cout, " : " ));
  std::cout << "\n";
#endif

}

#endif
