#ifndef XACTION_API_H_
#define XACTION_API_H_

class ccm_comm_packet_c {
  private:
    char op;
    unsigned int id;
    double da;
    unsigned int s_op;
    bool ok;
  protected:
  public:
    // constructor(s)
    ccm_comm_packet_c();
    // destructor
    ~ccm_comm_packet_c();
    // methods
    char getop();
    void setop( char );
    unsigned int getid();
    void setid( unsigned int );
    double getda();
    void setda( double );
    unsigned int gets_op();
    void sets_op( unsigned int );
    bool is_ok();
    void set_ok( bool );

    static void reset( ccm_comm_packet_c * pack )
    { 
      pack->op = '\0';
      pack->id = 0;
      pack->da = 0.0;
      pack->s_op = 0;
      pack->ok = false;
    }
};


double Read( int, unsigned int );
int Write( int, unsigned int, double );
void Start();
void Commit();
void Rollback();

#endif /*XACTION_API_H_*/
