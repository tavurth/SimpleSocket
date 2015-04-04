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

#include "SSocket.hpp"

std::string SSocket::READ_BUFFER = "";
SSocket::Callback SSocket::STREAM_FUNC = 0;

size_t SSocket::WRITE_CALLBACK(void *contents, size_t size, size_t nmemb, void *userp) {
  //Callback function used for writing data from the web directly into a string
  ((std::string*) userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
size_t SSocket::STREAM_CALLBACK(void *contents, size_t size, size_t nmemb, void *userp) {
  //Callback function used for streaming callback

  //Return true from user function to exit stream prematurely
  return ((SSocket::STREAM_FUNC)(contents, size, nmemb)) ? 0 : size * nmemb;
}
SSocket::SSocket() {
//Construcrtor
  //Initialize our error code
  this->res  = (CURLcode) 0;
  
  //Setup our socket if first time
  this->curl = curl_easy_init();
  
  //Maximum wait before timeout
  this->option(CURLOPT_TIMEOUT, 10);
  this->option(CURLOPT_WRITEDATA, &SSocket::READ_BUFFER);
  this->option(CURLOPT_WRITEFUNCTION, SSocket::WRITE_CALLBACK);
}

SSocket::~SSocket() {
//Destructor
  if (this->curl != NULL)
    curl_easy_cleanup(this->curl);
}

CURL * SSocket::handle() {
  return this->curl;
}

////
//
// Error reporting and debugging
//

bool SSocket::check(CURLcode res) {
  return ((this->res = res) == CURLE_OK) ? true : false;
}
const char * SSocket::error() {
  return curl_easy_strerror(this->res);
}

////
//
// Curl internal option setup
//

bool SSocket::option(CURLoption option, long param) {
  return this->check(curl_easy_setopt(this->curl, option, param));
}
bool SSocket::option(CURLoption option, void * param) {
  return this->check(curl_easy_setopt(this->curl, option, param));
}
bool SSocket::option(CURLoption option, const char * param) {
  return this->check(curl_easy_setopt(this->curl, option, param));
}
bool SSocket::option(CURLoption option, std::string * param) {
  return this->check(curl_easy_setopt(this->curl, option, param));
}
bool SSocket::option(CURLoption option, size_t (*param)(void *, size_t, size_t, void *)) {
  return this->check(curl_easy_setopt(this->curl, option, param));
}

void SSocket::debug(bool param) {
  this->option(CURLOPT_VERBOSE, (param ? 1L : 0L));
}

////
//
// Helper function for building HTTP queries
//

std::string SSocket::build_query(const std::map<std::string, std::string> query) {
   //Reuse our static string buffer
  SSocket::READ_BUFFER = "";

  //Loop through the hash and add the keys to the buffer
  for (const auto &it : query)
    SSocket::READ_BUFFER += it.first + "=" + it.second + "&";

  // Remove the trailing "&"
  SSocket::READ_BUFFER.pop_back();
  
  return SSocket::READ_BUFFER;
}
std::string SSocket::http_build_query(const std::map<std::string, std::string> query) {
//Return a query for url get arguments to a server based php script

  //Use the specified options to save the query as a string
  //Ignore return value to reuse SSocket::READ_BUFFER
  this->build_query(query);
    
  //Return the buffer
  return (query.empty() ? "" : "?") + SSocket::READ_BUFFER;
}

std::string SSocket::http_build_query(const std::string url, const std::map<std::string, std::string> query) {
//Return a query for passing url get arguments to a server based php script
  //Call internal query func and return concat
  return url + this->http_build_query(query);
}

////
//
// Generic functionality
//

void SSocket::configure(std::string url) {
//Return or initialize this socket object
  this->option(CURLOPT_URL, url.c_str());
}

std::string SSocket::execute() {
//Call curl_easy_perform and check for errors

  //Reinitialize our read buffer
  SSocket::READ_BUFFER = "";

  //If curl has failed somehow
  if (! this->curl)
    return "Curl initialization failure";

  //Perform and check for errors
  if (! this->check(curl_easy_perform(this->curl)))
    return this->error();

  //Return the buffer
  return SSocket::READ_BUFFER;
}

////
//
// Authentication of Host and Client
//

void SSocket::auth(int value) {
  //Tell our socket to verify both the host and peer
  this->auth_peer(value);
  this->auth_host(value);
}
void SSocket::auth_peer(int value) {
  //Tell our socket to verify the client or not
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, value);
}
void SSocket::auth_host(int value) {
  //Tell our socket to verify the host or not
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, value);
}

