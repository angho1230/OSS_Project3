#include <stdio.h>
#include "net.h"

typedef struct _text
{
    char c[51];
    int mod;
    char name[21];
}text;

typedef struct _coord_st
{
    int x_co;
    int y_co;
    int turn;
}coord_st;

void reversi_init(char * name1, char*name2);
int reversi_next_move(coord_st * coord, int turn);
int reversi_flip(coord_st coord);
int reversi_refresh(char * name1, char*name2);
int reversi_check(coord_st coord);
int reversi_input(text * txt, int turn, int* str_len);
int reversi_input_to_coord(coord_st *coord, int turn, text t);
void reversi_term();
void add_messages(char * m);
int reversi_available(int turn);
int reversi_score(int player);
int reversi_run(int conn_fd);
void reversi_ai_move(coord_st *, int turn);
int reversi_send(text t, int conn_fd);