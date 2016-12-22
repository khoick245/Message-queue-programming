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
    // create queue id 
    int qid = msgget(ftok(".", 'u'), IPC_EXCL | IPC_CREAT | 0600);
 
    stringMsg center[60];   // store strings from clients
    stringMsg receiveMsg;   // store string receiving each single time
    stringMsg msg;      // response to client
 
    // calculate size of real message
    int sizeStringMsg = sizeof(stringMsg) - sizeof(long);
 
    int centerIndex = 0;    // index of center
    int countExit = 0;
 
    do
    {
        // receive string from client with mtype = 117
        msgrcv(qid, (struct msgbuf *)&receiveMsg, sizeStringMsg, 117, 0);
 
        if (receiveMsg.content[0] == '1')   // client send string
        {
            center[centerIndex] = receiveMsg;   // put string to center
            cout << "\nServer receive string " << centerIndex + 1 << " from client " << receiveMsg.content[1] << endl;
            cout << "String: ";
            for (int j = 3; j < 103;j++)
                cout << receiveMsg.content[j];
            centerIndex++;
        }
 
        if(receiveMsg.content[0] == '2')    // client request data
        {
            cout << "\nServer receive a request from client " << receiveMsg.content[1] << " for string " << receiveMsg.content[2];
            cout << "\nSending a respone...";
            for(int i=0; i < centerIndex; i++)
                if(receiveMsg.content[1] == center[i].content[1] && receiveMsg.content[2] == center[i].content[2])  // the same client and id
                {
                    msg.mtype = receiveMsg.content[1] - '0'; // respone mtype = client name
                    strcpy(msg.content, center[i-1].content);
                    msgsnd(qid, (struct msgbuf *)&msg, sizeStringMsg, 0);   
                }
        } 
        if(receiveMsg.content[0] == '3')    // exit
        {
            countExit++;
        }
         
    } while (countExit < 3);
 
    cout << "\nServer's exitting...";
     
    // now safe to delete message queue
    msgctl(qid, IPC_RMID, NULL);
 
    return 0;
}
