# TCP Socket Server Side

The server in the TCP protocol in Socket programming, that is, the control end of the Trojan.

## Usage

### 1) WSAStartup function

Initialize WinSock, specify the Socket library version used

```c++
int WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData );
```

Programs using Socket must call the WSAStartup function before using Socket. The first parameter of this function indicates the Socket version requested by the program. The high byte indicates the minor version and the low byte indicates the major version. The operating system uses the second parameter to return the requested socket version. When an application calls the WSAStartup function, the operating system searches for the corresponding Socket library according to the requested Socket version, and then binds the found Socket library to the application. Later, the application can call other Socket functions in the requested Socket library. The function returns 0 after successful execution.

Example: If a program uses Socket version 2.2, the program code is as follows

```c++
wVersionRequested = MAKEWORD( 2, 2 );

ret = WSAStartup( wVersionRequested, &wsaData );
```

### 2) Create a socket
```c++
SOCKET socket( int af, int type, int protocol );
```
The application calls the socket function to create a socket capable of network communication. The first parameter specifies the protocol family of the communication protocol used by the application. For the TCP/IP protocol family, this parameter is set to PF_INET; the second parameter specifies the type of socket to be created. The stream socket type is SOCK_STREAM, datagram The socket type is SOCK_DGRAM; the third parameter specifies the communication protocol used by the application. This function returns the descriptor of the newly created socket if the call succeeds, and returns INVALID_SOCKET if it fails.

The socket descriptor is an integer value. There is a socket descriptor table in the process space of each process, which stores the corresponding relationship between the socket descriptor and the socket data structure. There is a field in the table to store the descriptor of the newly created socket, and the other field to store the address of the socket data structure, so the corresponding socket data structure can be found according to the socket descriptor. Each process has a socket descriptor table in its own process space, but the socket data structure is in the kernel buffer of the operating system.

### 3) Bind the socket to a local address and port (bind)
```c++
int bind( SOCKET s, const struct sockaddr FAR *name, int namelen );
```
When a Socket is created, there is a default IP address and a default port number in the socket data structure. A service program must call the bind function to bind an IP address and a specific port number. The client program generally does not need to call the bind function to bind the IP address and port number for its Socket. The first parameter of this function specifies the Socket descriptor to be bound; the second parameter specifies a sockaddr structure, which is defined as follows:

```c++
struct sockaddr {

       u_short sa_family;

       char sa_data[14];
```

sa_family specifies the address family, for sockets of the TCP/IP protocol family, set AF_INET to it. When binding the sockets of the TCP/IP protocol family, we usually use another address structure:

```c++
struct sockaddr_in {

      short sin_family;

      u_short sin_port;

     struct in_addr sin_addr;

      char sin_zero[8];

      };
```

Among them, sin_family is set to AF_INET; sin_port indicates the port number; there is only one unique field s_addr in the sin_addr structure, which represents the IP address. This field is an integer. Generally, the function inet_addr() is used to convert the IP address in the form of a string to an unsigned long type.The integer value is then set to s_addr.

Some servers are multi-hosted machines and have at least two network cards. The service program running on such a server can set htonl(INADDR_ANY) to s_addr when binding its Socket IP address. The advantage of this is no matter which Client programs on the network segment can communicate with the service program;

If only a fixed IP address is bound to the Socket of the service program running on the multi-host computer, then only the client program on the same network segment as the IP address can communicate with the service program. We use 0 to fill the sin_zero array, the purpose is to make the size of the sockaddr_in structure consistent with the size of the sockaddr structure.

### 4) Set the socket to monitor mode, ready to accept client requests

send function

```c++
int listen( SOCKET s, int backlog );
```
The server program can call the listen function to make the stream socket s in the listening state. The stream socket s in the listening state will maintain a client connection request queue, which can hold up to backlog client connection requests. If the function is executed successfully, it returns 0; if the execution fails, it returns SOCKET_ERROR.

### 5) Accept Request

Waiting for the client request to arrive; when the request comes, accept the connection request and return a new socket corresponding to this connection
```c++
SOCKET accept(

       SOCKET s,

       struct sockaddr FAR *addr,

       int FAR *addrlen

       );
```
The service program calls the accept function to take out the top client request from the client connection request queue of the stream socket s in the listening state, and creates a new socket to create a connection channel with the client socket.

If the connection is successful, the descriptor of the newly created socket will be returned, and the newly created socket will exchange data with the client socket later; if it fails, INVALID_SOCKET will be returned.

The first parameter of this function specifies the stream socket in the listening state; the operating system uses the second parameter to return the address structure of the newly created socket; the operating system uses the third parameter to return the length of address structure of the newly created socket.


### 6) Use the returned socket to communicate with the client (send/recv)

### 7) Return and wait for another customer request

### 8) Close the socket and release resources

If there are two persons, A and B, A (client) is going to visit B (server). A is relatively simple. You only need to know B's street (IP address) and house number (port) to find B (connect), communicate with B (send/recv), and then the visit ends (Close).

And B (server side) is more complicated. B’s house first occupies the street (IP address) and house number (port), namely bind. Then wait at home for A's arrival (Listen). When A comes and knocks on the door to send a visit request, open the door to receive A (Accept), invite A to take a room in the house (Accept), and communicate with A (Send/recv ), Finally, after A finishes the visit and leaves, he closes the room. (Close).

There are many rooms in B's house (new sockets after Accept), which can wait for other people like A (such as C, D, E and other clients) to visit.