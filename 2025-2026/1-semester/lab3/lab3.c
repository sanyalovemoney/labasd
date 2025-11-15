#include <stdio.h>
#include <windows.h>

#define ROW 24
#define COL 80

void vriter(int row, int col, char ch) {
    COORD coord;
    coord.X = col;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    putchar(ch);
    fflush(stdout);
}

int main(void) {
    system("cls");

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cci);

    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++)
            putchar(' ');
        putchar('\n');
    }

    int row = ROW / 2;
    int col = COL / 2;

    int dir = 0;

    int step_len = 1;
    int steps_in_leg = 0;
    int legs_done = 0;

    while (1) {
        vriter(row, col, '*');
        Sleep(40);

        if (row == 0 && col == 0)
            break;

        int nr = row;
        int nc = col;

        if (dir == 0)      nc++;   
        else if (dir == 1) nr--;   
        else if (dir == 2) nc--;   
        else if (dir == 3) nr++;   

        if (nr == 0 && nc == 0) {
            row = 0;
            col = 0;
            vriter(row, col, '*');
            break;
        }

        if (nr < 0 || nr >= ROW || nc < 0 || nc >= COL)
            break;

        row = nr;
        col = nc;
        steps_in_leg++;

        if (steps_in_leg == step_len) {
            steps_in_leg = 0;
            dir = (dir + 1) % 4;

            legs_done++;
            if (legs_done % 2 == 0)
                step_len++;
        }
    }

    COORD endPos = { 0, ROW };
    SetConsoleCursorPosition(hOut, endPos);
    cci.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cci);

    return 0;
}
