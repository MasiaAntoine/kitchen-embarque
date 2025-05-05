#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

class Credentials {
private:
  String username;
  String password;

public:
  Credentials();
  Credentials(const String& username, const String& password);
  
  void setUsername(const String& username);
  void setPassword(const String& password);
  
  String getUsername() const;
  String getPassword() const;
  
  String getBasicAuthString() const;
};

#endif