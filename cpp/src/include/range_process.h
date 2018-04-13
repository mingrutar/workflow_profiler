#ifndef __RANGE_PROCESS_H__
#define __RANGE_PROCESS_H__

// test:  g++ -O3 -std=c++11  -o build/trange2 -DMY_TEST_ONLY -I ./src/include src/*.cpp && build/trange2 | tee ../logs/trange2.log
// gdb: g++ -O3 -o build/trange_d --std=c++11 -ggdb  -DMY_TEST_ONLY -I ./src/include src/*.cpp
// run:  g++ -O3 -std=c++11  -o ../build/range -I ./include *.cpp && ../build/range | tee ../logs/range1201_14.log
// on compute-x-xx: ~/bin/trange.bash ddd      # full
//                  ~/bin/trange.bash          # TEST

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdint>
#include <fstream>
#include <iostream>

using namespace std;

// #define MY_TEST_ONLY 1
// #define SHOW_PROGRESS 1  

// copied to C:\Users\mrutarx\myprojects\GenomicsDBPerfTest\dev_apps\py_scratch\cpp
// wc -l merged_16p_10000.intpos => 145418043  
// parser: WIN[0]: 4M=1879 sec; WIN[2] O3: 4M=740s
const int RUN_PARSER = 3;               // parser 0 , 1
const unsigned int NUM_FIELDS = 5;

#ifdef _WIN32
    #define PATH_MAX 1024
    #ifdef MY_TEST_ONLY
        const char* const MY_INTPOS_FPATH = 
        "C:\\Users\\mrutarx\\myprojects\\GenomicsDBPerfTest\\dev_data\\query_density\\1000g_interesting_positions.200";
        const unsigned int WIN10K = 5;
        const unsigned int WIN100K = 10;
        const unsigned int WIN1M = 20;
    #else
        const char* const MY_INTPOS_FPATH = 
        "C:\\Users\\mrutarx\\myprojects\\GenomicsDBPerfTest\\dev_data\\illumina\\interesting_pos\\sample10000_28\\interesting_pos_3";
        const unsigned int WIN10K = 1000;
        const unsigned int WIN100K = 10000;
        const unsigned int WIN1M = 100000;
    #endif
    const unsigned long MAX_LINES = 4000000ul;
#else
    #ifdef MY_TEST_ONLY
        const char* const MY_INTPOS_FPATH = "interesting_pos/sample10000_28/interesting_pos_3";
        const unsigned int WIN10K = 1000;
        const unsigned int WIN100K = 10000;
        const unsigned int WIN1M = 100000;
        const unsigned long MAX_LINES = 4000000ul;
    #else
        const char* const MY_INTPOS_FPATH = "interesting_pos/merged_16p_10000.intpos";
        const unsigned int WIN10K =  10000;
        const unsigned int WIN100K = 100000;
        const unsigned int WIN1M =   1000000;
        const unsigned long MAX_LINES = 150000000ul;
    #endif
#endif

const unsigned int DISPLAY_FOR = WIN1M;
// const unsigned int ROW_BSIZE = sizeof(long long) * NUM_FIELDS; 
// const unsigned long WIN10K_BSZ = WIN10K * ROW_BSIZE;
// const unsigned long WIN100K_BSZ = WIN100K * ROW_BSIZE;
// const unsigned long WIN1M_BSZ = WIN1M * ROW_BSIZE;

const unsigned long LWIN10K = WIN10K * NUM_FIELDS;
const unsigned long LWIN100K = WIN100K * NUM_FIELDS;
const unsigned long LWIN1M = WIN1M * NUM_FIELDS;

const int DOFFSET = 2;
const unsigned int offset_vals[] = {(WIN10K - DOFFSET), (WIN100K - DOFFSET), (WIN1M - DOFFSET)};
const string outext = ".range";

extern struct timeval tstart;
extern unsigned long long* pbuff;

long calc_durtime(unsigned char in_sec = 1);
void display_result(unsigned long long* pbuf);
void display_result_all(unsigned long long* pbuf, unsigned int ln);
void show_progress(unsigned long long* pb, unsigned long cidx, unsigned long& lidx);

// parsing
unsigned long parse_data_0(const char * fpath);
unsigned long parse_data_1(const char * fpath);
unsigned long parse_data_2(const char * fpath);
unsigned long parse_data_3(const char * fpath);

#endif