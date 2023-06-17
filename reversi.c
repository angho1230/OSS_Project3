#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <linux/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "reversi.h"
#include "draw.h"

extern int type;

static int g_mod;
static char messages[7][80];
static int board[8][8];
static int is_r;

int g_x;
int g_y;

typedef struct thread_s
{
    text *t;
    int conn_fd;
} ts;

void *recv_thread(void *tv)
{
    ts *t = (ts *)tv;
    is_r = recv_input(t->t, sizeof(text), t->conn_fd);
}

int reversi_run(int conn_fd)
{
    int turn = 1;
    int player = type == 2 ? -1 : 1;
    g_mod = MOVE;
    g_x = 3;
    g_y = 0;
    coord_st coord = {0, 0, 1};
    text txt_s;
    text txt_r;
    strcpy(&txt_r.name[0],"player");
    int str_len = 0;
    printf("what is your name? ");
    scanf("%20s", &(txt_s.name[0]));
    reversi_init(txt_s.name, txt_r.name);
    if (type == 1)
    {
        while (1)
        {
            int is_in = 0;
            int t_c = 0;
            ts struct_for_thread = {&txt_r, conn_fd};
            while (1)
            {
                is_r = 0;
                is_in = 0;
                if (t_c == 0)
                {
                    pthread_t tid;
                    is_r = pthread_create(&tid, NULL, recv_thread, &struct_for_thread);
                    t_c = 1;
                }
                is_in = reversi_input(&txt_s, turn, &str_len);
                if (is_in == 0 && is_r == 0)
                    continue;
                if (is_r == 1)
                {
                    t_c = 0;
                }
                break;
            }
            if (is_in == 1)
            {
                str_len = 0;
                if (turn == player)
                {
                    if (txt_s.mod == CHAT)
                    {
                        add_messages(txt_s.c);
                        send(conn_fd, &txt_s, sizeof(text), 0);
                        continue;
                    }
                    else
                    {
                        send(conn_fd, &txt_s, sizeof(text), 0);
                        if (reversi_input_to_coord(&coord, turn, txt_s) != 0)
                        {
                            txt_s.c[0] = '\0';
                            continue;
                        }
                    }
                }
                else
                {
                    if (txt_s.mod == CHAT)
                    {
                        add_messages(txt_s.c);
                        send(conn_fd, &txt_s, sizeof(text), 0);
                        continue;
                    }
                    else
                    {
                        add_messages("Not your turn.");
                        txt_s.c[0] = '\0';
                        continue;
                    }
                }
            }
            else if (is_r == 1)
            {
                if (turn == 1)
                {
                    if (txt_r.mod == CHAT)
                    {
                        char *m = malloc(sizeof(char) * 80);
                        sprintf(m, "%s> %s",txt_r.name, txt_r.c);
                        add_messages(m);
                        free(m);
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    if (txt_r.mod == CHAT)
                    {
                        char *m = malloc(sizeof(char) * 80);
                        sprintf(m, "%s> %s",txt_r.name, txt_r.c);
                        add_messages(m);
                        free(m);
                        continue;
                    }
                    else
                    {
                        if (reversi_input_to_coord(&coord, turn, txt_r) != 0)
                        {
                            continue;
                        }
                        /*
                        char *m = malloc(sizeof(char) * 80);
                        sprintf(m, "%s move (%d, %d)",txt_r.name ,coord.x_co, coord.y_co);
                        add_messages(m);
                        free(m);
                        */
                    }
                }
            }
            else
            {
                continue;
            }
            reversi_flip(coord);
            turn = turn * -1;
            if (reversi_available(turn) == 0)
            {
                if (reversi_available(turn * -1) == 0)
                {
                    reversi_refresh(txt_s.name, txt_r.name);
                    break;
                }
            }
            reversi_refresh(txt_s.name, txt_r.name);
            coord.turn = turn;
        }
    }
    else if (type == 2)
    {
        while (1)
        {
            int is_in = 0;
            int t_c = 0;
            ts struct_for_thread = {&txt_r, conn_fd};
            while (1)
            {
                is_r = 0;
                is_in = 0;
                if (t_c == 0)
                {
                    pthread_t tid;
                    is_r = pthread_create(&tid, NULL, recv_thread, &struct_for_thread);
                    t_c = 1;
                }
                is_in = reversi_input(&txt_s, turn, &str_len);
                if (is_in == 0 && is_r == 0)
                    continue;
                if (is_r == 1)
                {
                    t_c = 0;
                }
                break;
            }
            if (is_in == 1)
            {
                str_len = 0;
                if (turn == player)
                {
                    if (txt_s.mod == CHAT)
                    {
                        add_messages(txt_s.c);
                        send(conn_fd, &txt_s, sizeof(text), 0);
                        continue;
                    }
                    else
                    {
                        send(conn_fd, &txt_s, sizeof(text), 0);
                        if (reversi_input_to_coord(&coord, turn, txt_s) != 0)
                        {
                            txt_s.c[0] = '\0';
                            continue;
                        }
                    }
                }
                else
                {
                    if (txt_s.mod == CHAT)
                    {
                        add_messages(txt_s.c);
                        send(conn_fd, &txt_s, sizeof(text), 0);
                        continue;
                    }
                    else
                    {
                        add_messages("Not your turn.");
                        txt_s.c[0] = '\0';
                        continue;
                    }
                }
            }
            else if (is_r == 1)
            {
                if (turn == player)
                {
                    if (txt_r.mod == CHAT)
                    {
                        char *m = malloc(sizeof(char) * 80);
                        sprintf(m, "%s> %s",txt_r.name, txt_r.c);
                        add_messages(m);
                        free(m);
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    if (txt_r.mod == CHAT)
                    {
                        char *m = malloc(sizeof(char) * 80);
                        sprintf(m, "%s> %s",txt_r.name, txt_r.c);
                        add_messages(m);
                        free(m);
                        continue;
                    }
                    else
                    {
                        if (reversi_input_to_coord(&coord, turn, txt_r) != 0)
                        {
                            /*
                            char *m = malloc(sizeof(char) * 51 + 2);
                            sprintf(m, "move (%s)", txt_r.c);
                            add_messages(m);
                            free(m);*/
                            continue;
                        }
                        /*
                        char *m = malloc(sizeof(char) * 80);
                        sprintf(m, "move (%d, %d)", coord.x_co, coord.y_co);
                        add_messages(m);
                        free(m);*/
                    }
                }
            }
            else
            {
                continue;
            }
            reversi_flip(coord);
            turn = turn * -1;
            if (reversi_available(turn) == 0)
            {
                if (reversi_available(turn * -1) == 0)
                {
                    reversi_refresh(txt_r.name, txt_s.name);
                    break;
                }
            }
            reversi_refresh(txt_r.name, txt_s.name);
            coord.turn = turn;
        }
    }
    else if (type == 3)
    {
        while (1)
        {
            if (turn == 1)
            {
                int str_len = 0;
                int is_in = 0;
                while (is_in == 0)
                {
                    is_in = 0;
                    is_in = reversi_input(&txt_s, turn, &str_len);
                }

                if (reversi_input_to_coord(&coord, turn, txt_s) != 0)
                    continue;
            }
            else
            {
                reversi_ai_move(&coord, turn);
                sleep(1);
            }
            reversi_flip(coord);
            turn = -1 * turn;
            if (reversi_available(turn) == 0)
            {
                if (reversi_available(turn * -1) == 0)
                {
                    reversi_refresh(txt_s.name, "COM");
                    break;
                }
            }
            reversi_refresh(txt_s.name, "COM");
        }
        getch();
        reversi_term();
        return 0;
    }
    else if (type == 4)
    {
        while (1)
        {
            char *m = (char *)malloc(sizeof(char) * 80);
            if (turn == -1)
                sprintf(m, "Enter next move (WHITE)");
            if (turn == 1)
                sprintf(m, "Enter next move (BLACK)");
            message(m);
            free(m);
            if (turn == 1)
            {

                int str_len = 0;
                int is_in = 0;
                while (is_in == 0)
                {
                    is_in = 0;
                    is_in = reversi_input(&txt_s, turn, &str_len);
                }

                if (reversi_input_to_coord(&coord, turn, txt_s) != 0)
                    continue;
            }
            else
            {
                int str_len = 0;
                int is_in = 0;
                while (is_in == 0)
                {
                    is_in = 0;
                    is_in = reversi_input(&txt_s, turn, &str_len);
                }

                if (reversi_input_to_coord(&coord, turn, txt_s) != 0)
                    continue;
            }
            reversi_flip(coord);
            turn = -1 * turn;
            if (reversi_available(turn) == 0)
            {
                if (reversi_available(turn * -1) == 0)
                {
                    reversi_refresh("player1", "player2");
                    break;
                }
            }
            reversi_refresh("player1", "player2");
        }
        getch();
        reversi_term();
        return 0;
    }
}

void reversi_init(char * name1, char*name2)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = 0;
        }
    }
    board[3][4] = -1;
    board[3][3] = 1;
    board[4][3] = -1;
    board[4][4] = 1;
    for (int i = 0; i < 8; i++)
    {
        messages[i][0] = '\0';
    }
    draw_table();
    reversi_available(1);
    reversi_refresh(name1, name2);
    char *m = (char *)malloc(sizeof(char) * 80);
    sprintf(m, "Enter next move (BLACK)");
    message(m);
    free(m);
    return;
}
/*
int reversi_input(coord_st *coord, int turn, int conn_fd)
{
    char c;
    int max_str = 50;
    char str[51];
    int str_len = 0;
    int print = 1;
    do
    {
        c = key_input(print);
        if (c == 0)
        {
            continue;
        }
        else if (c == '\t')
        {
            if (g_mod == CHAT)
            {
                g_mod = MOVE;
                char *m = (char *)malloc(sizeof(char) * 51);
                if (turn == -1)
                    sprintf(m, "Enter next move (WHITE)");
                if (turn == 1)
                    sprintf(m, "Enter next move (BLACK)");
                message(m);
                free(m);
            }
            else
            {
                g_mod = CHAT;
                message("Type your message here:");
            }
        }
        else if (c == 8 || c == 127)
        {
            if (str_len > 0)
            {
                str[str_len - 1] = '\0';
                str_len--;
                print = 1;
            }
        }
        else
        {
            if (str_len >= max_str - 1)
            {
                print = 0;
                if (str_len == max_str)
                {
                    continue;
                }
            }
            str[str_len] = c;
            str_len++;
            str[str_len] = '\0';
        }
    } while (c != '\n');

    if (g_mod == CHAT)
    {
        add_messages(str);
    }
    else
    {
        char *str_c = strdup(str);
        if (reversi_input_to_coord(coord, turn, str_c) != 0)
        {
            free(str_c);
            return 1;
        }
        free(str_c);
    }
    text buf;
    strcpy(buf.c, str);
    buf.mod = g_mod;
        send(conn_fd, &buf, sizeof(buf), 0);
    return 0;
}*/

