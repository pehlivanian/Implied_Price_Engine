#ifndef __QUOTEPUBLISHER_HPP__
#define __QUOTEPUBLISHER_HPP__

#include <list>
#include <memory>

#include "Publisher.hpp"
#include "QuoteSubscriber.hpp"
#include "MarketGraph.hpp"

class QuotePublisher : public Publisher<int>
{
public:

  using QuotePublishEvent = QuoteSubscriber::QuotePublishEvent;

  QuotePublisher() {}
protected:
  // These probably should be weak_ptrs but we don't want
  // to incur the cost of calling .lock() to check for valid pointee.
  // Instead we just assume it is not dangling.
  inline void notify(const QuotePublishEvent&) override;

};

inline void 
QuotePublisher::notify(const QuotePublishEvent& e)
{
  for (auto& s: subscribers_)
    s->update(e);
}

#endif
