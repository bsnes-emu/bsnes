/*
  libvector : version 0.01 ~byuu
*/

#ifndef __LIBVECTOR
#define __LIBVECTOR

template<typename T> class vector {
public:
T *array;
int size, sizelimit;
  int findsize(int newsize) {
  int r = 1;
    while(r >= 1) {
      r <<= 1;
      if(r > sizelimit)return sizelimit;
      if(r >= newsize)return r;
    }
    return size;
  }

  void resize(int newsize) {
  T *newarray;
    newsize = findsize(newsize);

    if(newsize > sizelimit)newsize = sizelimit;
    if(newsize == size)return;

    newarray = (T*)malloc(sizeof(T) * newsize);
    if(newsize >= size) {
      memcpy(newarray, array, sizeof(T) * size);
    } else {
      memcpy(newarray, array, sizeof(T) * newsize);
    }
    free(array);

    array = newarray;
    size = newsize;
  }

  vector(int newsize, int newsizelimit) {
    size = newsize;
    sizelimit = newsizelimit;
    array = (T*)malloc(sizeof(T) * size);
  }

  vector(int newsize) {
    size = newsize;
    sizelimit = 1 << 24;
    array = (T*)malloc(sizeof(T) * size);
  }

  vector() {
    size = 16;
    sizelimit = 1 << 24;
    array = (T*)malloc(sizeof(T) * size);
  }

  ~vector() {
    if(array)free(array);
  }

  inline T &operator[](int index) {
    if(index >= size)resize(index + 1);
    if(index > sizelimit)return array[size - 1];
    return array[index];
  }
};

#endif
