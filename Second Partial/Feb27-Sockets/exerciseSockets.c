/*
    Simple program for a server using sockets
    Can only connect one client at a time
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

void usage(char * program);
void startServer(char * port);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    printf("=== SERVER PROGRAM ===\n");

    if (argc != 2)
        usage(argv[0]);

    startServer(argv[1]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Initialize the server to be ready for connections
void startServer(char * port)
{
    int server_fd;
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;

    // Fill in the hints structure with the necessary data
    // Use IPv4 addresses
    hints.ai_family = AF_INET;
    // Use a connection socket
    hints.ai_socktype = SOCK_STREAM;
    // Automatically fill the address of the server
    hints.ai_flags = AI_PASSIVE;

    // GETADDRINFO
    // Fill in the data about this computer
    if ( getaddrinfo(NULL, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // SOCKET
    // Create the file descriptor for the socket
    server_fd = socket(server_info.ai_family, server_info.ai_socktype, server_info.ai_protocol);
    if ( server_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    // BIND
    // Associate the socket to a port in the server
    if ( bind (server_fd, server_info.ai_sockaddr, server_info.ai_sockaddrlen) == -1 )
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    // FREEADDRINFO
    // Free the liked list of addrinfo
    freeaddrinfo(server_info);

    // LISTEN
    // Set the number of connections to receive
    if ( listen(server_fd, MAX_CONNECTIONS) == -1 )
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    printf("Server ready and listening ...\n");

    // Loop to accept connections from the clients
    waitForConnections(server_fd);

    // CLOSE
    // Close the file descriptor
    printf("Closing the listening socket\n");
    close(server_fd);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    int client_fd;
    struct sockaddr_in client_address;
    socklen_t client_address_length;

    // Prepare the client address
    client_address_length = sizeof client_address;
    bzero(client_address, client_address_length);
    // ACCEPT
    // Wait for a client to connect and open a file descriptor to it
    client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_length);
    if ( client_fd == -1 )
    {
        perror("ERROR: accept");
        exit(EXIT_FAILURE);
    }

    // Start talking with the client
    communicationLoop(client_fd);
}

// Talk between client and server
void communicationLoop(int client_fd)
{
    char buffer[BUFFER_SIZE];
    int message_number = 0;
    int bytes_read;

    while (1)
    {
        // Clear the buffer
        bzero(buffer, BUFFER_SIZE);
        // Receive a request from the client
        bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if ( bytes_read == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }
        else if ( bytes_read == 0 )
        {
            printf("Client has disconnected\n");
            break;
        }

        message_number++;
        printf("Message #%d from client: %s", message_number, buffer);

        // Prepare the message
        sprintf(buffer, "Reply to message #%d\n", message_number);
        if ( send(client_fd, buffer, strlen(buffer)+1, 0) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }
    }
}
