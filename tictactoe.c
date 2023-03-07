#include <stdio.h>
#include <stdlib.h>

// Check if there is a winner
int check_win(char *board)
{
    int i;
    // Check the vertical columns
    for (i = 0; i < 3; i++)
    {
        if (board[i] == board[i + 3] && board[i] == board[i + 6] && board[i] != ' ')
        {
            return 1;
        }
    }
    // Check tbe horizontal rows
    for (i = 0; i < 9; i += 3)
    {
        if (board[i] == board[i + 1] && board[i] == board[i + 2] && board[i] != ' ')
        {
            return 1;
        }
    }
    // Check the Diagonal /
    if (board[0] == board[4] && board[0] == board[8] && board[0] != ' ')
    {
        return 1;
    }
    // Check the Diagonal \ 
    if (board[2] == board[4] && board[2] == board[6] && board[2] != ' ')
    {
        return 1;
    }
    return 0;
}

void draw_board(char *board, int *turn)
{
    system("clear");
    printf("It is %c's turn\n", (char)*turn);
    int i;
    for (i = 0; i < 9; i++) 
    {
        printf("%c", board[i]);
        if (i % 3 == 2)
        {
            printf("\n");
        } 
        else 
        {
            printf(" | ");
        }
    }
}

void switch_turn(int *turn)
{
    if (*turn == 'x')
    {
        *turn = 'o';
    }
    else
    {
        *turn = 'x';
    }
}

void place_symbol(char *board, int position, int *turn)
{
    if (board[position] == ' ')
    {
        board[position] = (char)*turn;
        if (check_win(board))
        {
            return;
        }
        switch_turn(turn);
    }
}

int main()
{
    int *turn;
    *turn = 'x';
    system("clear");
    char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    draw_board(board, turn);
    int pos;
    do 
    {
        printf("Input position: ");
        scanf("%d", &pos);
        if (pos < 0 || pos > 8)
        {
            printf("Invalid position!\n");
            continue;
        }
        else
        {
            place_symbol(board, pos, turn);
            draw_board(board, turn);
        }
    } while (!check_win(board));
    printf("Game over!\n%c has won!\n", (char)*turn);
    return 0;
}