// name: http.cpp
// version: 1.2
// description: handles the http protocol

// libraries
#include <iostream>
#include <string.h>
#include "netsock.h"

// namespaces
using namespace std;

// http handler header
#include "http.h"

// constructors
http::http () { /* do nothing */ }

// grabs data from a web page
const char *http::get (const char *hostname, const char *location) {
	
	// declare variables
	netsock socket;
	string hget, host;
	
	// clear strings
	hget.empty();
	host.empty();
	
	// build get header
	hget = "GET ";
	hget += location;
	hget += " HTTP/1.0\r\n";
	
	// build host header
	host = "Host: ";
	host += hostname;
	host += "\r\n";
	
	// clear socket
	socket.disconnect();
	
	// make connection
	socket.connect(hostname, 80);
	
	// send get header
	socket.send(hget.c_str());
	
	// send host header
	socket.send(host.c_str());
	
	// send user agent header
	socket.send("User-Agent: Versidyne/1.0\r\n");
	
	// send end of line
	socket.send("\r\n");
	
	// return data
	return clean(socket.receive());
	
}

// grabs usable data from raw data
const char *http::clean (const char *data) {
	
	// declare variables
	int i, e, o, x;
	
	// set integers
	e = strlen(data);
	x = 0;
	
	// create local variable
	char temp[e];
	
	// scroll through data
	for (i = 0; i < e; i++) {
		// find last character of header
		if (data[i] == '\n' && data[i-1] == '\r' && data[i-2] == '\n' && data[i-3] == '\r') {
			// scroll through data starting with marker
			for (o = i+1; o < e; o++) {
				// set beginning of array at beginning of data
				temp[x] = data[o];
				// increment pointer integer
				x++;
			}
			// find final character and set to null byte
			if (temp[x-1] == '\n') {
				if (temp[x-2] == '\r') { temp[x-2] = '\0'; }
				else { temp[x-1] = '\0'; }
			}
			else { temp[x] = '\0'; }
			// exit scroll
			break;
		}
	}
	
	// return condition
	return temp;
	
}

// grabs internal header data
const char *http::head (const char *data) {
	
	// declare variables
	int i, e, o, x;
	
	// set integers
	e = strlen(data);
	x = 0;
	
	// create local variable
	char temp[e];
	
	// scroll through data
	for (i = 0; i < e; i++) {
		// find first instance of head tag
		if (data[i] == '>' && data[i-1] == 'd' && data[i-2] == 'a' && data[i-3] == 'e' && data[i-4] == 'h' && data[i-5] == '<') {
			// scroll through data starting with marker
			for (o = i+1; o < e; o++) {
				// detect closing head tag, set pointer, and stop scroll
				if (data[o] == '>' && data[o-1] == 'd' && data[o-2] == 'a' && data[o-3] == 'e' && data[o-4] == 'h' && data[o-5] == '/' && data[o-6] == '<') { x -= 6; break; }
				// set current array cell and increment
				else { temp[x++] = data[o];}
			}
			// find final character and set to null byte
			if (temp[x-1] == '\n') {
				if (temp[x-2] == '\r') { temp[x-2] = '\0'; }
				else { temp[x-1] = '\0'; }
			}
			else { temp[x] = '\0'; }
			// exit loop
			break;
		}
	}
	
	// return condition
	return temp;
	
}

// grabs internal body data
const char *http::body (const char *data) {
	
	// declare variables
	int i, e, o, x;
	
	// set integers
	e = strlen(data);
	x = 0;
	
	// create local variable
	char temp[e];
	
	// scroll through data
	for (i = 0; i < e; i++) {
		// find first instance of head tag
		if (data[i] == '>' && data[i-1] == 'y' && data[i-2] == 'd' && data[i-3] == 'o' && data[i-4] == 'b' && data[i-5] == '<') {
			// scroll through data starting with marker
			for (o = i+1; o < e; o++) {
				// detect closing head tag, set pointer, and stop scroll
				if (data[o] == '>' && data[o-1] == 'y' && data[o-2] == 'd' && data[o-3] == 'o' && data[o-4] == 'b' && data[o-5] == '/' && data[o-6] == '<') { x -= 6; break; }
				// set current array cell and increment
				else { temp[x++] = data[o];}
			}
			// find final character and set to null byte
			if (temp[x-1] == '\n') {
				if (temp[x-2] == '\r') { temp[x-2] = '\0'; }
				else { temp[x-1] = '\0'; }
			}
			else { temp[x] = '\0'; }
			// exit loop
			break;
		}
	}
	
	// return condition
	return temp;
	
}

// grabs all link data
const char *http::links (const char *data) {
	
	// declare variables
	int e, i, l, o, t;
	
	// set length
	e = strlen(data);
	t = 0;
	
	// create local variables
	char temp[e];
	char link[e];
	
	// scroll through data
	for (i = 0; i < e; i++) {
		// find first instance of head tag
		if (data[i] == '<' && data[i+1] == 'a' && data[i+2] == ' ' && data[i+3] == 'h' && data[i+4] == 'r' && data[i+5] == 'e' && data[i+6] == 'f' && data[i+7] == '=') {
		//if (data[i] == '=' && data[i-1] == 'f' && data[i-2] == 'e' && data[i-3] == 'r' && data[i-4] == 'h' && data[i-5] == ' ' && data[i-6] == 'a' && data[i-7] == '<') {
			// set beginning
			l = 0;
			// scroll through data starting with marker
			for (o = i+8; o < e; o++) {
				// detect closing link tag, set pointer, and stop scroll
				if (data[o] == '>') { break; }
				// set current array cell and increment
				else { link[l++] = data[o];}
			}
			// find final character and set to null byte
			if (link[l-1] == '\n') {
				if (link[l-2] == '\r') { link[l-2] = '\0'; }
				else { link[l-1] = '\0'; }
			}
			else { link[l] = '\0'; }
			// scroll through extracted data
			for (o = 0; o < e-8; o++) {
				if (link[o] == 'h' && link[o+1] == 't' && link[o+2] == 't' && link[o+3] == 'p') {
					if (link[o+4] == ':' && link[o+5] == '/' && link[o+6] == '/') {
						for (l = o; l < e; l++) { if (link[l] == '\0') { break; } else { link[l] = link[l+7]; } }
					}
					else if (link[o+4] == 's' && link[o+5] == ':' && link[o+6] == '/' && link[o+7] == '/') {
						for (l = o; l < e; l++) { if (link[l] == '\0') { break; } else { link[l] = link[l+8]; } }
					}
					//o = l;
					//break;
				}
				else if (link[o] == 'w' && link[o+1] == 'w' && link[o+2] == 'w' && link[o+3] == '.') { /* do nothing */ }
				else { /* do nothing */ }
			}
			// find final character and set to null byte
			if (link[o-1] == '\n') {
				if (link[o-2] == '\r') { link[o-2] = '\0'; }
				else { link[o-1] = '\0'; }
			}
			else { link[o] = '\0'; }
			// scroll through final data and append to temp
			for (o = 0; o < e; o++) {
				if (link[o] == '\0') { temp[t++] = ' '; temp[t] = '\0'; break; }
				else { temp[t++] = link[o]; }
			}
		}
	}
	
	// return condition
	return temp;
	
}
