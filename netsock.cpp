// name: netsock.cpp
// version: 1.0
// description: handles all networking routines

// standard libraries
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

// versidyne libraries
#include "os.h"
#include "error.h"

// windows networking
#if defined OS_WIN
	#include <windows.h>
	#include <windowsx.h>
	#include <commctrl.h>
	#include <winsock.h>
#endif

// unix networking
#if defined OS_UNIX
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
#endif

// class header
#include "netsock.h"

// constructors
netsock::netsock () { /* bi = 0; */ }

// operators
/*netsock & netsock::operator << (const char* buf) {

	int len = strlen(buf);

	memmove (buffer+bi, buf, len);

	bi += len;

	if (buf[len-1] == '\n' && buf[len-2] == '\r') {

		send(fd,buffer,bi,0);

		bi = 0;

	}

	return *this;

}*/

// connect function
int netsock::connect (const char *hostname, int port) {

	// create data blocks
	int csock;
	struct hostent *server;
	struct sockaddr_in serv_addr;

	#if defined OS_WIN
		DWORD dwError;
		WORD wVersionRequested;
		WSADATA wsaData;
	#endif

	// open socket
	csock = ::socket(AF_INET, SOCK_STREAM, 0);

	if (csock < 0) {
		error(1, 1, "opening socket");
	}

	// initialize winsock
	#if defined OS_WIN
		wVersionRequested = MAKEWORD( 1, 1 );
		WSAStartup( wVersionRequested, &wsaData );
	#endif

	// store server name
	server = gethostbyname(hostname);

	// check if the server exists
	if (server == NULL) {

		#if defined OS_WIN
			
			dwError = WSAGetLastError();
			
			if (dwError == WSAHOST_NOT_FOUND) {
				printf("Host not found\n");
			}
			
			else if (dwError == WSANO_DATA) {
				printf("No data record found\n");
			}
			
			else {
				printf("Function failed with error: %ld\n", dwError);
			}
			
		#endif

		#if defined OS_UNIX
			error (1, 2, "host non-existant");
		#endif

		return 0;

	}

	// prepare connection (clear the buffer)
	#if defined OS_UNIX
		bzero((char *) &serv_addr, sizeof(serv_addr));
	#endif

	#if defined OS_WIN
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
	#endif

	serv_addr.sin_family = AF_INET;

	#if defined OS_UNIX
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	#endif

	#if defined OS_WIN
		memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	#endif

	serv_addr.sin_port = htons(port);

	// connect to server
	if (::connect(csock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
		error(1, 3, "connecting");
	}
	
	// set local
	socket = csock;
	
	// return data
	return csock;

}

// disconnect interface
int netsock::disconnect () { disconnect (socket); };

// disconnect (windows)
#if defined OS_WIN
	//int netsock::disconnect (SOCKET sockfd) { closesocket(sockfd); }
	int netsock::disconnect (int sockfd) { closesocket(sockfd); }
#endif

// disconnect (unix)
#if defined OS_UNIX
	int netsock::disconnect (int sockfd) { close(sockfd); }
#endif

// listen for connection
int netsock::listen (int port) {

	// create data blocks
	int lsock;
	struct sockaddr_in serv_addr;

	// generate socket
	lsock = ::socket(AF_INET, SOCK_STREAM, 0);

	// check for errors
	if (lsock < 0) { error(1, 4, "opening socket"); }

	// set up socket (clear the buffer)
	#if defined OS_UNIX
		bzero((char *) &serv_addr, sizeof(serv_addr));
	#endif

	#if defined OS_WIN
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
	#endif

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// bind socket
	if (bind(lsock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error(1, 5, "binding");
	}

	// listen for connections
	::listen (lsock, 5);

	// return socket
	return lsock;

}        

// sending interface
int netsock::send (const char *data) { send(socket, data); };

// sending function
int netsock::send(int sock, const char *data) {

	// create data blocks
	int n;

	// write to server
	#if defined OS_UNIX
		n = write(sock, data, strlen(data));
	#endif

	#if defined OS_WIN
		n = send(sock, buffer, strlen(data), 0);
	#endif
	
	if (n < 0) { error(1, 6, "writing to socket"); }
	
	// return success
	return n;

}

// receiving interface
const char *netsock::receive() { return receive(socket); }

// receiving function
const char *netsock::receive(int sock) {

	// declare variables
	int bytes_read = 0;
	std::string temp;
	
	// erase string
	temp.empty();
	
	// receive
	// do {
	while (1) {
		memset(buffer, 0, strlen(buffer));
		bytes_read = recv(sock, buffer, sizeof(buffer), 0);
		if (bytes_read > 0) { temp += buffer; break; }
		else { break; }
	}
	//} while (bytes_read > 0)
	
	// handle errors
	if (bytes_read < 0) { error(1, 7, "reading from socket"); }
	
	// return data
	return temp.c_str();

}

/*int netsock::accept (int& sock, sockaddr_in* cli_addr, int* clilen) {

	// create data blocks
	int newsockfd;

	// run accept
	#if defined OS_UNIX
		newsockfd = ::accept (sock, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
	#endif

	#if defined OS_WIN
		newsockfd = ::accept (sock, (struct sockaddr *) &cli_addr, (int*) &clilen);
	#endif

	// return data natively
	return newsockfd;

}*/
