#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>


int key;
int width = 0, height = 0;
int PosX = 1, PosY = 1;
int position = 0;
int n_button;
char new_maze[240][67];
char maze[240][67];
int sneg_counter = 0;
int time_set = 200000;


// void print_zvezd_coordinates(int width, int height, char new_maze[height][width]) {
//     printw("Координаты всех символов '*':\n");
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             if (new_maze[y][x] == '*') {
//                 printw("(%d, %d)\n", y, x);
//             }
//         }
//     }
// }

// void print_wall_coordinates(int width, int height, char new_maze[height][width]) {
//     printw("Координаты всех символов '#':\n");
//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             if (new_maze[y][x] == '#') {
//                 printw("(%d, %d)\n", y, x);
//             }
//         }
//     }
// }


//создаем матрицу
void createlab2(int width, int height, char new_maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            new_maze[y][x] = '.';
        }
    }
}

// отображаем маорицу
void drawmain2(int width, int height, char new_maze[height][width], int PosX, int PosY) {
    clear();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == PosX && y == PosY) {
                mvaddch(y, x, '*');
            } else {
                mvaddch(y, x, new_maze[y][x]);
            }
        }
    }
    printw("\n\nE - бросить снежинку");
    printw("\n\nВВЕРХ - ускорить, ВНИЗ - замедлить");
    printw("\nСкорость usleep(чем меньше тем быстрее) - %i\n", time_set / 100000);

    // print_zvezd_coordinates(width, height, new_maze);
}


void otrisovka_snega(int PosY, int PosX, char new_maze[height][width]) {
    // первая строка запрет
    if (PosY == 0 && (new_maze[PosY + 1][PosX] == '#' || new_maze[PosY + 1][PosX] == '*')) {
        return;
    }

    new_maze[PosY][PosX] = '*';


    bool moved;  // отслжка
    do {
        moved = false;
        for (int y = height - 2; y >= 0; y--) {  // снизу вверх
            for (int x = 0; x < width; x++) {
                if (new_maze[y][x] == '*') {
                    int snowflake_count = 0;
                    for (int check_y = y + 1; check_y < height; check_y++) {
                        if (new_maze[check_y][x] == '*') {
                            snowflake_count++;
                        } else {
                            break;
                        }
                    }

                    //если пусто -  вниз
                    if (new_maze[y + 1][x] == '.') {
                        new_maze[y][x] = '.';
                        new_maze[y + 1][x] = '*';
                        moved = true;
                        break;
                    }
                    // если 2 есть то влево
                    // snowflake МОГУ УБРАТЬ огр
                    if (x > 0 && new_maze[y + 1][x] != '.' && new_maze[y + 1][x - 1] == '.' && new_maze[y][x - 1] == '.' && new_maze[y + 2][x - 1] == '.') {
                            new_maze[y][x] = '.';
                            new_maze[y + 1][x - 1] = '*';
                            moved = true;
                            break;
                        }
                    // вправо
                    if (x < width - 1 && new_maze[y + 1][x] != '.' && new_maze[y + 1][x + 1] == '.' && new_maze[y][x + 1] == '.' && new_maze[y + 2][x + 1] == '.') {
                            new_maze[y][x] = '.';
                            new_maze[y + 1][x + 1] = '*';
                            moved = true;
                            break;
                    }
                }
            }
        }


        drawmain2(width, height, new_maze, PosX, PosY);
        refresh();
        usleep(time_set);

    } while (moved); // пока перемещение
}


void keywork2(int width, int height, char new_maze[height][width]) {
    drawmain2(width, height, new_maze, PosX, 0);

    key = getch();
    if (key == KEY_RIGHT) {
        if (PosX < width - 1) PosX++;
    }
    if (key == KEY_LEFT) {
        if (PosX > 0) PosX--;
    }
    if (key == 'e') {
        otrisovka_snega(PosY, PosX, new_maze);
    }
    if (key == KEY_UP && time_set>0) {
        time_set-= 100000;
    }
    if (key == KEY_DOWN && time_set<400000) {
        time_set+= 100000;
    }
}


void matrix_copy(int width, int height, char maze[height][width], char new_maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            new_maze[y][x] = maze[y][x];
        }
    }
}

// отображаем маорицу
void drawmain(int width, int height, char maze[height][width], int PosX, int PosY) {
    clear();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == PosX && y == PosY) {
                mvaddch(y, x, '$'); //Player
            } else {
                mvaddch(y, x, maze[y][x]); //Lab
            }
        }
    }
    printw("\n\nQ - поставить препятствие");
    printw("\nDELETE - убрать препятствие");
    printw("\nW - сохранить и выйти");
}

void keywork(char maze[height][width]) {
    drawmain(width, height, maze, PosX, PosY);

    key = getch();

    if (key == KEY_UP) {
        if (PosY > 0) PosY--;
    }
    if (key == KEY_DOWN) {
        if (PosY < height - 1) PosY++;
    }
    if (key == KEY_RIGHT) {
        if (PosX < width - 1) PosX++;
    }
    if (key == KEY_LEFT) {
        if (PosX > 0) PosX--;
    }
    if (key == 'q' && PosY != 0) {
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
    printw("%s", "ver 1.0\n");
    printw("%s", "\n");
    printw("%s", "Перед использованием прочитайте инструкцию 3 п.\n");
    printw("%s", "-----------------\n");
    printw("%s", "\n");

    char screen[5][80] = {
        "1. Задать размеры и создать препятствия",
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
        // if (i == 4) {
        //     print_wall_coordinates(width, height, new_maze);
        // }
    }
}

//TODO НАПИШИ ПОТОМ ИНСТРУКЦИЮ
void instruction (){
    printw("%s", "-------------------------------------------------------\n");
    printw("%s", "Используйте стрелки для перемещения\n");
    printw("%s", "Вы - $ или *\n");
    printw("%s", "E - бросить снежинку, ВВЕРХ - ускорить, ВНИЗ - замедлить\n");
    printw("%s", "Скорость usleep(чем меньше тем быстрее)\n");
    printw("%s", "Q - поставить препятствие, DELETE - убрать препятствие, W - сохранить и выйти\n\n");

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
                printw("Введите размеры - ширину и высоту через пробел (240x55 max! 3x3 min!): ");
                scanw("%d %d", &width, &height);
                if (width < 3 || height < 3 || width > 240 || height > 55) {
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
                if (key == 'w') {
                    createlab2(width, height, new_maze);
                    matrix_copy(width, height, maze, new_maze);
                    keypad(stdscr, FALSE);
                    break;
                }
                if (key == 27) {
                    keypad(stdscr, FALSE);
                    break;
                }
            }
        }

        // 2 пункт
        if ((position == 1 && n_button == 10) || n_button == 50) {
            keypad(stdscr, TRUE);
            clear();
            PosY = 0, PosX = 0;
            if (width < 1 && height < 1) {
                printw("Ничего не ввели в первом пункте");
                getch();
                keypad(stdscr, FALSE);
            }else {
                while (1) {
                        keywork2(width, height, new_maze);
                        if (key == 27) {  // Выход по клавише ESC
                            keypad(stdscr, FALSE);
                            break;
                        }
                }
            }
        }

        // 3 пункт
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