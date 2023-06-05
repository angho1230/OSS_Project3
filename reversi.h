#include <stdio.h>

typedef struct _coord_st
{
    int x_co;
    int y_co;
    int turn;
}coord_st;

void reversi_init();
int reversi_next_move(coord_st * coord, int turn);
int reversi_flip(coord_st coord);
int reversi_refresh();
int reversi_check(coord_st coord);
int reversi_input(coord_st * coord, int turn, int conn_fd);
int reversi_input_to_coord(coord_st *coord, int turn, char *str);
void reversi_term();
void add_messages(char * m);
int reversi_available(int turn);
int reversi_score(int player);
int reversi_run(int type, int conn_fd);
void reversi_ai_move(coord_st *, int turn);