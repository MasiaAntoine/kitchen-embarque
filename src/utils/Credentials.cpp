#include "Credentials.h"

Credentials::Credentials() : username(""), password("") {}

Credentials::Credentials(const String& username, const String& password) {
  this->username = username;
  this->password = password;
}

void Credentials::setUsername(const String& username) {
  this->username = username;
}

void Credentials::setPassword(const String& password) {
  this->password = password;
}

String Credentials::getUsername() const {
  return username;
}

String Credentials::getPassword() const {
  return password;
}

String Credentials::getBasicAuthString() const {
  return username + ":" + password;
}