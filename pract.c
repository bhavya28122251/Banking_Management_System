#include "header_file.c"
int main(){
        struct sockaddr_in server;
        int sd;
        char buf[80];
        sd=socket(AF_UNIX,SOCK_STREAM,0);

        server.sin_family=AF_UNIX;
        server.sin_addr.s_addr=INADDR_ANY;
        server.sin_port=htons(PORT);

        connect(sd,(void*)(&server),sizeof(server));
        write(sd,"This is Client\n",14);
        sleep(10);
	read(sd,buf,sizeof(buf));
        printf("Message from Server: %s\n",buf);

        return 0;
}
