#include "header_file.h"
int main(){
        struct sockaddr_in server;
        int sd;
        char buf[1000],menu[1000],login_type[100];
        sd=socket(AF_INET,SOCK_STREAM,0);

        server.sin_family=AF_INET;
        server.sin_addr.s_addr=inet_addr("127.0.0.1");
        server.sin_port=htons(PORT);

        if((connect(sd,(void*)(&server),sizeof(server)))==-1){
		perror("Error");
	}

	/*read(sd,buf,sizeof(buf));
        printf("Message from Server: %s\n",buf);

	char ch_str[4];
	scanf("%s",ch_str);
	if((write(sd,ch_str,strlen(ch_str)))==-1)
		perror("Error");
	read(sd,login_type,sizeof(login_type));
	printf("%s",login_type);
	
		char prompt1[100], send_data[100];
		if((read(sd,prompt1,strlen(prompt1)))==-1){
			perror("Error");
		}
		printf("%s",prompt1);
		scanf("%s",send_data);
		write(sd,send_data,strlen(send_data));
		/* char prompt2[100], send_data2[100];
                read(sd,prompt2,strlen(prompt2));
                scanf("%s",send_data2);
                write(sd,send_data,strlen(send_data2));*/
	
while(1)
{
        char buff[1024];
        memset (buff,0,sizeof(buff));
        ssize_t read_data=read(sd,buff,sizeof(buff));
        if(read_data<=0)
                break;
        printf("%s\n",buff);
	char buffer[1024];
	scanf("%s",buffer);
	write (sd,buffer,strlen(buffer));

}
close(sd);
return 0;
}
