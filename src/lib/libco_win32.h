/*
  libco_win32 : version 0.09 ~byuu (2007-01-19)
*/

#define COTHREAD_STACKSIZE_TINY      0x1000
#define COTHREAD_STACKSIZE_SMALL     0x4000
#define COTHREAD_STACKSIZE_NORMAL   0x10000
#define COTHREAD_STACKSIZE_LARGE    0x40000
#define COTHREAD_STACKSIZE_HUGE    0x100000

typedef void (*cothread_t);
typedef void (*cothread_p)(void);

cothread_t co_active();
cothread_t co_create(cothread_p coentry, unsigned int heapsize);
void       co_delete(cothread_t cothread);
void       co_switch(cothread_t cothread);
void       co_exit(cothread_t cothread);
