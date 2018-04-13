
#include "include/range_process.h"

long calc_durtime(unsigned char in_sec) {
    struct timeval tend;
    gettimeofday(&tend, NULL);
    long dd = tend.tv_sec * 100000 + tend.tv_usec - (tstart.tv_sec * 100000 + tstart.tv_usec);
    return (in_sec == 0) ? dd : dd /100000;
}
void display_result(unsigned long long* pbuf) {
    cout << "\tpos\tnvar\t10k\t100k\t1m"<<endl;
    unsigned long long* p;
    for (unsigned int iv = 0; iv < sizeof(offset_vals) / sizeof(int); iv++) {
        p = pbuf + (offset_vals[iv] * NUM_FIELDS);
        for (unsigned int i = 0; i < (DOFFSET * 2) + 1;  i++) {
            cout << "[" << offset_vals[iv] + i << "]";
            for (unsigned int j = 0; j < NUM_FIELDS; j++) {
                cout << "\t" << *p++;
            }
            cout << endl;
        }
        cout << "..." << endl;
    }
}
void display_result_all(unsigned long long* pbuf, unsigned int ln) {
    cout << "\tpos\tnvar\t10k\t100k\t1m"<<endl;
    unsigned long long* p = pbuf;
    for (unsigned int i = 0; i < ln;  i++) {
        cout << "[" << i << "]";
        for (unsigned int j = 0; j < NUM_FIELDS; j++) {
            cout << "\t" << *p++;
        }
            cout << endl;
    }
}
void last_2(unsigned long long* pbuf, unsigned long idx) {
    for (unsigned int i = 0; i < 2; i++) {
        std::cout << " [" << idx + i << "]=";
        unsigned long long* p = pbuf + (NUM_FIELDS * (idx + i));
        for ( int j = 0; j < NUM_FIELDS; j++) {
            cout << *p++ <<",";
        }
    }
}
void show_progress(unsigned long long* pb, unsigned long cidx, unsigned long& lidx) {
    long delta = calc_durtime(0);   // in microsec
    cout << cidx << ":" << delta <<"us)";
    last_2(pb, cidx - 1);
    if (lidx > 0) {
        last_2(pb, lidx - 1);
    } else {
        cout << ", no last";
    }
    lidx = cidx;
    cout << endl;
}