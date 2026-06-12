#include "P3-SD.hpp"
int main(){
    hash_table_lancuch_two<int, int> table1;
    hash_table_otwarte<int, int> table2;
    for(int i=0; i<100; i++){
        printf("Element with key %d and value %d inserted\n", 17*i, i);
        table1.insert(17*i, i);
        table2.insert(17*i, i);
    }
    printf("%d %d\n", table1.get(17*5), table2.get(17*5));
}