#include "ImpliedEngine_test.hpp"

int main(int argc, char **argv)
{

  // Client* C = new Client(8080, "127.0.0.1");
  // char* file_buf = C->get();
  // std::cout << std::string(file_buf) << "\n\n";

  // quote_msg *qm = new quote_msg(file_buf);

    using IE8 = ImpliedEngine<6>;    
    IE8* IE = new IE8;

    // Test publishing mechanism
    IE->publish_bid(SecPair(0, -1, 1), QuotePublishEvent(4504));
    IE->publish_bid(SecPair(1, -1, 1), QuotePublishEvent(4604));
    IE->publish_bid(SecPair(2, -1, 1), QuotePublishEvent(4704));
    IE->publish_bid(SecPair(3, -1, 1), QuotePublishEvent(4804));
    IE->publish_bid(SecPair(4, -1, 1), QuotePublishEvent(4904));
    IE->publish_bid(SecPair(5, -1, 1), QuotePublishEvent(5004));
    
    IE->publish_ask(SecPair(0, -1, 1), QuotePublishEvent(4514));
    IE->publish_ask(SecPair(1, -1, 1), QuotePublishEvent(4614));
    IE->publish_ask(SecPair(2, -1, 1), QuotePublishEvent(4714));
    IE->publish_ask(SecPair(3, -1, 1), QuotePublishEvent(4814));
    IE->publish_ask(SecPair(4, -1, 1), QuotePublishEvent(4914));
    IE->publish_ask(SecPair(5, -1, 1), QuotePublishEvent(5014));

    IE->publish_ask(SecPair(1, -1, 1), QuotePublishEvent(4610));
    IE->publish_ask(SecPair(0, 3,  1), QuotePublishEvent(-300));
    IE->publish_bid(SecPair(1, 3, 1), QuotePublishEvent(-200));
    
    IE->publish_bid(SecPair(0,2,1), QuotePublishEvent(-200));
    IE->publish_bid(SecPair(2,3,1), QuotePublishEvent(-100));
    IE->publish_bid(SecPair(3,-1,1), QuotePublishEvent(4805));

    IE->write_user_curve();
    IE->write_implied_curve();
    
  
}
