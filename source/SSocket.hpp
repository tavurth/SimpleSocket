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
#include <curl/curl.h>

//SSocket offers a user friendly wrapper for Curl

class SSocket {
protected:

  CURL *	  curl;
  CURLcode 	  res;

  std::string execute();
  void configure(const std::string);
  std::string http_build_query(const std::map<std::string, std::string>);
  std::string http_build_query(const std::string, const std::map<std::string, std::string>);

  static std::string READ_BUFFER;
  static size_t WRITE_CALLBACK(void *, size_t, size_t, void *);
  
public:
  SSocket();
  ~SSocket();
    
  std::string del(const std::string);
  std::string get(const std::string, const std::string = "");
  std::string post(const std::string, const std::string = "", std::string = "");
  std::string patch(const std::string, const std::string = "", std::string = "");

  std::string get(const std::string, const std::map<std::string, std::string>);
  std::string post(const std::string, const std::map<std::string, std::string>, std::string = "");
  std::string patch(const std::string, const std::map<std::string, std::string>, std::string = "");
};

#endif
