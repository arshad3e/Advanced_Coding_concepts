/*How QNX Message Passing Works
QNX follows a client-server model where:

A server creates a communication channel (ChannelCreate()).
A client connects to the server using ConnectAttach().
The client sends a request using MsgSend(), which blocks until a response is received.
The server receives the message using MsgReceive(), processes it, and replies using MsgReply().
The client receives the reply and resumes execution.
🔹 QNX IPC Example: Client-Server Message Passing
Let's implement a server that listens for messages and replies, and a client that sends a message to the server.*/


