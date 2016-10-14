#ifndef __IMPLIEDBOOKSUBSCRIBER_HPP__
#define __IMPLIEDBOOKSUBSCRIBER_HPP__

#include <vector>

#include "BookSubscriber.hpp"
#include "BookPublisher.hpp"

using Price_Size_Pair = std::pair<int, size_t>;
using BookPublishEvent = BookSubscriber<Price_Size_Pair>::BookPublishEvent ;

class ImpliedBookSubscriber : public BookSubscriber<Price_Size_Pair>
{
public:
  ImpliedBookSubscriber(Price_Size_Pair *k ) : knot_(k) {}

  void update(const BookPublishEvent& e) override
  {
    ; //nop
  }

  void update_bid(const BookPublishEvent& e) override
  {
    // NOTE: There are 2 assumptions we can make for the
    // format of the incoming quotes, informally
    // UPDATE and DELTA mode.
    // In UPDATE mode, the last update is taken to be a
    // "snapshot" of the book and the price represents
    // top-of-book.
    // In DELTA mode, the last update is taken to be an
    // ADD-type message, and the top-of-book price is updated
    // if an improvement is seen.
    //
    // We will for the most part assume the quotes arrive in
    // UPDATE mode

    // DELTA mode
    // if ((e.payload_).first > (*knot_).first)
    //   *knot_ = e.payload_;
    // UPDATE mode
    *knot_ = e.payload_;
  }

  void update_ask(const BookPublishEvent& e) override
  {
    // DELTA mode
    // if ((e.payload_).first < (*knot_).first)
    //   *knot_ = e.payload_;
    // UPDATE mode
    *knot_ = e.payload_;
  }
private:
  Price_Size_Pair* knot_;
};

#endif
