#include "primesieve/primesieve.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <ctime>

#include <cstdint> //UINT_PTR
#include <sys/time.h> //SetTimer
#include <thread>
using namespace std;

enum poziciya{chasy,minuty,sekundy,sdelano,vremya,interval,provereno,skorostq,vsego_16,konec};
enum shablony{s_16=15,s_17,s_18,s_19,s_20,s_21,s_22,s_23,s_24,s_25,s_26,s_27,s_28,s_29,s_30,s_31,s_32,s_33};
 
const int koord2[100][100] = {{40,0},{51,0},{54,0},{13,2},{49,0},{0,1},{0,2},{0,3},{12,4},{0,22}};
const int koord[] = {1,2,3,4,5,66,77,8,9,10};


const int base_vals[5] = { 1000, 2000, 3500, 6000, 10000};

class _kursor{
        int ScrHandle;
public:
        _kursor(){ScrHandle = 0;}
        void skrytq(){
//              CONSOLE_CURSOR_INFO kursinf;
//              GetConsoleCursorInfo(ScrHandle , &kursinf);
//                kursinf.bVisible = false;
//              SetConsoleCursorInfo(ScrHandle , &kursinf);
//                cout << &kursinf << endl;
                cout << ScrHandle << endl;
        }
        void pokazatq(){
//              CONSOLE_CURSOR_INFO kursinf;
///             GetConsoleCursorInfo(ScrHandle , &kursinf);
//                kursinf.bVisible = true;;
///             SetConsoleCursorInfo(ScrHandle , &kursinf);
//                 cout << &kursinf << endl;
                 cout << ScrHandle << endl;
        }
        void pomestitq(intptr_t z){  cout << z  << endl; }//SetConsoleCursorPosition(ScrHandle, z);}
} kursor;


void threadFunction(int i, double d, const std::string &s)
{
     std::cout << i << ", " << d << ", " << s << std::endl;
	sleep(10);
}
 
int main()
{
     std::thread thr(threadFunction, 1, 2.34, "example");
     thr.join();
        cout << vsego_16 << endl;
	cout << koord[vsego_16] << endl;
	int krd = koord[vsego_16];
	cout << krd << endl;
//	cout << base_vals[1] << endl;
//	kursor.pomestitq(koord[1][0]);
return 0;
}


