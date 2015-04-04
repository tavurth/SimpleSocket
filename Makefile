##	Copyright 2015 William Whitty
##	Tavurth@gmail.com
##	
##	Licensed under the Apache License, Version 2.0 (the 'License');
##	you may not use this file except in compliance with the License.
##	You may obtain a copy of the License at
##	
##	    http://www.apache.org/licenses/LICENSE-2.0
##	
##	Unless required by applicable law or agreed to in writing, software
##	distributed under the License is distributed on an 'AS IS' BASIS,
##	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##	See the License for the specific language governing permissions and
##	limitations under the License.

CXX         =	g++
DIRECTORY   =	SimpleSocket
LIBRARY		=   ssocket
HEADER		=   SSocket.hpp
SHARED		=	libcurl.dll
LIB_TYPE	=   .dll

##	System specific and Miscellaneous compilation flags
SYSFLAGS	=  -lmingw32 -std=c++11 -Wall
MISCFLAGS	=  -lcurl

##	Location of source and build directories
SRC_DIR		=   include/
BIN_DIR		=   build/
LIB_DIR		=   lib/

##	list of objects and corresponding source files
SOURCES     =	$(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
OBJECTS     =	$(foreach dir, $(SOURCES:.cpp=.o), $(BIN_DIR)$(notdir $(dir)))

##	Compaction of all compiler commands
CFLAGS		= $(SYSFLAGS) 
LDFLAGS		= $(MISCFLAGS)

##	Compile everything
all: $(SOURCES) $(LIBRARY)

##	Compile the objects into the library
$(LIBRARY): $(OBJECTS)
	@echo
	@echo ======================
	@echo Linking object files.
	@echo ======================
	@echo
	@ar rvs lib$(LIBRARY)$(LIB_TYPE) $(OBJECTS)
	@mkdir -p $(LIB_DIR)
	@mv lib$(LIBRARY)$(LIB_TYPE) $(LIB_DIR)
	@echo
	@echo ======================
	@echo Compilation completed.
	@echo ======================
	@echo
	@echo Compile $(LIBRARY) into your application by using
	@echo
	@echo "	g++ $(SHARED) example.cpp -o example.exe -L$(DIRECTORY)/lib/ -l$(LIBRARY)"
	@echo
	@echo where '$(DIRECTORY)/lib/' is the address of the lib directory
	@echo
	@echo Dont forget to:
	@echo
	@echo "	#include \"$(DIRECTORY)/$(SRC_DIR)$(HEADER)\""
	@echo

##	Compile a single object from source
$(BIN_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

##	Cleanup	
clean:
	@echo
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	@echo
	@echo ======================
	@echo Make Finished cleanup.
	@echo ======================
	@echo
