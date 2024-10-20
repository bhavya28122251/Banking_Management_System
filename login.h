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


bool employee_login(int cd) {
	
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

        char line[300];
        struct Employee temp;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

        //off_t record_offset = 0;
        off_t current_position = 0;

         char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);

                int is_empl_int;
                sscanf(line, "%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass, &is_empl_int);
                temp.is_empl = (is_empl_int != 0); 
                printf("Read Employee: ID=%s, Name=%s, Password=%s, Is Employed=%d\n", temp.id, temp.name, temp.pass, temp.is_empl);

                if (strcmp(temp.id, empl_id) == 0 && strcmp(temp.pass,empl_pass)==0) {
                        printf("Employee ID matched and Password Match.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }
		
		write(cd,"Login Successfull",strlen("Login Successfull"));
		employee_options(cd);		

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }

                return true;
                break;
		
            }
        }}
	write(cd,"Login Failure",strlen("Login Failure"));
	return false;
    
}


bool change_employee_pass(int cd) {
	char format[300];
        char empl_id[100];
        char empl_pass[100];
	struct Employee data_new;
        const char* prompt1="-----Change Password-----\nEmployee ID:";
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

        const char * prompt2="Enter Existing Password:";
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

        char line[300];
        struct Employee temp;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

        //off_t record_offset = 0;
        off_t current_position = 0;

         char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);

                int is_empl_int;
                sscanf(line, "%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass, &is_empl_int);
                temp.is_empl = (is_empl_int != 0); 
                printf("Read Employee: ID=%s, Name=%s, Password=%s, Is Employed=%d\n", temp.id, temp.name, temp.pass, temp.is_empl);

                if (strcmp(temp.id, empl_id) == 0 && strcmp(temp.pass,empl_pass)==0) {
                        printf("Employee ID matched and Password Matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		write(cd, "New Password:", strlen("New Password:"));
                ssize_t data_read = read(cd,data_new.pass,sizeof(data_new.pass));

                if (data_read <= 0)
                {
                        close(cd);
                        exit(1);
                }

                if(data_new.id[data_read-1]=='\n')
                        data_new.pass[data_read-1]='\0';
                else    
                        data_new.pass[data_read]='\0';


                snprintf(format, sizeof(format), "%s,%s,%s,%d\n", temp.id, temp.name, data_new.pass, temp.is_empl);

                lseek(db_fd, current_position - strlen(line) - 1, SEEK_SET);

                if (write(db_fd, format, strlen(format)) == -1) {
                        write(cd, "Error in Updating Data", strlen("Error in Updating Data"));
                        close(db_fd);
                        return false;
                }

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }

                return true;
                break;

            }
        }}
        write(cd,"Pasword Not Changed",strlen("Pasword Not Changed"));
        return false;
    
}

bool customer_login(int cd){
	char cust_id[100];
        char cust_pass[100];

        const char* prompt1="-----Customer Login-----\nCustomer ID:";
        if(( write(cd,prompt1,strlen(prompt1)))==-1){
                perror("Error");
        }

        ssize_t data_read = read(cd,cust_id,sizeof(cust_id));

        if (data_read <= 0)
        {
                close(cd);
                exit(1);
        }

        if(cust_id[data_read-1]=='\n')
                cust_id[data_read-1]='\0';
        else    
                cust_id[data_read]='\0';

        const char * prompt2="Enter Password:";
        write(cd,prompt2,strlen(prompt2));

        ssize_t data_read2 = read(cd,cust_pass,sizeof(cust_pass));

        if (data_read2 <= 0)
        {
                close(cd);
                exit(1);
        }

        if(cust_pass[data_read2-1]=='\n')
                cust_pass[data_read2-1]='\0';
        else    
                cust_pass[data_read2]='\0';

        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Customer temp;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

        //off_t record_offset = 0;
        off_t current_position = 0;

         char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);

                int active_int;
                sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass, temp.balance,&active_int);
                temp.active = (active_int != 0); 
               // printf("Read Employee: ID=%s, Name=%s, Password=%s, Is Employed=%d\n", temp.id, temp.name, temp.pass, temp.is_empl);

                if (strcmp(temp.id, cust_id) == 0 && strcmp(temp.pass,cust_pass)==0 && temp.active==1) {
                        printf("CUstomer ID matched and Password Match.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

                write(cd,"Login Successfull",strlen("Login Successfull"));
                customer_options(cd);           

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }

                return true;
                break;

            }
        }}
        write(cd,"Login Failure",strlen("Login Failure"));
        return false;

}

