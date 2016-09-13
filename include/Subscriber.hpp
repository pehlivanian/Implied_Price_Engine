#ifndef __SUBSCRIBER_HPP__
#define __SUBSCRIBER_HPP__

template<typename T>
class Subscriber
{
public:
  class PublishEvent
  {
  public:
    PublishEvent(T t) : payload_(t) {}
    T payload_;
  };
  
  virtual void update(const PublishEvent&) = 0;
};

#endif
