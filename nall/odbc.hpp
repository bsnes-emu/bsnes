#ifndef NALL_ODBC_HPP
#define NALL_ODBC_HPP

//minimal wrapper for core ODBC v3 API
//requires Windows or unixODBC

#include <nall/string.hpp>
#include <nall/utility.hpp>

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

namespace nall {

struct ODBC {
  inline ODBC();
  inline ODBC(const string& database, const string& username, const string& password);
  inline ~ODBC();

  inline bool connected();
  inline bool connect(const string& hostname, const string& username, const string& password);
  inline void disconnect();
  template<typename... Args> inline bool execute(Args&&... args);
  inline void release();
  inline unsigned rows();
  inline lstring read();

private:
  char* buffer = nullptr;
  SQLHANDLE sqlEnvironment = nullptr;
  SQLHANDLE sqlConnection = nullptr;
  SQLHANDLE sqlStatement = nullptr;
};

ODBC::ODBC() {
  auto result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvironment);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) return;

  SQLSetEnvAttr(sqlEnvironment, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
  buffer = new char[65536]();
}

ODBC::ODBC(const string& database, const string& username, const string& password) : ODBC() {
  connect(database, username, password);
}

ODBC::~ODBC() {
  if(sqlEnvironment) {
    disconnect();
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvironment);
    sqlEnvironment = nullptr;

    delete[] buffer;
    buffer = nullptr;
  }
}

bool ODBC::connected() {
  return sqlConnection;
}

bool ODBC::connect(const string& hostname, const string& username, const string& password) {
  if(!sqlEnvironment) return false;
  disconnect();

  auto result = SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvironment, &sqlConnection);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) return false;

  SQLSetConnectAttr(sqlConnection, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
  result = SQLConnectA(sqlConnection,
    (SQLCHAR*)(const char*)hostname, SQL_NTS,
    (SQLCHAR*)(const char*)username, SQL_NTS,
    (SQLCHAR*)(const char*)password, SQL_NTS
  );
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
    disconnect();
    return false;
  }

  return true;
}

void ODBC::disconnect() {
  if(sqlConnection) {
    release();
    SQLDisconnect(sqlConnection);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnection);
    sqlConnection = nullptr;
  }
}

template<typename... Args>
bool ODBC::execute(Args&&... args) {
  string statement({args...});

  if(!sqlConnection) return false;
  release();

  auto result = SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &sqlStatement);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) return false;

  result = SQLExecDirectA(sqlStatement, (SQLCHAR*)(const char*)statement, SQL_NTS);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
    release();
    return false;
  }

  return true;
}

void ODBC::release() {
  if(sqlStatement) {
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
    sqlStatement = nullptr;
  }
}

//valid after update, insert or delete
unsigned ODBC::rows() {
  if(!sqlStatement) return 0;

  SQLLEN sqlRows = 0;
  auto result = SQLRowCount(sqlStatement, &sqlRows);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) return 0;

  return sqlRows;
}

//valid after select
lstring ODBC::read() {
  if(!sqlStatement) return {};

  auto result = SQLFetch(sqlStatement);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) return {};

  SQLSMALLINT sqlColumns = 0;
  result = SQLNumResultCols(sqlStatement, &sqlColumns);
  if(result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) return {};

  lstring data;
  for(unsigned column = 0; column < sqlColumns; column++) {
    SQLLEN length = 0;
    SQLGetData(sqlStatement, 1 + column, SQL_C_CHAR, buffer, 65535, &length);
    data.append(buffer);
  }

  return data;
}

}

#endif
