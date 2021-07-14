#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef pthread_mutex_t os_mutex_t;

#define Malloc malloc
#define Free free

static void *Zalloc(size_t s)
{
	void *m = Malloc(s);
	if (m)
		memset(m, 0, s);
	return m;
}

#define ERR(x) do fprintf(stderr, x); while(0)

#define util_mutex_init(x)	pthread_mutex_init(x, NULL)
#define util_mutex_destroy(x)	pthread_mutex_destroy(x)
#define util_mutex_lock(x)	pthread_mutex_lock(x)
#define util_mutex_unlock(x)	pthread_mutex_unlock(x)
#define util_atomic_load_explicit32	__atomic_load
#define util_atomic_load_explicit64	__atomic_load
#define util_atomic_store_explicit32	__atomic_store_n
#define util_atomic_store_explicit64	__atomic_store_n
#define util_fetch_and_add32	__sync_fetch_and_add
#define util_fetch_and_add64	__sync_fetch_and_add
#define util_fetch_and_sub32	__sync_fetch_and_sub
#define util_fetch_and_sub64	__sync_fetch_and_sub
#define util_fetch_and_and32	__sync_fetch_and_and
#define util_fetch_and_and64	__sync_fetch_and_and
#define util_fetch_and_or32	__sync_fetch_and_or
#define util_fetch_and_or64	__sync_fetch_and_or
#define util_lssb_index64(x)	((unsigned char)__builtin_ctzll(x))
#define util_mssb_index64(x)	((unsigned char)(63 - __builtin_clzll(x)))


#define NOFUNCTION do ; while(0)

// Make these an unthing for now...
#define ASSERT(x) NOFUNCTION
#define ASSERTne(x, y) ASSERT(x != y)
#define VALGRIND_ANNOTATE_NEW_MEMORY(p, s) NOFUNCTION
#define VALGRIND_HG_DRD_DISABLE_CHECKING(p, s) NOFUNCTION
