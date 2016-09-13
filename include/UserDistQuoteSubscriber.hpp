#ifndef __USERDISTQUOTESUBSCRIBER_HPP__
#define __USERDISTQUOTESUBSCRIBER_HPP__

#include "DistQuoteSubscriber.hpp"
#include "DistBookPublisher.hpp"

using QuotePublishEvent = DistQuoteSubscriber::QuotePublishEvent;
usign BookPublishEvent = DistBookSubscriber::BookPublishEvent;

class UserDistQuoteSubscriber : public DistQuoteSubscriber, public DistBookPublisher
{
public:
  UserDistQuoteSubscriber() = default;
  void update(const QuotePublishEvent& e) override
  {
    notify(BookPublishEvent(e.payload_));
  }
};


#endif
