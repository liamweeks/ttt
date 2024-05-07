#include <complex.h>
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>


#define E 'E'
#define X 'X'
#define O 'O'


void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw); // read terminal attributes
    raw.c_lflag &= ~(ICANON | ECHO); // disable canonical and echo modes
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &raw); // Apply new settings
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

    for(i = 0; i < spots; i++) {

        char symbol;

        switch (board[i]) {
            case E:
                symbol = '0' + i; // print the char version of the number
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
                printf("\n");
                break;
            case 5:
                printf("\n");
                break;
            default:
                break;
        }


    }
    printf("\n");

}

int main() {
    int board[] = {
        E, E, E,
        E, E, E,
        E, E, E
    };

    int active_tile = 0;
    char c;

    while(1) {
        system("clear");
        display_board(board, active_tile);

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
            default:
                break;
        }
    }
}
