#ifndef __QUOTE_SUBSCRIBER_HPP__
#define __QUOTE_SUBSCRIBER_HPP__

#include "Subscriber.hpp"

using Price_Size_Pair = std::pair<int, size_t>;

template<typename T>
class QuoteSubscriber : public Subscriber<T>
{
public:
  using QuotePublishEvent = typename Subscriber<T>::PublishEvent;
  virtual ~QuoteSubscriber() = default;

  virtual void update(const QuotePublishEvent&) = 0;
  virtual void update_bid(const QuotePublishEvent&) = 0;
  virtual void update_ask(const QuotePublishEvent&) = 0;
};


#endif
