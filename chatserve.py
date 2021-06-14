#Author: Alex Grasley
#Class: CS_272 Intro to Networks
#References and ideas: https://medium.com/swlh/lets-write-a-chat-app-in-python-f6783a9ac170

import socket
import select
from threading import Thread

IP = "127.0.0.1"
PORT = 0 #going to need to get user input()
nameBuffer = 10
msgBuffer = 4096
Address = (IP, PORT)

PORT = raw_input("Enter Desired Port: ")
PORT = int(PORT)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(Address)

#get username from server user
userName = raw_input("What's your name? \r\n")
print("Welcome %s, waiting for connection" % userName)

#start waiting for a single connection
server_socket.listen(1)
print("Ready to Connect")
client, client_address = server_socket.accept()

while True:
    #accept incoming connection    
    recvMessage = client.recv(1024)
    if not recvMessage:
        print("Error getting message")

    if(recvMessage == "\\quit\0"): #see if client asked to end session
        client.close() #client asked to end, close down
        server_socket.close()
        break;
    else:
        print(recvMessage)
        message = raw_input(userName + "> ")
        if( message == "\\quit"):
            client.send(message)
            client.close()
            server_socket.close()
            break;
        else:    
            message = userName + "> " + message
            client.send(message)
            message = ""
            recvMessage = ""

print("Nice chatting with you, bye!")

#its all over, get out
server_socket.close()