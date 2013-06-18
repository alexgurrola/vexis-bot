// name: http.h
// version: 1.2
// description: handles the http protocol

// ensure declarations only occur once
#ifndef HTTP_H
#define HTTP_H

// http handler
class http {
	
	public:
		// constructors
		http ();
		// functions
		const char *get (const char *hostname, const char *location);
		// document portions
		const char *head (const char *data);
		const char *body (const char *data);
		// header components
		//const char *metadata (const char *data);
		// body components
		const char *links (const char *data);
		//const char *data (const char *data);
		// sitemap gathering
		//const char *links (const char *data);
		
	private:
		// functions
		const char *clean (const char *data);
		
};

#endif
