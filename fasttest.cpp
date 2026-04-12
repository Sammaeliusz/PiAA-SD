#include "universal.hpp"
int a[10] = {1,2,3,4,5,6,7,8,9,10};
int main(){
    for(int i=0; i<10; i++){
        std::cout<<a[i]<<" ";
    }
    swap(a, a+1);
    for(int i=0; i<10; i++){
        std::cout<<a[i]<<" ";
    }
    return 0;
}