bool change_customer_pass(int cd){
   	char format[300];
        char cust_id[100];
        char cust_pass[100];
        struct Customer data_new;
        const char* prompt1="-----Change Password-----\nCustomer ID:";
        if(( write(cd,prompt1,strlen(prompt1)))==-1){
                perror("Error");
        }

        ssize_t data_read = read(cd,cust_id,sizeof(cust_id));

        if (data_read <= 0)
        {
                close(cd);
                exit(1);
        }

        if(cust_id[data_read-1]=='\n')
                cust_id[data_read-1]='\0';
        else    
                cust_id[data_read]='\0';

        const char * prompt2="Enter Existing Password:";
        write(cd,prompt2,strlen(prompt2));

        ssize_t data_read2 = read(cd,cust_pass,sizeof(cust_pass));

        if (data_read2 <= 0)
        {
                close(cd);
                exit(1);
        }

        if(cust_pass[data_read2-1]=='\n')
                cust_pass[data_read2-1]='\0';
        else    
                cust_pass[data_read2]='\0';

        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Customer temp;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

        //off_t record_offset = 0;
        off_t current_position = 0;

         char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);

                int active_int;
                sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass,temp.balance, &active_int);
                temp.active = (active_int != 0); 

                if (strcmp(temp.id, cust_id) == 0 && strcmp(temp.pass,cust_pass)==0) {

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

                write(cd, "New Password:", strlen("New Password:"));
                ssize_t data_read = read(cd,data_new.pass,sizeof(data_new.pass));

                if (data_read <= 0)
                {
                        close(cd);
                        exit(1);
                }

                if(data_new.id[data_read-1]=='\n')
                        data_new.pass[data_read-1]='\0';
                else    
                        data_new.pass[data_read]='\0';


                snprintf(format, sizeof(format), "%s,%s,%s,%s,%d\n", temp.id, temp.name, data_new.pass, temp.balance,temp.acitve);

                lseek(db_fd, current_position - strlen(line) - 1, SEEK_SET);

                if (write(db_fd, format, strlen(format)) == -1) {
                        write(cd, "Error in Updating Data", strlen("Error in Updating Data"));
                        close(db_fd);
                        return false;
                }

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }

                return true;
                break;

            }
        }}
        write(cd,"Pasword Not Changed",strlen("Pasword Not Changed"));
        return false;
}

bool manager_login(int cd) {

        char man_id[100];
        char man_pass[100];

        const char* prompt1="-----Manager Login-----\nManager ID:";
        if(( write(cd,prompt1,strlen(prompt1)))==-1){
                perror("Error");
        }

        ssize_t data_read = read(cd,man_id,sizeof(man_id));

        if (data_read <= 0)
        {
                close(cd);
                exit(1);
        }

        if(man_id[data_read-1]=='\n')
                man_id[data_read-1]='\0';
        else    
                man_id[data_read]='\0';

        const char * prompt2="Enter Password:";
        write(cd,prompt2,strlen(prompt2));

        ssize_t data_read2 = read(cd,man_pass,sizeof(man_pass));

        if (data_read2 <= 0)
        {
                close(cd);
                exit(1);
        }

        if(man_pass[data_read2-1]=='\n')
                man_pass[data_read2-1]='\0';
        else    
                man_pass[data_read2]='\0';

        int db_fd = open("employee.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Employee temp;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

        //off_t record_offset = 0;
        off_t current_position = 0;

         char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);

                int is_empl_int;
                sscanf(line, "%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass, &is_empl_int);
                temp.is_empl = (is_empl_int != 0); 
                printf("Read Employee: ID=%s, Name=%s, Password=%s, Is Employed=%d\n", temp.id, temp.name, temp.pass, temp.is_empl);

                if (strcmp(temp.id, man_id) == 0 && strcmp(temp.pass,man_pass)==0) {
                        printf("Manager ID matched and Password Match.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

                write(cd,"Login Successfull",strlen("Login Successfull"));
                manager_options(cd);           

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }

                return true;
                break;

            }
        }}
        write(cd,"Login Failure",strlen("Login Failure"));
        return false;
    
}
