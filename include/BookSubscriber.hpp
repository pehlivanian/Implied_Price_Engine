#ifndef __BOOKSUBSCRIBER_HPP__
#define __BOOKSUBSCRIBER_HPP__

#include "Subscriber.hpp"

template<typename T>
class BookSubscriber : public Subscriber<T>
{
public:
  using BookPublishEvent = typename Subscriber<T>::PublishEvent;

  virtual void update(const BookPublishEvent&) = 0;
};

#endif
