#include "Client.hpp"

ClientComponent::~ClientComponent()
{
  // delete IP_addr_;
  // delete recvBuff_;
}

Client::Client(const Client& rhs)
{
  port_ = rhs.port_;
  status_ = rhs.status_;
  delete IP_addr_;
  delete[] recvBuff_;
  strcpy(IP_addr_, rhs.IP_addr_);
  strcpy(recvBuff_, rhs.recvBuff_);
}

void
Client::receive(uint32_t* num, int fd)
{
  int received_int = 0;
  
  int return_status = read(fd, &received_int, sizeof(received_int));
  if (return_status > 0)
    {
      *num = ntohl(received_int);
    }
}

int
Client::fill_buf()
{
  // fprintf(stderr, "Client::fill_buf()\n");
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr; 

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_); 

    if(inet_pton(AF_INET, IP_addr_, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    uint32_t filesize = 0;
    receive(&filesize,sockfd);
    // fprintf(stderr, "FILE SIZE: %d\n", filesize);

    recvBuff_ = new char[filesize];
    memset(recvBuff_, '0', filesize);

    int bytes_read = read(sockfd, recvBuff_, filesize);
    if(bytes_read < 0)
    {
      printf("\n Read error \n");
      return 2;
    }

    // fprintf(stderr, "BYTES READ: %d\n", bytes_read);
    // fprintf(stderr, "%s", recvBuff_);

    return 0;

}

int
PublishDecorator::fill_buf()
{
    return comp_->fill_buf();
}

#if 0
int
PublishDecorator::fill_buf()
{
  int result = comp_->fill_buf();

    recvBuff_ = comp_->get_buf();
    // printf("PD::Input: %s\n", recvBuff_);

    using namespace std;
    using namespace rapidjson;

    Document document;

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
    string tok;
    stringstream ss(recvBuff_);
    while(getline(ss, tok, '\n'))
    {
        if (document.Parse(tok.c_str()).HasParseError())
        {
            fprintf(stderr, "Parse error!\n");
            return -1;
        };
        assert(document.HasMember("Inst"));
        if (document.HasMember("bid"))
            std::cout << "Inst = " << document["Inst"].GetString() << " BID\n";
        if (document.HasMember("ask"))
            std::cout << "Inst = " << document["Inst"].GetString() << " ASK\n";
#undef QUOTE(A, B)
    }

  return result;
}
#endif
