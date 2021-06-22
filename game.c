#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

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
} direction;

typedef struct
{
    position *position;
    direction direction;
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

int choose(char msg[], int initialChoice, char *choices[], size_t length)
{
    int choice = initialChoice;
    int y, x, key_code;

    printw("\n%s\n\n", msg);

    bool chose = false;
    while (!chose)
    {
        getyx(stdscr, y, x);
        move(y - 1, 0);
        clrtoeol();
        refresh();

        for (int i = 1; i <= length; i++)
        {
            if (i == choice)
            {
                attron(A_STANDOUT);
                printw("%s", choices[i - 1]);
                attroff(A_STANDOUT);
                printw(" ");
            }
            else
            {
                printw("%s ", choices[i - 1]);
            }
        }
        printw("\n");
        refresh();
        if (kbhit())
        {
            key_code = getch();
            flushinp();
            switch (key_code)
            {
            case KEY_LEFT:
                if (choice == 1)
                {
                    choice = length;
                }
                else
                {
                    choice--;
                }
                break;
            case KEY_RIGHT:
                if (choice == length)
                {
                    choice = 1;
                }
                else
                {
                    choice++;
                }
                break;
            case 10:
            case 32:
                chose = true;
                break;
            default:
                break;
            }
        }
        usleep((int)((1.0 / 30) * 1000) * 1000);
    }
    return choice;
}

position getNewApplePosition(window win, snake s)
{
    position pos;
    bool overlap;
    do
    {
        overlap = false;
        pos = (position){randInt(win.width), randInt(win.height)};
        for (int i = 0; i < s.length; i++)
        {
            if (pos.x == s.position[i].x && pos.y == s.position[i].y)
            {
                overlap = true;
                break;
            }
        }
    } while (overlap);
    return pos;
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

    int fps = 60;
    int key_code;
    char *yesno[2] = {"YES", "NO"};

    printw("Welcome to the Snake Game\n");
    refresh();
    sleep(2);

    if (choose("Display instructions?", 2, yesno, NELEMS(yesno)) == 1)
    {
        printw("\nInstructions: Maneuver the snake to eat the apples (X). Make sure not to run into any walls or into the snake tail.\n");
        refresh();
        sleep(5);

        printw("\nControls:\n");
        printw("\tSnake movement: arrow keys\n");
        refresh();
        sleep(3);

        printw("\nObjective: Get the highest score!\n");
        refresh();
        sleep(5);

        clear();
    }

    int difficulty = 1;
    bool playing = true;

    while (playing)
    {
        char *difficulties[3] = {"SLUG", "WORM", "PYTHON"};
        difficulty = choose("Choose difficulty level:", difficulty, difficulties, NELEMS(difficulties));

        printw("\nStarting in ");
        refresh();
        sleep(1);
        for (int i = 3; i > 0; i--)
        {
            printw("%d ", i);
            refresh();
            sleep(1);
        }

        int width, height;
        switch (difficulty)
        {
        case 1:
            width = 30;
            height = 15;
            break;
        case 2:
            width = 20;
            height = 10;
            break;
        case 3:
            width = 14;
            height = 6;
            break;
        default:
            width = 20;
            height = 10;
            break;
        }
        window win = {fps, width, height, 0};

        position *tempPtr = malloc(3 * sizeof(position));
        if (tempPtr == NULL)
            fail();
        snake s = {tempPtr, {1, 0}, 3};
        s.position[0] = (position){9, 4};
        s.position[1] = (position){8, 4};
        s.position[2] = (position){7, 4};

        apple a = {getNewApplePosition(win, s)};

        int score = 0;
        int speed = ((win.fps * 66) / ((int)pow(4, difficulty) * score + 100));

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
                    if (s.direction.y != 1)
                    {
                        s.direction = (direction){0, 0};
                        s.direction.y = -1;
                    }
                    break;
                case KEY_DOWN:
                    if (s.direction.y != -1)
                    {
                        s.direction = (direction){0, 0};
                        s.direction.y = 1;
                    }
                    break;
                case KEY_LEFT:
                    if (s.direction.x != 1)
                    {
                        s.direction = (direction){0, 0};
                        s.direction.x = -1;
                    }
                    break;
                case KEY_RIGHT:
                    if (s.direction.x != -1)
                    {
                        s.direction = (direction){0, 0};
                        s.direction.x = 1;
                    }
                    break;
                default:
                    break;
                }
            }
            speed = ((win.fps * 66) / ((int)pow(4, difficulty) * score + 100));
            printw("snake speed: %.3f secs\n", speed / 30.0);
            refresh();

            if ((win.frame % speed) == 0)
            {
                position buffer = (position){(s.position[0].x + s.direction.x), (s.position[0].y + s.direction.y)};
                for (int i = 0; i < s.length; i++)
                {
                    position temp = s.position[i];
                    s.position[i] = buffer;
                    buffer = temp;
                }
                if (s.position[0].x == a.position.x && s.position[0].y == a.position.y)
                {
                    s.length++;
                    s.position = (position *)realloc(s.position, s.length * sizeof(position));
                    if (s.position == NULL)
                        fail();
                    s.position[s.length - 1] = s.position[s.length - 2];

                    a.position = getNewApplePosition(win, s);
                    score++;
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
                    sleep(1);
                    break;
                }
            }

            usleep((int)((1.0 / win.fps) * 1000) * 1000);
            win.frame++;
        }

        if (choose("Play again?", 1, yesno, NELEMS(yesno)) == 2)
        {
            playing = false;
        }
    }
    endwin();
    return 0;
}