// name: netsock.h
// version: 1.0
// description: handles all networking routines

// ensure declarations only occur once
#ifndef NETSOCK_H
#define NETSOCK_H

// network sockets class
class netsock {

	public:
		// constructors
		netsock ();
		// interfaces
		int connect (const char *hostname, int port);
		int disconnect ();
		int send (const char *data);
		const char *receive ();
		// functions
		int listen (int port);
		//int accept (int& sockfd, sockaddr_in* cli_addr, int* clilen);
    
    private:
		// local variables
		int socket;
		char buffer[5120];
		// functions
		int disconnect (int sock);
		int send (int sock, const char *data);
		const char *receive (int sock);
    
};

#endif
