#include <cstdlib>
#include <iostream>

#include "xaction_api.h"

ccm_comm_packet_c::ccm_comm_packet_c()
{
  op = '\0';
  id = 0;
  da = 0.0;
  s_op = 0;
}

ccm_comm_packet_c::~ccm_comm_packet_c()
{
}

char ccm_comm_packet_c::getop()
{
  return (op);
}

void ccm_comm_packet_c::setop( char op )
{
  this->op = op;
}

unsigned int ccm_comm_packet_c::getid()
{
  return( id );
}

void ccm_comm_packet_c::setid( unsigned int id )
{
  this->id = id;
}

double ccm_comm_packet_c::getda()
{
  return( da );
}

void ccm_comm_packet_c::setda( double da )
{
  this->da = da;
}

unsigned int ccm_comm_packet_c::gets_op()
{
  return( s_op );
}

void ccm_comm_packet_c::sets_op( unsigned int s_op )
{
  this->s_op = s_op;
}

bool ccm_comm_packet_c::is_ok()
{
  return( ok );
}

void ccm_comm_packet_c::set_ok( bool ok )
{
  this->ok = ok;
}

double Read( int sockfd, unsigned int where )
{
  ccm_comm_packet_c pack = ccm_comm_packet_c();
  
  pack.setop( 'R' );
  pack.setid( where );
  
  if( write( sockfd, (void *)&pack, sizeof( pack ) ) < 0 ) {            // Send message
    std::cerr << "write socket in READ" << std::endl;
    exit(1);
  }

  ccm_comm_packet_c::reset( &pack );
  if( read( sockfd, (ccm_comm_packet_c *)&pack, sizeof( pack ) ) < 0 ) {
    std::cerr << "read socket in READ" << std::endl;
    exit(1);
  }

  std::cout << "Account #" << pack.getid() << " balance equals to " << pack.getda() << std::endl;

  return( pack.getda() );
}

void Write( int sockfd, unsigned int where, double what )
{
  ccm_comm_packet_c pack = ccm_comm_packet_c();
  
  pack.setop( 'W' );
  pack.setid( where );
  pack.setda( what );
  
  if( write( sockfd, (void *)&pack, sizeof( pack ) ) < 0 ) {            // Send message
    std::cerr << "write socket in WRITE" << std::endl;
    exit(1);
  }

  ccm_comm_packet_c::reset( &pack );
  if( read( sockfd, (ccm_comm_packet_c *)&pack, sizeof( pack ) ) < 0 ) {
    std::cerr << "read socket in WRITE" << std::endl;
    exit(1);
  }

  std::cout << "Account #" << pack.getid() << "\'s balance written to " << pack.getda() << std::endl;
}

void Start()
{
/*
 * (i) add a vertex for the calling transaction in
 *     lock manager's wait-for graph
 */
}

void Commit()
{
/*
 * (i) flush all data into disk
 * (ii) tell lock manager to release all held locks
 * (iii) delete the corresponding to the calling transaction,
 *       vertex in lock manager's wait-for graph
 */
}

void Rollback()
{
/*
 * (i) for all WRITE(i)s datum = prevdatum
 * (ii) tell lock manager to release all held locks
 * (iii) delete the corresponding to the calling transaction,
 *       vertex in lock manager's wait-for graph
 */
}
