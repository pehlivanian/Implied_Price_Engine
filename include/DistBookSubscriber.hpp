#ifndef __DISTBOOKSUBSCRIBER_HPP__
#define __DISTBOOKSUBSCRIBER_HPP__

#include "Subscriber.hpp"

class DistBookSubscriber : public Subscriber<int>
{
public:
  using BookPublishEvent = Subscriber::PublishEvent;
  
  virtual void update(const BookPublishEvent&) = 0;
  
};

#endif
