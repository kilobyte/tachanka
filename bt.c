#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#define ARRAYSZ(x) (sizeof(x)/sizeof((x)[0]))

static void *start[1024], *end[1024];
static int nm;
static void *pthread_start, *pthread_end; // hax!

extern void _start(void);

void read_maps(void)
{
    FILE *f = fopen("/proc/self/maps", "r");
    char exec;
    int inode;

    nm=0;
    while (fscanf(f, "%p-%p %*c%*c%cp %*x %*x:%*x %d", &start[nm], &end[nm], &exec, &inode) == 4)
    {
        char *file = 0;
        size_t dummy = 0;
        getline(&file, &dummy, f);
        printf("\e[%dm%p-%p\e[0m\n", (exec=='x')?93:33, start[nm], end[nm]);
        if (exec != 'x')
            continue;
        if (strstr(file, "libpthread"))
            pthread_start = start[nm], pthread_end = end[nm];
        if (nm++ >= ARRAYSZ(start))
            break;
    }
    fclose(f);
}

void bt(void)
{
    void **sp = __builtin_frame_address(0);
    void **bp = sp;
    printf("Frame 0 base: %p, ret: %p\n", (void*)bp, __builtin_return_address(0));

    // MurmurHash2 by Austin Appleby, public domain.
    const uint64_t M = 0xc6a4a7935bd1e995ULL;
    const int R = 47;
    uint64_t h = M;

    while (1)
    {
        void *addr = *sp;
        int s;
        for (s=0; s<nm; s++)
            if (start[s] > addr)
                break;
        if (s-- && addr < end[s])
        {
            printf("\e[35m%zd S: %p\e[0m\n", sp - bp, addr);
            if (addr == _start || addr >= pthread_start && addr < pthread_end)
            {
                printf("\e[95mTh-th-th-that's all folks!\e[0m\n");
                break;
            }

            uint64_t k = (uintptr_t)addr;
            k *= M;
            k ^= k >> R;
            k *= M;
            h ^= k;
            h *= M;
        }
        else
            printf("%zd !: %p\n", sp - bp, addr);
        sp++;
    }

#ifdef FINALIZE_HASH
    // Enable if we want "random" hash values.
    h ^= h >> R;
    h *= M;
    h ^= h >> R;
#endif

    printf("\e[31mhash=\e[1m%016lx\e[0m\n", h);
}

void* foo(void *dummy)
{
    bt();
    return 0;
}

int main()
{
    read_maps();

    pthread_t th;
    pthread_create(&th, 0, foo, 0);
    pthread_join(th, 0);
    foo(0);
    for (int i=0; i<2; i++)
        foo(0);

    return 0;
}
