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

bool employee_login(int cd){
	char empl_id[100];
        char empl_pass[100];
	
	const char* prompt1="-----Employee Login-----\nEmployee ID:";
        if(( write(cd,prompt1,strlen(prompt1)))==-1){
                perror("Error");
        }

        ssize_t data_read = read(cd,empl_id,sizeof(empl_id));

        if (data_read <= 0)
        {
                close(cd);
                exit(1);
        }

        if(empl_id[data_read-1]=='\n')
                empl_id[data_read-1]='\0';
        else    
                empl_id[data_read]='\0';

        const char * prompt2="Enter Password:";
        write(cd,prompt2,strlen(prompt2));

        ssize_t data_read2 = read(cd,empl_pass,sizeof(empl_pass));

        if (data_read2 <= 0)
        {
                close(cd);
                exit(1);
        }

        if(empl_pass[data_read2-1]=='\n')
                empl_pass[data_read2-1]='\0';
        else    
                empl_pass[data_read2]='\0';

	 int db_fd = open("employee.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

	printf("Input:%s %s ",empl_id,empl_pass);
	char line[300];
        struct Employee temp;


        char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

                sscanf(line, "%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass, &temp.is_empl);
                printf("Read Employee: ID=%s, Name=%s, Password=%s, Is Employed=%d\n", temp.id, temp.name, temp.pass, temp.is_empl);
		
		printf("%s %s %s %s",temp.id,empl_id,temp.pass,empl_pass);
                if (strcmp(temp.id, empl_id) == 0 && strcmp(temp.pass,empl_pass)==0) {
                        printf("Employee ID and password matched.\n");
			write(cd,"Login Successfull",strlen("Login Successfull"));
			employee_options(cd);
			return true;
                	break;
            	}
        }}
	write(cd,"Login Failure",strlen("Login Failure"));
	return false;
 }

