#ifndef __BOOKSUBSCRIBER_HPP__
#define __BOOKSUBSCRIBER_HPP__

#include "Subscriber.hpp"

class BookSubscriber : public Subscriber<int>
{
public:
  using BookPublishEvent = Subscriber::PublishEvent;

  virtual void update(const BookPublishEvent&) = 0;
};

#endif
