#ifndef HEADER_FILE  
#define HEADER_FILE

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<sys/file.h>


bool admin_login(int cd);
bool employee_login(int cd);
//bool manager_login(int cd);
//bool customer_login(int cd);
bool admin_options(int cd);
bool employee_options(int cd);
bool manager_options(int cd);
bool customer_options(int cd);
bool add_employee(int cd);
bool modify_employee(int cd);
bool display_employee_data(int cd);
bool change_admin_pass(int cd);
bool manage_user_roles(int cd);
//int my_strcmp(const char *s1, const char *s2);
struct Employee{
	char name[100];
	char pass[100];
	char id[10];
	bool is_empl;
};

#define PORT 5055
#endif
