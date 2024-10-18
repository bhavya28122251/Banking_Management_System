#include "header_file.h"

bool change_admin_pass(int cd){
	char new_pass[10];
send(clientSocket,"enter new password\n",strlen("enter new password\n"),0);
ssize_t rbytes=recv(clientSocket,new_pass,sizeof(new_pass),0);
if(rbytes<=0)
{send(clientSocket,"error in receiving password\n",strlen("error in receiving password\n"),0);
return false;}
if (new_pass[rbytes -1]=='\n')
{new_pass[rbytes -1]='\0';}
else
{new_pass[rbytes -1]='\0';}
save_credentials("Shatakshee",new_pass);
return true;
}

void save_credentials(const char *username, const char *password) {
FILE *file = fopen("adminlogin.txt", "w");
if (file == NULL) {
perror("Error opening file");
exit(EXIT_FAILURE);
}

    fprintf(file, "%s %s\n", username, password);
    fclose(file);
}
