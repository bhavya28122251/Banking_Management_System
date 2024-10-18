#include "header_file.h"

bool display_employee_data(int cd){
	struct Employee temp;
    	char empid[10];

    	char enter_id[] = "-----Display Employee Data-----\nEnter ID of the employee to display:\n";
    	write(cd, enter_id, sizeof(enter_id));

    	ssize_t bytes_id = read(cd, empid, sizeof(empid));
    	if (bytes_id == -1) {
        	perror("Error in receiving Employee ID");
        	return false;
    	}

    	int db_fd = open("employee_db.txt", O_RDONLY);
    	if (db_fd == -1) {
        	perror("Error in opening the database file");
        	return false;
    	}

    	struct flock lock;
    	memset(&lock, 0, sizeof(lock));
    	lock.l_type = F_RDLCK; 
    	lock.l_whence = SEEK_SET;

    	bool is_found = false;

    	while (read(db_fd, &temp, sizeof(struct Employee)) > 0) {
        	if (strcmp(temp.id, empid) == 0) {
            	lock.l_start = lseek(db_fd, 0, SEEK_CUR) - sizeof(struct Employee);
            	lock.l_len = sizeof(struct Employee);

            	if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                	perror("Error in obtaining read lock");
                	close(db_fd);
                	return false;
            	}

            	char display_msg[256];
            	snprintf(display_msg, sizeof(display_msg), "Employee Data:\nName: %s\nID: %s\n", temp.name, temp.pass);
            	write(cd, display_msg, strlen(display_msg));

            	lock.l_type = F_UNLCK;
            	if (fcntl(db_fd, F_SETLK, &lock) == -1) {
                	perror("Error in releasing the read lock");
                	close(db_fd);
                	return false;
            	}

            	is_found = true;
            	break;
        	}
    	}

    	close(db_fd);

    	if (!is_found) {
        	write(cd, "Employee not found.\n", strlen("Employee not found.\n"));
        	return false;
    	}

    return true;
}

