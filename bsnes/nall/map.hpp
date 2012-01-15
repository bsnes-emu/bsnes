#ifndef NALL_MAP_HPP
#define NALL_MAP_HPP

#include <nall/vector.hpp>

namespace nall {

template<typename LHS, typename RHS>
struct map {
  struct pair {
    LHS name;
    RHS data;
  };

  void reset() {
    list.reset();
  }

  unsigned size() const {
    return list.size();
  }

  //O(n)
  void append(const LHS &name, const RHS &data) {
    signed offset = size();
    for(unsigned n = 0; n < size(); n++) {
      if(name < list[n].name) { offset = n; break; }
    }
    list.insert(offset, { name, data });
  }

  //O(log n)
  RHS& operator[](const LHS &name) {
    signed first = 0, last = size() - 1;
    while(first <= last) {
      signed middle = (first + last) / 2;
           if(name < list[middle].name) last  = middle - 1;  //search lower half
      else if(name > list[middle].name) first = middle + 1;  //search upper half
      else return list[middle].data;                         //match found
    }
    throw;
  }

  //O(log n) nothrow
  const RHS& operator()(const LHS &name, const RHS &data) {
    signed first = 0, last = size() - 1;
    while(first <= last) {
      signed middle = (first + last) / 2;
           if(name < list[middle].name) last  = middle - 1;  //search lower half
      else if(name > list[middle].name) first = middle + 1;  //search upper half
      else return list[middle].data;                         //match found
    }
    return data;
  }

  pair* begin() { return list.begin(); }
  pair* end() { return list.end(); }
  const pair* begin() const { return list.begin(); }
  const pair* end() const { return list.end(); }

protected:
  vector<pair> list;
};

template<typename LHS, typename RHS>
struct bidirectional_map {
  map<LHS, RHS> lhs;
  map<RHS, LHS> rhs;

  void reset() {
    lhs.reset();
    rhs.reset();
  }

  unsigned size() const {
    return lhs.size();
  }

  void append(const LHS &ldata, const RHS &rdata) {
    lhs.append(ldata, rdata);
    rhs.append(rdata, ldata);
  }
};

}

#endif
