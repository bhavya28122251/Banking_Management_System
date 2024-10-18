#ifndef ADMIN_OPERATIONS
#define ADMIN_OPERATIONS

//#include"employee.h"
//#include"manager.h"
//#include"customer.h"

void admin_login(int cd);
void admin_options(int cd);
bool add_employee(int cd);
/*struct User_Cred{
	char admin_name[100];
	char admin_pass[100];
};
struct User_Cred Admin_Cred={"Bhavya32","MT32"};

bool admin_login(int cd){
	char admin_name[100];
	char admin_pass[100];
	
	char prompt1[]="Admin name:\n";
	write(cd,prompt1,strlen(prompt1));

	ssize_t data_read = read(cd,admin_name,sizeof(admin_name));

    	if (data_read <= 0)
   	{
        	close(cd);
        	return false;
    	}
	
	if(admin_name[data_read-1]=='\n')
		admin_name[data_read-1]='\0';
	else 	
		admin_name[data_read]='\0';

	char prompt2[]="Enter Password:";
        write(cd,prompt2,sizeof(prompt2));

         data_read = read(cd,admin_pass,sizeof(admin_pass));

        if (data_read <= 0)
        {
                close(cd);
                return false;
        }

        if(admin_pass[data_read-1]=='\n')
                admin_pass[data_read-1]='\0';
        else    
                admin_pass[data_read]='\0';

	if((strcmp(admin_name,Admin_Cred.admin_name)==0) && (strcmp(admin_pass,Admin_Cred.admin_pass)==0)){
		const char* success="Login Successful\n";
		write(cd,success,strlen(success));
		printf("Successful");
	}
	else{
                const char* fail="Login Failure\n";
                write(cd,fail,strlen(fail));
                printf("Fail");
		close(cd);

        }

		
}*/
#endif
