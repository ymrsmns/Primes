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

const int dlina_intervala = 2000000000;
const int chislo_shablonov = 18;
const int max_smew = ((s_33 + 1) >> 1);
const int centr = (s_33 >> 1) - 1;
const int ss = 100;
const int sss = 1000000;
const int koord[100][100] = {{40,0},{51,0},{54,0},{13,2},{49,0},{0,1},{0,2},{0,3},{12,4},{0,22}};

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
	void pomestitq(int*z){  cout << z  << endl; }
} kursor;

int hpotok; //HANDLE
uint32_t idpotok; //DWORD
intptr_t idTimer;
int speed, najdeno[chislo_shablonov];
ull start, endint, max_stop = primesieve::get_max_stop();
unsigned progress;
bool otobr, poisk, pros, provr;
vector<ull> primes;
clock_t nachtik;

void TimerProc();
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
			//kursor.pomestitq(koord[chasy]); 
			cout << koord[chasy] << endl;
 			cout << setw(10) << chas;
		}
		else minuta++;
		// ошибка типов данных в классе
		//kursor.pomestitq(koord[minuty]);
                cout << koord[minuty]  << endl;
		cout << setw(2) << setfill('0') << minuta;
		// ошибка типов данных в классе
		//kursor.pomestitq(koord[sekundy]);
		cout << koord[sekundy]  << endl;
		cout << setw(2) << tik << setfill(' ');
	}
	else{
		tik +=2;				
                // ошибка типов данных в классе
                //kursor.pomestitq(koord[sekundy]);
                cout << koord[sekundy]  << endl;
		cout << setw(2) << setfill('0') << tik << setfill(' ');
	}
}
inline void inf_pros(){
        // ошибка типов данных в классе
	//kursor.pomestitq(koord[interval]);
	cout << koord[interval]  << endl;
	cout << "Текущий интервал: [" << start << " ... " << endint << "]\nПросеяно  :      0%";
	// ошибка типов данных в классе
	//kursor.pomestitq(koord[skorostq]);
	cout << koord[skorostq]  << endl;
	cout << "Скорость  : ";
	if(speed) cout << setw(6) << speed;
	else for(int i = s_16; i <= s_33; i++) cout << "\nНайдено " << (i + 1) << ':';
	pros = false;
}
inline void inf_provr(){
	//kursor.pomestitq(koord[provereno]);
        // ошибка типов данных в классе
        cout << koord[provereno]  << endl;
	cout << "Проверено :      0%";
	provr = false;
}
inline void inf_otobr(){
	unsigned t;
//	kursor.pomestitq(koord[sdelano]);
	cout << koord[sdelano]  << endl;
	if(poisk){
		t = progress / ((primes.size() - 1) / ss);
		cout << setw(5) << t;
		int krd = koord[vsego_16][0];//изменино
		//for(int i = 0; i < chislo_shablonov; i++, krd.Y++){
		  for(int i = 0; i < chislo_shablonov; i++){// {XZ 4to za Y
			//kursor.pomestitq(krd);
			cout << krd  << endl;
			cout << setw(6) << najdeno[i];
		}
	}
	else{
		t = primes.size() / sss;
		cout << setw(5) << t;
	}
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

void Sleep(float s)
 
{
 
    int sec = int(s*1000000);
 
    usleep(sec);
 
}

int main(){
	setlocale(LC_CTYPE, "rus");
	kursor.skrytq();
	cout << "Поиск ассоциативных наборов простых";
	//kursor.pomestitq(koord[vremya]);	
	cout << koord[vremya]  << endl;
	cout << "0:00:00";
	//hpotok = CreateThread(NULL, 0, potok, NULL, 0, &idpotok);	
	//std::thread thr(potok);
	//thr.join();
	potok();
	//Таймер обновления окна
	//idTimer = settimer(NULL, 1, 2000, TimerProc(koord));
	//Выкинул, передача событий окну
	//MSG msg;	
	//while(true){
	//	GetMessage(&msg, NULL, 0, 0);
	//	DispatchMessage(&msg);
	//}	
	return 0;
}

void potok(){
	ifstream fin("start.txt");
	if(fin) fin >> start;
	fin.close();
//	start &= 0;
	while(vypolnitq());
	//убиваем таймер
//	KillTimer(NULL, idTimer);
	Sleep(100);
	cout << "\n\nДостигнут максимум " << max_stop << "\nДля выхода нажмите любую клавишу . . .\n";
//	system("pause > nul");
	kursor.pokazatq();
	exit(0);
//	return 0;
}

void TimerProc(int koord[]){
	vremya_vyvod();
	if(pros) inf_pros();
	if(provr) inf_provr();
	if(otobr) inf_otobr();
//	kursor.pomestitq(koord[konec]);
	cout << koord[konec]  << endl;

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

bool init(int nabor[]){
	static bool inicial;
	if(inicial){
		if(primes.size() <= s_33){
			if(primes.size() == s_33) prov_ne_poln(nabor);
			return false;
		}
		for(int i = 0; i < s_33; i++) nabor[i] = int(primes[i + 1] - primes[i]);
	}else{
		if(primes.size() <= s_33){
			prov_nach_ne_poln(nabor);
			return false;
		}
		for(int i = 0; i < s_33; i++) nabor[i] = int(primes[i + 1] - primes[i]);
		for(int i = s_16; i <= s_30; i++)
			for(int k = 0; k < max_smew - ((i + 1) >> 1); k++) prov_nach(nabor, i, k);
		inicial = true;
	}
	return true;
}

void proverka(int nabor[]){
	poisk = provr = true;
	int st = 0;
	for(size_t i = s_33;; i++){
		proverka_ch(nabor, st, i);
		proverka_nch(nabor, st, i);
		if(i >= primes.size() - 1) break;
		nabor[st] = int(primes[i + 1] - primes[i]);	
		st = (st + 1) & s_32;
		progress = i;
	}
	speed = int(7200000.0 / (clock() - nachtik) + 0.5);
	start = primes[primes.size() - s_33] - 1;
	ofstream fout("start.txt");
	fout << start;
	fout.close();
}

//s_17 - ((shablon + 1) >> 1)

void vyvod(long long  stprime, int nabor[], int st, int shablon){
	ostringstream imya;
	imya << "kpppch_" << (shablon + 1) << ".txt";
	ofstream fout("kpppch_.txt", ios::out | ios::app);
	fout << stprime << ": 0";
	for(int i = 0, sum = 0; i < shablon; i++) fout << ' ' << (sum += nabor[(st + i) & s_32]);
	fout << endl;
	najdeno[shablon - s_16]++;
}

void prov_ne_poln(int nabor[]){
	for(int i = 0; i < s_32; i++) nabor[i] = int(primes[i + 1] - primes[i]);
	for(int i = s_16; i <= s_31; i++){
		for(int j = s_16 - ((i + 1) >> 1); j <= s_20 - ((i + 2) >> 1); j++){
			for(int k = 0; k < (i >> 1); k++){if(nabor[k + j] != nabor[i - 1 - k + j]) goto next;}
			vyvod(primes[j], nabor, j, i);
next:		;
		}
	}
}

void prov_nach_ne_poln(int nabor[]){
	int raz = primes.size() - 1;
	for(int i = 0; i < raz; i++) nabor[i] = int(primes[i + 1] - primes[i]);
	for(int i = s_16; i <= raz; i++){
		for(int j = 0; j <= raz - i; j++){
			for(int k = 0; k < (i >> 1); k++){if(nabor[k + j] != nabor[i - 1 - k + j]) goto next;}
			vyvod(primes[j], nabor, j, i);
next:		;
		}
	}
}




