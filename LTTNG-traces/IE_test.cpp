//
// Created by charles on 10/25/16.
//

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <iterator>
#include <thread>
#include <chrono>

#include "ImpliedEngine.hpp"
#include "ImpliedServer.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <iterator>
#include <unistd.h>

#include "ImpliedEngine.hpp"
#include "ImpliedServer.hpp"
#include "Client.hpp"

#include "IE-lttng.hpp"

void run_me(size_t n)
{
    tracepoint(IE_test, tracing_IE, n);
    ImpliedServer<6>* IS = new ImpliedServer<6>;
    IS->process();
}

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
int main(int argc, char **argv)
{

    // Client* C = new Client(8080, "127.0.0.1");
    // char* file_buf = C->get();
    // std::cout << std::string(file_buf) << "\n\n";

    // using IS_n = ImpliedServer<6>;
    // IS_n* IS = new IS_n;

    // std::cout << IS->get_buf() << "\n";
    // IS->process();

    // IS->write_user_curve();
    // IS->write_implied_curve();
    // IS->write_merged_curve();

  size_t n = 14;
    run_me(n);
    std::this_thread::sleep_for(std::chrono::seconds(30));

    return 0;
}
