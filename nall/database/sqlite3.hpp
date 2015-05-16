#ifndef NALL_DATABASE_SQLITE3_HPP
#define NALL_DATABASE_SQLITE3_HPP

/* SQLite3 C++ RAII wrapper for nall
 *
 * Note on code below: it is safe (no-op) to call sqlite3_* functions on null sqlite3 objects
 */

#include <sqlite3.h>

#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall { namespace Database {

struct SQLite3 {
  struct Statement {
    using type = Statement;

    Statement(sqlite3_stmt* stmt) : stmt(stmt) {}
    Statement(const Statement& source) { operator=(source); }
    Statement(Statement&& source) { operator=(move(source)); }

    auto operator=(const Statement& source) -> type& { stmt = source.stmt, stepped = source.stepped, response = source.response; return *this; }
    auto operator=(Statement&& source) -> type& { operator=(source); source.stmt = nullptr, source.stepped = false, source.response = SQLITE_OK; return *this; }
    explicit operator bool() { return !finished(); }

    auto bind(unsigned column, nullptr_t) -> type& { sqlite3_bind_null(stmt, 1 + column); return *this; }
    auto bind(unsigned column, int32_t value) -> type& { sqlite3_bind_int(stmt, 1 + column, value); return *this; }
    auto bind(unsigned column, uint32_t value) -> type& { sqlite3_bind_int(stmt, 1 + column, value); return *this; }
    auto bind(unsigned column, int64_t value) -> type& { sqlite3_bind_int64(stmt, 1 + column, value); return *this; }
    auto bind(unsigned column, uint64_t value) -> type& { sqlite3_bind_int64(stmt, 1 + column, value); return *this; }
    auto bind(unsigned column, double value) -> type& { sqlite3_bind_double(stmt, 1 + column, value); return *this; }
    auto bind(unsigned column, const string& value) -> type& { sqlite3_bind_text(stmt, 1 + column, value.data(), value.size(), SQLITE_TRANSIENT); return *this; }
    auto bind(unsigned column, const vector<uint8_t>& value) -> type& { sqlite3_bind_blob(stmt, 1 + column, value.data(), value.size(), SQLITE_TRANSIENT); return *this; }

    auto integer(unsigned column) -> int64_t { return sqlite3_column_int64(handle(), column); }
    auto decimal(unsigned column) -> uint64_t { return sqlite3_column_int64(handle(), column); }
    auto real(unsigned column) -> double { return sqlite3_column_double(handle(), column); }

    auto text(unsigned column) -> string {
      string result;
      if(auto text = sqlite3_column_text(handle(), column)) {
        result.resize(sqlite3_column_bytes(handle(), column));
        memory::copy(result.pointer(), text, result.size());
      }
      return result;
    }

    auto data(unsigned column) -> vector<uint8_t> {
      vector<uint8_t> result;
      if(auto data = sqlite3_column_blob(handle(), column)) {
        result.resize(sqlite3_column_bytes(handle(), column));
        memory::copy(result.data(), data, result.size());
      }
      return result;
    }

    auto integer() -> int64_t { return integer(column++); }
    auto decimal() -> uint64_t { return decimal(column++); }
    auto real() -> double { return real(column++); }
    auto text() -> string { return text(column++); }
    auto data() -> vector<uint8_t> { return data(column++); }

  protected:
    auto step() -> bool { stepped = true; response = sqlite3_step(stmt); return response == SQLITE_ROW; }
    auto handle() -> sqlite3_stmt* { if(!stepped) step(); return stmt; }
    auto finished() -> bool { handle(); return response != SQLITE_ROW; }
    auto finalize() -> void { handle(); sqlite3_finalize(stmt); stmt = nullptr; }

  private:
    sqlite3_stmt* stmt = nullptr;
    bool stepped = false;
    int response = SQLITE_OK;
    unsigned column = 0;  //this value is not copied/moved via operator=
  };

  struct Query : Statement {
    Query(sqlite3_stmt* stmt) : Statement(stmt) {}
    Query(const Query& source) : Statement(source) {}
    Query(Query&& source) : Statement(move(source)) {}
    ~Query() { finalize(); }

    auto operator=(const Query& source) -> Query& { return Statement::operator=(source), *this; }
    auto operator=(Query&& source) -> Query& { return Statement::operator=(move(source)), *this; }
    explicit operator bool() { return !finished(); }

    using Statement::step;
    using Statement::finalize;

    struct Iterator {
      auto operator*() -> Statement { return Statement(query); }
      auto operator!=(const Iterator& source) const -> bool { return finished != source.finished; }
      auto operator++() -> Iterator& { finished = !query.step(); return *this; }
      Iterator(Query& query, bool finished) : query(query), finished(finished) {}

    protected:
      Query& query;
      bool finished = false;
    };

    auto begin() -> Iterator { return Iterator(*this, finished()); }
    auto end() -> Iterator { return Iterator(*this, true); }
  };

  SQLite3() = default;
  SQLite3(const string& filename) { open(filename); }
  ~SQLite3() { close(); }

  explicit operator bool() const { return database; }

  auto open(const string& filename) -> bool {
    close();
    sqlite3_open(filename, &database);
    return database;
  }

  auto close() -> void {
    sqlite3_close(database);
    database = nullptr;
  }

  template<typename... P> auto execute(const string& statement, P&&... p) -> Query {
    if(!database) return {nullptr};

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(database, statement.data(), statement.size(), &stmt, nullptr);
    if(!stmt) {
      if(debug) print("[sqlite3_prepare_v2] ", sqlite3_errmsg(database), "\n");
      return {nullptr};
    }

    Query query{stmt};
    _bind(query, 0, forward<P>(p)...);
    return query;
  }

  auto lastInsertID() const -> uint64_t {
    return database ? sqlite3_last_insert_rowid(database) : 0;
  }

  auto setDebug(bool debug = true) -> void {
    this->debug = debug;
  }

protected:
  auto _bind(Query&, unsigned) -> void {}
  template<typename T, typename... P> auto _bind(Query& query, unsigned column, const T& value, P&&... p) -> void {
    query.bind(column, value);
    _bind(query, column + 1, forward<P>(p)...);
  }

  bool debug = false;
  sqlite3* database = nullptr;
};

}}

#endif
