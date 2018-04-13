#include "include/range_process.h"

void aggregate(unsigned long long* const pbuf, unsigned int nlines, unsigned int offset, unsigned int nloop, unsigned int nstep) {
      // nloop = 10; cur for filling
    unsigned int nstep_sz = NUM_FIELDS * nstep;              // 5 * 10
    unsigned long long * p;
    unsigned long long * p4add = pbuf + offset;
    cout<<"aggregate: nlines="<<nlines<<", offset="<<offset<<", nstep="<<nstep<<",nloop="<<nloop<<endl;
    try {
        for (unsigned int k = 0; k < nlines; k++) {
        // for (; p4add < (pbuf + (NUM_FIELDS*nlines)); ) {
            p = p4add - 1; 
            for (int i = 0; i < nloop; i++) {
                *p4add +=  *p;
                p += nstep_sz;
            }
            // if ((k > 3990000) && (k % nstep == 2)) {
            //     cout<<"k="<<k<<", nlines="<<nlines<<", pbuf="<<pbuf<<", p4add="<<p4add<<", p="<<p<<endl;
            // }
            p4add += NUM_FIELDS;
        }
    } catch (...) {
        cout<<"exception: pbuf="<<pbuf<<", p4add="<<p4add<<", p="<<p<<endl;
    }
}

unsigned long parse_data_3(const char * fpath) {
    unsigned long long * pcur = pbuff;  // cur for filling
    unsigned long long * pto10K;
    std::ifstream ifs(fpath);
    unsigned long pos, n2, n3, nvar;
    unsigned long last_i = 0ul;

    unsigned long long * p4add;          // last for adding, go backwards
    unsigned long idx = 0;
    unsigned long n_totallines = 0;

    cout<<"... in "<< __FUNCTION__<<endl;
    while (ifs >> pos >> n2 >> n3 >> nvar) {
        p4add = pcur - NUM_FIELDS;    
        *pcur++ = pos;
        *pcur++ = nvar;
        *pcur++ = nvar;
        *pcur++ = 0;
        *pcur++ = 0;
        pto10K = (   (pcur - pbuff) >= LWIN10K) ? pcur - LWIN10K : pbuff;
        while (p4add >= pto10K) {
            *(p4add + NUM_FIELDS - 3) += nvar;      // fill 10k
            p4add -= NUM_FIELDS;      // go backwards 
        }
        ++idx;
#if defined(MY_TEST_ONLY) || defined(_WIN32)
        if (idx == MAX_LINES) {
            break;
        }
#else
        if (idx % DISPLAY_FOR == 2) {             // dsiplay
            show_progress(pbuff, idx-1, last_i);  // display cur and last
        }
#endif
    }
    ifs.close();
    cout << ".. 1) finished WIN10K in " << calc_durtime() << "s, for " << idx << " lines." << endl;

    aggregate(pbuff, idx-WIN100K+1, NUM_FIELDS-2, WIN100K/WIN10K, WIN10K);
    cout << ".. 2) finished WIN100K in " << calc_durtime() << "s, for " << idx-WIN10K+1 << " lines." << endl;

    aggregate(pbuff, idx-WIN1M+1, NUM_FIELDS-1, WIN1M/WIN100K, WIN100K);
    cout << ".. 2) finished WIN1M in " << calc_durtime() << "s, for " << idx-WIN100K+1 << " lines." << endl;
    return idx;
}

