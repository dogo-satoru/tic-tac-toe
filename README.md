# Tic-Tac-Toe Game in C

## Local -- Play with two people on the same machine

* So far the base premis of the game works
* After running the program, the game will prompt a user for x's turn
  * The game asks for a position as imput
* It will then perform some checks on whether or not it was a valid placement
* If so switch turns and repeat until a winner is decided!

## VS CPU -- Play Against a CPU

### Currently Developing...

* Give one thread to the computer to determine their moves
* The other thread will be used for the user to interact with
* Uses same functions as the Basic Implementation, however will have wait conditions until one player mkaes a move.

## GUI Interface

### Planned implementation

* Make a GUI interface using GTK/GTK+ library
  * Possibly something else however seems as though this is the best/ most widely used.

## Socket Implementation

### Planned implementation

* Work with sockets to provide a multiplayer experience
* Will have the host be one player and the client as another.
