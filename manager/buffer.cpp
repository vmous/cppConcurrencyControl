/*
 * buffer.cpp
 * Implementation of header definitions in file
 * buffer.h
 * 
 * author:
 * Vassilis Moustakas
 * Departement of Informatics & Telecommunications
 * National & Kapodistrian University of Athens
 * grad0913@di.uoa.gr
 * 
 */
#include <iostream>
#include <fstream>
 
#include "buffer.h"

/* constructor(s) */
buffer_c::buffer_c()
{
  max_buf_size = DEFAULT_BUFFER_CAPACITY;
  cur_buf_size = 0;
  pool = new datum_c[DEFAULT_BUFFER_CAPACITY * sizeof( datum_c )];
}

buffer_c::buffer_c( unsigned int capacity )
{
  max_buf_size = capacity;
  cur_buf_size = 0;
  pool = new datum_c[capacity * sizeof( datum_c )];
}

// copy constructor
/*buffer_c::buffer_c( const buffer_c &from )
{
  memcpy( pool, from.pool, DEFAULT_BUFFER_CAPACITY * sizeof( datum_c ) );
  max_buf_size = from.max_buff_size;
  
}*/

/* destructor */
buffer_c::~buffer_c()
{
  delete [] pool;
}

/* operator overloaders */
/*buffer_c::buffer_c operator = (const buffer_c &from)
{
  memcpy( pool, from.pool, DEFAULT_BUFFER_CAPACITY * sizeof( datum_c ) );
  current_element = from.current_element;
}
*/

/* methods */

unsigned int buffer_c::currentsize()
{
  return( cur_buf_size );
}

unsigned int buffer_c::size()
{
  return( max_buf_size );
}

/*
 * put an object into buffer
 * args: the obj to be put
 * returns: the index inside the buffer pool if the object was
 *          successfuly put, -1 otherwise 
 */
int buffer_c::put( datum_c * obj )
{
  int where = -1;

  // if buffer has empty blocks...
  if(cur_buf_size < max_buf_size) {
    where = cur_buf_size++;
    pool[where] = *obj;
  }
  else { // ...replace according to LRU policy
    bool first = true;

    // locate the object to be replaced
    for(unsigned int i = 0; i < cur_buf_size; i++) {
      // must be unpinned...
      if( !pickindex(i)->ispinned() ) {
        if( first ) {
          where = i;
          first = false;
        }
        // ... and if a choice should be made choose the LRU!!
        else if( ( pickindex(i)->gettstamp() <  pickindex(where)->gettstamp() ) ) {
          where = i;
        }
      }
    }

    if( where >= 0 ) {
      flushit( &pool[where] ); // flush the object to be replaced to disk
      pool[where] = *obj;  // the actual replacement
    }
  }

  return( where );
}

datum_c * buffer_c::get( unsigned int id )
{
  datum_c * obj = NULL;
  bool inme = false; // found in buffer?

  // search buffer pool first
  for(unsigned int i = 0; i < cur_buf_size; i++) {
    if(pool[i].getid() == id) {
      std::cout << "Buffer hit:-)" << std::endl;
      obj = &pool[i];
      inme = true;
      break;
    }
  }

  // if not found in memory retrieve it from disk
  if(!inme) {
    std::cout << "Retrieving from disk..." << std::endl;
    unsigned int count;
    unsigned int cur_id;
    double cur_datum;
    std::fstream bin;
    std::fstream::pos_type pos;

    bin.open("/home/billy/Desktop/accounts.jdb", std::ios::out | std::ios::in | std::ios::binary);
 
    bin.read((char *)&count, sizeof(count));
    for(unsigned int i = 0; i < count; i++) {
      pos = bin.tellg();
      bin.read((char *)&cur_id, sizeof(cur_id));
      bin.read((char *)&cur_datum, sizeof(cur_datum));  
      if(cur_id == id) {
        obj = new datum_c( cur_id, cur_datum, pos );
        break;
      }
    }

    bin.close();

    obj = &pool[put( obj )];
  }

  return obj;
}

int buffer_c::getindex( unsigned int id )
{
  int where = -1;
  datum_c * obj;
  bool inme = false; // found in buffer?

  // search buffer pool first
  for(unsigned int i = 0; i < cur_buf_size; i++) {
    if(pool[i].getid() == id) {
      std::cout << "Buffer hit:-)" << std::endl;
      where = i;
      inme = true;
      break;
    }
  }

  // if not found in memory retrieve it from disk
  if(!inme) {
    std::cout << "Retrieving from disk..." << std::endl;
    unsigned int count;
    unsigned int cur_id;
    double cur_datum;
    std::fstream bin;
    std::fstream::pos_type pos;

    bin.open("/home/billy/Desktop/accounts.jdb", std::ios::out | std::ios::in | std::ios::binary);
 
    bin.read((char *)&count, sizeof(count));
    for(unsigned int i = 0; i < count; i++) {
      pos = bin.tellg();
      bin.read((char *)&cur_id, sizeof(cur_id));
      bin.read((char *)&cur_datum, sizeof(cur_datum));  
      if(cur_id == id) {
        obj = new datum_c( cur_id, cur_datum, pos );
        where = put( obj );
        break;
      }
    }

    bin.close();
  }

  return where;
}

datum_c * buffer_c::pick( unsigned int id )
{
  datum_c * obj = '\0';

  for(unsigned int i = 0; i < cur_buf_size; i++) {
    if(pool[i].getid() == id) {
      obj = &pool[i];
      break;
    }
  }
  return obj;
}

datum_c * buffer_c::pickindex( unsigned int indx )
{
  datum_c * obj = '\0';
  if( indx <= ( cur_buf_size - 1) )
    obj = &pool[indx];

  return obj;
}

void buffer_c::flushit( datum_c * obj )
{
  unsigned int newid;
  double newdatum;
  std::fstream bin;
  
  newid = obj->getid();
  newdatum = obj->getdatum(); 
  bin.open("/home/billy/Desktop/accounts.jdb", std::ios::out | std::ios::in | std::ios::binary);

  bin.seekp(obj->getbinpos());
  bin.write((char *)&newid, sizeof(newid));
  bin.write((char *)&newdatum, sizeof(newdatum));
  std::cout << "Flushed obj!" << std::endl;

  // object flushed, so it got clean in buffer
  obj->rollfront();

  bin.close();
}

void buffer_c::flushme()
{
  unsigned int newid;
  double newdatum;
  std::fstream bin;

  bin.open("/home/billy/Desktop/accounts.jdb", std::ios::out | std::ios::in | std::ios::binary);

  for(unsigned int i = 0; i < cur_buf_size; i++) {
    if(pool[i].isdirty()) {
      newid = pool[i].getid();
      newdatum = pool[i].getdatum();    
    
      bin.seekp(pool[i].getbinpos());  
      bin.write((char *)&newid, sizeof(newid));
      bin.write((char *)&newdatum, sizeof(newdatum));

      std::cout << "Flushed obj!" << std::endl;
    }
  }

  bin.close();
}
