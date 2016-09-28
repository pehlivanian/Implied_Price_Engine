#ifndef __USERBOOKSUBSCRIBER_HPP__
#define __USERBOOKSUBSCRIBER_HPP__

#include <vector>

#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"

using BookPublishEvent = BookSubscriber::BookPublishEvent;

class UserBookSubscriber : public BookSubscriber
{
public:
  UserBookSubscriber(int* k) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    ; // nop
  }

  void update_bid(const BookPublishEvent& e) override
  {
    int p = e.payload_;
    if (p > *knot_)
      *knot_ = p;
  }

  void update_ask(const BookPublishEvent& e) override
  {
    int p = e.payload_;
    if (p < *knot_)
      *knot_ = p;
  }
  int* knot_;
};


#endif
