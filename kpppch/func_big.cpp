
#include <iostream>
#include <vector>
#include<iterator>
//работа с файлами
#include <fstream>
#include <string>
#include <cstring>
//определяем тип данных
#include <typeinfo>
// заголовочный файл с прототипом функции clock()
#include <ctime>
#include <cmath>
#include <iostream>
#include <locale>
using namespace std;

//при 100 на эталоне вылезает лишний патер июъб использовать как проверку
const int MAXSIZE=100;
int N; //глобпльная переменная
int firstNum =0;
/*
// http://mathhelpplanet.com/viewtopic.php?f=54&t=43217
//эталон проверки
{12003179,  12003191,  12003197,  12003209,  12003221,  12003227,  12003239}
12003179: 0, 12, 18, 30, 42, 48, 60
*/
void show_vector( vector<int>&a){
        for (vector<int>::iterator it = a.begin() ; it!=a.end() ; ++it)
                cout<<*it<<" ";
	cout << endl;
}

bool cheskCl(int n){//проверка на простое число
    bool flag = true;
    float end = sqrt((float)n);
    for (int i=2; i <= end; ++i) if (n%i == 0) flag = false;
    return flag;
}
void show_array(int*arr1){ //вывод массивов
        for (int z = 0; z < N; z++) if(arr1[z]) cout << arr1[z] << " ";
        cout << endl;
}

void cr_arr(int*mas,int n){
 for(int i=n; i<=n+MAXSIZE; i++){
        if(cheskCl(i)){
           if(firstNum == 0 ){firstNum=i;}
          mas[N]=i-firstNum;
          N++;}
    }
}
void cr_sim(int*mas, int*sim){//Ищим разницу
	for (int i = 0; i < N; ++i){
          if(mas[i]){
                int r = mas[i]-mas[i-1];
                if ( r > 0 ) sim[i]=r;
                else sim[i] = 0;
          }
          else { sim[i] = 0;}
	}
}
void cr_res(vector<int>&arr){

	       for (int j = 0; j < arr.size(); ++j) if(arr[j] != 0) {arr.erase(arr.begin()+1,arr.begin()+j);break;}
                 for (int j = 1; j < arr.size(); ++j) if(arr[j] == 0) {arr.erase(arr.begin()+j,arr.begin()+arr.size() );break;}
                cout << "clear vector:" << firstNum << ": " ;
                show_vector(arr);


}
void search_zerkalo(int*mas, int*sim, vector<int>&arr){
int Zsim=0;
for (int i = 0; i < N; ++i){
        if(sim[i] == sim[i+1]) {
		//cout << "[1]nashli simetrii bez centera: " << sim[i] ;cout << " " << sim[i+1] << " "; cout << endl;
                int z1= i; int z2= i+1;
                arr[30]=mas[i];  arr[31]=mas[i+1]; //записываем 2 значения в вектор
                // cout << "[1.1] id sim:" << z1 << " "; cout  << z2 << " "; cout << endl;
                for (int j = 0; j < N; ++j){//проверяем разначения по краям
                        if(sim[z1-j] == sim [z2+j]) {
                                        Zsim++;
                                //      cout << "[1/2] i:" << i << " ";
                                //      cout << " j:" <<j <<" ";
                                //      cout << " Zsim:" <<Zsim <<" ";
                                //      cout << sim[z1-j] << " "; cout << sim[z2+j] << " ";  cout << endl;
                                        arr[31+j]=mas[z2+j];
                                        arr[30-j]=mas[z1-j];
                        }
                        else if (sim[z1-j] != sim [z2+j] &&  Zsim >0 ){ cr_res(arr);break;}
                }
                //записываем последовательность
    //            show_vector(arr);//чистим вектор
        //         for (int j = 0; j < arr.size(); ++j) if(arr[j] != 0) {arr.erase(arr.begin()+1,arr.begin()+j);break;}
         //        for (int j = 1; j < arr.size(); ++j) if(arr[j] == 0) {arr.erase(arr.begin()+j,arr.begin()+arr.size() );break;}
          //      cout << "clear vector:" << firstNum << ": " ;
           //     show_vector(arr);
        }/*
        else if(sim[i] == sim[i+2]) {cout << "[2]nashli center simetrii: " << sim[i] ; cout << " " << sim[i+1] << " ";cout << sim[i+2] << " "; cout << endl;
                for (int j = i+1; j < N; ++j){//проверяем разначения по краям
                        cout << "[2/1]: " << sim[j-1] << " "; cout << sim[i+1] << " ";cout << sim[j+i] << " "; cout << endl;
                }
        }
        else {cout <<"[3]"<< sim[i] << " ";cout << sim[i+1] << " "; cout << endl;}
        */
}

}
//int cr_arr(int, int, int); //// объявление функции

int Prime(unsigned long n)
{
    // число
     int d;  // делитель
     int r;  // остаток от деления n на d
 
     scanf("%d", &n);
     d = 2;        // сначала делим на 2
     do {
        r = n % d;
        if (r != 0) d++;
     }
     while ( r != 0 ); // пока n не разделится на d
     if (d == n)
       printf("%d - Prostoe" ,n);
     else  printf("%d - Ne prostoe" ,n);
 }

int main(){
setlocale(LC_ALL,"Russian");
int mas[MAXSIZE+1],sim[MAXSIZE+1];//массив
unsigned long long int  n =28682112781481849;//12003169; //начало перебора
vector<int> arr(100);


unsigned long long int z;
z = Prime(n);
cout << "runtime = " << clock()/1000.0 << endl; // время работы программы
cout << z << endl;

z = sqrt((float)4);

cout << n%2 << endl;

//for (int i = 0; i < 2; ++i){
if (cheskCl(n)){
N=0;
cr_arr(mas,n);  //создаем  массив простых чисел
show_array(mas);
//cr_sim(mas,sim); //создаем симетрию
//show_array(sim);
//search_zerkalo(mas,sim,arr);
//memset(mas, 0, sizeof(mas));
//memset(mas, 0, sizeof(sim));

//firstNum=0;arr.clear();
}
//n++;}

return 0;
}
