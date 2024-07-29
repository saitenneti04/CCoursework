#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphics.h"

#define GRID_SIZE 10
#define MAX_MOVES 100
#define WINDOW_SIZE 600
#define RECT_SIZE 50
#define SLEEP_TIME_MS 1000
#define FORWARD 8
#define RIGHT 9
#define FREE_SPACE 0
#define HOME 1
#define MARKER 2
#define BLOCK 3
//default values if command-line arguments are invalid
#define INITIAL_MARKERS 4
#define INITIAL_BLOCKS 3
#define INITIAL_XPOS 6 
#define INITIAL_YPOS 5 

typedef struct robot{
    int x;
    int y;
    char *direction;
}robot;

// represents the environment where the robot operates
typedef struct environment{
    int markers;
    int blocks;
    int grid[GRID_SIZE][GRID_SIZE];
}environment;

// global variables (avoids passing them around between over 20 functions)
int *moves;
int movesPos;
robot Robot;
environment Environment;

void allocateMovesMemory(); 
void drawBackground();
void generateMarker();
void drawEnvironment();
void initialiseGrid();
void drawTriangle();
void drawEast();
void drawWest();
void drawSouth();
void drawNorth();
void forward();
void redrawTriangle();
void left();
void right();
bool atMarker();
int canMoveForward();
void start();
void robotAlgorithm();
void returnHome();
void pickUpMarker();
bool againstWall(int x, int y);
void generateBlocks();
void createSetUp(int argc, char **argv);
void generateItems(int item, int quantity);

int main(int argc, char **argv){
    setWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    createSetUp(argc, argv);
    allocateMovesMemory();
    srand(time(NULL));
    start(); // begins simulation
    return 1;
}

// initialies robot and environment based on command-line arguments
void createSetUp(int argc, char **argv){
    Robot.x = (argc == 6 && atoi(argv[1]) < 10 && atoi(argv[1]) > 0) ? atoi(argv[1]) : INITIAL_XPOS;
    Robot.y = (argc == 6 && atoi(argv[2]) < 10 && atoi(argv[2]) > 0) ? atoi(argv[2]) : INITIAL_YPOS;
    Robot.direction = (argc == 6) ? argv[3] : "south";
    Environment.markers = (argc == 6 && atoi(argv[4]) < 10 && atoi(argv[4]) > 0) ? atoi(argv[4]) : INITIAL_MARKERS;
    Environment.blocks = (argc == 6 && atoi(argv[5]) < 10 && atoi(argv[5]) > 0) ? atoi(argv[5]) : INITIAL_BLOCKS;
}

// allocates memory to record robot moves
void allocateMovesMemory(){
    moves = malloc(MAX_MOVES * sizeof(int));

    if (moves == NULL){
        printf("Memory allocation failed.\n");
        exit(-1);
    }   
}

void start(){
    initialiseGrid();
    drawBackground();
    drawTriangle();
    generateMarker();
    generateBlocks();
    drawEnvironment();

    // main loop to execute bringing every marker home
    for (int i = 0; i < Environment.markers; i++){
        robotAlgorithm();
    }
    free(moves);
}

// each square in the grid is initialised as free space
void initialiseGrid(){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            Environment.grid[i][j] = FREE_SPACE;
        }
    }
}

void drawBackground(){
    background();
    // displays environment grid
    for (int x = 50; x < 501; x += RECT_SIZE){
        for (int y = 50; y < 501; y += RECT_SIZE){
            drawRect(x, y, RECT_SIZE, RECT_SIZE);
        }
    }
    // draws the home and set its position in the grid
    setColour(blue);
    fillRect(50 + 50 * Robot.x, 500 - 50 * Robot.y, RECT_SIZE, RECT_SIZE);
    Environment.grid[Robot.x][Robot.y] = HOME; 
}

// checks robot's direction before drawing the corresponding triangle
void drawTriangle(){
    foreground();
    setColour(green);

    if (strcmp(Robot.direction, "east") == 0){
        drawEast();
    }
    else if (strcmp(Robot.direction, "west") == 0){
        drawWest();
    }
    else if (strcmp(Robot.direction, "north") == 0){
        drawNorth();
    }
    else if (strcmp(Robot.direction, "south") == 0){
        drawSouth();
    }
}

// draws triangle facing east
void drawEast(){
    int x[] = {50 + 50 * Robot.x, 50 + 50 * Robot.x, 100 + 50 * Robot.x};
    int y[] = {550 - 50 * Robot.y, 500 - 50 * Robot.y, 525 - 50 * Robot.y};
    fillPolygon(3, x, y);
}

void drawWest(){
    int x[] = {100 + 50 * Robot.x, 100 + 50 * Robot.x, 50 + 50 * Robot.x};
    int y[] = {550 - 50 * Robot.y, 500 - 50 * Robot.y, 525 - 50 * Robot.y};
    fillPolygon(3, x, y);
}

void drawSouth(){
    int x[] = {50 + 50 * Robot.x, 100 + 50 * Robot.x, 75 + 50 * Robot.x};
    int y[] = {500 - 50 * Robot.y, 500 - 50 * Robot.y, 550 - 50 * Robot.y};
    fillPolygon(3, x, y);
}

