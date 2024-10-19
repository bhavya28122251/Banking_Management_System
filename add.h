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

bool add_customer(int cd){
	struct Customer cust;

        char buf_name[]="-----Adding New Customer-----\nName:";
        write(cd,buf_name,sizeof(buf_name));

        int bytes_name=read(cd,cust.name,sizeof(cust.name));
        if(bytes_name<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        cust.name[bytes_name]='\0';
        if(cust.name[bytes_name-1]=='\n'){
                cust.name[bytes_name-1]='\0';
        }

        char buf_pass[]="Password:";
        write(cd,buf_pass,sizeof(buf_pass));
        int bytes_pass=read(cd,cust.pass,sizeof(cust.pass));
        if(bytes_pass<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        cust.pass[bytes_pass]='\0';
        if(cust.pass[bytes_pass-1]=='\n'){
                cust.pass[bytes_pass-1]='\0';
        }

        char buf_id[]="\nID:";
        write(cd,buf_id,sizeof(buf_id));
        int bytes_id=read(cd,cust.id,sizeof(cust.id));
        if(bytes_id<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receivin dta from client\n"));
                return false;
        }
        cust.id[bytes_id]='\0';
        if(cust.id[bytes_id-1]=='\n'){
                cust.id[bytes_id-1]='\0';
        }

	char buf_balance[]="\nInitial Balance: ";
        write(cd,buf_balance,sizeof(buf_balance));
        int bytes_balance=read(cd,cust.balance,sizeof(cust.balance));
        if(bytes_balance<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receivin dta from client\n"));
                return false;
        }
        cust.balance[bytes_balance]='\0';
        if(cust.balance[bytes_balance-1]=='\n'){
                cust.balance[bytes_balance-1]='\0';
        }

        cust.active=true;

        FILE *file=fopen("customer_db.txt","a");
        if(file!=NULL){
                fprintf(file,"%s,%s,%s,%s,%d\n",cust.id,cust.name,cust.pass,cust.balance,cust.active);
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
