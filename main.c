#include <stdio.h>
#include "reversi.h"
#include "net.h"

int g_mod;
int g_x;
int g_y;
char messages[7][51];
int board[8][8];

int main(int argc, char **argv)
{   
    int type;
    int con;
    printf("Welcom! Choose mod(Host: 1, Client: 2, Single-player: 3, local-multi: 4) : ");
    scanf("%d", &type);
    if(type == 1){
        char input[128];
        printf("What is your port number? ");
        fflush(stdout);
        scanf("%s", input);
        con = host(input);
    }
    else if (type == 2)
    {
        char input[2][128];
        printf("Enter IP you wish to connect \n");
        fflush(stdout);
        scanf("%s", input[0]);
        printf("%s", input[0]);
        printf("What is your port number? ");
        fflush(stdout);
        scanf("%s", input[1]);
        con = client(input);
    }
    else{
        printf("ERROR!");
    }
    reversi_run(type, con);
    return 0;
}

/*
char input[2][128];
        printf("Enter IP you wish to connect \n");
        fflush(stdout);
        scanf(input[0], 127, stdin);
        printf("What is your port number? ");
        fflush(stdout);
        fgets(input[1], 127, stdin);
        client(input);
        */