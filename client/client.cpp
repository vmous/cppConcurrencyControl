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

void processXaction();

int mainc(int argc, char *argv[])     /* Client with Internet stream sockets  */
{
  int port, sock;
  char bufop[256], bufid[256], bufda[256], bufso[256];
  unsigned int serverlen;
  struct sockaddr_in server;
  struct sockaddr *serverptr;
  struct hostent *rem;

  if (argc < 3) {     /* Are server’s host name and port number given?  */
    printf("Please give host name and port number\n");
    exit(1);
  }
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { /* Create socket  */
    std::cerr << "socket" << std::endl;
    exit(1);
  }
  if ((rem = gethostbyname(argv[1])) == NULL) { /* Find server address  */
    std::cerr << "gethostbyname" << std::endl;
    exit(1);
  }

  port = atoi(argv[2]);              /* Convert port number to integer  */
  server.sin_family = PF_INET;                       /* Internet domain */
  bcopy( (char *)rem->h_addr, (char *)&server.sin_addr, rem->h_length );
  server.sin_port = htons(port); /* Server’s Internet address and port  */
  serverptr = (struct sockaddr *) &server;
  serverlen = sizeof server;
  if (connect(sock, serverptr, serverlen) < 0) { /* Request connection  */
    std::cerr << "connect" << std::endl;
    exit(1);
  }
  printf("Requested connection to host %s port %d\n", argv[1], port);

  int looping = 1;
  do {






    bzero( bufop, sizeof bufop );                       /* Initialize buffer */
    bzero( bufid, sizeof bufid );                       /* Initialize buffer */
    bzero( bufda, sizeof bufda );                       /* Initialize buffer */
    bzero( bufso, sizeof bufso );                       /* Initialize buffer */
    printf( "Give operation: " );
    fgets( bufop, sizeof bufop, stdin );          /* Read message from stdin */
    bufop[strlen(bufop)-1] = '\0';            /* Remove newline character  */

    if( strcmp( bufop, "r" ) == 0 ) {
      printf( "Give id: " );
      fgets( bufid, sizeof bufid, stdin );          /* Read message from stdin */
      bufid[strlen(bufid)-1] = '\0';            /* Remove newline character  */

      Read( sock, atoi( bufid ) );
    }
    else if(strcmp(bufop, "w") == 0) {
      printf("Give id: ");
      fgets(bufid, sizeof bufid, stdin);          /* Read message from stdin */
      bufid[strlen(bufid)-1] = '\0';            /* Remove newline character  */

      printf("Give data to write: ");
      fgets(bufda, sizeof bufda, stdin);          /* Read message from stdin */
      bufda[strlen(bufda)-1] = '\0';            /* Remove newline character  */

      Write(sock, atoi(bufid), atof(bufda));
    }
    else if(strcmp(bufop, "s") == 0) {
      printf("Give special operation: ");
      fgets(bufso, sizeof bufso, stdin);          /* Read message from stdin */
      bufso[strlen(bufso)-1] = '\0';            /* Remove newline character  */

      ccm_comm_packet_c pack = ccm_comm_packet_c();
      pack.setop('s');
      pack.sets_op( atoi(bufso) );

      switch( pack.gets_op() ) {
        case 0:
          break;
        case 1:
          looping = 0;
          break;
        case 2:
          printf("Give id to flash: ");
          fgets(bufid, sizeof bufid, stdin);          // Read message from stdin
          bufid[strlen(bufid)-1] = '\0';            // Remove newline character

          pack.setid( atoi(bufid) );
          break;
        case 3:
        case 4:
        default:
          break;
      }
      
      if ( write( sock, (void *)&pack, sizeof( pack ) ) < 0 ) {            // Send message
        std::cerr << "write" << std::endl;
        exit(1);
      }
    }








      // bzero(buf, sizeof buf);                       // Initialize buffer
      //if (read(sock, buf, sizeof buf) < 0) {        // Receive message
         //std::cerr << "read" << std::endl; exit(1); }
      //printf("Read string:       %s\n", buf);*/
   } while ( looping );                 /* Finish on "end" */
   close(sock);
   exit(0);
}                      /* Close socket and exit */
