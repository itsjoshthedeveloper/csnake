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

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR)
    {
        ungetch(ch);
        return true;
    }
    else
    {
        return false;
    }
}

void draw(window win, snake s)
{
    for (int i = -1; i <= win.height; i++)
    {
        for (int j = -1; j <= win.width; j++)
        {
            if ((i == -1 || i == win.height) || (j == -1 || j == win.width))
            {
                printw("#");
            }
            else if (j == s.headPosition.x && i == s.headPosition.y)
            {
                printw("O");
            }
            else
            {
                printw(" ");
            }
        }
        printw("\n");
    }
}

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

    int key_code;

    while (true)
    {
        clear(); // Clear window
        refresh();

        printw("frame: %d\n", win.frame);
        draw(win, s);

        if (kbhit())
        {
            key_code = getch();
            flushinp();
            // printw("%d\n", key_code);
            switch (key_code)
            {
            case KEY_UP:
                s.headSpeed = (speed){0, 0};
                s.headSpeed.y = -1;
                break;
            case KEY_DOWN:
                s.headSpeed = (speed){0, 0};
                s.headSpeed.y = 1;
                break;
            case KEY_LEFT:
                s.headSpeed = (speed){0, 0};
                s.headSpeed.x = -1;
                break;
            case KEY_RIGHT:
                s.headSpeed = (speed){0, 0};
                s.headSpeed.x = 1;
                break;
            default:
                break;
            }
        }
        refresh();

        usleep((int)((1.0 / win.fps) * 1000) * 1000);

        if ((win.frame % win.fps) == 0)
        {
            s.headPosition = (position){(s.headPosition.x + s.headSpeed.x), (s.headPosition.y + s.headSpeed.y)};
        }
        win.frame++;
    }

    endwin();
    return 0;
}