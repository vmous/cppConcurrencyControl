#include <iostream>
#include <fstream>

//#include "buffer.h"
//#include "data.h"
//#include "buffer.h"

using namespace std;

//int main( int argc, char **argv )
//{
//  /*
//  typedef db_datum_c<int> IntegerDataBaseData;
//  typedef buffer_c<IntegerDataBaseData> Buffa;
//
//  IntegerDataBaseData mydata = IntegerDataBaseData(17);
//
//  std::cout << "Container " << mydata.getid() << std::endl;
//  std::cout << "datum: " << mydata.getdatum() << std::endl;
//  std::cout << "# pinned: " << mydata.ispinned() << std::endl;
//  std::cout << "dirty?: " << ( mydata.isdirty()?"yup":"nop" ) << std::endl;
//  std::cout << "time stamped: " << mydata.gettstamp() << std::endl;
//  std::cout << std::endl;
//
//
//
//
//
//
//  Buffa mybuffa = Buffa();
//  int skat[9] = {1, 34, 536, 1, 76, 113, 8684, 2424, 34};
//
//  for(int i=0; i < 9; i++){
//    mybuffa.put(skat[i]);
//  }
//
//  unsigned int end = mybuffa.size();
//
//  for(unsigned int i = 0; i < end; i++) {
//    IntegerDataBaseData tmp = mybuffa.get(i);
//    std::cout << "__________________" << std::endl;
//    std::cout << "Container " << tmp.getid() << std::endl;
//    std::cout << "datum: " << tmp.getdatum() << std::endl;
//    std::cout << "# pinned: " << tmp.ispinned() << std::endl;
//    std::cout << "dirty?: " << ( tmp.isdirty()?"yup":"nop" ) << std::endl;
//    std::cout << "time stamped: " << tmp.gettstamp() << std::endl;
//    std::cout << "__________________" << std::endl;
//  }
//
//  */
//
//  unsigned int * id;
//  double * datum;
//  unsigned int size = 10;
//
//  id = new unsigned int [size];
//  datum = new double [size];
//
//  for(unsigned int i = 0; i < size; i++) {
//    *(id + i) = i + 1;
//    *(datum + i) = (i + 1) * 10;
//  }
//
//  std::fstream skat;
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
///*                                   WRITE                                      */
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
//  skat.open("/home/billy/Desktop/yooo.jdb", std::ios::out | std::ios::in | std::ios::binary);
//
//  skat.write((char *)(&size), sizeof(size));
//  for(unsigned int i = 0; i < size; i++) {
//    skat.write((char *)(id + i), sizeof(*(id + i)));
//    skat.write((char *)(datum + i), sizeof(*(datum + i)));
//  }
//
//  skat.close();
//
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
///*                                   READ                                       */
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
//  unsigned int r1_size;
//  unsigned int r1_id;
//  double r1_datum;
//
//
//  skat.open("/home/billy/Desktop/yooo.jdb", std::ios::out | std::ios::in | std::ios::binary);
//
//  std::fstream::pos_type pos, thepos;
//  unsigned int theid = 7;
//
//  skat.read((char *)(&r1_size), sizeof(r1_size));
//  std::cout << "size=" << r1_size << std::endl;
//  for(unsigned int i = 0; i < r1_size; i++) {
//    pos = skat.tellg();
//    skat.read((char *)(&r1_id), sizeof(r1_id));
//    skat.read((char *)(&r1_datum), sizeof(r1_datum));
//    std::cout << "id=" << r1_id << ", money=" << r1_id << std::endl;
//    if(r1_id == theid)
//       thepos = pos;
//  }
//  skat.close();
//
//
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
///*                                WRITE AGAIN                                   */
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
//  //unsigned int w2_size;
//  unsigned int w2_id = 666;
//  double w2_datum = 999;
//
//  skat.open("/home/billy/Desktop/yooo.jdb", std::ios::out | std::ios::in | std::ios::binary);
//  //skat.read((char *)(&w2_size), sizeof(w2_size));
//  //std::cout << "size=" << w2_size << std::endl;
//  skat.seekp(thepos);
//  skat.write((char *)&w2_id, sizeof(w2_id));
//  skat.write((char *)&w2_datum, sizeof(w2_datum));
//
//  skat.close();
//
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
///*                                 READ AGAIN                                   */
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
//  unsigned int r2_size;
//  unsigned int r2_id;
//  double r2_datum;
//
//  skat.open("/home/billy/Desktop/yooo.jdb", std::ios::out | std::ios::in | std::ios::binary);
//  skat.read((char *)(&r2_size), sizeof(r2_size));
//  std::cout << "dasize=" << r2_size << std::endl;
//  std::cout << "Rewriting the id=" << theid << std::endl;
//  skat.seekg(thepos);
//  skat.read((char *)&r2_id, sizeof(r2_id));
//  skat.read((char *)&r2_datum, sizeof(r2_datum));
//  std::cout << "id=" << r2_id << ", money=" << r2_datum << std::endl;
//  skat.close();
//
//
//
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
///*                                   READ                                       */
///********************************************************************************/
///********************************************************************************/
///********************************************************************************/
//
//  unsigned int newer_size, newer_id;
//  double newer_datum;
//  skat.open("/home/billy/Desktop/yooo.jdb", std::ios::out | std::ios::in | std::ios::binary);
//
//  skat.read((char *)(&newer_size), sizeof(newer_size));
//  std::cout << "newer size=" << newer_size << std::endl;
//  //while(!skat.eof()) {
//  for(unsigned int i = 0; i < newer_size; i++) {
//
//    skat.read((char *)&newer_id, sizeof(newer_id));
//    skat.read((char *)&newer_datum, sizeof(newer_datum));
//    std::cout << "newer id=" << newer_id << ", newer money=" << newer_datum << std::endl;
//  }
//  skat.close();
//
//
//
//
//
//
//  return 0;
//}
