/*How QNX Message Passing Works
QNX follows a client-server model where:

A server creates a communication channel (ChannelCreate()).
A client connects to the server using ConnectAttach().
The client sends a request using MsgSend(), which blocks until a response is received.
The server receives the message using MsgReceive(), processes it, and replies using MsgReply().
The client receives the reply and resumes execution.
🔹 QNX IPC Example: Client-Server Message Passing
Let's implement a server that listens for messages and replies, and a client that sends a message to the server.*/

//QNX server code

#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>

#define SERVER_CHANNEL 1  // Arbitrary channel ID

typedef struct {
    int msg_type;
    char text[100];
} Message;

int main() {
    int chid = ChannelCreate(0);
    if (chid == -1) {
        perror("ChannelCreate failed");
        return -1;
    }

    printf("Server running. Waiting for messages...\n");

    while (1) {
        Message msg;
        int rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
        if (rcvid == -1) {
            perror("MsgReceive failed");
            continue;
        }

        printf("Received message: %s\n", msg.text);

        // Replying to the client
        Message reply;
        reply.msg_type = 1;
        strcpy(reply.text, "Hello from the server!");
        MsgReply(rcvid, 0, &reply, sizeof(reply));
    }

    return 0;
}

//qnx client code

#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>

#define SERVER_CHANNEL 1  // Same ID as server

typedef struct {
    int msg_type;
    char text[100];
} Message;

int main() {
    int server_coid = ConnectAttach(ND_LOCAL_NODE, 0, SERVER_CHANNEL, _NTO_SIDE_CHANNEL, 0);
    if (server_coid == -1) {
        perror("ConnectAttach failed");
        return -1;
    }

    Message msg;
    msg.msg_type = 1;
    strcpy(msg.text, "Hello, Server!");

    Message reply;

    if (MsgSend(server_coid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1) {
        perror("MsgSend failed");
        return -1;
    }

    printf("Received reply: %s\n", reply.text);
    return 0;
}


