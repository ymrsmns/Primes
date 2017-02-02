/* Муравьиный алгоритм для решения задачи коммивояжёра */

#include <locale>
#include <stdlib.h>
#include <iostream>
#include <malloc.h>
#include <cmath> //pow

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

using namespace std;

#define N_MIN	3			// минимальное количество вершин
#define N_MAX	100			// максимальное количество вершин

#define ALPHA	1			// вес фермента
#define BETTA	1			// коэффициент эвристики


#define T_MAX	100			// время жизни колонии
#define M		100			// количество муравьев в колонии
#define Q		1000			// количество
#define RHO		0.5			// коэффициент испарения феромона

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
// END BOINC
#define DEBUG 1
// структура ПУТЬ (длинна, массив вершин, количество вершин)
struct WAY_TYPE {
	int itabu;
	int length;
	int *tabu;
};

// вероятность перехода муравья ant в вершину to
double probability (int to, WAY_TYPE ant, double **pheromone, double **distance, int vertex) {
	// если вершина уже посещена, возвращаем 0
	for (int i=0; i<ant.itabu; ++i) if (to == ant.tabu[i]) return 0;
 
	double sum = 0.0;
	int from = ant.tabu[ant.itabu-1];
	// считаем сумму в знаминателе
	for (int j=0; j<vertex; ++j) {
		int flag = 1;
		// проверяем, посещал ли муравей j вершину
		for (int i=0; i<ant.itabu; ++i) if (j == ant.tabu[i]) flag = 0;
		// если нет, тогда прибавляем к общей сумме
		if (flag) sum += pow (pheromone[from][j], ALPHA) * pow (distance[from][j], BETTA);
	}
	// возвращаем значение вероятности
	return pow (pheromone[from][to], ALPHA) * pow (distance[from][to], BETTA) / sum;
}

// основная функция алгоритма поиска
WAY_TYPE AntColonyOptimization (double **distance0, int vertex, int start, int finish) {
// инициализация данных о лучшем маршруте
	WAY_TYPE way;
	way.itabu = 0;
	way.length = -1;
	way.tabu = (int *) malloc (sizeof (int) * vertex);
// инициализация данных о расстоянии и количестве феромона
	double **distance = NULL, **pheromone = NULL;
	distance = (double **) malloc (sizeof (double *) * vertex);
	pheromone = (double **) malloc (sizeof (double *) * vertex);
	for (int i=0; i<vertex; ++i) {
		distance[i] = (double *) malloc (sizeof (double) * vertex);
		pheromone[i] = (double *) malloc (sizeof (double) * vertex);
		for (int j=0; j<vertex; ++j) {
			pheromone[i][j] = 1.0 / vertex;
			if (i != j) distance[i][j] = 1.0 / distance0[i][j];
		}
	}
    if(DEBUG)   cout << "[1]инициализация данных" << endl;
// инициализация муравьев
	WAY_TYPE ants[M];
	for (int k=0; k<M; ++k) {
		ants[k].itabu = 0;
		ants[k].length = 0.0;
		ants[k].tabu = (int *) malloc (sizeof (int) * vertex);
		ants[k].tabu[ants[k].itabu++] = start;
	}
 if(DEBUG)	cout << "[2] инициализация муравьев" << endl;
// основной цикл
	for (int t=0; t<T_MAX; ++t) {
if(DEBUG)cout << "[3]t:" << t << endl;
if(DEBUG)cout << "[3]M:" << M << endl;
	// цикл по муравьям
		for (int k=0; k<M; ++k) {
if(DEBUG)cout << "[4.0]k:" << k << endl;		
if(DEBUG)cout << "[4.1] do vertex:" << vertex << endl;
// поиск маршрута для k-го муравья
			do {
				int j_max = -1;
				double p_max = 0.0;
				for (int j=0; j<vertex; ++j) {
if(DEBUG)cout << "[5.0] j:" << j << endl;
if(DEBUG)cout << "[5.1] ants[k].tabu[ants[k].itabu-1]:" << ants[k].tabu[ants[k].itabu-1]  << endl;
				// Проверка вероятности перехода в вершину j
					if (ants[k].tabu[ants[k].itabu-1] != j) {
						double p = probability (j, ants[k], pheromone, distance, vertex);
						if (p && p >= p_max) {
							p_max = p;
							j_max = j;
						}
					}
				}
if(DEBUG)cout << "[5.2] j_max:" << j_max << endl;

				ants[k].length += distance0[ants[k].tabu[ants[k].itabu-1]][j_max];
				ants[k].tabu[ants[k].itabu++] = j_max;
//if(DEBUG)cout << "[5.0] j_max:" << j_max << endl;
			} while (ants[k].tabu[ants[k].itabu-1] != finish);
		// оставляем феромон на пути муравья
if(DEBUG)cout << "[4.2] оставляем феромон :"  << endl;
			for (int i=0; i<ants[k].itabu-1; ++i) {
				int from = ants[k].tabu[i % ants[k].itabu];
				int to = ants[k].tabu[(i+1) % ants[k].itabu];
				pheromone[from][to] += Q / ants[k].length;
				pheromone[to][from] = pheromone[from][to];
			}
if(DEBUG)cout << "[4.3] проверка на лучшее решение :"  << endl;
		// проверка на лучшее решение
			if (ants[k].length < way.length || way.length < 0) {
				way.itabu = ants[k].itabu;
				way.length = ants[k].length;
				for (int i=0; i<way.itabu; ++i) way.tabu[i] = ants[k].tabu[i];
			}
		// обновление муравьев
			ants[k].itabu = 1;
			ants[k].length = 0.0;
		}
if(DEBUG)cout << "[4.4] цикл по ребрам  vertex:" << vertex  << endl;
	// цикл по ребрам
		for (int i=0; i<vertex; ++i)
			for (int j=0; j<vertex; ++j)
			// обновление феромона для ребра (i, j)
				if (i != j) pheromone[i][j] *= (1 - RHO);

//        fd = nchars/fsize;
//        if (cpu_time) fd /= 2;
      //  boinc_fraction_done(t);
	}
	// возвращаем кратчайший маршрут
	return way;
}

