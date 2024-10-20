# include "header_file.h"

bool apply_loan(int cd){
	 struct Loan l;

        char buf_id[]="-----Applying for Loan-----\nCustomer ID:";
        write(cd,buf_id,sizeof(buf_id));

        int bytes_id=read(cd,l.cust_id,sizeof(l.cust_id));
        if(bytes_id<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        l.cust_id[bytes_id]='\0';
        if(l.cust_id[bytes_id-1]=='\n'){
                l.cust_id[bytes_id-1]='\0';
        }

	
	l.status=0;
	//strcmp(l.empl_id,"EEEEE");

        FILE *file=fopen("loan_db.txt","a");
        if(file!=NULL){
                fprintf(file,"%s,%d,%s\n",l.cust_id,l.status,"EEEEE");
                fclose(file);
                printf("true\n");
                fflush(stdout);
                return true;
        }
        perror("Error in opening Loan file");
        printf("false\n");
        fflush(stdout);
        return false;

}

bool assign_loan(int cd){
	struct Loan data_new;
        char custid[10];
        char format[300];
        char enter_id[] = "-----Assign Loan Application Processes to Employee-----\nEnter ID of the Customer whose Loan Application needs to be assigned to an Employee:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, custid, sizeof(custid));
        if (bytes_id == -1) {
                perror("Error in receiving Customer ID");
                return false;
        }
        custid[bytes_id] = '\0';  
        printf("Received Customer ID: %s\n", custid);

        int db_fd = open("loan_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Loan temp;
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

                sscanf(line, "%[^,],%d,%[^,]", temp.cust_id,&temp.status, temp.empl_id);
               // printf("Read Customer: ID=%s, Name=%s, Password=%s, Is Employed=%d\n", temp.id, temp.name, temp.pass, temp.is_empl);

                if (strcmp(temp.cust_id, custid) == 0) {
                        printf("Customer ID matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

                write(cd, "Enter Employee ID:", strlen("Enter Employee ID:"));
                ssize_t data_read1=read(cd, data_new.empl_id, sizeof(data_new.empl_id));
                if (data_read1 <= 0)
                {
                        close(cd);
                        exit(1);
                }

                if(data_new.empl_id[data_read1-1]=='\n')
                        data_new.empl_id[data_read1-1]='\0';
                else    
                        data_new.empl_id[data_read1]='\0';


		data_new.status=1;//Means empl is assigned
		snprintf(format, sizeof(format), "%s,%d,%s\n", temp.cust_id, data_new.status, data_new.empl_id);

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
                write(cd, "Customer Not Found", strlen("Employee Not Found"));
                return false;
        }

        return true;

}
