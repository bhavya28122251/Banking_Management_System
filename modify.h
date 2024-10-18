#include "header_file.h"

bool modify_employee(int cd){
        struct Employee data_new;
        char empid[10];

        char format[300];

        char enter_id[]="-----Update Emplopyee Data-----\nEnter ID of the employee whose data needs to be changed:";
        write(cd,enter_id,sizeof(enter_id));

        ssize_t bytes_id=read(cd,empid,sizeof(empid));
        if(bytes_id==-1){
                perror("Error in receving Employee ID");
                return false;
        }
        empid[bytes_id] = '\0';
        printf("%s",empid);

        write(cd,"Name:",strlen("Name:"));
        read(cd,data_new.name,sizeof(data_new.name));

        write(cd,"ID:",strlen("ID:"));
        read(cd,data_new.pass,sizeof(data_new.pass));

        // write(cd,"Na:",strlen("Name:"));
        //read(cd,data_new.name,sizeof(data_new.name));

        int db_fd=open("employee.txt",O_RDWR);
        if(db_fd==-1){
                perror("Error in opening the database file");
                return false;
        }

        struct Employee temp;

        bool is_there=false;

        struct flock lock;
        memset(&lock,0,sizeof(lock));
        lock.l_type=F_WRLCK;
        lock.l_whence=SEEK_SET;

        while(read(db_fd,&temp,sizeof(struct Employee))>0){char line[300];
                sscanf(line, "%[^,],%[^,],%[^,],%d", temp.id, temp.name, temp.pass, &temp.is_empl);
                printf("Read Employee: ID=%s, Name=%s, Password=%s, Is Employed=%d\n",temp.id, temp.name, temp.pass, temp.is_empl);
                if(my_strcmp(temp.id,empid)==0){
                        printf("ID matched");
                        lock.l_start=lseek(db_fd,0,SEEK_CUR)-sizeof(struct Employee);
                        lock.l_len=sizeof(struct Employee);

                        if(fcntl(db_fd,F_SETLK,&lock)==-1){
                                perror("Error in obtaining lock");
                                close(db_fd);
                                return false;
                        }

                        lseek(db_fd,-sizeof(struct Employee),SEEK_CUR);

                        strcpy(data_new.pass,temp.pass);
                        temp.is_empl=data_new.is_empl;

                        snprintf(format,300,"%s,%s,%s,%d\n",data_new.id,data_new.name,data_new.pass,data_new.is_empl);
                         size_t format_len = 0;
                        while (format[format_len] != '\0') 
                                format_len++;


                        if((write(db_fd,format,sizeof(format_len)))==-1){
                                write(cd,"Error in Updating Data",strlen("Error in Updating Data")); 
                                close(db_fd);
                                return false;
                        }

                        lock.l_type=F_UNLCK;
                        if(fcntl(db_fd,F_SETLK,&lock)==-1){
                                perror("Error in releasing the lock");
                                close(db_fd);
                                return false;
                        }
                        is_there=true;
                        break;
                }
        }
        close(db_fd);
        if(!is_there){  
                write(cd,"Employee Not Found",strlen("Employee Not Found"));
                return false;
        }
        return true;

}

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
