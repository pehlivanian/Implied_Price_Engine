#ifndef __IMPLIEDDISTBOOKSUBSCRIBER_HPP__
#define __IMPLIEDDISTBOOKSUBSCRIBER_HPP__

class ImpliedDistBookSubscriber : public DistBookSubscriber
{
  ImpliedDistBookSubscriber(int* k) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    *knot = e.payload_;
  }
  
private:
  int* knot_;
};

#endif
