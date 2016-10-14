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

struct quote_object
{
  quote_object(char* I, int BP, size_t BS, int AP, size_t AS) :
    Inst(I),
    BidPrice(BP),
    BidSize(BS),
    AskPrice(AP),
    AskSize(AS) {}
  char*     Inst;
  int         BidPrice;
  size_t    BidSize;
  int         AskPrice;
  size_t    AskSize;
};

int receive(uint32_t* num, int fd)
{

  int received_int = 0;
  
  int return_status = read(fd, &received_int, sizeof(received_int));
  if (return_status > 0) {
    *num = ntohl(received_int);
  }
  else {
    return -1;
  }
}

int main(int argc, char *argv[])
{
  int port = atoi(argv[2]);
    int sockfd = 0, n = 0;
    int sizeBuff[1];
    struct sockaddr_in serv_addr; 

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
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
    fprintf(stdout, "SIZE: %d\n", filesize);

    char recvBuff[filesize];
    memset(recvBuff, '0',sizeof(recvBuff));

    int bytes_read = read(sockfd, recvBuff, sizeof(recvBuff));
    if(bytes_read < 0)
    {
      printf("\n Read error \n");
      return 2;
    } 

    fprintf(stdout, "BYTES READ: %d\n", bytes_read);
    fprintf(stdout, "%s", recvBuff);

    return 0;
}
