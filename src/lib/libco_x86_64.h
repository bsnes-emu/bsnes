/*
  libco_x86_64 : version 0.10 ~byuu (2007-09-08)
  license: public domain
*/

#ifndef LIBCO_H
#define LIBCO_H

typedef void *cothread_t;

extern "C" cothread_t co_active();
extern "C" cothread_t co_create(unsigned int heapsize, void (*coentry)());
extern "C" void       co_delete(cothread_t cothread);
extern "C" void       co_switch(cothread_t cothread);

#endif