void drawNorth(){
    int x[] = {50 + 50 * Robot.x, 100 + 50 * Robot.x, 75 + 50 * Robot.x};
    int y[] = {550 - 50 * Robot.y, 550 - 50 * Robot.y, 500 - 50 * Robot.y};
    fillPolygon(3, x, y);
}

void generateMarker(){
    generateItems(MARKER, Environment.markers);
}

void generateBlocks(){
    generateItems(BLOCK, Environment.blocks);
}

// sets up the positions of markers and blocks in the grid
void generateItems(int item, int quantity){
    int x, y;
    int counter = 0;
    while (counter < quantity){
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
        // checks position is free and suitable for the item type
        // markers must be next to a wall whilst blocks must not
        if ((item == MARKER && againstWall(x, y)) || (item == BLOCK && !againstWall(x, y))){
            if (Environment.grid[x][y] == FREE_SPACE){
                Environment.grid[x][y] = item;
                counter++;
            }
        }
    }
}

// displays gray markers and black blocks in the grid
void drawEnvironment(){
    for (int x = 0; x < GRID_SIZE; x++){
        for (int y = 0; y < GRID_SIZE; y++){
            if (Environment.grid[x][y] == 2){
                background();
                setColour(gray);
                fillRect(50 + 50 * x, 500 - 50 * y, RECT_SIZE, RECT_SIZE);
            }
            if (Environment.grid[x][y] == 3){
                background();
                setColour(black);
                fillRect(50 + 50 * x, 500 - 50 * y, RECT_SIZE, RECT_SIZE);
            }
        }
    }   
}

void robotAlgorithm(){
    movesPos = 0; // resets position in the moves tracking array
    while (true){
        while (!atMarker()){
            while (canMoveForward() && !atMarker()){
                forward();
                moves[movesPos++] = FORWARD; // records the move
            }
            if (atMarker()){
                break; 
            }
            right(); // the robot turns right if it can't move forward
            moves[movesPos++] = RIGHT; 
        }
        pickUpMarker(); 
        returnHome(); 
        break; // loop ends when the robot returns home with one marker
    }
}

bool atMarker(){
    if (Environment.grid[Robot.x][Robot.y] == MARKER){
        return true;
    }
    return false;
}

// checks if the robot would run into a block or wall based on its current direction and position
int canMoveForward(){
    if (((Robot.x == 0) || (Environment.grid[Robot.x - 1][Robot.y] == BLOCK)) && (strcmp(Robot.direction, "west") == 0)){
        return false;
    }
    if (((Robot.x == GRID_SIZE - 1) || (Environment.grid[Robot.x + 1][Robot.y] == BLOCK)) && (strcmp(Robot.direction, "east") == 0)){
        return false;
    }
    if (((Robot.y == 0) || (Environment.grid[Robot.x][Robot.y - 1] == BLOCK)) && (strcmp(Robot.direction, "south") == 0)){
        return false;
    }
    if (((Robot.y == GRID_SIZE - 1) || (Environment.grid[Robot.x][Robot.y + 1] == BLOCK)) && (strcmp(Robot.direction, "north") == 0)){
        return false;
    }
    return true;
}

// updates the robot's position based on its direction
void forward(){
    if (strcmp(Robot.direction, "east") == 0){
        Robot.x += 1;
    }
    else if (strcmp(Robot.direction, "west") == 0){
        Robot.x -= 1;
    }
    else if (strcmp(Robot.direction, "north") == 0){
        Robot.y += 1;
    }
    else if (strcmp(Robot.direction, "south") == 0){
        Robot.y -= 1;
    }
    redrawTriangle(); 
}

void right() {
    if (strcmp(Robot.direction, "east") == 0){
        Robot.direction = "south";
    }
    else if (strcmp(Robot.direction, "west") == 0){
        Robot.direction = "north";
    }
    else if (strcmp(Robot.direction, "north") == 0){
        Robot.direction = "east";
    }
    else if (strcmp(Robot.direction, "south") == 0){
        Robot.direction = "west";
    }
    redrawTriangle();
}

void left(){
    if (strcmp(Robot.direction, "east") == 0){
        Robot.direction = "north";
    }
    else if (strcmp(Robot.direction, "west") == 0){
        Robot.direction = "south";
    }
    else if (strcmp(Robot.direction, "north") == 0){
        Robot.direction = "west";
    }
    else if (strcmp(Robot.direction, "south") == 0){
        Robot.direction = "east";
    }
    redrawTriangle();
}

// updates the robot's visual representation
void redrawTriangle(){
    sleep(SLEEP_TIME_MS / 10);
    foreground();
    clear();
    drawTriangle();
}

// remove the marker from the environment grid and visually erase it from the display
void pickUpMarker(){
    sleep(SLEEP_TIME_MS);
    background();
    setColour(white);
    Environment.grid[Robot.x][Robot.y] = 0; 
    fillRect(51 + 50 * Robot.x, 501 - 50 * Robot.y, RECT_SIZE - 1, RECT_SIZE - 1);
}

void returnHome(){
    right();
    sleep(SLEEP_TIME_MS / 4);
    right();
    // the robot retraces its steps back home from the marker
    for (int i = movesPos - 1; i > -1; i--){
        if (moves[i] == FORWARD){
            forward();
        }
        else{
            left();
        }
    }
    sleep(SLEEP_TIME_MS);
}

bool againstWall(int x, int y){
    if (x == 0 || x == 9 || y == 0 || y == 9){
        return true;
    }
    return false;
}