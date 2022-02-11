/*
 
 udp_client.c
 Rahul Seth
 3D3: Computer Networks Project 1
 
 This file serves as the client node L1. L1 is connected to an IR sensor that sends a signal to the light ON/OFF depeding upon traffic movement.
 Once a trigger ON has been sent to the node L1, L1 turns on sends a further signal to the RCU indicating traffic.
 
 As, we were only interested in the data transfer on the project, for simulation purposes I pass on random number to the client which indicate state of the IR sensor 0->No traffic, 1->Traffic (L1 turns on).
 I send the numbers as strings from the client to the server at an interval of every 5s (for simulation purposes). In reality, this would be different as the IR sensor would be sending real time signals.
 
 Usage: make udp_client.c
        ./udp_client.c
 */

#include <stdio.h> //std I/O
#include <stdlib.h> //std lib
#include <string.h> //for strings
#include <unistd.h> // for close
#include <math.h>
#include <time.h> //rand, srand
#include <sys/socket.h> //socket
#include <sys/types.h> //socket
#include <netinet/in.h> //struct sockaddr
#include <arpa/inet.h> //in_addr
#define PORT 8081

// --------------------------------------------//
// Fucntions for random number generation and list to store numbers //
int rand50(){
    return rand() & 1;
}
//list to store IR data

struct node{
    int traffic_load;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

void insert(struct node **head, int data){
    struct node *link = (struct node*)malloc(sizeof(struct node));
    link -> data = data;
    link -> next = *head;
    *head = link;
}

//Testing
void printList(struct node *head){
    while(head){
        printf(" %d ", head -> data);
        head = head -> next;
    }
}





// --------------------------------------------//
int main(){
    
    struct node *ptr = NULL;
    
    //putting ir data to list
    srand(time(NULL));
    for(int i=0; i<10; i++){
        insert(&ptr,rand50());
    }
    
    //printing data
   // printf("\n\t Data in List: \n");
   // printList(ptr);
    char buffer[1024]; // msg sent to server
    char server_message[1024]; //msg by server
    //clearing buffer and server message
    memset(buffer, '\0', sizeof(buffer));
    memset(server_message, '\0', sizeof(server_message));
    
    // --------------------------------------------//
    int clientSocket;
    int sendData;
    int rcvData;
    
    // --------------------------------------------//
    //Server Architecture
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int server_struct_size = sizeof(server_addr);
    
    //---------Creating socket------------//
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    printf("\n\tSocket no: %d\n", clientSocket);
    for(int i=0; i<50; i++)
        printf("-");
    
    //Checking if socket creation was successful
    if(clientSocket < 0){
        printf("\n\t[-]Error Creating Socket\n");
        exit(1);
    }
    else
        printf("\n\t[+]Socket Creation Success.\n");
    //--------------------------------------//
    for(int i=0; i<50; i++)
        printf("-");
    
    //The loop sends 1 number from the list to the server at one time at an interval of every 5s
    int i=0;
    while(ptr!=NULL){
        
        printf("\n\tSending IR sensor data to server: \n");
        for(int i=0; i<50; i++)
            printf("-");
        printf("\n");
        //data conversion
        sprintf(buffer, "%d", ptr -> data); //conversion of int to string
        //gets(buffer);
        //Sending data to server
        sendData = sendto(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*) &server_addr, server_struct_size);
        
        if(strcmp(buffer,"exit")==0){
            printf("\n\t[+]Client Disconnecting.\n");
            break;
        }
        //Checking if data transmission was succcesful
        if(sendData<0){
            printf("\n\t[-]Data Transmission Unsucessful.\n");
            exit(1);
        }
        else printf("\n\tData Transmission Sent.\n");
        
        for(int i=0; i<50; i++)
            printf("-");
        printf("\n");
            
        // receiving data from server, ack
        rcvData = recvfrom(clientSocket, server_message, sizeof(server_message), 0, (struct sockaddr *) & server_addr, &server_struct_size);
        //checking recv data
        if(rcvData < 0){
            printf("\n\t[-]Error receiving Data\n");
            exit(1);
        } else printf("\n\t[+]Data Recive Success.\n");
        
        printf("\n\tServer Message: %s", server_message);
        printf("\n");
        
        for(int i=0; i<50; i++)
            printf("-");
        printf("\n");
        
        //Clearing buffer, server message
        memset(server_message, '\0', sizeof(server_message));
        memset(buffer, '\0', sizeof(buffer));
        sleep(4); //stops for 4s
        ptr = ptr -> next; //next number for transmission
    }
    
    // --------------------------------------------//
    //clsing socket
    close(rcvData);
    close(clientSocket);
    
    return 0;
}
