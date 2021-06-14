/*******
**@Author Alex Grasley
**@Class CS_272 Intro to NetWorks
**@Project 1 - TCP Chat Client in C++
**@References -  Sloan Kelly - https://gist.github.com/codehoose/d7dea7010d041d52fb0f59cbe3826036
*******/


#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

main(){

	// create a socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		return 1; // error, bail out
	}
	// create a hint structure for server
	int port = 0; 
	string ipAddress = "";  

	//get server IP from user
	cout << "Please Enter your desired server's IP: ";
	cin >> ipAddress;
	//get server port from user
	cout << "Please enter the server's active port number: ";
	cin >> port;
	//clear input buffer so we can use cin and getline together
	cin.ignore();
	cin.clear();
	cin.sync();

	// get user input for server port and address
	//cout << "Please enter server port: ";
	//cin >> port;
	//cout << endl;
	//cout << "Enter host server IP4 Address: ";
	//cin >> ipAddress;


	// cout << "I have accepted the address as " << ipAddress << endl;
	

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// get username
	string userName = "";
	cout << "What's your name?\r\n";

	getline(cin,userName);
	cout << "Hello " << userName << "\r\n"
		 << "Ready to chat!" << endl;



	//attempt to connect to server.
	int connectResult = connect(sock, (sockaddr*)& hint, sizeof(hint));
	if (connectResult == -1) {
		cout << "Failure Connecting..." << endl;
		return 1; // error, bail out
	}
	else {
		cout << "connected!" << endl;
	}

	//set up variables for communication
	char buffer[4096];
	string userInput;
	//while loop for main communicat4ion
	while (true) {
		//enter lines of text
		cout << userName << ">";
		getline(cin, userInput);
		//cout << userInput.size() << endl;

		if (userInput.size() > 0) {			
			//send to server
			if (userInput == "\\quit") {
				send(sock, userInput.c_str(), userInput.size() + 1, 0);
				close(sock);
				break;
			}
			userInput = userName + "> " + userInput;
			//send(sock, userInput.c_str(), userInput.size() + 1, 0);
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult == -1) {
				cout << "Failure Sending, please try again.\r\n";
				continue;
			}
		}
		

		//wait for response
		memset(buffer, 0, 4096);
		int bytesRec = recv(sock, buffer, 4096, 0);
		if (bytesRec == -1) {
			// error
			cout << "Error receiving from server\r\n";
		}

		if (string(buffer, bytesRec) == "\\quit") {
			cout << "Server has ended session" << endl;
			close(sock);
			break;
		}
		else {
			//display response
			cout << string(buffer, bytesRec) << endl;
		}
		userInput = "";
		bytesRec = 0;
	}

	cout << "Nice chatting with you, bye!" << endl;
	// close socket
	close(sock);

	return 0;
}
