#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int key;
int width, height;
int PosX = 1, PosY = 1;
int position = 0;
int n_button;
// char maze;
int restrictToTopRow = 0;

// отображаем маорицу
void drawmain(int width, int height, char maze[height][width], int PosX, int PosY) {
    clear(); // Очищаем экран
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == PosX && y == PosY) {
                mvaddch(y, x, '$'); //Player
            } else {
                mvaddch(y, x, maze[y][x]); //Lab
            }
        }
    }
}

void keywork(char maze[height][width]) {
    drawmain(width, height, maze, PosX, PosY);
    key = getch();

    if (restrictToTopRow) {  // Режим перемещения только по верхней строке
        PosY = 0; // Устанавливаем позицию игрока на верхнюю строку
        if (key == KEY_RIGHT && PosX < width - 1) PosX++;
        if (key == KEY_LEFT && PosX > 0) PosX--;
    } else {  // Обычное перемещение
        if (key == KEY_UP && PosY > 0) PosY--;
        if (key == KEY_DOWN && PosY < height - 1) PosY++;
        if (key == KEY_RIGHT && PosX < width - 1) PosX++;
        if (key == KEY_LEFT && PosX > 0) PosX--;
    }

    if (key == 'q') {
        maze[PosY][PosX] = '#';
    }
    if (key == 127) {
        maze[PosY][PosX] = '.';
    }
}
//создаем матрицу
void createlab(int width, int height, char maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
                maze[y][x] = '.';
        }
    }
}

void menu() {
    printw("%s", "Sneginki by Tikhanov Oleg\n");
    printw("%s", "ver 0.1\n");
    printw("%s", "\n");
    printw("%s", "Перед использованием прочитайте инструкцию 3 п.\n");
    printw("%s", "-----------------\n");
    printw("%s", "\n");

    char screen[5][60] = {
        "1. Задать размеры и создать поля",
        "2. Играть",
        "3. Инструкция",
        "4. Выход",
        "",
    };

    for (int i = 0; i <= 4; ++i) {
        if (i == position) {
            printw("%s  %s\n", screen[i], "<---");
        } else {
            printw("%s\n", screen[i]);
        }

    }
}

//TODO НАПИШИ ПОТОМ ИНСТРУКЦИЮ
void instruction (){
    printw("%s", "-------------------------------------------------------\n");
    printw("%s", "Используйте стрелки для перемещения\n");
    printw("%s", "Вы - $\n");
    printw("%s", "Q - установка стены, DELETE - удалить значение в клетке\n");
    printw("%s", "S - установка старта, E - установка конца\n");
    printw("%s", "P - расчет пути\n\n");

}



int main() {
    setlocale(LC_ALL, "");
    initscr();
    while (true) {
        menu();

        n_button = getch();

        if (n_button == 27) {
            // ESC
            n_button = getch();
            if (n_button == 91) {
                // enter
                n_button = getch();
                if (n_button == 65) {
                    // up
                    position = position - 1;
                    if (position < 0) {
                        position = 3;
                    }
                } else if (n_button == 66) {
                    // down
                    position = position + 1;
                    if (position > 3) {
                        position = 0;
                    }
                }
            }
        }

        //1 laba
        if ((position == 0 && n_button == 10) || n_button == 49) {
            keypad(stdscr, TRUE);
            clear();
            while (true) {
                printw("Введите размеры - ширину и высоту через пробел (240x67 max! 3x3 min!): ");
                scanw("%d %d", &width, &height);
                if (width < 3 || height < 3 || width > 240 || height > 67) {
                    clear();
                    printw("Неверные значения!\n");
                } else {
                    break;
                }
            }
            clear();
            char maze[width][height];
            PosX = 1, PosY = 1;
            createlab(width, height, maze);
            while (1) {
                // Отображаем лаб
                keywork(maze);
                if (key == 'h') {
                    restrictToTopRow = 1; // Переключаем режим верхней строки
                }
                if (key == 27) {
                    keypad(stdscr, FALSE);
                    break;
                }
            }
        }

        // 2 пункт
        if ((position == 2 && n_button == 10) || n_button == 51) {
            clear();
            instruction();
            n_button = getch();
            if (n_button != 27) {
                clear();
                menu();
            }
        }

        //exit
        if ((position == 3 && n_button == 10) || n_button == 27 || n_button == 52) {
            break;
        }

        clear();
        endwin();
    }
    return 0;
}

