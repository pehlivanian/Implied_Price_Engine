#ifndef __USERQUOTESUBSCRIBER_HPP__
#define __USERQUOTESUBSCRIBER_HPP__

#include "QuoteSubscriber.hpp"
#include "BookPublisher.hpp"

using QuotePublishEvent = QuoteSubscriber::QuotePublishEvent;
using BookPublishEvent = BookSubscriber::BookPublishEvent;

class UserQuoteSubscriber : public QuoteSubscriber, public BookPublisher
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
