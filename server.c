#include"header_file.h"
//#include"admin.h"
#include "add.h"
#include "login.h"
#include "menu.h"
#include "modify.h"
#include "display.h"

void handle_client(int cd);
bool admin_login(int cd);

int main(){
        struct sockaddr_in server_addr,client_addr;
        int sd,cd;
        //char buf[1000],menu[1000],login_type[100];
        sd=socket(AF_INET,SOCK_STREAM,0);
	if(sd==-1){
		perror("Error in creating Socket\n");
		exit(1);
	}
	
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=INADDR_ANY;
        server_addr.sin_port=htons(PORT);

        if((bind(sd,(void*)(&server_addr),sizeof(server_addr)))==-1){
		perror("Error in binding\n");
		exit(0);
	}

        if(listen(sd,5)==-1){
		perror("Error in listen");
		exit(1);
	}

	socklen_t sz_client=sizeof(client_addr);

        while(1){
		
                cd=accept(sd,(void*)(&client_addr),&sz_client);
		printf("CD in server %d",cd);
		if(cd==-1){
			perror("Error in accept\n");
			exit(1);
		}
		                
		if(!(fork())){
			close(sd);
			//read(cd,buf,sizeof(buf));
			//printf("Hiii %s",buf);
                  	write(cd,"Welcome to Joshi's Bank",23);
			handle_client(cd);
			write(cd,"Connection Terminated",21);                 
                        close(cd);
			exit(0);
                }
		else {
          
            		close(cd);  
        	}
                  	
        }
	
	close(sd);

        return 0;
}

void handle_client(int cd){

	while(1){	
	char menu[]="Choose from options 1-4\n1.Customer\n2.Employee\n3.Manager\n4.Admin";
	write(cd,menu,strlen(menu));
	
	char ch_str[20];
   	int bytes_read = read(cd, ch_str, sizeof(ch_str)-1);  
    	if (bytes_read > 0) {
        	ch_str[bytes_read] = '\0'; 
		for (int i = 0; i < bytes_read; i++) {
            		if (ch_str[i] == '\n' || ch_str[i] == '\r') {
                		ch_str[i] = '\0';
                		break;
            		}
        	} 
        	//printf("1Choice is %s\n", ch_str);
        }

	else {
        	printf("Error reading from client.\n");
    	}


	if (strstr(ch_str, "4") !=NULL) {
        	//printf("Admin in server");
		admin_login(cd);
		
			
    	}
	/* else if (strstr(ch_str, "2") !=NULL) {
        	write(cd, "employee_login(int cd)\n", 23);
    	} 
	else if (strstr(ch_str, "3") !=NULL) {
        	write(cd, "manager_login(int cd)\n", 22);
    	} 
	else if (strstr(ch_str, "4") !=NULL) {
        	write(cd, "admin_login(int cd)\n", 20);
    	} 
	else {
        	write(cd, "Choose a number from 1 to 4\n", 27);
    }*/
	}
}

	
