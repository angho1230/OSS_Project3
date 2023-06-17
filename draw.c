#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "draw.h"

extern int g_mod;
extern int g_x;
extern int g_y;

void rectangle(int y1, int x1, int y2, int x2)
{
	mvhline(y1, x1, 0, x2 - x1);
	mvhline(y2, x1, 0, x2 - x1);
	mvvline(y1, x1, 0, y2 - y1);
	mvvline(y1, x2, 0, y2 - y1);
	mvaddch(y1, x1, ACS_ULCORNER);
	mvaddch(y2, x1, ACS_LLCORNER);
	mvaddch(y1, x2, ACS_URCORNER);
	mvaddch(y2, x2, ACS_LRCORNER);
	refresh();
}

void rectangle_color(int y1, int x1, int y2, int x2, int color)
{
	start_color();
	int c;
	if (color == 1)
	{
		init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
		c = BLACK;
	}
	else if (color == -1)
	{
		init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
		c = WHITE;
	}
	else if (color == 2)
	{
		init_pair(OUTLINE, COLOR_YELLOW, COLOR_GREEN);
		c = OUTLINE;
	}
	else
	{
		init_pair(BLANK, COLOR_YELLOW, COLOR_YELLOW);
		c = BLANK;
	}
	attron(COLOR_PAIR(c));
	move(y1, x1);
	for (int x = x1; x < x2; x++)
	{
		for (int y = y1; y < y2 + 1; y++)
		{
			mvprintw(y, x, " ");
		}
	}
	attroff(COLOR_PAIR(c));
	refresh();
}

void draw_table()
{
	initscr();
	clear();
	int row = 0, col = 0;

    while(row < 38 || col < 90){
		getmaxyx(stdscr,row,col);
		mvprintw(0,0,"This screen has %d rows and %d columns\n try make at least 38 rows and 90 columns",row,col);
		refresh();
		sleep(1);
    }
	clear();
	// noecho();
	cbreak();
	curs_set(2);
	start_color();
	init_pair(BG, COLOR_BLACK, COLOR_YELLOW);
	attron(COLOR_PAIR(BG));
	bkgd(COLOR_PAIR(BG));
	attroff(COLOR_PAIR(BG));

	for (int x = 0; x < 7; x++)
	{
		mvvline(START_Y, (x + 1) * PADDING_X + START_X, 0, 8 * PADDING_Y);
	}
	for (int y = 0; y < 7; y++)
	{
		mvhline((y + 1) * PADDING_Y + START_Y, START_X, 0, 8 * PADDING_X);
	}
	for (int x = 1; x < 8; x++)
	{
		mvprintw(START_Y - 1, x * PADDING_X + PADDING_X / 2 + START_X, "%d", x + 1);
	}
	for (int y = 1; y < 8; y++)
	{
		mvprintw(PADDING_Y / 2 + y * PADDING_Y + START_Y, START_X - 1, "%d", y + 1);
	}
	mvprintw(START_Y - 1 + PADDING_Y / 2, START_X - 1 + PADDING_X / 2, "1");
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			mvaddch((y + 1) * PADDING_Y + START_Y, (x + 1) * PADDING_X + START_X, ACS_PLUS);
		}
	}
	curs_set(2);
	getmaxyx(stdscr, row, col);
	rectangle(row - 10, 3, row - 1, col - 3);
	refresh();
}

void message(char *m)
{
	int row, col;
	getmaxyx(stdscr, row, col);
	mvprintw(row - 2, 4, "%s", m);
	getyx(stdscr, g_y, g_x);
	refresh();
}
void message_color(char *m, int color)
{
	int row, col;
	start_color();
	init_pair(TEXT_COL, color, color);
	getmaxyx(stdscr, row, col);
	mvprintw(row - 2, 4, "%s", m);
	getyx(stdscr, g_y, g_x);
	refresh();
}

char key_input(int print)
{
	curs_set(2);
	noecho();
	move(g_y, g_x + 1);
	int c = 0;

	c = getch();
	timeout(0);
	if(c==0) return 0;
	switch (c)
	{
	case '\t':
	case '\n':
		for (int i = 0; i < 51; i++)
		{
			mvprintw(g_y, 28 + i, " ");
		}
		g_x = 28;
		break;
	case 127:
	case '\b':
		if (g_x > 28)
		{
			mvprintw(g_y, g_x, " ");
			g_x--;
			refresh();
		}
		break;
	default:
		if (isprint(c))
		{
			if (print)
			{
				g_x++;
				mvprintw(g_y, g_x, "%c", c);
				refresh();
			}
		}
		else
		{
			return 0;
		}
		break;
	}
	return c;
}

void print_messages(char (*messages)[80])
{
	int row, col;
	getmaxyx(stdscr, row, col);
	for (int i = 0; i < 7; i++)
	{
		move(row - 3 - i, 5);
		for (int j = 0; j < 80; j++)
		{
			printw(" ");
		}
		mvprintw(row - 3 - i, 5, "%s", messages[i]);
	}
}

void score_print(char * name1, char * name2, int score1, int score2)
{
	int row, col;
	getmaxyx(stdscr, row, col);
	mvprintw( 7, col -30, "              ");
	mvprintw( 9, col -30, "              ");
	mvprintw( 7, col -30, "%s : %d",name1, score1);
	mvprintw( 9, col -30 , "%s : %d",name2, score2);
}