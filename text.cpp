// name: text.cpp
// version: 1.0
// description: opens a file and displays contents

// libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

// namespaces
using namespace std;

// text parser header
#include "text.h"

// constructors
text::text () { /* do nothing */ }

// constructors
text::text (char *raw) { storage = raw; }

// operators
void text::operator = (char *raw) { storage = raw; }

// remove whitespace from string
void text::clean (char *data) {
	
	// declare variables
	int i, x;
	
	// formulate page for digestion
	for (i = 0; i < strlen(data); i++) {
		
		// check for skipped whitespace
		if (data[i-1] == ' ' || data[i-1] == '\t') { i = i-1; }
		
		// remove whitespace
		if (data[i] == ' ' || data[i] == '\t') {
			
			// erase current character
			for (x = i; x < strlen(data); x++) { data[x] = data[x+1]; }
			
		}

	}
	
}

// remove whitespace from string
void text::clean (char *data, char delim) {
	
	// declare variables
	int i, x;
	
	// formulate page for digestion
	for (i = 0; i < strlen(data); i++) {
		
		// check for skipped whitespace
		if (data[i-1] == ' ' || data[i-1] == '\t') { i = i-1; }
		
		// check for a delimiter to stop
		if (data[i] == delim || data[i-1] == delim) { break; }
		
		// remove whitespace
		if (data[i] == ' ' || data[i] == '\t') {
			
			// erase current character
			for (x = i; x < strlen(data); x++) { data[x] = data[x+1]; }
			
		}

	}
	
}

// finds if a needle exists in a haystack
bool text::contains (char *haystack, char needle) {
	
	// declare variables
	bool cont = false;
	char *temp;
	
	// find first instance of needle		
	temp = strchr(haystack, needle);
	
	// check if found
	if (temp != NULL) { cont = true; }
	
	// return condition
	return cont;
	
}

// finds first occurrence of data in 
int text::find (char *haystack) {
	
	// declare variables
	int i, pos = 0;
	
	// save and break on first alphanumeric
	for (i = 0; i < strlen(haystack); i++) {
		if (isalnum(haystack[i])) { pos = i; break; }
	}
	
	// return condition
	return pos;
	
}

// finds first occurrence of needle in a haystack
int text::find (char *haystack, char needle) {
	
	// declare variables
	int pos = 0;
	char *temp;
	
	// find first instance of needle		
	temp = strchr(haystack, needle);
	
	// determine position
	if (temp != NULL) { pos = strlen(haystack) - strlen(temp); }
	
	// return condition
	return pos;
	
}

// determines the type of data in a string
// unknown=0, int=1, float=2, char*=3
int text::type (char *data) {
	
	// declare variables
	int datatype, i;
	
	// initial unknown
	datatype = 0;
	
	// evaluate data
	for (i = 0; i < strlen(data); i++) {
		
		// detect numeric
		if (isdigit(data[i])) { if (datatype == 0) { datatype = 1; } }
		
		// detect float
		if (datatype == 1 && data[i] == '.') { datatype = 2; }
		
		// detect alphabet
		if (isalpha(data[i])) { datatype = 3; }
		
	}
	
	// return type
	return datatype;
	
}

// returns a value from a text file
char *text::get (const char *section, const char *key) { return get (section, key, storage); }

// returns a value from a text file
char *text::getchar (const char *section, const char *key) { return get (section, key, storage); }

// converts a value to float
float text::getfloat (const char *section, const char *key) { return (float)atof(get(section, key, storage)); }

// converts a value to int
int text::getint (const char *section, const char *key) { return (float)atoi(get(section, key, storage)); }

// parses a text file
char *text::get (const char *section, const char *key, const char *file) {
	
	// declare variables
	bool spres=false, kpres=false, vpres = false;
	char ch, *line, pline[2048], *temp;
	ifstream fin(file);
	
	// check if file exists
	if (fin) {
		
		// initialize array
		line = pline;
		
		// erase array
		memset(line, 0, strlen(line));
		
		// cycle through data
		while (fin.get(ch)) {
			
			// append character to string
			line[strlen(line)] = ch;
			line[strlen(line)+1] = '\0';
			
			// check if character was end of a line
			if (line[strlen(line)-1] == '\n' && line[strlen(line)-2] == '\r') {
				
				// save line
				temp = parseline(line);
				
				// pass complete line to interpreter
				if (strcmp(temp, section) == 0) { spres = true; kpres = false;}
				if (strcmp(temp, key) == 0) { kpres = true; }
				
				// return value
				if (spres && kpres) {
					// set
					spres = false;
					kpres = false;
					vpres = true;
					// stop searching
					break;
				}
				
				// erase array for new line
				memset(line, 0, strlen(line)); 
				
			}
			
		}
		
	}
	
	fin.close();
	
	if (vpres) { return parseline(line, true); }
	else { return '\0'; }
	
}

