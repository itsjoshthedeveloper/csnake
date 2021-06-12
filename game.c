#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

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

typedef struct
{
    position position;
} apple;

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

int randInt(int max)
{
    return rand() % max;
}

void draw(window win, snake s, apple a)
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
            else if (j == a.position.x && i == a.position.y)
            {
                printw("X");
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
    srand(time(NULL));

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
    snake s = {{9, 4}, {1, 0}};
    apple a = {{randInt(win.width), randInt(win.height)}};

    int score = 0;
    int key_code;

    while (true)
    {
        clear(); // Clear window
        refresh();

        printw("score: %d\tframe: %d\n", score, win.frame);
        draw(win, s, a);

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

        if ((win.frame % (win.fps / 2)) == 0)
        {
            s.headPosition = (position){(s.headPosition.x + s.headSpeed.x), (s.headPosition.y + s.headSpeed.y)};
            if (s.headPosition.x == a.position.x && s.headPosition.y == a.position.y)
            {
                a.position = (position){randInt(win.width), randInt(win.height)};
                score++;
            }
            if (s.headPosition.x < 0 || s.headPosition.x > (win.width - 1) || s.headPosition.y < 0 || s.headPosition.y > (win.height - 1))
            {
                printw("Game over!\n");
                refresh();
                sleep(10);
                break;
            }
        }
        win.frame++;
    }

    endwin();
    return 0;
}