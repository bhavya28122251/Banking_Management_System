#include "header_file.h"

bool view_balance(int cd){
 	char cust_id[100];

        const char* prompt1="-----View Account Balance-----\nCustomer ID:";
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

        int db_fd = open("customer_db.txt", O_RDONLY);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Customer temp;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_RDLCK;  
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

		if(strcmp(temp.id,cust_id)==0){

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }
		
		write(cd,"Balance:",strlen("Balance:"));
		write(cd,temp.balance,sizeof(temp.balance));

                lock.l_type = F_UNLCK;
                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd);
                        return false;
                }
		printf("view balance\n");
                return true;
                break;

            }
        }}
        write(cd,"Failure",strlen("Failure"));
        return false;

return true;
}


bool deposit_money(int cd){
/*
	
	struct Transaction add;
	write(cd,"Enter sender ID\n",strlen("Enter sender ID\n"));
	int read1=read(cd,add.sender_id,sizeof(add.sender_id));
	add.sender_id[read1 -1]='\0';
	
	write(cd,"enter receiver\n",strlen("enter receiver\n"));
 	read1=read(cd,add.rec_id,sizeof(add.rec_id));
	if(read1<=0){
	send(cd,"error in receiver\n",strlen("error in receiver\n"),0);
	return false;}
	add.rec_id[read1-1]='\0';

	write(cd,"enter amount\n",strlen("enter amount\n"));
	read1=read(cd,add.sent,sizeof(add.sent));
	int amount=atoi(add.sent);
	if(read1<=0)
	{send(cd,"error in amount\n",strlen("error in amount\n"),0);
	return false;
	}
	add.sent[read1-1]='\0';

	strcpy(add.flag, "1");

	FILE *file=fopen("transaction_db.txt","a");
	if(file!=NULL)
	{
	fprintf(file,"%s,%s,%s,%s\n",add.sender_id,add.rec_id,add.sent,add.flag);
	fclose(file);
	printf("true\n");
	fflush(stdout);
	}
	else{
	perror("Can not open employee file");
	printf("false\n");
	fflush(stdout);
	return false;
}

	struct Customer data_new;
        char cid[10];
        char format[300];
        char enter_id[] = "Enter Sender id:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, cid, sizeof(cid));
        if (bytes_id == -1) {
                perror("Error in receiving customer id");
                return false;
        }
        cid[bytes_id] = '\0';  
        printf("Received customer id: %s\n", cid);

        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the customer file");
                return false;
        }

        char line[300];
        struct customer temp;
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

                int active_int;
                sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass,temp.bal, active_int);
                temp.active = (active_int != 0); 

                if (strcmp(temp.id,)==atoi( cid)) {
                        printf("customer is matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		int new_bal=amount+atoi(temp.balance);
		sprintf(data_new.balance, "%d", new_balance);
		printf("%s\n",data_new.balance);
                snprintf(format, sizeof(format), "%s,%s,%s,%s,%d\n", temp.id, temp.name, temp.pass,data_new.balance,temp.active);

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
                write(cd, "Failure", strlen("Failure"));
                return false;
        }
*/
        return true;

}

