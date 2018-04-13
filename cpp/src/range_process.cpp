#include "include/range_process.h"

struct timeval tstart;
typedef unsigned long(*add_func)(const char * fpath);
static struct stat sbuf;
static char msgbuf[512];
static add_func funcs[] = {&parse_data_0, &parse_data_1, &parse_data_2, &parse_data_3};

unsigned long process(const char* fpath, unsigned int option) {
    if (option < sizeof(funcs)) {
        cout << "input file is " << fpath << endl;
        if (stat(fpath, &sbuf) == 0) {
            unsigned long  num_lines = min <unsigned long>(sbuf.st_size / 13ul, MAX_LINES);
            cout << "st_size = "<<sbuf.st_size<<" B, num_lines = " << num_lines << ", RUN_PARSER="<<option<<endl;
            pbuff = (unsigned long long*) malloc(sizeof(unsigned long long) * num_lines * NUM_FIELDS);
            num_lines = (*funcs[option])(fpath);
            // switch (RUN_PARSER) {
            //     case 0:
            //         num_lines = parse_data_0(fpath);
            //         break;
            //     case 1:
            //         num_lines = parse_data_1(fpath);
            //         break;
            //     case 2:
            //         num_lines = parse_data_2(fpath);
            //         break;
            //     default:
            //         num_lines = parse_data_0(fpath);
            // }
            string ofname = fpath + outext;
            ofstream ofs(ofname.c_str(), ios::binary);
            size_t outsize = sizeof(unsigned long long) * num_lines * NUM_FIELDS;
            ofs.write(reinterpret_cast<const char*>(pbuff), outsize);
            ofs.close();
            cout << "saved calculated ranges to " << ofname << ", " <<outsize<<" B" << endl;
            if (num_lines > 300) {
                display_result(pbuff); 
            } else {
                display_result_all(pbuff, num_lines);
            }
            free(pbuff);
            return num_lines;
        } else {
            cout << "could not find file " << fpath << endl;
            return 0;
        }
    } else {
        sprintf(msgbuf, "#func=%d, , option=%d", sizeof(funcs), option);
        throw new string(msgbuf);
    }
}
static const char* path_at_host[] = {"/mnt/app_ssd/scratch/illumina/", "/mnt/app_hdd1/scratch/illumina/", "/home/mingrutar/GenomicsDBPerfTest/dev_data/illumina/", NULL};
unsigned long long* pbuff = NULL;

static char pathbuf[PATH_MAX];
const  char* find_input(const char* psubpath) {
    const char** p = path_at_host;
    do {
        strcpy(pathbuf, *p++);
        strcat(pathbuf, psubpath);
        if (stat(pathbuf, &sbuf) == 0) {
            printf("..found %s", pathbuf);
            return pathbuf;
        } 
    } while (*p != NULL);
    sprintf(msgbuf, "cannot find path for %s", psubpath);
    throw new string(msgbuf);
}
int main(int argc, char *argv[]) {
    unsigned int sel_option = RUN_PARSER;
    if (argc > 1) {
        sel_option = atoi(argv[1]);
    }
    gettimeofday(&tstart, NULL);
    unsigned long nlines = 0;
    try {
    #ifdef _WIN32
        nlines = process(MY_INTPOS_FPATH, sel_option);
    #else
        nlines = process(find_input(MY_INTPOS_FPATH), sel_option);
    #endif
        cout << "Done in " << calc_durtime() <<"s, " << nlines << " lines." << endl;
    } catch (string exstr) {
        cout << "caught exception: " << exstr << endl;
    }
    return 0;
}