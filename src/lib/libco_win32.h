/*
  libco_win32 : version 0.06 ~byuu (05/21/2006)
*/

typedef void (*thread_t);
typedef void (*thread_p)();

void     co_init();
void     co_term();
thread_t co_active();
thread_t co_create(thread_p coentry, unsigned int heapsize);
void     co_delete(thread_t cothread);
void     co_jump(thread_t cothread);
void     co_call(thread_t cothread);
void     co_return();
