#define _DEFAULT_SOURCE

#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void die(char *s)
{
    if (errno)
        perror(s);
    else
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

typedef struct {
    char board[9];
    char turn;
    pthread_mutex_t mutex;
    pthread_cond_t user_cond;
    pthread_cond_t computer_cond;
} game;

typedef struct {
    char id;
    game *game_items;
} thread_arg_t;

void add_random(char id)
{
    usleep(1000 + 1000 * (id * 1000) % 5);
}


// Check if there is a winner
int check_win(char *board)
{
    int i;
    // Check the vertical columns
    for (i = 0; i < 3; i++)
    {
        if (board[i] == board[i + 3] && board[i] == board[i + 6] && board[i] != ' ')
        {
            printf("a");
            return 1;
        }
    }
    // Check tbe horizontal rows
    for (i = 0; i < 9; i += 3)
    {
        if (board[i] == board[i + 1] && board[i] == board[i + 2] && board[i] != ' ')
        {
            printf("b");
            return 1;
        }
    }
    // Check the Diagonal /
    if (board[0] == board[4] && board[0] == board[8] && board[0] != ' ')
    {
        printf("c");
        return 1;
    }
    // Check the Diagonal \ 
    if (*board[2] == *board[4] && *board[2] == *board[6] && *board[2] != ' ')
    {
        printf("d");
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
            switch_turn(turn);
        }
        return;
    }
}

// TODO
void* user(void *thread_arg)
{
    thread_arg_t *arg = (thread_arg_t *)thread_arg;
    int id = arg->id;
    game *game_items = arg->game_items;

}

void* computer(void *thread_arg)
{
    thread_arg_t *arg = (thread_arg_t *)thread_arg;
    int id = arg->id;
    game *game_items = arg->game_items;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2)
    {
        printf("Too many or few arguemtns provided.\n");
        printf("Correct call should be:\n");
        printf("./tic-tac-toe {param}\n");
        printf("{d} = default | {t} = threaded | {s} = socketed\n");
        exit(1);
    }
    if (*argv[1] != 'd' && *argv[1] != 't' && *argv[1] != 's')
    {
        printf("Error parsing parameter!\n");
        printf("Correct call should be:\n");
        printf("./tic-tac-toe {param}\n");
        printf("{d} = default | {t} = threaded | {s} = socketed\n");
        exit(1);
    }

    if (*argv[1] == 'd')
    {
        int *turn;
        char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
        *turn = 'x';
        system("clear");
        draw_board(board, turn);
        int pos;
        do 
        {
            printf("Input position: ");
            scanf("%d", &pos);
            printf("%d\n",pos);
            place_symbol(board, pos, turn);
            draw_board(board, turn);
        } while (1);
        printf("Game over!\n%c has won!\n", (char)*turn);
        return 0;
    }
}