int memento()
{
    int a=0;
    return 0;
}
void fall()
{
      int* p = 0x00000000; 
      *p = 13;
}
void posix_death_signal(int signum)
{
    memento();
    signal(signum, SIG_DFL);
    exit(3);
}

// точка входа в программу
int main (int argc, char *argv[]) {
	setlocale (LC_ALL, "Russian");
signal(SIGSEGV, posix_death_signal);
	double **D = NULL;
	int N = 0, A = 0, B = 0;
//подключаем файлы in/out
  int rc;                       // return code from various functions
  //char resolved_name[512], D_tmp[512] ;      // physical file name for out.txt

 char input_path[512], output_path[512],  D_tmp[1024], buf[256] ;
  FILE *file_i, *file_o;
  //      file = fopen("input.txt", "r");
  //boinc_resolve_filename(INPUT_FILENAME, input_path, sizeof(input_path));
  //file_i = boinc_fopen(input_path, "r");
  //boinc_resolve_filename(OUTPUT_FILENAME, output_path, sizeof(output_path));
  //file_o = boinc_fopen(output_path, "wb");

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

  /*
   * Input and output files need to be "resolved" from their logical name
   * for the application to the actual path on the client's disk
   */

  boinc_resolve_filename(INPUT_FILENAME, input_path, sizeof(input_path));
  file_i = boinc_fopen(input_path, "r");
  if (!file_i) {
        fprintf(stderr,
            "%s Couldn't find input file, resolved name %s.\n",
            boinc_msg_prefix(buf, sizeof(buf)), input_path
        );
        exit(-1);
    }
  boinc_resolve_filename(OUTPUT_FILENAME, output_path, sizeof(output_path));
  file_o = boinc_fopen(output_path, "wb");
  if (!file_o) {
        fprintf(stderr,
            "%s Couldn't find input file, resolved name %s.\n",
            boinc_msg_prefix(buf, sizeof(buf)), input_path
        );
        exit(-1);
    }

//  if (file_o){
//    fprintf(stderr, "APP: cannot resolve output file name. RC=%d\n", rc);
//    boinc_finish(rc);    /* back to BOINC core */
//  }


////////////////////////////////////////////////////////

//Размер входного file
double fsize ;
file_size(input_path, fsize);

// инициализация матрицы расстояний

fscanf(file_i, "%d",&N);
        fprintf(file_o, "Количество вершин: [ %d ].\n", N );
	cout << "Количество вершин:" << N << endl;
// инициализация начальной и конечной точек
fscanf(file_i, "%d",&A);
        fprintf(file_o, "Начальная вершина: [ %d ].\n", A );
	cout << "Начальная вершина:" << A << endl;

fscanf(file_i, "%d",&B);
        fprintf(file_o, "Конечная вершина: [ %d ].\n", B );
        cout << "Конечная вершина:" << B << endl;
////////////////////////////////////
	cout << "Введите матрицу расстояний" << endl;
	D = (double **) malloc (sizeof (double *) * N);
	for (int i=0; i<N; ++i) {
		D[i] = (double *) malloc (sizeof (double) * N);
		for (int j=0; j<N; ++j){// cin >> D[i][j];
			fscanf(file_i, "%s",D_tmp);
			D[i][j] = atof(D_tmp);
		      //  fprintf(file_o, "D_tmp: [ %f ].\n",  D[i][j] );
		      //  cout << "D_tmp:" << D[i][j] << endl ;
		}
	}
cout << "Пошел расчет" << endl;
// запускаем алгоритм
	WAY_TYPE way = AntColonyOptimization (D, N, --A, --B);

// выводим результат
fprintf(file_o, "Длинна пути: [ %d ].\n", way.length );
//	cout << "Длинна пути: " << way.length << endl;
fprintf(file_o, "Путь: [ %d ]",  ++way.tabu[0] );
//	cout << "Путь: " << way.tabu[0];
	for (int i=1; i<way.itabu; ++i) {
		fprintf(file_o, "->: [ %d ]",  ++way.tabu[i] );
//		cout << " -> " << way.tabu[i];
	}
//	return 0;
//cout << ""  << endl ;
  boinc_fraction_done(1);
  boinc_finish(rc);       /* does not return */

}
#ifdef _WIN32

/*******************************************************
 * Windows: Unix applications begin with main() while Windows applications
 * begin with WinMain, so this just makes WinMain() process the command line
 * and then invoke main()
 */

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                   LPSTR Args, int WinMode)
{
  LPSTR command_line;
  char* argv[100];
  int argc;

  command_line = GetCommandLine();
  argc = parse_command_line( command_line, argv );
  return main(argc, argv);
}

#endif
//EOF//

