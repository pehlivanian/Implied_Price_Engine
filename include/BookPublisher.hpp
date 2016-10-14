#ifndef __BOOKPUBLISHER_HPP__
#define __BOOKPUBLISHER_HPP__

#include <list>
#include <memory>

#include "Publisher.hpp"
#include "QuoteSubscriber.hpp"
#include "MarketGraph.hpp"

using Price_Size_Pair = std::pair<int, size_t>;

class BookPublisher : public Publisher<Price_Size_Pair>
{
public:

  using BookPublishEvent = QuoteSubscriber<Price_Size_Pair>::QuotePublishEvent;

  BookPublisher() {}
protected:
  // These probably should use weak_ptrs but we don't want
  // to incur the cost of calling .lock() to check for valid pointee.
  // Instead we just assume it is not dangling.
  inline void notify(const BookPublishEvent&) override;
  inline void notify_bid(const BookPublishEvent&) override;
  inline void notify_ask(const BookPublishEvent&) override;
  
};

inline void
BookPublisher::notify(const BookPublishEvent& e)
{
  for(auto& s : subscribers_)
    s->update(e);
}

inline void
BookPublisher::notify_bid(const BookPublishEvent& e)
{
  for(auto& s : bid_subscribers_)
    s->update_bid(e);
}

inline void
BookPublisher::notify_ask(const BookPublishEvent& e)
{
  for(auto& s : ask_subscribers_)
    s->update_ask(e);
}

#endif
