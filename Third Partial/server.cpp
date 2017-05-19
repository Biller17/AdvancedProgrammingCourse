/* Final project Andromeda server
 Adrian Biller
 Lucia Garza*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

void usage(char * program);
void startServer(char * port);
void waitForConnections(int server_fd);
void gameCommunication(int connection_fd);

int main(int argc, char * argv[])
{
    
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
    struct addrinfo hints;
    struct addrinfo * server_info;
    int server_fd;
    
    // Prepare the information to determine the local address
    // Clear all fields
    bzero(&hints, sizeof hints);
    // Use internet sockets with IPv4
    hints.ai_family = AF_INET;
    // Use connected sockets
    hints.ai_socktype = SOCK_STREAM;
    // Determine the ip of this machine automatically
    hints.ai_flags = AI_PASSIVE;
    
    ///// GETADDRINFO
    // Get the actual address of this computer
    if ( getaddrinfo(NULL, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }
    
    ///// SOCKET
    // Use the information obtained by getaddrinfo
    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if ( server_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    
    ///// BIND
    // Connect with the desired port
    if ( bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1 )
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }
    
    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);
    
    ///// LISTEN
    // Prepare for connections
    if ( listen(server_fd, MAX_QUEUE) == -1 )
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Server ready and waiting!\n");
    // Stand by to receive connections from the clients
    waitForConnections(server_fd);
    
    // Close the socket
    close(server_fd);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    pid_t pid;
    srand(time(NULL));
    
    // Loop to wait for client connections
    while (1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        // Receive the data from the client and open a new socket connection
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if ( connection_fd == -1 )
        {
            perror("ERROR: accept");
            exit(EXIT_FAILURE);
        }
        
        // Identify the client
        // Get the ip address from the structure filled by accept
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received connection from: %s : %d\n", client_presentation, client_address.sin_port);
        
        //Creating child that will attend the socket
        pid = fork();
        
        if(pid == 0){
            // Establish the communication
            printf("Creating process to attend connection\n");
            
            gameCommunication(connection_fd);
            
        }
        
    }
}


// Do the actual receiving and sending of data
void gameCommunication(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int chars_read;
    int hand[BUFFER_SIZE];
    int extraCard = 0;
    int totalPoints;
    char winner[BUFFER_SIZE];
    
    while (1)
    {
        // Clear the buffer
        bzero(buffer, BUFFER_SIZE);
        
        ///// RECV
        // Read the request from the client
        chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
        // Error when reading
        if ( chars_read == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }
        // Connection finished
        if ( chars_read == 0 )
        {
            printf("Client disconnected\n");
            break;
        }
        if(strcmp(buffer, "y") == 0){
            printf("The client wishes to end connection\n");
            break;
        }
        
        printf("Game will start, generating meteors \n");

        ///// SEND

        
        
        
        
        //template to send info
        /*
        if(write(connection_fd, hand, BUFFER_SIZE) == -1){
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }
        */
        
        
        //template to receive info
        /*
        //receive answer from client if they want or not another card
        chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
        // Error when reading
        if ( chars_read == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }*/


        
        
        //wipe buffer
        bzero(buffer, BUFFER_SIZE);
        
        
        
        
        
        
        
        
        
        
        
        
        /*if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
         {
         perror("ERROR: send");
         exit(EXIT_FAILURE);
         }*/
    }
}
