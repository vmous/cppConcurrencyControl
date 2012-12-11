/* File: int_str_client.c */
#include <sys/types.h>                                    /* For sockets */
#include <sys/socket.h>                                   /* For sockets */
#include <netinet/in.h>                          /* For Internet sockets */
#include <netdb.h>                                  /* For gethostbyname */
#include <cstdio>                                            /* For I/O */
#include <cstdlib>                                          /* For exit */
#include <cstring>                         /* For strlen, bzero, bcopy  */

#include <iostream>

#include "xaction_api.h"

int Transfer( unsigned int, unsigned int, double );
int Deposit( unsigned int, double );
int Withdraw( unsigned int, double );
void processXaction();

int port, sock;

int main(int argc, char *argv[])     /* Client with Internet stream sockets  */
{
  //char bufop[256], bufid[256], bufda[256], bufso[256];
  unsigned int serverlen;
  struct sockaddr_in server;
  struct sockaddr *serverptr;
  struct hostent *rem;

  if (argc < 4) {     /* Are server’s host name and port number given?  */
    printf("Please give host name and port number\n");
    exit(1);
  }
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { /* Create socket  */
    perror("socket");
    exit(1);
  }
  if ((rem = gethostbyname(argv[1])) == NULL) { /* Find server address  */
    perror("gethostbyname");
    exit(1);
  }

  port = atoi(argv[2]);              /* Convert port number to integer  */
  server.sin_family = PF_INET;                       /* Internet domain */
  bcopy( (char *)rem->h_addr, (char *)&server.sin_addr, rem->h_length );
  server.sin_port = htons(port); /* Server’s Internet address and port  */
  serverptr = (struct sockaddr *) &server;
  serverlen = sizeof server;
  if (connect(sock, serverptr, serverlen) < 0) { /* Request connection  */
    perror("connect");
    exit(1);
  }
  printf("Requested connection to host %s port %d\n", argv[1], port);

  int stop = 0;
  unsigned int from;
  unsigned int to;
  double amount;

  do {

    switch(atoi(argv[3])) {
      case 0:
        // deposit
        to = (unsigned int)atoi( argv[4] );
        amount = atof( argv[5] );
        stop = Deposit( to, amount );
        break;
      case 1:
        // withdraw
        from = (unsigned int)atoi( argv[4] );
        amount = atof( argv[5] );
        stop = Withdraw( from, amount );
        break;
      case 2:
        // transfer
        from = (unsigned int)atoi( argv[4] );
        to = (unsigned int)atoi( argv[5] );
        amount = atof( argv[6] );
        stop = Transfer( from, to, amount );
        break;
      default:
        stop = 1;
        break;
    }

  } while ( !stop );
  close(sock);
  exit(0);
} // Close socket and exit

int Transfer( unsigned int from, unsigned int to, double amount )
{
  double temp;
  int ret;

  if( ( temp = Read( sock, from ) ) < 0 )
    return 0;
  if( temp < amount ) {
    std::cout << "Insufficient funds" << std::endl;
    Rollback();
    ret = 1;
  }
  else {
    processXaction();  // virtual processing time
    if( ( Write( sock, from, ( temp - amount ) ) ) < 0 )
      return 0;
    if( ( temp = Read( sock, to ) ) < 0 )
      return 0;
    processXaction();  // virtual processing time
    if( ( Write( sock, to, ( temp + amount ) ) ) < 0 )
      return 0;
    Commit();
    std::cout << "Trensfer of " << amount << " Euros from " << from << " to " << to << "completed" << std::endl;
    ret = 1;
  }
  return( ret );
}

int Deposit( unsigned int to, double amount )
{
  double temp;

  if( ( temp = Read( sock, to ) ) < 0 )
    return 0;
  processXaction();  // virtual processing time
  if( ( Write( sock, to, ( temp + amount ) ) ) < 0 )
    return 0;
  Commit();

  std::cout << "Deposit of " << amount << " Euros to " << to << " completed" << std::endl;
  return 1;
}

int Withdraw( unsigned int from, double amount )
{
  double temp;
  int ret;

  if( ( temp = Read( sock, from ) ) < 0 )
    return 0;
  if( temp < amount ) {
    std::cout << "Insufficient funds" << std::endl;
    Rollback();
    ret = 1;
  }
  else {
    processXaction();  // virtual processing time
    if( ( Write( sock, from, ( temp - amount ) ) ) < 0 )
      return 0;
    processXaction();  // virtual processing time
    Commit();
    std::cout << "Withdraw of " << amount << " Euros from " << from << " completed" << std::endl;
    ret = 1;
  }
  return( ret );
}

void processXaction()
{
  unsigned int start;
  unsigned int stop;

  start = 1;
  stop = 3;
  srand(time(NULL) + getpid());
  double for_how_long = rand() % stop + start;
  sleep((unsigned int)for_how_long);
}
