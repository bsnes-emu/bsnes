ulong __vector_resize(ulong val)
{
int i;
  for(i=0;i<32;i++) {
    if((1<<i) >= val)break;
  }
//if the value is >2 million, this will fail, in which case,
//just use value. otherwise, use the power of 2.
  if((1<<i) > val)val = 1<<i;
  return val;
}

class vectorlist {
public:
ulong *list, *newlist, size, newsize, max_size;

  void alloc(ulong size) {
    if(size < max_size)return;
    newsize = __vector_resize(size);
    newlist = (ulong*)malloc(newsize*4);
    memcpy(newlist, list, max_size*4);
    free(list);
    list     = newlist;
    max_size = newsize;
  }

  void add(ulong val) {
    size++;
    if(size > max_size)alloc(size + 1);
    list[size - 1] = val;
  }

  void set(ulong num, ulong val) {
    if(num >= max_size)alloc(num + 1);
    list[num] = val;
    if(++num > size)size = num;
  }

  ulong get(ulong num) {
    if(num >= max_size)return 0;
    return list[num];
  }

  vectorlist() {
    list     = 0;
    size     = 0;
    max_size = 8;
    list     = (ulong*)malloc(8*4);
  }

  ~vectorlist() {
    if(list)free(list);
  }

};
