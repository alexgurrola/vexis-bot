// name: emotion.h
// version: 1.0
// description: creates emotions

// ensure declarations only occur once
#ifndef EMOTION_H
#define EMOTION_H

// text parsing class
class emotion {
	
	public:
		// constructors
		emotion ();
		emotion (char *raw);
		// operators
		void operator = (char *raw);
		
	private:
		// local data
		char *storage;
		
};

#endif
