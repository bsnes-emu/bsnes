/*
  libvector : version 0.04 ~byuu (12/24/05)
*/

#ifndef __LIBVECTOR
#define __LIBVECTOR

template<typename T> class vector {
private:
T *array;
int size, sizelimit;

//find next array size that is a power of two
  int findsize(int newsize) {
  int r = 1;
    while(r >= 1) {
      r <<= 1;
      if(r > sizelimit)return sizelimit;
      if(r >= newsize)return r;
    }
    return size;
  }

public:
  void resize(int newsize) {
    newsize = findsize(newsize);

    if(newsize > sizelimit)newsize = sizelimit;
    if(newsize == size)return;

    array = (T*)realloc(array, sizeof(T) * newsize);

    if(newsize > size) {
      for(int i = size; i < newsize; i += sizeof(T)) {
        array[i] = (T)0;
      }
    }

    size = newsize;
  }

//used to free up memory used by vector, but without
//actually destroying the vector itself
  void release() {
    resize(16);
  }

  T *handle() { return (T*)array; }

  void copy(T *source, uint32 copy_size) {
    if(copy_size > size) {
      resize(copy_size);
      copy_size = size;
    }
    memcpy(array, source, copy_size * sizeof(T));
  }

  void clear() {
    memset(array, 0, size * sizeof(T));
  }

  vector(int newsize, int newsizelimit) {
    size = newsize;
    sizelimit = newsizelimit;
    array = (T*)calloc(size, sizeof(T));
  }

  vector(int newsize) {
    size = newsize;
    sizelimit = 1 << 24;
    array = (T*)calloc(size, sizeof(T));
  }

  vector() {
    size = 16;
    sizelimit = 1 << 24;
    array = (T*)calloc(size, sizeof(T));
  }

  ~vector() {
    if(array) {
      free(array);
      array = 0;
    }
  }

//operator T() { return array[0]; }

  inline T &operator[](int index) {
    if(index >= size)resize(index + 1);
    if(index > sizelimit)return array[size - 1];
    return array[index];
  }
};

#endif
