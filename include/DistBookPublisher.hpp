#ifndef __DISTBOOKPUBLISHER_HPP__
#define __DISTBOOKPUBLISHER_HPP__

#include <list>
#include <memory>

#include "Publisher.hpp"
#include "DistQuoteSubscrier.hpp"
#include "MarketGraph.hpp"

class DistBookPublisher : public Publisher<int>
{
public:
  using BookPublishEvent = DistQuoteSubscriber::QuotePublishEvent;

  DistBookPublisher() {}
protected:
  // These probably should be weak_ptrs but we don't want
  // to incur the cost of calling .lock() to check for valid pointee.
  // instead we just assume it is not dangling.
  inline void notify(const BookPublishEvent&) override;
};

inline void
DistBookPublisher::notify(const BookPublishEvent& e)
{
  for(auto& s : subscribers_)
    s->update(e);
}

#endif
