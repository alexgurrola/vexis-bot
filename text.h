// name: text.h
// version: 1.0
// description: opens a file and displays contents

// ensure declarations only occur once
#ifndef TEXT_H
#define TEXT_H

// text parsing class
class text {
	
	public:
		// constructors
		text ();
		text (char *raw);
		// operators
		void operator = (char *raw);
		// information
		bool contains (char *haystack, char needle);
		int find (char *haystack);
		int find (char *haystack, char needle);
		int type (char *data);
		// manipulation
		void clean (char *data);
		void clean (char *data, char delim);
		// gathering interfaces
		char *get (const char *section, const char *key);
		char *getchar (const char *section, const char *key);
		float getfloat (const char *section, const char *key);
		int getint (const char *section, const char *key);
		// setting interfaces
		bool set (const char *section, const char *key, const char *data);
		bool set (const char *section, const char *key, float data);
		bool set (const char *section, const char *key, int data);
		
		
	private:
		// local data
		char *storage;
		// conversion interface
		char *parseline (char *data);
		// conversion function
		char *parseline (char *data, bool rvalue);
		// gathering function
		char *get (const char *section, const char *key, const char *file);
		// setting function
		bool set (const char *section, const char *key, const char *data, const char *file);
		
};

#endif
