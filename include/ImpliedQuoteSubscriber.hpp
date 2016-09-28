#ifndef __IMPLIEDQUOTESUBSCRIBER_HPP__
#define __IMPLIEDQUOTESUBSCRIBER_HPP__

#include <vector>
#include <string>
#include <exception>

#include "QuoteSubscriber.hpp"
#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"
#include "MarketGraph.hpp"
#include "cat1_visitor.hpp"

using QuotePublishEvent = QuoteSubscriber::QuotePublishEvent;
using BookPublishEvent = BookSubscriber::BookPublishEvent;

class ImpliedQuoteSubscriber : public QuoteSubscriber, public BookPublisher
{
public:
  ImpliedQuoteSubscriber() : v1_(-1), v2_(-1), G_(new MarketGraph()) {}
  ImpliedQuoteSubscriber(int v1, int v2, MarketGraph* G, int leg) :
    v1_(v1),
    v2_(v2),
    G_(G),
    ask_visitor_(new cat1_visitor(0)),
    bid_visitor_(new cat1_visitor(1)),
    leg_(leg)
  {}
  ~ImpliedQuoteSubscriber() = default;

  void update(const QuotePublishEvent& e ) override;
  void update_bid(const QuotePublishEvent& e) override;
  void update_ask(const QuotePublishEvent& e) override;

private:
  int v1_;
  int v2_;
  MarketGraph *G_;
  cat1_visitor* ask_visitor_;
  cat1_visitor* bid_visitor_;
  cat1_visitor *bf_;
  int leg_;  
};

#endif
