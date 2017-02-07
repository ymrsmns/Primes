#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <ctime>
#include <cstdint> //UINT_PTR, BYTE=>uint8_t WORD =>uint16_t DWORD=>uint32_t
#include <sys/time.h> //SetTimer
#include <time.h>
#include <thread> //работа  с потоками
//#include <Windows.h>
#include "primesieve/primesieve.hpp"

using namespace std;

typedef unsigned long long ull;
enum poziciya{chasy,minuty,sekundy,sdelano,vremya,interval,provereno,skorostq,vsego_16,konec};
enum shablony{s_16=15,s_17,s_18,s_19,s_20,s_21,s_22,s_23,s_24,s_25,s_26,s_27,s_28,s_29,s_30,s_31,s_32,s_33};

const int dlina_intervala = 20000000;//2000 000 000;
const int chislo_shablonov = 18;
const int max_smew = ((s_33 + 1) >> 1);
const int centr = (s_33 >> 1) - 1;
const int ss = 100;
int kolnum = 0;
const int chikli_stop = 12001;//5000001; 
int chikli_last = 0;
const int sss = 1000000;
const int koord[10][10] = {{40,0},{51,0},{54,0},{13,2},{49,0},{0,1},{0,2},{0,3},{12,4},{0,22}};
const int koord2[] = {1,2,3,4,5,66,77,8,9,10};
class _kursor{
	int ScrHandle; //HANDLE
public:
	//_kursor(){ScrHandle = GetStdHandle(STD_OUTPUT_HANDLE);}
	_kursor(){ScrHandle = 0;}
	void skrytq(){
//		CONSOLE_CURSOR_INFO kursinf;	
//		GetConsoleCursorInfo(ScrHandle , &kursinf);
//		kursinf.bVisible = false;
//		SetConsoleCursorInfo(ScrHandle , &kursinf);
//		cout << &kursinf << endl
		cout << ScrHandle << endl;
	}
	void pokazatq(){
//		CONSOLE_CURSOR_INFO kursinf;	
//		GetConsoleCursorInfo(ScrHandle , &kursinf);
//		kursinf.bVisible = true;
//		SetConsoleCursorInfo(ScrHandle , &kursinf);
//		 cout << &kursinf << endl
		 cout << ScrHandle << endl;
	}
//	void pomestitq(COORD z){SetConsoleCursorPosition(ScrHandle, z);}
	void pomestitq(int z){  
		 cout << z  ; cout << "  " ;}
		 //printf(" %d ", z);     }
} kursor;

int hpotok; //HANDLE
uint32_t idpotok; //DWORD
intptr_t idTimer;
int speed, najdeno[chislo_shablonov];
ull  start, endint, max_stop = chikli_stop*dlina_intervala;
//max_stop = 1844674403075987866;
//max_stop = primesieve::get_max_stop();
unsigned progress;
bool otobr, poisk, pros, provr;
vector<ull> primes;
clock_t nachtik;

void TimerProc(const int koord[][10]); //криво
void potok();//potok(void*);
bool vypolnitq();
bool init(int nabor[]);
void proverka(int nabor[]);
void vyvod(long long stprime, int nabor[], int st, int shablon);
void prov_ne_poln(int nabor[]);
void prov_nach_ne_poln(int nabor[]);
inline void vremya_vyvod(){
	static int tik, minuta, chas;
	if(tik > 57){
		tik = 0;
		if(minuta > 58){
			minuta = 0;
			chas++;
			// ошибка типов данных в классе
			kursor.pomestitq((int)koord[chasy]); 
 			cout << setw(10) << chas;
		}
		else minuta++;
		// ошибка типов данных в классе
		kursor.pomestitq((int)koord[minuty]);
                //cout << koord[minuty]  << endl;
		cout << setw(2) << setfill('0') << minuta;
		// ошибка типов данных в классе
		kursor.pomestitq((int)koord[sekundy]);
		//cout << koord[sekundy]  << endl;
		cout << setw(2) << tik << setfill(' ');
	}
	else{
		tik +=2;				
                // ошибка типов данных в классе
                //kursor.pomestitq((int)koord[sekundy]);
                cout << koord[sekundy]  ;
		cout << setw(2) << setfill('0') << tik << setfill(' ');
	}
}
inline void inf_pros(){
        // ошибка типов данных в классе
//	kursor.pomestitq((int)koord[interval]);
	
	cout << "Текущий интервал: [" << start << " ... " << endint << "]\n";
	// ошибка типов данных в классе
//	kursor.pomestitq((int)koord[skorostq]);
	cout << "Скорость: ";

	if(speed) cout << setw(6) << speed;
	else for(int i = s_16; i <= s_33; i++) cout << "\nНайдено " << (i + 1) << ':';
	pros = false;
	cout << " Пройдено: " << kolnum << " " ;
	kolnum = endint;
	cout << " Пройдено циклов: " << chikli_last << " " ;
}
inline void inf_provr(){
	kursor.pomestitq((int)koord[provereno]);
        // ошибка типов данных в классе
	cout << "Проверено :      0%";
	provr = false;
}
inline void proverka_ch(int nabor[], int st, int i){
	for(int j = 1, t = centr + st; j <= 7; j++){
		if(nabor[(t - j) & s_32] != nabor[(t + j) & s_32]) return;
	}
	vyvod(primes[i - s_25], nabor, st + 8, s_16);
	for(int j = 8, t = centr + st; j <= centr; j++){
		if(nabor[(t - j) & s_32] != nabor[(t + j) & s_32]) return;
		vyvod(primes[i - s_18 - j], nabor, st + s_16 - j, s_16 + ((j - 7) << 1));
	}
}
inline void proverka_nch(int nabor[], int st, int i){
	for(int j = 0, t = centr + st; j <= 7; j++){
		if(nabor[(t - j) & s_32] != nabor[(t + j + 1) & s_32]) return;
	}
	vyvod(primes[i - s_25], nabor, st + 8, s_17);
	for(int j = 8, t = centr + st; j <= centr; j++){
		if(nabor[(t - j) & s_32] != nabor[(t + j + 1) & s_32]) return;
		vyvod(primes[i - s_18 - j], nabor, st + s_16 - j, s_17 + ((j - 7) << 1));
	}
}
inline void prov_nach(int nabor[], int i, int k){
	for(int j = 0; j < (i >> 1); j++) if(nabor[k + j] != nabor[i - 1 + k - j]) return;
	vyvod(primes[k], nabor, k, i);
}
//&
int main(){
	setlocale(LC_CTYPE, "rus");
	kursor.skrytq();
	cout << "Поиск ассоциативных наборов простых";
	primesieve::generate_primes(start, endint, &primes);
	return 0;
}

bool vypolnitq(){
	if(start >= max_stop) return false;
	endint = min(start + dlina_intervala, max_stop);
	if(!otobr) otobr = true;
	pros = true;	
	poisk = false;
	primes.clear();
	nachtik = clock();
	primesieve::generate_primes(start, endint, &primes);
	if(primes.size() <= s_16) return false;
	int nabor[s_33];
	if(!init(nabor)) return false;
	proverka(nabor);
	return true;
}
