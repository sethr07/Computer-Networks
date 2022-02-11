/*
 
 3D3: Project 1: Smart Street Lights
 udp_server.c
 Rahul Seth
 Group 24
 
 UDP client-server model
 
 Server File
 
 The server acts as the RCU in our design. Data from IR sensor triggers the first node in the design.
 The lode L1(refer to diagram in report) sends a trigger signal to the server indication it is on.
 The server applies some lgorithim to predict the trajectory of the car/pedestrian traffic and turns on certian lights in a radius for a finite time.
 This Server desgined by us receives a message from the node L1 indication whether it is on/off. It sends an acknowldgement back to the node after receiving message.
 If L1 -> ON, the server sends a broadcast signal to the nodes calculated by the trajectory algorithim indicating them to turn ON for a finite time.
 */

#include <stdio.h> //standard I/O
#include <stdlib.h>
#include <string.h>
#include <netdb.h> //hostnet
#include <unistd.h> // for close
#include <sys/socket.h> //socket
#include <sys/types.h> //socket
#include <netinet/in.h> //strcut sockaddr
#include <arpa/inet.h> //inaddr
#define PORT 8081


// --------------------------------------------//

int main(){
    
    char server_message[2000]; // server acknowledgement
    char client_msg1[2000]; //Msg from client
    char bmesg[2000];
    
    // --------------------------------------------//
    int newSocket; // socket no
    int bind_socket; //variable for binding socket
    int rcv_data; //variable for rcving of data
    int sent_data; //sent data variable
    int address_len; // IP address length
    int sent_data2;
    
    // --------------------------------------------//
    //For host details
    struct hostent *host_entry;
    int hostname;
    char hostbuffer[256];
    char *IPbuffer;
    
    //server structures
    struct sockaddr_in address, clientAddr;
    
    //Clearing Data using memery set
    memset(server_message, '\0', sizeof(server_message));
    memset(client_msg1, '\0', sizeof(client_msg1));
    
    // --------------------------------------------//
    //Server Acknowledgement messages
    strcpy(server_message, "\n\tAcknowledged.\n");
    strcpy(bmesg, "\n\tTurn on L7, L9, L4.\n");
  
    // --------------------------------------------//
    //Assiging IP/PORT
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    address.sin_family = AF_INET;
    
    //clientAddr.sin_port = htons(BPORT);
    //clientAddr.sin_family = AF_INET;
    
    int cli_len = sizeof(clientAddr);
    address_len = sizeof(address);
   
    
    //---------Creating socket------------//
    newSocket = socket(AF_INET, SOCK_DGRAM, 0);
    printf("\n\tSocket no: %d\n", newSocket);
    
    //Checking socket creation
    if(newSocket < 0){
        printf("\t[-]Error Creating Socket\n");
        exit(1);
    }
    else printf("\n\t[+]Socket Creation Success.\n");
    //--------------------------------------//
    
    
    //---------Binding socket------------//
    bind_socket = bind(newSocket, (struct sockaddr *)&address, sizeof(address));
    //Checking Bind Process
    if(bind_socket < 0){
        printf("\t[-]Error Binding Socket\n");
        exit(1);
    }
    else printf("\n\t[+]Binding Success.\n");
    //--------------------------------------//
    for(int i=0; i<50; i++)
        printf("-");

    //--------------------------------------//
here:
    printf("\n\tREMOTE CONTROL UNIT SERVER ON.\n\n");
    printf("-");
    for(int i=0; i<50; i++)
        printf("-");
    printf("\n");
    
    while(1){
        
        //Receive Data from client
        rcv_data = recvfrom(newSocket, client_msg1, sizeof(client_msg1), 0, (struct sockaddr *)&address, &address_len);
        
        //retreving Client Data name
        hostname = gethostname(hostbuffer, sizeof(hostbuffer));
        host_entry = gethostbyname(hostbuffer);
        IPbuffer = inet_ntoa(*((struct in_addr*) host_entry -> h_addr_list[0]));
        
        //checking if recv process was successful
        if(rcv_data < 0){
            printf("\n\t[-]Error receiving data\n");
            exit(1);
        }
        else printf("\n\t[+]Message recieved Success.\n");
        
        for(int i=0; i<50; i++)
            printf("-");
        printf("\n\t[+] Client Details: \n");
        for(int i=0; i<50; i++)
            printf("-");
        printf("\n");
        // --------------------------------------------//
        printf("\n\t[+]Hostname: %s\n", hostbuffer);
        printf("\n\t[+]Host IP: %s\n", IPbuffer);
        printf("\n");
        // --------------------------------------------//
        //printing msg from client, i.e. on/off
        printf("\n\tMessage from Client: %s\n", client_msg1);
        // --------------------------------------------//

        // Logic for making sense of IR data //
        if(strcmp(client_msg1, "1")==0){
            
            printf("\n\tL1 -> ON. Applying trajectory algorithim and sending broadcasting to other lights.\n");
            sent_data = sendto(newSocket, server_message, sizeof(server_message), 0, (struct sockaddr*)&address, address_len); // send ack
            //checking if data transmission was succesful
            if(sent_data < 0){
                printf("\n\t[-]Error sending ACK\n");
                exit(1);
            } else printf("\n\t[+]Acknowledgement sent.\n");
            
            // --------------------------------------------//
            //sending broadcast
            //clientAddr.sin_addr.s_addr = inet_addr("225.225.225.225");
            sent_data2 = sendto(newSocket, bmesg, sizeof(bmesg), 0, (struct sockaddr *)&address, address_len);
            if(sent_data2 < 0){
                printf("\n\t[-]Error sending Broadcast\n");
                exit(1);
            } else printf("\n\t[+]Broadcast sent.\n");
        }
        // --------------------------------------------//

        if(strcmp(client_msg1, "0")==0){
            printf("\n\tL1 -> OFF.\n");
            sent_data = sendto(newSocket, server_message, sizeof(server_message), 0, (struct sockaddr*)&address, address_len); //send ack
            if(sent_data < 0){
                printf("\n\t[-]Error sending data\n");
                exit(1);
                }
            else printf("\n\t[+]Acknowledgement sent.\n");
     }
        // --------------------------------------------//
        if(strcmp(client_msg1, "exit")==0){
            printf("\n\t[-]Client has Disconnected.\n");
            goto here;
        }
        
        //clearing all buffers
        
        memset(server_message, '\0', sizeof(server_message));
        memset(client_msg1, '\0', sizeof(client_msg1));
}
    
    close(newSocket);
    return 0;
}
