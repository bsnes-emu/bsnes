#ifndef NALL_SMTP_HPP
#define NALL_SMTP_HPP

#include <nall/base64.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

#if !defined(_WIN32)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
#else
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <windows.h>
#endif

namespace nall {

struct SMTP {
  enum class Format : unsigned { Plain, HTML };

  inline void server(string server, uint16_t port = 25);
  inline void from(string mail, string name = "");
  inline void to(string mail, string name = "");
  inline void cc(string mail, string name = "");
  inline void bcc(string mail, string name = "");
  inline void attachment(const uint8_t* data, unsigned size, string name);
  inline bool attachment(string filename, string name = "");
  inline void subject(string subject);
  inline void body(string body, Format format = Format::Plain);

  inline bool send();
  inline string message();
  inline string response();

  #ifdef _WIN32
  inline int close(int);
  inline SMTP();
  #endif

private:
  struct Information {
    string server;
    uint16_t port;
    struct Contact {
      string mail;
      string name;
    };
    Contact from;
    vector<Contact> to;
    vector<Contact> cc;
    vector<Contact> bcc;
    struct Attachment {
      vector<uint8_t> buffer;
      string name;
    };
    string subject;
    string body;
    Format format = Format::Plain;
    vector<Attachment> attachments;

    string message;
    string response;
  } info;

