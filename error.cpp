// name: error.cpp
// version: 1.0
// description: handles error messages

// standard libraries
#include <string>
#include <stdio.h>

// namespaces
using namespace std;

// header file
#include "error.h"

// error handler
void error(int abort, int code, const char *message) {  
    
    /*int tui = 0;
    if (tui == 1) { tui::display(msg); }
    else { perror(temp.c_str()); }*/
    
    // declare variables
    string temp;
    
    // build error message
    temp = "error %d: ";
    temp += message;
    temp += '\n';
    
    // declare variables
    char buffer[temp.size()];
    
    // insert error code
    sprintf (buffer, temp.c_str(), code);
    
    // set data
    temp = buffer;
    
    // display message
    perror(temp.c_str());
    
    // handle abort
    //if (abort == 1) { exit(1); }
    
}
