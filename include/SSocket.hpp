/*

  Copyright 2015 William Whitty

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#ifndef __TAVURTH__SIMPLE__SOCKET__
#define __TAVURTH__SIMPLE__SOCKET__

#include <map>
#include <string>
#include <fstream>
#include <curl/curl.h>

//SSocket offers a user friendly wrapper for Curl

class SSocket {
  
public:
  // Type of streaming callback function 
  typedef bool (*Callback)(const void *, const unsigned, const unsigned);

  SSocket();
  ~SSocket();

  CURL * handle();
  bool check(CURLcode);
  const char* error();
  
  bool option(CURLoption, long);
  bool option(CURLoption, void *);
  bool option(CURLoption, const char *);
  bool option(CURLoption, std::string *);
  bool option(CURLoption, size_t (*)(void *, size_t, size_t, void *));

  void debug(bool);
  void timeout(long);
  
  void auth(int);
  void auth_peer(int);
  void auth_host(int);

  std::string del(const std::string &);
  std::string get(const std::string &, const std::string & = "");
  std::string post(const std::string &, const std::string & = "");
  std::string patch(const std::string &, const std::string & = "");

  std::string get(const std::string &, const std::map<std::string, std::string> &);
  std::string post(const std::string &, const std::map<std::string, std::string> &);
  std::string patch(const std::string &, const std::map<std::string, std::string> &);

  void stream(const std::string &, Callback);
  void stream(const std::string &, std::string &);
  void stream(const std::string &, const char *, bool = false);
  
protected:
  CURL *	  curl;
  CURLcode 	  res;

  std::string execute();
  void configure(const std::string);
  void stream(const std::string &);
  // Map to string
  std::string build_query(const std::map<std::string, std::string>);

  // Map to HTTP query string
  std::string http_build_query(const std::map<std::string, std::string>);
  std::string http_build_query(const std::string, const std::map<std::string, std::string>);

  static std::string READ_BUFFER;
  static Callback    STREAM_FUNC;
  static size_t FPUT_CALLBACK(void *, size_t, size_t, void *);
  static size_t WRITE_CALLBACK(void *, size_t, size_t, void *);
  static size_t STREAM_CALLBACK(void *, size_t, size_t, void *);
};

#endif
