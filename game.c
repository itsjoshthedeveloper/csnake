#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>

typedef struct
{
    int fps;
    int width;
    int height;
    int frame;
} window;

typedef struct
{
    int x;
    int y;
} position;

typedef struct
{
    int x;
    int y;
} speed;

typedef struct
{
    position headPosition;
    speed headSpeed;
} snake;

int main()
{
    initscr(); // Start curses mode

    cbreak();               // Line buffering disabled
    keypad(stdscr, TRUE);   // Enable arrow keys
    noecho();               // No echoing of typed keys
    nodelay(stdscr, TRUE);  // No delay for input
    scrollok(stdscr, TRUE); // Not sure

    printw("Welcome to the Snake Game\n");
    refresh();
    sleep(3);

    window win = {30, 20, 10, 0};
    snake s = {{9, 4}, {0, 0}};

    endwin();
    return 0;
}