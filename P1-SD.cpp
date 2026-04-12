#include ".\universal.hpp"
#include "P1-SD.hpp"

int main(){
    int samplesize = 1000;
    start<OW_Node<int>>* list = new start<OW_Node<int>>[samplesize];
    start<TW_Node<int>>* list2 = new start<TW_Node<int>>[samplesize];
    dynamic_array<int>* d_array = new dynamic_array<int>[samplesize];
    //Inicjalizacja
    for(int i = 0; i<samplesize; i++){
        for(int j = 0; j<1000; j++){
            push_back(&d_array[i], j);
            push_back(&list[i], j);
            push_back(&list2[i], j);
        }
    }
    //Badanie czasu wstawiania
    {
    tic
    for(int i = 0; i<1000; i++){
        for(int j = 0; j<samplesize; j++){
            push_begin(&d_array[i], 1);
            //push_begin(&list[i], 1);
            //push_begin(&list2[i], 1);
            //push_back(&d_array[i], 1);
            //push_back(&list[i], 1);
            //push_back(&list2[i], 1);
            //push_at(&d_array[i], d_array[i].size/2, 1);
            //push_at(&list[i], list[i].size/2, 1);
            //push_at(&list2[i], list2[i].size/2, 1);
            
        }
    }
    toc
    std::cout<<"Wstawianie: "<<(long long int)time/samplesize<<" microseconds\n";
    }
    //Badanie czasu usuwania
    {
    tic
    for(int i = 0; i<1000; i++){
        for(int j = 0; j<samplesize; j++){
            pop_begin(&d_array[i]);
            //pop_begin(&list[i]);
            //pop_begin(&list2[i]);
            //pop_back(&d_array[i]);
            //pop_back(&list[i]);
            //pop_back(&list2[i]);
            //pop_at(&d_array[i], d_array[i].size/2);
            //pop_at(&list[i], list[i].size/2);
            //pop_at(&list2[i], list2[i].size/2);
        }
    }
    toc
    std::cout<<"Usuwanie: "<<time/samplesize<<" microseconds\n";
    }
    //Badanie czasu znajdowania
    std::cout<<d_array[0].size<<"\n";
    {
    tic
    for(int i = 0; i<1000; i++){
        for(int j = 0; j<samplesize; j++){
            find(&d_array[i], -15);
            //find(&list[i], -15);
            //find(&list2[i], -15);
        }
    }
    toc
    std::cout<<"Znajdowanie: "<<time/samplesize<<" microseconds\n";
    }
    return 0;
}
