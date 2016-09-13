#ifndef __BOOKPUBLISHER_HPP__
#define __BOOKPUBLISHER_HPP__

#include <list>
#include <memory>

#include "Publisher.hpp"
#include "QuoteSubscriber.hpp"
#include "MarketGraph.hpp"

class BookPublisher : public Publisher<int>
{
public:

  using BookPublishEvent = QuoteSubscriber::QuotePublishEvent;

  BookPublisher() {}
protected:
  // These probably should be weak_ptrs but we don't want
  // to incur the cost of calling .lock() to check for valid pointee.
  // Instead we just assume it is not dangling.
  inline void notify(const BookPublishEvent&) override;
  
};

inline void
BookPublisher::notify(const BookPublishEvent& e)
{
  for(auto& s : subscribers_)
    s->update(e);
}

#endif
