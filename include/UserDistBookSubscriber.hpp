#ifndef __USERDISTBOOKSUBSCRIBER_HPP__
#define __USERDISTBOOKSUBSCRIBER_HPP__

#include "DistBookSubscriber.hpp"
#include "DistBookPublisher.hpp"

using BookPublishEvent = BookSubscriber::BookPublishEvent;

class UserDistBookSubscriber : public DistBookSubscriber
{
public:
  UserDistBookSubscriber(int* k) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    *knot = e.payload_;
  }

  int* knot_;
};


#endif
