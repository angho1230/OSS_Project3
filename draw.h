#define BLACK 1
#define WHITE 2
#define START_X 4
#define START_Y 3
#define PADDING_X 6
#define PADDING_Y 3
#define BLACK 1
#define WHITE 2
#define BG 3
#define TEXT_COL 4
#define OUTLINE 5
#define BLANK 6

void rectangle(int y1, int x1, int y2, int x2);
void rectangle_color(int y1, int x1, int y2, int x2, int color);
void draw_table();
void message_color(char *m, int color);
void message(char *m);
char key_input(int print);
void print_messages(char (*messages)[80]);
void score_print(char * name1, char * name2, int score1, int score2);