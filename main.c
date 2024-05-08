#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>



#define E 1 // 3(1) = 3
#define X 2 // 3(2) = 6
#define O 3 // 3(3) = 9


void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw); // read terminal attributes
    raw.c_lflag &= ~(ICANON | ECHO); // disable canonical and echo modes
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &raw); // Apply new settings
}

int random_move() {
    return (int) time(NULL) % 9;
}

int sum_line(int board[], int start, int end, int step, char player) {
    int magic_square[] = {
        4, 9, 2,
        3, 5, 7,
        8, 1, 6,
    };

    int total = 0;
    for(int i = start; i <= end; i += step) {
        if (board[i] == player) {
            total += magic_square[i];
        }
    }

    return total;
}

bool check_win(int board[], char player) {

   int possible_win_states[] = {
    // Horizontal
    sum_line(board, 0, 2, 1, player),
    sum_line(board, 3, 5, 1, player),
    sum_line(board, 6, 8, 1, player),

    // Vertical
    sum_line(board, 0, 6, 3, player),
    sum_line(board, 1, 7, 3, player),
    sum_line(board, 2, 8, 3, player),

    // Diagonal
    sum_line(board, 0, 8, 4, player),
    sum_line(board, 2, 6, 2, player),
   };

   for(int i = 0; i < sizeof(possible_win_states)/sizeof(int); i++) {
       if (possible_win_states[i] == 15) { return true;}
   }
    return false; // didn't find 3 in a row

}

bool legal_move(int active_tile, int movement) {
    int spot = active_tile + movement;

    return 0 <= spot && spot <= 8;
}

char get_input() {
    enable_raw_mode();
    char c = getchar();
    system("stty cooked echo");
    return c;
}

void place_letter(int active_tile, char letter, int board[]) {
    board[active_tile] = letter;
}

void display_board(int board[], int active_tile) {

    int i;
    int spots = 9;

    printf("|");
    for(i = 0; i < spots; i++) {

        char symbol;

        switch (board[i]) {
            case E:
                symbol = ' ';//'0' + i; // print the char version of the number
                break;

            case X:
                symbol  = 'X';
                break;

            case O:
                symbol = 'O';
                break;

            default:
                break;
        }

        if (i == active_tile) {
            printf("[%c]", symbol);
        } else {
            printf(" %c ", symbol);
        }

        // determine new lines
        switch(i) {
            case 2:
                printf("|\n|");
                break;
            case 5:
                printf("|\n|");
                break;
            default:
                break;
        }


    }
    printf("|\n");

}

int main() {
    int board[] = {
        E, E, E,
        E, E, E,
        E, E, E
    };

    int active_tile = 0;
    int computer_move;
    bool placed = false;
    char c;

    while(1) {
        system("clear");
        display_board(board, active_tile);

        if (check_win(board, X)) {
            printf("Player X won!\n");
            break;
        }
        if (check_win(board, O)) {
            printf("Player O won!\n");
            break;
        }

        c = get_input();


        switch(c) {
            case 'w':
                if (legal_move(active_tile, -3)) { active_tile += -3; }
                break;
            case 'a':
                if (legal_move(active_tile, -1)) { active_tile += -1; }
                break;
            case 's':
                if (legal_move(active_tile, 3)) { active_tile += 3; }
                break;
            case 'd':
                if (legal_move(active_tile, 1)) { active_tile += 1; }
                break;
            case '\n':
                place_letter(active_tile, X, board);
                placed = true;
                break;
            default:
                break;
        }

        if (placed) {
            computer_move = random_move();

            while(board[computer_move] != E) {
                computer_move = random_move();
            }

            place_letter(computer_move, O, board);
            placed = false;
        }


    }
}
