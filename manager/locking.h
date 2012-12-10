#ifndef LOCKING_H_
#define LOCKING_H_

#include <list>
#include <pthread.h>

#include "graph.h"

#define LOCK_TYPE_READ 0
#define LOCK_TYPE_WRITE 1

typedef struct lock_request_s {
  unsigned long int xact_id;
  int granted;
  int type;
  lock_request_s * next;
  
  static void init( lock_request_s * lreqp, unsigned long int xact_id, int granted, int type )
  {
    lreqp->xact_id = xact_id;
    lreqp->granted = granted;
    lreqp->type = type;
    lreqp->next = NULL;
  }
}lock_req_t;

class lock_manager_c {
  private:
    unsigned int numlocks;
    lock_req_t * lock_table;
    graph_c * wait_for;

  protected:
  public:
    // constructor(s)
    lock_manager_c( unsigned int );
    //destructor
    ~lock_manager_c();
    // methods
    void add_tranz_vertex( unsigned long int );
    void rem_tranz_vertex( unsigned long int );
    int detect_deadlock();
    int grant_lock( unsigned long int, unsigned int, int );
    void release_locks( unsigned long int );
    
    void printwaitfor();
};

#endif /*LOCKING_H_*/
