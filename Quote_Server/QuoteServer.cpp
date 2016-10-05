#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

const int BUF_SIZE = 1024;

int send(int num, int fd)
{
  int converted_number = htonl(num);
 
 write(fd, &converted_number, sizeof(converted_number));
 return 0;
}

int main(int argc, char **argv)
{

  int port = atoi(argv[1]);
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr;
  char* filename = "serveme.txt";
  size_t ret_in;
  
  // Enough size?
  char sendBuff[1025*10];
  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  
  listen(listenfd, 5);
  
  while(true)
    {
      connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

      size_t input_fd = open(filename, O_RDONLY);
      if (input_fd == -1)
	{
	  perror("open");
	  return 2;
	}
      
      ticks = time(NULL);
     
      struct stat file_stat;
      fstat(input_fd, &file_stat);
      send(file_stat.st_size, connfd);
      
      while((ret_in = read(input_fd, &sendBuff, file_stat.st_size)) >0)
	{
	  write(connfd, sendBuff, file_stat.st_size);
	}

      // snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
      // write(connfd, sendBuff, strlen(sendBuff));

      close(connfd);
      sleep(1);
    }
  
  return 0;
}
