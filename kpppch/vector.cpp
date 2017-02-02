#include <iostream>
#include <vector>
#include<iterator>
//работа с файлами
#include <fstream>
#include <string>
//определяем тип данных
#include <typeinfo>

#include <cmath>
#include <iostream>
#include <locale>

using namespace std;
ifstream in("INPUT.TXT");

//при 100 на эталоне вылезает лишний патер июъб использовать как проверку
const int MAXSIZE=100;

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

void readlong(int *a){
    int i;
    string s;
    in>>s;
    a[0]=s.length();

    for (int i=1; i<=a[0]; ++i)
        a[a[0]-i+1]=s[i-1]-48;
}

bool cheskCl(int n){
    bool flag = true;
    float end = sqrt((float)n);
    for (int i=2; i <= end; ++i) {
        if (n%i == 0) flag = false;
    }
//debug    if(flag) cout << "PR: " << n << endl;
    return flag;
}


int main()
{
setlocale(LC_ALL,"Russian");
unsigned long long  S = 12003179ULL;
int a[MAXSIZE], N, Zsim,Csim, mas[MAXSIZE+1],sim[MAXSIZE+1],R[MAXSIZE+1],firstNum;//массив
int n = 12003178;
vector<int> arr(100);
Zsim = 0;
N=0;
firstNum=0;


for(int i=n; i<=n+MAXSIZE; i++){
	if(cheskCl(i)){
	 if(firstNum == 0 )firstNum=i; //записываем первое простое число число
	 mas[N]=i-firstNum;
	 N++;
	}
}
cout << "etalon: 12003179: 0, 12, 18, 30, 42, 48, 60" << endl;
///проверку на симетрию
cout << "simetria check" << endl;
//вывод массива
//Ищим разницу
for (int i = 0; i < N; ++i){
	if(mas[i]){ 
		int r = mas[i]-mas[i-1];
		if ( r > 0 ) sim[i]=r;
		else sim[i] = 0;
	}
	else { sim[i] = 0;}
}
//вывод массива
cout << firstNum << ": sim=>:  ";
for (int i = 1; i < N; ++i) if(sim[i])cout << sim[i] << " "; 
cout << endl;

for (int i = 0; i < N; ++i){
	if(sim[i] == sim[i+1]) {cout << "[1]nashli simetrii bez centera: " << sim[i] ;cout << " " << sim[i+1] << " "; cout << endl;
		int z1= i; int z2= i+1; 
		arr[30]=mas[i];  arr[31]=mas[i+1]; //записываем 2 значения в вектор
		// cout << "[1.1] id sim:" << z1 << " "; cout  << z2 << " "; cout << endl;
		for (int j = 0; j < N; ++j){//проверяем разначения по краям
                        if(sim[z1-j] == sim [z2+j]) {
					Zsim++;
                 		//	cout << "[1/2] i:" << i << " ";
				//	cout << " j:" <<j <<" "; 
 				//	cout << " Zsim:" <<Zsim <<" ";
				//	cout << sim[z1-j] << " "; cout << sim[z2+j] << " ";  cout << endl;
					arr[31+j]=mas[z2+j];
					arr[30-j]=mas[z1-j];
			}
			else if (sim[z1-j] != sim [z2+j] &&  Zsim >0 ){break;}
                }
		//записываем последовательность
		show_vector(arr);//чистим вектор
		 for (int j = 0; j < arr.size(); ++j) if(arr[j] != 0) {arr.erase(arr.begin()+1,arr.begin()+j);break;}
		 for (int j = 1; j < arr.size(); ++j) if(arr[j] == 0) {arr.erase(arr.begin()+j,arr.begin()+arr.size() );break;}
	 	cout << "clear vector:" <<endl ;
		show_vector(arr);
	}/*
	else if(sim[i] == sim[i+2]) {cout << "[2]nashli center simetrii: " << sim[i] ; cout << " " << sim[i+1] << " ";cout << sim[i+2] << " "; cout << endl;
	        for (int j = i+1; j < N; ++j){//проверяем разначения по краям
			cout << "[2/1]: " << sim[j-1] << " "; cout << sim[i+1] << " ";cout << sim[j+i] << " "; cout << endl;
		}
	}
        else {cout <<"[3]"<< sim[i] << " ";cout << sim[i+1] << " "; cout << endl;}
	*/
}

//вывод массива
cout << firstNum << ":  ";
for (int i = 0; i < N; ++i) if(mas[i])cout << mas[i] << " ";
cout << endl;
//вывод массива
cout << firstNum << ":  ";
for (int i = 0; i < N; ++i) if(sim[i])cout << sim[i] << " ";
cout << endl;
cout << "" << endl;

}

