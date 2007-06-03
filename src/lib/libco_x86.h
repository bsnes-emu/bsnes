/*
  libco_x86 : version 0.10 ~byuu (2007-04-18)
*/

#ifndef LIBCO_H
#define LIBCO_H

#define cocall fastcall
typedef void (*cothread_t);

extern "C" cothread_t cocall co_active();
extern "C" cothread_t cocall co_create(unsigned int heapsize, void (cocall *coentry)(void *data), void *data);
extern "C" void       cocall co_delete(cothread_t cothread);
extern "C" void       cocall co_switch(cothread_t cothread);

#endif
