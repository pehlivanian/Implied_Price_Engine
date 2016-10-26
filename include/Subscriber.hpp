#ifndef __SUBSCRIBER_HPP__
#define __SUBSCRIBER_HPP__

#include <algorithm>

template<typename T>
class Subscriber
{
public:
  class PublishEvent
  {
  public:
      PublishEvent(T t) : payload_(t) {}
    virtual ~PublishEvent() = default;
    PublishEvent(const PublishEvent& P) : payload_(P.payload_) {}
    PublishEvent operator-() const { return PublishEvent(-payload_); }
    T payload_;
  };
  
  virtual void update(const PublishEvent&) = 0;
  virtual void update_bid(const PublishEvent&) = 0;
  virtual void update_ask(const PublishEvent&) = 0;
};

#endif
