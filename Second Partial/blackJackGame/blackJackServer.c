/*homework program Adrian Biller A01018940

Advanced programming course

BlackJack server*/

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
int randomCard();
int checkCard(int card);
int countCards(int one, int two, int three);
int checkWinner(int client, int dealer);
void gameCommunication(int connection_fd, int dealerCards);

int main(int argc, char * argv[])
{
    printf("\n********* Black Jack Online server program ************\n");

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

          gameCommunication(connection_fd, countCards(randomCard(), randomCard(), 0));

        }

    }
}

//generates random card from 1 to 13
int randomCard(){
  return (rand() % 13) + 1;
}


//checks the value of the card considering jack, queen and king as 10
int checkCard(int card){
  if(card > 9){
    return 10;
  }
  return card;
}

//counts the cards dealt including counting the ace as either 1 or 11
int countCards(int one, int two, int three){
  int total;
  int temp;
  one = checkCard(one);
  two = checkCard(two);
  three = checkCard(three);

  if(one == 1 || two == 1 || three == 1){
    if(one == 1){
      temp = two + three + 11;
      if(temp > 21)
        total += 1;
      else
        total += 11;
    }
    if(two == 1){
      temp = total + three + 11;
      if(temp > 21)
        total += 1;
      else
        total += 11;
    }
    if(three == 1){
      temp = total + 11;
      if(temp > 21)
        total += 1;
      else
        total += 11;
    }
  }
  else{
    total = one+two+three;
  }

  return total;
}

//checks if either client or dealer won
int checkWinner(int client, int dealer){

  if(dealer > 21 && client > 21){
    return 2;
  }
  if(dealer > 21){
    return 1;
  }
  if(client > 21){
    return 0;
  }
  if(client > dealer){
    return 1;
  }
  else {
    return 0;
  }
  return 2;
}

// Do the actual receiving and sending of data
void gameCommunication(int connection_fd, int dealerCards)
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

        printf("Game will start, generating random cards\n");
        //get two first random cards
        hand[0] = randomCard();
        hand[1] = randomCard();


        ///// SEND
        // Send a reply to the client
        if(write(connection_fd, hand, BUFFER_SIZE) == -1){
          perror("ERROR: send");
          exit(EXIT_FAILURE);
        }

        printf("Sent hand to client\n");

        //receive answer from client if they want or not another card
        chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
        // Error when reading
        if ( chars_read == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }

        //dealing extra card
        if(strlen(buffer) > 1){
          //generating new card
          extraCard = randomCard();
          write(connection_fd, &extraCard, BUFFER_SIZE);
        }


        //get total points of client
        totalPoints = countCards(hand[0], hand[1], extraCard);

        //sending total points
        printf("Counted total card points for client: %d \n", totalPoints);
        if( write(connection_fd, &totalPoints, 4) == -1){
          perror("ERROR: send");
          exit(EXIT_FAILURE);
        }
        bzero(buffer, BUFFER_SIZE);


        //checking winner
        printf("Checking winner\n Dealer: %d\nClient: %d\n", dealerCards, totalPoints);
        switch (checkWinner(totalPoints, dealerCards)) {
          case 0:
            sprintf(buffer, "Sorry the dealer won, try again");
            break;
          case 1:
            sprintf(buffer, "Congratulations, you won!");
            break;
          case 2:
            sprintf(buffer, "It's a draw");
            break;
          default:
            perror("ERROR: count");
            exit(EXIT_FAILURE);
        }
        //sending winner info
        if ( write(connection_fd, &buffer, BUFFER_SIZE) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }













        /*if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }*/
    }
}
