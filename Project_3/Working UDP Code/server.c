//
//  main.c
//  CS158A_Project2
//
//  Created by Phuc Nguyen on 3/9/15.
//  Copyright (c) 2015 Phuc Nguyen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <time.h>

#define USAGE "ether N"


//Print system error message if there is any
void printError(const char* message)
{
    perror(message);
    exit(1);
}

int main(int argc, char* argv[])
{
    //Properties
    int sock;
    int serverLength;
    int returnValue;
    int portNumber;
    socklen_t clientLength;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[2048];
    
    if (argc < 2) {
        fprintf(stderr, "Error! Must provide port number.\n");
        exit(1);
    }
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock < 0) {
        printError("Error! Cant't open socket.\n");
    }
    
    serverLength = sizeof(server);
    clientLength = sizeof(client);

    
    bzero(&server, serverLength);
    portNumber = atoi(argv[1]);
    
    server.sin_family = AF_INET;
//	inet_aton("10.189.249.188", server.sin_addr);
   // server.sin_addr.s_addr = "10.189.249.188";
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portNumber);
    
    if (bind(sock, (struct sockaddr *) &server, serverLength) < 0) {
        printError("Error! Can't bind.\n");
    }
    
    //Variables for keeping track of timeslot
    time_t timeSlot = 0.8;
    time_t start, end, now;
    time_t diff;
    
    int count = 1;
    while (1) {
        
        
        //Receive from Client
        returnValue = recvfrom(sock, buffer, 2048, 0, (struct sockaddr*)&client, &clientLength);
        
        //if there is a collision
        while (returnValue <= 0) {
            puts("Error! Can't receive from client.\n");
            puts("Collision happens. Code = 1");
            char code = '1';
            returnValue = sendto(sock,&code, 1, 0, (struct sockaddr*)&client, clientLength);
        }
        
        
        
        printf("Packet #%d received: %s\n", count++, buffer);
    
        //Send to client
        returnValue = sendto(sock,"Packet received.\n", 17, 0, (struct sockaddr*)&client, clientLength);
        
        if (returnValue < 0) {
            printError("Error! Can't send to client");
        }
        //Clear the buffer
        bzero(buffer, strlen(buffer));

    }
    
    return 0;
    
}
