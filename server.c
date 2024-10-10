#include"header_file.c"

int main(){

        int sd,nsd,sz;
        struct sockaddr_in server,client;
        char buf[80];
        sd=socket(AF_UNIX,SOCK_STREAM,0);

        server.sin_family=AF_UNIX;
        server.sin_addr.s_addr=INADDR_ANY;
        server.sin_port=htons(PORT);

        bind(sd,(void*)(&server),sizeof(server));
        listen(sd,5);
        sz=sizeof(client);

        while(1){
                nsd=accept(sd,(void*)(&client),&sz);
                if(!(fork())){
                        close(sd);
                        read(nsd,buf,sizeof (buf));
                        printf("Message from Client: %s\n",buf);
                        write(nsd,"This is Server\n",14);
                        exit(0);
                }
                else 
                        close(nsd);
        }

        return 0;
}
