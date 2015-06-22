#ifndef NALL_SHARED_MEMORY_HPP
#define NALL_SHARED_MEMORY_HPP

#include <nall/memory.hpp>
#include <nall/string.hpp>

#if defined(API_POSIX)
  #include <nall/posix/shared-memory.hpp>
#endif

#if defined(API_WINDOWS)
  #include <nall/windows/shared-memory.hpp>
#endif

#endif
