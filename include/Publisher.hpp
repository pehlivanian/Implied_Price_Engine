#ifndef __PUBLISHER_HPP__
#define __PUBLISHER_HPP__

#include "Subscriber.hpp"

template<typename T>
class Publisher
{
public:

  using PublishEvent = typename Subscriber<T>::PublishEvent;

  Publisher() = default;
  inline void attach(Subscriber<T>* s)                                                { add_subscriber(s); }
  inline void attach(std::shared_ptr<Subscriber<T>> s)                      { add_subscriber(s); }
  void nudge(const PublishEvent& e)                                                 { notify(e); }
  size_t num_subscribers() const                                                         { return subscribers_.size(); }
  std::list<std::shared_ptr<Subscriber<T>>> get_subscribers() const  { return subscribers_; }
private:
  virtual void notify(const PublishEvent&) = 0;
  void add_subscriber(Subscriber<T>*);
  void add_subscriber(std::shared_ptr<Subscriber<T>>);

protected:  
  std::list<std::shared_ptr<Subscriber<T>>> subscribers_;
};

template<typename T>
void 
Publisher<T>::add_subscriber(Subscriber<T>* S)
{
  subscribers_.push_back(std::shared_ptr<Subscriber<T>>(S));
}

template<typename T>
void
Publisher<T>::add_subscriber(std::shared_ptr<Subscriber<T>> S)
{
  subscribers_.push_back(S);
}

#endif
