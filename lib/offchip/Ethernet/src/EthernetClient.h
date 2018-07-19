#ifndef ethernetclient_h
#define ethernetclient_h
// #include "Arduino.h"	
// #include "Print.h"
#include "Client.h"
#include "IPAddress.h"

class EthernetClient : public Client {

public:
  EthernetClient();
  EthernetClient(uint8_t sock);

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
  virtual bool operator==(const EthernetClient&);
  virtual bool operator!=(const EthernetClient& rhs) { return !this->operator==(rhs); };

  friend class EthernetServer;
  
//  using Print::write;
  int println(const char* str);
  int println();
  int print(const char* str);
  int getWriteError() { return write_error; }
  
protected:
  void setWriteError(int err = 1) { write_error = err; }

private:
  static uint16_t _srcport;
  uint8_t _sock;
  int write_error;
};

#endif
