#ifndef __DISTQUOTEPUBLISHER_HPP__
#define __DISTQUOTEPUBLISHER_HPP__

#include <list>
#include <memory>
#include <thread>
#include <condition_variable>

#include "DistQuoteSubscriber.hpp"
#include "MarketGraph.hpp"

class DistQuotePublisher : public Publisher<int>
{
public:

  using QuotePublishEvent = DistQuoteSubscriber::QuotePublishEvent;

  DistQuotePublisher() {}
protected:
  // These probably should be weak_ptrs but we don't want
  // to incur the cost of calling .lock() to check for valid pointee.
  // Instead we just assume it is not dangling.
  std::list<std::shared_ptr<DistQuoteSubscriber>> subscribers_;
  intline void notify(const QuotePublishEvent&);

};

inline void
DistQuotePublisher:notify(const QuotePublishEvent& e)
{
  for (auto& s: subscribers_)
    s->update(e);
}

#endif
