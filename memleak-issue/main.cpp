#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <vector>
#include <gperftools/malloc_extension.h>

using namespace std;

pthread_barrier_t barrier;

struct alloc_args {
    int bs;
    int count;
};

void usage()
{
    cout << "./main [num threads]" << endl;
}

void *book(void *a)
{
    struct alloc_args *args;
    char *block;
    vector<char*> v;
    args = (struct alloc_args*) a;
    cout << args->bs << endl;

    for (int i = 0; i < args->count; i++) {
        block = new char[args->bs];
        memset(block, 0, args->bs);
        usleep(1000);
        v.push_back(block);
    }

    for (int i = 0; i < args->count; i++) {
        delete v[v.size() - 1];
        v.pop_back();
    }
    
    pthread_barrier_wait(&barrier);
    while (1) {
        sleep(2);
    }
}

void unbook()
{
}

int main(int argc, char **argv)
{
    pthread_t *threads;
    struct alloc_args thread_arg;
    int num_threads = 10;

    num_threads = atoi(argv[1]);
    thread_arg.bs = 10486;
    thread_arg.count = 20000;

    threads = new pthread_t[num_threads];
    pthread_barrier_init(&barrier, NULL, num_threads + 1);

    cout << "Allocating and freeing memory" << endl;
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, book, (void*)&thread_arg);
    }

    pthread_barrier_wait(&barrier);
    malloc_stats();
    cout << endl << "ReleaseFreeMemory()" << endl << endl;
    MallocExtension::instance()->ReleaseFreeMemory();
    malloc_stats();

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    delete threads;

    return 0;
}
