#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#include <ncurses.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <linux/socket.h>
#include <arpa/inet.h>
#include "net.h"

int listen_at_port (int portnum) 
{
	int sock_fd = socket(AF_INET /*IPv4*/, SOCK_STREAM /*TCP*/, 0 /*IP*/) ;
	if (sock_fd == 0)  { 
		perror("socket failed : "); 
		exit(EXIT_FAILURE); 
	}
	int opt = 2 ;
	if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
		fprintf(stderr, "fail at setsockopt\n") ;
		exit(EXIT_FAILURE) ;
	}

	struct sockaddr_in address ; 
	bzero(&address, sizeof(address)) ; 	
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY /* localhost */ ; 
	address.sin_port = htons(portnum); 

	if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed: "); 
		exit(EXIT_FAILURE); 
	} 

	if (listen(sock_fd, 16 /* the size of waiting queue*/) < 0) { 
		perror("listen failed : "); 
		exit(EXIT_FAILURE); 
	} 

	int addrlen = sizeof(address); 
	int conn_fd = accept(sock_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen) ;
	if (conn_fd < 0) {
		perror("accept failed: "); 
		exit(EXIT_FAILURE); 
	}
	return conn_fd ;
}

void host_chat (int conn_fd) 
{
	char buf[256] ;
    getchar();

	do {
		int s ;
		
		while ( (s = recv(conn_fd, buf, 255, 0)) == 0 ) ;
		if (s == -1)
			break ;
		buf[s] = '\0' ;
		printf(">%s\n", buf) ;
        fflush(stdout);
		
		fgets(buf, 256, stdin) ;
		buf[strlen(buf) - 1] = '\0' ;
		if (strcmp(buf, "quit()") == 0)
			break ;

		send(conn_fd, buf, strlen(buf), 0) ;

	} while (strcmp(buf, "quit()") != 0) ;
}

int host (char * input) 
{	
	int conn_fd = listen_at_port(atoi(input)) ;
    printf("connected\n");
    fflush(stdout);
	return conn_fd;
} 

int connect_ipaddr_port (const char * ip, int port)
{
	int sock_fd ;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0) ;
	if (sock_fd <= 0) {
		perror("socket failed : ") ;
		exit(EXIT_FAILURE) ;
	}
	int opt = 2 ;
	if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
		fprintf(stderr, "fail at setsockopt\n") ;
		exit(EXIT_FAILURE) ;
	}

	struct sockaddr_in address ;
	bzero(&address, sizeof(address)) ;
	address.sin_family = AF_INET ; 
	address.sin_port = htons(port) ; 
	if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
		perror("inet_pton failed : ") ; 
		exit(EXIT_FAILURE) ;
	} 

	if (connect(sock_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("connect failed : ") ;
		exit(EXIT_FAILURE) ;
	}
	return sock_fd ;
}


void client_chat (int conn_fd)
{
	char buf[256] ;

	do {
        getchar();
		fgets(buf, 256, stdin) ;
		buf[strlen(buf) - 1] = '\0' ;
		if (strcmp(buf, "quit()") == 0)
			break ;
        printf("sending %s\n", buf);
        fflush(stdout);
		send(conn_fd, buf, strlen(buf), 0) ;

		int s ;
		while ((s = recv(conn_fd, buf, 1024, 0)) == 0) ;
		if (s == -1)
			break ;
		buf[s] = '\0' ;

		printf(">%s\n", buf) ;
        fflush(stdout);
	} while (strcmp(buf, "quit()") != 0) ;
}

int client (char (*input)[128])
{ 
	struct sockaddr_in serv_addr; 
	int sock_fd ;
	int s, len ;
	char buffer[1024] = {0}; 
	char * data ;

	int conn_fd = connect_ipaddr_port(input[0], atoi(input[1])) ;

	return conn_fd ;
} 

int recv_input(void * t, int size, int conn_fd){
	
	int r = 0;
	r = recv(conn_fd, t, size, 0);
	timeout(0);
	if(r==0){
		return 0;
	}
	else{
		return 1;
	}
}