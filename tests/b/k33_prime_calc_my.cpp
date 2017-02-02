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
//#include "next_prime_my.h"
using namespace std;

typedef unsigned long long ull;
enum poziciya{chasy,minuty,sekundy,sdelano,vremya,interval,provereno,skorostq,vsego_16,konec};
enum shablony{s_16=15,s_17,s_18,s_19,s_20,s_21,s_22,s_23,s_24,s_25,s_26,s_27,s_28,s_29,s_30,s_31,s_32,s_33};

const int koord1[55] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257};

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
//	void pomestitq(COORD z){SetConsoleCursorPosition(ScrHandle, z);}
	void pomestitq(int z){  
		 //cout << z  ; cout << " ";}
		 printf(" %d ", z);     }
} kursor;

int hpotok; //HANDLE
uint64_t idpotok; //DWORD
intptr_t idTimer;
int speed, najdeno[chislo_shablonov];
uint64_t start, endint;//, max_stop = 1844674403075987866;
unsigned progress;
bool otobr, poisk, pros, provr;
//vector<ull> primes;
std::vector<uint64_t> primes;
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
	cout << "Текущий интервал: [" << start << " ... " << endint << "]\n";
	cout << "Скорость:";
	if(speed) cout << setw(6) << speed;
//	else for(int i = s_16; i <= s_33; i++) cout << "\nНайдено " << (i + 1) << ':';
	pros = false;
	cout << " Пройдено циклов: " << chikli_last << " " ;
	pros = false;
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
//int prime_calc(uint64_t from, uint64_t to)
//{
//       if(from & 1) from--; // делаем четным чтобы не пропустить простое from
//        if(from <= 2) from = 1;
//        int cnt = 0;
//        uint64_t step = (to - from) / 100; // дл€ индикатора расчета
//        uint64_t show = from; // следующий вывод индикатора
//        uint64_t x = next_prime(from);  //поиск числа
//        primes.push_back(x);
//        while(x && x < to) {
//                cnt++;
//                x = next_prime(x);
//                primes.push_back(x);
//        }
//        next_prime(0); // освобождение пам€ти
//        if(x == 0) cnt = -1; // ошибка в next_prime()
//        return cnt;
//}

int  checkNArr(uint64_t number){
    if (number < 2) return false;
    if (number == 2) return true;
    for (int i = 0; i < 55 ; i++){ if (number % koord1[i] == 0) {return false;} }
    return true;
}

void checkN(uint64_t number){
 int i = 1;
 uint64_t tmp, r;
   if(!checkNArr(number))
        while(i < 1000){
                i++;
                tmp = checkNArr(number+i);
                if ( tmp ) {r = number+i; primes.push_back(r); break;}
        }
}
int prime_calc_my(uint64_t from, uint64_t to){
        checkN(from);
        uint64_t x  = primes.front();// checkN((uint64_t)from);
         while(x && x < to) {
                checkN(x+1);
                x  = primes.back();
     }
}


int main(){
	setlocale(LC_CTYPE, "rus");
	//kursor.skrytq();
	cout << "Поиск ассоциативных наборов простых";
	//kursor.pomestitq((int)koord[vremya]);	
	//hpotok = CreateThread(NULL, 0, potok, NULL, 0, &idpotok);	
	std::thread thr(potok);
	thr.detach();
	while(true){
		if(pros) inf_pros();
		sleep(10);
	}
	return 0;
}

void potok(){
	ifstream fin("start.txt");
	if(fin) fin >> start;
	fin.close();
	cout << " START: " << start << endl;
//	start &= 0;
/// beskon	while(vypolnitq());
        for(chikli_last = 0; chikli_last < chikli_stop; chikli_last++) if(!vypolnitq())break;
	//убиваем таймер
//	KillTimer(NULL, idTimer);
//	cout << "\n\nДостигнут максимум " << max_stop << "\nДля выхода нажмите любую клавишу . . .\n";
//	system("pause > nul");
	//kursor.pokazatq();
	exit(0);
//	return 0;
}

void TimerProc(const int koord[][10]){
	//vremya_vyvod();
	if(pros) inf_pros();
	//if(provr) inf_provr();
	//kursor.pomestitq((int)koord[konec]);

}

bool vypolnitq(){
	endint = start + dlina_intervala;
//	next_prime(endint); 
	if(!otobr) otobr = true;
	pros = true;	
	poisk = false;
	primes.clear();
	nachtik = clock();
        prime_calc_my(start,endint); //заменена функция
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
	cout << (shablon + 1) << endl;
	//imya << "kpppch_" << (shablon + 1) << ".txt";
	ofstream fout("out.txt", ios::out | ios::app);
	fout << (shablon + 1) << "=> ";
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




