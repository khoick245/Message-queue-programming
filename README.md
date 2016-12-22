# Message-queue-programming
CECS 326 - CSULB - Ratana Ngo

Objectives: Interprocess communication and basic process coordination
Design & implement a "data center". Processes can store data & later retrieve it. A data center allows
client program to send data to be stored until it is needed later. When a client needs some data, the
client can request the data from the center.
The assignment is to construct one program for the data center & program(s) for 3 separate clients. (All
3 clients can & may be implemented as a single program.) The data center & the 3 clients execute as
concurrent processes. They communicate via a message queue.
When each client program starts, it sends 20 strings (numbered 1 to 20) to the data center to be stored.
Each string is sent separately. (Each string consists of 100 random upper case letters.) Then, the client
will retrieve & output a string based on an index (between 1 & 20) the user enters. For example, when
the user enters an index 5, it will retrieve & display the fifth string it sent to the data center. Repeatedly
prompts the user to enter an integer, where minus 1 denotes exit. Upon exit, the client notifies the data
center.
The data center program accepts and stores strings from the 3 clients. Then, it returns whichever string
that its clients request. After all 3 clients terminated, the center also terminates.
