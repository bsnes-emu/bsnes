#ifdef SCPU_CPP

//delta queue scheduler is used to time multiple events with the following properties:
//O(1)     test   (tick)
//O(log n) insert (enqueue)
//O(log n) remove (dequeue)
//
//notes:
//implementation uses binary min-heap array;
//tick() uses 2's-complement signed math to avoid overflow errors;
//enqueue() takes relative timestamps, which are converted to absolute times internally.

class deltaqueue {
  enum { queuesize = 64 };  //maximum number of events that can be queued at the same time.

public:
  alwaysinline void tick(unsigned ticks) {
    counter += ticks;

    while(heapsize) {
      if((signed)(counter - heap[0].counter) < 0) break;  //if(counter < heap[0].counter) break;
      unsigned event = heap[0].event;
      dequeue();
      cpu.queue_event(event);
    }
  }

  void enqueue(unsigned event, unsigned ticks) {
    unsigned child = heapsize++;

    heap[child].event = event;
    heap[child].counter = counter + ticks;

    while(child) {
      unsigned parent = (child - 1) >> 1;
      if(heap[child].counter >= heap[parent].counter) break;
      swap(parent, child);
      child = parent;
    }
  }

  void dequeue() {
    heap[0].counter = heap[--heapsize].counter;
    heap[0].event = heap[heapsize].event;
    heap[heapsize].counter = ~0;

    unsigned parent = 0;
    while(true) {
      unsigned child = (parent << 1) + 1;
      if(heap[child + 1].counter < heap[child].counter) child++;
      if(heap[parent].counter <= heap[child].counter) break;
      swap(parent, child);
      parent = child;
    }
  }

  void reset() {
    counter = 0;
    heapsize = 0;
    for(unsigned i = 0; i < queuesize << 1; i++) heap[i].counter = ~0;
  }

  deltaqueue() {
    reset();
  }

public:
  unsigned counter;
  unsigned heapsize;

  struct {
    unsigned counter;
    unsigned event;
  } heap[queuesize << 1];

  void swap(unsigned x, unsigned y) {
    unsigned counter = heap[x].counter;
    unsigned event = heap[x].event;

    heap[x].counter = heap[y].counter;
    heap[x].event = heap[y].event;

    heap[y].counter = counter;
    heap[y].event = event;
  }
};

#endif  //ifdef SCPU_CPP