unsigned long parse_data_2(const char * fpath) {
    unsigned long long * pcur = pbuff;  // cur for filling
    unsigned long long * pto10K;
    unsigned long long * p4add100k, *p4add1M;
    std::ifstream ifs(fpath);
    unsigned long pos, n2, n3, nvar;
    unsigned long last_i = 0ul;

    unsigned long long * p4add;          // last for adding, go backwards
    unsigned long idx = 0;

    cout<<"... in "<< __FUNCTION__<<endl;
    while (ifs >> pos >> n2 >> n3 >> nvar) {
        p4add = pcur - NUM_FIELDS;    
        *pcur++ = pos;
        *pcur++ = nvar;
        *pcur++ = nvar;
        *pcur++ = 0;
        *pcur++ = 0;
        pto10K = (   (pcur - pbuff) >= LWIN10K) ? pcur - LWIN10K : pbuff;
        while (p4add >= pto10K) {
            *(p4add + NUM_FIELDS - 3) += nvar;      // fill 10k
            if (idx >= WIN100K) {
                p4add100k = p4add + NUM_FIELDS - LWIN100K + NUM_FIELDS - 2;
                for (int j = 0; j < WIN100K/WIN10K; j++) {  // only add 10 of 10k val
                    *p4add100k += *(p4add - (LWIN10K * j) + NUM_FIELDS -3);
                }
            }
            if (idx >= WIN1M) {
                p4add1M = p4add + NUM_FIELDS - LWIN1M + NUM_FIELDS - 1;    // filed for add
                for (int j = 0; j < WIN1M/WIN100K; j++) {
                    *p4add1M += *(p4add - (LWIN100K * j) + NUM_FIELDS -2);
                }
            }
            p4add -= NUM_FIELDS;      // go backwards 
        }
        ++idx;
#if defined(MY_TEST_ONLY) || defined(_WIN32)
        if (idx == MAX_LINES) {
            break;
        }
#endif
        if (idx % DISPLAY_FOR == 2) {             // dsiplay
            show_progress(pbuff, idx-1, last_i);  // display cur and last
        }
    }
    ifs.close();
    cout << "Done parsing 2 in " << calc_durtime() << "s, for " << idx << " lines." << endl;
    return idx;
}

// generic solution
unsigned long parse_data_1(const char * fpath) {
    unsigned long long * pcur = pbuff, *p, *pto;
    cout<<"... in "<< __FUNCTION__<<endl;
    std::ifstream ifs(fpath);
    unsigned long pos, n2, n3, nvar;
    unsigned long i = 0, last_i = 0ul;
    while (ifs >> pos >> n2 >> n3 >> nvar) {
        p = pcur - NUM_FIELDS;
        *pcur++ = pos;
        *pcur++ = nvar;
        *pcur++ = nvar;
        *pcur++ = nvar;
        *pcur++ = nvar;
        pto = ((pcur - pbuff) >= LWIN1M) ? pcur - LWIN1M : pbuff;
        while (p >= pto) {
            *(p + NUM_FIELDS - 1) += nvar;
            if ((pcur - p) <= LWIN100K) {
                *(p + NUM_FIELDS - 2) += nvar;
            }
            if ((pcur - p) <= LWIN10K) {
                *(p + NUM_FIELDS - 3) += nvar;
            }
            p -= NUM_FIELDS;
        }
#if defined(MY_TEST_ONLY) || defined(_WIN32)
        if (++i == MAX_LINES) {
            break;
        }
#endif
        #ifdef SHOW_PROGRESS
        if (i % DISPLAY_FOR == 2) {                       // dsiplay
            show_progress(pbuff, i-1, last_i);
        }
        #endif
    }
    ifs.close();
    cout << "Done parsing 1 in " << calc_durtime() << "s, for " << i << " lines." << endl;
    return i;
}
// generic solution, BUT incomplete - only calc 10k field
unsigned long parse_data_0(const char * fpath) {
    cout<<"... in "<< __FUNCTION__<<endl;
    unsigned long long * pcur = pbuff, *p, *pto;
    std::ifstream ifs(fpath);
    unsigned long pos, n2, n3, nvar;
    unsigned long i = 0, last_i = 0ul;
    while (ifs >> pos >> n2 >> n3 >> nvar) {
        p = pcur - NUM_FIELDS;
        *pcur++ = pos;
        *pcur++ = nvar;
        *pcur++ = nvar;
        *pcur++ = nvar;
        *pcur++ = nvar;
        pto = ((pcur - pbuff) >= LWIN10K) ? pcur - LWIN10K : pbuff;
        while (p >= pto) {
            *(p + NUM_FIELDS - 3) += nvar;
            p -= NUM_FIELDS;
        }
#if defined(MY_TEST_ONLY) || defined(_WIN32)
        if (++i == MAX_LINES) {
            break;
        }
#endif
        #ifdef SHOW_PROGRESS
        if (i % DISPLAY_FOR == 2) {                       // dsiplay
            show_progress(pbuff, i-1, last_i);
        }
        #endif
    }
    ifs.close();
    cout << "Done parsing in " << calc_durtime() << "s, for " << i << " lines." << endl;
    return i; 
}
