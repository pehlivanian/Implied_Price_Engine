#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#include <iostream>
#include <memory>
#include <string>
#include <regex>
#include <utility>
#include <iterator>

#include "SecPair.hpp"
#include "QuoteSubscriber.hpp"

using Price_Size_Pair = std::pair<int, size_t>;
using QuotePublishEvent = QuoteSubscriber<Price_Size_Pair>::QuotePublishEvent;

struct quote_msg
{
    quote_msg(SecPair&& mkt, QuotePublishEvent&& e) : mkt(std::move(mkt)), e(std::move(e)) {}
    SecPair mkt;
    QuotePublishEvent e;
};


class ClientComponent
{
public:
  ClientComponent(int port, char* IP_addr) : port_(port), IP_addr_(IP_addr) {}
  ClientComponent() = default;
  inline char* fetch() { status_ = fill_buf(); }
  inline char* get_buf() { return recvBuff_; }
  virtual ~ClientComponent();
protected:
  virtual int fill_buf() = 0;
  int port_;
  int status_;
  char* IP_addr_;
  char *recvBuff_;
};

class Client : public ClientComponent
{
public:
  Client(int port, char* IP_addr) : ClientComponent(port, IP_addr) {}
  Client() = default;
  Client(const Client& rhs);
  int fill_buf() override;
private:
  void receive(uint32_t*, int);
};

class PublishDecorator : public Client
{
public:
  PublishDecorator(Client* C) :
    comp_(C) {}
  int fill_buf() override;
    void publish();
private:
  Client* comp_;
};
#endif
