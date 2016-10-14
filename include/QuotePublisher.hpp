#ifndef __QUOTEPUBLISHER_HPP__
#define __QUOTEPUBLISHER_HPP__

#include <list>
#include <memory>

#include "Publisher.hpp"
#include "QuoteSubscriber.hpp"
#include "MarketGraph.hpp"

using Price_Size_Pair = std::pair<int, size_t>;

class QuotePublisher : public Publisher<Price_Size_Pair>
{
public:

  using QuotePublishEvent = QuoteSubscriber<Price_Size_Pair>::QuotePublishEvent;

  QuotePublisher() {}
protected:
  // These probably should use weak_ptrs but we don't want
  // to incur the cost of calling .lock() to check for valid pointee.
  // Instead we just assume it is not dangling.
  inline void notify(const QuotePublishEvent&) override;
  inline void notify_bid(const QuotePublishEvent&) override;
  inline void notify_ask(const QuotePublishEvent&) override;
};

inline void 
QuotePublisher::notify(const QuotePublishEvent& e)
{
  for (auto& s: subscribers_) {
    try {
      s->update(e);
    }
    catch (...) {
      throw;
    }
  }
}

inline void
QuotePublisher::notify_bid(const QuotePublishEvent& e)
{
  for(auto& s : bid_subscribers_) {
    try {
      s->update_bid(e);
    }
    catch (...) {
      throw;
    }
  }
}

inline void
QuotePublisher::notify_ask(const QuotePublishEvent& e)
{
  for(auto& s : ask_subscribers_) {
      try {
          s->update_ask(e);
      }
      catch (...) {
          throw;
      }
  }
}

#endif
