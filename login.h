#include "header_file.h"

bool admin_login(int cd){
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

       /* if((strcmp(admin_name,Admin_Cred.admin_name)==0) && (strcmp(admin_pass,Admin_Cred.admin_pass)==0)){
                const char* success="Login Successful\n";
                write(cd,success,strlen(success));
                admin_options(cd);
        }
        else{
                const char* fail="Login Failure\n";
                write(cd,fail,strlen(fail));
                printf("Fail");
                close(cd);


        }*/

	/*FILE *file = fopen("admin_login.txt", "l");
	if (file == NULL) {
		perror("Error in opening the file");
		exit(EXIT_FAILURE);
	}

    	fprintf(file, "%s %s\n", admin_name, admin_pass);
    	fclose(file);
	*/
	 FILE *file = fopen("admin_login.txt", "r");
	 if (file == NULL) {
        	perror("Error in opening file");
        	return false;
    	}

	char stored_name[100];
	char stored_pass[10];
	while(fscanf(file,"%s %s",stored_name,stored_pass)!=EOF){
		if(strcmp(stored_name,admin_name)==0 && strcmp(stored_pass,admin_pass)==0){
			write(cd,"Login Successful",strlen("Login Successful"));
			fclose(file);
			admin_options(cd);
			return true;
		}
	}
	fclose(file);
	write(cd,"Invalid Login Id or Password",strlen("Invalid Login Id or Password"));
	return false;
	


}

bool change_admin_pass(int cd){
	char new_pass[10];
	write(cd,"Enter New Password: ",strlen("Enter New Password: "));
	ssize_t data_read = read(cd,new_pass,sizeof(new_pass));

        if (data_read <= 0)
        {
                close(cd);
                return false;
        }

        if(new_pass[data_read-1]=='\n')
                new_pass[data_read-1]='\0';
        else    
                new_pass[data_read]='\0';
	
	FILE *file = fopen("admin_login.txt", "w");
    	if (file == NULL) {
        	perror("Error in opening file");
        	return false;
    	}
	 	
	fprintf(file,"%s %s\n","b",new_pass);
	fclose(file);
	printf("New pass %s",new_pass);
	return true;
}	


