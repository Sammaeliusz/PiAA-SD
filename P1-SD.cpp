#include ".\universal.hpp"
#include "P1-SD.hpp"
#include <typeinfo>
#include <type_traits>

int main(){
    start<OW_Node<int>>* list = new start<OW_Node<int>>;
    start<TW_Node<int>>* list2 = new start<TW_Node<int>>;
    dynamic_array<int>* d_array = new dynamic_array<int>;
    tic
    for(int i = 0; i<=100000; i++){
        push_begin(list, i);
    }
    toc
    std::cout<<dur<<"\n";
    tic
    for(int i = 0; i<=100000; i++){
        push_back(list, i);
    }
    toc
    std::cout<<dur<<"\n";
    return 0;
}