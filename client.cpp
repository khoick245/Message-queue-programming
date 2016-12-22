#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
 
using namespace std;
 
struct stringMsg
{
    long mtype;
    char content[103];
};
 
/*
 
content[0]: type of message.
    content[0] = '1': sending message
    content[0] = '2': request a string
    content[0] = '3': notice to exist
 
content[1]: name of client. Value = '1', '2', or '3'
 
content[2]: id of string of client. Value = '1' to '20'
 
content[3]-content[103]: content of string 
 
*/
 
int main()
{
 
    // get id of queue
    int qid = msgget(ftok(".", 'u'), 0);    // key_t ftok(const char *pathname, int proj_id);
 
    int client;     // hold client name
 
    // calculate size of real message
    int sizeStringMsg = sizeof(stringMsg) - sizeof(long);
 
    stringMsg msg;      // for sending to data center
    stringMsg receivedMsg;  // for receiving from data center
 
    cout << "Enter client(1-3): ";
    cin >> client;
 
    // send 20 strings
    for (int i = 0;i < 20;i++)
    {
        msg.mtype = 117;            // only one mtype for all sending messages
        msg.content[0] = '1';           // send data
        msg.content[1] = '0' + client;      // change client from int to char
        msg.content[2] = '0' + i;       // change index of each string from int to char
        cout << "\nClient " << client << "'s sending string " << i+1 << " to data center" << endl;
        for (int j = 3; j < 103;j++)
        {
            msg.content[j] = rand() % (90 - 65) + 65;       // generate random ascii between A and Z
            cout << msg.content[j];
        }       
 
        msgsnd(qid, (struct msgbuf *)&msg, sizeStringMsg, 0);       // send message string
    }
 
    int index;  // index of string to retrieve
    do
    {
        cout << "\nEnter index to retrieve(-1 to exist): ";
        cin >> index;
 
        if(index != -1) // not exit
        {
            msg.mtype = 117;
            msg.content[0] = '2';       // request data
            msg.content[1] = '0' + client;
            msg.content[2] = '0' + index;
            msgsnd(qid, (struct msgbuf *)&msg, sizeStringMsg, 0);           // send message string
            msgrcv(qid, (struct msgbuf *)&receivedMsg, sizeStringMsg, client, 0);   // wait a respone from server with mtype = client 
            cout << "String: ";
            for (int j = 3; j < 103;j++)
                cout << receivedMsg.content[j];
        }
        else // exit
        {
            msg.mtype = 117;
            msg.content[0] = '3';       // notice to exist
            msg.content[1] = '0' + client;
            msgsnd(qid, (struct msgbuf *)&msg, sizeStringMsg, 0);   
        }
    }while(index != -1);
 
    cout << "\nClient " << client << " existed...";
 
    return 0;
}
