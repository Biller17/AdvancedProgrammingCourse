/*homework program Adrian Biller A01018940

Advanced programming course

BlackJack client*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void connectToServer(char * address, char * port);
void cardGame(int connection_fd);

int main(int argc, char * argv[])
{
    printf("\n=== Black Jack Game ===\n");

    if (argc != 3)
        usage(argv[0]);

    connectToServer(argv[1], argv[2]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int connection_fd;

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
    if ( getaddrinfo(address, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if ( connection_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    ///// CONNECT
    // Connect with the desired port
    if ( connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1 )
    {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    // Establish the communication
    cardGame(connection_fd);

    // Close the socket
    close(connection_fd);
}


/*char* translate(int card){
  char translation[BUFFER_SIZE];
  switch (card) {
    case 1:
      return "Ace";
    case 11:
      return "Jack";
    case 12:
      return "Queen";
    case 13:
      return "King";
  }
  return sprintf(translation, "%d", card);
}*/
// Do the actual receiving and sending of data
void cardGame(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int hand[BUFFER_SIZE];
    int extraCard = 0;
    int totalPoints;

    while (1)
    {
        printf("Welcome to Black Jack Online \n do you wish to play? (yes = y/no = enter)");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Finish the connection with a string containing only the '\n'
        if (strlen(buffer) == 1)
        {
            printf("Finishing the connection\n");
            break;
        }

        ///// SEND
        // Send a reply to the client
        if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }

        // Clear the buffer
        bzero(buffer, BUFFER_SIZE);

        ///// RECV
        // Read the request from the client
        if(read(connection_fd,hand, BUFFER_SIZE) == -1 ){
          perror("ERROR: recv");
          exit(EXIT_FAILURE);
        }
        printf("These are your cards\nCard 1: %d \nCard 2: %d\n", hand[0], hand[1]);


        //asking to the dealer for another card
        printf("Do you wish to get another card? (yes = y / no = enter)\n");
        fgets(buffer, BUFFER_SIZE, stdin);
        if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 ){
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }


        //receiving extra card
        if(strlen(buffer) > 1){
          if(read(connection_fd, &extraCard, BUFFER_SIZE) == -1){
            perror("ERROR: read");
            exit(EXIT_FAILURE);
          }
          printf("Dealer has dealt you a %d\n", extraCard);
        }

        //receiving total points
        if(read(connection_fd, &totalPoints, 4) == -1 ){
          perror("ERROR: read");
          exit(EXIT_FAILURE);
        }
        printf("Your total points are %d\n", totalPoints);


        //Receiving who's the winner
        if(read(connection_fd, &buffer, BUFFER_SIZE) == -1)
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }

        printf("%s\n",buffer);
        printf("Do you wish to play again? (y = yes/ enter = no)");


        fgets(buffer, BUFFER_SIZE, stdin);

        // Finish the connection with a string containing only the '\n'
        if (strlen(buffer) == 1)
        {
            printf("Finishing the connection\n");
            break;
        }



    }
}
