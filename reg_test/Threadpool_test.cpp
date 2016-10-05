#include "Threadpool_test.hpp"

template<int N>
struct bid_publisher_wrapper
{
  bid_publisher_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) : 
    sec_pair_(s),
    p_(p),
    IE_(IE)
  {}

  int  operator()()
  {
    IE_->publish_bid(sec_pair_, p_);
    return 0;
  }

  SecPair sec_pair_;
  QuotePublishEvent p_;
  ImpliedEngine<N>* IE_;
};

template<int N>
struct ask_publisher_wrapper
{
  ask_publisher_wrapper(const SecPair& s, const QuotePublishEvent& p, ImpliedEngine<N>* IE) : 
    sec_pair_(s),
    p_(p),
    IE_(IE)
  {}

  int operator()()
  {
    IE_->publish_ask(sec_pair_, p_);
    return 0;
  }

  SecPair sec_pair_;
  QuotePublishEvent p_;
  ImpliedEngine<N>* IE_;
};

int main(int argc, char **argv)
{
  
    using IE6 = ImpliedEngine<6>;    
    using bid_p = bid_publisher_wrapper<6>;
    using ask_p = ask_publisher_wrapper<6>;
    IE6* IE = new IE6;

    std::vector<std::future<int> > bid_futures(6);
    std::vector<std::future<int>> ask_futures(6);
    std::vector<int> bid_results(6);
    std::vector<int> ask_results(6);

    threadpool pool;

    for(size_t i=0; i<6; ++i)
      {
	bid_results[i] = pool.submit(bid_p(SecPair(i, -1, 1), QuotePublishEvent(4504 + i*100), IE)).get();
      }
    
    for(size_t i=0; i<6; ++i)
      {
	ask_results[i] = pool.submit(ask_p(SecPair(i, -1, 1), QuotePublishEvent(4514 + i*100), IE)).get();
      }

    int r1 = pool.submit(ask_p( SecPair(1, -1, 1), QuotePublishEvent(4610), IE)).get();
    int r2  =pool.submit(ask_p( SecPair(0, 3,  1), QuotePublishEvent(-300), IE)).get();
    int r3 = pool.submit(bid_p( SecPair(1, 3, 1), QuotePublishEvent(-200), IE)).get();

    int r4 = pool.submit(bid_p( SecPair(0, 2, 1), QuotePublishEvent(-200), IE)).get();
    int r5  =pool.submit(bid_p( SecPair(2, 3,  1), QuotePublishEvent(-100), IE)).get();
    int r6 = pool.submit(bid_p( SecPair(3, -1, 1), QuotePublishEvent(4805), IE)).get();


    IE->write_user_curve();
    IE->write_implied_curve();

  return 0;
}





