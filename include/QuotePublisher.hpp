#ifndef __QUOTEPUBLISHER_HPP__
#define __QUOTEPUBLISHER_HPP__

#include <list>
#include <memory>
#include <thread>
#include <future> // async

#include "Publisher.hpp"
#include "QuoteSubscriber.hpp"
#include "MarketGraph.hpp"
// #include "threadpool.hpp"
// #include "LW_threadpool.hpp"

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
    // LW_threadpool<void> pool_;
};


inline void 
QuotePublisher::notify(const QuotePublishEvent& e)
{
#if 1
    for(auto& sb : subscribers_)
        sb->update(e);

#endif

#if 0
    auto sb = subscribers_.begin();
    auto se = subscribers_.end();
    auto fb = futures_.begin();
    for(; sb!=se; ++sb,++fb)
        pool_.submit([&e,sb](){ (*sb)->update(e); });
#endif
#if 0
    auto sb = subscribers_.begin();
    auto se = subscribers_.end();
    auto fb = futures_.begin();
    for(; sb!=se; ++sb,++fb)
    {
      // (*fb) = std::async([&e](auto s){ s->update(e); return 0; }, *sb);
        (*fb) = pool_.submit([&e, sb](){ (*sb)->update(e); return 0; });
    }
    for(auto& f : futures_)
        f.get();
#endif


}

inline void
QuotePublisher::notify_bid(const QuotePublishEvent& e)
{
#if 1
    for(auto& sb : bid_subscribers_)
        sb->update_bid(e);

#endif
#if 0
    auto sb = bid_subscribers_.begin();
    auto se = bid_subscribers_.end();
    auto fb = bid_futures_.begin();
    for(; sb!=se; ++sb,++fb)
        pool_.submit([&e, sb](){ (*sb)->update_bid(e); });
#endif

#if 0
    auto sb = bid_subscribers_.begin();
    auto se = bid_subscribers_.end();
    auto fb = bid_futures_.begin();
    for(; sb!=se; ++sb,++fb)
    {
      // (*fb) = std::async([&e](auto s){ s->update_bid(e); return 0; }, *sb);
      (*fb) = pool_.submit([&e,sb](){ (*sb)->update_bid(e); return 0; });
    }
    for(auto& f : bid_futures_)
        f.get();
#endif

}

inline void
QuotePublisher::notify_ask(const QuotePublishEvent& e)
{
#if 1
    for(auto& sb : ask_subscribers_)
        sb->update_ask(e);

#endif
#if 0
    auto sb = ask_subscribers_.begin();
    auto se = ask_subscribers_.end();
    auto fb = ask_futures_.begin();
    for(; sb!=se; ++sb,++fb)
        pool_.submit([&e, sb](){ (*sb)->update_ask(e); });
#endif
#if 0
    auto sb = ask_subscribers_.begin();
    auto se = ask_subscribers_.end();
    auto fb = ask_futures_.begin();
    for(; sb!=se; ++sb,++fb)
    {
      // (*fb) = std::async([&e](auto s){ s->update_ask(e); return 0; }, *sb);
        (*fb) = pool_.submit([&e,sb](){ (*sb)->update_ask(e); return 0; });
    }
    for(auto& f : ask_futures_)
        f.get();
#endif

}

#endif
