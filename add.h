#include "header_file.h"
bool add_employee(int cd){
        struct Employee empl;

        char buf_name[]="-----Adding New Employee-----\nName:";
        write(cd,buf_name,sizeof(buf_name));

        int bytes_name=read(cd,empl.name,sizeof(empl.name));
        if(bytes_name<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        empl.name[bytes_name]='\0';
        if(empl.name[bytes_name-1]=='\n'){
                empl.name[bytes_name-1]='\0';
        }

        char buf_pass[]="\nPassword:";
        write(cd,buf_pass,sizeof(buf_pass));
        int bytes_pass=read(cd,empl.pass,sizeof(empl.pass));
        if(bytes_pass<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        empl.pass[bytes_pass]='\0';
        if(empl.pass[bytes_pass-1]=='\n'){
                empl.pass[bytes_pass-1]='\0';
        }

        char buf_id[]="\nID:";
        write(cd,buf_id,sizeof(buf_id));
        int bytes_id=read(cd,empl.id,sizeof(empl.id));
        if(bytes_id<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receivin dta from client\n"));
                return false;
        }
        empl.id[bytes_id]='\0';
        if(empl.id[bytes_id-1]=='\n'){
                empl.id[bytes_id-1]='\0';
        }

        empl.is_empl=true;

        /*const char * dbFile="employee_db.txt";
        int db_fd=open(dbFile,O_RDWR|O_CREAT,0644);
        if(db_fd==-1){
                perror("Error in opening the database file");
                return false;
        }

        off_t last=lseek(db_fd,0,SEEK_END);

        if(flock(db_fd,LOCK_EX)==-1){
                perror("Error in locking the file");
                close(db_fd);
                return false;
        }

        ssize_t write_size=pwrite(db_fd,&empl,sizeof(struct Employee),last);
        if(write_size==-1 || write_size<sizeof(struct Employee)){
                perror("Error in writing record to database");
                close(db_fd);
                return false;
        }

        fprintf("%s%s%s",empl.name,empl.pass,empl.id);
        if(flock(db_fd,LOCK_UN)==-1){
                perror("Error in unlocking");
                close(db_fd);
                return false;
        }

        close(db_fd);

return true;*/
	FILE *file=fopen("employee.txt","a");
	if(file!=NULL){
		fprintf(file,"%s,%s,%s,%d\n",empl.id,empl.name,empl.pass,empl.is_empl);
		fclose(file);
		printf("true\n");
		fflush(stdout);
		return true;
	}
	perror("Error in opening employee file");
	printf("false\n");
	fflush(stdout);
	return false;
}
