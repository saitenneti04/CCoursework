I've completed stages 1 - 4 of the coursework assignment. My program displays a 10 by 10 grid containing the following: a triangular robot (green) that starts at the home square (blue); markers (gray) which are against the walls of the grid; and blocks (black) which are around the grid but not against any wall. 

The robot generates the location of blocks and markers randomly, so the grid will look different each time the program is run. The robot uses an algorithm to keep travelling until it finds a marker (gray), recording all movements in an array. Once the robot finds a marker, the robot picks it up and then uses the array of moves made to retrace it's steps back home. This process repeats until all the markers in the grid are collected and brought home.

Commands to compile and run: 

Compile: gcc -o robot robot2.c graphics.c
Run1: ./robot | java -jar drawapp-3.0.jar

Compile: gcc -o robot robot2.c graphics.c
Run2: ./robot 3 7 east 4 2 | java -jar drawapp-3.0.jar

This version of compiling and running creates an executable called 'robot'. 
Run1 gives no extra command line arguments, so default values are used to set up the grid environment.
Run2 gives 5 extra command line arguments: robot starting x position, robot starting y position, robot starting direction, number of markers, and number of blocks. Note that the 4 numbers have to be between 1-9 (inclusive) to be accepted, and the direction has to be one of lowercase 'north', 'east', 'south', or 'west'.
Otherwise, default values will be used.







 