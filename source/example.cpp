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

int main(int argc, char ** argv) {
  SSocket sock = SSocket();

  //Perform a basic POST request, using a const char * or std::string as query data
  printf("%s\n\n", sock.post("http://posttestserver.com/post.php", "note=\"Hello, world!\"").c_str());

  //Create a std::map for query data
  std::map<std::string, std::string> query = {{"VarA", "ValueA"}, {"VarB", "ValueB"}};

  //Perform and add some data to the POST body
  printf("%s\n\n", sock.post("http://posttestserver.com/post.php", query, "THIS IS A TEST OF EXTRA BODY OF DATA").c_str());
}
