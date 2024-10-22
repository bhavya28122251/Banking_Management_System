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
			
			char buff[256];
			int len = sprintf(buff, "Balance: %s\n", temp.balance); 
			write(cd, buff, len);

                	lock.l_type = F_UNLCK;
                	if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                        	perror("Error in releasing the lock");
                        	close(db_fd);
                        	return false;
                	}
			printf("view balance\n");
                	return true;

            }
        }}
        write(cd,"Customer not Found",strlen("Customer not Found"));
        return false;

}


bool deposit_money(int cd){
	
	struct Transaction add;
	 struct Customer temp;
	 struct Customer data_new;

	write(cd,"Enter ID:",strlen("Enter ID:"));
	ssize_t read1=read(cd,add.sender_id,sizeof(add.sender_id));
	add.sender_id[read1]='\0';
        if(add.sender_id[read1-1]=='\n'){
               add.sender_id[read1-1]='\0';
        }

	

	write(cd,"Enter Amount to Deposit:",strlen("Enter Amount to Deposit:"));
	ssize_t read2=read(cd,add.sent,sizeof(add.sent));
	int amount=atoi(add.sent);
	printf("%d",amount);
	if(read2<=0){
		send(cd,"Error in Amount\n",strlen("Error in Amount\n"),0);
		return false;
	}
	 add.sent[read2]='\0';
        if(add.sent[read2-1]=='\n'){
               add.sent[read2-1]='\0';
        }



	strcpy(add.rec_id,add.sender_id);
	strcpy(add.flag,"0");

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

        char format[300];
        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the customer file");
                return false;
        }

        char line[300];
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
		
		printf("%s",add.sender_id);
                if ((strcmp(temp.id,add.sender_id))==0) {
                        printf("customer is matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		int new_bal=amount+atoi(temp.balance);
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
                write(cd, "Failure", strlen("Failure"));
                return false;
        }

        return true;

}

bool withdraw_money(int cd){
	 struct Transaction add;
         struct Customer temp;
         struct Customer data_new;

        write(cd,"Enter ID:",strlen("Enter ID:"));
        ssize_t read1=read(cd,add.sender_id,sizeof(add.sender_id));
        add.sender_id[read1]='\0';
        if(add.sender_id[read1-1]=='\n'){
               add.sender_id[read1-1]='\0';
        }

        write(cd,"Enter Amount to Withdraw:",strlen("Enter Amount to Withdraw:"));
        ssize_t read2=read(cd,add.sent,sizeof(add.sent));
        int amount=atoi(add.sent);
        printf("%d",amount);
        if(read2<=0){
                send(cd,"Error in Amount\n",strlen("Error in Amount\n"),0);
                return false;
        }
         add.sent[read2]='\0';
        if(add.sent[read2-1]=='\n'){
               add.sent[read2-1]='\0';
        }



        strcpy(add.rec_id,add.sender_id);
        strcpy(add.flag,"1");

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

        char format[300];

        int db_fd = open("customer_db.txt", O_RDWR);
        if (db_fd == -1) {
                perror("Error in opening the customer file");
                return false;
        }

        char line[300];
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

                printf("%s",add.sender_id);
                if ((strcmp(temp.id,add.sender_id))==0) {
                        printf("customer is matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
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
                write(cd, "Failure", strlen("Failure"));
                return false;
        }

        return true;

}





bool transfer_funds(int cd)
{
	struct Transaction add;
	write(cd,"Enter Sender ID:",strlen("Enter Sender ID:"));
	ssize_t read1=read(cd,add.sender_id,sizeof(add.sender_id));
	add.sender_id[read1]='\0';
        if(add.sender_id[read1-1]=='\n'){
               add.sender_id[read1-1]='\0';
        }


	write(cd,"Enter Receiver ID:",strlen("Enter Receiver ID:"));
	ssize_t read2=read(cd,add.rec_id,sizeof(add.rec_id));
	add.rec_id[read2]='\0';
        if(add.rec_id[read2-1]=='\n'){
               add.rec_id[read2-1]='\0';
        }
	
	write(cd,"Enter Amount to be Sent:",strlen("Enter Amount to be Sent:"));
	ssize_t read3=read(cd,add.sent,sizeof(add.sent));
	add.sent[read3]='\0';
        if(add.sent[read3-1]=='\n'){
               add.sent[read3-1]='\0';
        }

	int amount=atoi(add.sent);
	printf("Amount trnasferred  %d\n",amount);

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

                if((strcmp(temp.id,add.sender_id))==0) {
                        printf("Customer is matched.\n");

                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLKW, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		int new_balance=atoi(temp.balance)-amount;
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

        struct Customer data_new1;
        char format1[300];
	
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

                if ((strcmp(temp1.id,add.rec_id))==0) {
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
        return true;


}


bool view_transaction_history(int cd){
	struct Transaction data_new;
        char custid[10];
        char format[300];
        char enter_id[] = "-----View Transaction History-----\nEnter Your ID:";

        write(cd, enter_id, sizeof(enter_id));
        ssize_t bytes_id = read(cd, custid, sizeof(custid));
        if (bytes_id == -1) {
                perror("Error in receiving Customer ID");
        
        return false;
        }
        custid[bytes_id] = '\0';  
        printf("Received Customer ID: %s\n", custid);

        int db_fd = open("transaction_db.txt", O_RDWR);printf("opened file\n");
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }
	char buffer;
        int line_index = 0;
	char transaction_buffer[500]="";
	char line[300];
        
	struct Transaction temp;
	off_t current_position=0;		

	while (read(db_fd, &buffer, 1) > 0) {printf("inside while\n");
	   if (buffer != '\n') {
			printf("if");
        	   line[line_index++] = buffer;
        }

         else {
		printf("else");
                line[line_index] = '\0';
                line_index = 0;

                current_position = lseek(db_fd, 0, SEEK_CUR);
		printf("before taking data in temp");
                sscanf(line, "%[^,],%[^,],%[^,],%[^,]", temp.sender_id,temp.rec_id, temp.sent,temp.flag);
		printf("after taking data in temp");
		char temp1[256]="";
                if((strcmp(temp.sender_id,custid)==0) && (strcmp(temp.rec_id,custid)==0)) {
			if(strcmp(temp.flag,"0")==0){
				
				snprintf(temp1,sizeof(temp1),"Deposited %s\n",temp.sent);
			}
			else{
				snprintf(temp1,sizeof(temp1),"Withdrawn %s\n",temp.sent);
			}
		}
		else if(strcmp(temp.sender_id,custid)==0){
			snprintf(temp1,sizeof(temp1),"Sent %s to %s\n",temp.sent,temp.rec_id);
		}						
		else if(strcmp(temp.rec_id,custid)==0){
			snprintf(temp1,sizeof(temp1),"Received %s from %s\n",temp.sent,temp.sender_id);
		}
		printf("Temp1 content: %s\n", temp1);
        	printf("Transaction buffer content so far:\n%s\n", transaction_buffer);		
		strcat(transaction_buffer, temp1);
		continue;            
		


         
          }  }
		printf("%d\n",strlen(transaction_buffer));
	    if(strlen(transaction_buffer)>0){
	    write(cd,transaction_buffer,strlen(transaction_buffer));
	   return true;

        	}
	    else{
		write(cd,"No Transcation\n",strlen("No Transaction\n"));
		return false;
		}
	
return true;
}
