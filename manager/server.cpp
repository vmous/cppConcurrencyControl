/* File: int_str_server.c */
#include <sys/types.h>                                   /* For sockets */
#include <sys/socket.h>                                  /* For sockets */
#include <netinet/in.h>                         /* For Internet sockets */
#include <netdb.h>                                 /* For gethostbyaddr */
#include <cstdio>                                           /* For I/O */
#include <cstdlib>                                         /* For exit */
#include <cstring>                                /* For strlen, bzero */

#include <iostream>
#include <pthread.h>
#include <csignal>
#include "buffer.h"
#include "locking.h"
#include "xaction_api.h"

#include <list>

#define MAX_BUFFER_SIZE 256
#define DEFAULT_BUFFER_SIZE 20
#define MAX_THREAD_POOL_SIZE 256

void * serve_client( void * );

buffer_c * mybuff;
lock_manager_c * keeper;

pthread_t * threads;
pthread_mutex_t mtx;
pthread_cond_t cv;

/* Server with Internet stream sockets */
int main(int argc, char *argv[])
{
  int port, sock, newsock;
  unsigned int serverlen, clientlen;
  struct sockaddr_in server, client;
  struct sockaddr *serverptr, *clientptr;
  struct hostent *rem;

  if (argc < 2) {            /* Check if server’s port number is given */
    printf("Please give the port number\n");
    exit(1);
  }

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
  std::cout << "Starting up CCM service" << std::endl;

  std::cout << "Initializing data structures...\t\t\t";
  mybuff = new buffer_c( DEFAULT_BUFFER_SIZE );
  keeper = new lock_manager_c( mybuff->size() );

  threads = new pthread_t[MAX_THREAD_POOL_SIZE * sizeof( pthread_t )];
  pthread_mutex_init( &mtx, NULL );
  pthread_cond_init( &cv, NULL );

  std::cout << "[OK]" << std::endl;

  std::cout << "Setting up communication layer" << std::endl;
  std::cout << "> Creating socket...\t\t\t\t";
  if ( (sock = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) { /* Create socket */
    perror("socket");
    exit(1);
  }
  std::cout << "[OK]" << std::endl;

  std::cout << "> Preparing net structs...\t\t\t";
  port = atoi( argv[1] );              /* Convert port number to integer */
  server.sin_family = PF_INET;                      /* Internet domain */
  server.sin_addr.s_addr = htonl( INADDR_ANY );   /* My Internet address */
  server.sin_port = htons( port );                     /* The given port */
  serverptr = (struct sockaddr *) &server;
  serverlen = sizeof( server );
  std::cout << "[OK]" << std::endl;

  std::cout << "> Binding socket to address...\t\t\t";
  if (bind( sock, serverptr, serverlen ) < 0) {/* Bind socket to address */
    perror( "bind" );
    exit(1);
  }
  std::cout << "[OK]" << std::endl;

  std::cout << "> Listening for connections to port " << port << "\t";
  if (listen( sock, 5 ) < 0) {                 /* Listen for connections */
    perror( "listen" );
    exit(1);
  }
  std::cout << "[OK]" << std::endl;

  std::cout << "CCM service is up and running!" << std::endl << std::endl;

//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
  int t_count = 0;
  while(1) {
    clientptr = (struct sockaddr *) &client;
    clientlen = sizeof( client );
    /* Accept connection */
    if( ( newsock = accept( sock, clientptr, &clientlen ) ) < 0 ) {
      perror( "accept" );
      exit(1);
    }
    /* Find client’s address */
    if ( ( rem = gethostbyaddr( (char *) &client.sin_addr.s_addr, sizeof( client.sin_addr.s_addr ), client.sin_family ) ) == NULL) {
      perror( "gethostbyaddr" );
      exit(1);
    }
    printf("Accepted connection from %s\n", rem->h_name);

    if( pthread_create( &threads[t_count], NULL, serve_client, &newsock ) != 0 )
      perror("pthread create");
    else
      pthread_detach(threads[t_count]);

    keeper->add_tranz_vertex( threads[t_count] );
    keeper->printwaitfor();

    t_count++;
  }

  pthread_cond_destroy( &cv );
  pthread_mutex_destroy( &mtx );
  delete [] threads;
  delete keeper;
  delete mybuff;

  return 0;
}












void * serve_client(void * args)
{
  ccm_comm_packet_c pack = ccm_comm_packet_c();
  int where;
  int newsock = *((int *)args);
  bool looping = true;
  std::list<int> dirtied;
  
  std::string msg[] = { "New thread created", "Terminating thread..."};

  std::cout << msg[0] << std::endl;
  do {
    // Initialize message packet
    //pack.op = '\0';
    //pack.id = 0;
    //pack.da = 0.0;
    //pack.s_op = 0;

    if( read( newsock, (ccm_comm_packet_c *)&pack, sizeof( pack ) ) < 0 ) { /* Get message */
      perror("read");
      exit(1);
    }

    std::list<int>::iterator iter;
    switch(pack.getop()) {
      case 'r':
      case 'R':
      case '0' :
        pthread_mutex_lock( &mtx );
        // find the buffer block
        where = mybuff->getindex( pack.getid() );
        while( !keeper->grant_lock( pthread_self(), where, LOCK_TYPE_READ ) ) {
          if( keeper->detect_deadlock() ) {
            int tmp;
            for( iter = dirtied.begin(); iter != dirtied.end(); ++iter ) {
              tmp = (int)*iter;
              (mybuff->pickindex( tmp ))->rollback();
            }             

            keeper->release_locks( pthread_self() );

            pthread_cond_broadcast( &cv );

            ccm_comm_packet_c::reset( &pack );

            pthread_mutex_unlock( &mtx );
            goto HOP_R;
          }
          std::cout << "Thread " << pthread_self() << " suspended on cv" << std::endl;
          pthread_cond_wait( &cv, &mtx );
        }
        pthread_mutex_unlock( &mtx );
        std::cout << "i'm reading..." << std::endl;
        pack.setda( mybuff->pickindex( where )->getdatum() );
        pack.set_ok( true );

HOP_R:
        std::cout << "read " << pack.getda() << " Euros." << std::endl;
        if ( write( newsock, (void *)&pack, sizeof( pack ) ) < 0 ) { /* Send message */
           perror("write");
           exit(1);
        }
        break;
      case 'w':
      case 'W':
      case '1':
        pthread_mutex_lock( &mtx );
        // find the buffer block
        where = mybuff->getindex( pack.getid() );
        while( !keeper->grant_lock( pthread_self(), where, LOCK_TYPE_WRITE ) ) {
          if( keeper->detect_deadlock() ) {
            int tmp;
            for( iter = dirtied.begin(); iter != dirtied.end(); ++iter ) {
              tmp = (int)*iter;
              (mybuff->pickindex( tmp ))->rollback();
            }             

            keeper->release_locks( pthread_self() );

            pthread_cond_broadcast( &cv );

            ccm_comm_packet_c::reset( &pack );

            pthread_mutex_unlock( &mtx );
            goto HOP_W;
          }
          std::cout << "Thread " << pthread_self() << " suspended on cv" << std::endl;
          pthread_cond_wait( &cv, &mtx );
        }
        pthread_mutex_unlock( &mtx );
        std::cout << "i'm writting..." << std::endl;
        (mybuff->get( pack.getid() ))->setdatum( pack.getda() );
        pack.set_ok( true );
        // TODO
        dirtied.push_back(where);
        std::cout << "wrote " << pack.getda() << " Euros." << std::endl;

HOP_W:
        if ( write( newsock, (void *)&pack, sizeof( pack ) ) < 0 ) { /* Send message */
           perror("write");
           exit(1);
        }
        break;
      case 's':
      case 'S':
      case '9':
        switch(pack.gets_op()) {
          case 0:
              std::cout << "Executing special op rollback..." << std::endl;
              pthread_mutex_lock( &mtx );
 
              int tmp;
              for( iter = dirtied.begin(); iter != dirtied.end(); ++iter ) {
                tmp = (int)*iter;
                (mybuff->pickindex( tmp ))->rollback();
              }
              dirtied.clear();

              keeper->release_locks( pthread_self() );

              pthread_cond_broadcast( &cv );
              pthread_mutex_unlock( &mtx );
            break;
          case 1:
              pthread_mutex_lock( &mtx );
              std::cout << "Executing special op commit..." << std::endl;

              for( iter = dirtied.begin(); iter != dirtied.end(); ++iter ) {
                mybuff->flushit( mybuff->pickindex( *iter ) );
              }
              keeper->release_locks( pthread_self() );

              pthread_cond_broadcast( &cv );
              pthread_mutex_unlock( &mtx );
              
              looping = false;
            break;
          case 2:
              std::cout << "Executing special op flashing..." << std::endl;
              if( ( mybuff->pick( pack.getid() ) != '\0' ) && ( mybuff->pick( pack.getid() )->isdirty() ) )
                mybuff->flushit( mybuff->pick( pack.getid() ) );
            break;
          case 3:
              std::cout << "Executing special op flashin all..." << std::endl;
              mybuff->flushme();
            break;
          case 4:
              std::cout << "Executing special op tostring..." << std::endl;
              for(unsigned int i = 0; i < mybuff->currentsize(); i++)
                std::cout << mybuff->pickindex(i)->tostring() << std::endl;
            break;
          default:
            std::cout << "not a valid special operation..." << std::endl;
            break;
        }
        break;
      default:
        std::cout << "not a valid operation..." << std::endl;
        looping = 0;
        break;
    }

    ccm_comm_packet_c::reset( &pack );              // reset package

  } while (looping);        /* Finish on "x" */
  keeper->rem_tranz_vertex( pthread_self() );
  close(newsock);           /* Close socket */

  keeper->printwaitfor(); // FOR DEBUG
  std::cout << msg[1] << std::endl;
  return 0;
}


//int serve_read()
//{
//}

int handlesignal(int sig, int code, struct sigcontext *scp)
{
  char s[10];
  fprintf(stderr, "Signal %d received, code=%d. Do you wish to continue ? (y/n)", sig, code);
  scanf("%10s",s);
  if(*s=='y' || *s=='Y')
    return 0; /* continue execution */
  exit(0); /* The simplest of many possible actions is to exit doing nothing */
} 
