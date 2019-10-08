/*
  libco v19 (2019-02-18)
  author: byuu
  license: ISC
*/

#ifndef LIBCO_H
#define LIBCO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* cothread_t;

cothread_t co_active();
cothread_t co_derive(void*, unsigned int, void (*)(void));
cothread_t co_create(unsigned int, void (*)(void));
void co_delete(cothread_t);
void co_switch(cothread_t);
unsigned int co_size(unsigned int);
void co_save(cothread_t, void*, unsigned int);
void co_load(cothread_t, const void*, unsigned int);

#ifdef __cplusplus
}
#endif

/* ifndef LIBCO_H */
#endif