bool withdraw_money(int cd){
/*
	struct transaction add;
	write(cd,"Enter Sender ID\n",strlen("Enter sender ID\n"));
	int read1=read(cd,add.sender_id,sizeof(add.sender_id));

	add.sender_id[read1 -1]='\0';
	write(cd,"Enter Receiver ID\n",strlen("Enter Receiver ID\n"));
 	read1=read(cd,add.rec_id,sizeof(add.rec_id));
	if(read1<=0)
	{send(cd,"Error in receiving\n",strlen("Error in receiving\n"),0);
	return false;}

	add.rec_id[read1-1]='\0';
	write(cd,"Enter amount\n",strlen("Enter amount\n"));
	read1=read(cd,add.sent,sizeof(add.sent));
	int amount=atoi(add.sent);
	
	if(read1<=0)
	{send(cd,"Error in amount\n",strlen("Error in amount\n"),0);
	return false;}
	add.sent[read1-1]='\0';

	strcpy(add.flag, "0");

	FILE *file=fopen("transaction_db.txt","a");
	if(file!=NULL)
	{fprintf(file,"%s,%s,%s,%s\n",add.sender_id,add.rec_id,add.sent,add.flag);
	fclose(file);
	printf("true\n");
	fflush(stdout);
	}
	else{
	perror("Can not open database file");
	printf("false\n");
	fflush(stdout);
	return false;
	}

	struct Customer data_new;
        char cid[10];
        char format[300];
        char enter_id[] = "Enter Sender id:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, cid, sizeof(cid));
        if (bytes_id == -1) {
                perror("Error in receiving customer id");
                return false;
        }
        cid[bytes_id] = '\0';  
        printf("Received Customer id: %s\n", cid);

        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct customer temp;
        bool is_there = false;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

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

                int is_active_int;
                sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass,temp.balance, &active_int);
                temp.active = (active_int != 0); 

                if (strcmp(temp.id,cid))==0) {
                        printf("Customer is Matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in Obtaining lock");
                        close(db_fd);
                        return false;
                }

		int new_bal=atoi(temp.balance)-amount;
		sprintf(data_new.balance, "%d", new_bal);
		printf("%s\n",data_new.balance);
                snprintf(format, sizeof(format), "%s,%s,%s,%s,%d\n", temp.id, temp.name, temp.pass,data_new.balance,temp.active);

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
*/
        return true;

}


bool transfer_funds(int cd)
{
/*	struct Transaction add;
	write(cd,"Enter sender id\n",strlen("Enter sender id\n"));
	int read1=read(cd,add.sender_id,sizeof(add.sender_id));
	add.sender_id[read1 -1]='\0';

	write(cd,"Enter receiver\n",strlen("Enter receiver\n"));
	read1=read(cd,add.rec_id,sizeof(add.rec_id));

	if(read1<=0){
	write(cd,"Error in receiver\n",strlen("Error in receiver\n"));
	return false;}
	add.rec_id[read1-1]='\0';

	write(cd,"Enter amount\n",strlen("Enter amount\n"));
	read1=read(cd,add.sent,sizeof(add.sent));
	int amount=atoi(add.sent);
	printf("Amount tran %d\n",amount);
	if(read1<=0)
	{write(cd,"Error in amount\n",strlen("Error in amount\n"));
	return false;}
	add.amount[read1-1]='\0';

	strcpy(add.flag, "2");

	FILE *file=fopen("transaction_db.txt","a");
	if(file!=NULL)
	{fprintf(file,"%s,%s,%s,%s\n",add.sender_id,add.rec_id,add.sent,add.flag);
	fclose(file);
	printf("true\n");
	fflush(stdout);
	}
	else{
	perror("Can not open database file");
	printf("false\n");
	fflush(stdout);
	return false;
}

	struct Customer data_new;
        char cid[10];
        char format[300];
        char enter_id[] = "Enter Sender id:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, cid, sizeof(cid));
        if (bytes_id == -1) {
                perror("Error in receiving customer id");
                return false;
        }
        cid[bytes_id] = '\0';  
        printf("Received customer id: %s\n", cid);

        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the customer file");
                return false;
        }

        char line[300];
        struct Customer temp;
        bool is_there = false;

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;  
        lock.l_whence = SEEK_SET;  

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

                if((strcmp(temp.id, cid))==0) {
                        printf("Customer is matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		int new_bal=atoi(temp.balance)-amount;
		sprintf(data_new.balance, "%d", new_balance);
		printf("%s\n",data_new.bal);
                snprintf(format, sizeof(format), "%s,%s,%s,%s,%d\n", temp.id, temp.name, temp.pass,data_new.balance,temp.active);

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
                write(cd, "Failure", strlen("Failure"));
                return false;
        }

        struct customer data_new1;
        char cid1[10];
        char format1[300];
        char enter_id1[] = "Enter customer id:";

        write(cd, enter_id1, sizeof(enter_id1));
        ssize_t bytes_id1 = read(cd, cid1, sizeof(cid1));
        if (bytes_id1 == -1) {
                perror("Error in receiving customer id");
                return false;
        }
        cid1[bytes_id1] = '\0';  
        printf("Received customer id: %s\n", cid1);

        int db_fd1 = open("customer_db.txt", O_RDWR);
        if (db_fd1 == -1) {
                perror("Error in opening the customer file");
                return false;
        }

        char line1[300];
        struct Customer temp1;
        bool is_there1 = false;

        struct flock lock1;
        memset(&lock1, 0, sizeof(lock1));
        lock1.l_type = F_WRLCK;  
        lock1.l_whence = SEEK_SET;  

        //off_t record_offset = 0;
        off_t current_position1 = 0;

         char buffer1;
        int line_index1 = 0;

        while (read(db_fd1, &buffer1, 1) > 0) {
        if (buffer1 != '\n') {
            line1[line_index1++] = buffer1;
        }
         else {
                line1[line_index1] = '\0';
                line_index1 = 0;

                current_position1 = lseek(db_fd1, 0, SEEK_CUR);

                int active_int1;
                sscanf(line1, "%[^,],%[^,],%[^,],%[^,],%d", temp1.id, temp1.name, temp1.pass,temp1.balance, &active_int1);
                temp1.active = (active_int1 != 0); 

                if ((strcmp(temp1.id,cid1))==0) {
                        printf("customer is matched.\n");

                lock1.l_start = current_position1 - strlen(line1) - 1;  
                lock1.l_len = strlen(line1) + 1; 

                if (fcntl(db_fd1, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd1);
                        return false;
                }

		int new_bal1=atoi(temp1.balance)+amount;
		sprintf(data_new1.balance, "%d", new_bal1);
		printf("%s\n",data_new1.balance);
                snprintf(format1, sizeof(format1), "%s,%s,%s,%s,%d\n", temp1.id, temp1.name, temp1.pass,data_new1.balance,temp1.active);

                lseek(db_fd, current_position1 - strlen(line1) - 1, SEEK_SET);

                if (write(db_fd1, format1, strlen(format1)) == -1) {
                        write(cd, "Error in Updating Data", strlen("Error in Updating Data"));
                        close(db_fd1);
                        return false;
                }

                lock1.l_type = F_UNLCK;
                if (fcntl(db_fd1, F_SETLK, &lock1) == -1) {
                        perror("Error in releasing the lock");
                        close(db_fd1);
                        return false;
                }

                is_there1 = true;
                break;
            }
        }
    }

        close(db_fd1);

        if (!is_there1) {
                write(cd, "Failure", strlen("Failure"));
                return false;
        }
*/
        return true;


}


