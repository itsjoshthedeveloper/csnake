#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>

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

    endwin();
    return 0;
}