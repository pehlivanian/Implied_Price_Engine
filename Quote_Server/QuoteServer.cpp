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

size_t file_size(char* filename)
{
  size_t fd = open(filename, O_RDONLY);
  struct stat file_stat;
  fstat(fd, &file_stat);
  close(fd);

  return file_stat.st_size;
}

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
  char* filename = (char *)"serveme.txt";
  size_t ret_in;

  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  
  listen(listenfd, 5);
  
  while(true)
    {
      connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        // Do this everytime in a tight loop so that we
        // can avoid shutdown when underlygin file changes
      size_t f_size = file_size(filename);
      char sendBuff[f_size];
      memset(sendBuff, '0', f_size);

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
