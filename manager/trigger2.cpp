#include <iostream>
#include <fstream>

#include "buffer.h"
#include "data.h"
//#include "buffer.h"



//int mainx( int argc, char **argv )
//{
//  char buf[256];
//  bool looping = true;
//  char op;
//  unsigned int id;
//  double da;
//
//  do {
//      bzero(buf, sizeof buf);                       /* Initialize buffer */
//      op = '\0';
//      id = 0;
//      da = 0.0;
//
//      printf("Give opidda: ");
//      fgets(buf, sizeof buf, stdin);          /* Read message from stdin */
//      buf[strlen(buf)-1] = '\0';
//
//      sscanf( buf, "%c%4u%6lf", &op, &id, &da );
//      switch(buf[0]) {
//        case 'r':
//        case 'R':
//        case '0' :
//          std::cout << "i'm reading..." << std::endl;
//          std::cout << "op: " << op << std::endl;
//          std::cout << "id: " << id << std::endl;
//          std::cout << "da: " << da << std::endl;
//          std::cout << std::endl;
//          break;
//        case 'w':
//        case 'W':
//        case '1':
//          std::cout << "i'm writting..." << std::endl;
//          std::cout << "op: " << op << std::endl;
//          std::cout << "id: " << id << std::endl;
//          std::cout << "da: " << da << std::endl;
//          std::cout << std::endl;
//          break;
//        case 'x':
//        case 'X':
//        case '2':
//          std::cout << "Executing special op..." << std::endl;
//          std::cout << "op: " << op << std::endl;
//          std::cout << "id: " << id << std::endl;
//          std::cout << "da: " << da << std::endl;
//          std::cout << std::endl;
//
//          looping = false;
//          break;
//        default:
//          std::cout << "not a valid operation..." << std::endl;
//          break;
//      }
//   } while( looping );
//   return 0;
//}
//
///*  unsigned int acc_no;
//  double acc_money;
//  unsigned int cur_size;
//
//  buffer_c mybuff = buffer_c(20);
//
//  while(true) {
//    std::cout << "Enter account number: ";
//    std::cin >> acc_no;
//    if(acc_no == 0)
//      break;
//
//    cur_size = mybuff.currentsize();
//    std::cout << "Buffer size equal to " << cur_size << std::endl;
//    std::cout << "Bring me account " << acc_no << std::endl;
//    acc_money = mybuff.get(acc_no).getdatum();
//    std::cout << "Account " << acc_no << " has a " << acc_money << "Euro deposit" << std::endl;
//
//    cur_size = mybuff.currentsize();
//    std::cout << "Buffer size equal to " << cur_size << std::endl;
//    std::cout << std::endl;
//
//    for(unsigned int i = 0; i < cur_size; i++) {
//      std::cout << i << ": " << mybuff.peek(i).getid() << " | " << mybuff.peek(i).getdatum() << std::endl;
//    }
//  }
//
//  std::cout << std::endl << "Terminating..." << std::endl;
//
//  return 0;
//}
//*/
