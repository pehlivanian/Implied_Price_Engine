#include "ImpliedEngine_test.hpp"

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
int main(int argc, char **argv)
{

  // Client* C = new Client(8080, "127.0.0.1");
  // char* file_buf = C->get();
  // std::cout << std::string(file_buf) << "\n\n";

  // quote_msg *qm = new quote_msg(file_buf);

    using IS8 = ImpliedServer<8>;
    IS8* IS = new IS8;

    // std::cout << IS->get_buf() << "\n";
    IS->process();

    IS->write_user_curve();
    IS->write_implied_curve();
    IS->write_merged_curve();

}
