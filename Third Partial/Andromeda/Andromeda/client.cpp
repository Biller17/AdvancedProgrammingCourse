
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <ctime>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "bullet.h"
#include <sstream>



#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024


void usage(char * program);
void* connectToServer(void* params);
void communicationLoop(int connection_fd);
int getScore();


//global variables
float spaceshipPosY = 600;
float spaceshipPosX = 450;
float asteroids [10] = {.3,.5,.6,.7,.2,1.2,1.4,.3,1.1,.9};
float asteroidPositions[10] = {10,0,5,0,100,0,200,0,980,0};
float asteroidYpositions[10] ={150, 250, 300, 350,450, 320, 450, 410, 430, 170};
//int hit = 20;
float spaceshipVel = 2;
float bulletVelx = 0;
float bulletVely = 1;
float bulletX = 460;
float bulletY = 450;
int score = 0;
int enemyScore = 0;
float outOfBounds = 0;
int num_player = 0;
int gameover_display = 0;
int winner;
int initiateGame;


struct serverParams{
    char address[20];
    char port[20];
};

void checkColl(Bullet bullet) {
    if (bullet.getRight() >  spaceshipPosX +10
        && bullet.getTop() < spaceshipPosY
        && bullet.getBottom() > spaceshipPosY +5
        && bullet.getLeft() < spaceshipPosX +160) {
        bulletVely*=-1.005;
        bulletVelx = -0.5+1*((float)rand())/RAND_MAX;
    }
}


void checkAsteroidCollision(Bullet bullet){
    for(int i = 0; i < 10; i++){
        if (bullet.getRight() > asteroidPositions[i]
            && bullet.getTop() < asteroidYpositions[i] +50
            && bullet.getBottom() > asteroidYpositions[i]
            && bullet.getLeft() < asteroidPositions[i] +50
            && asteroids[i] != 0) {
            asteroids[i] = 0;
            score ++;
        }
    }
}


void resetGame(){
    spaceshipPosX = 450;
    bulletVely = 1;
    bulletVelx = 0;
    //sleep(5);
    
}


//checking

int checkBoundaries(Bullet bullet) {
    if (bullet.getRight() > 1100) {
        bulletVelx = (rand()/(RAND_MAX + 1)*(1.6 - 1)+1)*-1;
        return 0;
        
    }
    else if(bullet.getLeft() < 100) {
        bulletVelx = (rand()/(RAND_MAX + 1)*(1.6 - 1)+1);
        return 0;
        
    }
    else if (bullet.getTop() < -2) {
        bulletVely*=-1.005;
        bulletVelx = -0.5+1*((float)rand())/RAND_MAX;
        return 0;
    }
    else if (bullet.getBottom() > 900) {
        resetGame();
        return 1;
    }
}

