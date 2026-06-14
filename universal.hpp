
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <vector>
#include <utility>
#include <random>
#include <thread>
#include <mutex>
#include <map>
#include <condition_variable>

using namespace std::chrono;
#define tic start_t = high_resolution_clock::now();
#define toc stop_t = high_resolution_clock::now(); minelo = duration_cast<nanoseconds>(stop_t - start_t); czas = minelo.count();
#define uint unsigned int
auto start_t= high_resolution_clock::now();
auto stop_t= high_resolution_clock::now();
auto minelo= duration_cast<nanoseconds>(stop_t - start_t); 
auto czas = minelo.count();
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
    T* tmp = new T[new_size];

    for(int i = 0; i < old_size && i < new_size; i++){
        tmp[i] = data[i];
    }

    delete[] data;
    return tmp;
}
