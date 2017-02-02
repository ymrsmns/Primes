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
#include "bigint/BigIntegerLibrary.hh"


using namespace std;
const int MAXSIZE=100;
int N; //глобпльная переменная
int firstNum =0;



bool cheskCl(int n){//проверка на простое число
    bool flag = true;
    float end = sqrt((float)n);
    for (int i=2; i <= end; ++i) if (n%i == 0) flag = false;
    return flag;
}

int Prime(unsigned long n)
{
		return false;
 }


int main(){
setlocale(LC_ALL,"Russian");
int mas[MAXSIZE+1],sim[MAXSIZE+1];//массив
//unsigned long long int  n =28682112781481849ULL;//12003169; //начало перебора
vector<int> arr(100);





//unsigned long long int z;
int z = 12003169;

z = z/2;
cout << "runtime = " << clock()/1000.0 << endl; // время работы программы
cout << z << endl;

cout << "======================================" << endl;


typedef vector<int> lnum;
const int base = 1000*1000*1000;



int carry = 0;
for (int i=(int)a.size()-1; i>=0; --i) {
	long long cur = a[i] + carry * 1ll * base;
	a[i] = int (cur / b);
	carry = int (cur % b);
}
while (a.size() > 1 && a.back() == 0)
	a.pop_back();


return 0;
}
