/*
  libco
  auto-selection module
  license: public domain
*/

#if defined(__GNUC__) && defined(__i386__)
  #include "x86.c"
#elif defined(__GNUC__) && defined(__amd64__) && !defined(__MINGW64__)
  #include "x86-64.c"
#elif defined(__MINGW64__)
  #include "fiber.c"
#elif defined(__GNUC__)
  #include "sjlj.c"
#elif defined(_MSC_VER) && defined(_M_IX86)
  #include "x86.c"
#elif defined(_MSC_VER) && defined(_M_AMD64)
  #include "fiber.c"
#else
  #error "libco: unsupported processor, compiler or operating system"
#endif
