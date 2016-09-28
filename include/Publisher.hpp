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
  inline void attach_bid(Subscriber<T>* s)			         { add_bid_subscriber(s); }
  inline void attach_ask(Subscriber<T>* s)		         { add_ask_subscriber(s); }
  inline void attach(std::shared_ptr<Subscriber<T>> s)                      { add_subscriber(s); }
  inline void attach_bid(std::shared_ptr<Subscriber<T>> s)	         { add_bid_subscriber(s); }
  inline void attach_ask(std::shared_ptr<Subscriber<T>> s)	         { add_ask_subscriber(s); }
  void nudge(const PublishEvent& e)                                                 { notify(e); }
  void nudge_bid(const PublishEvent& e)			         { notify_bid(e); }
  void nudge_ask(const PublishEvent& e)			         { notify_ask(e); }
  size_t num_subscribers() const                                                         { return subscribers_.size(); }
  std::list<std::shared_ptr<Subscriber<T>>> get_subscribers() const  { return subscribers_; }
private:
  virtual void notify(const PublishEvent&) = 0;
  virtual void notify_bid(const PublishEvent&) = 0;
  virtual void notify_ask(const PublishEvent&) = 0;
  void add_subscriber(Subscriber<T>*);
  void add_bid_subscriber(Subscriber<T>*);
  void add_ask_subscriber(Subscriber<T>*);
  void add_bid_subscriber(std::shared_ptr<Subscriber<T>>);
  void add_ask_subscriber(std::shared_ptr<Subscriber<T>>);
  void add_subscriber(std::shared_ptr<Subscriber<T>>);

protected:  
  std::list<std::shared_ptr<Subscriber<T>>> subscribers_;
  std::list<std::shared_ptr<Subscriber<T>>> bid_subscribers_;
  std::list<std::shared_ptr<Subscriber<T>>> ask_subscribers_;
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

template<typename T>
void
Publisher<T>::add_bid_subscriber(Subscriber<T>* S)
{
  bid_subscribers_.push_back(std::shared_ptr<Subscriber<T>>(S));
}

template<typename T>
void
Publisher<T>::add_ask_subscriber(Subscriber<T>* S)
{
  ask_subscribers_.push_back(std::shared_ptr<Subscriber<T>>(S));
}

template<typename T>
void
Publisher<T>::add_bid_subscriber(std::shared_ptr<Subscriber<T>> S)
{
  bid_subscribers_.push_back(S);
}

template<typename T>
void
Publisher<T>::add_ask_subscriber(std::shared_ptr<Subscriber<T>> S)
{
  ask_subscribers_.push_back(S);
}

#endif
