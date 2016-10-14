#ifndef __USERQUOTESUBSCRIBER_HPP__
#define __USERQUOTESUBSCRIBER_HPP__

#include "QuoteSubscriber.hpp"
#include "BookPublisher.hpp"

using Price_Size_Pair = std::pair<int, size_t>;
using QuotePublishEvent = QuoteSubscriber<Price_Size_Pair>::QuotePublishEvent;
using BookPublishEvent = BookSubscriber<Price_Size_Pair>::BookPublishEvent;

class UserQuoteSubscriber : public QuoteSubscriber<Price_Size_Pair>, public BookPublisher
{
public:
  UserQuoteSubscriber()  = default;
  void update(const QuotePublishEvent& e ) override
  {
    notify(BookPublishEvent(e.payload_));
  }
  void update_bid(const QuotePublishEvent& e) override
  {
    notify_bid(BookPublishEvent(e.payload_));
  }
  void update_ask(const QuotePublishEvent& e) override
  {
    notify_ask(BookPublishEvent(e.payload_));
  }
 
};

// No specializations needed

#endif
