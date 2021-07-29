#include <pthread.h>
#include <stdio.h>
#include "critnib.h"
#include "bt.h"

static struct tblock
{
    void *addr;
    size_t size;
    long accesses;
    int next;
} tblocks[MAXBLOCKS];
static int nblocks = 0;

static critnib *htb, *mtb;

void *nb(size_t size)
{
    uint64_t h = bthash(size);
    void *addr = malloc(size);
    if (!addr)
        return 0;
    struct tblock *bl = &tblocks[nblocks++];
    critnib_insert(mtb, (uintptr_t)addr, bl, 0);
    critnib_insert(htb, h, bl, 0);
}

void* foo(void *dummy)
{
    uint64_t h = bthash(0);
    uint64_t cnt = (uintptr_t)critnib_get(c, h);
    //racey
    critnib_insert(c, h, (void*)++cnt, 1);
    printf("\e[31mhash=\e[1m%016lx\e[22m cnt=\e[1m%zd\e[0m\n", h, cnt);
    return 0;
}

int main()
{
    read_maps();
    htb = critnib_new();
    mtb = critnib_new();

    pthread_t th;
    pthread_create(&th, 0, foo, 0);
    pthread_join(th, 0);
    foo(0);
    for (int i=0; i<2; i++)
        foo(0);

    critnib_delete(c);
    return 0;
}
