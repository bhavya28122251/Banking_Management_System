#include"header_file.h"

bool admin_options(int cd){
	
	ssize_t bytes_read;

	while(1){
		char admin_menu[]="\nChoose from the given options:\n1.Add New Bank Emplyee\n2.Modify Customer Details\n3.Display Customer Deatils\n4.Modify Employee Details\n5.Display Employee Details\n6.Manage User Roles\n7.Change Password\n8.Logout\n9.Exit";
		write(cd,admin_menu,sizeof(admin_menu));
		
		char ch_str[20];
        	 bytes_read = read(cd, ch_str, sizeof(ch_str)-1);  
       		 if (bytes_read > 0) {
                	ch_str[bytes_read] = '\0'; 
                	for (int i = 0; i < bytes_read; i++) {
                        	if (ch_str[i] == '\n' || ch_str[i] == '\r') {
                                	ch_str[i] = '\0';
                                	break;
                        	}
                	}
		} 
                printf("1Choice is %s\n", ch_str);
		int ch=atoi(ch_str);
		switch(ch){
			case 1:
				if(add_employee(cd)){
					write(cd,"New Bank Employee added Successfully\n",strlen("New Bank Employee added Successfully\n"));
        			}
				break;
			case 2:
				if(1){
					write(cd,"Customer Details Modified\n",strlen("Customer Details Modified\n"));
				}
				break;
			 case 3:
                                if(1){
                                        write(cd,"Customer Datails displayed Successfully\n",strlen("Customer Datails displayed Successfully\n"));
                                }
                                break;

			case 4:
                                if(modify_employee(cd)){
                                        write(cd,"Employee Details Modified\n",strlen("Employee Details Modified\n"));
                                }
                                break;
			 case 5:
                                if(display_employee_data(cd)){
                                        write(cd,"Employee Details displayed Successfully\n",strlen("Employee Details displayed Successfully\n"));
                                }
                                break;

			case 6:
                                if(manage_user_roles(cd)){
                                        write(cd,"User Roles Managed\n",strlen("User Roles Managed\n"));
                                }
                                break;
			case 7:
                                if(change_admin_pass(cd)){
                                        write(cd,"Password has Changed\n",strlen("Password has Changed\n"));
                                }
                                break;
			case 8:
                                write(cd,"Logged Out\n",strlen("Logged Out\n"));
                                return true;
                                break;
			case 9:
                                write(cd,"Exit",strlen("Exit"));
                               	close(cd);
                                break;
			default:
				write(cd,"Wrong Input\nPlease enter a number from 1-9\n",strlen("Wrong Input\nPlease enter a number from 1-7\n"));
				break;
		}
	}

}

bool employee_options(int cd){
	  ssize_t bytes_read;

        while(1){
                char menu[]="\nChoose from the given options:\n1.Add New Customer\n2.Modify Customer Details\n3.Process Loan Applications\n4.Approve/Reject Loans\n5.View Assigned Loan Apllications\n6.View Customer Transactions\n7.Change Password\n8.Logout\n9.Exit";
                write(cd,menu,sizeof(menu));

                char ch_str[20];
                 bytes_read = read(cd, ch_str, sizeof(ch_str)-1);  
                 if (bytes_read > 0) {
                        ch_str[bytes_read] = '\0'; 
                        for (int i = 0; i < bytes_read; i++) {
                                if (ch_str[i] == '\n' || ch_str[i] == '\r') {
                                        ch_str[i] = '\0';
                                        break;
                                }
                        }
                } 
                printf("Choice is %s\n", ch_str);
                int ch=atoi(ch_str);
                switch(ch){
                        case 1:
                                if(add_new_customer(cd)){
                                        write(cd,"New Customer added Successfully\n",strlen("New Customer added Successfully\n"));
                                }
                                break;
                        case 2:
                                if(1){
                                        write(cd,"Customer Details Modified\n",strlen("Customer Details Modified\n"));
                                }
                                break;
                         case 3:
                                if(1){
                                        write(cd,"Loan Applications Processed Successfully\n",strlen("Loan Applications Processed Successfully\n"));
                                }
                                break;

                        case 4:
                                if(1){
                                        write(cd,"Loan Approved/Rejected Successfully\n",strlen("Loan Approved/Rejected Successfully\n"));
                                }
                                break;
                         case 5:
                                if(1){
                                        write(cd,"Assigned Loan Applications displayed Successfully\n",strlen("Assigned Loan Applications displayed Successfully\n"));
                                }
                                break;

                        case 6:
                                if(1){
                                        write(cd,"Customer Transactions Displayed Successfully\n",strlen("Customer Transactions Displayed Successfully\n"));
                                }
                                break;
                        case 7:
                                if(change_admin_pass(cd)){
                                        write(cd,"Password has Changed\n",strlen("Password has Changed\n"));
                                }
                                break;
                        case 8:
                                write(cd,"Logged Out\n",strlen("Logged Out\n"));
                                return true;
                                break;
                        case 9:
                                write(cd,"Exit",strlen("Exit"));
                                close(cd);
                                break;
                        default:
                                write(cd,"Wrong Input\nPlease enter a number from 1-9\n",strlen("Wrong Input\nPlease enter a number from 1-7\n"));
                                break;
                }
        }

}

