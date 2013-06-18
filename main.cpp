// main.cpp -- attempts to chat with irc
// File Version: 2.0

// standard libraries
#include <iostream>
#include <string.h>

// versidyne libraries
//#include "http.h"
#include "netsock.h"
#include "text.h"

// namespaces
using namespace std;

// functions
char* lowercase(char* input);

// main program code
int main (int argc, char *argv[]) {

	// create data blocks
	bool pm;
	int port, senderlen;
	char temp[2048];
	char* output, *line, *outping, delims[] = ":";
	string input, buffer, sender, hostname, nickname, realname, channel;
	netsock socket;
	
	// set info
	nickname = "ShadowBot";
	realname = "ShadowDemon";
	hostname = "irc.cryto.net";
	channel = "#crytocc";
	//hostname = "irc.anonplus.com";
	//channel = "#jtorchat";
	port = 6667;
	
	// api access
	//cout << web.get("api.versidyne.com", "/?info=ip") << "\n";
	//cout << web.get("api.versidyne.com", "/?info=servername") << "\n";
	//cout << web.get("api.versidyne.com", "/?login=email&pass=password") << "\n";
	//cout << web.get("api.versidyne.com", "/?info=channels&session=session") << "\n";
	
	// connect and send info
	cout << "Connecting to: " << hostname << "\n";
	socket.disconnect();
	socket.connect(hostname.c_str(), port);
	input = "NICK " + nickname + "\r\n";
	cout << input;
	socket.send(input.c_str());
	input = "USER " + nickname + " " + "hostname" + " " + "servername" + " :" + realname + "\r\n";
	cout << input;
	socket.send(input.c_str());

	// begin eternal handling loop
	while (1) {
		
		// clear variables
		input.clear();
		buffer.clear();
		sender.clear();
		pm = false;

		// read incoming data
		output = (char*)socket.receive();
		if (output != NULL) { cout << output; }
		// initialize and copy memory
		//outcpy = malloc(strlen(output) + 1);
		//strcpy(outcpy, output);
		
		// replace PING with PONG and return
		outping = strstr(output, "PING :");
		if (outping != NULL) {
			strncpy(outping,"PO",2);
			cout << output;
			socket.send(output);
			continue;
		}
		
		// verify string format
		if (strstr(output, " ") != NULL) {
			buffer = strtok(output, " ");
			if ((char*)buffer.c_str() != NULL) {
				// save sender
				sender = buffer;
				buffer = strtok(NULL, " ");
				if (buffer.compare("001") == 0) {
					// set mode
					input = "MODE " + nickname + " +B\r\n";
					cout << input;
					socket.send(input.c_str());
					// set channel
					input = "JOIN " + channel + "\r\n";
					cout << input;
					socket.send(input.c_str());
				}
				else if (buffer.compare("353") == 0) {
					cout << "I have seen some users!\n";
				}
				else if (buffer.compare("PRIVMSG") == 0) {
					// gather sender and recipient
					sender.erase(0,1);
					senderlen = sender.find("!");
					sender.erase(int(senderlen),int(sender.length())-int(senderlen));
					buffer = lowercase(strtok(NULL, " "));
					// verify private messages
					if (buffer.compare(lowercase((char*)nickname.c_str())) == 0) { pm = true; }
					// jump to next cell (text)
					buffer = strtok(NULL, " ");
					// check if username was said
					if (strstr(lowercase((char*)buffer.c_str()), lowercase((char*)nickname.c_str())) != NULL) {
						buffer = lowercase(strtok(NULL, " "));
						// jump to the second character in output
						//buffer = strchr((char*)buffer.c_str(),':');
						// jump to the pointer where text begins
						//buffer = strchr((char*)buffer.c_str()+1,':');
						if (strstr(buffer.c_str(), "hi") != NULL) { input = "PRIVMSG " + channel + " :There's dirt, there's shit, then there's " + sender + "\r\n"; }
						else if (strstr(buffer.c_str(), "hello") != NULL) { input = "PRIVMSG " + channel + " :I don't understand why I bother responding.\r\n"; }
						else { input = "PRIVMSG " + channel + " :You're talking all this bullshit when you could be stroking my ego?\r\n"; }
						cout << input;
						socket.send(input.c_str());
					}
					// verify private message
					else if (pm == true) {
						// return to sender
						if (strstr(lowercase((char*)sender.c_str()), lowercase((char*)nickname.c_str())) == NULL) {
							if (strstr(buffer.c_str(), "help") != NULL) { input = "PRIVMSG " + sender + " :I don't get paid enough to help whatever you are.\r\n"; }
							else if (strstr(buffer.c_str(), "mode") != NULL) { input = "MODE " + nickname + " +B\r\n"; }
							else if (strstr(buffer.c_str(), "join") != NULL) { input = "JOIN " + channel + "\r\n"; }
							else if (strstr(buffer.c_str(), "part") != NULL) { input = "PART " + channel + "\r\n"; }
							else if (strstr(buffer.c_str(), "quit") != NULL) { input = "QUIT :I don't need this shit!\r\n";
								cout << input; socket.send(input.c_str()); break; }
							else { input = "PRIVMSG " + sender + " :If you need help, just ask.\r\n"; }
							cout << input;
							socket.send(input.c_str());
						}
					}
					// person spoke, but not specifically to the bot
					else {
						// compare text of user with his last text
						// if true, add 1 point
						// if false, reset to 0
					}
				}
				// unhandled messages
				else {
					// check for an incomplete block
					//cout << "\nUnhandled message: " << buffer << "\n";
				}
			}
		}
		// switch to next line
		//line = strstr(line, "\r\n");
		// break if end of line
		//if (line == NULL) { break; }
	}
	socket.disconnect();
	return 0;
}

// lowercase function
char* lowercase(char* input) {
	for (int i=0; i < strlen(input); i++) {
		input[i] = tolower(input[i]);
	}
	return input;
}