int reversi_input(text *txt, int turn, int *str_len)
{
    char c;
    int max_str = 10;
    int print = 1;
    c = key_input(print);
    if (c == 0)
    {
    }
    else if (c == '\t')
    {
        if (g_mod == CHAT)
        {
            g_mod = MOVE;
            txt->mod = MOVE;
            char *m = (char *)malloc(sizeof(char) * 80);
            if (turn == -1)
                sprintf(m, "Enter next move (WHITE)");
            if (turn == 1)
                sprintf(m, "Enter next move (BLACK)");
            message(m);
            free(m);
        }
        else
        {
            g_mod = CHAT;
            txt->mod = CHAT;
            message("Type your message here:");
        }
    }
    else if (c == 8 || c == 127)
    {
        if (*str_len > 0)
        {
            txt->c[(*str_len) - 1] = '\0';
            *str_len = (*str_len) - 1;
            print = 1;
        }
    }
    else if (c == '\n')
    {
        return 1;
    }
    else
    {
        if (*str_len >= max_str - 1)
        {
            print = 0;
            if (*str_len == max_str)
            {
                return 0;
            }
        }
        txt->c[*str_len] = c;
        *str_len = (*str_len) + 1;
        txt->c[*str_len] = '\0';
    }
    return 0;
}

int reversi_input_to_coord(coord_st *coord, int turn, text t)
{
    char * str = strdup(t.c);
    char *x_s = strtok(str, " ");
    char *y_s = strtok(0x0, " ");
    if (x_s == 0x0 || y_s == 0x0)
    {
        free(str);
        add_messages("Wrong input! Try again!");
        add_messages(str);
        return 1;
    }
    int x = atoi(x_s);
    int y = atoi(y_s);
    free(str);
    coord->x_co = x;
    coord->y_co = y;
    coord->turn = turn;
    char *message = (char *)malloc(sizeof(char) * 80);
    if (reversi_check(*coord) != 0)
    {
        sprintf(message, "%s wrong move : (%d, %d)", t.name, x, y);
        add_messages(message);
        free(message);
        return 1;
    }
    else
    {
        sprintf(message, "%s move : (%d, %d)", t.name, x, y);
    }
    add_messages(message);
    free(message);
    return 0;
}