  inline bool send(int sock, const string& text);
  inline string recv(int sock);
  inline string boundary();
  inline string filename(const string& filename);
  inline string contact(const Information::Contact& contact);
  inline string contacts(const vector<Information::Contact>& contacts);
  inline string split(const string& text);
};

void SMTP::server(string server, uint16_t port) {
  info.server = server;
  info.port = port;
}

void SMTP::from(string mail, string name) {
  info.from = {mail, name};
}

void SMTP::to(string mail, string name) {
  info.to.append({mail, name});
}

void SMTP::cc(string mail, string name) {
  info.cc.append({mail, name});
}

void SMTP::bcc(string mail, string name) {
  info.bcc.append({mail, name});
}

void SMTP::attachment(const uint8_t* data, unsigned size, string name) {
  vector<uint8_t> buffer;
  buffer.resize(size);
  memcpy(buffer.data(), data, size);
  info.attachments.append({std::move(buffer), name});
}

bool SMTP::attachment(string filename, string name) {
  if(!file::exists(filename)) return false;
  if(name == "") name = notdir(filename);
  auto buffer = file::read(filename);
  info.attachments.append({std::move(buffer), name});
  return true;
}

void SMTP::subject(string subject) {
  info.subject = subject;
}

void SMTP::body(string body, Format format) {
  info.body = body;
  info.format = format;
}

bool SMTP::send() {
  info.message.append("From: =?UTF-8?B?", Base64::encode(contact(info.from)), "?=\r\n");
  info.message.append("To: =?UTF-8?B?", Base64::encode(contacts(info.to)), "?=\r\n");
  info.message.append("Cc: =?UTF-8?B?", Base64::encode(contacts(info.cc)), "?=\r\n");
  info.message.append("Subject: =?UTF-8?B?", Base64::encode(info.subject), "?=\r\n");

  string uniqueID = boundary();

  info.message.append("MIME-Version: 1.0\r\n");
  info.message.append("Content-Type: multipart/mixed; boundary=", uniqueID, "\r\n");
  info.message.append("\r\n");

  string format = (info.format == Format::Plain ? "text/plain" : "text/html");

  info.message.append("--", uniqueID, "\r\n");
  info.message.append("Content-Type: ", format, "; charset=UTF-8\r\n");
  info.message.append("Content-Transfer-Encoding: base64\r\n");
  info.message.append("\r\n");
  info.message.append(split(Base64::encode(info.body)), "\r\n");
  info.message.append("\r\n");

  for(auto& attachment : info.attachments) {
    info.message.append("--", uniqueID, "\r\n");
    info.message.append("Content-Type: application/octet-stream\r\n");
    info.message.append("Content-Transfer-Encoding: base64\r\n");
    info.message.append("Content-Disposition: attachment; size=", attachment.buffer.size(), "; filename*=UTF-8''", filename(attachment.name), "\r\n");
    info.message.append("\r\n");
    info.message.append(split(Base64::encode(attachment.buffer)), "\r\n");
    info.message.append("\r\n");
  }

  info.message.append("--", uniqueID, "--\r\n");

  addrinfo hints;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  addrinfo* serverinfo;
  int status = getaddrinfo(info.server, string(info.port), &hints, &serverinfo);
  if(status != 0) return false;

  int sock = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
  if(sock == -1) return false;

  int result = connect(sock, serverinfo->ai_addr, serverinfo->ai_addrlen);
  if(result == -1) return false;

  string response;
  info.response.append(response = recv(sock));
  if(!response.beginswith("220 ")) { close(sock); return false; }

  send(sock, {"HELO ", info.server, "\r\n"});
  info.response.append(response = recv(sock));
  if(!response.beginswith("250 ")) { close(sock); return false; }

  send(sock, {"MAIL FROM: <", info.from.mail, ">\r\n"});
  info.response.append(response = recv(sock));
  if(!response.beginswith("250 ")) { close(sock); return false; }

  for(auto& contact : info.to) {
    send(sock, {"RCPT TO: <", contact.mail, ">\r\n"});
    info.response.append(response = recv(sock));
    if(!response.beginswith("250 ")) { close(sock); return false; }
  }

  for(auto& contact : info.cc) {
    send(sock, {"RCPT TO: <", contact.mail, ">\r\n"});
    info.response.append(response = recv(sock));
    if(!response.beginswith("250 ")) { close(sock); return false; }
  }

  for(auto& contact : info.bcc) {
    send(sock, {"RCPT TO: <", contact.mail, ">\r\n"});
    info.response.append(response = recv(sock));
    if(!response.beginswith("250 ")) { close(sock); return false; }
  }

  send(sock, {"DATA\r\n"});
  info.response.append(response = recv(sock));
  if(!response.beginswith("354 ")) { close(sock); return false; }

  send(sock, {info.message, "\r\n", ".\r\n"});
  info.response.append(response = recv(sock));
  if(!response.beginswith("250 ")) { close(sock); return false; }

  send(sock, {"QUIT\r\n"});
  info.response.append(response = recv(sock));
//if(!response.beginswith("221 ")) { close(sock); return false; }

  close(sock);
  return true;
}

string SMTP::message() {
  return info.message;
}

string SMTP::response() {
  return info.response;
}

bool SMTP::send(int sock, const string& text) {
  const char* data = text.data();
  unsigned size = text.size();
  while(size) {
    int length = ::send(sock, (const char*)data, size, 0);
    if(length == -1) return false;
    data += length;
    size -= length;
  }
  return true;
}

string SMTP::recv(int sock) {
  vector<uint8_t> buffer;
  while(true) {
    char c;
    if(::recv(sock, &c, sizeof(char), 0) < 1) break;
    buffer.append(c);
    if(c == '\n') break;
  }
  buffer.append(0);
  return buffer;
}

string SMTP::boundary() {
  random_lfsr random;
  random.seed(time(0));
  string boundary;
  for(unsigned n = 0; n < 16; n++) boundary.append(hex<2>(random()));
  return boundary;
}

string SMTP::filename(const string& filename) {
  string result;
  for(auto& n : filename) {
    if(n <= 32 || n >= 127) result.append("%", hex<2>(n));
    else result.append(n);
  }
  return result;
}

string SMTP::contact(const Information::Contact& contact) {
  if(!contact.name) return contact.mail;
  return {"\"", contact.name, "\" <", contact.mail, ">"};
}

string SMTP::contacts(const vector<Information::Contact>& contacts) {
  string result;
  for(auto& contact : contacts) {
    result.append(this->contact(contact), "; ");
  }
  result.rtrim<1>("; ");
  return result;
}

string SMTP::split(const string& text) {
  string result;

  unsigned offset = 0;
  while(offset < text.size()) {
    unsigned length = min(76, text.size() - offset);
    if(length < 76) {
      result.append(text.slice(offset));
    } else {
      result.append(text.slice(offset, 76), "\r\n");
    }
    offset += length;
  }

  return result;
}

#ifdef _WIN32
int SMTP::close(int sock) {
  return closesocket(sock);
}

SMTP::SMTP() {
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED) {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      WSACleanup();
      return;
    }
  } else {
    close(sock);
  }
}
#endif

}

#endif
