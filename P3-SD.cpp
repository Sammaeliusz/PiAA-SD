#include "P3-SD.hpp"
#define SIZE 100
#define ilosc 50000
int main(int argc, char* argv[]){
    hash_table_otwarte<int, int> table1[SIZE];
    hash_table_lancuch<int, int> table2[SIZE];
    hash_table_lancuch_two<int, int> table3[SIZE];
    long long unsigned int sum_time[6];
    for(int tab=0; tab<SIZE; tab++){
        for(int i=0; i<ilosc; i++){
            //printf("Element with key %d and value %d inserted\n", 17*i, i);
            table1[tab].insert(i, i);
            //table2[tab].insert(i, i, 0);
            //table3[tab].insert(i, i,0);
        }
        tic
            table1[tab].insert(ilosc, ilosc+1);
        toc
        sum_time[0]+=czas;
        tic
            table2[tab].insert(ilosc, ilosc+1,0);
        toc
        sum_time[1]+=czas;
        tic
            table3[tab].insert(ilosc+1,ilosc+1,1);
        toc
        sum_time[2]+=czas;
        tic
            table1[tab].remove(ilosc);
        toc
        sum_time[3]+=czas;
        tic
            table2[tab].remove(ilosc+1,0);
        toc
        sum_time[4]+=czas;
        tic
            table3[tab].remove(ilosc+1,0);
        toc
        sum_time[5]+=czas;
        printf("Round %d done.\n", tab);
    }
    printf("Dodawanie %llu %llu %llu\n", sum_time[0]/SIZE, sum_time[1]/SIZE, sum_time[2]/SIZE);
    printf("Usuwanie %llu %llu %llu\n", sum_time[3]/SIZE, sum_time[4]/SIZE, sum_time[5]/SIZE);
}