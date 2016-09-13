#ifndef __DISTQUOTESUBSCRIBER_HPP__
#define __DISTQUOTESUBSCRIBER_HPP__

#include <thread>
#include <condition_variable>


class DistQuoteSubscriber : public Subscriber<int>
{
public:
  using QuotePublishEvent = Subscriber::PublishEvent;
  
  virtual void update(const QuotePublishEvent&) = 0;
};



#endif
