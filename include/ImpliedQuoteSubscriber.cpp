#include "ImpliedQuoteSubscriber.hpp"

void
ImpliedQuoteSubscriber::update(const QuotePublishEvent& e)
{
  ; // nop
}

void 
ImpliedQuoteSubscriber::update_bid(const QuotePublishEvent& e)
{
    G_->updateEdgeWeight(v1_, v2_, -1 * e.payload_.first, e.payload_.second);
    
    // Ask side first
    try
        { G_->accept(ask_visitor_); }
    catch(std::exception &e)
        { std::cerr << e.what() << "\n"; throw; }

    notify_ask(BookPublishEvent(std::make_pair( ask_visitor_->get_distance(1),
                                                ask_visitor_->get_size(1))));

    // Bid side second
  try
    {
        G_->accept(bid_visitor_);
    }
  catch(std::exception& e)
    {
      std::cerr << e.what() << "\n"; throw;
    }
    notify_bid(BookPublishEvent(std::make_pair( -1 * bid_visitor_->get_distance(0),
                                                bid_visitor_->get_size(0))));
}

void
ImpliedQuoteSubscriber::update_ask(const QuotePublishEvent& e)
{
  G_->updateEdgeWeight(v1_, v2_, e.payload_.first, e.payload_.second);
  
  // Ask side first
  try
  {
      G_->accept(ask_visitor_);
  }
  catch(std::exception &e)
    {
        std::cerr << e.what() << "\n"; throw;
    }
    notify_ask(BookPublishEvent(std::make_pair( ask_visitor_->get_distance(1),
                                                ask_visitor_->get_size(1))));

  // Bid side second
  try
  {
      G_->accept(bid_visitor_);
  }
  catch(std::exception& e)
  {
      std::cerr << e.what() << "\n"; throw;
  }
    notify_bid(BookPublishEvent(std::make_pair( -1 * bid_visitor_->get_distance(0),
                                                bid_visitor_->get_size(0))));
}


