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
#include "impl.hpp"

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

using SERIALIZER_W = boost::lock_guard<boost::shared_mutex>;
using SERIALIZER_R = boost::shared_lock<boost::shared_mutex>;

#define SERIALIZE_WRITES SERIALIZER_W lg(mut_);
#define SERIALIZE_READS SERIALIZER_R ls(mut_);

using Price_Size_Pair = std::pair<int, size_t>;

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
    void write_merged_curve();
  void write_dot(int, char*);
    void write_dot(int, char*, const std::vector<int>&, const std::vector<int>&);

    int get_num_legs() const                        { return  p_->n_; }
    Price_Size_Pair get_bid(int leg) const          { return merge_quote_bid_(leg); }
    Price_Size_Pair get_user_bid(int leg) const     { return (p_->uQuote_)[0][leg]; }
    Price_Size_Pair get_implied_bid(int leg) const  { return (p_->iQuote_)[0][leg]; }
    Price_Size_Pair get_ask(int leg) const          { return merge_quote_ask_(leg); }
    Price_Size_Pair get_user_ask(int leg) const     { return (p_->uQuote_)[1][leg]; }
    Price_Size_Pair get_implied_ask(int leg) const  { return (p_->iQuote_)[1][leg]; }
    std::vector<MarketGraph*> get_Graphs() { return p_->G_; }
    MarketGraph* get_Graph(int i) { return p_->G[i];}

private:
  void init_();
  void init_decomposition_();
  void init_markets_();
  void init_graphs_();
  void init_subscribers_();
  void init_weights_();
    Price_Size_Pair merge_quote_bid_(int leg);
    Price_Size_Pair merge_quote_ask_(int leg);
  void write_curve_(const std::vector<std::vector<std::pair<int, size_t>>>&);

   mutable boost::shared_mutex mut_;
  std::unique_ptr<impl<ImpliedEngine<N>>> p_;

};

#include "ImpliedEngine_impl.hpp"

#endif
