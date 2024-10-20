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
bool manager_login(int cd);
bool customer_login(int cd);

bool admin_options(int cd);
bool employee_options(int cd);
bool manager_options(int cd);
bool customer_options(int cd);

bool add_employee(int cd);
bool add_customer(int cd);
bool add_manager(int cd);

bool modify_employee(int cd);
bool modify_customer(int cd);

bool display_employee_data(int cd);
bool change_admin_pass(int cd);
bool change_employee_pass(int cd);
bool change_customer_pass(int cd);
bool change_manager_pass(int cd);
bool manage_user_roles(int cd);

bool view_balance(int cd);

bool apply_loan(int cd);
bool assign_loan(int cd);
bool view_assigned_loan_appl(int cd);
bool approve_reject_loan(int cd);

//int my_strcmp(const char *s1, const char *s2);
struct Employee{
	char name[100];
	char pass[100];
	char id[10];
	bool is_empl;
};

struct Customer{
	char id[10];
	char name[100];
	char pass[100];
	bool active;
	char balance[10];
};

struct Transaction{
	char sender_id[10];
	char receiver_id[10];
	char money_sent[10];
};

struct Loan{
	char cust_id[10];
	char empl_id[10];
	int status;
};

struct Feedback{
	char cust_id[10];
	char feedback[1000];
};


#define PORT 5055
#endif
