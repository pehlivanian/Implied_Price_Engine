#ifndef __IMPLIEDDISTQUOTESUBSCRIBER_HPP__
#define __IMPLIEDDISTQUOTESUBSCRIBER_HPP_

#include <vector>

#include "DistQuoteSubscriber.hpp"
#include "DistBookSubscriber.hpp"
#include "DistBookPublisher.hpp"
#include "MarketGraph.hpp"
#include "cat1_visitor.hpp"

using QuotePublishEvent = DistQuoteSubscriber::QuotePublishEvent;
using BookPublishEvent = DistBookSubscriber::BookPublishEvent;

class ImpliedDistQuoteSubscriber : public DistQuoteSubscriber, public DistBookPublisher
{
public:
  ImpliedDistQuoteSubscriber() : v1_(-1), v2_(-1), G_(new MarketGraph()) { }
  ImpliedDistQuoteSubscriber(int v1, int v2, MarketGraph *G) : 
    v1_(v1),
    v2_(v2),
    G_(G),
    bf_(new cat1_visitor())
  {}
						    
  void update(const QuotePublishEvent& e) override
  {
    G_->updateEdgeWeight(v1_, v2_, e.payload_);
    G_->accept(bf_);
    G_->update_predecessor(bf_->get_predecessor());
    G_->update_distance(bf_->get_distance());
    // The leg always resides at node 1, so notify on it
    auto d = G_->get_distance();
    notify(BookPublishEvent(d[1]));
  }

private:
  int v1_;
  int v2_;
  MarketGraph *G_;
  cat1_visitor *bf_;
};

#endif
