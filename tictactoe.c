#define _DEFAULT_SOURCE

#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int turn;
    int finished;
    pthread_mutex_t mutex;
    pthread_cond_t turn_cond;
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
        if (board[i] == board[i + 3] && board[i] == board[i + 6] && board[i] != '_')
        {
            return 1;
        }
    }
    // Check tbe horizontal rows
    for (i = 0; i < 9; i += 3)
    {
        if (board[i] == board[i + 1] && board[i] == board[i + 2] && board[i] != '_')
        {
            return 1;
        }
    }
    // Check the Diagonal /
    if (board[0] == board[4] && board[0] == board[8] && board[0] != '_')
    {
        return 1;
    }
    if (board[2] == board[4] && board[2] == board[6] && board[2] != '_')
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

int place_symbol(char *board, int position, int *turn)
{
    if (board[position] == '_')
    {
        // Places an 'x' or 'y' depending on which thread it was called by.
        board[position] = (char)*turn;
        if (check_win(board))
        {
            return 1;
        }
        switch_turn(turn);
    }
    return 0;
}

void* user(void *thread_arg)
{
    thread_arg_t *arg = (thread_arg_t *)thread_arg;
    int id = arg->id;
    game *game_items = arg->game_items;

    do {
        pthread_mutex_lock(&game_items->mutex);
        while ((char)game_items->turn != id && game_items->finished == 0)
        {
            pthread_cond_wait(&game_items->turn_cond, &game_items->mutex);
        }
        // Receive input from the user
        if (game_items->finished == 1)
        {
            break;
        }
        // Draw the board after the thread gets access to it.
        draw_board(game_items->board, &game_items->turn);

        // Initialize and prompt user for a position to place a new symbol
        int pos;
        printf("Input position: ");
        scanf("%d", &pos);

        // Assign finished to the return value of place_symbol, it will return 1 if the game has been won, otherwise 0
        // On top of that it will assign the position provided to the current threads symbol.
        game_items->finished = place_symbol(game_items->board, pos, &game_items->turn);

        // Signal and unlock the other thread as it is their turn to move.
        pthread_cond_signal(&game_items->turn_cond);
        pthread_mutex_unlock(&game_items->mutex);
   } while(game_items->finished != 1);
    pthread_exit(NULL);
}

void* computer(void *thread_arg)
{
    thread_arg_t *arg = (thread_arg_t *)thread_arg;
    int id = arg->id;
    game *game_items = arg->game_items;

    do {
        pthread_mutex_lock(&game_items->mutex);
        while ((char)game_items->turn != id && game_items->finished == 0)
        {
            pthread_cond_wait(&game_items->turn_cond, &game_items->mutex);
        }
        // This is for the check for the thread that loses
        if (game_items->finished == 1)
        {
            break;
        }
        draw_board(game_items->board, &game_items->turn);
        int pos;
        pos = rand() % 9;
        printf("%d\n", pos);
        game_items->finished = place_symbol(game_items->board, pos, &game_items->turn);
        pthread_cond_signal(&game_items->turn_cond);
        pthread_mutex_unlock(&game_items->mutex);
    } while(game_items->finished != 0);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2)
    {
        printf("Too many or few arguemtns provided.\n");
        printf("Correct call should be:\n");
        printf("./tic-tac-toe {param}\n");
        printf("{l} = default | {c} = vs cpu | {s} = socketed\n");
        exit(1);
    }
    if (*argv[1] != 'l' && *argv[1] != 'c' && *argv[1] != 's')
    {
        printf("Error parsing parameter!\n");
        printf("Correct call should be:\n");
        printf("./tic-tac-toe {param}\n");
        printf("{l} = local | {c} = vs cpu | {s} = socketed\n");
        exit(1);
    }
    // Initialize the shared data
    // Threads will be treated as users and will wait until it is
    // their turn to access to board and make their turn.
    game *game_items = malloc(sizeof(game));
    for (int i = 0; i < 9; i++)
    {
        printf("%d\n",i);
        game_items->board[i] = '_';
    }
    // Have x start first
    game_items->turn = 'x';
    game_items->finished = 0;
    pthread_t threads[2];
    pthread_mutex_init(&game_items->mutex, NULL);
    pthread_cond_init(&game_items->turn_cond, NULL);

    // L stands for local
    // Will create two threads for two players who are physically together to play.
    if (*argv[1] == 'l')
    {
        for (int i = 0; i < 2; i++)
        {
            thread_arg_t *user_arg = malloc(sizeof(thread_arg_t));
            if (i == 0) { user_arg->id = 'x'; }
            else { user_arg->id = 'o'; }
            user_arg->game_items = game_items;
            pthread_create(&threads[i], NULL, user, user_arg);
        }

        for (int i = 0; i < 2; i++) 
        {
            pthread_join(threads[i], NULL);
        }

        draw_board(game_items->board, &game_items->turn);
        printf("Game over!\n%c has won!\n", (char)game_items->turn);
    }
    if (*argv[1] == 'c')
    {
        thread_arg_t *user_arg = malloc(sizeof(thread_arg_t));
        thread_arg_t *comp_arg = malloc(sizeof(thread_arg_t));

        user_arg->id = 'x';
        comp_arg->id = 'o';

        user_arg->game_items = game_items;
        comp_arg->game_items = game_items;

        pthread_create(&threads[0], NULL, user, user_arg);
        pthread_create(&threads[1], NULL, computer, comp_arg);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);

        draw_board(game_items->board, &game_items->turn);
        printf("Game over!\n%c has won!\n", (char)game_items->turn);
    }
    return 0;
}