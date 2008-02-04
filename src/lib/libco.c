/*
  libco
  auto-selection module
  license: public domain
*/

#if defined(__GNUC__) && defined(__i386__)
  #include "libco/x86.c"
#elif defined(__GNUC__) && defined(__amd64__) && !defined(__MINGW64__)
  #include "libco/x86-64.c"
#elif defined(__MINGW64__)
  #include "libco/fiber.c"
#elif defined(__GNUC__)
  #include "libco/sjlj.c"
#elif defined(_MSC_VER) && defined(_M_IX86)
  #include "libco/x86.c"
#elif defined(_MSC_VER) && defined(_M_AMD64)
  #include "libco/fiber.c"
#else
  #error "libco: unsupported processor, compiler or operating system"
#endif
