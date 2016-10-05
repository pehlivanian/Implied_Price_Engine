#ifndef __IMPLIEDENGINE_HPP__
#define __IMPLIEDENGINE_HPP__

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <limits>

#include "cat1_visitor.hpp"
#include "QuotePublisher.hpp"
#include "ImpliedQuoteSubscriber.hpp"
#include "UserQuoteSubscriber.hpp"
#include "ImpliedBookSubscriber.hpp"
#include "UserBookSubscriber.hpp"
#include "Decomposer.hpp"
#include "Graph.hpp"
#include "Graph_utils.hpp"
#include "MarketGraph.hpp"

template<class T>
class impl;

template<int N>
class ImpliedEngine;

template<int N>
class ImpliedEngine
{
public:
  ImpliedEngine() : p_(std::make_unique<impl<ImpliedEngine<N>>>()) { init_(); }

  inline void publish_bid(const SecPair&, const QuotePublishEvent&);
  inline void publish_ask(const SecPair&, const QuotePublishEvent&);

  void write_user_curve();
  void write_implied_curve();
  void write_dot(int, char*);
  
private:
  void init_();
  void init_decomposition_();
  void init_markets_();
  void init_graphs_();
  void init_subscribers_();
  void init_weights_();
  void write_curve_(std::vector<std::vector<int>>);

  std::unique_ptr<impl<ImpliedEngine<N>>> p_;

};

#include "ImpliedEngine_impl.hpp"

#endif