int reversi_check(coord_st coord)
{
    if (!((0 < coord.x_co && coord.x_co < 9) && (0 < coord.y_co && coord.y_co < 9)))
    {
        return 1;
    }
    if (board[coord.x_co - 1][coord.y_co - 1] == 2)
    {
        board[coord.x_co - 1][coord.y_co - 1] = coord.turn;
        return 0;
    }
    return 1;
}

int reversi_flip(coord_st coord)
{
    int x = coord.x_co - 1;
    int y = coord.y_co - 1;
    for (int dir = 0; dir < 8; dir++)
    {
        int t = 0;
        int t_x = x;
        int t_y = y;
        while (0 <= t_x && t_x <= 7 && 0 <= t_y && t_y <= 7)
        {
            if (dir == 0)
            {
                t_x++;
            }
            else if (dir == 1)
            {
                t_x++;
                t_y++;
            }
            else if (dir == 2)
            {
                t_y++;
            }
            else if (dir == 3)
            {
                t_x--;
                t_y++;
            }
            else if (dir == 4)
            {
                t_x--;
            }
            else if (dir == 5)
            {
                t_x--;
                t_y--;
            }
            else if (dir == 6)
            {
                t_y--;
            }
            else
            {
                t_x++;
                t_y--;
            }
            if (0 <= t_x && t_x <= 7 && 0 <= t_y && t_y <= 7)
            {
                if (board[t_x][t_y] == -1 * coord.turn)
                {
                    t++;
                }
                else if (board[t_x][t_y] == coord.turn)
                {
                    t++;
                    if (t)
                    {
                        if (dir == 0)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x + k][y] = coord.turn;
                            }
                        }
                        else if (dir == 1)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x + k][y + k] = coord.turn;
                            }
                        }
                        else if (dir == 2)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x][y + k] = coord.turn;
                            }
                        }
                        else if (dir == 3)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x - k][y + k] = coord.turn;
                            }
                        }
                        else if (dir == 4)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x - k][y] = coord.turn;
                            }
                        }
                        else if (dir == 5)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x - k][y - k] = coord.turn;
                            }
                        }
                        else if (dir == 6)
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x][y - k] = coord.turn;
                            }
                        }
                        else
                        {
                            for (int k = 0; k <= t; k++)
                            {
                                board[x + k][y - k] = coord.turn;
                            }
                        }
                    }
                    break;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return 0;
}
int reversi_refresh(char * name1, char*name2)
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            rectangle_color(START_Y + y * PADDING_Y + 1,
                            START_X + x * PADDING_X + 1,
                            START_Y + y * PADDING_Y + PADDING_Y - 1,
                            START_X + x * PADDING_X + PADDING_X - 1,
                            board[x][y]);
        }
    }

    score_print(name1,name2, reversi_score(1), reversi_score(-1));

    refresh();
    return 0;
}
void reversi_term()
{
    int player1 = reversi_score(1);
    int player2 = reversi_score(-1);
    if (player1 > player2)
    {
        add_messages("Winner is Player1!");
    }
    else if (player1 < player2)
    {
        add_messages("Winner is Player2!");
    }
    else
    {
        add_messages("Score is even!!");
    }
    getch();
    add_messages("Press any key to exit.");
    endwin();
}
void add_messages(char *m)
{
    for (int j = 6; j > 0; j--)
    {
        strcpy(messages[j], messages[j - 1]);
    }
    strcpy(messages[0], m);

    print_messages(messages);
}