bool view_transaction_history(int cd){
/*	 struct Transaction data_new;
        char emplid[10];
        char format[300];
        char enter_id[] = "-----View Transaction History-----\nEnter Your ID:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, emplid, sizeof(emplid));
        if (bytes_id == -1) {
                perror("Error in receiving Customer ID");
                return false;
        }
        emplid[bytes_id] = '\0';  
        printf("Received Customer ID: %s\n", emplid);

        int db_fd = open("loan_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }
	 char buffer;
        int line_index = 0;
	char transaction_buffer[500];
	char line[300];
        
	struct Transaction temp;
        bool is_there = false;
	off_t current_position = 0;
	

	while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }

         else {
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);

                sscanf(line, "%[^,],%[^,],%[^,],%[^,]", temp.sender_id,temp.rec_id, temp.sent,temp.flag);

                if (strcmp(temp.sender,emplid)==0 || strcmp(temp.rec_id,emplid)==0) {
                        printf("Employee ID matched.\n");

		char temp1[256];
               // snprintf(temp1, sizeof(temp1), "CID: %s,amount:%s,transaction performed:%s\n",temp.sender,temp.amount,temp.flag);
		
		 strcat(transaction_buffer, temp1);
		continue;            
		

                }

         
            }}
	    if(transaction_buffer!=NULL){
	    write(cd,transaction_buffer,strlen(transaction_buffer));
	   return true;

        	}
	    else
		write(cd,"No Loan Applications Assigned\n",strlen("No Loan Applications Assigned\n"));
		return false;
*/	
return true;
}
