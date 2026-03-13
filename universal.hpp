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