int reversi_available(int turn)
{
    int available = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y] == 2)
            {
                board[x][y] = 0;
            }
        }
    }
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y] == turn)
            {
                // check from right
                for (int dir = 0; dir < 8; dir++)
                {
                    int t_x = x;
                    int t_y = y;
                    int t = 0;
                    while (0 <= t_x && t_x <= 7 && 0 <= t_y && t_y <= 7)
                    {
                        if (dir == 0)
                        {
                            t_x++;
                        }
                        else if (dir == 1)
                        {
                            t_x++;
                            t_y++;
                        }
                        else if (dir == 2)
                        {
                            t_y++;
                        }
                        else if (dir == 3)
                        {
                            t_x--;
                            t_y++;
                        }
                        else if (dir == 4)
                        {
                            t_x--;
                        }
                        else if (dir == 5)
                        {
                            t_x--;
                            t_y--;
                        }
                        else if (dir == 6)
                        {
                            t_y--;
                        }
                        else
                        {
                            t_x++;
                            t_y--;
                        }
                        if (0 <= t_x && t_x <= 7 && 0 <= t_y && t_y <= 7)
                        {
                            if (board[t_x][t_y] == -1 * turn)
                            {
                                t = 1;
                            }
                            else if (board[t_x][t_y] == 0)
                            {
                                if (t)
                                {
                                    available++;
                                    board[t_x][t_y] = 2;
                                }
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if (available == 0)
    {
        add_messages("No available move! Passing turn.");
    }
    return available;
}

int reversi_score(int player)
{
    int score = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y] == player)
            {
                score++;
            }
        }
    }
    return score;
}

