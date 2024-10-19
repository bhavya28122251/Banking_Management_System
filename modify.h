bool modify_employee(int cd) {
	struct Employee data_new;
    	char empid[10];
    	char format[300];
    	char enter_id[] = "-----Update Employee Data-----\nEnter ID of the employee whose data needs to be changed:";

    	write(cd, enter_id, sizeof(enter_id));
    	ssize_t bytes_id = read(cd, empid, sizeof(empid));
    	if (bytes_id == -1) {
        	perror("Error in receiving Employee ID");
        	return false;
    	}
    	empid[bytes_id] = '\0';  
    	printf("Received Employee ID: %s\n", empid);

    	int db_fd = open("employee.txt", O_RDWR);
    	if (db_fd == -1) {
        	perror("Error in opening the database file");
        	return false;
    	}

    	char line[300];
    	struct Employee temp;
    	bool is_there = false;

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

            	if (strcmp(temp.id, empid) == 0) {
                	printf("Employee ID matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                   	 perror("Error in obtaining lock");
                    	close(db_fd);
                    	return false;
                }

                write(cd, "Name:", strlen("Name:"));
                read(cd, data_new.name, sizeof(data_new.name));

                 write(cd, "ID:", strlen("ID:"));
		ssize_t data_read = read(cd,data_new.id,sizeof(data_new.id));

        	if (data_read <= 0)
        	{
                	close(cd);
                	exit(1);
        	}

        	if(data_new.id[data_read-1]=='\n')
                	data_new.id[data_read-1]='\0';
        	else    
                	data_new.id[data_read]='\0';

		
                snprintf(format, sizeof(format), "%s,%s,%s,%d\n", data_new.id, data_new.name, temp.pass, temp.is_empl);

                lseek(db_fd, current_position - strlen(line) - 1, SEEK_SET);

                if (write(db_fd, format, strlen(format)) == -1) {
                    	write(cd, "Error in Updating Data", strlen("Error in Updating Data"));
                    	close(db_fd);
                    	return false;
                }

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                    	perror("Error in releasing the lock");
                    	close(db_fd);
                    	return false;
                }

                is_there = true;
                break;
            }
        }
    }

    	close(db_fd);

	if (!is_there) {
        	write(cd, "Employee Not Found", strlen("Employee Not Found"));
        	return false;
    	}

    	return true;
}

bool manage_user_roles(int cd){
	struct Employee data_new;
        char empid[10];
        char format[300];
        char enter_id[] = "-----Manage User Role-----\nEnter ID of the employee ormanager whose role needs to be changed:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, empid, sizeof(empid));
        if (bytes_id == -1) {
                perror("Error in receiving Employee ID");
                return false;
        }
        empid[bytes_id] = '\0';  
        printf("Received Employee ID: %s\n", empid);

        int db_fd = open("employee.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Employee temp;
        bool is_there = false;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

        off_t record_offset = 0;
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

                if (strcmp(temp.id, empid) == 0) {
                        printf("Employee ID matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		data_new.is_empl= 1-temp.is_empl;

                snprintf(format, sizeof(format), "%s,%s,%s,%d\n", temp.id, temp.name, temp.pass, data_new.is_empl);

                lseek(db_fd, current_position - strlen(line) - 1, SEEK_SET);

                if (write(db_fd, format, strlen(format)) == -1) {
                        write(cd, "Error in Updating Data", strlen("Error in Updating Data"));
                        close(db_fd);
                        return false;
                }

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }

                is_there = true;
                break;
            }
        }
    }

        close(db_fd);

        if (!is_there) {
                write(cd, "Employee Not Found", strlen("Employee Not Found"));
                return false;
        }

        return true;
}


