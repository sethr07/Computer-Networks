#3D3: Computer Networks Project 1

#Provided File:
server.c and client.c

#Usage:
Build both files and then run the server first and then the client.
make server
./server
make client
./client


The server acts as the Remote Control in our design. Data from the IR sensor triggers the first node in the design.
The Node L1 (refer to the diagram in the report) sends a trigger signal to the server indicating it is on. The server applies some algorithm to predict the trajectory of the car/pedestrian traffic and turns on certain lights in a radius for a finite time.
This Server designed by us receives a message from the node L1 indication whether it is on/off. It sends an acknowledgment back to the node after receiving the message. If L1 -> ON, the server sends a broadcast signal to the nodes calculated by the trajectory algorithm indicating them to turn ON for a finite time. For the purpose project, we send a broadcast to random nodes. It is assumed the trajectory algorithm is there and working fine.

This file serves as the client node L1. L1 is connected to an IR sensor that sends a signal to the light ON/OFF depending upon traffic movement. Once a trigger ON has been sent to the node L1, L1 sends a further signal to the RCU indicating traffic.
As we were only interested in the data transfer on the project, for simulation purposes we passed on random numbers to the client which indicate the state of the IR sensor.
0->No traffic, 1->Traffic (L1 turns on).
We converted the numbers to string and sent the data from the client to the server at an interval of every 4s (for simulation purposes). In reality, this would be different as the IR sensor would be sending real time signals