int main(int argc, char * argv[])
{
    
    pthread_t tid;
    struct serverParams params;
    
    //setting parameters to run in server
    strcpy(params.address, "10.49.65.93");
    strcpy(params.port, "8080");
    sf::Clock gametime;
    sf::Time t1 = sf::seconds(0.1f);
    srand(time(NULL));
    score = 0;
    //creating thread for communication
    int status = pthread_create(&tid, NULL, &connectToServer, (void*)&params);
    //creating ball
    std::vector<Bullet> bulletVec;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Andromeda");
    
    
    //creating spaceship texture
    sf::Texture spaceship;
    if (!spaceship.loadFromFile(resourcePath() + "spaceship.png")) {
        return EXIT_FAILURE;
    }
    
    //Player
    sf::Sprite spaceshipSprite(spaceship);
    spaceshipSprite.setScale(.5, .5);
    spaceshipSprite.setPosition(spaceshipPosX, spaceshipPosY);
    
    
    //Asteroids
    sf::Texture asteroid;
    if (!asteroid.loadFromFile(resourcePath() + "asteroid.png")) {
        return EXIT_FAILURE;
    }
    
    std::vector<sf::Sprite> asteroidSprites(10, sf::Sprite(asteroid));
    
    for(int i = 0; i< 10; i++){
        asteroidSprites[i].setPosition(asteroidPositions[i], asteroidYpositions[i]);
        asteroidSprites[i].setScale(.2, .2);
    }
    
    
    
    
    
    //setting explosion sprite when asteroids are hit
    sf::Texture explosion;
    if (!explosion.loadFromFile(resourcePath() + "explosion.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite explosionSprite(explosion);
    explosionSprite.setScale(.2, .2);
    
    
    
    
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(resourcePath() + "space.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Sprite sprite(texture);
    
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    
    
    //set user score
    
    std::stringstream stream;
    stream << "Score: "<< score;
    sf::Text userScore(stream.str(), font, 50);
    userScore.setPosition(0, 700);
    userScore.setFillColor(sf::Color::White);
    
    
    //set game over
    
    std::stringstream ss;
    ss << "GAME OVER . PLAYER: "<< winner <<" WINS";
    sf::Text gameover(stream.str(), font, 50);
    gameover.setPosition(250, 250);
    gameover.setFillColor(sf::Color::White);
    
    //set num of player
    
    std::stringstream player;
    player << "PLAYER : "<< num_player;
    sf::Text displayplayer(stream.str(), font, 50);
    displayplayer.setPosition(0, 10);
    displayplayer.setFillColor(sf::Color::White);
    
    //set menu
    
    sf::Text menu("WELCOME TO CYGNUS-5 EXPANSE", font, 70);
    menu.setPosition(25, 250);
    menu.setFillColor(sf::Color::White);
    
    //set instructions
    
    sf::Text instructions("KILL THE CROMULONS BEFORE YOUR OPONENT", font, 40);
    instructions.setPosition(150, 350);
    instructions.setFillColor(sf::Color::White);
    
    //set waiting message
    
    sf::Text wait("waiting for your oponent to start...", font, 30);
    wait.setPosition(380, 450);
    wait.setFillColor(sf::Color::White);
    
    
    
    
    //respawn text when player looses a ball
    sf::Text respawn("recharging", font, 30);
    respawn.setPosition(450, 400);
    respawn.setFillColor(sf::Color::White);
    
    
    
    sf::Text waitingforPlayers("Waiting for other player", font, 40);
    waitingforPlayers.setPosition(450, 400);
    waitingforPlayers.setFillColor(sf::Color::White);
    
    
    //creating bullet object for ball
    Bullet newBullet(15);
    
    // Start the game loop
    while (window.isOpen())
    {
        //t1 = gametime.getElapsedTime();
        window.pushGLStates();
        
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        
        if(initiateGame == 0)
        {
            window.draw(menu);
            window.draw(instructions);
            window.draw(wait);
        }
        
        else if(initiateGame == 1)
        {
            if(gameover_display == 0)
            {
                //window.draw(enemySpaceshipSprite);
                window.draw(spaceshipSprite);
                //drawing asteroids
                for(int i = 0; i< 10; i++){
                    window.draw(asteroidSprites[i]);
                }
                
                //moving asteroids
                for(int i = 0; i<10 ; i++){
                    asteroidSprites[i].move(asteroids[i], 0);
                    asteroidPositions[i] += asteroids[i];
                    if(asteroids[i] == 0){
                        explosionSprite.setPosition(asteroidPositions[i], asteroidYpositions[i]);
                        window.draw(explosionSprite);
                        asteroidSprites[i].setPosition(4234432, 4234432);
                    }
                    
                    //checking positions of asteroids so they dont get out of bounds
                    
                    if(asteroidPositions[i] > 1100 || asteroidPositions[i] < 0){
                        asteroids[i]*=-1;
                    }
                }
                
                //checking if spaceship moves left or right
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                    spaceshipPosX+=spaceshipVel;
                    spaceshipSprite.setPosition(spaceshipPosX, spaceshipPosY);
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                    spaceshipPosX-=spaceshipVel;
                    spaceshipSprite.setPosition(spaceshipPosX, spaceshipPosY);
                    
                }
                
                
                //moving ball
                for(int i = 0; i < 10; i++)
                {
                    newBullet.setPos(bulletX, bulletY);
                    bulletVec.push_back(newBullet);
                    bulletVec[i].draw(window);
                    bulletVec[i].fire(bulletVelx, bulletVely);
                    //checking ball collisions
                    checkColl(bulletVec[i]);
                    
                    if(checkBoundaries(bulletVec[i]) == 1){
                        bulletVec.clear();
                        bulletVec[i].setPos(450, 300);
                        window.draw(respawn);
                        //sleep(3);
                    }
                    //check if ball collisions with any asteroid
                    checkAsteroidCollision(bulletVec[i]);
                    
                }
                
                
                //if spaceship moves out of bounds change them to make move continuous
                if(spaceshipPosX > 1100){
                    spaceshipSprite.setPosition(0, 600);
                    spaceshipPosX = 0;
                }
                if(spaceshipPosX < -100){
                    spaceshipSprite.setPosition(1100, 600);
                    spaceshipPosX = 1100;
                }
                
            }
            
            
            //if any of the two players wins it enters here and ends game
            if(gameover_display == 1){
                
                ss << "GAME OVER PLAYER: " << winner <<" WINS";
                gameover.setString(ss.str());
                ss.str(std::string());
                
                window.draw(gameover);
            }
            
            //drawing player name
            player << "PLAYER: " << num_player;
            displayplayer.setString(player.str());
            player.str(std::string());
            
            window.draw(displayplayer);
            
            
            
            //setting scores in text
            stream << "Score: "<< score;
            
            userScore.setString(stream.str());
            stream.str(std::string());
            // Draw the string
            window.draw(userScore);
        }
        
        
        window.popGLStates();
        // Update the window
        window.display();
    }
    
    return EXIT_SUCCESS;
}







// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}







// Establish a connection with the server indicated by the parameters
void* connectToServer(void* params)
{
    struct serverParams *sp = (struct serverParams*)params;
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
    
    
    if ( getaddrinfo(sp->address, sp->port, &hints, &server_info) == -1 )
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
    communicationLoop(connection_fd);
    
    // Close the socket
    close(connection_fd);
}

int getScore(){
    return score;
}



void communicationLoop(int connection_fd)
{
    int gameover;
    
    char buffer[BUFFER_SIZE];
    while (1)
    {
        read(connection_fd, &initiateGame, sizeof(int));
        read(connection_fd, &num_player, sizeof(int));
        write(connection_fd, &score, sizeof(int));
        
        
        read(connection_fd, &gameover, sizeof(int));
        read(connection_fd, &winner, sizeof(int));
        
        if(gameover == 1)
        {
            gameover_display = 1;
        }
    }
}