////
//
// Request types
//

std::string SSocket::get(const std::string & url,
                         const std::string & queryData) {
//Perform a Simple get request
  this->configure(url + (queryData.length() > 0 ? "?" + queryData : ""));
	
  if (this->curl) {
    //get request setup
    this->option(CURLOPT_HTTPGET, 1);
    this->option(CURLOPT_CUSTOMREQUEST, "GET");
  }
  
  return this->execute();
}		

std::string SSocket::get(const std::string & url,
                         const std::map<std::string, std::string> & queryData) {
//Send a get request, using a std::map (Hashed vector) for the query data
  return this->get(url, this->http_build_query(queryData));
}

std::string SSocket::post(const std::string & url,
                          const std::string & postData) {
//Send a post request using a simple std::string for the data 
  this->configure(url); 
  if (this->curl) {
    //Tell curl we want to post
    this->option(CURLOPT_POST, 1);				
    this->option(CURLOPT_CUSTOMREQUEST, "POST");	  
    //Concatenate body and post data
    this->option(CURLOPT_POSTFIELDSIZE, postData.length());
    this->option(CURLOPT_POSTFIELDS, postData.c_str());
  }
  return this->execute();
}

std::string SSocket::post(const std::string & url,
                          const std::map<std::string, std::string> & queryData) {
//Send a post request using a std::map (Hashed vector) for the input 
  return this->post(url, this->http_build_query(queryData));
}

std::string SSocket::patch(const std::string & url,
                           const std::string & postData) {
//Send a patch request using a simple std::string for the data
  //Send a patch request 	
  this->configure(url);
  
  if (this->curl) {
    //Tell curl we want to post
    this->option(CURLOPT_POST, 1);
    //patch request setup
    this->option(CURLOPT_CUSTOMREQUEST, "PATCH");
    //Concatenate body and post data
    this->option(CURLOPT_POSTFIELDSIZE, postData.length());
    this->option(CURLOPT_POSTFIELDS, postData.c_str());
  }
  return this->execute();
}

std::string SSocket::patch(const std::string & url,
                           const std::map<std::string, std::string> & queryData) {
//Send a post request using a std::map (Hashed vector) for the input 
  return this->patch(url, this->http_build_query(queryData));
}

std::string SSocket::del(const std::string & url) {
//Send a simple Delete request, using only a URL
  //Send a DELETE request 
  this->configure(url);

  if (this->curl)
    //DELETE request setup
    this->option(CURLOPT_CUSTOMREQUEST, "DELETE");
	
  return this->execute();
}
void SSocket::stream(const std::string & url) {

  //Url setup
  this->configure(url);
  //Start the stream, return true from (*callback) to exit
  this->execute();
}
void SSocket::stream(const std::string & url, SSocket::Callback callback) {
  //Open a new curl stream to url, and call (*callback) for each data package

  //Setup our streaming callback, which will in turn call (*callback)
  this->option(CURLOPT_WRITEFUNCTION, SSocket::STREAM_CALLBACK);

  //Save our callback function
  SSocket::STREAM_FUNC = callback;

  //Launch the stream
  this->stream(url);

  //Reset our curl callback to the write buffer
  this->option(CURLOPT_WRITEFUNCTION, SSocket::WRITE_CALLBACK);

}
void SSocket::buffer(const std::string & url, std::string & buffer) {
//Setup our streaming callback, which will in turn call (*callback)
  //Set our curl callback to the write buffer
  this->option(CURLOPT_WRITEDATA, &buffer);
  this->option(CURLOPT_WRITEFUNCTION, SSocket::WRITE_CALLBACK);
  
  //Launch the stream
  this->stream(url);

  //Reset our curl callback to the write buffer
  this->option(CURLOPT_WRITEDATA, &SSocket::READ_BUFFER);
}
