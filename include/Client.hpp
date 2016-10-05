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
#include <iterator>

struct quote_msg
{
  quote_msg(std::string input) : input_(input)
  {
    init_();
  }
  quote_msg(char* input) : input_(input)
  {
    init_();
  }

  std::string input_;
private:
  void init_()
  {
    using namespace std;

    string tok;
    stringstream ss(input_);
    while(getline(ss, tok))
      {
	regex pat(R"([^,]+)");
	for(sregex_iterator p(tok.begin(), tok.end(),pat);
	    p!=sregex_iterator{};
	    ++p)
	  {
	    std::cout << (*p)[0] << " ";
	  }
	std::cout << "\n";
      }
  }
  
};

class ClientComponent
{
public:
  ClientComponent(int port, char* IP_addr) : port_(port), IP_addr_(IP_addr) {}
  ClientComponent() = default;
  inline char* get() { status_ = fill_buf(); return recvBuff_; }
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

class ParseDecorator : public Client
{
public:
  ParseDecorator(Client* C) :
    comp_(C) {}
  int fill_buf() override;

private:
  Client* comp_;
};
#endif
