/*
8. Implementati un joc in doi de tip X si 0, care se poate juca de la o
 aceeasi tastaura / mouse: pe o tabla 3x3 fiecare jucator plaseaza alternativ
 semnul sau, X sau 0; cine reuseste primul sa completeze o linie, coloana
 sau diagonala, castiga; jocul se poate termina remiza.
   Bifarea pozitiilor se poate face doar cu mouse-ul. Programul va verifica
 corectitudinea actiunilor (nu va permite actiuni incorecte), va detecta si
 va afisa cine este la rand, victoria sau remiza.
*/

#include <ncurses.h>
#include <stdlib.h>

#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

char board[3][3];
int currentPlayer = 1;  // 1 pentru X, 0 pentru O

// Functie pentru a initializa tabla
void init_board() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Functie pentru a desena tabla de joc
void draw_board() {
    clear();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mvprintw(2 + i * 4, 4 + j * 4, "%c", board[i][j]);
            if (j < 2) mvprintw(2 + i * 4, 5 + j * 4, "|");
        }
        if (i < 2) mvprintw(3 + i * 4, 4, "---+---+---");
    }
    mvprintw(14, 0, "Jucatorul curent: %c", currentPlayer == 1 ? PLAYER_X : PLAYER_O);
    refresh();
}

// Functie pentru a verifica daca mutarea este valida
int is_valid_move(int row, int col) {
    return board[row][col] == EMPTY;
}

// Functie pentru a face mutarea
void make_move(int row, int col) {
    board[row][col] = currentPlayer == 1 ? PLAYER_X : PLAYER_O;
    currentPlayer = 1 - currentPlayer;  // Schimba jucatorul
}

// Functie pentru a verifica daca un jucator a castigat
int check_winner() {
    for (int i = 0; i < 3; i++) {
        // Verifica liniile
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY)
            return board[i][0];
        // Verifica coloanele
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY)
            return board[0][i];
    }
    // Verifica diagonalele
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY)
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY)
        return board[0][2];
    
    return 0;  // Niciun castigator
}

// Functie pentru a verifica daca tabla este plina (remiza)
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
    int row, col;
    MEVENT event;

    // Initializare ncurses
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
                // Calculam randul si coloana in functie de coordonatele mouse-ului
                row = (event.y - 2) / 4;
                col = (event.x - 4) / 4;
                if (row >= 0 && row < 3 && col >= 0 && col < 3 && is_valid_move(row, col)) {
                    make_move(row, col);
                    draw_board();
                    // Verifica daca avem un castigator
                    char winner = check_winner();
                    if (winner) {
                        mvprintw(16, 0, "Jucatorul %c a castigat!", winner);
                        refresh();
                        getch();
                        break;
                    }
                    // Verifica remiza
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

    // Terminare ncurses
    endwin();
    return 0;
}