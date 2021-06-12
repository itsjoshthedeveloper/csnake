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
    position *position;
    speed speed;
    int length;
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

void fail(void)
{
    printw("Memory could not be allocated\n");
    exit(EXIT_FAILURE);
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
            else if (j == s.position[0].x && i == s.position[0].y)
            {
                printw("O");
            }
            else if (j == a.position.x && i == a.position.y)
            {
                printw("X");
            }
            else
            {
                bool snakeBody = false;
                for (int k = 1; k < s.length; k++)
                {
                    if (j == s.position[k].x && i == s.position[k].y)
                    {
                        snakeBody = true;
                        break;
                    }
                }

                if (snakeBody)
                {
                    printw("o");
                }
                else
                {
                    printw(" ");
                }
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

    position *tempPtr = malloc(3 * sizeof(position));
    if (tempPtr == NULL)
        fail();
    snake s = {tempPtr, {1, 0}, 3};
    s.position[0] = (position){9, 4};
    s.position[1] = (position){8, 4};
    s.position[2] = (position){7, 4};

    apple a = {{randInt(win.width), randInt(win.height)}};

    int score = 0;
    int key_code;

    while (true)
    {
        clear(); // Clear window
        refresh();

        printw("score: %d\tframe: %d\n", score, win.frame);
        draw(win, s, a); // Draw game

        if (kbhit())
        {
            key_code = getch();
            flushinp();
            // printw("%d\n", key_code);
            switch (key_code)
            {
            case KEY_UP:
                s.speed = (speed){0, 0};
                s.speed.y = -1;
                break;
            case KEY_DOWN:
                s.speed = (speed){0, 0};
                s.speed.y = 1;
                break;
            case KEY_LEFT:
                s.speed = (speed){0, 0};
                s.speed.x = -1;
                break;
            case KEY_RIGHT:
                s.speed = (speed){0, 0};
                s.speed.x = 1;
                break;
            default:
                break;
            }
        }
        refresh();

        usleep((int)((1.0 / win.fps) * 1000) * 1000);

        if ((win.frame % (win.fps / 2)) == 0)
        {
            position buffer = (position){(s.position[0].x + s.speed.x), (s.position[0].y + s.speed.y)};
            for (int i = 0; i < s.length; i++)
            {
                position temp = s.position[i];
                s.position[i] = buffer;
                buffer = temp;
            }
            if (s.position[0].x == a.position.x && s.position[0].y == a.position.y)
            {
                bool overlap;
                do
                {
                    overlap = false;
                    position tempPosition = (position){randInt(win.width), randInt(win.height)};
                    for (int i = 0; i < s.length; i++)
                    {
                        if (tempPosition.x == s.position[i].x && tempPosition.y == s.position[i].y)
                        {
                            overlap = true;
                            break;
                        }
                    }
                } while (overlap);
                a.position = (position){randInt(win.width), randInt(win.height)};
                score++;

                s.length++;
                s.position = (position *)realloc(s.position, s.length * sizeof(position));
                if (s.position == NULL)
                    fail();
                s.position[s.length - 1] = s.position[s.length - 2];
            }
            bool collide = false;
            if (s.position[0].x < 0 || s.position[0].x > (win.width - 1) || s.position[0].y < 0 || s.position[0].y > (win.height - 1))
            {
                collide = true;
            }
            for (int i = 1; i < s.length; i++)
            {
                if (collide)
                {
                    break;
                }
                if ((s.position[0].x == s.position[i].x && s.position[0].y == s.position[i].y))
                {
                    collide = true;
                }
            }
            if (collide)
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