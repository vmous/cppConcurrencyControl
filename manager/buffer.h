#ifndef BUFFER_H_
#define BUFFER_H_

#include "data.h"

#define DEFAULT_BUFFER_CAPACITY 10

class buffer_c {
  private:
    datum_c * pool;
    unsigned int max_buf_size;
    unsigned int cur_buf_size;
  protected:
  public:
    /* constructor(s) */
    buffer_c();
    buffer_c(unsigned int);
    // copy constructor
    //buffer_c(const buffer_c &);

    /* destructor */
    ~buffer_c();

    /* operator overloaders */
    //buffer_c operator = (const buffer_c &);

    /* methods */
    unsigned int currentsize();
    unsigned int size();

    int put( datum_c * );
    datum_c * get( unsigned int );
    int getindex(unsigned int);
    datum_c * pick( unsigned int );
    datum_c * pickindex( unsigned int );
    void flushit( datum_c * );
    void flushme();
};

#endif /*BUFFER_H_*/
