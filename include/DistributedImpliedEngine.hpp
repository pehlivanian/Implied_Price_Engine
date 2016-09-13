#ifndef __DISTRIBUTEDIMPLIEDENGINE_HPP__
#define __DISTRIBUTEDIMPLIEDENGINE_HPP__

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <limits>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "cat1_visitor.hpp"
#include "QuotePublisher.hpp"
#include "ImpliedDistQuoteSubscriber.hpp"
#include "UserDistQuoteSubscriber.hpp"
#include "ImpliedDistBookSubscriber.hpp"
#include "UserDistBookSubscriber.hpp"
#include "Decomposer.hpp"
#include "Graph.hpp"
#include "Graph_utils.hpp"
#include "MarketGraph.hpp"

template<class T>
class impl;

template<int N>
class DistributedmpliedEngine
{
public:
  DistributedImpliedEngine() : p_(std::make_unique<impl<DistributedImpliedEngine<N>>>()) { init_(); }
private:
  void init_();
  void init_decomposition_();
  void init_markets_();
  void init_graphs_();
  void init_subscribers_();
  void init_weights_();

  inline void publish_(size_t, const SecPair&, const QuotePublishEvent&);
  inline void publish_bid_(const SecPair&, const QuotePublishEvent&);
  inline void publish_ask_(const SecPair&, const QuotePublishEvent&);

  std::unique_ptr<impl<ImpliedEngine<N>>> p_;
};

#include "DistributedImpliedEngine.hpp"

#endif
