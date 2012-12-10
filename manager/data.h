#ifndef DATA_H_
#define DATA_H_

#include <fstream>
#include <string>

class datum_c
{
  private:
    static unsigned long clock; //TODO: long type maybe a problem if too many datum_c objects!!!

    unsigned int id;            // object identification
    double datum;               // the actual data
    std::fstream::pos_type binpos;  // position of id&&datum in binary

    double prevdatum;          // the data before change
    unsigned int pinned;       // the data is used by pinned users
    bool dirty;                // denotes wheather the data has been modified since retrieved from disk
    unsigned long tstamp;      // contains the logical time when pinned became 0 
  protected:
  public:
    /* constructor(s) */
    datum_c();
    datum_c( unsigned int, double, std::fstream::pos_type );

    /* destructor */
    ~datum_c();

    /* methods */
    unsigned int getid();
    void setid( unsigned int );

    double getdatum();
    void setdatum( double );

    std::fstream::pos_type getbinpos();
    void setbinpos( std::fstream::pos_type );

    double getprevdatum();
    void setprevdatum( double );

    unsigned long gettstamp();
    void setstamp( unsigned long );

    void pinme();
    void unpinme();
    unsigned int ispinned();

    void dirtyme();
    void cleanme();
    bool isdirty();
    void rollback();
    void rollfront();
    std::string tostring();
};

#endif /*DATA_H_*/
