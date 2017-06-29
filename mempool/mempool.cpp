#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

using namespace std;

int64_t get_mem_use(const char* msg)
{
  ifstream ifs;
  ifs.open("/proc/self/status");
  const char* mem_use_keyword="VmRSS:";
  int64_t mem_use = 0;
  char s[256];
  while(ifs.getline(s, sizeof(s))) {
    char* name = strstr(s, mem_use_keyword);
    if ( name != NULL) {
      cout << msg << s << std::endl;
      name += strlen(mem_use_keyword);
      mem_use = strtol(name, NULL, 10);
      mem_use <<= 10; // * 1024
    }
  }
  ifs.close();
  return mem_use;
}


size_t bsize = 4096;
// size_t rem = 8192 - bsize - 100;
size_t alloc_count = 1048576; // allocate

struct fake_raw_posix_aligned{
    char stub[8];
    void* data;
    fake_raw_posix_aligned() {
                           // align // size
        posix_memalign(&data, 4096, bsize);
        memset(data, 0, 0x1000);
    }
    ~fake_raw_posix_aligned() {
        free(data);
    }
};

int main()
{
    vector <fake_raw_posix_aligned*> allocs2;
    allocs2.resize(alloc_count);
    int64_t mem_use_after, mem_use_before, used, accounted = 0;
    time_t t;
    int al = 0, notal = 0;

    srand((unsigned) time(&t));

    mem_use_before = get_mem_use("Mem before2: ");
    for ( int64_t i = 0; i < alloc_count; ++i) {
        allocs2[i] = new fake_raw_posix_aligned();
        accounted += bsize;
        al++;
    }

    mem_use_after = get_mem_use("Mem after2: ");

    used = mem_use_after - mem_use_before;
    cout << "Mem actually used: " << used << " bytes" << std::endl;
    cout << "Mem that should be used: " << accounted << " bytes" << std::endl;
    cout << "Difference: " << used - accounted << " bytes, " <<
          (float) (used - accounted) / 1024 / 1024 / 1024 << " gb" << endl;

    for( int64_t i = 0; i < alloc_count; ++i) {
        delete allocs2[i];
    }
    allocs2.clear();

    return 0;
}
