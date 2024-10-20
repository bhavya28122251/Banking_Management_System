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


                lock.l_start = current_position - strlen(line) - 1;  
                lock.l_len = strlen(line) + 1; 

                if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                         perror("Error in obtaining lock");
                        close(db_fd);
                        return false;
                }

		write(cd,temp.balance,sizeof(temp.balance));
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
