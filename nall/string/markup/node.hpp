#ifdef NALL_STRING_INTERNAL_HPP

//note: specific markups inherit from Markup::Node
//vector<Node> will slice any data; so derived nodes must not contain data nor virtual functions
//vector<Node*> would incur a large performance penalty and greatly increased complexity

namespace nall {
namespace Markup {

struct Node {
  string name;
  string data;
  bool attribute;

  bool exists() const {
    return !name.empty();
  }

  string text() const {
    return string{data}.strip();
  }

  intmax_t integer() const {
    return numeral(text());
  }

  uintmax_t decimal() const {
    return numeral(text());
  }

  void reset() {
    children.reset();
  }

  bool evaluate(const string& query) const {
    if(query.empty()) return true;
    lstring rules = string{query}.replace(" ", "").split(",");

    for(auto& rule : rules) {
      enum class Comparator : unsigned { ID, EQ, NE, LT, LE, GT, GE };
      auto comparator = Comparator::ID;
           if(rule.match("*!=*")) comparator = Comparator::NE;
      else if(rule.match("*<=*")) comparator = Comparator::LE;
      else if(rule.match("*>=*")) comparator = Comparator::GE;
      else if(rule.match ("*=*")) comparator = Comparator::EQ;
      else if(rule.match ("*<*")) comparator = Comparator::LT;
      else if(rule.match ("*>*")) comparator = Comparator::GT;

      if(comparator == Comparator::ID) {
        if(find(rule).size()) continue;
        return false;
      }

      lstring side;
      switch(comparator) {
      case Comparator::EQ: side = rule.split<1> ("="); break;
      case Comparator::NE: side = rule.split<1>("!="); break;
      case Comparator::LT: side = rule.split<1> ("<"); break;
      case Comparator::LE: side = rule.split<1>("<="); break;
      case Comparator::GT: side = rule.split<1> (">"); break;
      case Comparator::GE: side = rule.split<1>(">="); break;
      }

      string data = text();
      if(side(0).empty() == false) {
        auto result = find(side(0));
        if(result.size() == 0) return false;
        data = result(0).data;
      }

      switch(comparator) {
      case Comparator::EQ: if(data.match(side(1)) ==  true)      continue; break;
      case Comparator::NE: if(data.match(side(1)) == false)      continue; break;
      case Comparator::LT: if(numeral(data)  < numeral(side(1))) continue; break;
      case Comparator::LE: if(numeral(data) <= numeral(side(1))) continue; break;
      case Comparator::GT: if(numeral(data)  > numeral(side(1))) continue; break;
      case Comparator::GE: if(numeral(data) >= numeral(side(1))) continue; break;
      }

      return false;
    }

    return true;
  }

  vector<Node> find(const string& query) const {
    vector<Node> result;

    lstring path = query.split("/");
    string name = path.take(0), rule;
    unsigned lo = 0u, hi = ~0u;

    if(name.match("*[*]")) {
      lstring side = name.split<1>("[");
      name = side(0);
      side = side(1).rtrim<1>("]").split<1>("-");
      lo = side(0).empty() ?  0u : numeral(side(0));
      hi = side(1).empty() ? ~0u : numeral(side(1));
    }

    if(name.match("*(*)")) {
      lstring side = name.split<1>("(");
      name = side(0);
      rule = side(1).rtrim<1>(")");
    }

    unsigned position = 0;
    for(auto& node : children) {
      if(node.name.match(name) == false) continue;
      if(node.evaluate(rule) == false) continue;

      bool inrange = position >= lo && position <= hi;
      position++;
      if(inrange == false) continue;

      if(path.size() == 0) result.append(node);
      else {
        auto list = node.find(path.merge("/"));
        for(auto& item : list) result.append(item);
      }
    }

    return result;
  }

  Node operator[](const string& query) const {
    auto result = find(query);
    return result(0);
  }

  vector<Node>::iterator begin() { return children.begin(); }
  vector<Node>::iterator end() { return children.end(); }

  const vector<Node>::constIterator begin() const { return children.begin(); }
  const vector<Node>::constIterator end() const { return children.end(); }

  Node() : attribute(false), level(0) {}

protected:
  unsigned level;
  vector<Node> children;
};

}
}

#endif
