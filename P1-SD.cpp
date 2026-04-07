#include ".\universal.hpp"
#include "P1-SD.hpp"

int main(){
    int samplesize = 10000;
    start<OW_Node<int>>* list = new start<OW_Node<int>>[samplesize];
    start<TW_Node<int>>* list2 = new start<TW_Node<int>>[samplesize];
    dynamic_array<int>* d_array = new dynamic_array<int>[samplesize];
    long long int tot_time = 0;
    for(int i = 0; i<samplesize; i++){
        for(int j = 0; j<1000; j++){
            push_back(&d_array[i], j);
            push_back(&list[i], j);
            push_back(&list2[i], j);
        }
    }
    tic
    for(int i = 0; i<samplesize; i++){
        push_back(&list[i], i);
    }
    toc
    std::cout<<"Wstawianie: "<<time<<"\n";
    std::cout<<"rozmiar: "<<list[0].size<<"\n";
    /*
    tot_time = 0;
    for(int i = 0; i<samplesize; i++){
        tic
        for(int j = 0; j<=10000; j++){
            //printf("Pushing %d to dynamic array %d\n", j, i);
            pop_at(&d_array[i],7);
        }
        toc
        tot_time += time;
    }
    std::cout<<"Usuwanie: "<<tot_time/samplesize<<"\n";
    tot_time = 0;
    for(int i = 0; i<samplesize; i++){
        tic
        for(int j = 0; j<=10000; j++){
            find(&list[i], j*10);
        }
        toc
        tot_time += time;
    }
    std::cout<<"Znajdowanie: "<<tot_time/samplesize<<" microseconds\n";*/
    return 0;
}
