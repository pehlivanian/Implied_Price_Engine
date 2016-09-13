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
    std::cerr << "UserBookSubscriber(): " << e.payload_ << "\n";
    *knot_= e.payload_;
  }

  int* knot_;
};

#endif
