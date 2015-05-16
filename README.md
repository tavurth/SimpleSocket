# SimpleSocket
Basic C++ curl wrapper, emphasizing simplicity for basic GET POST PATCH DELETE operations.

# Compiling

    This library was created using mingw32 on windows, and was converted to CMake on Mac OSX.
    Cross compiling should not be a problem.

    # Requirements
      * Curl
      * C++ standard 11 (Or 0x)
    
    When running an application, copy the from your curl directory
        * curl.dylib (MAC)
        * curl.dll (WIN)
    And put into your application directory


    # Build

    cd build
    cmake ..
    make 

    When using the library, compile into your application as follows: (G++)

    g++ example.cpp -o test -L../SimpleSocket/lib/ -lssocket