#define CHAT 1
#define MOVE 2

int listen_at_port (int portnum);
void host_chat (int conn_fd) ;
int host (char * input) ;
int connect_ipaddr_port (const char * ip, int port);
void client_chat (int conn_fd);
int client (char (*input)[128]);
int recv_input(void * t, int size, int conn_fd);