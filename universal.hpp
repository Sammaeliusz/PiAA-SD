
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std::chrono;
#define tic auto start = high_resolution_clock::now();
#define toc auto stop = high_resolution_clock::now(); auto duration = duration_cast<microseconds>(stop - start); auto time = duration.count();
template<typename T>
void swap(T* a, T* b){
    T tmp = *a;
    *a = *b;
    *b = tmp;
}
template <typename T>
T min(T a, T b){
    if(a<b){
        return a;
    }
    return b;
}
template <typename T>
T max(T a, T b){
    if(a>b){
        return a;
    }
    return b;
}
template <typename T>
T* relokuj(T* data, int old_size, int new_size){
    T *tmp = new T[new_size];
    for(int i=0; i<min(new_size, old_size); i++){
        tmp[i] = data[i];
    }
    delete[] data;
    return tmp;
}
    