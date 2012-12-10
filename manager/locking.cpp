#include <cstdlib>
#include <iostream>
//#include <pthread.h>

#include "locking.h"

// constructor(s)
lock_manager_c::lock_manager_c( unsigned int numlocks )
{
  this->numlocks = numlocks;

  // allocate memory for lock table
  lock_table = new lock_req_t[numlocks * sizeof( lock_req_t )];
  // allocate memory for graph object
  wait_for = new graph_c();

  // ...and initialize mutexes, condvars and locktable
  for(unsigned int i = 0; i < numlocks; i++) {
    lock_table[i].xact_id = 0;
    lock_table[i].type = 0;
    lock_table[i].granted = 0;
    lock_table[i].next = NULL;
  }

}

// destructor
lock_manager_c::~lock_manager_c()
{
  delete [] lock_table;
  delete wait_for;
}

// methods
void lock_manager_c::add_tranz_vertex( unsigned long int id )
{
  vertex_t * tranz = new vertex_t;

  tranz->element = id;
  tranz->colour = WHITE;
  tranz->edges = NULL;
  tranz->next = NULL;

  wait_for->add_vertex( tranz );
}

void lock_manager_c::rem_tranz_vertex( unsigned long int elem )
{
  wait_for->remove_vertex( wait_for->get_vertex( elem ) );
}

int lock_manager_c::detect_deadlock()
{
  return( wait_for->detect_cycle() );
}

int lock_manager_c::grant_lock( unsigned long int owner, unsigned int item, int locktype )
{
  std::list<unsigned long int> waitingfor;

  lock_req_t * lreqp = NULL;
  lock_req_t * tmp = NULL;
  tmp = lreqp = &lock_table[item];
  int head = 1;
  
  while( tmp != NULL ) {
    if(head && !tmp->granted){
      break;
    }
    else if( tmp->xact_id == owner ) {
      lreqp = tmp;
      break;
    }
    else{
      waitingfor.push_back(tmp->xact_id);
      lreqp = tmp;
      tmp = tmp->next;
      head = 0;
    }
  }

  // if it's already granted...
  if( lreqp->granted ) {
    // if that for the requesting owner
    if( lreqp->xact_id == owner ) {
      // in case it was previously a READ grant and now he wants WRITE
      if( lreqp->type < locktype ) {
        if(head) {
          if( lreqp->next == NULL ) {
            lreqp->type = LOCK_TYPE_WRITE;  // grant a lock upgrade
            return 1;
          }
          else {
            // someone else is granted the lock after you so no upgrade
            for( lock_req_t * waiting = lreqp->next; waiting != NULL; waiting = waiting->next)
              waitingfor.push_back(waiting->xact_id);
            // add edges to wait for graph
            std::list<unsigned long int>::iterator i;
            vertex_t * from, * to;
            for( i = waitingfor.begin(); i != waitingfor.end(); ++i ) {
              from = wait_for->get_vertex( owner );
              to = wait_for->get_vertex( * i );
              wait_for->add_edge(from, to);
            }
            wait_for->printme();
            return 0;
          }
        }
        else {
          // someone else is granted the lock before you so no upgrade
          // add edges to wait for graph
          std::list<unsigned long int>::iterator i;
          vertex_t * from, * to;
          for( i = waitingfor.begin(); i != waitingfor.end(); ++i ) {
            from = wait_for->get_vertex( owner );
            to = wait_for->get_vertex( * i );
            wait_for->add_edge(from, to);
          }
          wait_for->printme();
          return 0;
        }
      }
      return 1;
    }
    else { // if granted for other...
      // ... a READ lock
      if( lreqp->type == LOCK_TYPE_READ ) {
        // if requesting to READ...
        if( locktype == LOCK_TYPE_READ ) {
          // add node to end of queue
          lock_req_t * tmp;
          tmp = new lock_req_t;
          lock_req_t::init( tmp, owner, 1, locktype );
          if(lreqp->next != NULL) {
            std::cerr << "malakia paixthke!" << std::endl;
            exit(1);
          }
          lreqp->next = tmp;
          return 1;
        }
        else { // else if requesting WRITE
          // owner waits for all other in list in wait-for graph
          // add edges to wait for graph
          std::list<unsigned long int>::iterator i;
          vertex_t * from, * to;
          for( i = waitingfor.begin(); i != waitingfor.end(); ++i ) {
            from = wait_for->get_vertex( owner );
            to = wait_for->get_vertex( * i );
            wait_for->add_edge(from, to);
          }
          wait_for->printme();
          return 0;
        }
      }
      else { // ...a WRITE lock
        // add edges to wait for graph
        std::list<unsigned long int>::iterator i;
        vertex_t * from, * to;
        for( i = waitingfor.begin(); i != waitingfor.end(); ++i ) {
          from = wait_for->get_vertex( owner );
          to = wait_for->get_vertex( * i );
          wait_for->add_edge(from, to);
        }
        wait_for->printme();
        return 0;
      } 
    }
  }
  else { // it's not granted at all
    lock_req_t tmp;
    lock_req_t::init( &tmp, owner, 1, locktype );
    if(lreqp->next != NULL) {
      std::cerr << "malakia paixthke!" << std::endl;
      exit(1);
    }
    lock_table[item] = tmp;
    // OK
    return 1;
  }

  return 0;
}

/**
 * Strict 2φ locking means at commit or abort
 * search lock_table and find locks that are held by 
 * xactid
 * for each of them
 *     (i)delete lock_table entry,
 *     (ii)release lock,
 *     (iii)signal all waiting on its conditional variable
 */
void lock_manager_c::release_locks( unsigned long int owner )
{
  lock_req_t * lreqp;
  lock_req_t * prevlreqp;
  bool base;
  for( unsigned int i = 0; i < numlocks; i++ ) {
    base = true;
    for( lreqp = &lock_table[i], prevlreqp = NULL; lreqp != NULL; lreqp = lreqp->next ) {
      if( !lreqp->granted )
        break;
      else if( lreqp->xact_id == owner ) {
        if(base) {
          if(lreqp->next == NULL) {
            lock_table[i].xact_id = 0;
            lock_table[i].granted = 0;
            lock_table[i].type = 0;
            lock_table[i].next = 0;
          }
          else {
            lreqp = lreqp->next;

            lock_table[i].xact_id = lreqp->xact_id;
            lock_table[i].granted = lreqp->granted;
            lock_table[i].type = lreqp->type;
            lock_table[i].next = lreqp->next;
          }
        }
        else {
          prevlreqp->next = lreqp->next;
          lreqp->next = NULL;
          break;
        }
      }
      base = false;
      prevlreqp = lreqp;
    }
  }

  wait_for->remove_all_edges( wait_for->get_vertex( owner ) );
}

void lock_manager_c::printwaitfor()
{
  wait_for->printme();
}

