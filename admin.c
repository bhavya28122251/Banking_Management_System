#include"header_file.h"
#include"admin.h"

struct User_Cred{
        char admin_name[100];
        char admin_pass[100];
};
struct User_Cred Admin_Cred={"b","m"};

void admin_login(int cd){
	printf("CD in admin %d\n",cd);
        char admin_name[100];
        char admin_pass[100];

        const char* prompt1="-----Admin Login-----\nAdmin name:";
        if(( write(cd,prompt1,strlen(prompt1)))==-1){
		perror("Error");
	}

        ssize_t data_read = read(cd,admin_name,sizeof(admin_name));

        if (data_read <= 0)
        {
                close(cd);
                exit(1);
        }

        if(admin_name[data_read-1]=='\n')
                admin_name[data_read-1]='\0';
        else    
                admin_name[data_read]='\0';

        const char * prompt2="Enter Password:";
        write(cd,prompt2,strlen(prompt2));

        ssize_t data_read2 = read(cd,admin_pass,sizeof(admin_pass));

        if (data_read2 <= 0)
        {
                close(cd);
                exit(1);
        }

        if(admin_pass[data_read2-1]=='\n')
                admin_pass[data_read2-1]='\0';
        else    
                admin_pass[data_read2]='\0';

        if((strcmp(admin_name,Admin_Cred.admin_name)==0) && (strcmp(admin_pass,Admin_Cred.admin_pass)==0)){
                const char* success="Login Successful\n";
                write(cd,success,strlen(success));
                admin_options(cd);
        }
        else{
                const char* fail="Login Failure\n";
                write(cd,fail,strlen(fail));
                printf("Fail");
                close(cd);
		

        }

}

void admin_options(int cd){
	//printf("Helllllooooo\n");
	
	ssize_t bytes_read,bytes_write;
	char buf_read[100],buf_write[100];
	

	while(1){
		char admin_menu[]="Choose from the given options:\n1.Add New Bank Emplyee\n2.Modify Customer Details\n3.Modify Employee Setails\n4.Manage User Roles\n5.Change Password\n6.Logout\n7.Exit";
		write(cd,admin_menu,sizeof(admin_menu));
		
		char ch_str[20];
        	 bytes_read = read(cd, ch_str, sizeof(ch_str)-1);  
       		 if (bytes_read > 0) {
                	ch_str[bytes_read] = '\0'; 
                	for (int i = 0; i < bytes_read; i++) {
                        	if (ch_str[i] == '\n' || ch_str[i] == '\r') {
                                	ch_str[i] = '\0';
                                	break;
                        	}
                	}
		} 
                printf("1Choice is %s\n", ch_str);
		int ch=atoi(ch_str);
		switch(ch){
			case 1:
				if(1){
					write(cd,"New Bank Employee added Successfully\n",strlen("New Bank Employee added Successfully\n"));
        			}
				break;
			case 2:
				if(1){
					write(cd,"Customer Details Modified\n",strlen("Customer Details Modified\n"));
				}
				break;
			case 3:
                                if(1){
                                        write(cd,"Employee Details Modified\n",strlen("Employee Details Modified\n"));
                                }
                                break;
			case 4:
                                if(1){
                                        write(cd,"User Roles Managed\n",strlen("User Roles Managed\n"));
                                }
                                break;
			case 5:
                                if(1){
                                        write(cd,"Password has Changed\n",strlen("Password has Changed\n"));
                                }
                                break;
			case 6:
                                if(1){
                                        write(cd,"Logged Out\n",strlen("Logged Out\n"));
                                }
                                break;
			case 7:
                                if(1){
                                        write(cd,"Exit\n",strlen("Exit\n"));
                                }
                                break;
			default:
				write(cd,"Wrong Input\nPlease enter a number from 1-7\n",strlen("Wrong Input\nPlease enter a number from 1-7\n"));
				break;
		}
	}

}
