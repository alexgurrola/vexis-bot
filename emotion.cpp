// name: emotion.cpp
// version: 1.0
// description: creates emotions

// libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

// namespaces
using namespace std;

// emotion class header
#include "emotion.h"

// constructors
emotion::emotion () { /* do nothing */ }

// constructors
emotion::emotion (char *raw) { storage = raw; }

// operators
void emotion::operator = (char *raw) { storage = raw; }
