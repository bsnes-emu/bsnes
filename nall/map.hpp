#ifndef NALL_MAP_HPP
#define NALL_MAP_HPP

#include <nall/set.hpp>

namespace nall {

template<typename T, typename U> struct map {
  struct node_t {
    T key;
    U value;
    bool operator< (const node_t& source) const { return key <  source.key; }
    bool operator==(const node_t& source) const { return key == source.key; }
    node_t() = default;
    node_t(const T& key) : key(key) {}
    node_t(const T& key, const U& value) : key(key), value(value) {}
  };

  maybe<U&> find(const T& key) const {
    if(auto node = root.find({key})) return node().value;
    return nothing;
  }

  void insert(const T& key, const U& value) { root.insert({key, value}); }
  void remove(const T& key) { root.remove({key}); }
  unsigned size() const { return root.size(); }
  void reset() { root.reset(); }

  typename set<node_t>::iterator begin() { return root.begin(); }
  typename set<node_t>::iterator end() { return root.end(); }
  const typename set<node_t>::iterator begin() const { return root.begin(); }
  const typename set<node_t>::iterator end() const { return root.end(); }

protected:
  set<node_t> root;
};

template<typename T, typename U> struct bimap {
  maybe<U&> find(const T& key) const { return tmap.find(key); }
  maybe<T&> find(const U& key) const { return umap.find(key); }
  void insert(const T& key, const U& value) { tmap.insert(key, value); umap.insert(value, key); }
  void remove(const T& key) { if(auto p = tmap.find(key)) { umap.remove(p().value); tmap.remove(key); } }
  void remove(const U& key) { if(auto p = umap.find(key)) { tmap.remove(p().value); umap.remove(key); } }
  unsigned size() const { return tmap.size(); }
  void reset() { tmap.reset(); umap.reset(); }

  typename set<typename map<T, U>::node_t>::iterator begin() { return tmap.begin(); }
  typename set<typename map<T, U>::node_t>::iterator end() { return tmap.end(); }
  const typename set<typename map<T, U>::node_t>::iterator begin() const { return tmap.begin(); }
  const typename set<typename map<T, U>::node_t>::iterator end() const { return tmap.end(); }

protected:
  map<T, U> tmap;
  map<U, T> umap;
};

}

#endif
