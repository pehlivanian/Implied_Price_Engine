#ifndef __QUOTE_SUBSCRIBER_HPP__
#define __QUOTE_SUBSCRIBER_HPP__

#include "Subscriber.hpp"

class QuoteSubscriber : public Subscriber<int>
{
public:
  using QuotePublishEvent = Subscriber::PublishEvent;
  virtual ~QuoteSubscriber() = default;

  virtual void update(const QuotePublishEvent&) = 0;
  virtual void update_bid(const QuotePublishEvent&) = 0;
  virtual void update_ask(const QuotePublishEvent&) = 0;
};


#endif