// parses a text string into an array
char *text::parseline (char *data) { parseline(data, false); }

// parses a text string into an array
char *text::parseline (char *data, bool rvalue) {
	
	/* clean lines */
	
	// data strings
	if (contains(data, ':')) { clean(data, ':'); }
	
	// sections and empty lines
	else if (contains(data, ']') || type(data) == 0) { clean(data); }
	
	// extended data
	else { clean(data, data[find(data)]); }
	
	/* parse data */
	
	// skip any blank lines
	if(strlen(data) == 0 || strlen(data) == 1 && data[0] == '\n') { return (char*)"\0\0"; }

	// skip any comment lines
	//else if(data[0] == '#') { /* do nothing */ }
	
	// handle bracketed lines
	else if(data[0] == '[') {
		
		// determine section name
		string stemp = data;
		stemp = stemp.substr(1, find(data, ']')-1);
		char *sname = (char*)stemp.c_str();
		
		return sname;
		
	}
	
	// process valid lines
	else {
		
		// initial data
		if (contains(data, ':')) {
			
			// find data splitter
			int pos = find(data, ':');
			
			// split key & data
			string dtemp = data;
			string keytemp = dtemp.substr(0, pos);
			string valuetemp = dtemp.substr(pos+1, strlen(data)-pos+1);
			
			// assign data
			char *key = (char*)keytemp.c_str();
			char *value = (char*)valuetemp.c_str();
			
			// clean data
			clean(value, value[find(value)]);
			
			// return data
			if (rvalue == false) { return key; }
			else { return value; }
			
		}
		
		// data continuation
		else {
			
			// move characters backwards
			int x;
			
			for (x = strlen(data); x > 0; x--) { data[x] = data[x-1]; }
			
			data[0] = ' ';
			data[strlen(data)] = '\0';
			
			return data;
			
		}
		
	}
	
}

// sets a value in a text file
bool text::set (const char *section, const char *key, const char *data) { return set (section, key, data, storage); }

// sets a value in a text file
bool text::set (const char *section, const char *key, float data) {
	
	// declare variables
	string cdata;
	ostringstream converter;
	
	// convert data
	converter << data;
	cdata = converter.str();
	
	// append end of data
	cdata += " \r\n";
	
	// return success
	return set (section, key, cdata.c_str(), storage);
	
}

// sets a value in a text file
bool text::set (const char *section, const char *key, int data) {
	
	// declare variables
	string cdata;
	stringstream converter;
	
	// convert data
	converter << data;
	cdata = converter.str();
	
	// append end of data
	cdata += "\r\n";
	
	// return success
	return set (section, key, cdata.c_str(), storage);
	
}

// sets a value in a text file
bool text::set (const char *section, const char *key, const char *data, const char *file) {
	
	// declare variables
	bool spres=false, kpres=false, vpres = false;
	char ch, *line, pline[2048], *temp;
	string ofile = file;
	
	// backup file name
	ofile += ".bak";
	
	// rename original file
	rename(file, ofile.c_str());
	
	// construct streams
	ofstream fout(file);
	ifstream fin(ofile.c_str());
	
	// check if file exists
	if (fin) {
		
		// initialize array
		line = pline;
		
		// erase array
		memset(line, 0, strlen(line));
		
		// cycle through data
		while (fin.get(ch)) {
			
			// append character to string
			line[strlen(line)] = ch;
			line[strlen(line)+1] = '\0';
			
			// check if character was end of a line
			if (line[strlen(line)-1] == '\n' && line[strlen(line)-2] == '\r') {
				
				// save line
				temp = parseline(line);
				
				// pass complete line to interpreter
				if (strcmp(temp, section) == 0) { spres = true; kpres = false;}
				if (strcmp(temp, key) == 0) { kpres = true; }
				
				// check for value
				if (spres && kpres) {
					// set
					spres = false;
					kpres = false;
					vpres = true;
					// replace value
					if (data[strlen(data)] == '\n' && data[strlen(data)-1] == '\r') { fout << key << ": " << data; }
					else { fout << key << ": " << data << "\r\n"; }
				}
				
				// change nothing
				else { fout << line; }
				
				// erase array for new line
				memset(line, 0, strlen(line)); 
				
			}
			
		}
		
	}
	
	// close files
	fin.close();
	fout.close();
	
	// remove original file
	remove(ofile.c_str());
	
	// return success
	if (vpres) { return true; }
	else { return false; }
	
}
