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

##	Compiler and output file
CXX         =	g++
LIB         =   SSocket
TARGET      =	SocketTest
NICE_NAME   =   SimpleSocket
LIB_TYPE    = 	.dll

##	Location of source and build directories
SRC_DIR		=   source
BIN_DIR		=   build
LIB_DIR		=   lib

##	List of objects and corresponding source files
SOURCES     =	$(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
OBJECTS     =	$(foreach dir, $(SOURCES:.cpp=.o), $(BIN_DIR)/$(notdir $(dir)))
EXTERNL     =   libcurl.dll
LIB_FILE    =	lib$(LIB)$(LIB_TYPE)
##	System specific and misc compilation flags
SYSFLAGS	= -lmingw32 
MSCFLAGS	= -std=c++11 -Wall

##	Compaction of all compiler commands
CFLAGS		= $(MSCFLAGS)
LDFLAGS		= $(SYSFLAGS)

##	How to Compile everything
all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	@echo
	@echo ======================
	@echo Linking object files.
	@echo ======================
	@echo
	$(CXX) $(EXTERNL) $(OBJECTS) $(LDFLAGS) -o $@
	@echo
	@echo ======================
	@echo Compilation completed.
	@echo ======================
	@echo

	./$(TARGET)

##	Compile the objects into the library
library: $(OBJECTS)
	@echo
	@echo ======================
	@echo Linking object files.
	@echo ======================
	@echo
	@ar rvs $(LIB_FILE) $(OBJECTS)
	@mkdir -p $(LIB_DIR)
	@mv $(LIB_FILE) $(LIB_DIR)
	@echo
	@echo ======================
	@echo Compilation completed.
	@echo ======================
	@echo
	@echo Copy the compiled $(LIB_DIR)$(LIB)$(LIB_TYPE) into your application directory.
	@echo
	@echo Compile $(LIB) into your application by using
	@echo
	@echo "	g++ example.cpp $(EXTERNL) $(LIB_FILE) -o example"
	@echo
	@echo
	@echo Dont forget to:
	@echo
	@echo "	#include \"$(NICE_NAME)/$(SRC_DIR)/$(LIB).hpp\""
	@echo
	@echo

##	Compile a single object from source
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

##	Cleanup	
clean:
	@echo
	@echo ======================
	@echo Make Commence cleanup.
	@echo ======================
	@echo
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	rm -f $(TARGET)*
	@echo
	@echo ======================
	@echo Make Finished cleanup.
	@echo ======================
	@echo
