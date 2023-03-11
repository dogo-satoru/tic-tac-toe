<style>
r {color: Red}
o {color: Orange}
g {color: Green}
b {color: Blue}
lg {color: lightgreen}
</style>

# Tic-Tac-Toe Game in C

## <lg>Basic Implementation</lg>

* So far the base premis of the game works
* After running the program, the game will prompt a user for x's turn
  * The game asks for a position as imput
* It will then perform some checks on whether or not it was a valid placement
* If so switch turns and repeat until a winner is decided!

## <o>Multi-threading</o>

### Currently Developing...

* Give one thread to the computer to determine their moves
* The other thread will be used for the user to interact with
* Uses same functions as the Basic Implementation, however will have wait conditions until one player mkaes a move.

## <r>GUI Interface</r>

### Planned implementation

* Make a GUI interface using GTK/GTK+ library
  * Possibly something else however seems as though this is the best/ most widely used.

## <r>Socket Implementation</r>

### Planned implementation

* Work with sockets to provide a multiplayer experience
* Will have the host be one player and the client as another.
