
template<class T> struct Event_of {
  uint32 time;
  uint32 node;
  T      action;
  
  Event_of() : time(0), node(0) {}
  Event_of(T action) : time(0), node(0), action(action) {}
  inline bool operator<(const Event_of& rhs) const { return time - rhs.time >= 0x80000000u; }
  inline bool operator<(uint32 rhs) const          { return time - rhs      >= 0x80000000u; }
};

template<class T> struct EventQueue_of {
  typedef Event_of<T> Elem;
  
  uint32 time;
  
  EventQueue_of(uint32 capacity) : size(0), time(0) {
    //items.reserve(capacity+1);  // to simplify math, items[0] is unused
    for(unsigned n = 0; n < capacity+1; n++)
      items[n] = nullptr;
  }
  
  void reset() {
    clear();
    time = 0;
  }
  void clear() {
    for(unsigned n = 1; n <= size; n++) {
      items[n]->node = 0;
      items[n] = nullptr;
    }
    size = 0;
  }
  bool valid(uint32 n = 1) {
    return (2*n+0 > size || *(items[n]) < items[2*n+0]->time+1 && valid(2*n+0))
        && (2*n+1 > size || *(items[n]) < items[2*n+1]->time+1 && valid(2*n+1));
  }
  void add(uint32 dt, Elem& e) {
    if(e.node) {
      remove(e);
      return add(dt, e);
    }
    uint32 old = e.time;
    e.time = time + dt;
    e.node = ++size;
    bubble(e);
    if(valid() == false) asm volatile("\n\t int $3");
  }
  void remove(Elem& e) {
    if(e.node == 0) return;
    
    items[e.node] = items[size];
    items[e.node]->node = e.node;
    items[size--] = nullptr;
    
    if(size > 1 && e.node <= size) {
      Elem& m = *(items[e.node]);
      
      if(m < e) {
        // This can happen if removing something besides the min element.
        bubble(m);
        if(valid() == false) asm volatile("\n\t int $3");
      }
      else {
        while(2*m.node <= size) {
          uint32 l = 2*m.node;
          uint32 r = 2*m.node + (l < size);
          uint32 c = *(items[r]) < *(items[l])? r : l;
          
          if(!(*(items[c]) < m)) break;
          
          items[m.node] = items[c];
          items[m.node]->node = m.node;
          m.node = c;
        }
        items[m.node] = &m;
        if(valid() == false) asm volatile("\n\t int $3");
      }
    }
    e.node = 0;
  }
  
  inline void step(uint32 ticks) {
    ticks += time;  // relative -> absolute time
    time = ticks;   // default case for early out
    
    // Caution! We're running under the assumption our queue is never empty.
    // Fortunately, due to audio/video events, that's true in dasShiny.
    // Gives around 16% speedup in some cases.
    if(!(*(items[1]) < ticks)) return;  // early out, nothing to do
    
    auto &next = items[1];
    do {
      Elem& e = *next;
      time = e.time;    // update the time so add()
      remove(e);        // works during the callback.
      e.action();
    } while(/*size && */*next < ticks);
    
    if(time - ticks >= 0x80000000u)
      time = ticks;
  }
  
//private:
  //linear_vector<Elem*> items;
  Elem*  items[64];
  uint32 size;
  
  void bubble(Elem& e) {
    while(1 < e.node && e < *(items[e.node/2])) {
      items[e.node]       = items[e.node/2];
      items[e.node]->node = e.node;
      e.node /= 2;
    }
    items[e.node] = &e;
  }
  
  EventQueue_of();
};
