#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
// #include <set>
#include <ctime> //C++ clock_t clock( void );
//?#include <cstdint> //UINT_PTR, BYTE=>uint8_t WORD =>uint16_t DWORD=>uint32_t
#include <sys/time.h> //SetTimer
//#include <time.h> //C clock_t clock( void );
//#include <thread> //работа  с потоками
//#include <Windows.h>
#include "primesieve.hpp"
//#include <stdlib.h> //DEBUG
//#include <signal.h> //DEBUG

// SPART BOINC
#include <cstddef>              // now required for NULL, etc.
#ifdef _WIN32                //  Stuff we only need on Windows:
#include "boinc_win.h"
#include "util.h"            // parse_command_line(), boinc_sleep()
#endif
/* BOINC API */
#include "boinc_api.h"
#include "diagnostics.h"     // boinc_init_diagnostics()
#include "filesys.h"         // boinc_fopen(), etc...
#include "str_util.h"            // for parse_command_line()
// version.h is only used to get BOINC version numbers
#include "version.h"
#define INPUT_FILENAME "in.txt"
#define OUTPUT_FILENAME "out.txt"
#define CHECKPOINT_FILENAME "checkpoint.txt"
#define PRINT_CONSOLE 1
// END BOINC
using namespace std;


typedef unsigned long long ull;
//enum poziciya{chasy,minuty,sekundy,sdelano,vremya,interval,provereno,skorostq,vsego_16,konec};
enum shablony{s_16=15,s_17,s_18,s_19,s_20,s_21,s_22,s_23,s_24,s_25,s_26,s_27,s_28,s_29,s_30,s_31,s_32,s_33};

const int dlina_intervala = 20000000;//2000 000 000;
const int chikli_stop = 5005;//5000001;
int chikli_last = 0;

const int chislo_shablonov = 18;
const int max_smew = ((s_33 + 1) >> 1);
const int centr = (s_33 >> 1) - 1;
const int ss = 100;
const int sss = 1000000;

int speed, najdeno[chislo_shablonov];
uint64_t start, endint;  //, max_stop = 1844674403075987866;
//max_stop = primesieve::get_max_stop();
bool otobr, poisk, pros, provr;
vector<ull> primes;
clock_t nachtik;
//BOINC
char input_path[512], output_path[512],  D_tmp[1024], buf[256] ;
FILE *file_i, *file_o ,*file_ch;
int rc;



bool potok();//potok(void*);
bool vypolnitq();
bool init(int nabor[]);
void proverka(int nabor[]);
void vyvod(long long stprime, int nabor[], int st, int shablon);
void prov_ne_poln(int nabor[]);
void prov_nach_ne_poln(int nabor[]);
inline void inf_pros(){
	cout << "Текущий интервал: [" << start << " ... " << endint << "]\n";
	cout << "Скорость: ";

	if(speed) cout  << speed;
	//else for(int i = s_16; i <= s_33; i++) cout << "\nНайдено " << (i + 1) << ':';
	cout << " Пройдено циклов: " << chikli_last << " " ;
	pros = false;
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
////DEBUG
//void posix_death_signal(int signum)
//{
//	signal(signum, SIG_DFL);
//	exit(3);
//}
int main(){
	//DEBUG signal(SIGSEGV, posix_death_signal);
	setlocale(LC_CTYPE, "rus");
	cout << "Поиск ассоциативных наборов простых";

  boinc_init_diagnostics(BOINC_DIAG_REDIRECTSTDERR|
                         BOINC_DIAG_MEMORYLEAKCHECKENABLED|
                         BOINC_DIAG_DUMPCALLSTACKENABLED|
                         BOINC_DIAG_TRACETOSTDERR);

  /* Output written to stderr will be returned with the Result (task) */
  rc = boinc_init();
  if (rc){
    fprintf(stderr, "APP: boinc_init() failed. rc=%d\n", rc);
    fflush(0);
    exit(rc);
  }
//	std::thread thr(potok);
//	thr.detach();
	potok();
//	while(true){	if(pros) inf_pros();sleep(10); 	}
  fprintf(file_o, "\n[END ] %llu : 0", endint  );
  boinc_fraction_done(1);
  boinc_finish(rc);       /* does not return */
}

bool potok(){
	//signal(SIGSEGV, posix_death_signal);
	boinc_resolve_filename(INPUT_FILENAME, input_path, sizeof(input_path));
	ifstream fin(input_path);
	if(!fin) {
        fprintf(stderr,
            "%s Couldn't find input file, resolved name %s.\n",
            boinc_msg_prefix(buf, sizeof(buf)), input_path
        );
        exit(-1);
    	}

	fin >> start;
	fin.close();

  boinc_resolve_filename(OUTPUT_FILENAME, output_path, sizeof(output_path));
  file_o = boinc_fopen(output_path, "wb");
  if (!file_o) {
        fprintf(stderr,
            "%s Couldn't find input file, resolved name %s.\n",
            boinc_msg_prefix(buf, sizeof(buf)), input_path
        );
        exit(-1);
    }

  boinc_resolve_filename(CHECKPOINT_FILENAME, output_path, sizeof(output_path));
  file_ch = boinc_fopen(output_path, "wb");
  if (!file_ch) {
        fprintf(stderr,
            "%s Couldn't find input file, resolved name %s.\n",
            boinc_msg_prefix(buf, sizeof(buf)), input_path
        );
        exit(-1);
    }

//	cout << "\n START: " << start << endl;
        for(chikli_last = 0; chikli_last < chikli_stop; chikli_last++) if(!vypolnitq())break;
//	cout << " STOP: " << endint << endl;
 fprintf(file_o, "\n[END ] %llu : 0", endint  );
 return true;
}
bool vypolnitq(){
	endint = start + dlina_intervala;
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
// v0.13	double st = ((chikli_last*1.1)/chikli_stop);
//cout.precision(17);
//cout << "Pi: " << fixed << st << endl;
//        boinc_fraction_done(st);
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
	}
	speed = int(7200000.0 / (clock() - nachtik) + 0.5);
	start = primes[primes.size() - s_33] - 1;
	ofstream fout("in.txt");
	fout << start;
	fout.close();
}

//s_17 - ((shablon + 1) >> 1)

void vyvod(long long  stprime, int nabor[], int st, int shablon){
	fprintf(file_o, "\n[ %d ] ", (shablon + 1) );
	fprintf(file_o, "%llu : 0", stprime );
	for(int i = 0, sum = 0; i < shablon; i++) {
	 sum += nabor[(st + i) & s_32];
	fprintf(file_o, " %d", sum );
	}
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




