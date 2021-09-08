/* Very minimal pthread implementation for Windows */
#include <Windows.h>
#include <assert.h>

typedef HANDLE pthread_t;
typedef struct pthread_attr_s pthread_attr_t;

static inline int pthread_create(pthread_t *pthread,
                                 const pthread_attr_t *attrs,
                                 LPTHREAD_START_ROUTINE function,
                                 void *context)
{
    assert(!attrs);
    *pthread = CreateThread(NULL, 0, function,
                            context, 0, NULL);
    return *pthread? 0 : GetLastError();
}


typedef struct {
    unsigned status;
    CRITICAL_SECTION cs;
} pthread_mutex_t;
#define PTHREAD_MUTEX_INITIALIZER {0,}

static inline CRITICAL_SECTION *pthread_mutex_to_cs(pthread_mutex_t *mutex)
{
retry:
    if (mutex->status == 2) return &mutex->cs;
    
    unsigned expected = 0;
    unsigned desired = 1;
    if (__atomic_compare_exchange(&mutex->status, &expected, &desired, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
        InitializeCriticalSection(&mutex->cs);
        mutex->status = 2;
        return &mutex->cs;
    }
    goto retry;
}

static inline int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    EnterCriticalSection(pthread_mutex_to_cs(mutex));
    return 0;
}

static inline int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    LeaveCriticalSection(pthread_mutex_to_cs(mutex));
    return 0;
}

typedef struct {
    unsigned status;
    CONDITION_VARIABLE cond;
} pthread_cond_t;
#define PTHREAD_COND_INITIALIZER {0,}

static inline CONDITION_VARIABLE *pthread_cond_to_win(pthread_cond_t *cond)
{
retry:
    if (cond->status == 2) return &cond->cond;
    
    unsigned expected = 0;
    unsigned desired = 1;
    if (__atomic_compare_exchange(&cond->status, &expected, &desired, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
        InitializeConditionVariable(&cond->cond);
        cond->status = 2;
        return &cond->cond;
    }
    goto retry;
}


static inline int pthread_cond_signal(pthread_cond_t *cond)
{
    WakeConditionVariable(pthread_cond_to_win(cond));
    return 0;
}

static inline int pthread_cond_wait(pthread_cond_t *cond,
                                    pthread_mutex_t *mutex)
{
    // Mutex is locked therefore already initialized
    return SleepConditionVariableCS(pthread_cond_to_win(cond), &mutex->cs, INFINITE);
}