void reversi_ai_move(coord_st *coord, int turn)
{
    int mx = -1, my = -1;
    int mf = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board[x][y] == 2)
            {
                // check from right
                for (int dir = 0; dir < 8; dir++)
                {
                    int t_x = x;
                    int t_y = y;
                    int t = 0;
                    while (0 <= t_x && t_x <= 7 && 0 <= t_y && t_y <= 7)
                    {
                        if (dir == 0)
                        {
                            t_x++;
                        }
                        else if (dir == 1)
                        {
                            t_x++;
                            t_y++;
                        }
                        else if (dir == 2)
                        {
                            t_y++;
                        }
                        else if (dir == 3)
                        {
                            t_x--;
                            t_y++;
                        }
                        else if (dir == 4)
                        {
                            t_x--;
                        }
                        else if (dir == 5)
                        {
                            t_x--;
                            t_y--;
                        }
                        else if (dir == 6)
                        {
                            t_y--;
                        }
                        else
                        {
                            t_x++;
                            t_y--;
                        }
                        if (0 <= t_x && t_x <= 7 && 0 <= t_y && t_y <= 7)
                        {
                            if (board[t_x][t_y] == -1 * turn)
                            {
                                t++;
                            }
                            else if (board[t_x][t_y] == turn)
                            {
                                if (t)
                                {
                                    if ((x == 0 && y == 0) || (x == 7 && y == 7) || (x == 0 && y == 7) || (x == 7 && y == 0))
                                    {
                                        if ((mx == 0 && my == 0) || (mx == 7 && my == 7) || (mx == 0 && my == 7) || (mx == 7 && my == 0))
                                        {
                                            if (t > mf)
                                            {
                                                mx = x;
                                                my = y;
                                                mf = t;
                                            }
                                        }
                                        else
                                        {
                                            mx = x;
                                            my = y;
                                            mf = t;
                                        }
                                    }
                                    else if (t > mf)
                                    {
                                        if (!((mx == 0 && my == 0) || (mx == 7 && my == 7) || (mx == 0 && my == 7) || (mx == 7 && my == 0)))
                                        {
                                            mf = t;
                                            mx = x;
                                            my = y;
                                        }
                                    }
                                }
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    coord->x_co = mx + 1;
    coord->y_co = my + 1;
    coord->turn = turn;
    char *m = (char *)malloc(sizeof(char) * 80);
    sprintf(m, "AI is moving..(%d %d)", mx, my);
    add_messages(m);
}
