#ifndef NALL_THREAD_HPP
#define NALL_THREAD_HPP

#include <nall/platform.hpp>
#include <nall/function.hpp>
#include <nall/intrinsics.hpp>

#if defined(PLATFORM_X) || defined(PLATFORM_OSX)
  #include <pthread.h>

namespace nall {
  void* thread_entry_point(void*);

  struct thread {
    thread(function<void ()> entryPoint) : entryPoint(entryPoint), completed(false), dead(false) {
      pthread_create(&pthread, NULL, thread_entry_point, (void*)this);
    }

    ~thread() {
      join();
    }

    bool active() const {
      return completed == false;
    }

    void join() {
      if(dead) return;
      dead = true;
      pthread_join(pthread, NULL);
    }

  private:
    pthread_t pthread;
    function<void ()> entryPoint;
    volatile bool completed, dead;
    friend void* thread_entry_point(void*);
  };

  void* thread_entry_point(void *parameter) {
    thread *context = (thread*)parameter;
    context->entryPoint();
    context->completed = true;
    pthread_exit(0);
  }
}
#elif defined(PLATFORM_WIN)
namespace nall {
  inline DWORD WINAPI thread_entry_point(LPVOID);

  struct thread {
    thread(function<void ()> entryPoint) : entryPoint(entryPoint), completed(false), dead(false) {
      hthread = CreateThread(NULL, 0, thread_entry_point, (void*)this, 0, NULL);
    }

    ~thread() {
      join();
    }

    bool active() const {
      return completed == false;
    }

    void join() {
      if(dead) return;
      dead = true;
      WaitForSingleObject(hthread, INFINITE);
      CloseHandle(hthread);
    }

  private:
    HANDLE hthread;
    function<void ()> entryPoint;
    volatile bool completed, dead;
    friend DWORD WINAPI thread_entry_point(LPVOID);
  };

  inline DWORD WINAPI thread_entry_point(LPVOID parameter) {
    thread *context = (thread*)parameter;
    context->entryPoint();
    context->completed = true;
    return 0;
  }
}
#endif

#endif
