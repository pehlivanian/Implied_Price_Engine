#ifndef __IMPLIEDBOOKSUBSCRIBER_HPP__
#define __IMPLIEDBOOKSUBSCRIBER_HPP__

#include <vector>

#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"

using BookPublishEvent = BookSubscriber::BookPublishEvent;

class ImpliedBookSubscriber : public BookSubscriber
{
public:
  ImpliedBookSubscriber(int *k ) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    ; //nop
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
private:
  int* knot_;
};

#endif
