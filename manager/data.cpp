/*
 * data.cpp
 * Implementation of header definitions in file
 * data.h
 * 
 * author:
 * Vassilis Moustakas
 * Departement of Informatics & Telecommunications
 * National & Kapodistrian University of Athens
 * grad0913@di.uoa.gr
 * 
 */
 
 #include "data.h"
#include <string.h>
 
/* static variables */
unsigned long datum_c::clock = 0;

/* constructor(s) */
datum_c::datum_c() {
  this->id = 0;
  this->datum = 0;
  this->binpos = 0;

  this->prevdatum = 0;
  this->pinned = 1;
  this->dirty = false;
  this->tstamp = 0; //TODO time stamping initialization stuff
}

datum_c::datum_c( unsigned int id, double datum, std::fstream::pos_type binpos ) {
  this->id = id;
  this->datum = datum;
  this->binpos = binpos;

  this->prevdatum = datum;
  this->pinned = 1;
  this->dirty = false;
  this->tstamp = 0; //TODO time stamping initialization stuff
}

/* destructor(s) */
datum_c::~datum_c() {}

/* methods */
unsigned int datum_c::getid() {
  return( id );
}

void datum_c::setid(unsigned int id) {
  this->id = id;
}

double datum_c::getdatum() {
  return( datum );
}

void datum_c::setdatum(double datum) {
  this->datum = datum;
  this->dirty = true;
}

std::fstream::pos_type datum_c::getbinpos()
{
  return( binpos );
}

void datum_c::setbinpos( std::fstream::pos_type binpos )
{
  this->binpos = binpos;
}

double datum_c::getprevdatum() {
  return( prevdatum );
}

void datum_c::setprevdatum(double prevdatum) {
  this->prevdatum = prevdatum;
}

unsigned long datum_c::gettstamp() {
  return( tstamp );
}

void datum_c::setstamp(unsigned long tstamp) {
  this->tstamp = tstamp;
}

void datum_c::pinme() {
  pinned++;
}

void datum_c::unpinme() {
  if(pinned) {
   pinned--;
   if(!pinned)
    tstamp = clock;
  }
}

unsigned int datum_c::ispinned() {
  return( pinned );
}

void datum_c::dirtyme() {
  dirty = true;
}

void datum_c::cleanme() {
  dirty = false;
}

bool datum_c::isdirty() {
  return( dirty );
}

void datum_c::rollback()
{
  if(dirty) {
    datum = prevdatum;
    dirty = false;
  }
}

void datum_c::rollfront()
{
  prevdatum = datum;
  dirty = false;
}

std::string datum_c::tostring() {
  char buf[50];
  std::string rets = "";
  bzero( buf, sizeof( buf ) );
  sprintf(buf, "| id=%4u | datum=%6.2f | pinned=%d | dirty=%d | tstamp=%ld |", id, datum, pinned, dirty, tstamp);
  rets = buf;
  
  return rets;
}
