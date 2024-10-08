/*
8. Implementati un joc in doi de tip X si 0, care se poate juca de la o
 aceeasi tastaura / mouse: pe o tabla 3x3 fiecare jucator plaseaza alternativ
 semnul sau, X sau 0; cine reuseste primul sa completeze o linie, coloana
 sau diagonala, castiga; jocul se poate termina remiza.
   Bifarea pozitiilor se poate face doar cu mouse-ul. Programul va verifica
 corectitudinea actiunilor (nu va permite actiuni incorecte), va detecta si
 va afisa cine este la rand, victoria sau remiza.
*/

/*
gcc -o main.o main.c -lncurses
./main.o
*/

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

char board[3][3];
char current_player = EMPTY;

void switch_current_player() {
    if (current_player == PLAYER_X) {
        current_player = PLAYER_O;
    }
    else {
        current_player = PLAYER_X;
    }
}

void init_board() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = EMPTY;
        }
    }

    srand(time(NULL));

    int random_player_id = rand() % 2;

    if (random_player_id == 0) {
        current_player = PLAYER_X;
    }
    else {
        current_player = PLAYER_O;
    }
}

void draw_board() {
    clear();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mvprintw(2 + i * 4, 4 + j * 4, "%c", board[i][j]);
            if (j < 2) mvprintw(2 + i * 4, 5 + j * 4, "|");
        }
        if (i < 2) mvprintw(3 + i * 4, 4, "---+---+---");
    }

    mvprintw(14, 0, "Jucatorul curent: %c", current_player);

    refresh();
}

int can_make_move(int row, int col) {
    return board[row][col] == EMPTY;
}

void make_move(int row, int col) {
    board[row][col] = current_player;
    switch_current_player();    
}

int check_winner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY)
            return board[i][0];

        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY)
            return board[0][i];
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY)
        return board[0][0];

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY)
        return board[0][2];
    
    return 0;
}

int check_draw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY)
                return 0;
        }
    }

    return 1;
}

int main() {
    int row = 0, col = 0;
    MEVENT event;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    init_board();
    draw_board();

    while (1) {
        int c = getch();
        if (c == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                row = (event.y - 2) / 4;
                col = (event.x - 2) / 4;

                if (row >= 0 && row < 3 && col >= 0 && col < 3 && can_make_move(row, col)) {
                    make_move(row, col);
                    draw_board();

                    char winner = check_winner();

                    if (winner) {
                        mvprintw(16, 0, "Jucatorul %c a castigat!", winner);
                        refresh();
                        getch();
                        break;
                    }

                    if (check_draw()) {
                        mvprintw(16, 0, "Remiza!");
                        refresh();
                        getch();
                        break;
                    }
                }
            }
        }
    }

    endwin();

    return 0;
}