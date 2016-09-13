#ifndef __IMPLIEDBOOKSUBSCRIBER_HPP__
#define __IMPLIEDBOOKSUBSCRIBER_HPP__

#include <vector>

#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"

using BookPublishEvent = BookSubscriber::BookPublishEvent;

class ImpliedBookSubscriber : public BookSubscriber
{
public:
  ImpliedBookSubscriber(int* k) : knot_(k) {}
  
  void update(const BookPublishEvent& e) override
  {
    std::cerr << "ImpliedBookSubscriber(): " << e.payload_ << "\n";
    *knot_ = e.payload_;
  }
private:
  int* knot_;
};

#endif
