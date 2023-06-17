#include <stdio.h>
#include <string.h>
#include "reversi.h"
/*
int g_mod;
int g_x;
int g_y;
char messages[7][51];
int board[8][8];*/
int type;

int main(int argc, char **argv)
{
    int con;
    if (argc == 3)
    {
        if (strcmp(argv[1],"-server")==0){

            con = host(argv[2]);
            type = 1;
            reversi_run(con);
        }
        else printf("ERROR!\n");
    }
    else if (argc == 4)
    {
        if (strcmp(argv[1],"-client")==0){
            type = 2;
            char input[2][128];
            strcpy(input[0],argv[2]);
            strcpy(input[1],argv[3]);
            con = client(input);
            reversi_run(con);
        }
        else printf("ERROR!\n");
    }
    else if (argc == 1)
    {
        
        printf("Welcom! Choose mod(Host: 1, Client: 2, Single-player: 3, local-multi: 4, 0-player 5 :");
        scanf("%d", &type);
        if (type == 1)
        {
            char input[128];
            printf("What is your port number? ");
            fflush(stdout);
            scanf("%s", input);
            con = host(input);
            reversi_run(con);
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
            reversi_run(con);
        }
        else if (type == 3)
        {
            reversi_run(0);
        }
        else if (type == 4){
            reversi_run(0);
        }
        else if(type ==5){
            reversi_run(0);
        }
        else printf("ERROR!\n");
    }
    else{
        printf("ERROR!\n");
    }
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